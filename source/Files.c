#include "../headers/Files.h"
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>

char** ScanDirectory(char* Directory)
{
    DIR* DirPointer=opendir(Directory);

    if(DirPointer==NULL){
        printf("Fatal error: Directory was deleted/not found");
        exit(1);
    }

    OBJECTLIST* List=NULL;

    struct dirent* tmp;// temporary object for "reading" files from directory
    

    char* tmppath;  // tmp variables for storaging info- for better code apperance 
    int tmptype;
    long int tmpDate;
    unsigned long tmpsize;

    struct stat* tmpStatObj;



    while((tmp=readdir(DirPointer))!=NULL)// read all objects in directory
    {
        if( strcmp(tmp->d_name,".")!=0 && strcmp(tmp->d_name,"..")!=0){// ignore ".." and "." "files"

                if(strcmp(&tmp->d_type,"DT_DIR")!=0)// if the file is anything than directory
                    tmptype=0;// its simple file, not directory
                else
                    tmptype=1;// its directory
                    
                tmppath=malloc(sizeof(char)*(strlen(Directory)+strlen(tmp->d_name)+5)); // make space for whole name + 5 chars just in case
                
                strcat(tmppath,Directory);

            if(Directory[strlen(Directory)-1]!='/')        //user can type directory like: "/home/user/directory" or it can type "/home/user/directory/" we need to know if there is a '/' char
            {
                strcat(tmppath,"/");// add this char
            }
            strcat(tmppath,tmp->d_name);// at this point tmpPath name is ready

            if(tmptype==0) // when its anything than directory- check its size and last modyfing date
            {
                stat(tmppath,tmpStatObj);
                tmpsize=tmpStatObj->st_size;
                tmpDate=tmpStatObj->st_mtime;
                // dobrz trzeba ustalić jak przekonwetowac ten czas na cza utime(...) bo nie działa, coś to moze byc zwiazane z Timestamp, trzeba to ogarnac 

            }
            else
            {
                
            }
            
            








            
        }
        
    }
                                              // TRZEBA ZIGNOROWAC PLIKI .. I . BO TO TEZ JEST POKAZYWANE

}


void Add(OBJECTLIST* first,char* path,int type,char* date,unsigned long size )
{
    if(first=NULL){// there is no elements in list
    first= malloc(sizeof(OBJECTLIST));
    }
    else 
    {
        while(first->next!=NULL){
            first=first->next;
        }
        first->next= malloc(sizeof(OBJECTLIST)); // alloct memory
        first=first->next;// become this newly created memory
    }

    first->date=date;
    first->next=NULL;
    first->path=path;
    first->type=type;
    first->size=size;
}