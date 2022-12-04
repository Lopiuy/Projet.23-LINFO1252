#ifndef MYTTASMUTEX_H
#define MYTTASMUTEX_H

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

void lock(int *verrou){
    while (testAndSet(verrou,1)){
        while(*verrou);
    }
}


void unlock(int *verrou) {
    testAndSet(verrou,0);
}


#endif //MYTTASMUTEX_H
