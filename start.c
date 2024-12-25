#include <n7OS/cpu.h>
#include <inttypes.h>
#include <n7OS/processor_structs.h>
#include <n7OS/console.h>
#include <unistd.h>
#include <n7OS/sys.h>
#include <stdio.h>


extern void handler50();
extern void handler_IT_timer();

void kernel_start(void)
{
    // test affichage pagination
    uint32_t pagedir = initialise_paging();
    setup_base(pagedir);
    init_console();


    // initialisation des interruptions
    //init_irq_entry(50, (uint32_t)handler50);

    // Activation des IT
    sti();
    

    init_syscall();
    // Activation du timer
    init_timer();
    
    printf("0x%x\n", pagedir);
    printf("\f");
    printf("Bienvenue...\n");

    init_process();
    


    // Envoi de l'IT numéro 50
    //__asm__ ("int $50" ::);
   
   // desactivation des IT
   //cli();

     uint32_t *ptr = (uint32_t *)0x10000;
     uint32_t do_page_fault = *ptr; 
     do_page_fault ++;

    

    //on ne doit jamais sortir de kernel_start
    while (1) {
        // cette fonction arrete le processeur
        hlt();
    }
}
