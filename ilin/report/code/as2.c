/**************************************************************************/
/*This is the 1st C code for my assignment algorithm.
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "as2.h"

/*---------- global variables -------------*/
sort_t star_arc[MAX_ARCS+1];  /* Arcs need sentinel with high node */
sort_t bk_star_arc[MAX_ARCS+1]; /* backup use for update before loop */
node_t star_node[MAX_NODES+1];
int head[MAX_NODES+1];   /* Head needs a sentinel at (n+1)st slot */
int ii;

void main()
{
  /* read network data, duplicate virtual networks, output to topo2.dat */
 
  FILE *fptopo, *fpmino;
  int i=0, j;
  int curr_arc=0;

  high_node= 0;       /* Highest node number; assume node nos > 0 */
  
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

  fscanf(fptopo, "%d %d", &org_n_node, &org_n_arc); /* read original #nodes #arcs */
  /* produce Virutal network data file, #nodes #arcs */

  new_n_node=8*org_n_node+2;
  new_n_arc=8*org_n_arc+16;
  high_node=new_n_node;

  /* ---------------------------------------read original data */
  for (i=0;i<(org_n_arc);i++) /* loop to read original network topology data 
			    and output to the virtual network data file */
    {
      fscanf(fptopo, "%d %d %d %d", &(star_arc[i].from), &(star_arc[i].to), 
	     &(star_arc[i].cost), &(star_arc[i].u));
      star_node[star_arc[i].from].n_arc++;
      star_arc[i].key=star_arc[i].from;  /*!! here, we need key !!*/
      star_arc[i].wavelength=1;
      star_arc[i].org_no=i;
      star_arc[i].norg_no=i;
      star_arc[i].flag=-1;
    }
    
  for (j=2;j<=8;j++) /* loop to duplicate the Virtual Network */
    {
      for (i=0;i<org_n_arc;i++)
	{
	  curr_arc=(j-1)*org_n_arc+i;
	  star_arc[curr_arc].from=(j-1)*org_n_node+star_arc[i].from;
	  star_arc[curr_arc].key=star_arc[curr_arc].from;
	  star_arc[curr_arc].to=(j-1)*org_n_node+star_arc[i].to;
	  star_arc[curr_arc].cost=star_arc[i].cost;
	  star_arc[curr_arc].u=star_arc[i].u;

	  star_node[star_arc[curr_arc].from].n_arc++;
	  star_arc[curr_arc].wavelength=j;
          star_arc[curr_arc].org_no=i;
	  star_arc[curr_arc].norg_no=curr_arc;
          star_arc[curr_arc].flag=-1;
	}
    }

  /* node is from 1 to new_n_node; */
  for(j=1;j<=(new_n_node-2);j++)
    {
      star_node[j].no=j;
    
      star_node[j].wavelength=j/org_n_node+1;
      star_node[j].org_no=j%org_n_node;
      if (star_node[j].org_no==0)
	{
	  star_node[j].org_no=org_n_node;
	  star_node[j].wavelength--;
	}
    }
  star_node[j].no=j;      /* source of VN */
  star_node[j+1].no=j+1;  /* sink of VN   */


  for (i=new_n_arc-16;i<new_n_arc;i++)
    {
      star_arc[i].norg_no=-2;
      star_arc[i].org_no=i;
    }

  while(fscanf(fpmino, "%d %d %d", &org_root_node, &org_dest_node, &amount)!=EOF) 
    {
    
printf("---------------------\n");
      for (i=0;i<=new_n_node;i++)
	{
	  star_node[i].dist=0;
	  star_node[i].pred=-1;
	  star_node[i].n_arc=0;
	  star_node[i].pred_arc=-1;
	}  

      for(i=0;i<(new_n_arc-16);i++)  /* read 8 virutal networks data */
	{
	  star_node[star_arc[i].to].exist=1;
	  star_node[star_arc[i].key].exist=1; /* so that I can know existence*/
	  star_node[star_arc[i].key].n_arc++;  /* of nodes, and # of out arcs */
	  if ((star_arc[i].key==new_n_node-1)&&
	    (star_node[star_arc[i].key].n_arc>8))
	    print_network();
	  
	}


      for (j=0;j<=7;j++) /* loop to make the arcs connecting from virutal 
			    source to all corresponding sources nodes in 8 
			    networks, and arcs connecting from all 
			    corresponding sink nodes in 8 networks to the 
			    virtual sink node */
	{
	  curr_arc=new_n_arc-16+2*j;
	  star_arc[curr_arc].to= org_root_node+j*org_n_node; /* only this changes */
/* printf("***** star_arc[%d].to=%d \n",curr_arc,star_arc[curr_arc].to);
*/
/*now, this is ok */

	  star_arc[curr_arc].key= new_n_node-1;     	  
	  star_arc[curr_arc].from=star_arc[curr_arc].key;
	  star_arc[curr_arc].cost=0;
	  star_arc[curr_arc].u=10000;
	  star_node[star_arc[curr_arc].to].exist=1;
	  star_node[star_arc[curr_arc].key].exist=1; 
	  star_node[star_arc[curr_arc].key].n_arc++;  
	  
	  star_arc[curr_arc+1].to= new_n_node;
	  star_arc[curr_arc+1].key= org_dest_node+j*org_n_node;
	  star_arc[curr_arc+1].from=star_arc[curr_arc+1].key;
	  star_arc[curr_arc+1].cost=0;
	  star_arc[curr_arc+1].u=10000;
	  star_node[star_arc[curr_arc+1].to].exist=1;
	  star_node[star_arc[curr_arc+1].key].exist=1; 
	  star_node[star_arc[curr_arc+1].key].n_arc++; 
	  if ((star_arc[i].key==new_n_node-1)&&
	    (star_node[star_arc[i].key].n_arc>8))
	    print_network();
	}

      /* sort the arcs by the from_node in ascending order, this is for the 
	 convenience to record the arc number */
      select_sort(star_arc, new_n_arc);

      /* identify the head[i] for node i, so that we will know what arcs going
	 out from node i */
      forward_star(star_arc, new_n_arc, high_node, head, star_node);
/*print_network();
exit(0);

     if (! init_arc_list())
	exit(0);
*/      
 
      /* lopps for solving shortest path, once for one unit of demands */
      for (i=0;i<amount;i++)
	{
	  s_short(star_arc, new_n_arc, high_node, head, org_root_node, org_dest_node, star_node);

	/*   printf("===========i/amout=%d/%d==============================\n",i,amount);*/

	}

      for (i=0;i<new_n_arc;i++)   /* update original network order with new data */
	{
	  j=star_arc[i].norg_no;  /* I have to use a temp star_arc o.w. it may 
				mess up the original data */
	  if (j<(new_n_arc-16))
	    {
	      bk_star_arc[j].norg_no=j;
	      bk_star_arc[j].org_no=star_arc[i].org_no;
	      bk_star_arc[j].from=star_arc[i].from;
	      bk_star_arc[j].key=bk_star_arc[j].from;
	      bk_star_arc[j].to=star_arc[i].to;
	      bk_star_arc[j].cost=star_arc[i].cost;
	      bk_star_arc[j].u=star_arc[i].u;
	      bk_star_arc[j].flag=star_arc[i].flag;
	    }
	}

      for (i=0;i<(new_n_arc-16);i++)  /* now copy the temp star_arc to the original
				    one. */
	{
	  star_arc[i].norg_no=bk_star_arc[i].norg_no;
	  star_arc[i].org_no=bk_star_arc[i].org_no;
	  if (star_arc[i].norg_no!=i)
	    {
	      printf("!@#$!@#$!@#$!@#$!@#$!@#$!@#$!@#$!@#$!@#$!@#$\n");
	      printf("star_arc[%d].norg_no=%d, i=%d\n",i,star_arc[i].norg_no, i);
 	    }
	  star_arc[i].from=bk_star_arc[i].from; 
	  star_arc[i].key=bk_star_arc[i].from;
	  star_arc[i].to=bk_star_arc[i].to;
	  star_arc[i].cost=bk_star_arc[i].cost;
	  star_arc[i].u=bk_star_arc[i].u;
	  star_arc[i].flag=bk_star_arc[i].flag;
	}
     for (i=new_n_arc-16;i<new_n_arc;i++)
        {
	  star_arc[i].norg_no=-2;
	  star_arc[i].org_no=i;
	}


printf("---------------------\n");
    }


      
      
      clearerr(fptopo);
      fclose(fptopo);
      clearerr(fpmino);
      fclose(fpmino);



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
void forward_star(sort_t star_arc[], int arcs, int high_node, int head[],
		  node_t star_node[])
{
  int i;

  head[star_arc[0].key]=0;    /* head start from 0 */
  for (i=1;i<new_n_arc;i++)     /* loop for arcs, arc no.:1--#arcs-1 */
    {
      if (star_arc[i].key>star_arc[i-1].key) 
	{
	  head[star_arc[i].key]=i;
	  continue;
	}
    }
  for (i=0;i<=(new_n_node+1);i++) /* loop for nodes, fill empty node, 
			      !!Note!! This should be useless here. */
    {                 /* assume node no. continous, start from 1--high node */
      if (star_node[i].exist==0)
	head[i]=head[i-1]+star_node[i-1].n_arc;
      star_node[i].no=i;
    }
  head[high_node+1]=head[high_node]+1;
}
/*======================== output_forward_star() ==========================*/
/* This function is to print out forward star structure for check */
/*-------------------------------------------------------------------------*/
void output_forward_star(sort_t star_arc[], int arcs, 
			 int high_node, int head[])
{
  int node, arc;
  printf("The forward star structure is as follows:\n");
  printf(" node \thead[node]\n");
  for (node=0; node <= (high_node+1); node++)    /* High node +1 is sentinel */
    printf(" %d\t%d\n", node, head[node]);

  printf("\narc\tfrom\tto\tcost\tu\n");
  for (arc=0; arc < arcs; arc++)
    printf(" %d\t%d\t%d\t%d\t%d\n", arc, 
	   star_arc[arc].key, star_arc[arc].to, 
	   star_arc[arc].cost, star_arc[arc].u);

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

void s_short(sort_t star_arc[], int arcs, int high_node, int head[], 
	     int org_root_node, int org_dest_node, node_t star_node[])
{
  int i,k, temp_node, path_nodes, curr_arc;
  node_t *heap[MAX_NODE+1], *ptop_node;
  int heap_size, to_node, tmp_dist, position, curr_arc_no;

  /* initialization */
  for (i=0;i<=high_node;i++)
    {
      star_node[i].pred=NULL_NODE;
      star_node[i].dist=INT_MAX;
      star_node[i].key=star_node[i].dist;
    }

  /* Sentinel */
  star_node[0].dist=INT_MIN;
  star_node[0].key=star_node[0].dist;
  heap[0]=&star_node[0];

  /* Root */

  new_root_node=high_node-1;
  new_dest_node=high_node;
  
  star_node[new_root_node].pred=new_root_node;
  star_node[new_root_node].dist=0;
  star_node[new_root_node].key=star_node[new_root_node].dist;

  /* initialize heap to contain just the root */
  heap[1]=&star_node[new_root_node];

  heap_size=1;
  construct_heap((info_t **)heap, heap_size);

  while (heap_size>0)  /* loops till no node in heap */
    {
      ptop_node=(node_t *)remove_heap((info_t **)heap, &heap_size);

      for(i=0;i<ptop_node->n_arc;i++)  /* loop to scan all outgoing arcs from 
					 node ptop_node */
	{
	  curr_arc_no=head[ptop_node->no]+i;
	  to_node=star_arc[curr_arc_no].to;
	
	  tmp_dist=ptop_node->dist + star_arc[curr_arc_no].cost;
	     
	  if (star_node[to_node].dist>tmp_dist) /* improvement!! */
	    {                    /* update distance label for this to_node */
	      star_node[to_node].pred=ptop_node->no;   /* record pred_node */
	      star_node[to_node].pred_arc=curr_arc_no; /* record pred_arc  */
	      star_node[to_node].dist=tmp_dist;
	      star_node[to_node].key=star_node[to_node].dist;
	                          /* compute position of this node in heap */ 
	      position=node_in_heap((node_t **)heap, to_node, heap_size);
	 /*  printf("position of to_node in heap=%d\n",position); */
	      if (position!=0)     /* found it, up_heap */
		up_heap(position,(info_t **) heap);
	      else                 /* new node, insert it to heap */
		insert_heap((info_t *)&star_node[to_node], (info_t **)heap, 
			    &heap_size);
	    }
	 /*  if (to_node==new_dest_node)
	     printf("dest_node=%d, pred=%d\n",to_node,star_node[to_node].pred);
	   */  
	}
    }
  /* store assigned path information */
  temp_node=new_dest_node;
  path[0]=temp_node;
  path_nodes=0;
/*  printf("the path from %d to %d is  ", new_root_node, temp_node); */

  while ((temp_node!=new_root_node)&&(temp_node!=-1))
    {
      path_nodes++;
      temp_node=star_node[temp_node].pred;
      path[path_nodes]=temp_node;

     /* printf("$$$$ star_node[%d].org_no=%d $$$$$$$$$\n", path[path_nodes],
	    star_node[path[path_nodes]].org_no);*/
    }
  /* [8n+1]th node -> root_node */
  printf(" %d(%d) ", path[path_nodes], star_node[path[path_nodes]].org_no);

  /* now we focus on the 2nd node on the path */
  i=path_nodes-1;
  /*printf(" ->[%d,%d] %d ", star_arc[star_node[path[i]].pred_arc].key, 
	 star_arc[star_node[path[i]].pred_arc].to, path[i]);*/
  printf(" -> %d(%d) ", path[i], star_node[path[i]].org_no);

  if (star_node[path[i]].org_no!=org_root_node)
    {
      printf("@@@@@@@@@@@@@@@@star_node[%d].org_no=%d   org_root_node=%d@@@@@@@@@@@@\n", path[i],star_node[path[i]].org_no,org_root_node );
      print_artiarc();
      exit(0);
    }

  /*____________________________________________________________________*/
  /* now, we can start the update part */
  /* inside the virtual network of some wavelength */
  /* it is from the 2nd node to the last 2nd node  */
  for(i=path_nodes-2;i>0;i--) /* i=the 3rd node to the last 2nd node in the 
				 path, which travels on the path in the 
				 virtual network */
    {
      curr_arc=star_node[path[i]].pred_arc;
      if (star_arc[curr_arc].flag==1) /* block for arcs with flag-ON */
	{
	  star_arc[curr_arc].u=1;

	  /* increase cost for current arc, because we are reserving it */
	  /* !!Note!! now I only use 10000 for increment, this amount may 
	     be some function in the future */
	  star_arc[curr_arc].cost+=10000;
	
	  /* now updating their corresponding arcs in other Virutal Networks*/
	  /* the method I am using here is stupid, and inefficient !!!! */
	  for (k=0;k<new_n_arc;k++)
	    {
	      if ((star_arc[k].org_no==star_arc[curr_arc].org_no)&&
		  (k!=curr_arc))
		{
		  if (star_arc[k].flag==1)
		    {
		      star_arc[k].flag=0;
		      star_arc[k].u=1;
		    }
		  else
		    star_arc[k].u++;
		}
	    }
	}
      else  /* block for arcs with flag-OFF */
	{
	  star_arc[curr_arc].u--;
	  if (star_arc[curr_arc].u==0)
	    {
	      star_arc[curr_arc].flag=1;
	      star_arc[curr_arc].u=1;
	      star_arc[curr_arc].cost+=10000;
	    }
	}
      
      /* output current cost, and to_node for this arc */
      /* printf("->[%d] %d ", star_arc[star_node[path[i]].pred_arc].cost, 
	path[i]); */     
      printf("-> %d(%d) ", path[i], star_node[path[i]].org_no);     
    }

  /* output the final arc which connect to the virtual sink(now i=0) */
  /*printf(" ->[%d,%d] %d ", star_arc[star_node[path[i]].pred_arc].key, 
	 star_arc[star_node[path[i]].pred_arc].to, path[i]);
  printf("----------------------- nodes= %d   arcs = %d --------------", 
  org_n_node, org_n_arc);*/

  printf(" ->%d(%d) COST=[%d] Wave{%d}\n", path[i], star_node[path[i]].org_no, star_node[path[i]].dist, star_node[star_node[path[i]].pred].wavelength);

  /* sometimes, it is impossible to reach the destination from the root,
     in such case, we have to report it */
  if (temp_node==-1)
    {
      printf("-> impossible!!\n");
      for (i=0;i<path_nodes;i++)
	path[i]=-1;
    }
}
/*======================== output_short() ===============================*/
/* This function is to output the final shortest path */
/*-------------------------------------------------------------------------*/
void output_short(node_t star_node[], int high_node, int root_node, int dest_node)
{
  int i;
/*  int temp_node=0;*/
  printf("The shortest path is the tree as follows:\n");
  printf(" i       pred[i]       dist[i]\n");
  printf("===------=======-------=======\n");
  for (i=0;i<=high_node;i++)
    {
      if (star_node[i].exist!=0)
	printf(" %d         %d             %d\n",i,star_node[i].pred,
	       star_node[i].dist);
    }
/*  printf("\n the path from %d back to %d is ", dest_node, root_node);

  temp_node=dest_node;
  printf("%d ", temp_node);
  while ((temp_node!=root_node)&&(temp_node!=-1))
    {
      printf("-> %d ", star_node[temp_node].pred);
      temp_node=star_node[temp_node].pred;
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
      printf("heap[%d]->(no,dist,n_arc)=(%d,%d,%d) ",i,heap[i]->no,
	     heap[i]->key,heap[i]->n_arc);
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
/*======================== print_network() =============================*/
void print_network()
{
  int i;
  sort_t *ptr_arc;
  node_t *ptr_node;

  
  printf("%d nodes, %d arcs\n",new_n_node, new_n_arc);
  printf("Arcs flows:\n");

  for(i=0, ptr_arc=star_arc;i<new_n_arc;i++, ptr_arc++)
  {
    printf("%3d (%2d): %3d->%3d  C:%3d U:%3d F:%1d W:%1d\n", i, ptr_arc->org_no, ptr_arc->from, ptr_arc->to, ptr_arc->cost, ptr_arc->u, ptr_arc->flag, ptr_arc->wavelength);
  }
  printf("Nodes:\n");
  for (i=0,ptr_node=star_node;i<=new_n_node;i++,ptr_node++)
  {
    printf
      ("#%3d (%2d): W:%1d pred=%3d, dist=%9d n_arc=%2d \n", ptr_node->no, ptr_node->org_no, ptr_node->wavelength, ptr_node->pred, ptr_node->dist, ptr_node->n_arc);
  }
/*  exit(0);*/
}


/*======================== print_artiarc() =============================*/
void print_artiarc()
{
  int i;
  sort_t *ptr_arc;
  node_t *ptr_node;

  
  printf("%d nodes, %d arcs\n",new_n_node, new_n_arc);
  printf("Arcs flows:\n");

  for(i=0, ptr_arc=star_arc;i<new_n_arc;i++, ptr_arc++)
  {
    if (ptr_arc->norg_no==0)
      {
	printf("%3d (%2d): %3d->%3d  C:%3d U:%3d F:%1d W:%1d\n", i, ptr_arc->org_no, ptr_arc->from, ptr_arc->to, ptr_arc->cost, ptr_arc->u, ptr_arc->flag, ptr_arc->wavelength);
      }
  }
  printf("Nodes:\n");
  for (i=0,ptr_node=star_node;i<=new_n_node;i++,ptr_node++)
  {
    if ((ptr_node->no==new_n_node-1)||(ptr_node->no==new_n_node))
      {
	printf
	  ("#%3d (%2d): W:%1d pred=%3d, dist=%9d n_arc=%2d \n", ptr_node->no, ptr_node->org_no, ptr_node->wavelength, ptr_node->pred, ptr_node->dist, ptr_node->n_arc);
      }
  }
/*  exit(0);*/
}

/*======================== init_arc_list() =============================*/
int init_arc_list()
{ 
  int i, node;
  sort_t *ptr_arc;
  node_t *ptr_node;
  for(i=0, ptr_node=star_node; i<=new_n_node; i++, ptr_node++)	
    /* Allocate the space for adjacency arc list */
    {
      if ((ptr_node->arc_list=(int *)malloc(ptr_node->n_arc*sizeof(int)))==NULL)
	{
	  /* Output error and return failure if failed to allocate */
	  printf("Cannot allocate memory, job terminated\n");
	  return(0);
	}
    }
  for (i=0, ptr_arc=star_arc; i<new_n_arc; i++, ptr_arc++)
    /* Initialize the adjacency arc list */
    {
      node = ptr_arc->from; 
      (star_node[node].arc_list)[(star_node[node].current_arc)++] = i;
     /*printf("star_node[%d].%d=%d\n ",node, star_node[node].current_arc,i);*/
     /* node = ptr_arc->to;
      (star_node[node].arc_list)[(star_node[node].current_arc)++] = i;
      */
   }

  return (1);
}
