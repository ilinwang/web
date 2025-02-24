#include<iostream>
#include <stdio.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
using namespace std;

int main(int argc, char *argv[])
{
	char input_name[200], alg_name[200]="FWA";
	FILE *pFile;
	char string [9999];
	float number;
	int n,m,i,j,k;
	float **d,**td;
	int **p,**tp;
	strcpy(input_name,argv[1]);
	if ((pFile=fopen(input_name,"r"))==NULL)
		{printf("Cannot open %s, job skipped!!\n",input_name);exit(0);}
		do
		{
			fgets (string , 2 , pFile);
			switch(tolower(string[0]))
			{
			case 'c':
				fgets (string , 100 , pFile);
				break;
			case 't':
				fscanf (pFile, "%s", string);
				break;
			case 'p':
				fscanf (pFile, "%s", string);
				fscanf (pFile, "%f", &number);
				n=static_cast<int>(number);
				fscanf (pFile, "%f", &number);
				m=static_cast<int>(number);
				d=new float*[n];
				p=new int*[n];
				td=new float*[n];
				tp=new int*[n];
				for(i=0;i<n;i++)
				{
					d[i]=new float[n];
					p[i]=new int[n];
					td[i]=new float[n];
					tp[i]=new int[n];
				}
				for(i=0;i<n;i++)
					for(j=0;j<n;j++)
					{
						if(i==j)
							td[i][j]=0;
						else
							td[i][j]=99999999;
						tp[i][j]=i;
					}
				break;
			case 'n':
				fscanf (pFile, "%f", &number);
				break;
			case 'a':
				int temp1,temp2;
				float temp3;
				fscanf (pFile, "%f", &number);
				temp1=static_cast<int>(number)-1;
				fscanf (pFile, "%f", &number);
				temp2=static_cast<int>(number)-1;
				fscanf (pFile, "%f", &number);
				temp3=number;
				if(td[temp1][temp2]>temp3)
					td[temp1][temp2]=temp3;
				break;
		   }
		}while (!feof(pFile));
	fclose (pFile);
	int loop;
	double n_loop=1;//loop¦¸¼Æ
	double compare=0;
	clock_t t1,t2;
	for(loop=0;loop<n_loop;loop++)
	{
		for(i=0;i<n;i++)
		{
			for(j=0;j<n;j++)
			{
				d[i][j]=td[i][j];
				p[i][j]=tp[i][j];
			}
		}
		t1 = clock ();
		for(k=0;k<n;k++)
		{
			for(i=0;i<n;i++)
			{
				for(j=0;j<n;j++)
				{
					if(j!=k&&i!=k&&i!=j)
					{
						compare++;
						if(d[i][j]>d[i][k]+d[k][j])
						{
							d[i][j]=d[i][k]+d[k][j];
							p[i][j]=p[k][j];
						}
					}
				}
			}
		}
		t2 = clock ();
	}
	double time=t2-t1;
	cout<<alg_name<<" "<<input_name<<" "<<time/CLOCKS_PER_SEC<<endl;
	return 0;
}
