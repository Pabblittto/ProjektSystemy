#include "../headers/Start.h"// biblioteka z funkcjami, które sprawdzają czy istnieje dany katalog itd


CONFIG CheckDirectories(int ArgNum,char* List[])
{
    char* firstDir="";// temporary variables
    char* seconDir="";
    int time=-1;
    int synch=0;//false
    int size=-1;

    CONFIG structure;

    if(ArgNum==1){
        printf("No arguments!!\n");
        exit(1);
    }


    for(int i=1;i<ArgNum;i++){// iliterate for all parameters


        if(!strcmp(List[i],"-R") || !strcmp(List[i],"-r"))
        {
            synch=1; 
            continue;
        }
        else if (!strcmp(List[i],"-t") || !strcmp(List[i],"-T")) // change time
        {
            i++;
               if(ISnumber(List[i]) && atoi(List[i])>0)//check if the parameter is a number or its smaller than 0
               {
                    time=atoi(List[i]);
               } 
               else
               {            // this is not a number, return an error
                    printf("Wrong parameter after -t argument\n");
                    exit(1);
               }
               
            continue;
        }
        else if (!strcmp(List[i],"-S") || !strcmp(List[i],"-s")) 
        {
            i++;
               if(ISnumber(List[i])|| atoi(List[i])>0)//check if the parameter is a number or its smaller than 0
               {
                    size=atoi(List[i]);
               } 
               else
               {            // this is not a number, return an error
                    printf("Wrong parameter after -s argument\n");
                    exit(1);
               }
            continue;
        }
        else                // its definitelly directory path
        {
            if (firstDir=="")
            { // if the first directory is empty- save string to it
                firstDir=List[i];
                DIR* tmp=opendir(firstDir);
                if (tmp==NULL) {// didnt found directory
                    printf("Wrong first directory path!\n");
                    printf("%s\n",strerror(errno));
                    exit(1);
                }
            }
             else if(seconDir=="")
             {
                seconDir=List[i];
                DIR* tmp=opendir(seconDir);
                if (tmp==NULL) {// didnt found directory
                    printf("Wrong second directory path!\n");
                    exit(1);
                }
             }
            else
            {
                printf("Wrong syntax- too many arguments after drirectory paths\n");
                exit(1);
            }
        }

    }// end for

    // if everything was good - program reach this place


    structure=InitConfigurationStructure(time,size,synch,firstDir,seconDir);


    return structure;
}

int ISnumber(char*string){// returns 1 if string is long number
    int good=1;// its good, its long number
    int lenght= strlen(string);

    for(int i=0;i<lenght;i++){
        if(!isdigit(string[i]))
            good=0;

    }

    return good;
}

//funkcje które trzeba zaimplementować :
// -t x     - zmiana czasu spania deamona gdzie x to czas w sekundach
// -R       -teraz trzeba też synchronizowac katalogi, normalnie to synchronizuje tylko pliczki- więc trzeba napisać 
//                  funkcję, ktora synchronizuje pliczkiw katalogu a dzięki -r walnie się to rekurencją 
// -S x     (Size)- x okresla od jakiego progu plik jest liczony jako duży plik- kiedy się wykorzystuje 
//                      odpowiednią funkcję kopiującą (read/write lub mmap/write)
CONFIG InitConfigurationStructure(int time,int SizeMB,int Synch,char* First,char* Second)
{
    CONFIG tmp;
    if (time==-1) 
        tmp.time_wait=DEFAULT_TIME
    else
        tmp.time_wait=time;
    
    if (SizeMB==-1) 
        tmp.FileSize=DEFAULT_SIZE
    else
        tmp.FileSize=SizeMB;

    tmp.deepSynch=Synch;// value will be given directly to function , without -1

    tmp.FirstDir=First;
    tmp.SecondDir=Second;   

    return tmp;
}