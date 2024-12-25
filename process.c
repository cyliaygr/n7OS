#include <n7OS/process.h>
#include <string.h>
#include <n7OS/cpu.h>
#include <stdio.h>

extern void idle();
extern void processus1();

/* PROCESSUS COURANT */
static process_t *proc_courant;

/* COMPTEUR PROCESS/PID (pour l'ordonnancement)*/
pid_t cpt_pid = 0;

extern void ctx_sw(void *ctx_old, void *ctx_new);

/* HORLOGE */
static time_t time = 0;

void increment_time() {
    time++;
}

/* INITIALISATION TABLEAU DE PROCESSUS */
void init_procTable(){
    for (int i = 0; i < NB_PROC; i++){
        procTable[i].etat = PROC_VIDE;
    }
}

/* CRÉATION DE PROCESSUS */
pid_t fork(const char *name, fnptr function) {
    printf("Création %s\n", name);

    if (cpt_pid >= NB_PROC){
        printf("dépassement tableau de processus \n");
    } 

    process_t *proc;

    for (int i = 0; i < NB_PROC; i++) {
        proc = &procTable[cpt_pid];
        if (proc->etat == PROC_VIDE) { // case vide dispo
            printf("case vide trouve \n");
            proc->pid = cpt_pid;
            proc->etat = PROC_PRET;
            //printf("pid process : %d \n", proc->pid);
            printf("Processus %s est %d \n", proc->nom_proc, proc->pid);
            strcpy(proc->nom_proc, name);

            // initialiser pile
            memset(proc->pile.pile_exec, 0, sizeof(proc->pile.pile_exec));
            proc->pile.pile_exec[SIZE_STACK - 2] = (uint32_t)function; // on stocke la fn au sommet de pile
            // initialiser ctx
            memset(&proc->contexte, 0, sizeof(proc->contexte));
            // stocker le ctx en sommet de pile
            proc->contexte.registres[1] = (uint32_t)&proc->pile.pile_exec[SIZE_STACK - 2]; //PROBLÈME PILE
            return proc->pid;
       }

       cpt_pid++;
        
    }
   
    // erreur lors de la création du processus
    return -1;
}

/* GETTER PID */
pid_t getPid() {
    return proc_courant->pid;
}

/* ORDONNANCEMENT DES PROCESSUS */
void scheduler() {
    printf("Debut d'ordonnancement \n");
    pid_t pid_courant = getPid();
    pid_t nouveau_pid = -1;

    // Ordo pour trouver le prochain process pret
    for (pid_t i = 1; i < NB_PROC; i++) {
        pid_t pid = (pid_courant + i) % NB_PROC;
        if (procTable[pid].etat == PROC_PRET) {
            nouveau_pid = pid;
            break;
        }
    }
    printf("Nouveau process %i en cours \n", nouveau_pid);

    // cas ou on schedule entre 2 processus existants
    if (nouveau_pid != -1) {
        if (proc_courant->etat == PROC_ELU) {
            // on place le processus courant en état pret
            proc_courant->etat = PROC_PRET;
        }
        // on donne la main au proc suivant
        procTable[nouveau_pid].etat = PROC_ELU;
        proc_courant = &procTable[nouveau_pid];
        ctx_sw(&procTable[pid_courant].contexte, &procTable[nouveau_pid].contexte);
    } else {
        printf("Ordonnancement impossible : 0 processus pret \n");
    }
}

/* TERMINER PROCESS */
int exit() {
    printf("Exit de %s\n", proc_courant->nom_proc);
    // changement état
    proc_courant->etat = PROC_VIDE;
    memset(proc_courant, 0, sizeof(process_t));
    scheduler();
    return 0; 
}

/* BLOQUER PROCESS */
int sleep(int seconds) {
    printf("Sleep de %d for %d seconds \n", proc_courant->pid, seconds);
    proc_courant->reveil = time + seconds;
    proc_courant->etat = PROC_BLOQUE;
    scheduler();
    return 0; 
}

/* INITIALISATION D'UN PROCESSUS */
void init_process() {
    init_procTable();
    pid_t pidProc = fork("idle", (fnptr)idle);
    fork("processus1", (fnptr)processus1);
    proc_courant = &procTable[pidProc];
    proc_courant->etat = PROC_ELU;
    idle();
}
