
#include <stdio.h>
#include "../RH_math.h" 
int main(int argc, char const *argv[])
{
	printf("%ld\n" , __pascal_triangle(6,0));
	printf("%ld\n" , __pascal_triangle(6,1));
	printf("%ld\n" , __pascal_triangle(6,2));
	printf("%ld\n" , __pascal_triangle(5,3));
	printf("%ld\n" , __pascal_triangle(5,4));
	printf("%ld\n" , __pascal_triangle(5,5));
	return 0;
}
