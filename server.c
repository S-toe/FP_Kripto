#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>
#include <ctype.h>
#include<pthread.h>
#define PORT 8080

pthread_t tid[3000];
int find;
int darah1,darah2,darahtmp;
int att1,att2,atttmp;

struct Akun
       {
              char name[50];
              char passwd[50];
       };

struct player
       {
              int id;
              int health;
              int attack;
       };

int stringcmp (char *s1,char *s2)
{
    int i=0,diff=0;
    for(i=0; s1[i]!='\0'; i++)
    {
        if( toupper(s1[i])!=toupper(s2[i]) )
            return 1;           
    }
    return 0;
}

void *client(void *tmp){
    FILE *fp, *fp2, *fp3;
    struct Akun akun,akun2;
    int valread;
    char buffer[1024] = {0};
    char hello[1024];
    char health1[1024] = {0};
    char health2[1024] = {0};
    char attack1[1024] = {0};
    char attack2[1024] = {0};
    char *regist = "register";
    char *login = "login";
    int new_socket = *(int *)tmp;
    int player = new_socket;

     while(1){
        
        valread = read( new_socket , buffer, 1024);
        
        //check for login
        if(!stringcmp(login,buffer)){
            valread = read( new_socket , akun.name, 1024);
            valread = read( new_socket , akun.passwd, 1024);
            fp3 = fopen("akun.txt","a+");
            int flag=0;

            while(fread(&akun2, sizeof(struct Akun), 1, fp3)){
                if(strcmp(akun.name,akun2.name)==0 && strcmp(akun.passwd,akun2.passwd)==0){
                char *buffer = "login success";
                send(new_socket , buffer , strlen(buffer) , 0 );
                flag=1;
                break;
                }
            }

            if (flag==0){
                printf("Auth Failed\n");

                char *buffer = "login failed";
                send(new_socket , buffer , strlen(buffer) , 0 );
            }
            //player already login
            else if(flag==1){
                printf("Auth success\n");
                //read healt and attack player
                valread = read( new_socket , health1, 1024);
                valread = read( new_socket , attack1, 1024);
                printf("Akun dengan client-id:%d username:%s health:%s attack:%s berhasil masuk\n",new_socket,akun.name,health1,attack1);

                while(1){ 
                    valread = read( new_socket , buffer, 1024);
                    if(!stringcmp("logout",buffer)){
                        break;
                    }
                    else if(!stringcmp("find",buffer)){
                        int opponent;
                        struct player player1,player2;

                        if(!find){
                            //player search opponent
                            find=player;
                            
                            //store healt from first player or player1
                            darah1=darahtmp=atoi(health1);
                            att1=atttmp=atoi(attack1);

                            //waiting for another player search opponent
                            while (find == player)
                            {
                                send(player,"find",4,0);
                                sleep(1);
                            }

                            //player1 store the health and attack and found opponent
                            send(player,"found",5,0);
                            darah2=darah1;
                            darah1=darahtmp;
                            att2=att1;
                            att1=atttmp;
                            opponent = find;
                            player1.id=player;
                            player2.id=opponent;
                            player1.health=darah1;
                            player2.health=darah2;
                            player1.attack=att1;
                            player2.attack=att2;
                            printf("darah2:%d 1:%d, %d dan %d\n",darah2,darah1,player,opponent); 
                            sleep(1); 
                               
                        }

                        //player2 will come here if search oppoenent and store health & attack
                        else{
                            send(player,"found",5,0);
                            darah2=darah1;
                            darah1=atoi(health1);
                            att2=att1;
                            att1=atoi(attack1);
                            opponent=find;
                            find=player;
                            // printf("darah1:%d darah1:%d %d dan %d\n",darah1,darah2, player, opponent); 
                            sleep(1.9);
                            find=0;
                            player1.id=player;
                            player2.id=opponent;
                            player1.health=darah2;
                            player2.health=darah1;
                            player1.attack=att2;
                            player2.attack=att1;
                        }

                        //store and send message for player
                        char msg[1024]= "kamu sudah dapat lawan dengan darah ";
                        char msg2[1024]= " dan attack ";
                        char hlt[1024];
                        char atc[1024];
                        sprintf(hlt,"%d",player2.health);
                        sprintf(atc,"%d",player2.attack);
                        strcat(msg,hlt);
                        strcat(msg2,atc);
                        strcat(msg,msg2);
                        send(player,msg,1024,0);

                        //the game
                        while(1){
                            //read the message hit from client
                            memset(buffer,0,1024);
                            valread = read( new_socket , buffer, 1024);

                            //if player send hit
                            if(!strcmp(buffer,"hit")){

                                    //enemy hp will decrease
                                    player2.health = player2.health - player1.attack;

                                    //send note to enemy
                                    char healt[1024];
                                    sprintf(healt,"%d",player2.health);
                                    char buffer[1024] = "kamu diserang, darahmu tersisa ";
                                    strcat(buffer,healt);
                                    send(opponent,buffer,1024,0);
                                
                                //if enemy death
                                if(player2.health <= 0) {
                                     send(player, "win", strlen("win"), 0);
                                     send(opponent, "dead", strlen("dead"), 0);
                                     break;
                                 }
                                //if the player death
                                if(player1.health <= 0) {
                                     send(player, "dead", strlen("dead"), 0);
                                     send(opponent, "win", strlen("win"), 0);
                                     break;
                                 }
                            }

                            if(!strcmp(buffer,"dead"))break;
                        }
                    }
                }
            }
              fclose(fp3);
        }
        // if(!stringcmp(regist,buffer)){
        //     fp2 = fopen("akun.txt","a");
        //     valread = read( new_socket , akun.name, 1024);
        //     valread = read( new_socket , akun.passwd, 1024);
        //     fwrite(&akun,sizeof(akun),1,fp2);
        //     fclose(fp2);
        //     fp = fopen("akun.txt","r");
        //     while(fread(&akun, sizeof(struct Akun), 1, fp)) 
        //     printf ("username = %s password = %s\n", akun.name, akun.passwd);
        //     fclose(fp);
        // }

    }

}

int main(int argc, char const *argv[]) {
    FILE *fp, *fp2, *fp3;
    struct Akun akun,akun2;
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    // int Count=0;
    char hello[1024];
    char *regist = "register";
    char *login = "login";
    find=0;
    darah2=0;
      
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }
      
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt))) {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
      
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0) {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    int total=0;
    //looping for new player come
    while(1){

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0) {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        
        pthread_create(&(tid[total]), NULL, &client, &new_socket);
        total++;
        // printf("%d\n",total);
    }
    return 0;
}
