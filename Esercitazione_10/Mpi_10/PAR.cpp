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
   
   MPI_Comm_size(MPI_COMM_WORLD, &size); //Chiamata comune per dire quanti nodi partecipano al lavoro in parallelo.   
   MPI_Comm_rank(MPI_COMM_WORLD, &rank); //Chiamata comune per dire che nodo sono io nel lavoro in parallelo.   

   Input(rank);
   
   int mesg; //Messaggio, variabile che vorrei ridurre.
   int irecv; //Variabile dove inserisco il risultato ridotto.

   mesg = rank;
   cout << "Sono il nodo " << rank << " e comunico il messaggio = " << mesg << endl;  

   //La funzione Reduce svolge delle operazioni, come somma, prodotto o minimo, e invia i risultati da un nodo agli altri.
   //Buffer dei dati che vuoi ridurre, buffer dei dati ridotti, numero di elementi nel buffer per nodo, tipo di dato, 
   //operazione che vuoi svolgere per ridurre, nodo che riceve il risultato ridotto, gruppo dei nodi coinvolti nel processo.   
   MPI_Reduce(&mesg, &irecv, 1, MPI_INTEGER, MPI_MIN, 0, MPI_COMM_WORLD); 

   //Stampa i risultati solo il nodo = 0.
   if(rank==0){
      cout << "Minimo dei rank: " << irecv << endl;   
   } 

   //0 comunica a tutti gli altri nodi chi e' il massimo.
   //Indirizzo della variabile che mando, quante sono, tipo, chi lo invia, gruppo dei destinatari.
   MPI_Bcast(&irecv, 1, MPI_INTEGER, 0, MPI_COMM_WORLD); 

   //Tutti i nodi dicono che cosa hanno ricevuto.
   cout << "Sono " << rank << " e ho ricevuto il minimo di tutti = " << irecv << endl;

   if(mesg==irecv){ //Se tu sei nodo il cui messaggio personale e' uguale al massimo inviato a tutti, dillo.
      cout << "Sono il nodo " << rank << " e questo era il mio messaggio per tutti!" << endl;
   }
   
   rnd.SaveSeed();   
  
   MPI_Finalize(); //Funzione che specifica che il lavoro in parallelo è finito.   
   //Bisogna sempre specificare l'inizio con Init e la fine con Finalize del processo in parallelo!   

   return 0;
}

  /*Esempio di output per 4 nodi.
   Sono il nodo 0 e comunico il messaggio = 0
   Sono il nodo 3 e comunico il messaggio = 3
   Sono il nodo 2 e comunico il messaggio = 2
   Sono il nodo 1 e comunico il messaggio = 1
   Minimo dei rank: 0
   Sono 0 e ho ricevuto il minimo di tutti = 0
   Sono 1 e ho ricevuto il minimo di tutti = 0
   Sono 2 e ho ricevuto il minimo di tutti = 0
   Sono 3 e ho ricevuto il minimo di tutti = 0 
   Sono il nodo 0 e questo era il mio messaggio per tutti!*/


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
