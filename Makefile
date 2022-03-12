
#Compiler to use
CC = gcc
CFLAGS = -lreadline

#recompile
rc: slick.c builtins/builtin.c /usr/bin/.ssrc handler.c
	$(CC) slick.c $(CFLAGS) -o /usr/bin/slick


#Generate binary file
slick:
	$(CC) slick.c $(CFLAGS) -o /usr/bin/slick
	sudo touch /usr/bin/.ssrc




