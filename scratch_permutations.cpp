#include <stdio.h>
#define N    4   // number of elements to permute.  Let N > 2

void display(unsigned int *a, unsigned int j, unsigned int i)            
{
   for(unsigned int x = 0; x < N; x++)
      printf("%d ",a[x]);
   printf("   swapped(%d, %d)\n", j, i);
//    getch();  // press any key to continue...
} // display()

void QuickPerm(void)
{   
   unsigned int a[N], p[N+1];
   unsigned int i, j, tmp; // Upper Index i; Lower Index j

   for(i = 0; i < N; i++)   // initialize arrays; a[N] can be any type
   {
      a[i] = i + 1;   // a[i] value is not revealed and can be arbitrary
      p[i] = i;
   }
   p[N] = N; // p[N] > 0 controls iteration and the index boundary for i
   //display(a, 0, 0);   // remove comment to display array a[]
   i = 1;   // setup first swap points to be 1 and 0 respectively (i & j)

   display(a, 0, i);
   while(i < N)
   {
      p[i]--;             // decrease index "weight" for i by one
      j = i % 2 * p[i];   // IF i is odd then j = p[i] otherwise j = 0
      tmp = a[j];         // swap(a[j], a[i])
      a[j] = a[i];
      a[i] = tmp;

    //   if (!(a[0] < a[N-1])){
        display(a, j, i); // remove comment to display target array a[]
    //   }

      i = 1;              // reset index i to 1 (assumed)
      while (!p[i])       // while (p[i] == 0)
      {
         p[i] = i;        // reset p[i] zero value
         i++;             // set new index value for i (increase by one)
      } // while(!p[i])
   } // while(i < N)
} // QuickPerm()




int main(void)
{
   QuickPerm();
   return 0;
} // main()