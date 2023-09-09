/*
 *----------------------------------------
 * UNIVERSIDAD DEL VALLE DE GUATEMALA
 * CC3086 - Programacion de Microprocesadores
 * Modificado por: KB
 * Fecha: 2023/09/03
 * Referencias:
 * "Predicates" - The C++ Standard Library (Josuttis)
 * pp.226-227
 * "Shared Objects and Synchronization" - The Art of 
 * Multiprocessor Programming (Herlihy & Shavit)
 * pp.3-5
 * 
 * primes3.cpp - V3.2023
 * ---------------------------------------
 * Imprime los números primos en un rango de 10^n
 * Implementación simplista usando pthreads
 * ----------------------------------------
 */

#include <iostream>     //cout, cin, endl
#include <cmath> 		//pow
#include <pthread.h>	//pthread_
#include <ctime>		//time_t, clock, difftime	

using namespace std;

#define THREADS 4

//Prototipo función identificadora
bool isPrime(int number);

//Prototipo rutina para hilos
void *primeTest (void *thisNumber);

/*
 * MAIN
 */
int main()
{
	int exponente, numThreads = THREADS;

    // Contadores para el reloj
	clock_t start_t, end_t;
	double total_t;
	
	cout<<"-----------------------------------------------\n";
	cout<<"Impresión de los numeros primos menores a 10^n\n";
	cout<<"-----------------------------------------------\n";
	cout<<"Ingrese el valor del exponente para base 10: ";

    //Lectura desde stdin de un valor segun el tipo de la variable destino
	cin>>exponente;
	
	cout<<"Ingrese el numero de threads: ";
	cin>>numThreads;
	
	long limit = pow(10,exponente);
	
	cout<<"Buscando primos entre 0 y "<<limit<<"...";
	cout<<endl;
	
	long i = 0;
	long j = 0; 		//Global
	int rc;
	
    //Definicion del objeto pthread
	pthread_t threadID[numThreads];
	
    //Contador de inicio en ticks de reloj
	start_t = clock();
	
    //Recoremos todo el dominio de numeros 10^n
	for(i=0; i<limit;)
	{
		//Asignamos alternamente numeros a cada hilo
        for(j = 0; j<numThreads; j++)	
		{
			rc = pthread_create(&threadID[j],NULL,primeTest,(void *)i);
			if(rc)
			{
				cout<<"No se pudo crear el hilo "<<j<<endl;
				exit(-1);
			}
			i++;
		}
	}
	
	for(j = 0; j<numThreads; j++)	
	{
		rc = pthread_join(threadID[j],NULL);
		if(rc)
		{
			cout<<"No se pudo unir el hilo "<<j<<endl;
			exit(-1);
		}
	}
	
	end_t = clock();
	total_t = difftime(end_t,start_t)/CLOCKS_PER_SEC;
	cout<<"Total time: "<<total_t<<endl;

	pthread_exit(NULL);
	
	return 0;
}

/*
 * primeTest
 */
void *primeTest (void *thisNumber)
{

	long intNumber = (long)thisNumber;

    //Acciones solamente si el numero es primo
	if(isPrime(intNumber))
	{
		cout<<intNumber<<" from thread num: "<<pthread_self()<<endl;
	}
	pthread_exit(NULL);
}

/*
 * isPrime - function
 */
bool isPrime (int number)
{
	if (number == 0 || number == 1)
	{
		return false;
	}
	int divisor;
	for (divisor = number/2; number%divisor != 0; --divisor)
	{
		; //Algoritmo rapido para encontrar el residuo de un primo
	}
	if (divisor != 1)
	{
		return false;  //El residuo <> 1 si el numero NO es primo
	}
	else
	{
		return true;  //El residuo es 1 si el numero es primo
	}
}