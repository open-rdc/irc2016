#include <stdio.h>
#include <math.h>
#include <stdlib.h>
 
#define NUM 5
#define A 0.9
//PSD‚©‚ç15cm‚ÌêŠ

int comp( const void *c1, const void *c2 )
{
  int tmp1 = *(int *)c1;
  int tmp2 = *(int *)c2;

  if( tmp1 < tmp2 )  return -1;
  if( tmp1 > tmp2 )  return  1;
  return 0;
}

int main(void){
	FILE *file;
    int i,j;
    float w[104],x[100],sort[NUM];
	
	file=fopen("PSD.txt","r");
	
	for(i=0;i<104;i++){
		fscanf(file,"%f",&w[i]);
	}
	fclose(file);
	
	for(i=4;i<104;i++){
		//x[i]=(w[i]+w[i-1]+w[i-2]+w[i-3]+w[i-4])/NUM;
		//x[i]=(A*w[i-1])+(1-A)*w[i];
		for(j=0;j<NUM;j++){
			sort[j]=w[i-j];
		}
		qsort(sort,NUM,sizeof(float),comp);
		x[i]=sort[(NUM-1)/2];
		printf("%f\n",x[i]);
	}
	
	file=fopen("median.txt","w");
	
	for(i=4;i<104;i++){
		fprintf(file,"%f\n",x[i]);
	}
	fclose(file);
	
    return 0;
}