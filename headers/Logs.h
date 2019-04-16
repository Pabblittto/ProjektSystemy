#ifndef LOGS_H
#define LOGS_H
void startLog(char *Arg[],int ArgNum);
void copyLog(char *filename, char *directory);
void copyDirLog(char *directory);
void deleteLog(char *filename, char type, char *path);
void endLog();
void DeamonState(int stan);
#endif