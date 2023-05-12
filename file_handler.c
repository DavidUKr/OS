#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>

bool check_c_extenssion(char* name){
    int len=strlen(name);
    if(name[len-1]=='c' && name[len-2]=='.') return true;
    else return false;
}

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

    char link_path[100];
    printf("\nSpecify link path/name:\n");
    scanf("%99s", link_path);
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

int count_c_files(char* dir){
    int counter=0;

    DIR *directory=opendir(dir);
    struct dirent *entry;

    while((entry=readdir(directory))){
        if(check_c_extenssion(entry->d_name)) counter++;
    }
    
    return counter;
}

void print_dir_size(char* dir){
    //handling opening directory
    DIR *directory=opendir(dir);
    //handle entries
    struct dirent *entry;
    long size=0;

    while((entry=readdir(directory))){
        
        struct stat info;
        stat(entry, &info);

        if(S_ISDIR(info.st_mode)) print_dir_size(entry->d_name);
        else size+=info.st_size;
    }

    printf("Dir size: %ld bits = %ld MB\n", size, size/8/1024/1024);

}

void handle_file(char* file, struct stat INFO){ //file descriptor and stat info

    printf("REGULAR FILE\n");
    printf("----MENU----\n->n: name\n->d: size\n->h: hard link count\n->m: last modification\n->a: acces rights\n->l: create symbolic link \n");
    printf("\nPlease enter your options\n");

    char commands[7]; commands[6]='\0'; //commands scanning
    scanf("-%6s", commands);
    int pid[6];

    for(int i=0; commands[i]!='\0'; i++){
        if((pid[i]=fork())<0){
            printf("could not create child process");
            exit(2);
        }
        
        if(pid[i]==0){
            printf("process id: %d; ", getpid());
            printf("parent id: %d\n", getppid()); 
            switch (commands[i]) {
                case 'n':{printf("Name of file: %s\n", file);break;}
                case 'd':{printf("Total size of file(in bytes): %ld\n", INFO.st_size);break;}
                case 'h':{printf("Number of hard links: %d\n", INFO.st_nlink);break;}
                case 'm':{print_time_last_modif(INFO);break;}
                case 'a':{print_access(INFO);break;}
                case 'l':{create_symlink(file, INFO);break;}
                default: {printf("Please input accepted commands: n,d,h,m,a,l.\n");break;}
            }
            exit(0);
        }
        
    }

    int p2id;
    if((p2id=fork())<0){
        printf("Could not start second child process\n");
    }
    if(p2id==0){
        if(check_c_extenssion(file)){
            printf("W/E:\n");
            execlp("c_comp.sh", file, (char*)NULL);
            printf("\n");
        }
        else {
            printf("Number of newlines in file:");    
            execlp("wc","wc", "-l", file, (char*)NULL);
            printf("\n");
        }
        exit(0);
    }

    for(int j=0; pid[j]!=NULL; j++){
        printf("The process with PID <%d>, index <%d>, has ended with the exit code <%d>\n", pid[j], j, waitpid(pid[j]));
    }

    printf("The process with PID <%d>, has ended with the exit code <%d>\n", p2id, waitpid(p2id));
}

void handle_link(char* symlink, struct stat INFO){//file descriptor and stat info

    printf("HARDLINK\n");
    printf("----MENU----\n->n: name\n->l: delete\n->d: size of sl\n->t: size of target file\n->a: access rights\n");
    printf("\nPlease enter your options\n");

    char commands[6]; commands[5]='\0';
    scanf("-%5s", commands);

    bool exit_flag=false;

    for(int i=0; commands[i]!='\0'; i++){
        int pid;
        if((pid=fork())<0){
            printf("could not create child process");
            exit(2);
        }
        if(pid==0){
            switch (commands[i]) {
                case 'n':{printf("Name of link: %s\n", symlink);break;}
                case 'l':{
                    printf("deleting link\n");
                    unlink(symlink);
                    exit_flag=true;
                    break;
                    }
                case 'd':{print_link_size(symlink);break;}
                case 't':{printf("Size of target: %ld", INFO.st_size);break;}
                case 'a':{print_access(INFO);break;}
                default: {printf("Please input accepted commands: n,l,d,t,a.\n");break;}
            }
            if(exit_flag) break;
            exit(0);
        }
    }

    for(int j=0; commands[j]!='\0'; j++){
        wait();
    }
} 

void handle_dir(char* dir, struct stat INFO){
    
    printf("DIRECTORY\n");
    printf("----MENU----\n->n: name\n->d: size\n->a: access rights\n->c: Number of files with .c extension\n");
    printf("\nPlease enter your options\n");

    char commands[5]; commands[6]='\0';
    scanf("-%4s", commands);

    for(int i=0; commands[i]!='\0'; i++){
        int pid;
        if((pid=fork())<0){
            printf("could not create child process");
            exit(2);
        }
        if(pid==0){
            switch (commands[i]) {
                case 'n':{printf("Name of direcory: %s\n", dir);break;}
                case 'd':{
                    print_dir_size(dir);break;
                    }
                case 'a':{print_access(INFO);break;}
                case 'c':{printf("Files with .c extension: %d\n",count_c_files(dir));break;}
                default: {printf("Please input accepted commands: n,d,a,c.\n");break;}
            }
            exit(0);
        }
    }

    for(int j=0; commands[j]!='\0'; j++){
        wait();
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
    else if(S_ISDIR(INFO.st_mode))handle_dir(argv[1], INFO);
    //else printf("Please input regular file or symbolic link\n");

    return 0;
}