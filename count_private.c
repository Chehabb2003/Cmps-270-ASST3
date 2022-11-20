/*This module is a modification of count_race, but will deal with race conditions using private variables
At the end of the function call each thread will have it's own local number of iterations and counted 1's
and will be incremented to the private number of iterations and counted 1's.
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
// to compile: gcc -g -pthread count_private.c -o count_private
// .\count_private.exe
// creating a global variables
int ThreadCount;
long *array;
long size;
long privcount = 0;
long priviterations = 0;
// Here each thread has it's own local variables while executing this function.
void *ThreadsExecution(void *rank)
{
   long my_rank = (long)rank;
   long iterations = 0;
   long count = 0;
   long perthread = size / ThreadCount;
   long start = my_rank * perthread;
   long end = start + perthread;
   if ((my_rank == ThreadCount - 1 && size % ThreadCount != 0))
   {
      for (long i = ((size / ThreadCount) * ThreadCount) - perthread; i < size; ++i)
      {
         if (array[i] == 1)
         {
            ++count;
         }
         ++iterations;
      }
   }

   else
   {
      for (long i = start; i < end; ++i)
      {
         if (array[i] == 1)
         {
            ++count;
         }
         ++iterations;
      }
   }
   privcount += count;
   priviterations += iterations;
}

void main()
{
   srand(time(0));
   printf("Enter of size of Array: ");
   scanf("%ld", &size);
   printf("Enter the number of threads: ");
   scanf("%ld", &ThreadCount);
   clock_t start, end;
   start = clock();
   for (int i = 0; i < 100; ++i) // using a for loop to run 100 times
   {
      array = (long *)malloc(sizeof(long) * size);
      for (long i = 0; i < size; ++i)
      {
         array[i] = rand() % 6;
      }
      pthread_t T[ThreadCount];
      for (int i = 0; i < ThreadCount; ++i)
      {
         pthread_create(&T[i], NULL, &ThreadsExecution, (void *)i);
      }

      for (int i = 0; i < ThreadCount; ++i)
      {
         pthread_join(T[i], NULL);
      }
   }
   end = clock();
   printf("\nThe below results are with fixing the Race Condition which must be correct\n");
   printf("Number of Threads: %ld\n", ThreadCount);
   printf("Time Taken: %f\n", (double)(end - start) / (CLOCKS_PER_SEC * 1.0));
   printf("count of 1's= %ld\n", privcount);
   printf("Iterations = %ld", priviterations);
}
