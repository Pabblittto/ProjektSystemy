/*
#include<stdio.h> // biblioteka z funkcjami, które sprawdzają czy istnieje dany katalog itd
#include <errno.h>// biblioteka do obsługi błędów
#include <sys/types.h>// biblioteka do typów systemowych
#include <dirent.h>// biblioteka do funkcji Linuxa do obslugi 
#include<ctype.h>
#include<stdlib.h>
#include <string.h>
*/
#include<stdio.h>
#include<sys/types.h>
#include<dirent.h>

typedef struct Object{// object from directory
    char* path;     // path to this certain 
    int type;       // 0-its normal file
                    // 1 -its directory
    char* date;     //date of created object
    struct Object* next;// pointer to next object
    long size;      // size of the file, if it is directory it is set to 0
} OBJECTLIST;

void Add(OBJECTLIST*,char* path,int type,char* date,unsigned long size );// adding object to the end of list

char** ScanDirectory(char* Directory);
//function scans directory and returns list of all objects inside(later directories can be ignored)  


void CopyFiles(char* FiirstDir,char* SecondDir,int IfDeepSynch);
//main function for doing whole job
