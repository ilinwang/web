/*
   This is a flower graph generator
   
   usage:
     spflower xxx n1 n2 d2 c1min c1max c2min c2max issym seed
     where xxx is the algorithm output filename, e.g. real_cases.txt
           n1 is the number of vertexs on inner complete graph
           n2 is the number of vertexs on outer graph
           d2 is the degree of vertexs on outer graph
           c1min is the minimum edge between n1  
           c1max is the maximum edge between n1
           c2min is the minimum edge between n1 and n2 
           c2max is the maximum edge between n1 and n2
	   issym direct(0) or nondirect(1)
           seed is random seed
	   
   output files:
     xxx
	   
   compile:
     gcc -o spflower spflower.c
   
*/



#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

  char xxx[256] ; 
  int n1 , n2 , d2 ;
  int c1min , c1max , c2min , c2max , seed , issym ;

  int n , m ;
	
  int i , j , k , cost , choose , range1 , range2 ;

  //FILE *f ; 

  int *temp ; 
  int p , same ; 

  //////////////////////////////
  /*
  printf ("\n") ; 
  printf ("\n Enter xxx : " ) ; 
  scanf ("%s" , &xxx ) ;

  printf ("\n Enter n1 : " ) ;
  scanf ("%d" , &n1 ) ; 

  printf ("\n Enter n2 : " ) ;
  scanf ("%d" , &n2 ) ;

  printf ("\n Enter d2 : " ) ;
  scanf ("%d" , &d2 ) ;

  printf ("\n Enter c1min : " ) ;
  scanf ("%d" , &c1min ) ;

  printf ("\n Enter c1max : " ) ;
  scanf ("%d" , &c1max ) ;
  
  printf ("\n Enter c2min : " ) ;
  scanf ("%d" , &c2min ) ;

  printf ("\n Enter c2max : " ) ;
  scanf ("%d" , &c2max ) ;

  printf ("\n Enter issym : " ) ;
  scanf ("%d" , &issym ) ;

  printf ("\n Enter seed : " ) ;
  scanf ("%d" , &seed ) ;

  printf ( "\n\n\n\n\n" ) ;
	
  //////////////////////////////////
  */

  strcpy( xxx , argv[1] ) ;
  n1 = atoi( argv[2] ) ;
  n2 = atoi( argv[3] ) ;
  d2 = atoi( argv[4] ) ; 
  c1min = atoi( argv[5] ) ; 
  c1max = atoi( argv[6] ) ;
  c2min = atoi( argv[7] ) ;
  c2max = atoi( argv[8] ) ; 
  issym = atoi( argv[9] ) ;
  seed = atoi( argv[10] ) ; 
  
  temp = (int*) calloc( d2+1 , sizeof(int) ) ; 
  
  MyRand( seed );

  n = n1 + n2 ; 
  m = n1*(n1 -1) + 2*n2*d2 ; 
  
  range1 = c1max - c1min ;
  range2 = c2max - c2min ;
  
  if ( n1>1 && n2>1 && d2<=n1 ){
    
    //f = fopen ( xxx , "w" );
		
    ////////////data information////////////

    printf ("c Flower graph network with %d nodes %d arcs \n", n , m ) ;
    //fprintf ( f ,"c Flower graph network with %d nodes %d arcs \n" , n , m ) ;

    printf ("c by.jphf\n") ;
    //fprintf ( f , "c by.jphf\n") ;

    printf ("t %s \n", xxx ) ; 
    //fprintf ( f , "t %s \n", xxx ) ;
		
    printf ("p sp	%d	%d\n" , n , m ) ; 
    //fprintf ( f , "p sp	%d	%d\n" , n , m ) ; 

    printf ("n %d\n" , n ) ;
    //fprintf ( f , "n %d\n" , n ) ;

    /////////////////////////////////////////

    // generate n1 complete graph 
    for ( i = n ; i > n2 ; i-- ){
      for ( j = i ; j > n2 ; j-- ){

	if ( j == i ) continue ;
				
	cost = c1min + (int)( MyRand(0) * range1 ) ; 

	printf ( "a   %d    %d    %d.000000 \n" , i , j , cost ) ;
	//fprintf ( f , "a   %d    %d    %d.000000 \n" , i , j , cost ) ;

	if( issym == 0 ){
	  cost = c1min + (int)( MyRand(0) * range1 ) ;
	}

	printf ( "a   %d    %d    %d.000000 \n" , j , i , cost ) ;
	//fprintf ( f , "a   %d    %d    %d.000000 \n" , j , i , cost ) ;

	
      }
    }

    // generate n2 part graph
    for( i = n2 ; i >= 1 ; i-- ){

      temp[0] = i ;
      p = 1 ;

      for( j = 0 ; j < d2 ; j++ ){

	do{
	  same = 0 ;
	  choose = n2 + 1 + (int)( MyRand(0) * n1 ) ;

	  for( k = 0 ; k < p ; k++ ){
	    if( choose == temp[k] ){
	      same = 1 ; 
	    }
	  }

	}while(same == 1 ) ;

	temp[p] = choose ;
	p++ ;


	cost = c2min + (int)( MyRand(0) * range2 ) ; 

	printf ( "a   %d    %d    %d.000000 \n" , i , choose , cost ) ;
	//fprintf ( f , "a   %d    %d    %d.000000 \n" , i , choose , cost ) ;

	if( issym == 0 ){
	  cost = c2min + (int)( MyRand(0) * range2 ) ;
	}

	printf ( "a   %d    %d    %d.000000 \n" , choose , i , cost ) ;
	//fprintf ( f , "a   %d    %d    %d.000000 \n" , choose , i , cost ) ;
      }
    }

    //fclose(f) ; 
  }
  else {
    printf ("ERROR!!\n1.n1&n2 must bigger than 1\n2.d2 should be less or equal than n2\n3.cost isn't 0 ") ; 
  }


}






