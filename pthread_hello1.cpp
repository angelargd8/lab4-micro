/*
*------------------------------------------
* pthread_hello.cpp
* -----------------------------------------
* UNIVERSIDAD DEL VALLE DE GUATEMALA
* FACULTAD DE INGENIERÍA
* DEPARTAMENTO DE CIENCIA DE LA COMPUTACIÓN

* CC3086 - Programacion de Microprocesadores
*
*------------------------------------------
* Descripción: demostracion de operaciones 
* básicas para creación de hilos posix
*------------------------------------------
*/
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <iostream>
#include <math.h>

#define NTHREADS 100

/* -----------------------------------------------------------
** Subrutina usada por el pthread, luego de crearse.
** Se debe declarar con punteros, debido a que los threads
** emplean referencias de memoria.  La información pasa por 
** referencia indirecta
** ---------------------------------------------------------*/

void *say_hello(void *threadNumber) {
	// Declarar la variable local que recibe el parametro
	long tID;
	// Casting del parametro tipo void a la variable local tipo long
  tID = (long)threadNumber;
  double raiz = sqrt(tID);
  printf("Hello thread: %ld", tID);
  printf(" la raiz es: %f\n", raiz);
  pthread_exit(NULL);
}
/* -----------------------------------------------------------
** Programa principal
** ---------------------------------------------------------*/
int main() {
  int rc;
  long i;
  // Declarar una variable tipo pthread_t
  pthread_t tid[NTHREADS];
  
  // Recomendable: declarar tambien un atributo tipo pthread_attr_t
  pthread_attr_t attr;
  
  // Al usar atributos, inicializar el objeto
  pthread_attr_init(&attr);
  // Modificar el atributo especifico en este caso el atributo setdetachstate = JOINABLE
  pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

  for (i=0; i<NTHREADS; i++) {
    //printf("Main: creating thread %ld\n", i);
    
	/* -----------------------------------------------------------
	** Al crear un hilo, se pasan los  siguientes parametros:	
	** el valor del ID, el valor del atributo , la subrutina a
	** ejecutar por el hilo y el puntero para el parametro de 
	** la subrutina:
	**
	** pthread_create(&id,NULL,subrutine,(void *)parameter
	** ---------------------------------------------------------*/
	rc = pthread_create(&tid[i], &attr, say_hello, (void *)i); 
  //se usa para almacenar el codigo de retorno de las funciones
	

	// La variable rc recibe errores en formato entero
    if (rc) {              
      printf("ERROR; return code from pthread_create() is %d\n", rc);
      exit(-1);
    }

   
  }
  
  for (i=0; i<NTHREADS; i++) {
       // Esperar a que cada thread termine en orden
    rc = pthread_join(tid[i], NULL);
    if (rc) {
      printf("ERROR; return code from pthread_join() is %d\n", rc);
      exit(-1);
    }
      
  }
  printf("Main: program completed. Exiting.\n");

  // Liberar espacios de memoria utilizados para atributo
  pthread_attr_destroy(&attr);
  pthread_exit(NULL);

}