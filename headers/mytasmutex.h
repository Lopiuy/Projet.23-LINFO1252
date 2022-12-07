#ifndef MYTASMUTEX_H
#define MYTASMUTEX_H

int testAndSet(int* verrou,int a);

void lock(int *verrou);

void unlock(int *verrou);

#endif //MYTASMUTEX_H