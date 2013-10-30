#include <stdio.h>
extern int* buf;

int* bufp0 = &buf[0];
int* bufp1;


void swap(){
    int temp;

    bufp1 = &buf[1];
    temp = *bufp0;
    *bufp0 = *bufp1;
    *bufp1 = temp;
}

class Foo {
public:
    Foo(){};
    int m;
};


//Foo::Foo(int i):m(i){ 
//    printf("hehe");
//}

Foo g_dycdyc;
