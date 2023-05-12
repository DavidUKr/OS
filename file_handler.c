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

void count_we(char* input){
    printf("W/E:\n");
    execlp("./c_comp.sh", input, (char*)NULL);
    printf("\n");
}

void print_line_num(char* input){
    printf("Number of newlines in file:\n");    
    execlp("wc","wc", "-l", input, (char*)NULL);
    printf("\n");
}

void change_rights(char* input){
    execlp("chmod", "chmod", "=770", input, (char*)NULL);
    printf("changed file access rights =770\n");
}

void makefile(char* dir){
    char* name="_file.txt";
    strcat(dir,name);
    char* path="output/";
    strcat(path, dir);
    execlp("touch", "touch", path,(char*)NULL);
    printf("created file: %s\n", dir);
}

void manageChildStdioPipes(int* pfd_toParent){
    close(pfd_toParent[0]); //reading end of toParent
    dup2(pfd_toParent[1], 1); //redirect standard output to writing end of toParent
}

FILE* manageParentStdioPipes(int* pfd_toParent){
    close(pfd_toParent[1]); //writing end of toParent
    //we leave stdin of parent as normal

    return fdopen(pfd_toParent[0],"r"); //returning pipe reading end as stream
}

void calculate_score(FILE* stream, char* file_name){
    int err_count, warn_count;
    double score;
    fscanf(stream,"%d %d",&err_count, &warn_count);
    
    if(err_count==0){
        if(warn_count==0) score=10;
        else if(warn_count>10) score=2;
        else score=2+8*(10-warn_count)/10;
    }
    else score=0; 

    FILE* grades;
    if((grades=fopen("output/grades.txt", "a"))<0){
        perror("Could not open grades.txt file");
        exit(3);
    }
    else {
        fprintf(grades, "%s:%f\n", file_name, score); //for windows \r\n
        fclose(grades);
    }
}

void print_stream_to_stdout(FILE* stream){
    char string[100];
    fscanf(stream, "%s", string);
    printf("%s\n", string);
}

void handle_file(char* file, struct stat INFO){ //file descriptor and stat info

    printf("REGULAR FILE\n");
    printf("----MENU----\n->n: name\n->d: size\n->h: hard link count\n->m: last modification\n->a: acces rights\n->l: create symbolic link \n");
    printf("\nPlease enter your options\n");

    char commands[7]; commands[6]='\0'; //commands scanning
    scanf("-%6s", commands);

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
        printf("\n");
    }
}

void handle_link(char* symlink, struct stat INFO){//file descriptor and stat info

    printf("HARDLINK\n");
    printf("----MENU----\n->n: name\n->l: delete\n->d: size of sl\n->t: size of target file\n->a: access rights\n");
    printf("\nPlease enter your options\n");

    char commands[6]; commands[5]='\0';
    scanf("-%5s", commands);

    bool exit_flag=false;

    for(int i=0; commands[i]!='\0'; i++){
        switch (commands[i]) {
            case 'n':{printf("Name of link: %s\n", symlink);break;}
            case 'l':{
                printf("deleting link\n");
                unlink(symlink);
                exit_flag=true;
                break;
                }
            case 'd':{print_link_size(symlink);break;}
            case 't':{printf("Size of target: %ld\n", INFO.st_size);break;}
            case 'a':{print_access(INFO);break;}
            default: {printf("Please input accepted commands: n,l,d,t,a.\n");break;}
        }
        printf("\n");
        if(exit_flag) break;
    }
    
}

void handle_dir(char* dir, struct stat INFO){
    
    printf("DIRECTORY\n");
    printf("----MENU----\n->n: name\n->d: size\n->a: access rights\n->c: Number of files with .c extension\n");
    printf("\nPlease enter your options\n");

    char commands[5]; commands[6]='\0';
    scanf("-%4s", commands);

    for(int i=0; commands[i]!='\0'; i++){
        switch (commands[i]) {
            case 'n':{printf("Name of direcory: %s\n", dir);break;}
            case 'd':{
                print_dir_size(dir);break;
                }
            case 'a':{print_access(INFO);break;}
            case 'c':{printf("Files with .c extension: %d\n",count_c_files(dir));break;}
            default: {printf("Please input accepted commands: n,d,a,c.\n");break;}
        }
        printf("\n");
    }
}


int main(int argc, char* argv[]){

    if(argc<2) {
        printf("Provide file, directory or symbolic link! Exiting...\n");
        exit(2);
    }
    char* input;

    struct stat INFO;
    for(int i=1; i<argc; i++){
        input=argv[i];

        int pid1;
        if((pid1=fork())<0){
            perror("Could not start second child process\n");
            exit(1);
        }
        if(pid1==0){
            printf("process id: %d; ", getpid());
            printf("parent id: %d\n", getppid());

            stat(input, &INFO);
            if(S_ISREG(INFO.st_mode)){
                handle_file(input, INFO);
            }
            else if(S_ISLNK(INFO.st_mode)){
                handle_link(input, INFO);
            }
            else if(S_ISDIR(INFO.st_mode)){
                handle_dir(input, INFO);
            }
            exit(0);
        }

        int pfd_toParent[2];
        if(pipe(pfd_toParent)<0){
            perror("Pipe to parent creation error\n");
            exit(2);
        }
        
        int pid2;
        if((pid2=fork())<0){
            printf("Could not start second child process\n");
        }
        if(pid2==0){
            //printf("process id: %d; ", getpid());
            //printf("parent id: %d\n", getppid());

            manageChildStdioPipes(pfd_toParent);

            stat(input, &INFO);
            if(S_ISREG(INFO.st_mode)){
                if(check_c_extenssion(input)) {count_we(input);}
                else {
                    print_line_num(input);
                }
            }
            else if(S_ISLNK(INFO.st_mode)){
                change_rights(input);
            }
            else if(S_ISDIR(INFO.st_mode)){
                makefile(input);
            }

            exit(0);
        }

        FILE* stream=manageParentStdioPipes(pfd_toParent);
        if(check_c_extenssion(input)) calculate_score(stream, input);
        else {
            print_stream_to_stdout(stream);
        }

        close(pfd_toParent[0]);

        int status;
        int exit_pid=wait(&status);
        if(WIFEXITED(status)) printf("The first child process with PID %d, has ended with the exit code %d\n", 
                            exit_pid, WEXITSTATUS(status));

        exit_pid=wait(&status);
        if(WIFEXITED(status)) printf("The second child process with PID %d, has ended with the exit code %d\n", 
                            exit_pid, WEXITSTATUS(status));
        //printf("The second child process with PID %d, has ended with the exit code %d\n", p2id, waitpid(p2id));
    }
    return 0;
}