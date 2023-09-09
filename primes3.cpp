/*
 *----------------------------------------
 * UNIVERSIDAD DEL VALLE DE GUATEMALA
 * CC3086 - Programacion de Microprocesadores
 * Modificado por: Francis y Angela :D
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
#include <iostream>
#include <cmath>
#include <pthread.h>
#include <ctime>

using namespace std;

// Prototipo función identificadora
bool isPrime(int number);

// Prototipo rutina para hilos
void *primeTest(void *threadData);

struct ThreadData {
    long start;
    long end;
};

int main() {
    int exponente, numThreads;

    // Contadores para el reloj
    clock_t start_t, end_t;
    double total_t;

    cout << "-----------------------------------------------\n";
    cout << "Impresión de los numeros primos menores a 10^n\n";
    cout << "-----------------------------------------------\n";
    cout << "Ingrese el valor del exponente para base 10: ";

    // Lectura desde stdin de un valor según el tipo de la variable destino
    cin >> exponente;

    cout << "Ingrese el numero de threads: ";
    cin >> numThreads;

    long limit = pow(10, exponente);

    int rc;

    // Definición del objeto pthread
    pthread_t threadID[numThreads];
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);

    // Contador de inicio en ticks de reloj
    start_t = clock();

    // Calcula cuántos números manejará cada hilo
    long numPorThread = limit / numThreads;
    long sobrante = limit % numThreads;
    long posi = 0;

    ThreadData threadData[numThreads];

    for (int j = 0; j < numThreads; j++) {
        threadData[j].start = posi;
        threadData[j].end = posi + numPorThread + (j < sobrante ? 1 : 0);

        rc = pthread_create(&threadID[j], &attr, primeTest, &threadData[j]);

        cout << "Buscando primos entre " << threadData[j].start << " y " << threadData[j].end - 1 << "...";
        cout << endl;

        if (rc) {
            cout << "No se pudo crear el hilo " << j << endl;
            exit(-1);
        }

        posi = threadData[j].end;
    }

    for (int j = 0; j < numThreads; j++) {
        rc = pthread_join(threadID[j], NULL);
        if (rc) {
            cout << "No se pudo unir el hilo " << j << endl;
            exit(-1);
        }
    }

    end_t = clock();
    total_t = difftime(end_t, start_t) / CLOCKS_PER_SEC;
    cout << "Total time: " << total_t << " seconds\n";

    pthread_exit(NULL);

    return 0;
}

// Implementación de la función que se ejecutará en cada hilo
void *primeTest(void *threadDataPtr) {
    ThreadData *threadData = (ThreadData *)(threadDataPtr);

    for (long i = threadData->start; i < threadData->end; i++) {
        if (isPrime(i)) {
            cout << i << " from thread num: " << pthread_self() << "numeros "<<threadData->start <<threadData->end<<endl;
        }
    }

    pthread_exit(NULL);
}

// Función para verificar si un número es primo
bool isPrime(int number) {
    if (number <= 1) {
        return false;
    }

    for (int divisor = 2; divisor * divisor <= number; divisor++) {
        if (number % divisor == 0) {
            return false;
        }
    }

    return true;
}
