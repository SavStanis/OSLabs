#include<stdio.h>
#include "unistd.h"


void new_func1(void);

void FirstFuction(void)
{
    for(int i = 0; i < 100; i++)
	{
	}

    return;
}

static void SecondFunction(void)
{
    for(int i = 0; i < 100; i++)
	{
	}
    return;
}
static void ThirdFunction(void)
{
    for(int i = 0; i < 100; i++)
	{
	}
    return;
}

int main(void)
{
    printf("\n Inside main()\n");
	int numoffunc;
	scanf("%i", &numoffunc);
	
    int i = 0;

    for(;i<0xffffff;i++);
	switch(numoffunc){
	case 1:
	    FirstFuction();
		break;
	case 2:
		SecondFunction();
		break;
	case 3:
		ThirdFunction();
		break;
	}

    return 0;
}