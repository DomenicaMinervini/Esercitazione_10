CC = mpicxx
CFLAGS = -Wall --std=c++11

PAR.exe : PAR.o random.o
	$(CC) random.o PAR.o -o PAR.exe
PAR.o : PAR.cpp PAR.o
	$(CC) -c PAR.cpp -o PAR.o $(CFLAGS)
random.o : random.cpp random.h
	$(CC) -c random.cpp -o random.o $(CFLAGS)
run :
	mpiexec -np 4 ./PAR.exe
clean :
	rm *.o PAR.exe seed.out	

