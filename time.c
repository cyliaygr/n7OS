#include <n7OS/sys.h>
#include <n7OS/syscall_defs.h>
#include <n7OS/console.h>
#include <n7OS/irq.h>
#include <n7OS/cpu.h>
#include <unistd.h>
#include <stdio.h>
#include <debug.h>
#include <string.h>

#include <n7OS/time.h>

// Constantes pour les ports et les canaux
#define TIMER_CONTROL_PORT 0x43
#define TIMER_DATA_PORT 0x40
#define CHANNEL_0 0x00
#define ACCESS_MODE_LOW_HIGH 0x30 // Poids faible/poids fort
#define TIMER_MODE_GENERATOR 0x02 // Générateur d'impulsion

// Fréquence de l'horloge
#define CLOCK_FREQUENCY 1193180
#define DESIRED_FREQUENCY 1000 // 1 kHz


extern void handler_IT_timer();

int time = 0;
uint16_t FREQUENCE = 1190; // facteur (1.19 Mhz / 1 kHz = 1190)

void init_timer()
{
    // Écriture de la configuration dans le registre de contrôle/commande
    outb(0x34, 0x43);

    // Écriture du diviseur de fréquence (poids faible puis poids fort)
    outb(FREQUENCE & 0xFF, 0x40);
    outb(FREQUENCE >> 8, 0x40);

    // Ajout de l'interruption
    init_irq_entry(0x20, (uint32_t)handler_IT_timer);

    // Démasque l'interruption
    outb(inb(0x21) & 0xfe, 0x21);
}

// Fonction de conversion du compteur système vers hh:mm:ss et d'affichage
void display_system_runtime() {

    int s = time / 1000;
    int m = s / 60;
    int h = m / 60;

    display_time(h, m, s);
}

void handler_timer()
{
    // Masquage de l'interruption
    outb(inb(0x21) | 1, 0x21);
    // Acquittement de l'IT
    outb(0x20, 0x20);
    // Incrémentation cpt systeme

    // Démasquage de l'it
    outb(inb(0x21) & 0xfe, 0x21);

    // Mise à jour du cpt à l'écran
    if (time % 1000 == 0)
    { 
        display_system_runtime();
    }
    time++; 
}

