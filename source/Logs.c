#include "../headers/Logs.h"

#include <syslog.h>
#include <pwd.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

void startLog(char *Args[], int ArgNum)
{

    openlog ("SynchDirLog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); // Open log connection

    //uid_t uid = geteuid();
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
    
    syslog (LOG_INFO, "Program started with parametrs: %s",str);
    syslog (LOG_INFO, "Copying files from  %s to %s ...",Args[ArgNum-2], Args[ArgNum-1]);

    closelog ();
}

void randomLog(char *filename){
    openlog ("SynchDirLog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); // Open log connection
    syslog (LOG_INFO, "Copying %s", filename);
    closelog ();
}

void endLog(){
    openlog ("SynchDirLog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); // Open log connection
    syslog (LOG_NOTICE, "Closed ...");
    closelog ();
}