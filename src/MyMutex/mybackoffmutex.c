#include "../../headers/mybackoffmutex.h"

int testAndSet(int* verrou,int a){
    int ret;
    asm (
            "movl %2, %%eax;"
            "xchg %%eax, %1;"
            "movl %%eax, %0"
            :"=r"(ret), "=m"(*verrou)
            :"r"(a)
            :"%eax");
    return ret;
}

void lock(int *verrou,int init, int max){
    int wait = init;
    while (testAndSet(verrou,1)){
        while(*verrou);
        for (int i = 0; i < wait; i++);
        if(wait<max){
            wait = 2*wait;
        }
    }
}


void unlock(int *verrou) {
    testAndSet(verrou,0);
}