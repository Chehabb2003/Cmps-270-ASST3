/*This module is a modification of count_race, but will deal with race conditions using private variables
At the end of the function call each thread will have it's own local number of iterations and counted 1's
and will be incremented to the private number of iterations and counted 1's.
*/
#include <stdio.h>
#include <pthread.h> 
#include <stdlib.h>
#include<time.h>
// to compile: gcc -g -pthread count_private.c -o count_private
// .\count_private.exe
#define ThreadCount 4//change number of threads here
long*array; //creating a global array
long size;
long iterations=0;
long privcount=0;
long priviterations=0;

void* ThreadsExecution(void* rank){ //Here each function has it's own local variables
   long my_rank = (long) rank;
   long perthread = size/ThreadCount;
   long iterations=0;
   long count=0;
   long start = my_rank*perthread;
   long end = start+perthread;
   for(long i=start;i<end;++i){
      if(array[i]==1){
          ++count;
      }
      ++iterations;
   }
   privcount+=count;
   priviterations+=iterations;
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
   printf("The below results are with fixing the Race Condition which must be correct\n");
   printf("Number of Threads: %d\n",ThreadCount);
   printf("Time Taken: %f\n",(((double)timetaken/CLOCKS_PER_SEC*1.0)));
   printf("count of 1's= %d\n", privcount);
   printf("Iterations = %d",priviterations);
}
