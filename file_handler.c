#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void handle_file(int fd, struct stat INFO){ //file descriptor and stat info

    printf("----MENU----\n->n: name\n->d: size\n->h: hard link count\n->m: last modification\n->a: acces rights\n->l: create symbolic link \n");
    printf("\nPlease enter your options\n");

    char commands[7]; commands[6]='\0'; //commands scanning
    scanf("-%6s", &commands);

    for(int i=0; /*commands[i]!=NULL && */commands[i]!='\0';i++){
        switch (commands[i]) {
            case "n":
            case "d":
            case "h":
            case "m":
            case "a":
            case "l":
            case default: printf("Please input accepted commands: n,d,h,m,a,l.");
        }       
    }
}

void handle_link(int fd, struct stat INFO){//file descriptor and stat info

    printf("----MENU----\n->n: name\n->l: delete\n->d: size of sl\n->t: size of target file\n->a: access rights\n");
    printf("\nPlease enter your options\n");

    char commands[6]; commands[5]='\0';
    scanf("-%5s", &commands);

    for(int i=0; /*commands[i]!=NULL && */commands[i]!='\0';i++){
        switch (commands[i]) {
            case 'n':
            case "l":
            case "d":
            case "t":
            case "a":
            case default: printf("Please input accepted commands: n,d,h,m,a,l.");
        }
    }
} 


int main(int argc, char* argv[]){

    if(argc<2) {
        printf("Provide file! Exiting...");
        exit(2);
    }

    int file1;
    if((file1=open(argv[1],O_RDONLY|O_CREAT, S_IRUSR|S_IRGRP|S_IROTH))!=0){
        printf("Could not open file. Exiting...");
        exit(2);
    };

    struct stat INFO;
    stat(argv[1], &INFO);

    if(S_ISREG(INFO.st_mode)) handle_file(file1, INFO);
    else if(S_ISLNK(INFO.st_mode))handle_link(file1, INFO);
    else printf("Please input regular file or symbolic link");

    close(file1);
    return 0;
}