#include"headers/test.h"// tu doajesz headera i ditara siema 
#include"headers/Start.h"
#include"headers/Files.h"


#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>
#include <signal.h>

/*

int main(int ArgNum,char* Arg[] )
{// faza pierwsza- napisać program w mainie, który po jednym odpaleniu jest w stanie skopiować/usuwać
            // ogólnie zzynchronizować dwa foldery
            // trzeba ogarnąć logi do systemu Barto :3 
            // funkja, która potrafi skanować folder i robi liste plików + czas utworzenia tych plików ..... nooo ostro może być


OBJECTLIST* pointer;

CONFIG Values;
Values=CheckDirectories(ArgNum,Arg);
printf("Wartosci to :\n");
printf("%s pierwsze, %s drugie, %d czas,%d synchronizacja,%d rozmiar\n",(Values).FirstDir,(Values).SecondDir,(Values).time_wait,(Values).deepSynch,(Values).FileSize);

//pointer=ScanDirectory(Values.FirstDir);

CopyFiles(Values.FirstDir,Values.SecondDir,Values.deepSynch,Values.FileSize);
DeleteExtraFiles(Values.SecondDir,Values.FirstDir,Values.deepSynch);


char* teskt= NameOfLastElement("siemaneczko/katalog/");

printf("asd");

return 0;
}
*/


//signal(SIGUSR1);


int main(int ArgNum,char* Arg[]) {

        CONFIG Values;
        Values=CheckDirectories(ArgNum,Arg);
        
        pid_t pid, sid;
        
        pid = fork();
        if (pid < 0) {
                exit(EXIT_FAILURE);
        }


        if (pid > 0) {
                exit(EXIT_SUCCESS);
        }


        umask(0);
                

        sid = setsid();
        if (sid < 0) {
                /* Log the failure */
                exit(EXIT_FAILURE);
        }
        

        
        /* Change the current working directory */
        if ((chdir("/")) < 0) {
                /* Log the failure */
                exit(EXIT_FAILURE);
        }
        
        /* Close out the standard file descriptors */
        close(STDIN_FILENO);
        close(STDOUT_FILENO);
        close(STDERR_FILENO);
        

        int IsDeamonWorking=0;// 0 means false- t s waiting

        while (1) {
           
                Wholeprogram(Values,&IsDeamonWorking);


           sleep(Values.time_wait); 
        }
   exit(EXIT_SUCCESS);
}