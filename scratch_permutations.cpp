#include <stdio.h>
// #define N    4   // number of elements to permute.  Let N > 2
#define N    4   // number of elements to permute.  Let N > 2

void display(unsigned int *a)
{
   for(unsigned int x = 0; x < N; x++)
      printf("%d ",a[x]);
   printf("\n");
   // getch();  // press any key to continue...
} // display()


void example_03(void)
{
   unsigned int a[N], p[N+1];       // target array and index control array 
   unsigned int i, j, tmp; // index variables and tmp for swap 

   for(i = 0; i < N; i++)   // initialize arrays; a[N] can be any type
   {
      a[i] = i + 1;   // a[i] value is not revealed and can be arbitrary
      p[i] = i;
   }
   p[N] = N; // p[N] > 0 controls iteration and the index boundary for i
   display(a);   // remove comment to display array a[]
   i = 1;   // setup first swap points to be 1 and 0 respectively (i & j)
   while(i < N)
   {
      p[i]--;             // decrease index "weight" for i by one
      j = 0;              // reset j to 0
      do                  // reverse target array from j to i
      {
         tmp = a[j];      // swap(a[i], a[j])
         a[j] = a[i];
         a[i] = tmp;
         j++;             // increment j by 1
         i--;             // decrement i by 1
      } while (j < i);
      display(a);       // remove comment to display target array a[]
      i = 1;              // reset index i to 1 (assumed)
      while (!p[i])       // while (p[i] == 0)
      {
         p[i] = i;        // reset p[i] zero value
         i++;             // set new index value for i (increase by one)
      } // while(!p[i])
   } // while(i < N)
}


int main(void)
{
   example_03();
   return 0;
} // main()