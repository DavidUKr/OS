#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

void print_access(char* file, struct stat INFO){

}

void create_symlink(char* file, struct stat INFO){

}

void print_time_last_modif(char* file, struct stat INFO){
    printf("Time of last modification %ld\n",INFO.st_mtime);
}

void print_link_size(char* link){

}

void handle_file(char* file, struct stat INFO){ //file descriptor and stat info

    printf("----MENU----\n->n: name\n->d: size\n->h: hard link count\n->m: last modification\n->a: acces rights\n->l: create symbolic link \n");
    printf("\nPlease enter your options\n");

    char commands[7]; commands[6]='\0'; //commands scanning
    scanf("-%6s", &commands);

    for(int i=0; commands[i]!='\0'; i++){
        switch (commands[i]) {
            case 'n':{printf("Name of file: %s\n", file);break;}
            case 'd':{printf("Total size of file(in bytes): %ld\n", INFO.st_size);break;}
            case 'h':{printf("Number of hard links: %d\n", INFO.st_nlink);break;}
            case 'm':{print_time_last_modif(file, INFO);break;}
            case 'a':{print_access(file, INFO);break;}
            case 'l':{create_symlink(file, INFO);break;}
            default: {printf("Please input accepted commands: n,d,h,m,a,l.\n");break;}
        }       
    }
}

void handle_link(char* symlink, struct stat INFO){//file descriptor and stat info

    printf("----MENU----\n->n: name\n->l: delete\n->d: size of sl\n->t: size of target file\n->a: access rights\n");
    printf("\nPlease enter your options\n");

    char commands[6]; commands[5]='\0';
    scanf("-%5s", &commands);

    for(int i=0; commands[i]!='\0'; i++){
        switch (commands[i]) {
            case 'n':{printf("Name of link: %s\n", symlink);break;}
            case 'l':{
                printf("deleting link\n");
                unlink(symlink);
                break;
                }
            case 'd':{print_link_size(symlink);break;}
            case 't':{printf("Size of target: %ld", INFO.st_size);break;}
            case 'a':{print_access(symlink, INFO);break;}
            default: {printf("Please input accepted commands: n,d,h,m,a,l.\n");break;}
        }
    }
} 


int main(int argc, char* argv[]){

    if(argc<2) {
        printf("Provide file or symbolic link! Exiting...");
        exit(2);
    }

    struct stat INFO;
    stat(argv[1], &INFO);

    if(S_ISREG(INFO.st_mode)) handle_file(argv[1], INFO);
    else if(S_ISLNK(INFO.st_mode))handle_link(argv[1], INFO);
    else printf("Please input regular file or symbolic link");

    return 0;
}