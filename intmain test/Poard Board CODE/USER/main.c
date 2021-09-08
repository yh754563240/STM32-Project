#include "stm32f10x.h"
#include "stm32f10x_it.h"
#include "stdio.h"
#include "stdlib.h"
#include "uart.h"
#include "string.h"
#include "clock.h"
//int *p;

void Exchg2(int *px, int *py)
{
  int tmp=*px;
  *px=*py;
  *py=tmp;
  printf("*px=%d,*py=%d\n",*px,*py);
}
void Exchg1(int x, int y)
{
	int temp;
	temp = x;
	x = y;
	y = temp;
	printf("x=%d y=%d",x,y);
}

 


int main(void)
{

		
uart_init();

//	int a = 1;
//	int b = 2;
////Exchg1(a,b);
//Exchg2(&a,&b);
//	printf("a=%d b=%d",a,b);
	
//	i=&o;
//	for(i=&o;*i<5;*i=*i+1)
//		
//	{
//		printf("*i=%d ",*i);
//	}
//	while(1)
//	{

//	}

  int i = 6; 
	int * p = (int *)malloc(sizeof(4));
//int *p;
//	p = &i;
* p = i;
printf("%d",*p);
	free(p); 
//	*p = NULL;
//printf("%d",*p);



}
	
