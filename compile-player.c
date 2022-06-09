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
#define PORT 8080

// static struct termios stored_settings;

struct player
       {
              char health;
       };



int main(int argc, char const *argv[]) {
    FILE *fp, *fp2, *fp3;
    struct player player1;
    fp2 = fopen("assets","w");
    player1.health = 70;
    fwrite(&player1,sizeof(player1),1,fp2);
    fclose(fp2);
    fp = fopen("assets","r");
    while(fread(&player1, sizeof(struct player), 1, fp)) 
    printf ("health = %d\n", player1.health); 
        // printf("masok");
    fclose(fp);
    // send(new_socket , hello , strlen(hello) , 0 );
    // printf("Hello message sent2\n");
    return 0;
}
