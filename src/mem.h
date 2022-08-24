#pragma once

#define FREE_MEM 0x10000;

void* memset(void* dest, unsigned char val, int count){ 
	unsigned char* destC = (unsigned char*)dest;
	int i;
	for (i = 0; i < count; i++)
		destC[i] = val;
	return dest;
}

/* Copy blocks of memory */
void memcp(unsigned char* source, unsigned char* dest, int count){
    for (int i = 0; i < count; i++)
        *(dest + i) = *(source + i);
}

int freeMem = FREE_MEM;

void initializeMem(){
    freeMem = FREE_MEM;
}

/* allocate space linearly (Welcome to the worst allocation method ever), starting from address 0x10000 */
void* kmalloc(int size){
    memset((void*)freeMem, 0, size); 
    void* address = (void*)freeMem;
    freeMem += size;
    return address;
}
/* "free" last bytes of memory */
void memcut(int size) {freeMem -= size;}

/* free the whole dynamic memory */
void memreset(){freeMem = FREE_MEM;}


/* the following two functions can be called at the beginning and end of a function; they implement a sort of "scoping" for dynamic memory, that makes the whole system a bit less shit */
int tmpMem;
void memrec(){
    tmpMem = freeMem;
}

void memunrec(){
    freeMem = tmpMem;
}

int getFreeMem(){
   return freeMem; 
}

mem_t memget(){
    mem_t* mem = (mem_t*)0x5000;
    return *mem;
}
