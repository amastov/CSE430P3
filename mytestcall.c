//-----------------------------------------------------------------------------
// Project 3 Userspace code.
//-----------------------------------------------------------------------------
 
#include <stdio.h>
#include <stdlib.h> 
#define __NR_sys_get_addr 359
 
 


void printBits(size_t const size, void const * const ptr)
{
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = b[i] & (1<<j);
            byte >>= j;
            printf("%u", byte);
        }
    }
    puts("");
}


int main(int argc, char* argv[])
{
    //unsigned long vfn;
    long pleaseWork =0 ;
    //unsigned long physical;
    unsigned long temp = strtoul(argv[2],NULL,16);
    printf("Address:");
    if(argc == 3)
    {

        //printf("before\n");
        pleaseWork = syscall(__NR_sys_get_addr, atoi(argv[1]),temp) ;
        //printf("after\n");

        printf("%lu\n\n\n",pleaseWork);

        printBits(sizeof(pleaseWork), &pleaseWork);

        pleaseWork = ( 0xfffff000 & pleaseWork);
        //printf("%lu\n",pleaseWork);

        printf("0x%x\n",pleaseWork);

    }
    else
    {
        printf("ERROR: Two Arguments expected! Number of Arguments:%d\n\n", (argc - 1));
    }

}