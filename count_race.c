/*The below code has a race condition and results in a wrong result!!
*/
#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>
#include<time.h>
// to compile: gcc -g -pthread count_race.c -o count_race
// .\count_race.exe
#define ThreadCount 1//change number of threads here
long*array; //creating a global array
long size;
long iterations=0;
long count=0;

void* ThreadsExecution(void* rank){
   long my_rank = (long) rank;
   long perthread = size/ThreadCount;
   long start = my_rank*perthread;
   long end = start+perthread;
   for(long i=start;i<end;++i){
      if(array[i]==1){
         ++count;
      }
      ++iterations;
   }  
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
   pthread_t T[ThreadCount];
   for(int i = 0;i<ThreadCount;++i){
      pthread_create(&T[i],NULL,&ThreadsExecution,(void*)i);
   }
   
   for(int i=0;i<ThreadCount;++i){
      pthread_join(T[i],NULL);
   }
   timetaken=clock();
   printf("The numbers below are incorrect due to the race condition between threads\n");
   printf("Number of Threads: %d\n",ThreadCount);
   printf("Time Taken: %f\n",(((double)timetaken/CLOCKS_PER_SEC*1.0)));
   printf("count of 1's= %d\n", count);
   printf("Iterations = %d\n",iterations);
}
