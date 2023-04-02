#include <pthread.h> //libreria hilos
#include <semaphore.h>//semaforo
#include <iostream>

#define MAX_CLIENTES 5 //maximo de clientes
void *cliente(void *num);
void *barbero(void *num);

//se definen los semaforos"
sem_t sillaEspera;
sem_t barberoSilla;
sem_t barberoDormir;
sem_t corteCabello;

int todosAtendidos = 0; //bandera que nos muestra que los clientes hayan sido atendidos por el momento 0

using namespace std;

int main (int argc, char ** argv) {
		
	pthread_t btid; //id barbero
	pthread_t tid[MAX_CLIENTES]; //clientes que hay en la barberia
	int i, nClientes, nSillas; 
	int number[MAX_CLIENTES]; 
	
	if(nClientes > MAX_CLIENTES)//asegura que nClientes no sea mayor a MAX CLIENTES  
	{
		cout << "El numero maximo de clientes es " << MAX_CLIENTES << endl; 
		exit(-1);
	}

	for(i=0; i<MAX_CLIENTES; i++)//asigna a el max de clientes un numero, sera la posicion que van a tener
	{
		number[i] = i;
	}
	
	sem_init(&sillaEspera, 0, MAX_CLIENTES*1000); 
	sem_init(&barberoSilla, 0, 1); 
	sem_init(&barberoDormir, 0, 0); 
	sem_init(&corteCabello, 0, 0);
	
    pthread_create(&btid, NULL, barbero, NULL); //se  crea hilo del barbero
    
    //pone a los clientes a trabajar
	for(i=0; i<MAX_CLIENTES; i++)
	{
		pthread_create(&tid[i], NULL, cliente, (void *)&number[i]);
	}
	
	for(i=0; i<MAX_CLIENTES; i++)
	{
		pthread_join(tid[i],NULL);
	}
	
	todosAtendidos = 1;
	sem_post(&barberoDormir);
	pthread_join(btid,NULL);	
	
    return 0;
}

void *cliente(void *number)
{
	int num = *(int *)number; 
	 
	cout << "El cliente "<< num+1 <<" llego a la barberia\n"; 
	sem_wait(&sillaEspera); //se va a sentar

	cout << "El cliente "<< num+1 <<" esta esperando\n"; 
	sem_wait(&barberoSilla); //espera a que el barbero se levante


	sem_post(&sillaEspera); //se levanta de la silla de espera

	cout << "El cliente "<< num+1 <<" despierta al barbero\n"; 
	sem_post(&barberoDormir); //levanta al barbero

	sem_wait(&corteCabello); //el barbero le corta el cabello

	sem_post(&barberoSilla); //deja la silla del barbero
	cout << "El cliente "<< num+1 <<" se va\n"; 
	
	return 0; 
}

void *barbero(void *num)
{
	while(!todosAtendidos){ //comprueba que todos los clientes hayan sido atendidos en caso de que si el barbero se dormira
		cout << "El barbero esta dormido\n"; 
		sem_wait(&barberoDormir); 
		if(!todosAtendidos){ //indica que el barbero esta haciendo un corte
			cout << "El barbero esta cortando el pelo\n";

			sem_post(&corteCabello); //termina el corte
			cout << "El barbero ya termino el corte de pelo\n"; 
		} 
	} 
	cout << "El barbero cerro la barberia\n"; //ya no tiene ningun cliente
	return 0; 
}


