/*
   This is a complete graph generator
   
   usage:
     spcomp xxx n cmin cmax issym seed
     where xxx xxx is the algorithm output filename, e.g. real_cases.txt
           n is the number of vertexs on complete graph
           cmin is the minimum of cost
	   cmax is the maximum of cost
           issym direct(0) or nondirect(1)
	   seed is random seed
	      
   output files:
     xxx

   compile:
     gcc -o spcomp spcomp.c

*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<math.h>

/* Random number generator                                    */
/* Before the first use of this function, you have to set the */
/* random number seed.  This seed can be any integer.         */
/* If the parameter seed is not 0, set the seed.  If it is 0, */
/* generate and return a random number between 0 and 1        */
double MyRand(int seed)
{
  if (seed != 0) srand(seed);
  return(rand()/(double)RAND_MAX);
}

void main( int argc , char *argv[] ){

  int n ; 
  char xxx[300] ;
  int cmin , cmax , seed , issym ;
  int arc ;
  int i , j , cost , range;

/*
  printf ("\n Enter xxx : " ) ;
  scanf ("%s" , &xxx ) ;

  printf ("\n Enter n : " ) ;
  scanf ("%d" , &n ) ; 

  printf ("\n Enter cmin : " ) ;
  scanf ("%d" , &cmin ) ;

  printf ("\n Enter cmax : " ) ;
  scanf ("%d" , &cmax ) ;

  printf ("\n Enter issym : " ) ;
  scanf ("%d" , &issym ) ;

  printf ("\n Enter seed : " ) ;
  scanf ("%d" , &seed ) ;

  printf ( "\n\n\n\n\n" ) ;
*/
  strcpy( xxx , argv[1] ) ; 
  n = atoi( argv[2] ) ; 
  cmin = atoi( argv[3] ) ; 
  cmax = atoi( argv[4] ) ;
  issym = atoi( argv[5] ) ;
  seed = atoi( argv[6] ) ; 

  MyRand( seed );

  arc = n * (n-1)  ;

  range = cmax - cmin ;

  if ( n > 1 ){
		
    ////////////data information////////////

    printf ("c Complete graph network with %d nodes %d arcs \n", n , arc ) ;
    printf ("c by.jphf\n") ;
    printf ("t %s\n",xxx) ; 
    printf ("p sp	%d	%d\n" , n , arc ) ; 
    printf ("n %d\n" , n ) ;

    /////////////////////////////////////////

    for ( i = 1 ; i <= n ; i++ ){
      for ( j = i ; j <= n ; j++ ){

	if ( j == i ) continue ;
				
	cost = cmin + (int)( MyRand(0) * range ) ; 

	printf ( "a   %d    %d    %d.000000 \n" , i , j , cost ) ;

	if( issym == 0 ){
	  cost = cmin + (int)( MyRand(0) * range ) ;
	}
				
	printf ( "a   %d    %d    %d.000000 \n" , j , i , cost ) ;
	
      }

    }

  }
  else {
    printf ("It can`t be run with 1 node or (range < 0) \n") ; 
  }


}


