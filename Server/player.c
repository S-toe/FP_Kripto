#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <ctype.h>
#include <termios.h>
#include <pthread.h>
#include "../RSA/rsa.h"
#define PORT 8090

// static struct termios stored;
int sock = 0, valread;
int status = 0;
pthread_t thread;
FILE *fp, *fp2, *fp3;
#define MAX_STR_LEN 256
#define MAX_BOOKS 256

static struct termios stored_settings;

struct player{
    char healt[1024];
    char attack[1024];
};

struct player player;

void set_keypress(void)
{
    struct termios new_settings;
    tcgetattr(0,&stored_settings);
    new_settings = stored_settings;
    new_settings.c_lflag &= (~ICANON);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;
    tcsetattr(0,TCSANOW,&new_settings);
    return;
}

void reset_keypress(void)
{
    tcsetattr(0,TCSANOW,&stored_settings);
    return;
}

//compare input
int stringcmp (char *s1,char *s2)
{
    int i=0,diff=0;
    int pjg=strlen(s2);
    int pjg2=strlen(s1);
    for(i=0; s1[i]!='\0'; i++)
    {
        if( toupper(s1[i])!=toupper(s2[i]) || pjg!=pjg2 )
            return 1;           
    }
    return 0;
}

//check player win or not
void *cekhasil( void *ptr )
{
    char buffer[1024];
    while(1) {
        valread = read(sock, buffer, 1024);
        printf("%s\n",buffer);
        if(!strcmp(buffer, "win")) {
            system("clear");
            printf("Game berakhir kamu menang\n");
            status = 2;
            break;
        }

        if(!strcmp(buffer, "dead")) {
            system("clear");
            printf("Game berakhir kamu kalah\n");
            send(sock , "dead" , strlen("dead"), 0 );
            status = -1;
            break;
        }

        memset(buffer, 0, sizeof(buffer));
    }

    status = 0;
    return NULL;
}

//read asset player health and attack
void readasset()
{
    /* FileStream for the Library File */
    FILE *asset;

    /* allocation of the buffer for every line in the File */
    char *buf = malloc(MAX_STR_LEN);
    char *tmp; 

    /* if the space could not be allocaed, return an error */
    if (buf == NULL) {
        printf ("No memory\n");
    }

    if ( ( asset = fopen( "asset.csv", "r" ) ) == NULL ) //Reading a file
    {
        printf( "File could not be opened.\n" );
    }

    int i = 0;
    while (fgets(buf, 255, asset) != NULL)
    {
        if ((strlen(buf)>0) && (buf[strlen (buf) - 1] == '\n'))
            buf[strlen (buf) - 1] = '\0';       

        
        tmp = strtok(buf, ",");
        sprintf(player.healt, "%d", atoi(tmp));

        tmp = strtok(NULL, ",");
        sprintf(player.attack, "%d", atoi(tmp));

        if (i==0){
            i++;
            continue;
        }
        printf("player.health= %s  player.attack: %s\n",player.healt,player.attack);

    }
    //free(buf);
    fclose(asset);
}

int main(int argc, char const *argv[]) {
    struct sockaddr_in address;
    struct sockaddr_in serv_addr;
    char hello[1025];
    // memset(hello, ‘ ‘, strlen(hello));
    char *login = "login";
    char *regist = "register";
    char kata[1024],usrname[1024],passwd[1024];
    char *cek = "cek";
    char temp;
    char buffer[1024] = {0};
    char health[1024];
    char attack[1024];

    //read asset and store to variable
    readasset();
    *health=*player.healt;
    *attack=*player.attack;

    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
      
    if(inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr)<=0) {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
  
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        printf("\nConnection Failed \n");
        return -1;
    }

    while(1){
        //player choice menu
        printf("1.Login\n2.Register\nChoice : ");
        scanf("%s",&*kata);
        if(!stringcmp(kata,login)){
            
            send(sock , login , strlen(login) , 0 );
            printf("Username : ");
            scanf("%c", &temp);
            scanf("%[^\n]", &*usrname); 
            send(sock , usrname , 1024 , 0 );
            printf("Password : ");
            scanf("%c", &temp);
            scanf("%[^\n]", &*passwd);
            send(sock , passwd , 1024 , 0 );
            memset(buffer, 0, sizeof buffer);
            valread = read( sock , buffer, 1024);
            printf("%s\n",buffer );
            if(!stringcmp(buffer,"login success")){
                send(sock , player.healt , strlen(player.healt) , 0 );
                sleep(1);
                send(sock , player.attack , strlen(player.attack) , 0 );
                printf("waiting for player %s\n",player.healt);

                //choice after login
                while (1){
                    printf("1.Find Match\n2.Logout\nChoice : ");
                    scanf("%s",&*kata);
                    if(!stringcmp(kata,"logout")){
                        send(sock , kata , 1024 , 0 );
                        break;
                    }
                    //player search opponent
                    else if(!stringcmp(kata,"find")){
                        send(sock , kata , 1024 , 0 );

                        //loop until find opponent
                        while(!stringcmp(kata,"find")){
                        read(sock,kata,1024);
                        printf("waiting for player\n");

                        }

                        //the game
                        char msg[1024];
                        read(sock, msg ,1024 );
                        sleep(2);
                        system("clear");
                        printf("%s\n",msg);
                        sleep(2);
                        printf("Game dimulai silahkan tap tap -space- secepat mungkin dalam\n");
                        printf("3\n");
                        sleep(1);
                        printf("2\n");
                        sleep(1);
                        printf("1\n");

                        sleep(1);

                        status = 1;

                        int iret = pthread_create(&thread,NULL,&cekhasil,NULL);

                        if(iret)
                        {
                            fprintf(stderr,"Error - pthread_create() return code: %d\n",iret);
                            exit(EXIT_FAILURE);
                        }

                        set_keypress();

                        while (status == 1)
                        {
                            if(status != 1) break;
                            // printf("status:%d\n",status);
                            char c = getc(stdin);
                            if(status == 2) break;
                            if(c == 32) {

                                // send hit to server
                                if(status != 2){
                                    send(sock , "hit" , strlen("hit"), 0 );
                                    printf("hit !!\n");
                                }
                            }
                        }
                        status = 0;

                        reset_keypress();


                    }
                }
            
            }
            
            memset(buffer, 0, sizeof buffer);
        }

        if(!stringcmp(kata,regist)){
            // send(sock , regist , strlen(regist) , 0 );
            printf("sorry, no service :(...\n");
            // printf("register\n");
            // printf("Username : ");
            // // scanf("%s",&*usrname);
            // scanf("%c", &temp);
            // scanf("%[^\n]", &*usrname); 
            // // fgets(usrname,1024,stdin);
            // send(sock , usrname , 1024 , 0 );
            // printf("Password : ");
            // // scanf("%s",&*passwd);
            // scanf("%c", &temp);
            // // fgets(passwd,1024,stdin);
            // scanf("%[^\n]", &*passwd); 
            // printf("register success\n");
            // send(sock , passwd , 1024 , 0 );
            // // valread = read( sock , buffer, 1024);
            // // printf("%s\n",buffer );
            // // memset(buffer, 0, sizeof buffer);
            // // memset(usrname, 0, sizeof buffer);
            // // memset(passwd, 0, sizeof buffer);
        }
    }
    return 0;
}
