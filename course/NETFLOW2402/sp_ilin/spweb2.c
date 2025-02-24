/*
   This is a web graph generator

   usage:
     spweb2 xxx x y c1min c1max c2min c2max rmin rmax seed
     where xxx is the algorithm output filename, e.g. real_cases.txt
           x is the number of out lines
	   y is the number of layers
	   c1min is the minimum cost of bone line
           c1max is the maximum cost of bone line
	   c2min is the minimum cost of edge link to near node on same layer
	   c2max is the maximum cost of edge link to near node on same layer
	   rmin is the minimum ratio of out line
	   rmax is the maximum ratio of out line
           seed is random seed

   output files:
     xxx

   compile:
     gcc -o spweb2 spweb2.c

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
   int c1min , c1max , c2min , c2max , rmin , rmax , seed ;

   int n ; 
   int arc ;
	
   int i , j , cost , ratio , range1 , range2 , range3 , temp , r1 , r ;
   int layer_l , layer_u ; 

   int *cost1 , *cost2 ; 

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

   printf ("\n Enter c1min : " ) ;
   scanf ("%d" , &c1min ) ;

   printf ("\n Enter c1max : " ) ;
   scanf ("%d" , &c1max ) ;

   printf ("\n Enter c2min : " ) ;
   scanf ("%d" , &c2min ) ;                                                      
   printf ("\n Enter c2max : " ) ;
   scanf ("%d" , &c2max ) ;

   printf ("\n Enter rmin : " ) ;
   scanf ("%d" , &rmin ) ;

   printf ("\n Enter rmax : " ) ;
   scanf ("%d" , &rmax ) ;
      
   printf ("\n Enter seed : " ) ;
   scanf ("%d" , &seed ) ;
	
   printf ( "\n\n\n\n\n" ) ;
   /////////////////////////////////
   */

   strcpy( xxx , argv[1] ) ;
   x = atoi( argv[2] ) ;
   y = atoi( argv[3] ) ; 
   c1min = atoi( argv[4] ) ; 
   c1max = atoi( argv[5] ) ;
   c2min = atoi( argv[6] ) ;
   c2max = atoi( argv[7] ) ;
   rmin = atoi( argv[8] ) ;
   rmax = atoi( argv[9] ) ;
   seed = atoi( argv[10] ) ; 

   MyRand( seed ) ; 

   n = 1 + x * y ;
   arc = 4 * x * y   ;

   range1 = c1max - c1min ;
   range2 = c2max - c2min ; 
   range3 = rmax - rmin ; 

   cost1 = (int*) calloc( x , sizeof( int ) ) ; 
   cost2 = (int*) calloc( x , sizeof( int ) ) ;

   for( i = 0 ; i < x ; i++ ){
      cost1[i] = c1min + (int)( MyRand(0) * range1 ) ;
      cost2[i] = c2min + (int)( MyRand(0) * range2 ) ;
   }

   r1 = 1 ;
   r = r1 ; 

   if ( x >= 1 && rmax > 0 ){
	
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

     for ( i = n - x ; i <= n - 1 ; i++ ){

       temp = (n-1) -  i ; 

       cost = r1 * cost1[temp] ; 

       printf ( "a   %d	%d	%d.000000 \n" , n , i , cost ) ;    
       //fprintf ( f , "a   %d	%d	%d.000000 \n" , n , i , cost ) ;
			
       printf ( "a   %d	%d	%d.000000 \n" , i , n , cost ) ;
       //fprintf ( f , "a   %d	%d	%d.000000 \n" , i , n , cost ) ;

     }

     ///////link to near node 
       
     for ( i = 1 ; i <= y ; i++ ){	

       layer_l = x * ( y - i ) + 1  ;
       layer_u = x * ( y - i + 1) ; 

       ratio = rmin + (int)( range3 * MyRand(0) ) ;
       r = r + ratio ; 

       for ( j = layer_l ; j <= layer_u ; j++ ){

	 temp = layer_u - j ; 

	 // link node to out
	 if ( i < y ){

	   cost = cost1[temp] * ratio ; 

	   printf ( "a   %d	%d	%d.000000 \n" , j , j - x , cost ) ;
	   //fprintf ( f , "a   %d	%d	%d.000000 \n" , j , j - x , cost ) ;
					
	   printf ( "a   %d	%d	%d.000000 \n" , j - x , j , cost ) ;
	   //fprintf ( f , "a   %d	%d	%d.000000 \n" , j - x , j , cost ) ;
	 }

	 // link node to node on the same layer
				
	 if( i != 1 ){
	   cost = cost2[temp] *(r-ratio) / r1 ;
	 }
	 else{
	   cost = cost2[temp] ; 
	 }
				
	 if ( j == layer_u ){	//if node is the largest one on the layer
	   printf ( "a   %d	%d	%d.000000 \n" , j - x + 1 , j , cost ) ;
	   //fprintf ( f , "a   %d	%d	%d.000000 \n" , j - x + 1 , j , cost ) ;
		      
		      		
	   printf ( "a   %d	%d	%d.000000 \n" , j , j - x + 1 , cost ) ;
	   //fprintf ( f , "a   %d	%d	%d.000000 \n" , j , j - x + 1 , cost ) ;

	 }
	 else{
	   
	   printf ( "a   %d	%d	%d.000000 \n" , j , j + 1 , cost ) ;
	   //fprintf ( f , "a   %d	%d	%d.000000 \n" , j , j + 1 , cost ) ;

		  
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




