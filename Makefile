CC = gcc -Wall

creche: creche.c
	$(CC) creche.c -o creche -lpthread

clean:
	rm -f creche

