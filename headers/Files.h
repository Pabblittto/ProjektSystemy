/*
#include<stdio.h> // biblioteka z funkcjami, które sprawdzają czy istnieje dany katalog itd
#include <errno.h>// biblioteka do obsługi błędów
#include <sys/types.h>// biblioteka do typów systemowych
#include <dirent.h>// biblioteka do funkcji Linuxa do obslugi 
#include<ctype.h>
#include<stdlib.h>
#include <string.h>
*/
#ifndef FILES_H
#define FILES_H 1

#include "../headers/Start.h"


typedef struct Object{// object from directory
    char* path;     // path to this certain 
    int type;       // 0-its normal file
                    // 1 -its directory
    long int date;     //date of created object
    struct Object* next;// pointer to next object
    long size;      // size of the file, if it is directory it is set to 0
} OBJECTLIST;

void Add(OBJECTLIST**,char* path,int type, long int date,unsigned long size );// adding object to the end of list

OBJECTLIST* Find(OBJECTLIST* list, char* path,int type);// function returns object if exists or it returns NULL if not


char* NameOfLastElement(char* path);// if we want to create directory or file , we need it's name- this function extract last part from path

OBJECTLIST* ScanDirectory(char* Directory);
//function scans directory and returns list of all objects inside(later directories can be ignored)  


void CopyFiles(char* FiirstDir,char* SecondDir,int IfDeepSynch,int FileSize);
//main function for doing whole job

void CopyFileWithReadWrite(char* PathToFile,char* PathToDirectory,long int TimeofModyfy);// finction which copy files with Read/Write functions
void CopyFileWithMmap(char* PathToFile,char* PathToDirectory,long int TimeOfModyfy,int FileSize);
void DeleteExtraFiles(char* DirectoryToCheck_, char* TheChoosenOneDirectory_,int deepSynch );//

void GetRealPath(char** Args[]);// function changes short paths iinto absolute path
void Wholeprogram(CONFIG configuration,int* isWorking );
#endif