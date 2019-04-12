#ifndef START_H
#define START_H 1


#include<stdio.h> // biblioteka z funkcjami, które sprawdzają czy istnieje dany katalog itd
#include <errno.h>// biblioteka do obsługi błędów
#include <sys/types.h>// biblioteka do typów systemowych
#include <dirent.h>// biblioteka do funkcji Linuxa do obslugi 
#include<ctype.h>
#include<stdlib.h>
#include <string.h>

#define DEFAULT_TIME 300;// its 5 minutes
#define DEFAULT_SIZE 10;

#ifndef CONFIG // ale to jest ogarniete- to trzeba przyzanc
typedef struct myconfig
{
    int time_wait;// how much time deamon waits - can be changed
    char* FirstDir;// 
    char* SecondDir;//names of directories
    int deepSynch;// boolean if there is -R parameter given, 1-true|| 0-false
    int FileSize;//maximum size in MB of "small" files
} CONFIG;
#endif

CONFIG CheckDirectories(int,char**);// function wchich check if directories exists + chcecks additional arguments
                                        // if the was a problem, returns null

CONFIG InitConfigurationStructure(int time,int SizeMB,int Synch,char* First,char* Second);// function for initialize default valuesin config structure
                                                                                // if the parameter is -1 -use default value

int ISnumber(char*string);// returns 1 if string is long number
#endif