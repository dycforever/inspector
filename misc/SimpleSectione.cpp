/* 
* SimpleSection.c* 
* Linux:
*   gcc -c SimpleSection.c** Windows:
*   cl SimpleSection.c /c /Za
*/
#include<stdio.h>

int printf( const char* format, ... );

extern "C" int global_init_var = 84;
extern "C" int global_uninit_var;

extern "C" void func1( int i ) {printf( "%d\n",  i );}

int main(void){
	static int static_var = 85;
	static int static_var2;    
	int a = 1;
	int b;     
	func1( static_var + static_var2 + a + b );    
	return a; 
}

