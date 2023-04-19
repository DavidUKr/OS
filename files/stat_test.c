#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main(){
    struct stat st;

   
    stat("Test.txt", &st);

    printf("%ld\n",st.st_size);

    printf("%d", st.st_mode);
}