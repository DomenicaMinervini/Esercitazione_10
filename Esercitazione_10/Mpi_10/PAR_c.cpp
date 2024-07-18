/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/

#include "PAR.h"
#include "mpi.h"

using namespace std;
 
int main (int argc, char *argv[]){

   //Questo è main random. Lo trasformo in modo da usarlo in parallelo.   
   //Dichiarare chi sono gli elementi che parteciperanno al processo prima di iniziare. 
   int size; //Numero di nodi che prendono parte al processo.
   int rank; //Che nodo sono io?

   MPI_Init(&argc, &argv); //Inizializzo con le referenze ai parametri passati da terminale.
   
   //MPI_COMM_WORLD è il gruppo dei nodi che partecipano al lavoro in parallelo. 
   //Così si specifica che si vuole svolgere questo tipo di lavoro.
   
   MPI_Comm_size(MPI_COMM_WORLD, &size); //Chiamata comune per dire quanti nodi partecipano al lavoro in parallelo.   
   MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Chiamata comune per dire che nodo sono io nel lavoro in parallelo.   

   MPI_Status stat; //Variabile che dice lo stato.
   
   Input(rank);

   int partner; //Numero estratto a sorte. E' il nodo a cui invio la comunicazione.
   partner = rnd.Rannyu(); //Il primo estratto e' sempre uguale per tutti i nodi.
   partner = rank; //Condizione per entrare nel ciclo while.

   while(rank == partner){ //Vado avanti ad estrarre finche' non trovo un nodo diverso da me.
      partner = int(size*rnd.Rannyu()); //Estraggo un numero tra 0 e rank-1.
   }

   //La variabile prima dello scambio è caso, quella dopo lo scambio è imesg.
   cout << "Prima. Sono il nodo " << rank << " e vorrei COMUNICARE con il nodo " << partner << endl;    
  
   int itag = 1; //Invio il numero estratto ad altri nodi.
   int imesg1 = rank;
   int imesg2 = rank;

   if(rank==1){
      MPI_Send(&imesg1, 1, MPI_INTEGER, 0, itag, MPI_COMM_WORLD);
      MPI_Recv(&imesg2, 1, MPI_INTEGER, 0, itag, MPI_COMM_WORLD, &stat);

      cout << "Dopo. Sono il nodo " << rank << " e ho INVIATO il numero " << imesg1 << endl;  
      cout << "Dopo. Sono il nodo " << rank << " e ho RICEVUTO il numero " << imesg2 << endl;   
   }
   
   else if(rank==0){
      MPI_Send(&imesg2, 1, MPI_INTEGER, 1, itag, MPI_COMM_WORLD);
      MPI_Recv(&imesg1, 1, MPI_INTEGER, 1, itag, MPI_COMM_WORLD, &stat);

      cout << "Dopo. Sono il nodo " << rank << " e ho INVIATO il numero " << imesg2 << endl;   
      cout << "Dopo. Sono il nodo " << rank << " e ho RICEVUTO il numero " << imesg1 << endl;  
   }  

   rnd.SaveSeed();   
  
   MPI_Finalize(); //Funzione che specifica che il lavoro in parallelo è finito.   
   //Bisogna sempre specificare l'inizio con Init e la fine con Finalize del processo in parallelo!   

   return 0;
}

  
   /*Elementi tra parentesi di Send:
   &imesg: Questo è l’indirizzo del buffer contenente i dati che desideri inviare.
   1: Rappresenta il numero di elementi nel buffer (nel mio caso, sto inviando un singolo elemento).
   MPI_INTEGER: Specifica il tipo di dato degli elementi nel buffer (in questo caso, un intero).
   0: È il rank del processo destinatario.
   itag: È l’etichetta (tag) associata al messaggio.
   MPI_COMM_WORLD: Questo è il comunicatore che collega tutti i processi MPI.*/  
     
   /*Elementi tra parentesi di Recv:
   &imesg: Questo è l’indirizzo del buffer in cui verranno memorizzati i dati ricevuti.
   1: Rappresenta il numero di elementi previsti nel buffer (nel mio caso, sto ricevendo un singolo intero).
   MPI_INTEGER: Specifica il tipo di dato degli elementi ricevuti (in questo caso, un intero).
   1: È il rank del processo mittente (il processo da cui mi aspetto di ricevere il messaggio).
   itag: È l’etichetta (tag) associata al messaggio (usata per il filtraggio dei messaggi).
   MPI_COMM_WORLD: Questo è il comunicatore che collega tutti i processi MPI.*/


void Input(int rank){

   //GENERATORE DI NUMERI CASUALI
   int p1, p2;
   ifstream Primes("Primes");
   if (Primes.is_open()){
     //Ripeto la riga Primes in base al numero di rank, così ogni nodo estrae una riga diversa.
     //Se tutti i nodi partono dalla stesso seme del generatore, ripetono la stessa sequenza.
     for(int i=0; i<rank; i++){
        Primes >> p1 >> p2;
     }
   } else cerr << "PROBLEM: Unable to open Primes" << endl;
   Primes.close();

   ifstream input("seed.in");
   string property;
   if (input.is_open()){
      while ( !input.eof() ){
         input >> property;
         if( property == "RANDOMSEED" ){
            input >> seed[0] >> seed[1] >> seed[2] >> seed[3];
            rnd.SetRandom(seed,p1,p2);
         }
      }
      input.close();
   } else cerr << "PROBLEM: Unable to open seed.in" << endl;
	
}

/*************************************************************
**************************************************************
    _/    _/  _/_/_/  _/       Numerical Simulation Laboratory
   _/_/  _/ _/       _/       Physics Department
  _/  _/_/    _/    _/       Universita' degli Studi di Milano
 _/    _/       _/ _/       Prof. D.E. Galli
_/    _/  _/_/_/  _/_/_/_/ email: Davide.Galli@unimi.it
**************************************************************
*************************************************************/
