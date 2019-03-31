#include<stdio.h>
#include"headers/test.h"// tu doajesz headera i ditara siema 
#include"headers/Start.h"
#include"headers/Files.h"



int main(int ArgNum,char* Arg[] )
{// faza pierwsza- napisać program w mainie, który po jednym odpaleniu jest w stanie skopiować/usuwać
            // ogólnie zzynchronizować dwa foldery
            // trzeba ogarnąć logi do systemu Barto :3 
            // funkja, która potrafi skanować folder i robi liste plików + czas utworzenia tych plików ..... nooo ostro może być


CONFIG Values;
Values=CheckDirectories(ArgNum,Arg);
printf("Wartosci to :\n");
printf("%s pierwsze, %s drugie, %d czas,%d synchronizacja,%d rozmiar\n",(Values).FirstDir,(Values).SecondDir,(Values).time_wait,(Values).deepSynch,(Values).FileSize);

ScanDirectory(Values.FirstDir);


return 0;
}