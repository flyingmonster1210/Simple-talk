run: main
	./main

main: main.o list.o general.o printer.o reader.o receiver.o sender.o
	gcc -std=c99 main.o list.o general.o printer.o reader.o receiver.o sender.o -o main -lpthread 

main.o: main.c general.h
	gcc -std=c99 -c main.c

list.o: list.c list.h
	gcc -std=c99 -c list.c

general.o: general.c general.h printer.h reader.h sender.h receiver.h list.h
	gcc -std=c99 -c general.c

printer.o: printer.c printer.h list.h
	gcc -std=c99 -c printer.c

reader.o: reader.c reader.h list.h sender.h
	gcc -std=c99 -c reader.c

sender.o: sender.c sender.h list.h
	gcc -std=c99 -c sender.c

receiver.o: receiver.c receiver.h list.h general.h printer.h
	gcc -std=c99 -c receiver.c

clean:
	rm main main.o list.o general.o printer.o reader.o receiver.o sender.o


