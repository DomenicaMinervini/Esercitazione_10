Prima di compilare ed eseguire il codice in parallelo digitare sul terminale: 

	module load mpi/mpich-x86_64
	
In questo modo si carica il modulo che contiene la libreria MPI. 
Senza questa istruzione il compilatore non riconosce la libreria MPI.


Per la cartella Esercizio_10 valgono le seguenti istruzioni per:

	compilare: make

	eseguire:  make run
	
	cancellare i file prodotti: make clean
	
L'istruzione make run e' fatta cosi':

	mpiexec -np 10 ./TSP.exe

essendo: mpiexec = comando esecutore della libreria MPI, -np = istruzione per comunicare 
il numero di nodi che partecipera' al processo in parallelo, 10 = numero di nodi usati, 
./TSP.exe = nome eseguibile. Ho usato 10 dei 12 nodi del processore del computer 
del laboratorio di calcolo. Modificare questa quantita' in base al numero dei nodi 
che si vuole/può usare a seconda di quanti nodi (core) ha il processore del computer. 
Se il processore ha n nodi, la quantita' ideale di nodi da usare e' al massimo n-1.

Il codice risolve cinque problemi: sette citta', sulla circonferenza, nel quadrato, 
capitali d'America, capoluoghi d'Italia. Prima di eseguire il codice, modificare nei file 
TSP.h e TSP.cpp i commenti relativi al problema che si vuole risolvere. Di sicuro non e' 
il massimo della comodita' per chi usa il codice ma non ho potuto fare altrimenti.

I valori dim (numero di citta') e generations (numero di generazioni) sono le dimensioni 
di vettori e devono essere dichiarati come const int. I valori di tipo const sono costanti 
e devono essere dichiarati subito, in modo che non siano modificabili durante l'esecuzione. 
Di conseguenza, non si possono dichiarare in un secondo momento da terminale ma solo nel file.h. 

Nel file TSP.cpp modificare il commento relativo a quali file di input con le label e con le coordinate 
si vuole usare in base al problema selezionato. I dati da caricare si chiamano Label_ e Coord_ .
Copiare i file di dati del problema scelto dalla cartella Dati nella cartella di lavoro Esercizio_10.


Nella cartella Mpi_10 si trovano alcune funzioni scritte prima di essere inserite nel codice 
più ampio TSP. Queste funzioni riguardano l'uso della libreria MPI, quindi la parallelizzazione. 

Nella cartella Mpi_10, Random_10 si trova il materiale propedeutico per la stesura
del codice finale TSP. La cartella che contiene il codice finale per l'esame e' Esercizio_10.
	

