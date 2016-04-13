#include <linux/syscalls.h>
#include <linux/kernel.h>    /* Needed for KERN_INFO */
#include <linux/list.h>


#include <linux/printk.h> /* Needed to print to the kernal log file */



#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/types.h>

#include <asm/pgtable.h>
#include <linux/highmem.h>



asmlinkage long sys_my_syscall( int pid, const unsigned long address)
{
   
   struct task_struct* task;
   struct mm_struct* mm;
   pgd_t* pgd;
   pud_t* pud;
   pmd_t* pmd;
   pte_t* pte;
   
   printk(KERN_INFO "ADDRESS: %lu\n", address);
   for_each_process(task)
   {
       if(task->pid == pid)
       {
            pr_info("IN\n");            
            printk(KERN_INFO "Task %d found\n", task->pid);
            mm = task->mm;
            
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
                        printk(KERN_INFO " after pmd if before pte = pre offset"); 
                        pte = pte_offset_map(pmd, address);
                        printk(KERN_INFO "PTE INFO: PRESENT: %d\n", pte_present(*pte));

                        if(pte_present(*pte))
                        {
                          printk(KERN_INFO " after pte if "); 

                          if(!pte_none(*pte))
                          {
                            printk(KERN_INFO " !pte_none(*pte) \n"); 

                            return pte->pte;
                            //return 0;
                          }
                          else
                          {
                            printk(KERN_INFO " in else \n"); 

                              return __pte_to_swp_entry(*pte).val;
                            //return 0;
                          }
                          
                        }
                    }
                }
            }

            printk(KERN_INFO "Data not found!\n");
        }
    }
    printk(KERN_INFO "Data not found!\n");
    return 0;
}