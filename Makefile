CC = cc
LIBS = -L/usr/local/lib -L/usr/local/mysql/lib -lmysqlclient
INCLUDE = -I /usr/local/include/ -I /usr/local/mysql/include/

Test: ICS491codeex.o
	$(CC) $(INCLUDE) -o Test ICS491codeex.o $(LIBS)

ICS491codeex.o: ICS491codeex.c
	$(CC) $(INCLUDE) -c ICS491codeex.c

clean:
	rm -f *.o *.d Test
	make