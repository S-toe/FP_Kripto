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
    // printf("player:%d soc_addr:%d\n",player,new_socket);

     while(1){
        
        valread = read( new_socket , buffer, 1024);
        // printf("Hello message sent1\n");
        // printf("%s\n",buffer );
        if(!stringcmp(login,buffer)){
            // Count++;
            valread = read( new_socket , akun.name, 1024);
            // printf("1\n");
            valread = read( new_socket , akun.passwd, 1024);
            // printf("%s %s %s\n",buffer,akun.name,akun.passwd);
            fp3 = fopen("akun.txt","a+");
            // if(fp3 == NULL)
            //   {
            //          printf("\nCan't open file or file doesn't exist.");
            //          exit(0);
            //   }
            // printf("%s2\n",buffer );
            // // akun2.name = malloc(sizeof(akun.name));
            // if (akun2.name == NULL)
            // exit(EXIT_FAILURE);

            // akun2.passwd = malloc(sizeof(akun.passwd));
            // if (akun2.passwd == NULL)
            // exit(EXIT_FAILURE);
            int flag=0;
            while(fread(&akun2, sizeof(struct Akun), 1, fp3)){
                if(strcmp(akun.name,akun2.name)==0 && strcmp(akun.passwd,akun2.passwd)==0){
                // printf ("username = %s password = %s\n", akun2.name, akun2.passwd);
                // char *othing = "login failed";
                char *buffer = "login success";
                // printf("%s\n",buffer);
                send(new_socket , buffer , strlen(buffer) , 0 );
                // printf("%s\n",buffer);
                flag=1;
                break;
                }
                // printf("masok");
            }
            if (flag==0){
                printf("Auth Failed\n");

                char *buffer = "login failed";
                // char *lucky = "login success";
                send(new_socket , buffer , strlen(buffer) , 0 );
            }
            else if(flag==1){
                printf("Auth success\n");
                valread = read( new_socket , health1, 1024);
                valread = read( new_socket , attack1, 1024);
                printf("Akun dengan client-id:%d username:%s health:%s attack:%s berhasil masuk\n",new_socket,akun.name,health1,attack1); 
                // printf("h1:%s\n",health1); 
                // printf("a1:%s\n",attack1); 
                while(1){ 
                    // send(player,"healt success",13,0);
                    valread = read( new_socket , buffer, 1024);
                    // printf("masuk pilihan log\n");
                    if(!stringcmp("logout",buffer)){
                        break;
                    }
                    else if(!stringcmp("find",buffer)){
                        // printf("client %d mencari lawan..\n",new_socket);
                        int opponent;
                        struct player player1,player2;

                        if(!find){
                            // printf("find:%d\n",find);
                            find=player;
                            // valread = read( new_socket , health1, 1024);
                            // printf("h1:%d\n",*health1); 
                            
                            darah1=darahtmp=atoi(health1);
                            att1=atttmp=atoi(attack1);

                            // printf("darah1:%d %d\n",darah1,darah2); 
                            // else if (find!=player)
                            // {
                            //     printf("before h2:%d\n",*health2);
                            //     valread = read( new_socket , health2, 1024);
                            //     printf("h2:%d\n",*health2);    
                            // }                     

                            // printf("1 find:%d client:%d opponent:%d\n",find,player,opponent);

                            while (find == player)
                            {
                                //waiting for opponent;
                                // printf("nyarii2\n");
                                // printf("loop find:%d client:%d opponent:%d\n",find,player,opponent);
                                send(player,"find",4,0);
                                sleep(1);
                            }
                            // printf("nemu!");
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
                            // printf("2 find:%d client:%d opponent:%d\n",find,player,opponent);
                            // printf("h1:%d h2:%d\n",*health1-50,*health2-60);     
                               
                        }
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
                        //  printf("h1:%d h2:%d\n",player1.health,player2.health);
                            // valread = read( player , &player1.health, 1024);
                            // printf("h1:%d\n",player1.id); 
                            // valread = read( opponent , &player2.health, 1024);
                            // printf("h2:%d\n",player2.id); 
                            
                            // printf("3 find:%d client:%d opponent:%d\n",find,player,opponent);
                        }
                            // player1.health=player2.health=100;
                        //  printf("last find:%d client:%d opponent:%d\n",find,player,opponent);
                         
                            // if(player2.health==100)
                            //     send(opponent,"health 100",10,0);

                        //  printf("nextt h1:%d h2:%d %d\n",player1.health,player2.health,player);
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
                        while(1){
                            memset(buffer,0,1024);
                            // printf("player nih boss:%d\n",new_socket);
                            // printf("player id: %d %d\n",player1.id,player2.id);
                            // printf("%d %d\n",player1.health,player2.health);
                            valread = read( new_socket , buffer, 1024);
                            // printf("ini buffer:%s\n",buffer);
                            // printf("last-last new_socket:%d find:%d client:%d opponent:%d\n",new_socket,find,player,opponent);
                           

                            if(!strcmp(buffer,"hit")){
                                
                                // printf("masuk hit\n");
                                // printf("hitt\n");
                                // if(new_socket == player){
                                    
                                    // printf("dalam player\n");
                                    player2.health = player2.health - player1.attack;
                                    // printf("%d %d\n",player1.health,player2.health);
                                    char healt[1024];
                                    // printf("setelah deklarasi healt\n");
                                    sprintf(healt,"%d",player2.health);
                                    // printf("setelah naro isi healt\n");
                                    char buffer[1024] = "kamu diserang, darahmu tersisa ";
                                    // printf("setelah naro healt ke buffer\n");
                                    strcat(buffer,healt);
                                    // printf("setelah setelah gabung healt\n");
                                    send(opponent,buffer,1024,0);
                                
                                if(player2.health <= 0) {
                                    //  printf("masuk player win\n");
                                     send(player, "win", strlen("win"), 0);
                                    //  printf("masuk player win\n");
                                     send(opponent, "dead", strlen("dead"), 0);
                                     break;
                                 }

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
            
            // char hello1[1024] = "Penambahan Berhasil";
            // send(new_socket , hello1 , strlen(hello1) , 0 );
            // printf("Hello message sent1\n");
        }
        if(!stringcmp(regist,buffer)){
            // sprintf(hello,"%d",Count);
            fp2 = fopen("akun.txt","a");

        // fclose(fp);
            // printf("register\n" );
            valread = read( new_socket , akun.name, 1024);
            // sleep(1);
            valread = read( new_socket , akun.passwd, 1024);
            // printf("%s %s %s\n",buffer,akun.name,akun.passwd);
            fwrite(&akun,sizeof(akun),1,fp2);
            fclose(fp2);
            fp = fopen("akun.txt","r");
            while(fread(&akun, sizeof(struct Akun), 1, fp)) 
            printf ("username = %s password = %s\n", akun.name, akun.passwd); 
                // printf("masok");
            fclose(fp);
            // send(new_socket , hello , strlen(hello) , 0 );
            // printf("Hello message sent2\n");
        }
        // else  printf("nope" );

    }

}

int main(int argc, char const *argv[]) {
    FILE *fp, *fp2, *fp3;
    // char name[50];
    // char passwd[50];
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
    // char *othing = "login failed";
    // char *lucky = "login success";
    // char *cek = "cek";
      
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
   
//    fp = fopen("akun.txt","a");
//         if(fp == NULL){
//             // system("touch /home/stoe/PRAKTIKUM/MODUL3/soal2/akun.txt");
//         }
//         fclose(fp);

int total=0;
    while(1){
        
        // valread = read( new_socket , buffer, 1024);
        // // printf("Hello message sent1\n");
        // // printf("%s\n",buffer );
        // if(!stringcmp(login,buffer)){
        //     // Count++;
        //     valread = read( new_socket , akun.name, 1024);
        //     // printf("1\n");
        //     valread = read( new_socket , akun.passwd, 1024);
        //     // printf("%s %s %s\n",buffer,akun.name,akun.passwd);
        //     fp3 = fopen("akun.txt","r");
        //     // if(fp3 == NULL)
        //     //   {
        //     //          printf("\nCan't open file or file doesn't exist.");
        //     //          exit(0);
        //     //   }
        //     // printf("%s2\n",buffer );
        //     // // akun2.name = malloc(sizeof(akun.name));
        //     // if (akun2.name == NULL)
        //     // exit(EXIT_FAILURE);

        //     // akun2.passwd = malloc(sizeof(akun.passwd));
        //     // if (akun2.passwd == NULL)
        //     // exit(EXIT_FAILURE);
        //     int flag=0;
        //     while(fread(&akun2, sizeof(struct Akun), 1, fp3)){
        //         if(strcmp(akun.name,akun2.name)==0 && strcmp(akun.passwd,akun2.passwd)==0){
        //         // printf ("username = %s password = %s\n", akun2.name, akun2.passwd);
        //         // char *othing = "login failed";
        //         char *buffer = "login success";
        //         // printf("%s\n",buffer);
        //         send(new_socket , buffer , strlen(buffer) , 0 );
        //         // printf("%s\n",buffer);
        //         flag=1;
        //         }
        //         // printf("masok");
        //     }
        //     if (flag==0){
        //         printf("Auth Failed\n");

        //         char *buffer = "login failed";
        //         // char *lucky = "login success";
        //         send(new_socket , buffer , strlen(buffer) , 0 );
        //     }
        //     else {
        //         printf("Auth success\n");
                
        //     }
        //       fclose(fp3);
            
        //     // char hello1[1024] = "Penambahan Berhasil";
        //     // send(new_socket , hello1 , strlen(hello1) , 0 );
        //     // printf("Hello message sent1\n");
        // }
        // if(!stringcmp(regist,buffer)){
        //     // sprintf(hello,"%d",Count);
        //     fp2 = fopen("akun.txt","a");

        // // fclose(fp);
        //     // printf("register\n" );
        //     valread = read( new_socket , akun.name, 1024);
        //     // sleep(1);
        //     valread = read( new_socket , akun.passwd, 1024);
        //     // printf("%s %s %s\n",buffer,akun.name,akun.passwd);
        //     fwrite(&akun,sizeof(akun),1,fp2);
        //     fclose(fp2);
        //     fp = fopen("akun.txt","r");
        //     while(fread(&akun, sizeof(struct Akun), 1, fp)) 
        //     printf ("username = %s password = %s\n", akun.name, akun.passwd); 
        //         // printf("masok");
        //     fclose(fp);
        //     // send(new_socket , hello , strlen(hello) , 0 );
        //     // printf("Hello message sent2\n");
        // }
        // // else  printf("nope" );

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
