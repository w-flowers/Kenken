#include "../inc/kenken-basic.h"

int main()
{
   int available[36][2];

   for( int i = 0; i < 36; i++ )
   {
      available[i][0] = i/6;
      available[i][1] = i%6;
   }

   int sqr[2] = {6, 6};

   int dmy[4][2] = {{0}};

   random_available_sqr( available, sqr, dmy );

   return 0;
}

