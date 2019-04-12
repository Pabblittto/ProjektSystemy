#include "../headers/Files.h"


#include<stdlib.h>
#include<string.h>

#include <unistd.h>
#include <sys/types.h>
#include <utime.h>
#include <sys/mman.h>
#include <unistd.h>
#include<stdio.h>
#include<dirent.h>


#include <limits.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>


OBJECTLIST* ScanDirectory(char* Directory)
{
    DIR* DirPointer=opendir(Directory);

    if(DirPointer==NULL){
        printf("Fatal error: %s",strerror(errno));
        closedir(DirPointer);
        return NULL;
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

                if(tmp->d_type!=4)// if the file is anything than directory (4 is asociaed with directories)
                    tmptype=0;// its simple file, not directory
                else
                    tmptype=1;// its directory
                    
                tmppath=calloc((strlen(Directory)+strlen(tmp->d_name)+5),sizeof(char)); // make space for whole name + 5 chars just in case
                
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

    closedir(DirPointer);
    return List;
}


void Add(OBJECTLIST** first,char* path,int type,long int date,unsigned long size )
{
    OBJECTLIST* beginning=*first;// the beggining of whole list
    OBJECTLIST* AddedObj= calloc(1,sizeof(OBJECTLIST));

    AddedObj->date=date;
    AddedObj->next=NULL;
    AddedObj->path=path;
    AddedObj->type=type;
    AddedObj->size=size;

    if(*first==NULL){// there is no elements in list
    (*first)=AddedObj;
    }
    else 
    {
        while((*first)->next!=NULL){
            (*first)=(*first)->next;
        }

        (*first)->next=AddedObj;
    }

    if(beginning!=NULL)
    *first=beginning;
}

OBJECTLIST* Find(OBJECTLIST* List, char* path,int type)
{
    OBJECTLIST* result=NULL;
    char* ArgumentLast=NameOfLastElement(path);
    char* ListLast=NULL;

    while(List!=NULL){

        ListLast=NameOfLastElement(List->path);
        if(strcmp(ListLast,ArgumentLast)==0 && List->type==type)// if there is an object with the same type and name path
        {
            result=List;
            break;
        }
        else
        {
            List=List->next;
        }
    }

    if(ListLast!=NULL)
    free(ListLast);
    free(ArgumentLast);

    return result;
}

char* NameOfLastElement(char* path)
{
    char tmp[250]="";
    int iterator=1;
    int lenght=strlen(path);

    if(path[lenght-1]=='/')
    iterator=2;
    char znak;
    int starter=iterator;// the value which is specyfiing on which number number iteretor starterd 

    while( path[lenght-iterator]!='/'){
        znak=path[lenght-iterator];
        tmp[iterator-starter]=znak;
        iterator++;

    }// this name is reversed

    int strlenght=strlen(tmp);
    char* result=calloc(strlenght,sizeof(char));


    iterator=1;
    for(; iterator <= strlenght; iterator++)
    {
        znak=tmp[strlenght-iterator];
        result[iterator-1]=znak;
    }
    
    return result;
}

void CopyFileWithReadWrite(char* PathToFile,char* PathToDirectory,long int TimeOfModyfy)// path to directory says where this file should be created
{                                                                               // if file should be replaced- do it
    char* filename=NameOfLastElement(PathToFile);
    char* NewFilePath=calloc((strlen(PathToDirectory)+strlen(filename)+3),sizeof(char));// allock enought space
    strcat(NewFilePath,PathToDirectory);
    char znak=NewFilePath[strlen(NewFilePath)-1];
    if(znak!='/')
        strcat(NewFilePath,"/");// add '/' on the end if neccesarry- becouse we need : "directory/filename"
        
    strcat(NewFilePath,filename);

    free(filename);// this pointer is allocated in nameoflastelement funtion

    int ReadFile= open(PathToFile,O_RDONLY);

    int CreatedFile= open(NewFilePath,O_WRONLY|O_CREAT| O_TRUNC ,S_IRWXU|S_IRWXG|S_IROTH);// user and group have full permission, file is truncated and created if not exists
    char buffer[1];

    while (read(ReadFile,buffer,sizeof(buffer)))// read one bit till if reach end
    {
        write(CreatedFile,buffer,1);
    }

    struct utimbuf ModyfiTime={TimeOfModyfy,TimeOfModyfy};
    utime(NewFilePath,&ModyfiTime);

    close(CreatedFile);
    close(ReadFile);
    free(NewFilePath);
}

void CopyFileWithMmap(char* PathToFile,char* PathToDirectory,long int TimeOfModyfy,int SizeOfFile)
{
    char* filename=NameOfLastElement(PathToFile);
    char* NewFilePath=calloc((strlen(PathToDirectory)+strlen(filename)+3),sizeof(char));// allock enought space
    strcat(NewFilePath,PathToDirectory);
    char znak=NewFilePath[strlen(NewFilePath)-1];
    if(znak!='/')
        strcat(NewFilePath,"/");// add '/' on the end if neccesarry- becouse we need : "directory/filename"
        
    strcat(NewFilePath,filename);

    free(filename);// this pointer is allocated in nameoflastelement funtion- now its useless

    remove(NewFilePath);// if file exists this line remove it

    int ReadFile= open(PathToFile,O_RDONLY); // file descriptor

    int CreatedFile= open(NewFilePath,O_WRONLY | O_CREAT| O_APPEND ,S_IRWXU|S_IRWXG|S_IROTH);// user and group have full permission, file is truncated and created if not exists

    size_t pagesize= getpagesize();

    char* region;
    

        region=mmap(NULL,SizeOfFile,PROT_READ|PROT_WRITE,MAP_PRIVATE,ReadFile,0);

        write(CreatedFile,region,SizeOfFile);

    struct utimbuf ModyfiTime={TimeOfModyfy,TimeOfModyfy};
    utime(NewFilePath,&ModyfiTime);
    
    close(CreatedFile);
    close(ReadFile);
    free(NewFilePath);
}

void DeleteEverythingIDire(char* pathToDirectory){
OBJECTLIST* allFilesInside= ScanDirectory(pathToDirectory);
    while(allFilesInside!=NULL){

        if(allFilesInside->type==1){// is this is a directory
            DeleteEverythingIDire(allFilesInside->path);
            remove(allFilesInside->path);
        }
        else
        {           // if ist normal file
                remove(allFilesInside->path);
        }
        
        allFilesInside=allFilesInside->next;
    }
    


}




void DeleteExtraFiles(char* DirectoryToCheck_, char* TheChoosenOneDirectory_,int deepSynch ){
    OBJECTLIST* DirectoryToCheck=ScanDirectory(DirectoryToCheck_);
    OBJECTLIST* TheChoosenOneDirectory=ScanDirectory(TheChoosenOneDirectory_);

    OBJECTLIST* BeginningToCheck=DirectoryToCheck;
    OBJECTLIST* BeginningChoosen=TheChoosenOneDirectory;

    OBJECTLIST* tmp;

    while(DirectoryToCheck!=NULL){ // delete files in second directory if it dont exists in first

        if(DirectoryToCheck->type==1 && deepSynch==1){// if the element is a directory and recursive method is on

            tmp=Find(TheChoosenOneDirectory,DirectoryToCheck->path,DirectoryToCheck->type);// 

            if(tmp!=NULL){// if this directory exists 
                DeleteExtraFiles(DirectoryToCheck->path,tmp->path,deepSynch);
            }
            else
            {
                DeleteEverythingIDire(DirectoryToCheck->path);
                remove(DirectoryToCheck->path);// delete this path
            }
            

        }else if(DirectoryToCheck->type==0)// if its normal file
        {
            tmp=Find(TheChoosenOneDirectory,DirectoryToCheck->path,DirectoryToCheck->type);// look for this file

                if(tmp==NULL){// if this file does not exists in first directory- delete it
                    remove(DirectoryToCheck->path);
                }
        }
        DirectoryToCheck=DirectoryToCheck->next;
    
    }

    if(DirectoryToCheck!=NULL)
    freeList(DirectoryToCheck);

    if(TheChoosenOneDirectory!=NULL)
    freeList(TheChoosenOneDirectory);
}



void CopyFiles(char* FirstDir,char* SecondDir,int IfDeepSynch,int FileSize)
{
OBJECTLIST* FilesInFirst=ScanDirectory(FirstDir);
OBJECTLIST* FilesInSecond=ScanDirectory(SecondDir);

OBJECTLIST* BeginningFirst=FilesInFirst; // pointers on the beginning of lists
OBJECTLIST* BeginnigSecond=FilesInSecond;

OBJECTLIST* tmp=NULL;// tmp pointer for copying


    FilesInFirst=BeginningFirst;// move all pointer to the beginning of list
    FilesInSecond=BeginnigSecond;

    while(FilesInFirst!=NULL){

        if(FilesInFirst->type==1 && IfDeepSynch==1)// if element is directory and recursive mode is on
        { 
            tmp=Find(FilesInSecond,FilesInFirst->path,FilesInFirst->type);// check if directory exists
            if(tmp!=NULL)//  directory exists, so go into it
            {
               CopyFiles(FilesInFirst->path,tmp->path,IfDeepSynch,FileSize);
            }
            else            // directory do not existrs - create it
            {               
                char* tmpName= calloc((strlen(SecondDir)+strlen(FilesInFirst->path)-strlen(FirstDir)+2),sizeof(char));// create place for new name 

                strcat(tmpName,SecondDir);

                char end=SecondDir[strlen(SecondDir)-1];
                if(end!='/')
                    strcat(tmpName,"/");

                strcat(tmpName,NameOfLastElement(FilesInFirst->path));
                mkdir(tmpName,S_IRWXU|S_IRWXG|S_IROTH);// create folder !!
                CopyFiles(FilesInFirst->path,tmpName,IfDeepSynch,FileSize);// if it does not exist- create it and create other  files nsde ths folder
            }
            
        }
        else if(FilesInFirst->type==0) // if this is a normal file
        {
            tmp=Find(FilesInSecond,FilesInFirst->path,FilesInFirst->type);// check if this file exists

            if(tmp!=NULL){// file was found, now check its date of modyfy

                if(tmp->date!=FilesInFirst->date){// date is not equal-  need to copy this file

                    if(FilesInFirst->size<=(FileSize*1000000))
                    {
                        CopyFileWithReadWrite(FilesInFirst->path,SecondDir,FilesInFirst->date);
                    }
                    else
                    {
                        CopyFileWithMmap(FilesInFirst->path,SecondDir,FilesInFirst->date,FilesInFirst->size);
                    }
                }
            }
            else// there is no file like that
            {
                    if(FilesInFirst->size<=(FileSize*1000000))
                    {
                        CopyFileWithReadWrite(FilesInFirst->path,SecondDir,FilesInFirst->date);
                    }
                    else
                    {
                        CopyFileWithMmap(FilesInFirst->path,SecondDir,FilesInFirst->date,FilesInFirst->size);
                    }
            }  
        }
        // if this is a directory and deepsynch is false - it will do nothing
        
        FilesInFirst=FilesInFirst->next;// move to next object 
    }


    if(BeginningFirst!=NULL)
    freeList(BeginningFirst);

    if(BeginnigSecond!=NULL)
    freeList(BeginnigSecond);

}


void Wholeprogram(CONFIG configuration,int* isWorking ){

      // if program is workking do not make it again 

        DeleteExtraFiles(configuration.SecondDir,configuration.FirstDir,configuration.deepSynch);
        CopyFiles(configuration.FirstDir,configuration.SecondDir,configuration.deepSynch,configuration.FileSize);



}

void freeList(OBJECTLIST* First){

OBJECTLIST * tmp=NULL;
if(First!=NULL){
while(First!=NULL){ 
    tmp=First;          
    First=First->next;
    free(tmp);
    }
}
}