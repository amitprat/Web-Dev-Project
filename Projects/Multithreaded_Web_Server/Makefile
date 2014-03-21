all: project

project: main.o utility.o thread.o
	gcc main.o utility.o thread.o -lpthread -g -o project

thread.o: thread.c
	gcc -c -g thread.c
	
main.o: main.c
	gcc -c -g main.c

utility.o: utility.c
	gcc -c -g utility.c

clean:
	rm -rf *o project
