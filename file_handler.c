#include <stdio.h>
#include <stdlib.h>



int main(int argc, char* argv[]){

    if(argc<2) {
        printf("Provide file! Exiting...");
        exit(2);
    }

    printf("----MENU----\n->n: name\n->d: size\n->h: hard link count\n->m: last modification\n->a: acces rights\n->l: create symbolic link \n");
    printf("\nPlease enter your options\n");

    char commands[7]; commands[6]='\0';
    scanf("-%6s", &commands);

    for(int i=0; commands[i]!=NULL && commands[i]!='\0';i++){
        printf("-%c", commands[i]);
    }

    //FILE* file=fopen(argv[1]);


    return 0;
}