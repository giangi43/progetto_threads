#include "header.h"

/*
verifica l'effettivo funzionamento della pipe
*/
void pipeCeck(int *p){
    if(pipe(p)==-1) {
        perror("pipe call");
        _exit(1);
    }
}

// void mutexCeck(pthread_mutex_t *m){
//     if(pthread_mutex_init(m, NULL) != 0) {
//         perror("mutex creation");
//         _exit(1);
//     }
// }

void mutexLock(pthread_mutex_t *m, char nomeMutex[]){
    // char statoMutex[] = "waiting";
    // if(pthread_mutex_trylock(m)==0){
    //     return;
    // }else{
    //     printMutexDebugLog(DEBUGGING_MUTEX, nomeMutex, statoMutex);
        
    //     pthread_mutex_timedlock(m,&deltatime);
    // }
    pthread_mutex_lock(m);
}
void mutexUnlock(pthread_mutex_t *m, char nomeMutex[]){
    //char statoMutex[] = "False";
    //printMutexDebugLog(DEBUGGING_MUTEX, nomeMutex, statoMutex);
    pthread_mutex_unlock(m);
}

void scrivi (struct proprietaOggetto *personaggio){
    //printStringCharDebugLog(DEBUGGING,"%c: ",&personaggio->segnaposto[0]);
    //printStringIntDebugLog(DEBUGGING, "%d, sta provando a scrivere\n",&personaggio->istanza);
        char str[20];
        strcpy(str,"scrittura ");
        strcat(str, personaggio->segnaposto);
        mutexLock(&lock, str);
        push(codaProprieta,personaggio);
        mutexUnlock(&lock,"scrittura");
    
    //printStringIntDebugLog(DEBUGGING, "%d, ha scritto\n",&personaggio->istanza);
}

void leggi ( struct proprietaOggetto *valore_letto){
    
    printStringIntDebugLog(DEBUGGING2, "sta provando a leggere %d\n",&debugIndex);

    *valore_letto = pop(codaProprieta);
    
    printStringCharDebugLog(DEBUGGING2,"%c: ",&valore_letto->segnaposto[0]);
    printStringIntDebugLog(DEBUGGING2, "%d, è stato letto\n",&valore_letto->istanza);

}



/*
versione modificata del forkSwitch
    anzi che dividere il processo in due da cui partono due funzioni diverse,
    crea un processo figlio che si stacca dal primo lasciando l'esecuzione del padre immutata
*/
pthread_t myThreadCreate(struct proprietaOggetto* personaggio, void* (*figlio) (void*)){
    printStringCharDebugLog(DEBUGGING,"%c: ",&personaggio->segnaposto[0]);
    printStringIntDebugLog(DEBUGGING, "%d, sto creando il tread\n",&personaggio->istanza);
    printProprietaOggettoDebugLog(DEBUGGING,personaggio);
    fflush(NULL);

    pthread_create(&personaggio->tid,NULL,figlio,personaggio);
    printStringIntDebugLog(DEBUGGING, "%d, tread creato\n",&personaggio->istanza);
    fflush(NULL);
    return personaggio->tid;
}



/*
    crea una serie di processi figli 
    con personaggio e comportamento passati come parametro
*/
void creaGruppoPersonaggi(struct proprietaOggetto personaggio[],  void* (*figlio) (void*), int numeroPersonaggi){
    int i;
    for (i = 0; i < numeroPersonaggi; i++)
    {
        myThreadCreate(&(personaggio[i]),figlio);
               
    }    
}


/*
    libera il buffer in caso di errori di gestione
*/
void freeTheBuffer(int pipein,struct proprietaOggetto valore_letto){    
    printStringIntDebugLog(DEBUGGING2,"dentro free the buffer! %d\n ", &debugIndex);

    while(sizeof(valore_letto)<=fcntl(pipein, F_SETFL, O_NONBLOCK)) // pipe non bloccante
    {
        printStringIntDebugLog(DEBUGGING,"free the buffer! %d\n", &debugIndex);
    }
}

/*
    chiude una serie di processi
*/
void killThemAll(struct proprietaOggetto personaggio[], int numeroPersonaggi){
    int i;
    printStringCharDebugLog(DEBUGGING," kill Them All! %c \n", &personaggio[0].segnaposto[0]);
    for (i=0; i<numeroPersonaggi; i++)
    {
        killIt(&personaggio[i]);       
    }
}


/*
    chiude un processo 
    assicurandosi che fosse aperto in precedenza.
    lancia un errore nel log in caso di probabile errore
*/
void killIt(struct proprietaOggetto *personaggio){
    //printProprietaOggettoDebugLog(personaggio->pid==0&&personaggio->oldX!=-1, personaggio);
    //printStringIntDebugLog(personaggio->pid==0&&personaggio->oldX!=-1," !!!!!!!!\nkillit pid = 0 per istanza = %d \n!!!!!!!!!\n", &(personaggio->istanza));

    if (personaggio->tid!=0){
        personaggio->isAlive=false;
        deletePropietaOggetto(personaggio);
        //kill(personaggio->pid,1);
        printStringIntDebugLog(DEBUGGING,"killit in esecuzione %d\n",&debugIndex);
        //fflush(NULL);
        
        
        printStringIntDebugLog(DEBUGGING_NEEDED,"killit eseguita %d\n",&debugIndex);
        pthread_join(personaggio->tid,NULL);
        personaggio->tid = 0;  
        deletePropietaOggetto(personaggio); 
        aliveProcesses--;
    }  
}





int checkContacts(struct proprietaOggetto *personaggioA, struct proprietaOggetto arrayPersonaggiB[], int numeroPersonaggiB){
    int index=0;
    index = indexOfWhoIsSameLocationArray(personaggioA,arrayPersonaggiB, numeroPersonaggiB);    
    if(index >=0){ 
        
        //mutexLock(&lifes,"vite");
         
        personaggioA->vite--;
        
        if (personaggioA->vite<=0){            
            //mutexUnlock(&lifes,"vite");
            killIt(personaggioA);
        }
        
        //mutexUnlock(&lifes,"vite");
        //mutexLock(&lifes,"vite");
        
        arrayPersonaggiB[index].vite--;
         
        if (arrayPersonaggiB[index].vite<=0){
           // mutexUnlock(&lifes,"vite");
            killIt(&arrayPersonaggiB[index]);
        }

        //mutexUnlock(&lifes,"vite");
        return index;
    }    
    return index;
}