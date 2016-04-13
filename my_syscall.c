#include <linux/syscalls.h>
#include <linux/kernel.h>    /* Needed for KERN_INFO */
#include <linux/list.h>
#include <linux/sched.h>
#include <linux/tty.h> /* Needed to access TTY stuff */
#include <linux/jiffies.h> /* Needed for time */
#include <linux/printk.h> /* Needed to print to the kernal log file */
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/string.h>
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/types.h>
//#include <linux/shmem_fs.h>
//#include <linux/swapops.h>
 
 
struct data
{
   int aval;
   long address;

};

#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

asmlinkage long sys_my_syscall(void* pnt, int pid, const unsigned long address)
{
   
   struct task_struct* task;
   struct mm_struct* mm;
   pgd_t* pgd;
   pud_t* pud;
   pmd_t* pmd;
   pte_t* pte;
   struct page *page;
   printk(KERN_INFO "adfgdfrgsfd\n");
   struct data *outputData = kmalloc(sizeof(struct data),__GFP_NOFAIL);
   printk(KERN_INFO "ADDRESS: %lu\n", address);
   for_each_process(task)
   {
       if(task->pid == pid)
       {
            pr_info("IN\n");            
            printk(KERN_INFO "Task %d found\n", task->pid);
            mm = task->mm;
            page = NULL;
            pgd = pgd_offset(mm, address);
 
            printk(KERN_INFO "PGD INFO: PRESENT: %d, NONE: %d, BAD: %d\n", pgd_present(*pgd), pgd_bad(*pgd), pgd_none(*pgd));
            if(!(pgd_none(*pgd) || pgd_bad(*pgd)) && pgd_present(*pgd))
            {
 
                pud = pud_offset(pgd, address);

                printk(KERN_INFO "PUD INFO: PRESENT: %d, NONE: %d, BAD: %d\n", pud_present(*pud), pud_bad(*pud), pud_none(*pud));
                   
                if(!(pud_none(*pud) || pud_bad(*pud)) && pud_present(*pud))
                {
                   
                    pmd = pmd_offset(pud, address);
                    printk(KERN_INFO "PMD INFO: PRESENT: %d, NONE: %d, BAD: %d\n", pmd_present(*pmd), pmd_bad(*pmd), pmd_none(*pmd));
                   
                    if(!(pmd_none(*pmd) || pmd_bad(*pmd)) && pmd_present(*pmd))
                    {
                         
                        pte = pte_offset_kernel(pmd, address);
                        printk(KERN_INFO "PTE INFO: PRESENT: %p\n", &pte);
 						//#define CHECK_BIT(var,pos) ((var) & (1<<(pos)))

                        //if(!CHECK_BIT(pte,63))
                        if(pte)
                        {
                        	page = pte_page(*pte);
                        	if(pte_present(*pte))
                        	{
                            	
                           		outputData->address = pte_pfn(*pte);
                            	outputData->aval = 1;
                            	copy_to_user(pnt, outputData, sizeof(struct data));
                            	return 0;
                        	}
                    
                        	else
                        	{
                        		outputData->address = __pte_to_swp_entry(*pte).val;
                           		//aval is set to swap.
                            	outputData->aval = 2;
                            	copy_to_user(pnt, outputData, sizeof(struct data));
                            	return 0;
                        	}
                        }
                    }
                }
            }
            outputData->aval = 0;
            printk(KERN_INFO "Data not found!\n");
            copy_to_user(pnt, outputData, sizeof(struct data));
            return 0;
        }
    }
    return 0;
}