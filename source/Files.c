#include "../headers/Files.h"
#include<stdlib.h>
#include<string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <utime.h>


OBJECTLIST* ScanDirectory(char* Directory)
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

    struct stat tmpStatObj;


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
                stat(tmppath,&tmpStatObj);
                tmpsize=tmpStatObj.st_size;
                tmpDate=tmpStatObj.st_mtime;
                // dobrz trzeba ustalić jak przekonwetowac ten czas na cza utime(...) bo nie działa, coś to moze byc zwiazane z Timestamp, trzeba to ogarnac 
            }
            else
            {
                tmpsize=0;
                tmpDate=0;
            }
            
            Add(&List,tmppath,tmptype,tmpDate,tmpsize); // add object to list 
        }
        
    }

    return List;
}


void Add(OBJECTLIST** first,char* path,int type,long int date,unsigned long size )
{
    //OBJECTLIST* first=*firstparam;
    OBJECTLIST* AddedObj= malloc(sizeof(OBJECTLIST));

    AddedObj->date=date;
    AddedObj->next=NULL;
    AddedObj->path=path;
    AddedObj->type=type;
    AddedObj->size=size;

    if(first==NULL){// there is no elements in list
    first= malloc(sizeof(OBJECTLIST));
    }
    else 
    {
        while((*first)->next!=NULL){
            first=(*first)->next;
        }
        (*first)->next= malloc(sizeof(OBJECTLIST)); // alloct memory//// teeeeeeeeeee ale jak stajesz sie nulem to nic na ciebie nie kop
        (*first)=(*first)->next;// become this newly created memory
    }



}

OBJECTLIST* Find(OBJECTLIST* List, char* path,int type)
{
    OBJECTLIST* result=NULL;

    while(List!=NULL){
        if(strcmp(List->path,path)==0 && List->type==type)// if there is an object with the same type and name path
        {
            result=List;
            return result;
        }
        else
        {
            List=List->next;
        }
    }
    return result;
}

char* NameOfLastElement(char* path)
{
    //char* tmp=malloc(sizeof(char)*256);
    char tmp[255];
    int end=0;// if
    int iterator=1;
    int lenght=strlen(path);

    if(path[lenght-1]=='/')
    iterator=2;
    char znak;

    while( path[lenght-iterator]!='/'){
        znak=path[lenght-iterator];
        tmp[iterator-2]=znak;
       // strcat(tmp,&znak);
        iterator++;

    }// this name is reversed

    int strlenght=strlen(tmp);
    char* result=malloc(sizeof(char)*strlenght);
    

    iterator=1;
    for(; iterator <= strlenght; iterator++)
    {
        znak=tmp[strlenght-iterator];
        result[iterator-1]=znak;
    }
    
    return result;
}


void CopyFiles(char* FirstDir,char* SecondDir,int IfDeepSynch)
{
OBJECTLIST* FilesInFirst=ScanDirectory(FirstDir);
OBJECTLIST* FilesInSecond=ScanDirectory(SecondDir);

OBJECTLIST* tmp;// tmp pointer for copying

    while(FilesInFirst!=NULL){

        if(FilesInFirst->type==1 && IfDeepSynch==1)// if element is directory and recursive mode is on
        { 
            tmp=Find(FilesInSecond,FilesInFirst->path,FilesInFirst->type);// check if directory exists
            if(tmp!=NULL)//  directory exists, so go into it
            {
               CopyFiles(FilesInFirst->path,tmp->path,IfDeepSynch);
            }
            else            // directory do not existrs - create it
            {               
                char* tmpName= malloc(sizeof(char)*(strlen(SecondDir)+strlen(FilesInFirst->path)-strlen(FirstDir)));// create place for new name 
                // tu trzeba zrobic pojebane działania na stringu, który połączy Seconddir + "nazwa kolejnego katalogu"

                strcat(tmpName,SecondDir);
                if(SecondDir[strlen(SecondDir-1)]!='/')
                    strcat(tmpName,"/");
                strcat(tmpName,NameOfLastElement(FilesInFirst->path));



                mkdir(tmpName,S_IRWXU|S_IRWXG|S_IROTH);// create folder !!

            }
            
        }
        else// if this is a normal file
        {
            /* code */
        }
        



        FilesInFirst=FilesInFirst->next;// move to next object 
    }

}