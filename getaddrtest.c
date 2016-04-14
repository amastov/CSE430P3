//-----------------------------------------------------------------------------
// Project 3 Userspace code.
//-----------------------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>

#define __NR_sys_get_addr 359

#define CHECK_BIT(v, pos) ((v) & (1 << pos)) // Checks the bit at the given position and returns 1 or 0.
#define PAGE_SWAP_BIT 62
#define GET_OFFSET(pte) ((pte) & (0b000111110000000000000000000000000000000000000000000000000000000))// ... I'm not even sure about this
#define GET_PFN(pte) ((pte) & (0b0000000001111111111111111111111111111111111111111111111111111111)) // Should be right
#define GET_SWAP_OFFSET(pte) ((pte) & (0b0000000001111111111111111111111111111111111111111111111111100000)) // Should be right
#define GET_VFN(v_addr) ((v_addr) & (0xFFFFF000)) // This seemed to work before
#define GET_IN_SWAP(pte) CHECK_BIT(pte, PAGE_SWAP_BIT) // yes, yes I did

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
		unsigned long v_addr = strtoul(argv[2],NULL,16); // we expect the virtual address in hex form for simplicity

		printf("[ %s ] PID: %d, Virtual Address: 0x%lx\n", argv[0], pid, v_addr);
		unsigned long pte = syscall(__NR_sys_get_addr, pid, v_addr) ;

		printBits(sizeof(pte), &pte); // DEBUG
		
		if(pte != 0)
		{
			printf("[ %s ] PFN: %d, VFN: %d, OFFSET: 0x%dx, ", argv[0], GET_PFN(pte), GET_VFN(v_addr), GET_OFFSET(pte));
			if(GET_IN_SWAP(pte))
			{
				// Address is in swap
				printf("SWAP: 0xlx\n", );
			}
			else
			{
				// Address is in memory
				printf("PHYS_ADDR: 0x%lx\n", );
			}
		}
		else
		{
			printf("[ %s ] Data Not Available.", argv[0]);
		}
	}
	else
	{
		printf("[ %s ] ERROR: Two Arguments expected! Number of Arguments:%d\n\n", argv[0], (argc - 1));
	}
}
