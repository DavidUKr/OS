#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>

void print_access(struct stat INFO){
    
    printf("Access rights:\n    User:\n");
    printf("Read-");
    if((INFO.st_mode & S_IRUSR)>0) printf("Yes\n");
    else printf("No\n");
    printf("Write-");
    if((INFO.st_mode & S_IWUSR)>0) printf("Yes\n");
    else printf("No\n");
    printf("Exec-");
    if((INFO.st_mode & S_IXUSR)>0) printf("Yes\n");
    else printf("No\n");
 
    printf("    Group:\n");
    printf("Read-");
    if((INFO.st_mode & S_IRGRP)>0) printf("Yes\n");
    else printf("No\n");
    printf("Write-");
    if((INFO.st_mode & S_IWGRP)>0) printf("Yes\n");
    else printf("No\n");
    printf("Exec-");
    if((INFO.st_mode & S_IXGRP)>0) printf("Yes\n");
    else printf("No\n");

    printf("    Others:\n");
    printf("Read-");
    if((INFO.st_mode & S_IROTH)>0) printf("Yes\n");
    else printf("No\n");
    printf("Write-");
    if((INFO.st_mode & S_IWOTH)>0) printf("Yes\n");
    else printf("No\n");
    printf("Exec-");
    if((INFO.st_mode & S_IXOTH)>0) printf("Yes\n");
    else printf("No\n");
    
}

void create_symlink(char* path, struct stat INFO){

    char* link_path;
    printf("\nSpecify link path/name:\n");
    scanf("%s", link_path);
    symlink(path, link_path);
}

void print_time_last_modif(struct stat INFO){

    printf("Time of last modification %s\n",ctime(&INFO.st_mtime));
}

void print_link_size(char* link){
    struct stat l_INFO;
    lstat(link, &l_INFO);
    printf("Link size (int bytes): %ld", l_INFO.st_size);
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
            case 'm':{print_time_last_modif(INFO);break;}
            case 'a':{print_access(INFO);break;}
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
            case 'a':{print_access(INFO);break;}
            default: {printf("Please input accepted commands: n,d,h,m,a,l.\n");break;}
        }
    }
} 

void handle_dir(char* dir, struct stat INFO){
    
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
    else if(S_ISDIR(INFO.st_mode))handle_dir(argv[1], INFO);
    else printf("Please input regular file or symbolic link");

    return 0;
}