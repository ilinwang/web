/***********************************************************/
/*                                                         */
/*               Executor of SP codes                      */
/*               (for usual Dijkstra)                      */
/*                                                         */
/***********************************************************/


#include <stdio.h>
#include <stdlib.h>

/* statistical variables */
long n_scans = 0;
long n_impr = 0;

/* definitions of types: node & arc */

#include "types_dh.h"

/* parser for getting extended DIMACS format input and transforming the
   data to the internal representation */

#include "parser_dh.c"

/* function 'timer()' for mesuring processor time */

#include "timer.c"

/* function for constructing shortest path tree */

#include "dikh.c"


main ()

{

float t;
arc *arp, *ta;
node *ndp, *source, *k;
long n, m, nmin, i; 
char name[21];
double sum_d = 0;

 parse( &n, &m, &ndp, &arp, &source, &nmin, name );
/*
printf ( "%s\nn= %ld, m= %ld, nmin= %ld, source = %ld\n",
        name,
        n,m,nmin, (source-ndp)+nmin );

printf ("\nordered arcs:\n");
for ( k = ndp; k< ndp + n; k++ )
  { i = (k-ndp)+nmin;
    for ( ta=k -> first; ta != (k+1)-> first; ta++ )
      printf ( " %2ld %2ld %4ld\n",
               i, ((ta->head)-ndp)+nmin, ta->len
             );

  }
*/
t = timer();

int counter = 0;
for (int i = 0; i < n; i++) {
    dikh ( n, ndp, ndp+i);
    counter += n_scans;
}

t = timer() - t;

printf ("Number of scans: %ld\n\
Running time of SP computation: %.2f\n\n",
         counter, t ); 
 
#define nd(ptr) (int)(ptr-ndp+nmin)

/*
for ( k=ndp; k< ndp+n; k++ )
printf (" %d %d %d\n", nd(k), nd(k->parent), k->dist);
*/
}
