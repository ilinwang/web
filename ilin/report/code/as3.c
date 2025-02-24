/**************************************************************************/
/*This is the 1st C code for my assignment algorithm using Dijkstra Algotirhm
  main procedures:
      1. initialization: read topology data(topo.dat), 
                         allocate memory, duplicate 8 wavelength virtual 
			 network.
      2. loop to read demand from mino.dat
              reset_network(): 
	      loop to run Dijkstra for 1 unit of current demand
	           short_path()
		   if a path can be found
		        update_network()
		   else
		      leave the loop, stop the program.  
      3. close the opened files
 
  function called: initialize(): do initialization
                   reset_network(): to reset the network for new demand 
	                            construct VN source/sink & associated 
				              new arcs
		   short_path(unit): Shortest path program to assign path
		                     for 1 unit of the current demand
		   date_network(): to trace the path arcs; 
		                   researve fiber in advance; 
				   update cost/capacity/flag		   
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "as3.h"


void main()
{
  int demand=0, unit=0;
 
  initialize(); /* initialize networks, allocate memory */

  /* --------- Loop to read demand data, 1 by 1 -----------*/
  while(fscanf(fpmino, "%d %d %d", &org_root_node, &org_dest_node, 
	       &amount)!=EOF) 
    {
      demand++;
      printf("---------------------\n");

      reset_network(); /* for each new demand with different source/sink, we
			  have to renew some network data such as predecussor
			  and n_arc for each node, because the topology is
			  changed */

      /* Now, begin the shortest path part */
      for (unit=0;unit<amount;unit++)  /* loop for # amount with same 
					  source/sink */
	{
	  if(!short_path(unit))    /* if the demand can be satisfied, we
				      have to update the network after we
				      assigned the path for this unit */
	    update_network();
	  else                     /* else the demand could not be satisfied
				      we should stop the program, warn the
				      user */
	    {
	      printf(" %dth demand's %dth unit could not be reached from %d to %d !!\n", demand, unit+1, org_dest_node, org_root_node);
	      break;
	    }
	}
      printf("---------------------\n");
      if (short_path(unit))  /* stop the program if a demand can't be sent */
	exit(0);
    }
  /*  print_network();*/
  clearerr(fptopo);
  fclose(fptopo);
  clearerr(fpmino);
  fclose(fpmino);
}
/*============================ initialize() =============================*/
/*  This function is to do the initialization.
    it includes open input files: topo.dat--network topology data
                                  mino.dat--demands data from Minoura San
		     allocate memory: memory_allocation()
		     read original network data
		     generate other wavelength network data
    function called: memory_allcation()
-------------------------------------------------------------------------*/
void initialize()
{
  int i,j;

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

 
  /* ---------------------------------------read original #nodes #arcs */
  /* produce Virutal network data file, #nodes #arcs */
  fscanf(fptopo, "%d %d", &org_n_node, &org_n_arc); 
  
  new_n_node=8*org_n_node+2;
  new_n_arc=8*org_n_arc+16;

  /* ---------------------------------------allocate memory */
  if (memory_allocation()==0)
    {
      printf("\n no memory!!!!\n");
      exit(0);
    }

  /* ---------------------------------------read original data */
  for (i=0;i<(org_n_arc);i++) /* loop to read original network topology data 
			    and output to the virtual network data file */
    {
      fscanf(fptopo, "%d %d %d %d", &(star_arc[i].from), &(star_arc[i].to), 
	     &(star_arc[i].cost), &(star_arc[i].u));
      star_node[star_arc[i].from].n_arc++;
      star_arc[i].wavelength=1;
      star_arc[i].org_no=i;
      star_arc[i].flag=-1;
    }
    
  for (j=2;j<=8;j++) /* loop to duplicate the Virtual Network */
    {
      for (i=0;i<org_n_arc;i++)
	{
	  curr_arc=(j-1)*org_n_arc+i;
	  star_arc[curr_arc].from=(j-1)*org_n_node+star_arc[i].from;
	  star_arc[curr_arc].to=(j-1)*org_n_node+star_arc[i].to;
	  star_arc[curr_arc].cost=star_arc[i].cost;
	  star_arc[curr_arc].u=star_arc[i].u;

	  star_node[star_arc[curr_arc].from].n_arc++;
	  star_arc[curr_arc].wavelength=j;
          star_arc[curr_arc].org_no=i;
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
      star_node[j].org_n_arc=star_node[j].n_arc; /* record original n_arc */
    }
  star_node[j].no=j;      /* source of VN */
  star_node[j+1].no=j+1;  /* sink of VN   */


  for (i=new_n_arc-16;i<new_n_arc;i++)
    star_arc[i].org_no=i;

}

/*======================== memory_allocation() ==========================*/
/*  MEMORY_ALLOCATION()
    allocate memories for the basic data structure 
   
   function called: none
-------------------------------------------------------------------------*/
int memory_allocation()
{
  int i;
  struct node *ptr_node;

  if ((star_node=(struct node *)malloc((new_n_node+1)*sizeof(struct node)))==NULL)
  {
    /* Output error and return failure if failed to allocate memory */
    printf("Cannot allocate memory, job terminated\n");
    return(0);
  }
  if ((star_arc=(struct arc *)malloc((new_n_arc)*sizeof(struct arc)))==NULL)
    /* Need n_node-1 more space for artificial arcs */
  {
    /* Output error and return failure if failed to allocate memory */
    printf("Cannot allocate memory, job terminated\n");
    return(0);
  }

  if ((heap=(int *)malloc((new_n_node+1)*sizeof(int)))==NULL)
    {
      /* Output error and return failure if failed to allocate memory */
      printf("Cannot allocate memory, job terminated\n");
      return(0);
    }
   for(i=0, ptr_node=star_node; i<=new_n_node; i++, ptr_node++)	
     /* Allocate the space for adjacency arc list */
    {
      if ((ptr_node->arc_list=(int *)malloc(new_n_arc*sizeof(int)))==NULL)
	{
	  /* Output error and return failure if failed to allocate */
	  printf("Cannot allocate memory, job terminated\n");
	  return(0);
	}
    }  
  return (1);
}

/*======================== reset_network() =============================*/
/* for each new demand with different source/sink, we have to renew some 
   network data such as predecussor and n_arc for each node,
   because the topology is changed 

   function called: init_arc_list(): initialize adjacent arcs for each node
                                     because different demand will change
				     the adjacency for the nodes
-------------------------------------------------------------------------*/
void reset_network()
{
  int i,j;

  new_root_node=new_n_node-1;
  new_dest_node=new_n_node;
  
  for (i=0;i<=new_n_node;i++)    /* renew all nodes data for new demand */
    {
      star_node[i].dist=INT_MAX;
      star_node[i].perm=0;
      star_node[i].pred=-2;
      star_node[i].pred_arc=-2;
      star_node[i].heap=-1;
      star_node[i].n_arc=star_node[i].org_n_arc; /* !! this is important !! */
      star_node[i].current_arc=0;                /* !! this is important !! */
    }  
  
  
  for (j=0;j<=7;j++) /* loop to make the arcs connecting from virutal 
			source to all corresponding sources nodes in 8 
			networks, and arcs connecting from all 
			corresponding sink nodes in 8 networks to the 
			virtual sink node */
    {
      curr_arc=new_n_arc-16+2*j;
      
      star_arc[curr_arc].to= org_root_node+j*org_n_node;
      star_arc[curr_arc].from= new_n_node-1;
      star_arc[curr_arc].cost=0;
      star_arc[curr_arc].u=U;
      
      star_node[star_arc[curr_arc].from].n_arc++;
      
      curr_arc++;
      star_arc[curr_arc].to= new_n_node;
      star_arc[curr_arc].from=org_dest_node+j*org_n_node;
      star_arc[curr_arc].cost=0;
      star_arc[curr_arc].u=U;
      
      star_node[star_arc[curr_arc].from].n_arc++;
    }
  
  if (! init_arc_list())
    {
      printf("Oh! Can't initialize arc_list for each node !!\n");
      exit(0);
    }
}

/*======================== init_arc_list() ===============================*/
/* This function is to record the outgoing arcs for each node.
   because whenever we change the demand, the souce/sink will be changed,
   then, the arc adjacency list will be changed, too.(for the Virtual Network)
   So, we have to do this whenever we are proceeding a new demand (with
   different source/sink as the last one)
  
  function called: none
--------------------------------------------------------------------------*/
int init_arc_list()
{ 
  int i, node;
  arc_t *ptr_arc;

  for (i=0, ptr_arc=star_arc; i<new_n_arc; i++, ptr_arc++)
    /* Initialize the adjacency arc list */
    {
      node = ptr_arc->from; 
      (star_node[node].arc_list)[(star_node[node].current_arc)++] = i;
    }

  return (1);
}

/*============================= short_path() =============================*/
/* This is the main part of the shortest path algorithm. Here I used
   Dijkstra algorithm.
   The basic procedures are:
   1. init_dij(): reset the heap[], nodes' distance/predecessor,
                  reset all nodes to be not permanent
		  put root to the top heap--heap[0]
   2. remove heap[0], #permanent node increases by 1
   3. Loop to do Dijkstra from source to sink
      {
        Do dijkstra(), which scans all the outgoing arcs from node j, 
	               update heap, so that the top heap has smallest distance
	
	if heap[0]==the destination 
	   we can just quit this loop, go to assign next unit of demands
	   because we have found the shortest path!
	else                          
	   remove the top heap(i.e. heap[0]),to be the next one we want to scan
	   replace heap[0] with heap[h](the last node in the heap)
	   check if the heap[0]==0, because the only reason why 0 is in the
	       heap is that the destination can't be reached. In such case,
	       we have to quit this function, and then output the warning
	       message in the main()
	   else
	       we can at least move 1 step forward, finish replacing the last
	       heap node to the top heap position

	       Do the heapify() to re-sort the distance for the whole heap
	       after heapify(), the top heap node will be the node with 
	       smallest distance label again. then, we can continue next loop
	       scan for the outgoing arcs from that heap[0]
      }
   4. if we have make the destination node permanent, we can then quit this
      function, and start to update the network data in the main()

  function called: init_dij(): reset the heap[]
                   dijkstra(j,&h) : 1 step of dijkstra for node j
                   heapify(0, h-1): sort the heap after heap[0] is deleted
		                    and replaced by heap[h]
-------------------------------------------------------------------------*/
int short_path(int unit)
{
  int j=0, h=0;
  root=new_root_node;
  h=new_n_node+1;
  
  init_dij(root, h);  /* rest heap, networks for 1 unit of demand */
                      /* now, heap[0]=root */  
  n_perm=0;           /* originally, no node is permanent */  
  h=1;                /* now, we plan to mark heap[0] to be permanent */
  j=heap[0];              /* remove heap[0] from the heap */
  star_node[j].heap= -1;
  h--;
  star_node[j].perm=1;    /* now, j (i.e. heap[0]) is permanent */
  n_perm++;
  
  while(n_perm<new_n_node)  /*loop to finish assigning 1 unit by
			      Dijkstra */
    {                         /*later I should make this faster by
				exitting the loop once I got sink
				permanent ! !!!!Not yet now!!!!*/
      dijkstra(j, &h);
      
      j=heap[0];              /* the top heap node is the one with
				 min. dist */
      if (j==new_dest_node)   /* once the new_dest_node is permanent,
				 we are done for this loop */
	break;
      star_node[j].heap= -1;  /* remove the top heap node */
      h--;                    /* # heaps-- */
      star_node[j].perm=1;    /* mark it to be perm */
      n_perm++;               /* # perm++ */
      heap[0]=heap[h];        /* replace the top heap with the bottom
				 one */
      if (heap[0]==0)         /* 0 should never be in the heap at all */
	{                     /* i.e. the destination can't be reached */
	  /*print_heap(h); print_network();*/
	  /*printf("h=%d \n",h);print_heap(3);*/
	  return(1);
	}
      star_node[heap[0]].heap=0; /* now the new top heap has an index 0 */

      heap[h]=-1;             /* replace the original bottom one with -1 */
				       
      heapify(0, h-1);        /* heapify the heap, !!Note!! if h<1, the 
				 function will do nothing but return */
    }
  return (0);

}  

/*============================= init_dij() =============================*/
/*  initialization for dijkstra:
    Everytime after we finished assigning a unit of demand, we have to
    reset the heap struction, also, we have to clear the path information
    associated with each node, because the predecessor information will change

  functino called: none
--------------------------------------------------------------------------*/
void init_dij(root, h)
int root, h;
{
  int i;
  struct node *ptr_node=star_node;
  
  for(i=0; i<h; i++)
    heap[i] = -1;
  for(i=0; i<=new_n_node; i++, ptr_node++)
    {
      if(ptr_node->dist!=INT_MAX)
	{
	  ptr_node->dist=INT_MAX;
	  ptr_node->perm=0;
	  ptr_node->pred = -2;
	  ptr_node->pred_arc = -2;
	  ptr_node->heap = -1;
	}
    }
  star_node[root].dist=0;
  heap[0]=root;
  star_node[root].heap=0;
}

/*============================= dijkstra() ===============================*/
/* DIJKSTRA(j, h):  one step of dijkstra.
   This function is to scan all the outgoing arcs from node j, 
	               update heap, so that the top heap has smallest distance

  function called: heapup()
--------------------------------------------------------------------------*/
void dijkstra(j, h)
int j, *h;
{
  int i, v;
  struct node p;
  struct arc q;

  p=star_node[j]; 
  for(i=0; i<p.n_arc; i++) 
    {
      q=star_arc[p.arc_list[i]]; 
             
      if(star_node[q.to].pred_arc!=p.arc_list[i]) 
	/*i.e. we only need to scan those arcs which are not on the path
	  yet. If it is on the path already, that means, its to-node's distance
	  is the smallest already(i.e. being permanent already) and could not 
	  be improved, so, we just skip it, and only scan for those which are 
	  not on the path yet. */
	{ 		
	  v=q.to;
	  if(star_node[v].perm==0) 	/* surely, we only scan for those nodes
					   which is not permanent yet */
	    {
	      if(star_node[v].dist>p.dist+q.cost) /* ok, we can improve it */
		{
		  if(star_node[v].dist==INT_MAX)  /* if its distance was INFTY
						     we have to put it into
						     the heap structure first*/
		    {
		      heap[(*h)]=v;
		      star_node[v].heap=(*h);
		      (*h)++; 		
		      if (v==0)
			{
			  printf("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKK\n");
			  exit(0);
			}
		    } 		
		  star_node[v].dist=p.dist+q.cost; 		
		  star_node[v].pred=j; 		
		  star_node[v].pred_arc=p.arc_list[i];
		  heapup( star_node[v].heap); /* update heap so that the one
						 with smalledst distance being
						 in the top */
		} 	 
	    } 	
	} 
    }
}

/*=========================== heapup() =================================*/
/*  HEAPUP(k)
    reconstruct the heap after the value of heap[k] is changed
    this function is like heapify()--also a recursive function.
    basically, it is to sort the heap struction so that every heap node's
    distance label will follow the rule that it won't be smaller than its
    parent's

  function called: itself
-------------------------------------------------------------------------*/
void heapup( k)
     
     int k;
{
  int q, qp;
  int d, dp, j;
  
  if (k>0){
    j=(k-1)/2; /* parent of k */
    q=heap[k];
    d=star_node[q].dist;
    qp=heap[j];
    dp=star_node[qp].dist;
if (q==0)
  {
    printf("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKK\n");
    exit(0);
  }
if (qp==0)
  {
    printf("KKKKKKKKKKKKKKKKKKKKKKKKKKKKKK\n");
    exit(0);
  }
    if (dp>d){
      heap[k]=qp;
      star_node[qp].heap=k;
      heap[j]=q;
      star_node[q].heap=j;
      heapup(j);
    }
  }
}

/*========================== heapify() =================================*/
/*  HEAPIFY(i, h)
    reconstruct the heap after heap[0] is deleted
    This function is not easily understandable. It uses binary tree data
    structure. Basically, it's a recursive function which will make the
    heap[0] move downwards till it is smaller than its parent.
    (because in the short_path(), we have replace the heap[0] by heap[h],
    so, the new heap[0] doesn't have the smallest distance label, and 
    this function is used to update the new heap structure so that the
    new heap[0] will be the one with smallest distance label again.

  function called: itself
-------------------------------------------------------------------------*/
void heapify( i, h)

int i, 
    h;                 /* heap[0], heap[1], ..., heap[h] */
{
  int q, ql, qr;
  int d, dl, dr, j;

  if ((h<=1))
    return;
  
  j=2*(i+1)-1;         /* left child */
  if (j<=h)            /* left child exists */
    {
      q=heap[i];
      d=star_node[q].dist;
      ql=heap[j];
      dl=star_node[ql].dist;
      if(j<h)          /* right child exists */
	{
	  qr=heap[j+1];
	  dr=star_node[qr].dist;
	  if(dl>dr)
	    {
	      j++;
	      dl=dr;
	      ql=qr;
	    }
	}
      if(dl<d)
	{
	  heap[i]=ql;
	  star_node[ql].heap=i;
	  heap[j]=q;
	  star_node[q].heap=j;
	}
      heapify( j, h);
    }
/*print_heap(h);*/
}

/*========================= update_network() =============================*/
/* This step is executed after we assigned a unit of demand successfully.
   The basic procedures are:
   1. trace the path backwoards. set a new data struction path[], 
      define path[0]=VN destination node(i.e. 8*org_n_node+2)
   2. update the networks
      Loops for those arcs on the assigned PATH, 1 by 1
          if the current_arc's flag is ON
	      set its capacity(u)=1; incease its cost (+10000 now)
	      Loops for its corresponding arcs on other wavelength Network
	            if the corresponding arc's flag is ON
		           turn it OFF; set its capacity(u)=1
		    else its flag is OFF
		           increase its capacity(u) by 1
			   (for such case, we don't change its flag/cost)
	   else the current_arc's flag is OFF
	      decrease its capacity(u) by 1;
	      if its capacity(u) becomes 0 after decreasing 1
	            turn its flag ON; increase its cost (+10000 now)
	      else 
	            no change
	      we do not update its correponding arcs on other wavelength
            
   function called: none 
-------------------------------------------------------------------------*/
void update_network()
{
  int i=0, k=0;
  int curr_arc_org_no=0;

  /* Now, trace the path backwards */
  temp_node=new_dest_node;
  path[0]=temp_node;          /* 1st node in path[] is the VN destination */
  path_nodes=0;               /* I define path_nodes to be the index of the
			         path node . i.e. path[0]=8*org_n_node+2
				 path[1]=path[0]'s predecessor .....till
				 path[path_nodes]==8*org_n_node+1 */

  while ((temp_node!=new_root_node)&&(temp_node!=-1))
                              /* loop to trace the path. in fact, it's 
				 impossible that temp_node=-1 here, becuause
				 I have filtered this possibility in 
				 short_path() (in the case when heap[0]==0)
				 anyway, it does not matter that I add it here
				 again */
    {
      path_nodes++;
      temp_node=star_node[temp_node].pred;
      path[path_nodes]=temp_node;
    }
  
  /* [8n+1]th node, i.e. the VSource root_node */
  printf(" %d(%d) ", path[path_nodes], star_node[path[path_nodes]].org_no);
  
  /* now we focus on the 2nd node on the path */
  i=path_nodes-1;
  
  printf(" -> %d(%d) ", path[i], star_node[path[i]].org_no);
  
  /*_____________________________________________________________*/
  /* now, we can start the update part 
     inside the virtual network of some wavelength 
     it is from the 2nd node to the last 2nd node  
  */
  for(i=path_nodes-2;i>0;i--) 
    /* i=the 3rd node to the last 2nd node in the path, which travels on 
       the path in the virtual network 
    */
    {
      curr_arc=star_node[path[i]].pred_arc;      /* temp arc pointer */
      curr_arc_org_no=star_arc[curr_arc].org_no;
      if (star_arc[curr_arc].flag==1) /* block for arcs with flag-ON */
	{
	  star_arc[curr_arc].u=1;
	  
	  /* increase cost for current arc, because we are reserving it */
	  /* !!Note!! now I only use 10000 for increment, this amount 
	     may be some function in the future */
	  star_arc[curr_arc].cost+=10000;
	  
	  /* now updating their corresponding arcs in other Virutal Networks*/
	  /* I am using an efficient way, unlike what I used in as2.c */
	  for (k=1;k<=8;k++)
	    {
	      if (k!=star_arc[curr_arc].wavelength)
		{
		  if (star_arc[curr_arc_org_no+(k-1)*org_n_arc].flag==1)
		    {
		      star_arc[curr_arc_org_no+(k-1)*org_n_arc].flag=0;
		      star_arc[curr_arc_org_no+(k-1)*org_n_arc].u=1;
		    }
		  else
		    star_arc[curr_arc_org_no+(k-1)*org_n_arc].u++;
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
      
      printf("-> %d(%d) ", path[i], star_node[path[i]].org_no);     
    }
  
  /* output the final arc which connect to the VN sink(now i=0) */
  
  printf(" ->%d(%d) COST=[%d] Wave{%d}\n", path[i], star_node[path[i]].org_no,
	 star_node[path[i]].dist, star_node[star_node[path[i]].pred].wavelength);
  
  /* sometimes, it is impossible to reach the destination from the root,
     in such case, we have to report it 
     !!Note!! This step may be redundant, and may be deleted later */
  if (temp_node==-1)
    {
      printf("-> impossible!!\n");
      for (i=0;i<path_nodes;i++)
	path[i]=-1;
    }
}



/*======================== print_network() ===============================*/
/* This function is mainly for debugging, it will output the network data
   including nodes/arcs and associated data.

  function called: none
--------------------------------------------------------------------------*/
void print_network()
{
  int i;
  struct arc *ptr_arc;
  struct node *ptr_node;
  
  
  printf("%d nodes, %d arcs\n",new_n_node, new_n_arc);
  printf("Arcs flows:\n");

  for(i=0, ptr_arc=star_arc;i<new_n_arc;i++, ptr_arc++)
    {
    printf("%3d (%2d): %3d->%3d  C:%9d U:%9d F:%1d W:%1d\n", i, ptr_arc->org_no, ptr_arc->from, ptr_arc->to, ptr_arc->cost, ptr_arc->u, ptr_arc->flag, ptr_arc->wavelength);
  }
  printf("Nodes:\n");
  for (i=0,ptr_node=star_node;i<=new_n_node;i++,ptr_node++)
    {
    printf
      ("#%3d (%2d): W:%1d pred=%3d, p_arc=%3d heap=%3d dist=%6d n_arc=%2d perm=%2d \n", ptr_node->no, ptr_node->org_no, ptr_node->wavelength, ptr_node->pred, ptr_node->pred_arc, ptr_node->heap, ptr_node->dist, ptr_node->n_arc, ptr_node->perm);
  }
  /*  exit(0);*/
}

/*============================ print_node() =============================*/
/* This function is mainly for debugging, it will output the node data
   ESPECIALLY, it will print out the outgoing arcs for each node

   function called: none
-------------------------------------------------------------------------*/
void print_node()
{
  int i,j;
/*  struct arc *ptr_arc;*/
  struct node *ptr_node;

   for (i=0,ptr_node=star_node;i<=new_n_node;i++,ptr_node++)
  {
/*    printf("%d ",ptr_node->no);*/
/*    for (j=0; j<ptr_node->n_arc; j++)
      printf("star_node[%d].arc_list[%d]=%d\n", i,j,star_node[i].arc_list[j]);
*/
    for (j=0; j<ptr_node->n_arc; j++)
      printf(" %d -> %d", star_arc[star_node[i].arc_list[j]].from, star_arc[star_node[i].arc_list[j]].to);
    printf("\n");

  }
  printf("\n");
}

/*======================== print_artiarc() =============================*/
/* This is just used for checking the data, see if something wrong for my code.
-------------------------------------------------------------------------*/
void print_artiarc()
{
  int i;
  arc_t *ptr_arc;
  node_t *ptr_node;

  
  printf("%d nodes, %d arcs\n",new_n_node, new_n_arc);
  printf("Arcs flows:\n");

  for(i=0, ptr_arc=star_arc;i<new_n_arc;i++, ptr_arc++)
  {
    if (ptr_arc->org_no==0)
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

/*=========================== print_heap(h) =============================*/
/* This function is to print out the heap struction, to see if something 
  wrong for my code.
---------------------------------------------------------------------------*/
void print_heap(int h)
{
  int i=0;
  
  while ((i<=h))
    {
      if (heap[i]==0)
	printf("OHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOH\n");
      if (heap[i]>new_n_node)
	printf("MANYMANYMANYMANYMANYMANYMANYMANY\n");
      printf("heap[%d]=%d(%d)[%d]  ",i,heap[i],star_node[heap[i]].org_no,star_node[heap[i]].dist);
      i++;
    }
  printf("===>h=%2d\n",h);
}
