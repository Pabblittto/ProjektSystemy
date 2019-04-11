
#include<stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include<string.h>
#include<signal.h>
#include <sys/types.h>
#include <stdlib.h>


int main(){
char tmp[244];
char *pwd=calloc(255,sizeof(char));
getcwd(tmp,sizeof(tmp));
strcat(pwd,tmp);
strcat(pwd,"/pid");
int fd=open("./pid",O_RDWR);
char *buffor=malloc(20*sizeof(char));

read(fd,buffor,20);
close(fd);

int number=0,len;
len=strlen(buffor);
for(int i = 0; i < len; i++)
{
    number= number *10 + (buffor[i] - '0');
}

kill(number,SIGUSR1);

    return 0;
}