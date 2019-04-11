#include"headers/test.h"// tu doajesz headera i ditara siema 
#include"headers/Start.h"
#include"headers/Files.h"


#include <sys/types.h>
#include <sys/stat.h>


#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>

#include <stdlib.h>
#include <stdio.h>




 int IsDeamonWorking=0;// 0 means false- t s waiting
 CONFIG Values;

 void Handler(int sig){

         if(IsDeamonWorking==0){
                 IsDeamonWorking=1;
                CopyFiles(Values.FirstDir,Values.SecondDir,Values.deepSynch,Values.FileSize);
                DeleteExtraFiles(Values.SecondDir,Values.FirstDir,Values.deepSynch);
                IsDeamonWorking=0;
         }
 }

int main(int ArgNum,char* Arg[]) {


        CONFIG Values;
        Values=CheckDirectories(ArgNum,Arg);
      
        pid_t pid, MyPid;

        pid = fork();
        if (pid < 0) {
                exit(EXIT_FAILURE);
        }


        if (pid > 0) {
                exit(EXIT_SUCCESS);
        }

        //////////////////////////////////////////////////////////////////////////
                MyPid = getpid();
                // code below save pd of the deamon and it can be read
        char pwd[255];// code which save pid number
        int tmpp=MyPid;
        getcwd(pwd,sizeof(pwd));

        int totalDigits=0;
        	while(tmpp!=0){ // this code counts how many numbers are in pid number
		//4
		tmpp = tmpp/10;
		totalDigits ++;
	        }

        char *PidString=calloc(totalDigits,sizeof(char)); // make enought space for piid in string
        strcat(pwd,"/pid");                     // add pid - becouse pid is our file
        sprintf(PidString,"%d",MyPid);                         // conwert int into string

        int TpmFileDesc= open(pwd,O_WRONLY|O_CREAT|O_TRUNC,S_IRWXU|S_IRWXG|S_IROTH);

        write(TpmFileDesc,PidString,strlen(PidString));
        close(TpmFileDesc);
        free(PidString);

        //////////////////////////////////////////////
        umask(0);
                
        if ((chdir("/")) < 0) {

                exit(EXIT_FAILURE);
        }
        
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);


        signal(SIGUSR1,Handler);


        while (1==1) {
                

                if(IsDeamonWorking==0){

                IsDeamonWorking=1;
                CopyFiles(Values.FirstDir,Values.SecondDir,Values.deepSynch,Values.FileSize);
                DeleteExtraFiles(Values.SecondDir,Values.FirstDir,Values.deepSynch);
                IsDeamonWorking=0;

                }

                sleep(Values.time_wait); 
        }

        exit(EXIT_SUCCESS);
   
}