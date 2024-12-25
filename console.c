#include <n7OS/console.h>
#include <n7OS/cpu.h>

#define SCREEN_WIDTH 80
#define SCREEN_HEIGHT 25

uint16_t *scr_tab;
uint16_t ligne, colonne; //ligne et colonne du curseur
char couleur;
uint16_t pos;

void init_console() {
    scr_tab = (uint16_t *) SCREEN_ADDR;
    ligne = 0;
    colonne = 0;
    // 3 premiers bits: clignotement, 3 suivants : fond, 3 derniers : couleur
    couleur = 0b00001101;
    set_cursor(ligne * SCREEN_WIDTH + colonne);
}

/* Positionne le curseur */ 
void set_cursor(int pos) {
    // Envoi de commandes au contrôleur de l'écran pour déplacer le curseur

    outb(CMD_LOW, PORT_CMD); // envoi de la commande 15 (0xF)
    outb((unsigned char)(pos & 0xFF), PORT_DATA); // envoi de la valeur du poids faible de la position
    
    outb(CMD_HIGH, PORT_CMD); // Envoi de la commande 14 (0xE)
    outb((unsigned char)((pos >> 8) & 0xFF), PORT_DATA); // Envoi de la valeur du poids fort
    
}

/* A COMPLÉTER*/
void console_putchar(const char c) {
    //if (c == 'a') 
        //scr_tab[0]= couleur<<8|c;

    if (c > 31 && c < 127) {
        //affiche le char à l'écran
        scr_tab[ligne * SCREEN_WIDTH + colonne] = couleur << 8 | c; //utiliser getcursor
        colonne++;
        if (colonne >= SCREEN_WIDTH) {
            colonne = 0;
            ligne++;
        }
        if (ligne >= SCREEN_HEIGHT) {
            ligne = 0;
        }
        set_cursor(ligne * SCREEN_WIDTH + colonne);

    } else if (c == '\n') { // ou ( c == 10 )
        //placer le curseur au début de la ligne suivante
        colonne = 0;
        ligne++;
        if (ligne >= SCREEN_HEIGHT) {
            ligne = 0;
        }
        set_cursor(ligne * SCREEN_WIDTH + colonne);

    } else if ( c == '\b') {
        //Déplace le curseur d’une colonne en arrière
        if (colonne > 0) {
            colonne--;
            set_cursor(ligne * SCREEN_WIDTH + colonne);
        }
        
    } else if ( c == '\t') {
        //Ajoute un espace de 8 caractères
        for (int i = 0; i < 8; i++)
            console_putchar(' ');
    
    } else if ( c == '\f') {
        //Efface l’écran et revient à la colonne 0 de la ligne 0
        for (int i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++) {
        scr_tab[i] = couleur << 8 | ' ';
        }
        colonne = 0;
        ligne = 0;
        set_cursor(ligne * SCREEN_WIDTH + colonne);

    } else if ( c == '\r') {
        //Déplace le curseur à la colonne 0 de la ligne courante
        colonne = 0;
        set_cursor(ligne * SCREEN_WIDTH + colonne);
    }
}

/* Affiche le caractère passé en argument ou demande le déplacement du curseur */
void console_putbytes(const char *s, int len) {
    for (int i= 0; i<len; i++) {
        console_putchar(s[i]);
    }
}

/* Affiche le l'heure à l'écran (timer) */
void display_time(int h, int m, int s)
{
        // Sauvegarde de la position actuelle
        uint16_t ligne_old = ligne;
        uint16_t colonne_old = colonne;

        // Placer le curseur en haut à droite de la console
        colonne = 72;
        ligne = 0;

        printf("%02d:%02d:%02d", h % 24, m % 60, s % 60);
 
        // Retour à l'ancienne position
        ligne = ligne_old;
        colonne = colonne_old;
        
    
}





 


