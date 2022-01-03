#include "delay.h"
void delay_ms(unsigned int ms)
{
	unsigned int a;
	while(ms)
	{
		a=1800;
		while(a--);
		ms--;
	}
	return;
}
