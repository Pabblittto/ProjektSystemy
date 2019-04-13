#include "../headers/Logs.h"

#include <syslog.h>
#include <pwd.h>
#include <unistd.h>

void loggingStuff (void)
{

    openlog ("SynchDirLog", LOG_CONS | LOG_PID | LOG_NDELAY, LOG_LOCAL1); // Open log connection

    char *Arg;
    uid_t uid = geteuid();
    struct passwd *pw = getpwuid(uid);
    
    if (pw)  // Print name if we can
    {
        syslog (LOG_NOTICE, "Program started by User %d",pw->pw_name);
    }
    else
        syslog (LOG_NOTICE, "Program started by User %d", getuid ());


    syslog (LOG_INFO, "Program started with parametrs %s",Arg );





    closelog ();
}