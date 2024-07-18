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
   
   //Parte in comune.
   Input(rank);

   int sender_rank=0; //Numero estratto a sorte. E' il nodo che invia la comunicazione.
   int receiver_rank=0; //Numero estratto a sorte. E' il nodo che riceve la comunicazione.

   cout << "Sono il nodo " << rank << endl;
 
   if(rank==0){ //Il nodo 0 fa le estrazioni.
      receiver_rank = int(size*rnd.Rannyu()); //Il primo estratto e' sempre uguale per tutti i nodi.
      receiver_rank = int(size*rnd.Rannyu());
      sender_rank = int(size*rnd.Rannyu());

      while(receiver_rank == sender_rank){ //Vado avanti ad estrarre finche' non trovo due nodi diversi.
         receiver_rank = int(size*rnd.Rannyu()); //Estraggo un numero tra 0 e rank-1.
      }

      cout << "Sono il nodo " << rank << " ho estratto " << sender_rank << " e " << receiver_rank << endl;

      //Una volta che il nodo 0 ha deciso chi saranno sender e receiver, lo comunica al gruppo dei nodi.
      //Indirizzo della variabile che mando, quante sono, tipo, chi lo invia, gruppo dei destinatari.
      MPI_Bcast(&sender_rank, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);  
      MPI_Bcast(&receiver_rank, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);  
   }

   else{
      cout << "Sono il nodo " << rank << " e ho ricevuto " << receiver_rank << " e " << sender_rank << endl; 
   }

   /*if(rank==sender_rank){ //Se io sono il nodo che invia.
      cout << "Sono il nodo " << rank << " e sono sender rank!" << endl;
   }

   else if(rank==receiver_rank){ //Se io sono il nodo che riceve.
      cout << "Sono il nodo " << rank << " e sono receiver rank!" << endl;
   }

   else{ //Tutti gli altri.
      cout << "Sono il nodo " << rank << " e non ho nessun compito." << endl;
   }*/

   rnd.SaveSeed();   
  
   MPI_Finalize(); //Funzione che specifica che il lavoro in parallelo è finito.   
   //Bisogna sempre specificare l'inizio con Init e la fine con Finalize del processo in parallelo!   

   return 0;
}


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
