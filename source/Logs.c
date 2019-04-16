#include "../headers/Logs.h"

#include <syslog.h>
#include <pwd.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void startLog(char *Args[], int ArgNum)
{

    openlog ("SynchDirLog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); // Open log connection

    struct passwd *pw = getpwuid(geteuid());
    char str[10];
    
    if (pw)  // Print name if we can
    {
        syslog (LOG_NOTICE, "Program started by User %s",pw->pw_name);
    }
    else
        syslog (LOG_NOTICE, "Program started by User %d", getuid ());


    for(int i=1;i<ArgNum - 2;i++){
        strcat(str,Args[i]);
        strcat(str," ");
    }
    
    syslog (LOG_NOTICE, "Program started with parametrs: %s",str);
    syslog (LOG_INFO, "Comparing files between %s and %s ...",Args[ArgNum-2], Args[ArgNum-1]);

    closelog ();
}

void copyLog(char *filename, char *directory){
    openlog ("SynchDirLog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); // Open log connection
    syslog (LOG_INFO, "Copying file %s to %s", filename, directory);
    closelog ();
}

void copyDirLog(char *directory){
    openlog ("SynchDirLog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); // Open log connection
    syslog (LOG_INFO, "Creating directory %s",directory);
    closelog ();
}

void DeamonState(int stan){
    openlog ("SynchDirLog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); // Open log connection
    if(stan==0){// Deamon idzei lulu
    syslog (LOG_INFO, "Deamon go to sleep");
    }   
    else
    syslog (LOG_INFO, "Deamon wakes up");
    closelog ();
}

void deleteLog(char *filename, char type, char *path){
    openlog ("SynchDirLog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); // Open log connection
    if(type == 'F')
    {
         syslog (LOG_INFO, "Deleteing file %s from directory %s",path ,filename);
    }
    else
    {
        syslog (LOG_INFO, "Deleteing directory %s from directory %s",path ,filename);
    }
    
    closelog ();
}

void endLog(){
    openlog ("SynchDirLog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); // Open log connection
    syslog (LOG_NOTICE, "Closed !");
    closelog ();
}