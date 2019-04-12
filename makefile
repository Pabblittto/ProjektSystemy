#dobra wiec to dziala tak, to na dole(Projekt) robi glowny program, a że główny program ma szalone includy to wymaga pośrednich pliczków
Projekt: main.c start.o Files.o
	gcc main.c ./exeFiles/start.o ./exeFiles/Files.o -g -o ./exeFiles/SynchDir
	rm ./exeFiles/*.o

#to na dole to jeden z posrednich pliczków, trzeba go zrobić z plik.h i plik.c
start.o: ./headers/Start.h ./source/Start.c
	gcc ./source/Start.c -c  -g -o ./exeFiles/start.o


Files.o: ./headers/Files.h ./source/Files.c
	gcc ./source/Files.c -c -g -o ./exeFiles/Files.o


# to tak dla przykładu jak by sie dodwałao kolejnego includa (na przykład cos.h i cos.c)to trzeba napisać dodatkowe dwie liniki:


#cos.o: ./headers/cos.h ./source/cos.c
#	gcc ./source/cos.c -c -o ./exeFiles/cos.o


#AAA do projektu(tego na samej górze trzeba dodać żeby wyszło coś takiego:


#Projekt: main.c start.o cos.o
#	gcc main.c ./exeFiles/start.o ./exeFiles/cos.o -o ./exeFiles/program

#później pliczki z rozszerzeniem .o można wywalić... chyba 