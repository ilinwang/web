/*
   This is a sun graph generator

   usage:
     spsun xxx n l cmin cmax issym seed
     where xxx is the algorithm output filename, e.g. real_cases.txt
           n is the number of vertexs on inner complete graph
           l is the number of layers out of inner graph
           cmin is the minimum of cost
           cmax is the maximum of cost
	   issym direct(0) or nondirect(1) 
           seed is random seed

   output files:
     xxx

   compile:
     gcc -o spsun spsun.c

*/


#include <stdio.h>
#include <stdlib.h>
#include <time.h>


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

  int n , l ;
  int cmin , cmax ;

  char xxx[256] ; 

  long int arc ;

  int i , j , cost , range , seed , issym ;
  int node , layer ;


  //FILE *f ; 

  //////////////data input/////////////
  /*
  printf ("\n") ; 
  printf ("\n Enter xxx : " ) ; 
  scanf ("%s" , &xxx ) ;

  printf ("\n Enter n : " ) ;
  scanf ("%d" , &n ) ; 

  printf ("\n Enter L : " ) ;
  scanf ("%d" , &l ) ;

  printf ("\n Enter cmin : " ) ;
  scanf ("%d" , &cmin ) ;

  printf ("\n Enter cmax : " ) ;
  scanf ("%d" , &cmax ) ;

  printf ("\n Enter issym : " ) ;
  scanf ("%d" , &issym ) ;

  printf ("\n Enter seed : " ) ;
  scanf ("%d" , &seed ) ;
  
  printf ( "\n\n\n\n\n" ) ;

  ////////////////////////////////////
  */

  strcpy( xxx , argv[1] ) ;
  n = atoi( argv[2] ) ; 
  l = atoi( argv[3] ) ;
  cmin = atoi( argv[4] ) ; 
  cmax = atoi( argv[5] ) ;
  issym = atoi( argv[6] ) ;
  seed = atoi( argv[7] ) ; 

  MyRand( seed );
  
  node = n * ( 1 + l ) ; 

  arc = n * ( n - 1 ) + ( 2 * l * n ) ;

  range = cmax - cmin ;

  if ( 1 ){
	
    //f = fopen ( xxx , "w" );
		
    ////////////data information////////////

    printf ("c Sun graph network with %d nodes %d arcs \n", node , arc ) ;
    //fprintf ( f ,"c Sun graph network with %d nodes %d arcs \n" , node , arc ) ;

    printf ("c by.jphf\n") ;
    //fprintf ( f , "c by.jphf\n") ;

    printf ("t %s \n", xxx ) ; 
    //fprintf ( f , "t %s \n", xxx ) ;
		
    printf ("p sp	%d	%d\n" , node , arc ) ; 
    //fprintf ( f , "p sp	%d	%d\n" , node , arc ) ; 

    printf ("n %d\n" , node ) ;
    //fprintf ( f , "n %d\n" , node ) ;

    /////////////////////////////////////////

    for ( i = 1 ; i <= n ; i++ ){
      for ( j = 0 ; j < l ; j++ ){
	
	layer = i + ( j * n ) - 1 ;
				
	cost = cmin + (int)( MyRand(0) * range ) ; 
	printf ( "a   %d    %d    %d.000000 \n" , node - layer , node - (layer + n) , cost ) ;
	//fprintf ( f , "a   %d    %d    %d.000000 \n" , node - layer , node - (layer + n) , cost ) ;
				
	if( issym == 0 ){
	  cost = cmin + (int)( MyRand(0) * range ) ; 
	}

	printf ( "a   %d    %d    %d.000000 \n" , node - (layer + n) , node - layer , cost ) ;
	//fprintf ( f , "a   %d    %d    %d.000000 \n" , node - (layer + n) , node - layer , cost ) ;
      }

      for ( j = i ; j <= n ; j++ ){

	if ( j == i ) continue ;
				
	cost = cmin + (int)( MyRand(0) * range ) ; 
	printf ( "a   %d    %d    %d.000000 \n" , node - i + 1 , node - j + 1 , cost ) ;
	//fprintf ( f , "a   %d    %d    %d.000000 \n" , node - i + 1 , node - j + 1 , cost ) ;

	if( issym == 0 ){
	  cost != cmin + (int)( MyRand(0) * range ) ;
	}

	printf ( "a   %d    %d    %d.000000 \n" , node - j + 1 , node - i + 1 , cost ) ;

	//fprintf ( f , "a   %d    %d    %d.000000 \n" , node - j + 1 , node - i + 1 , cost ) ;

      }

    }

    //fclose(f) ; 
  }
  else {
    printf ("It can`t be run with (range < 0) \n") ; 
  }


}

