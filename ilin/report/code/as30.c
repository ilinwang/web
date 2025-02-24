/**************************************************************************/
/*This is the 1st C code for my assignment algorithm.
***************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include "as3.h"


void main()
{
  /* read network data, duplicate virtual networks, output to topo2.dat */
  /*read_network(arc_list, &nodes, &arcs, &amount, &high_node, &root_node, 
    &dest_node, node_list);*/
  FILE *fptopo, *fpmino, *fpvtopo1;
  int i=0, j=0, k=0, from, to, cost, u;
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
  if ((fpvtopo1=fopen("vtopo1.dat", "w"))== NULL) /*open virtual network file*/
    {
      fprintf(stderr, " Error opening vtopo1.dat\n");
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

  fprintf(fpvtopo1,"%d %d\n", new_n_node, new_n_arc );
  
  /* ---------------------------------------read original data */
  for (i=0;i<(org_n_arc);i++) /* loop to read original network topology data 
			    and output to the virtual network data file */
    {
      fscanf(fptopo, "%d %d %d %d", &from, &to, &cost, &u);
  
      /* loop to duplicate 8 virutal networks */
      for (j=0;j<=7;j++) 
	fprintf(fpvtopo1, "%d %d %d %d\n", (from+j*org_n_node), 
		(to+j*org_n_node), cost, u);
      high_node=new_n_node;
    }    
  fclose(fpvtopo1);
  if ((fpvtopo1=fopen("vtopo1.dat", "r"))== NULL)
    {
      fprintf(stderr, " Error opening vtopo1.dat\n");
      exit(1);
    }

  /* ---------------------------------------read Virutal Network data */
  while(fscanf(fpmino, "%d %d %d", &root_node, &dest_node, &amount)!=EOF) 
    {
      /* read new 8 virtual networks data 
	 from here, nodes=new_n_node;arcs=new_n_arc*/
      fscanf(fpvtopo1, "%d %d", &nodes, &arcs); 

      reset_network();

      k=0;
      for(i=0;i<org_n_arc;i++)
	{
	  for (j=0;j<8;j++)
	    {
	      fscanf(fpvtopo1, "%d %d %d %d", &(star_arc[k].key),
		     &(star_arc[k].to), &(star_arc[k].cost), &(star_arc[k].u));
	      
	      star_arc[k].from=star_arc[k].key;
	      star_node[star_arc[k].from].n_arc++;
	      /*star_node[star_arc[i].to].n_arc++;*//*we only use outgoing */
	      star_arc[k].wavelength=j+1;
	      star_arc[k].org_no=i;
	      star_arc[k].flag=-1;
	      k++;
	    }
	}
      
      
      /* node is from 1 to new_n_node; */
      for(j=1;j<=(nodes-2);j++)
	{
	  star_node[j].wavelength=j/org_n_node+1;
	  star_node[j].org_no=j%org_n_node;
	  if (star_node[j].org_no==0)
	    {
	      star_node[j].org_no=org_n_node;
	      star_node[j].wavelength--;
	    }
	}
      
      printf("---------------------\n");

       

      
      for (j=0;j<=7;j++) /* loop to make the arcs connecting from virutal 
			    source to all corresponding sources nodes in 8 
			    networks, and arcs connecting from all 
			    corresponding sink nodes in 8 networks to the 
			    virtual sink node */
	{
       	  curr_arc=new_n_arc-16+2*j;

	  star_arc[curr_arc].to= root_node+j*org_n_node;/* only this changes */
	  star_arc[curr_arc].key= new_n_node-1;
     	  star_arc[curr_arc].from=star_arc[curr_arc].key;
	  star_arc[curr_arc].cost=0;
	  star_arc[curr_arc].u=U;
	  
	  star_node[star_arc[curr_arc].from].n_arc++;
	  /*star_node[star_arc[curr_arc].to].n_arc++;*//*we only use outgoing*/
	  
	  curr_arc++;
	  star_arc[curr_arc].to= new_n_node;
	  star_arc[curr_arc].key= dest_node+j*org_n_node;/*only this changes*/
	  star_arc[curr_arc].from=star_arc[curr_arc].key;
	  star_arc[curr_arc].cost=0;
	  star_arc[curr_arc].u=U;
	  
	  star_node[star_arc[curr_arc].key].n_arc++;
	  /*star_node[star_arc[curr_arc].to].n_arc++;*//*we only use outgoing*/
	}

      if (! init_arc_list())
	exit(0);
      
      /* Now, begin the short-test path part */

      print_network();
      /* print_node();*/
    }
/*  print_network();*/
  
  clearerr(fptopo);
  fclose(fptopo);
  clearerr(fpmino);
  fclose(fpmino);
  clearerr(fpvtopo1);
  fclose(fpvtopo1);
  
  
exit(0);



}

/*======================== memory_allocation() =============================*/
/*
**    MEMORY_ALLOCATION()
**    allocate memories for the basic data structure 
*/
int memory_allocation()
{

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

  return (1);
}

void reset_network()
{ 
  int i,j;
  for (i=0;i<=new_n_node;i++)
    {
      star_node[i].key=i;
      star_node[i].no=i;
      star_node[i].pred=-1;
      star_node[i].pred_arc=-1;
      star_node[i].n_arc=0;
      star_node[i].current_arc=0;
      /* star_node[i].arc_list do not reset */
    }
  for (j=0; j<new_n_arc; j++)
    {
      star_arc[j].key=0;
      star_arc[j].from=0;
      star_arc[j].to=0;
      star_arc[j].cost=0;
      star_arc[j].u=0;
      star_arc[j].flag=0;
    }
}

int init_arc_list()
{ 
  int i, node;
  struct arc *ptr_arc;
  struct node *ptr_node;
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


void print_network()
{
  int i;
  struct arc *ptr_arc;
  struct node *ptr_node;

  
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
      ("#%3d (%2d): W:%1d pred=%3d, dist=%9d n_arc=%2d current_arc=%2d\n", ptr_node->no, ptr_node->org_no, ptr_node->wavelength, ptr_node->pred, ptr_node->dist, ptr_node->n_arc, ptr_node->current_arc);
  }
  exit(0);
}

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
