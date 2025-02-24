/*
   This is a web graph generator

   usage:
     spweb xxx x y cmin cmax issym seed
     where xxx is the algorithm output filename, e.g. real_cases.txt
           x is the number of layers
	   y is the number of lines
	   cmin is the minimum of cost
           cmax is the maximum of cost
	   issym direct(0) or nondirect(1)
           seed is random seed

   output files:
     xxx

   compile:
     gcc -o spweb spweb.c

*/


#include <stdio.h>
#include <stdlib.h>

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

  int x , y ;
  char xxx[256] ; 
  int cmin , cmax ;

  int n ; 
  int arc ;
	
  int i , j , cost , range , seed , issym ;
  int layer_l , layer_u ; 

  //FILE *f ; 

  //////////data input/////////////
  /*
  printf ("\n") ; 
  printf ("\n Enter xxx : " ) ; 
  scanf ("%s" , &xxx ) ;

  printf ("\n Enter x : " ) ;
  scanf ("%d" , &x ) ; 

  printf ("\n Enter y : " ) ;
  scanf ("%d" , &y ) ;

  printf ("\n Enter cmin : " ) ;
  scanf ("%d" , &cmin ) ;

  printf ("\n Enter cmax : " ) ;
  scanf ("%d" , &cmax ) ;

  printf ("\n Enter issym : " ) ;
  scanf ("%d" , &issym ) ;

  printf ("\n Enter seed : " ) ;
  scanf ("%d" , &seed ) ;
	
  printf ( "\n\n\n\n\n" ) ;
  /////////////////////////////////
  */

  strcpy( xxx , argv[1] ) ;
  x = atoi( argv[2] ) ;
  y = atoi( argv[3] ) ; 
  cmin = atoi( argv[4] ) ; 
  cmax = atoi( argv[5] ) ;
  issym = atoi( argv[6] ) ;
  seed = atoi( argv[7] ) ; 

  MyRand( seed ) ; 

  n = 1 + x * y ;
  arc = 4 * x * y   ;

  range = cmax - cmin ;

  if ( x >= 1 ){
	
    //f = fopen ( xxx , "w" );
		
    ////////////data information////////////

    printf ("c Spider graph network with %d nodes %d arcs \n", n , arc ) ;
    //fprintf ( f ,"c Spider graph network with %d nodes %d arcs \n" , n , arc ) ;

    printf ("c by.jphf\n") ;
    //fprintf ( f , "c by.jphf\n") ;

    printf ("t %s \n", xxx ) ; 
    //fprintf ( f , "t %s \n", xxx ) ;
		
    printf ("p sp	%d	%d\n" , n , arc ) ; 
    //fprintf ( f , "p sp	%d	%d\n" , n , arc ) ; 

    printf ("n %d\n" , n ) ;
    //fprintf ( f , "n %d\n" , n ) ;


    /////////////////From Node n link to out//////////////// 

    for ( i = n - 1 ; i >= n - x ; i -- ){
		
      cost = cmin + (int)( MyRand(0) * range ) ; 
      printf ( "a   %d	%d	%d.000000 \n" , n , i , cost ) ;
      //fprintf ( f , "a   %d	%d	%d.000000 \n" , n , i , cost ) ;
			
      if( issym == 0 ){
	cost = cmin + (int)( MyRand(0) * range ) ;
      }
			
      printf ( "a   %d	%d	%d.000000 \n" , i , n , cost ) ;
      //fprintf ( f , "a   %d	%d	%d.000000 \n" , i , n , cost ) ;

    }

    ///////link to near node 

    for ( i = 1 ; i <= y ; i++ ){	

      layer_l = x * ( y - i ) + 1  ;
      layer_u = x * ( y - i + 1) ; 

      for ( j = layer_l ; j <= layer_u ; j++ ){

	// link node to out
	if ( i < y ){
	  cost = cmin + (int)( MyRand(0) * range ) ; 
	  printf ( "a   %d	%d	%d.000000 \n" , j , j - x , cost ) ;
	  //fprintf ( f , "a   %d	%d	%d.000000 \n" , j , j - x , cost ) ;
	  
	  if( issym == 0 ){
	    cost = cmin + (int)( MyRand(0) * range ) ;
	  }

					
	  printf ( "a   %d	%d	%d.000000 \n" , j - x , j , cost ) ;
	  //fprintf ( f , "a   %d	%d	%d.000000 \n" , j - x , j , cost ) ;
	}

	if ( j == layer_u ){	//if node is the largest one on the layer
	  cost = cmin + (int)( MyRand(0) * range ) ; 
	  printf ( "a   %d	%d	%d.000000 \n" , j - x + 1 , j , cost ) ;
	  //fprintf ( f , "a   %d	%d	%d.000000 \n" , j - x + 1 , j , cost ) ;
					
	  if( issym == 0 ){
	    cost = cmin + (int)( MyRand(0) * range ) ;
	  }
	  
	  printf ( "a   %d	%d	%d.000000 \n" , j , j - x + 1 , cost ) ;
	  //fprintf ( f , "a   %d	%d	%d.000000 \n" , j , j - x + 1 , cost ) ;

	}
	else{
	  
	  cost = cmin + (int)( MyRand(0) * range ) ; 
	  printf ( "a   %d	%d	%d.000000 \n" , j , j + 1 , cost ) ;
	  //fprintf ( f , "a   %d	%d	%d.000000 \n" , j , j + 1 , cost ) ;

	  if( issym == 0 ){
	    cost = cmin + (int)( MyRand(0) * range ) ;
	  }
 
	  printf ( "a   %d	%d	%d.000000 \n" , j + 1 , j , cost ) ;
	  //fprintf ( f , "a   %d	%d	%d.000000 \n" , j + 1 , j , cost ) ;
	  
	}
	
      }
      
    }

    //fclose(f) ; 
  }
  else {
    printf ("It can`t be run with 1 node or (range < 0) \n") ; 
  }
  


}




