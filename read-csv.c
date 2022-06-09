#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_STR_LEN 256
#define MAX_BOOKS 256

struct player{
    char healt;
    char attack;
};

struct player player;

/* PROTOTYPE OF FUNCTIONS */
int readasset();
// void printBookList();


int main(int argc, char **argv)
{   
    int isOK = 0;

    isOK = readasset();

    return 0;
}

int readasset()
{
    /* FileStream for the Library File */
    FILE *asset;

    /* allocation of the buffer for every line in the File */
    char *buf = malloc(MAX_STR_LEN);
    char *tmp; 

    /* if the space could not be allocaed, return an error */
    if (buf == NULL) {
        printf ("No memory\n");
        return 1;
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
        player.healt = atoi(tmp);

        tmp = strtok(NULL, ",");
        player.attack = atoi(tmp);

        if (i==0){
            i++;
            continue;
        }
        printf("player.health= %d  player.attack: %d\n",player.healt,player.attack);

    }
    //free(buf);
    fclose(asset);
    return 0;
}
