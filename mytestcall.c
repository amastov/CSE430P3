//-----------------------------------------------------------------------------
// Project 3 Userspace code.
//-----------------------------------------------------------------------------
 
#include <stdio.h>
#include <stdlib.h> 
#define __NR_sys_get_addr 359
 
 
int main(int argc, char* argv[])
{
    //unsigned long vfn;
    long pleaseWork =0 ;
    //unsigned long physical;
    unsigned long temp = strtoul(argv[2],NULL,16);
    printf("argc: %d\n", argc);
    if(argc == 3)
    {

        printf("before\n");
        pleaseWork = syscall(__NR_sys_get_addr, atoi(argv[1]),temp) ;
        printf("after\n");
 
        pleaseWork = ( 0xfffff000 & pleaseWork);
        printf("%lu\n",pleaseWork);

        printf("0x%x\n",pleaseWork);

    }
    else
    {
        printf("ERROR: Two Arguments expected! Number of Arguments:%d\n\n", (argc - 1));
    }

}
