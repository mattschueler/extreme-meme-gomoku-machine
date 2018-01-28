
#ifndef _STDLIB_H
#include <stdlib.h>
#endif
#define unfilled 0
#define white 1
#define black 2
#ifndef BLOCK_SIZE
#define BLOCK_SIZE 15
#endif
#ifndef INT_SIZE
#define INT_SIZE 64
#endif

int get__x(int __y, int __x);
void set__x(int __y, int __x, int value);
#include <stdio.h>


long long unsigned int* x = (long long unsigned int*) calloc(8, sizeof(long long unsigned int));
int main(int argc, char** argv){
	
	set__x(0,0, black);
printf("%d",get__x(0,0));
}

inline int get__x(int __y, int __x){
    //which int we will look at for retrieving the bitarray value
    int intID = (__x*BLOCK_SIZE + __y)/INT_SIZE;

    //how many bits we will shift our selector before grabbing our bits
    int intoffset = (__x*BLOCK_SIZE + __y) % INT_SIZE;
    return (x[intID] & (3 << intoffset)) >> intoffset;
};
inline void set__x(int __y, int __x, int value){
    //which int we will look at for setting the bitarray value
    int intID = (__x*BLOCK_SIZE + __y)/INT_SIZE;

    //how many bits we will shift our selector before grabbing our bits
    int intoffset = (__x*BLOCK_SIZE + __y) % INT_SIZE;
    x[intID] = (x[intID] & ~(3 << intoffset)) | (value & (3 << intoffset));
};
