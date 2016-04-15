#include <linux/syscalls.h>
#include <linux/kernel.h>    /* Needed for KERN_INFO */
#include <linux/list.h>
#include <linux/printk.h> /* Needed to print to the kernal log file */
#include <linux/mm.h>
#include <linux/mm_types.h>
#include <linux/types.h>
#include <asm/pgtable.h>
#include <linux/highmem.h>



asmlinkage long sys_my_syscall( int pid, unsigned long address)
{
   
   struct task_struct* task;
   struct mm_struct* mm;
   pgd_t* pgd;
   pud_t* pud;
   pmd_t* pmd;
   pte_t* pte;
   unsigned long pte_val ;
   printk(KERN_INFO "PID: %d, VIRTUAL_ADDR: 0x%lx\n", pid, address);
   for_each_process(task)
   {
     if(task->pid == pid)
     {           
      printk(KERN_INFO "Task %d found\n", task->pid);
      mm = task->mm;
      
      pgd = pgd_offset(mm, address);
      printk(KERN_INFO "PGD INFO: PRESENT: %d, BAD: %d, NONE: %d\n", pgd_present(*pgd), pgd_bad(*pgd), pgd_none(*pgd));
      if(!(pgd_none(*pgd) || pgd_bad(*pgd)) && pgd_present(*pgd))
      {
        printk(KERN_INFO "PGD INFO: PRESENT: %d, BAD: %d, NONE: %d\n", pgd_present(*pgd), pgd_bad(*pgd), pgd_none(*pgd));
        pud = pud_offset(pgd, address);
        printk(KERN_INFO "PUD INFO: PRESENT: %d, BAD: %d, NONE: %d\n", pud_present(*pud), pud_bad(*pud), pud_none(*pud));
           
        if(!(pud_none(*pud) || pud_bad(*pud)) && pud_present(*pud))
        {
          printk(KERN_INFO "PUD INFO: PRESENT: %d, BAD: %d, NONE: %d\n", pud_present(*pud), pud_bad(*pud), pud_none(*pud));
          pmd = pmd_offset(pud, address);
          printk(KERN_INFO "PMD INFO: PRESENT: %d, BAD: %d, NONE: %d\n", pmd_present(*pmd), pmd_bad(*pmd), pmd_none(*pmd));
           
          if(!(pmd_none(*pmd) || pmd_bad(*pmd)) && pmd_present(*pmd))
          {
            printk(KERN_INFO "PMD INFO: PRESENT: %d, BAD: %d, NONE: %d\n", pmd_present(*pmd), pmd_bad(*pmd), pmd_none(*pmd));
            pte = pte_offset_map(pmd, address);
            printk(KERN_INFO "PTE INFO: PRESENT: %d PTE: 0x%lx \n ", pte_present(*pte), pte->pte);


            pte_val = pte->pte;
            
            if(pte_val == 0)
                pte_val = __pte_to_swp_entry(*pte).val;
            
            pte_unmap(pte);
            printk(KERN_INFO "pte_val: %lx\n" , pte_val);
            return pte_val;
            
            }
        }
      }
    }
  }
  printk(KERN_INFO "Data not found!\n");
  return 0;
}