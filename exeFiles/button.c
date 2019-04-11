
#include<stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>
#include<signal.h>
#include <sys/types.h>
#include <stdlib.h>


int main(){

char pwd[255];
getcwd(pwd,sizeof(pwd));
strcat(pwd,"/pid");
int fd=open(pwd,O_RDONLY);
char *buffor=calloc(20,sizeof(char));

read(fd,buffor,strlen(buffor));
printf("przeczytana wartosc to %s",buffor);

close(fd);

//kill(123,SIGUSR1);// tu trzeba dac pid z pliku

    return 0;
}