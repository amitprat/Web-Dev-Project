all: proj1

proj1: amitprat_proj1.o utility.o cmd_iptr.o
	gcc amitprat_proj1.o utility.o cmd_iptr.o -o proj1

main.o: amitprat_proj1.c
	gcc -c -g amitprat_proj1.c

utility.o: utility.c
	gcc -c -g utility.c

cmd_iptr.o: cmd_iptr.c
	gcc -c -g cmd_iptr.c

clean:
	rm -rf *o proj1
