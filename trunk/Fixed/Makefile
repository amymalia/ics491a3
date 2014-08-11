CC = cc
LIBS = -L/usr/local/lib -L/usr/local/mysql/lib -lmysqlclient
INCLUDE = -I /usr/local/include/ -I /usr/local/mysql/include/

Test: RevisedVersion.o
	$(CC) $(INCLUDE) -o Test RevisedVersion.o $(LIBS)

RevisedVersion.o: RevisedVersion.c
	$(CC) $(INCLUDE) -c RevisedVersion.c

clean:
	rm -f *.o *.d Test
	make