/*Here we used mutex to prevent the interleaving of instructions, namely the count of 1's in each array distributed
among each thread.
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
// to compile: gcc -g -pthread count_mutex.c -o count_mutex
// .\count_mutex.exe
// creating a global variables
int ThreadCount; // change number of threads here
long *array;     // creating a global array
long size;
long iterations = 0;
long count = 0;
pthread_mutex_t mutex;
// Here each thread has it's own local variables while executing this function.
   void *ThreadsExecution(void *rank)
{
   long my_rank = (long)rank;
   long perthread = size / ThreadCount;
   long start = my_rank * perthread;
   long end = start + perthread;
   pthread_mutex_lock(&mutex);
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
   pthread_mutex_unlock(&mutex);
}
void main()
{
   srand(time(0));
   clock_t start, end;
   printf("Enter the size of Array: ");
   scanf("%ld", &size);
   printf("Enter the number of threads: ");
   scanf("%ld", &ThreadCount);
   array = (long *)malloc(sizeof(long) * size);
   start = clock();
   for (int i = 0; i < 100; ++i)
   {
      for (long i = 0; i < size; ++i)
      {
         array[i] = rand() % 6;
      }
      pthread_mutex_init(&mutex, NULL);
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
   printf("\nThe following count of 1's are correct, since we used mutex\n");
   printf("Number of Threads: %d\n", ThreadCount);
   printf("Time Taken: %f\n", (double)(end - start) / (CLOCKS_PER_SEC * 1.0));
   printf("count of 1's using threads = %d\n", count);
   printf("Iterations = %d\n", iterations);
}
