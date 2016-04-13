//-----------------------------------------------------------------------------
// Project 3 Userspace code.
//-----------------------------------------------------------------------------
 
#include <stdio.h>
#include <stdlib.h> 
#define __NR_sys_get_addr 359
 
struct data
{
    int aval;
    unsigned long address;

};
 
int main(int argc, char* argv[])
{
	unsigned long vfn;
	//unsigned long physical;
    unsigned long temp = strtoul(argv[2],NULL,16);
    if(argc == 3)
    {
        struct data *pointer = malloc(sizeof(struct data));
        // argv[1] is the pid
        // argv[2] is the virtual memory address
        syscall(__NR_sys_get_addr, pointer, atoi(argv[1]),temp) ;
        // Output data
        if(pointer->aval == 1) // in memory 
        {
        	//(? << 12) | (0b111111111111 & address)

        	vfn = (0xfffff000 & temp) >> 12;
        	printf("%08x\n",temp);
        	//printf("%08x\n",pointer->address + 0xC00000000);
        	printf("%08x\n",vfn);
        	//printf("%08x\n",0b111111111111);



            printf("PFN: %lu\tVFN: %lu\tPHYSICAL: 0x%x\n", pointer->address, vfn,(pointer->address << 12) |  vfn );

        }
        else if(pointer->aval == 2) // swap 
        {
        	vfn = (0xfffff000 & temp) >> 12;
        	printf("VFN: %lu\tSWAP: %lu\n", vfn, pointer->address);
        }
        else
        {
            printf("not available\n");
           // exit(0);
            return 0;
        }
       
    }
    else
    {
        printf("ERROR: Two Arguments expected! Number of Arguments:%d\n\n", (argc - 1));
    }
    return 0;
}
