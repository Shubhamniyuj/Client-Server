#variable declaration :-
cc=gcc      
MAKE=make
RM =rm

#Flags
FLAG = -Wall

#targets .
all: Client.c Server.c
	$(cc) $(FLAG) -o Client Client.c	
	$(cc) $(FLAG) -o Server Server.c -pthread -lbsd
	
clean:Server Client
	$(RM) *.o
