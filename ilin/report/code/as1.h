/***************************************************************************/
/* fstar.h - Header for putting arbitrary network into forward star format */
/* 1.204, spring 1995 
   This program is desigend by I-Lin Wang and Gary Wong.
   There are 9 files needed to be compiled:
   short_main.c: main program, contains procedures of this problem
   fstar_lib.h: header file for fstar_lib.c
   fstar_lib.c: lib programs for doing forward star data structure for arcs
   sort_lib.h: header file for sort_lib.c
   sort_lib.c: lib programs for selections sort.
   short.h: header file for short.c
   short.c: main program for doing Dijkstra's Algorithm for shortest path
   int_min_heap.h: header file for int_min_heap.c
   int_min_heap.c: lib programs for doing heaps, modified so that the top heap
                   with smallest key

   To compile this problem: 
       gcc short_main.c short.c fstar_lib.c sort_lib.c int_min_heap.c
***************************************************************************/
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/times.h>
#include <limits.h>

#define NULL_NODE -1
#define MAX_NODE 5000

#define MAX_ARCS 25000
#define MAX_NODES 5000
#define INFTY 1e30
#ifndef FSTAR_H
#define FSTAR_H
struct arc
{
  int key;     /* from, used as key for sort */
  int to;
  int no;
  int cost;
  int capy;
  int flag;
};
typedef struct arc sort_t;
#endif

struct node
{
  int key;  /*!!NOTE!!: I put it in the first element since later we will use*/
  int no;   /* info_t structure to sort node in heap, and info_t only has one*/
  int dist; /* element-->key, so, I must put key to be 1st element here, then*/
  int pred; /* when heap sorts for key, it will work !! */
  int arcs;   /* no. of outgoing arcs of this node */
  int exist;  /* 1 if exists; 0 o.w. */
  int pred_arc;
};
typedef struct node node_t;

#define MAX_HEAP 10000

#ifndef INFO_T
#define INFO_T
struct info
{
  int key;
};
typedef struct info info_t;
#endif

int org_nodes, org_arcs, nodes, arcs, amount;
int path[1000000];


extern void read_network(sort_t arc_list[], int *nodes, int *arcs, int *amount, int *high_node, int *root_node, int *dest_node, node_t node_list[]);

extern void forward_star(sort_t arc_list[], int arcs, 
				int high_node, int head[], node_t node_list[]);

extern void output_forward_star(sort_t arc_list[], int arcs, 
				       int high_node, int head[]);

/* extern void update_network(sort_t arc_list[], node_t node_list[]);*/

/***************************************************************************/
/* short.h shortest path algorithm, label setting */
/* 1.204, spring 1995 
***************************************************************************/


extern void s_short(sort_t arc_list[], int arcs, int high_node, int head[], 
             int root_node, int dest_node, node_t node_list[]);

extern void output_short(node_t node_list[], int high_node, int root_node, int dest_node);

/***************************************************************************/
/* int_min_heap.h-Header file for array priority queue data structure routines
   Operates on integer key value in info_t structure */
/***************************************************************************/

extern void print_heap(node_t *heap[], int heap_size);

extern int node_in_heap(node_t *heap[], int no, int heap_size);

extern void construct_heap(info_t *heap[], int n); 

extern void insert_heap(info_t *val, info_t *heap[], int *n);

extern info_t *remove_heap(info_t *heap[], int *n);

extern info_t *replace_heap(info_t *val, info_t *heap[], int n);

extern void heapsort_heap(info_t *heap[], int n);

extern void up_heap(int k, info_t *heap[]);

extern void down_heap(int k, info_t *heap[], int n);

/***************************************************************************/
/* sort_lib.h - Headers for sort functions */
/* 1.204, spring 1995 
***************************************************************************/
/* Selection sort */
extern void select_sort(sort_t sort_list[], int elements);
