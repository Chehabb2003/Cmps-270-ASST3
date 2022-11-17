/*Here we used mutex to prevent the interleaving of instructions, namely the count of 1's in each array distributed
among each thread.
*/
#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>
#include<time.h>
// to compile: gcc -g -pthread count_mutex.c -o count_mutex
// .\count_mutex.exe
#define ThreadCount 4//change number of threads here
long*array; //creating a global array
long size;
long iterations=0;
long count=0;
pthread_mutex_t mutex;
void* ThreadsExecution(void* rank){
   long my_rank = (long) rank;
   long perthread = size/ThreadCount;
   long start = my_rank*perthread;
   long end = start+perthread;
   pthread_mutex_lock(&mutex);
   for(long i=start;i<end;++i){
      if(array[i]==1){
         ++count;
      }
      ++iterations;
   }
   pthread_mutex_unlock(&mutex);
}

void main (){
   size=100000000; // set size of array here
   srand(time(0));
   array = (long *)malloc(sizeof(long)*size);
   for(long i=0;i<size;++i){
      array[i] = rand()%6;
      }
   printf("\n");
   clock_t timetaken;
   pthread_mutex_init(&mutex,NULL);
   pthread_t T[ThreadCount];
   for(int i = 0;i<ThreadCount;++i){
      pthread_create(&T[i],NULL,&ThreadsExecution,(void*)i);
   }
   
   for(int i=0;i<ThreadCount;++i){
      pthread_join(T[i],NULL);
   }
   timetaken=clock();
   printf("The following count of 1's are correct, since we used mutex\n");
   printf("Number of Threads: %d\n",ThreadCount);
   printf("Time Taken: %f\n",(((double)timetaken/CLOCKS_PER_SEC*1.0)));
   printf("count of 1's= %d\n", count);
   printf("Iterations = %d\n",iterations);
}
