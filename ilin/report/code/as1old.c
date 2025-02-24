/***************************************************************************/
/*This is the 1st C code for my assignment algorithm.
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "as1.h"


static sort_t arc_list[MAX_ARCS+1];  /* Arcs need sentinel with high node */
static node_t node_list[MAX_NODES+1];
static int high_node;
static int head[MAX_NODES+1];   /* Head needs a sentinel at (n+1)st slot */
static int root_node, dest_node;


void main()
{
  int i;
  /* read network data, duplicate virtual networks, output to topo2.dat */
  read_network(arc_list, &nodes, &arcs, &amount, &high_node, &root_node, 
	       &dest_node, node_list);

  /* sort the arcs by the from_node in ascending order, this is for the 
     convenience to record the arc number */
  select_sort(arc_list, arcs);

  /* identify the head[i] for node i, so that we will know what arcs going
     out from node i */
  forward_star(arc_list, arcs, high_node, head, node_list);
/*  printf("=========================================\n");
  output_forward_star(arc_list, arcs, high_node, head);
*/

  /* lopps for solving shortest path, once for one unit of demands */
  for (i=0;i<amount;i++)
    {
      s_short(arc_list, arcs, high_node, head, root_node, dest_node, 
	      node_list);
  /*    update_network(arc_list, node_list, path); */
      /*  printf("=========================================\n");
      output_short(node_list, high_node, root_node, dest_node);
      */
      }
  exit(0);
}

/*======================== read_network() ===============================*/
/* This function is to read the data of network */
/*-------------------------------------------------------------------------*/
void read_network(sort_t arc_list[],int *nodes, int *arcs, int *amount, int *high_node,	int *root_node, int *dest_node, node_t node_list[])
{
  FILE *fptopo, *fpmino, *fptopo2;
  int i=0, j, from, to, cost, capy;

  *high_node= 0;       /* Highest node number; assume node nos > 0 */
  
  if ((fptopo=fopen("topo.dat", "r"))== NULL)  /* open topology data file */
    {
      fprintf(stderr, " Error opening topo.dat\n");
      exit(1);
    }
  if ((fpmino=fopen("mino.dat", "r"))== NULL)  /* open demand data file */
    {
      fprintf(stderr, " Error opening mino.dat\n");
      exit(1);
    }
  if ((fptopo2=fopen("topo2.dat", "w"))== NULL) /* open virtual network file */
    {
      fprintf(stderr, " Error opening mino.dat\n");
      exit(1);
    }
  fscanf(fptopo, "%d %d", nodes, arcs); /* read original #nodes #arcs */
  /* produce Virutal network data file, #nodes #arcs */
  fprintf(fptopo2,"%d %d\n", 8*(*nodes)+2, 8*(*arcs)+16 );
  /* read damand data 
   !!Note!! now, I can only read 1 demand */
  fscanf(fpmino, "%d %d %d", root_node, dest_node, amount); 

  for (i=0;i<(*arcs);i++) /* loop to read original network topology data 
			   and output to the virtual network data file */
    {
      fscanf(fptopo, "%d %d %d %d", &from, &to, &cost, &capy);
      for (j=0;j<=7;j++)  /* loop to duplicate 8 virutal networks */
	{
	  fprintf(fptopo2, "%d %d %d %d\n", (from+j*(*nodes)), (to+j*(*nodes)), 
		  cost, capy);
	}
    }

  for (j=0;j<=7;j++) /* loop to make the arcs connecting from virutal source
			to all corresponding sources nodes in 8 networks,
			and arcs connecting from all corresponding sink nodes
			in 8 networks to the virtual sink node */
	{
	  fprintf(fptopo2, "%d %d %d %d\n", (8*(*nodes)+1), 
		  (*root_node+j*(*nodes)), 0, 1000000000);
	  fprintf(fptopo2, "%d %d %d %d\n", (*dest_node+j*(*nodes)), 
		  (8*(*nodes)+2), 0, 1000000000);
	}  

  *high_node=8*(*nodes)+2;

  fclose(fptopo2);
 
  if ((fptopo2=fopen("topo2.dat", "r"))== NULL)
    {
      fprintf(stderr, " Error opening topo.dat\n");
      exit(1);
    }
  
  /* Now, we can read this virtual network data again */
  fscanf(fptopo2, "%d %d", nodes, arcs);

  for(i=0;i<8*(*arcs)+16;i++)
    {
      fscanf(fptopo2, "%d %d %d %d", &arc_list[i].key, &arc_list[i].to,
	     &arc_list[i].cost, &arc_list[i].capy);
      node_list[arc_list[i].to].exist=1;
      node_list[arc_list[i].key].exist=1; /* so that I can know existence */
      node_list[arc_list[i].key].arcs++;  /* of nodes, and # of out arcs */
      
    }

  clearerr(fptopo);
  fclose(fptopo);
  clearerr(fpmino);
  fclose(fpmino);
  clearerr(fptopo2);
  fclose(fptopo2);
/*  *arcs= i; */
  return;
}
/*======================== forward_star() ===============================*/
/* This function is to record network as a forward star structure:
   i.e. define the head(pointer) of each node.; head[i] points to the 1st arc
   of node i 
   ex: arc no.  arc   ===>      node no.     head[node no]
         0     (1,2)               1            0  
         1     (2,4)               2            1 
         2     (4,5)               3            2
         3     (4,6)               4            2
                                   5            4
                                   6            4
                                   7            4 ===> useless in this prob
--------------------------------------------------------------------------*/
void forward_star(sort_t arc_list[], int arcs, int high_node, int head[],
		  node_t node_list[])
{
  int i;

  head[arc_list[0].key]=0;    /* head start from 0 */
  for (i=1;i<(8*arcs+16);i++)     /* loop for arcs, arc no.:1--#arcs-1 */
    {
      if (arc_list[i].key>arc_list[i-1].key) 
	{
	  head[arc_list[i].key]=i;
	  continue;
	}
    }
  for (i=0;i<=8*nodes+1;i++) /* loop for nodes, fill empty node, 
			      !!Note!! This should be useless here. */
    {                 /* assume node no. continous, start from 1--high node */
      if (node_list[i].exist==0)
	head[i]=head[i-1]+node_list[i-1].arcs;
      node_list[i].no=i;
    }
  head[high_node+1]=head[high_node]+1;
}
/*======================== output_forward_star() ==========================*/
/* This function is to print out forward star structure for check */
/*-------------------------------------------------------------------------*/
void output_forward_star(sort_t arc_list[], int arcs, 
			 int high_node, int head[])
{
  int node, arc;
  printf("The forward star structure is as follows:\n");
  printf(" node \thead[node]\n");
  for (node=0; node <= (high_node+1); node++)    /* High node +1 is sentinel */
    printf(" %d\t%d\n", node, head[node]);

  printf("\narc\tfrom\tto\tcost\tcapy\n");
  for (arc=0; arc < arcs; arc++)
    printf(" %d\t%d\t%d\t%d\t%d\n", arc, 
	   arc_list[arc].key, arc_list[arc].to, 
	   arc_list[arc].cost, arc_list[arc].capy);

  return;
}

/*============================== s_short() ===============================*/
/* This function is to implement Dijkstra's algorithm by heap, the proceduer:
   1. initialize nodes, fill all elements
   2. set heap[0] as the sentinel; also initialize root node
   3. put root node to be heap[1]; construct the heap
   4. Doing Dijkstra's algorithm:
      While there is some node in heap
      {
         remove the top node in heap, say, i
	 scan all outgoing arcs from the top node, say, (i,j)
	     if (current dist of node j)>(dist of i)+(cost of (i,j))
	     {
	       update the distance label of node j
	       compute its position in current heap
	       if j is already in heap
	          up_heap() to make the node with smallest dist be on top
	       else j is new for the heap
	          insert_heap() to insert j into heap, also, the node with
		  small dist will be on top, too.
	     }	  
             else no improvement on dist, so, go on next loop 
	}
-------------------------------------------------------------------------*/

void s_short(sort_t arc_list[], int arcs, int high_node, int head[], 
	     int org_root_node, int org_dest_node, node_t node_list[])
{
  int i, temp_node, path_nodes;
  node_t *heap[MAX_NODE+1], *ptop_node;
  int heap_size, to_node, tmp_dist, position, curr_arc_no;

  /* initialization */
  for (i=0;i<=high_node;i++)
    {
      node_list[i].pred=NULL_NODE;
      node_list[i].dist=INT_MAX;
      node_list[i].key=node_list[i].dist;
    }

  /* Sentinel */
  node_list[0].dist=INT_MIN;
  node_list[0].key=node_list[0].dist;
  heap[0]=&node_list[0];

  /* Root */

root_node=high_node-1;
dest_node=high_node;

  node_list[root_node].pred=root_node;
  node_list[root_node].dist=0;
  node_list[root_node].key=node_list[root_node].dist;

  /* initialize heap to contain just the root */
  heap[1]=&node_list[root_node];

  heap_size=1;
  construct_heap((info_t **)heap, heap_size);

  while (heap_size>0)  /* loops till no node in heap */
    {
      ptop_node=(node_t *)remove_heap((info_t **)heap, &heap_size);

      for(i=0;i<ptop_node->arcs;i++)  /* loop to scan all outgoing arcs from 
					 node ptop_node */
	{
	  curr_arc_no=head[ptop_node->no]+i;
	  to_node=arc_list[curr_arc_no].to;
	  tmp_dist=ptop_node->dist + arc_list[curr_arc_no].cost;
	     
	  if (node_list[to_node].dist>tmp_dist) /* improvement!! */
	    {                    /* update distance label for this to_node */
	      node_list[to_node].pred=ptop_node->no;
	      node_list[to_node].pred_arc=curr_arc_no;
	      node_list[to_node].dist=tmp_dist;
	      node_list[to_node].key=node_list[to_node].dist;
	                          /* compute position of this node in heap */ 
	      position=node_in_heap((node_t **)heap, to_node, heap_size);
	 /*  printf("position of to_node in heap=%d\n",position); */
	      if (position!=0)     /* found it, up_heap */
		up_heap(position,(info_t **) heap);
	      else                 /* new node, insert it to heap */
		insert_heap((info_t *)&node_list[to_node], (info_t **)heap, 
			    &heap_size);
	    }
	}
    }
  /* store assigned path information */
  temp_node=dest_node;
  path[0]=temp_node;
  path_nodes=0;
  printf("\n the path from %d to %d is  ", root_node, dest_node);

  while ((temp_node!=root_node)&&(temp_node!=-1))
    {
      path_nodes++;
      temp_node=node_list[temp_node].pred;
      path[path_nodes]=temp_node;
    }
  /* [8n+1]th node -> root_node */
  printf(" %d ", path[path_nodes]);
  i=path_nodes-1;
  printf(" ->[%d,%d] %d ", arc_list[node_list[path[i]].pred_arc].key, 
	 arc_list[node_list[path[i]].pred_arc].to, path[i]);

  /* inside the virtual network of some wavelength */
  for(i=path_nodes-2;i>0;i--)
    {
      arc_list[node_list[path[i]].pred_arc].flag=1;

      printf("->[%d] %d ", arc_list[node_list[path[i]].pred_arc].cost, path[i]);      arc_list[node_list[path[i]].pred_arc].cost+=10000;
    }
  /* dest_nodes -> [8n+2]th node */
  printf(" ->[%d,%d] %d ", arc_list[node_list[path[i]].pred_arc].key, 
	 arc_list[node_list[path[i]].pred_arc].to, path[i]);
  
  if (temp_node==-1)
    {
      printf("-> impossible!!");
      for (i=0;i<path_nodes;i++)
	path[i]=-1;
    }
 printf("\n");
}
/*======================== output_short() ===============================*/
/* This function is to output the final shortest path */
/*-------------------------------------------------------------------------*/
void output_short(node_t node_list[], int high_node, int root_node, int dest_node)
{
  int i;
/*  int temp_node=0;*/
  printf("The shortest path is the tree as follows:\n");
  printf(" i       pred[i]       dist[i]\n");
  printf("===------=======-------=======\n");
  for (i=0;i<=high_node;i++)
    {
      if (node_list[i].exist!=0)
	printf(" %d         %d             %d\n",i,node_list[i].pred,
	       node_list[i].dist);
    }
/*  printf("\n the path from %d back to %d is ", dest_node, root_node);

  temp_node=dest_node;
  printf("%d ", temp_node);
  while ((temp_node!=root_node)&&(temp_node!=-1))
    {
      printf("-> %d ", node_list[temp_node].pred);
      temp_node=node_list[temp_node].pred;
    }
  if (temp_node==-1)
     printf("-> impossible!!");
     printf("\n%d %d %d",(nodes), (arcs), (amount));
*/

  printf("\n ");
}
      
  
/***************************************************************************/
/* int_min_heap.c - Library of heap functions */
/***************************************************************************/
/* These require the key field in the info_t structure to be an integer */

/*============================ print_heap() ==============================*/
/* to print out the heap for checking what's inside it */
/*-------------------------------------------------------------------------*/
void print_heap(node_t *heap[], int heap_size)
{
  int i;
  if (heap_size==0)
    { printf(" No Heap!!!!\n"); return; }
  for (i=1;i<=heap_size;i++)
    {
      printf("heap[%d]->(no,dist)=(%d,%d) ",i,heap[i]->no,heap[i]->key);
    }
}
/*=========================== node_in_heap() ==============================*/
/* to find position of a node in the heap, return position if found; 0,O.W.*/
/*-------------------------------------------------------------------------*/
int node_in_heap(node_t *heap[], int no, int heap_size)
{
  int i;
  for (i=1;i<=heap_size;i++)  /* using linear search */
    if (heap[i]->no==no)
      return i;
  return 0;
}
/*========================= construct_heap() =============================*/
/* Creates heap from array of pointers to records with integer key*/
/* Creates sentinel at heap[0] shared across all heaps */
/* Will be a problem with Sedgewick's replace_heap function; if so, */
/* calling program must create its own sentinel for each heap */
/*-------------------------------------------------------------------------*/
void construct_heap(info_t *heap[], int n)
{
  int k;
  static info_t row0;
  row0.key= INT_MIN;
  heap[0] = &row0;      /* Sentinel */
  for (k= n/2; k >= 1; k--)
    down_heap(k, heap, n);
}
/*========================= insert_heap() =============================*/
/* Places node val in the correct position in the heap */
/*-------------------------------------------------------------------------*/
void insert_heap(info_t *val, info_t *heap[], int *n)
{
  heap[++(*n)]= val;
  up_heap(*n, heap);
}
/*========================= remove_heap() =============================*/
/* Removes top element from heap, adjusts heap afterwards */
/*-------------------------------------------------------------------------*/
info_t *remove_heap(info_t *heap[], int *n)
{
  info_t *val= heap[1];
  heap[1]= heap[(*n)--];
  down_heap(1, heap, *n);
  return val;
}
/*========================= replace_heap() =============================*/
/*!!NOTE!!: we don't use this function for this problem */
/* Replace key at root with new key, then adjust heap */
/* This trashes the sentinel heap[0]; modify this function to */
/* save heap[0] initially, save the resulting heap[0] from down_heap */
/* in a temporary variable, restore heap[0] and then return the temp */
/*-------------------------------------------------------------------------*/
info_t *replace_heap(info_t *val, info_t *heap[], int n)
{
  info_t *tmp1, *tmp2;
  tmp1=heap[0];
  heap[0]= val;
  down_heap(0, heap, n);
  tmp2=heap[0];
  heap[0]=tmp1;
  return tmp2;
}
/*========================= heapsort_heap() =============================*/
/*!!NOTE!!: we don't use this function for this problem */
/* Sorts elements in heap in descending order */
/* Note it changes internal value of n but does not affect calling program n */
/*-------------------------------------------------------------------------*/
void heapsort_heap(info_t *heap[], int n)
{
  int k;
  info_t *t;
  for (k= n/2; k >= 1; k--)
    down_heap(k, heap, n);
  while (n>1)
    {
      t= heap[1];
      heap[1]= heap[n];
      heap[n]= t;
      down_heap(1, heap, --n);
    }
}
/*============================ up_heap() ================================*/
/*!!NOTE!!: modify it so that it can also handle k=0 & 1, also, it will make*/
/*          the top heap with smallest key */
/* Fixes heap violation at position k, relative to elements */
/* ABOVE it in the heap.  Does not check below element k */
/* Usually not called by application but used by other heap functions */
/*-------------------------------------------------------------------------*/
void up_heap(int k, info_t *heap[])
{
  /* int i, kk; */
  info_t *val;
  if ((k==0)||(k==1))
    return;
  /* kk=k; */
  val= heap[k];
  while (heap[k/2]->key >= val->key)
    {
      heap[k]= heap[k/2];
      k /= 2;
    }
  heap[k]= val;
  /* the following is to check the result of up_heap */
  /* for (i=1;i<=kk;i++)
     printf("up_heap:current (heap#,key)=(%d,%d)\n",i,heap[i]->key); */
}
/*============================ down_heap() ================================*/
/*!!NOTE!!: modify it so that the top heap with smallest key */
/* Places element k in the correct position in the heap, relative */
/* to elements BELOW it in the heap.  Does not check above element k */
/* Usually not called by application but used by other heap functions */  
/*-------------------------------------------------------------------------*/
void down_heap(int k, info_t *heap[], int n)
{
  /* int i, kk; */
  int j;
  info_t *val;
  if ((n==0)||(n==1))
    return;
  /* kk=k; */
  val= heap[k];
  while (k <= n/2)
    {
      j= k+k;
      if (j<n && heap[j]->key > heap[j+1]->key)
	j++;
      if (val->key <= heap[j]->key)
	break;
      heap[k]= heap[j];
      k= j;
    }
  heap[k]= val;
  /* the following is to check the result of down_heap */
  /* for (i=1;i<=kk;i++)
    printf("down_heap:current (heap#,key)=(%d,%d)\n",i,heap[i]->key); */
}



/***************************************************************************/
/* sort_lib.c - Sorting routines, modified from Sedgewick */
/***************************************************************************/
/* Selection sort */
void select_sort(sort_t sort_list[], int elements)
{
  int i, j, min;
  sort_t t;

  /* Start at 0, not 1; different than Sedgewick */
  for (i=0; i < (elements-1); i++)
    {
      min= i;
      for (j= i+1; j < elements; j++)
	if (sort_list[j].key <= sort_list[min].key)
	  min= j;
      t= sort_list[min];
      sort_list[min]= sort_list[i];
      sort_list[i]= t;
    }
  return;
}
