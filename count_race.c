/*The below code has a race condition and results in a wrong result!!
 */
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <time.h>
// to compile: gcc -g -pthread count_race.c -o count_race
// .\count_race.exe
// creating a global variables
int ThreadCount;
long *array;
long size;
long iterations = 0;
long count = 0;
// Here each thread has it's own local variables while executing this function.
void *ThreadsExecution(void *rank)
{
   long my_rank = (long)rank;
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
}
void main()
{
   clock_t start, end;
   printf("Enter of size of Array: ");
   scanf("%ld", &size);
   printf("Enter the number of threads: ");
   scanf("%ld", &ThreadCount);
   srand(time(0));
   start = clock();
   for (int i = 0; i < 100; ++i)
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
   printf("\nThe numbers below are incorrect due to the race condition between threads\n");
   printf("Number of Threads: %d\n", ThreadCount);
   printf("Time Taken: %f\n", (double)(end - start) / (CLOCKS_PER_SEC * 1.0));
   printf("count of 1's= %d\n", count);
   printf("Iterations = %d\n", iterations);
}
