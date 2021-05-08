CC=gcc 
CFLAGS=-Wpedantic -std=gnu99 -Wall -g

all: FSreport

FSreport: FSreport.c FSreport.h
	$(CC) $(CFLAGS) FSreport.c -o FSreport

FSreport.o: FSreport.c FSreport.h
	$(CC) $(CFLAGS) -c FSreport.c -o FSreport.o
	
clean:
	rm -f *.o FSreport drawDir
