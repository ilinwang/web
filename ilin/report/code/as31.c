/**************************************************************************/
/*This is the 1st C code for my assignment algorithm.
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "as3.h"


void main()
{
  /* read network data, duplicate virtual networks, output to topo2.dat */

  FILE *fptopo, *fpmino;
  int i=0, j=0, k=0, h=0;
  int curr_arc=0, unit;
 


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
/*  if ((fpvtopo1=fopen("vtopo1.dat", "w"))== NULL) 
    {
      fprintf(stderr, " Error opening vtopo1.dat\n");
      exit(1);
    }
 */
 
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

/*  fprintf(fpvtopo1,"%d %d\n", new_n_node, new_n_arc );*/
  
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
    }
  star_node[j].no=j;      /* source of VN */
  star_node[j+1].no=j+1;  /* sink of VN   */


  for (i=new_n_arc-16;i<new_n_arc;i++)
    star_arc[i].org_no=i;



  /* ---------------------------------------read Virutal Network data */
  while(fscanf(fpmino, "%d %d %d", &org_root_node, &org_dest_node, 
	       &amount)!=EOF) 
    {
      new_root_node=new_n_node-1;
      new_dest_node=new_n_node;

      for (i=0;i<=new_n_node;i++)
	{
	  star_node[i].dist=INFTY;
	  star_node[i].perm=0;
	  star_node[i].pred=-2;
	  star_node[i].pred_arc=-2;
	  star_node[i].heap=-1;
	  star_node[i].n_arc=0;
	}  

      for(i=0;i<(new_n_arc-16);i++)  /* read 8 virutal networks data */
	{
	  star_node[star_arc[i].from].n_arc++;  
	  if ((star_arc[i].from==new_n_node-1)&&
	    (star_node[star_arc[i].from].n_arc>8))
	    print_network();
	  
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
	  /*star_node[star_arc[curr_arc].to].n_arc++;*//*we only use outgoing*/
	  
	  curr_arc++;
	  star_arc[curr_arc].to= new_n_node;
	  star_arc[curr_arc].from=org_dest_node+j*org_n_node;
	  star_arc[curr_arc].cost=0;
	  star_arc[curr_arc].u=U;
	  
	  star_node[star_arc[curr_arc].from].n_arc++;
	  /*star_node[star_arc[curr_arc].to].n_arc++;*//*we only use outgoing*/
	}
      
      if (! init_arc_list())
	exit(0);
      
/*print_network();
	      exit(0);
      print_node(); */
      
      /* Now, begin the shortest path part */
      for (unit=0;unit<amount;unit++)  /* loop for # amount with same source/sink */
	{
	  root=new_root_node;
	  h=new_n_node+1;
	  
	  init_dij(root, h);
/*printf("________________________unit=%d_________\n",unit);	  */
	  n_perm=0;
	  h=1;
	  j=heap[0];
	  star_node[j].heap= -1;
	  h--;
	  star_node[j].perm=1;
	  n_perm++;
	  if (heap[0]==0)
	    print_heap();  
	  while(n_perm < new_n_node)  /*loop to finish assigning 1 unit by
					Dijkstra */
	    {                         /*later I should make this faster by
					exitting the loop once I got sink
					permanent ! !!!!Not yet now!!!!*/
	      dijkstra(j, &h);
	      /*  printf("%d\n",k);*/
	      /*print_node();*/
	      j=heap[0];
	      star_node[j].heap= -1;
	      h--;
	      star_node[j].perm=1;
	      n_perm++;
	      heap[0]=heap[h];
	      
	      heap[h]=-1;
	      
	      star_node[heap[0]].heap=0;
	      heapify(0, h-1);
	    }
/*if(star_node[new_dest_node].pred==-2)*/
  {
    printf("^^^^^^^^^^^^^ unit=%d ^^^^^^\n",unit);
 /*   print_network();*/
/*    print_node();*/
    print_heap();
/*    exit(0);*/
  }
	  
	  /* Now, trace the path */
	  temp_node=new_dest_node;
	  path[0]=temp_node;
	  path_nodes=0;
	  /*printf("the path from %d to %d is  ", new_root_node, temp_node);*/ 
	  
	  while ((temp_node!=new_root_node)&&(temp_node!=-1))
	    {
	      path_nodes++;
/*printf("1.temp_node=%d\n",temp_node);  why star_node[50].pred=-1?? */
	      temp_node=star_node[temp_node].pred;
	      path[path_nodes]=temp_node;
/*printf("2.temp_node=%d\n",temp_node);  why star_node[50].pred=-1?? */      
/*	      printf("$$$$ star_node[%d].org_no=%d $$$$$$$$$\n", path[path_nodes],  star_node[path[path_nodes]].org_no);*/
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
	      printf("\n@@@@@@@@@@@@@@@@star_node[%d].org_no=%d   org_root_node=%d@@@@@@@@@@@@\n", path[i],star_node[path[i]].org_no,org_root_node );
	      print_artiarc();
	      exit(0);
	    }

	  /*_____________________________________________________________*/
	  /* now, we can start the update part */
	  /* inside the virtual network of some wavelength */
	  /* it is from the 2nd node to the last 2nd node  */
	  for(i=path_nodes-2;i>0;i--) 
	    /* i=the 3rd node to the last 2nd node in the path, which t
	       ravels on the path in the virtual network */
	    {
	      curr_arc=star_node[path[i]].pred_arc;
	      if (star_arc[curr_arc].flag==1) /* block for arcs with flag-ON */
		{
		  star_arc[curr_arc].u=1;
		  
		  /* increase cost for current arc, because we are reserving it */
		  /* !!Note!! now I only use 10000 for increment, this amount 
		     may be some function in the future */
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
    }
  /*print_network();*/
  printf("%d\n",k);
  clearerr(fptopo);
  fclose(fptopo);
  clearerr(fpmino);
  fclose(fpmino);

  
  
exit(0);



}

/*======================== memory_allocation() =============================*/
/*
**    MEMORY_ALLOCATION()
**    allocate memories for the basic data structure 
*/
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
  if ((bk_star_node=(struct node *)malloc((new_n_node+1)*sizeof(struct node)))==NULL)
  {
    /* Output error and return failure if failed to allocate memory */
    printf("Cannot allocate memory, job terminated\n");
    return(0);
  }
  if ((bk_star_arc=(struct arc *)malloc((new_n_arc)*sizeof(struct arc)))==NULL)
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
void reset_network()
{ 
  int i,j;
  for (i=0;i<=new_n_node;i++)
    {
      star_node[i].dist=INFTY;
      star_node[i].no=i;
      star_node[i].pred=-1;
      star_node[i].pred_arc=-1;
      star_node[i].n_arc=0;
      star_node[i].current_arc=0;
      /* star_node[i].arc_list do not reset */
    }
  for (j=0; j<new_n_arc; j++)
    {
      star_arc[j].from=0;
      star_arc[j].to=0;
      star_arc[j].cost=0;
      star_arc[j].u=0;
      star_arc[j].flag=0;
    }
}

/*======================== init_arc_list() =============================*/
int init_arc_list()
{ 
  int i, node;
  struct arc *ptr_arc;

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

/*======================== print_network() =============================*/
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
      ("#%3d (%2d): W:%1d pred=%3d, p_arc=%3d heap=%3d dist=%6d n_arc=%2d current_arc=%2d \n", ptr_node->no, ptr_node->org_no, ptr_node->wavelength, ptr_node->pred, ptr_node->pred_arc, ptr_node->heap, ptr_node->dist, ptr_node->n_arc, ptr_node->current_arc);
  }
/*  exit(0);*/
}

/*======================== print_node() =============================*/
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

/*======================== init_dij() =============================*/
/*
**    INIT_DIJ(root, h)
**    initialize for dijkstra
*/
void init_dij(root, h)
int root, h;
{
  int i;
  struct node *ptr_node=star_node;
  
  for(i=0; i<h; i++)
    heap[i] = -1;
  for(i=0; i<=new_n_node; i++, ptr_node++)
    {
      if(ptr_node->dist!=INFTY)
	{
	  ptr_node->dist=INFTY;
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

/*======================== dijkstra() =============================*/
/*
**    DIJKSTRA(j, h)
**    one step of dijkstra
*/
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
      if (q.from!=j)
	exit(0);
      if(star_node[q.to].pred_arc!=p.arc_list[i])
	{ 		
	  v=q.to;
	  if(star_node[v].perm==0) 		
	    {
	      if(star_node[v].dist>p.dist+q.cost)	
		{ 		 
		  if(star_node[v].dist==INFTY)
		    {
		      heap[(*h)]=v;
		      star_node[v].heap=(*h);
		      (*h)++; 			
		    } 		
		  star_node[v].dist=p.dist+q.cost; 		
		  star_node[v].pred=j; 		
		  star_node[v].pred_arc=p.arc_list[i];
		  heapup( star_node[v].heap);
		} 	 
	    } 	
	} 
    }
}

/*======================== heapify() =============================*/
/*
**    HEAPIFY(heap, i, h)
**    reconstruct the heap after heap[0] is deleted
*/
void heapify( i, h)

int i, 
    h;                 /* heap[0], heap[1], ..., heap[h] */
{
  int q, ql, qr;
  int d, dl, dr, j;
  
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
}

/*======================== heapup() =============================*/
/*
**    HEAPUP(heap, k)
**    reconstruct the heap after the value of heap[k] is changed
*/
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

/*======================== print_artiarc() =============================*/
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

/*------------------ print_heap() --------------*/
void print_heap()
{
  int i=0;
  
  while ((i<=new_n_node)&&(heap[i]!=-1))
    {
      if (heap[i]==0)
	printf("OHOHOHOHOHOHOHOHOHOHOHOHOHOHOHOH\n");
      if (heap[i]>new_n_node)
	printf("MANYMANYMANYMANYMANYMANYMANYMANY\n");
      printf("heap[%d]=%d  ",i,heap[i]);
      i++;
    }
  printf("\n");
}
