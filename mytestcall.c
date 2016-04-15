//-----------------------------------------------------------------------------
// Project 3 Userspace code.
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#define __NR_sys_get_addr 359

#define CHECK_BIT(v, pos) ((v) & (1l << pos)) // Checks the bit at the given position and returns 1 or 0.
#define PAGE_SWAP_BIT 0
#define GET_OFFSET(pte) ((pte) & (0b11111111111111111111000000000000))//
#define GET_PFN(pte) (((pte) & (0b11111111111111111111000000000000)) >> 12) // WORKS
#define GET_SWAP_OFFSET(pte) ((pte) & (0b11111111111111111111000000000000)) 
#define GET_VFN(v_addr) (((v_addr) & (0xFFFFF000)) >> 12) // WORKS
#define GET_IN_SWAP(pte) CHECK_BIT(pte, PAGE_SWAP_BIT) 


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
    if(argc == 3)
    {
        int pid = atoi(argv[1]);
        unsigned long vfn;
        unsigned long v_addr = strtoul(argv[2],NULL,16); // we expect the virtual address in hex form for simplicity

        printf("[ %s ] PID: %d, Virtual Address: 0x%lx\n", argv[0], pid, v_addr);
        unsigned long pte = syscall(__NR_sys_get_addr, pid, v_addr) ;

        if(pte != 0)
        {
            printf("Output: 0x%lx or %d \n", GET_OFFSET(pte) , (int)pte );
        }
        else
        {
            printf("[ %s ] Data Not Available.\n", argv[0]);
        }
    }
    else
    {
        printf("[ %s ] ERROR: Two Arguments expected! Number of Arguments:%d\n\n", argv[0], (argc - 1));
    }
}