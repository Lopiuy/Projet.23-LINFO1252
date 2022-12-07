#ifndef MYTTASMUTEX_H
#define MYTTASMUTEX_H

int testAndSet(int* verrou,int a);

void lock(int *verrou);

void unlock(int *verrou);

#endif //MYTTASMUTEX_H