#include <n7OS/mem.h>
#include <stdbool.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


static bool pageTable[MEMORY_SIZE]; // Table de pagination


/**
 * @brief Marque la page allouée
 * 
 * Lorsque la page a été choisie, cette fonction permet de la marquer allouée
 * 
 * @param addr Adresse de la page à allouer
 */
void setPage(uint32_t addr) {
    uint32_t pageIndex = addr / PAGE_SIZE;
    pageTable[pageIndex] = true;

}

/**
 * @brief Désalloue la page
 * 
 * Libère la page allouée.
 * 
 * @param addr Adresse de la page à libérer
 */
void clearPage(uint32_t addr) {
    int pageIndex = addr / PAGE_SIZE;
    pageTable[pageIndex] = false;
    

}

/**
 * @brief Fourni la première page libre de la mémoire physique tout en l'allouant
 * 
 * @return uint32_t Adresse de la page sélectionnée
 */
uint32_t findfreePage() {


    for (uint32_t i = 0; i < MEMORY_SIZE; i++) {
        if (!pageTable[i]) {
            pageTable[i] = true; // Marque la page comme allouée
            return i * PAGE_SIZE; // Retourne l'adresse de la page
        } 
    } // Si aucune page n'est disponible, retourne 0 (adresse invalide)

    return 0xFFFFFFFF;
}

/**
 * @brief Initialise le gestionnaire de mémoire physique
 * 
 */
void init_mem() {
    for (uint32_t i = 0; i < MEMORY_SIZE; i++) {
        pageTable[i] = false;
    }
}

/**
 * @brief Affiche l'état de la mémoire physique
 * 
 */
void print_mem() {
    for (uint32_t i = 0; i < MEMORY_SIZE; i++) {
        printf("Page %d: %s\n", i, pageTable[i] ? "Allouée" : "Libre");
    }
    
}
