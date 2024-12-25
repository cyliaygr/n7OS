#include <inttypes.h>
#include <n7OS/irq.h>


void init_irq_entry(int num_line, uint32_t addr) {

    idt_entry_t *idt_entry = (idt_entry_t *)&idt[num_line];

    //Initialisation des entrées de la table d'interruption
    idt_entry->offset_inf = addr & 0xFFFF;
    idt_entry->sel_segment = KERNEL_CS;
    idt_entry->zero = 0;
    idt_entry->type_attr = 0x8E;
    idt_entry->offset_sup = (addr >> 16) & 0xFFFF;


}
