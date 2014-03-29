#include <stdio.h>
#include <math.h>
int a=20000;

int main(){
	char loA=(char)((a<<8)>>8);
	char hiA=(char)((a>>8)<<8);
	printf("%c\n",hiA);
	printf("%c\n",loA);
}
