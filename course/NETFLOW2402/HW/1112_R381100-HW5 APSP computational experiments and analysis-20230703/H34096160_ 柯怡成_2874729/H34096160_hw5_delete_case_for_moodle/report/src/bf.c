void bf ( n, nodes, source )

long n;                         /* number of nodes */
node *nodes,                    /* pointer to the first node */
     *source;                   /* pointer to the source     */

{

#define NNULL (node*)NULL
#define VERY_FAR 1073741823

/* -----   queues definitions ----- */

node *begin,
     *end;

/* status of node regarding to queue */ 
#define IN_QUEUE     0
#define OUT_OF_QUEUE 1

#define INIT_QUEUE(source)\
{\
begin = end = source;\
source -> next   = NNULL;\
source -> status = IN_QUEUE;\
}

#define NONEMPTY_QUEUE           ( begin != NNULL )

#define NODE_IN_QUEUE(node)      ( node -> status == IN_QUEUE )

#define EXTRACT_FIRST(node)\
{\
node = begin;\
node -> status = OUT_OF_QUEUE;\
begin = begin -> next;\
}

#define INSERT_TO_QUEUE(node)\
{\
 if ( begin == NNULL )\
   begin = node;\
 else\
   end -> next = node;\
\
 end = node;\
 end -> next = NNULL;\
 node -> status = IN_QUEUE;\
}\

/* -------------------------------------- */

long dist_new,
     dist_from;

node *node_from,
     *node_to,
     *node_last,
     *i;

arc  *arc_ij,
     *arc_last;

long num_scans = 0;

/* initialization */

node_last = nodes + n ;
 
for ( i = nodes; i != node_last; i ++ )
   { 
      i -> parent   = NNULL;
      i -> dist     = VERY_FAR;
      i -> status   = OUT_OF_QUEUE;
   }

source -> parent = source;
source -> dist   = 0;

INIT_QUEUE (source)


/* main loop */

while ( NONEMPTY_QUEUE )
 {

   EXTRACT_FIRST ( node_from )

   arc_last  = ( node_from + 1 ) -> first;
   dist_from = node_from -> dist;
   
   for ( arc_ij = node_from -> first; arc_ij != arc_last; arc_ij ++ )
     { /* scanning arcs outgoing from  node_from  */
       node_to  = arc_ij -> head;

       dist_new = dist_from + ( arc_ij -> len );
	   if (dist_from != VERY_FAR && ( arc_ij -> len ) != VERY_FAR){
	   		num_scans++;
	   }

       if ( dist_new <  node_to -> dist )
	   { 
	     node_to -> dist   = dist_new;
             node_to -> parent = node_from;

             if ( ! NODE_IN_QUEUE ( node_to ) )
	          INSERT_TO_QUEUE ( node_to )
	   }
     } /* end of scanning  node_from */
 } /* end of the main loop */

n_scans = num_scans;
}
