
#include <math.h>
#include <time.h>
#include <sys/types.h>
#include <sys/times.h>
#include <limits.h>

#define NULL_NODE -1
#define MAX_NODE 50000

#define MAX_ARCS 250000
#define MAX_NODES 50000
#define INFTY 1e30
#define M 1e30	      /* a very high cost */
#define U 999999999	      /* a very big capacity */


struct arc
{
  int key,     /* from, used as key for sort, may be deleted later */
      from,
      to,
      cost,
      u,
      flag,
      org_no,      /* record its origianl arc no. */
      wavelength;  /* record its corresponding wavelength */
};
typedef struct arc arc_t;
typedef arc_t *Parc_t;

struct node
{
  int key,   /* this may be useless, and may be deleted in the future */
      no,   
      dist, 
      pred, 
      pred_arc,
      n_arc,
      current_arc,   /* a pointer to the array arc_list */
      *arc_list,   /* list of adjacent arcs, keeping the indeces in star_arc */
      heap,        /* a pointer to the heap array */
      d_pred,      /* predecessor of the node in dijkstra */
      d_pred_arc,  /* predecessor arc of the node in dijkstra */
      org_no,      /* record its origianl node no. */
      wavelength;  /* record its corresponding wavelength */
};
typedef struct node node_t;
typedef node_t *Pnode_t;


/*------ globol variables ------*/

struct node *star_node;  /* Pointer to array of node list for VN*/
struct arc  *star_arc;   /* Pointer to array of arc list for VN*/
struct node *bk_star_node; /* pointer to array of node list for Physical N*/
struct arc  *bk_star_arc;  /* Pointer to array of arc list for Physical N*/

int org_n_node, org_n_arc, new_n_node, new_n_arc;
int root_node, dest_node;
int nodes, arcs, amount;
int high_node, org_root_node, org_dest_node;

void print_network(), reset_network(), print_node();
int memory_allocation(), init_arc_list();
