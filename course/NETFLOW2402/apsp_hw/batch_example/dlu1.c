/*
  This code is an example to show how to use commond line to read the 
  network input filename, and then
  execute the shortest path algorithm and measure its running time.

  to compile:   gcc -o dlu1 dlu1.c
  to run:       dlu1 ./inputs/xxx.sp

*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
  char input_name[200], alg_name[200]="dlu";
  FILE *input;
  int n,m,n_loop;
  double time=0.;

  strcpy(input_name,argv[1]);
  if ((input=fopen(input_name,"r"))==NULL)
    {printf("Cannot open %s, job skipped!!\n",input_name);exit(0);}
  
  n_loop=9; /* assume # loop=9 */

  /* this is where you should put your main shortest path function */

  time=10.1; /* suppose your shortest path function takes 100.1 units
		 of time */

  printf("%s %s %lf %d %lf\n",input_name,alg_name,time,n_loop,time/n_loop);

  fclose(input);
}
