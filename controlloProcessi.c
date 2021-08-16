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


void scrivi (struct proprietaOggetto *personaggio){
    printStringCharDebugLog(DEBUGGING,"%c: ",&personaggio->segnaposto[0]);
    printStringIntDebugLog(DEBUGGING, "%d, sta provando a scrivere\n",&personaggio->istanza);
        pthread_mutex_lock(&lock);
        push(codaProprieta,personaggio);
        pthread_mutex_unlock(&lock);
    
    printStringIntDebugLog(DEBUGGING, "%d, ha scritto\n",&personaggio->istanza);
}

void leggi (int pipein, struct proprietaOggetto *valore_letto){
    
    printStringIntDebugLog(DEBUGGING, "sta provando a leggere %d\n",&debugIndex);

    //pthread_mutex_lock(&lock);
    //read(pipein,valore_letto,sizeof(valore_letto));
    *valore_letto = pop(codaProprieta);
    //pthread_mutex_unlock(&lock);
    
    printStringCharDebugLog(DEBUGGING,"%c: ",&valore_letto->segnaposto[0]);
    printStringIntDebugLog(DEBUGGING, "%d, è stato letto\n",&valore_letto->istanza);

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
    pthread_mutex_lock(&lock);

    if (personaggio->tid!=0){
        deletePropietaOggetto(personaggio);
        //kill(personaggio->pid,1);
        printStringIntDebugLog(DEBUGGING,"killit in esecuzione %d\n",&debugIndex);
        //fflush(NULL);
        //pthread_join(personaggio->tid,NULL);
        personaggio->flag=LOST;
        printStringIntDebugLog(true,"killit eseguita %d\n",&debugIndex);
        
        personaggio->tid = 0;  
        deletePropietaOggetto(personaggio); 
        aliveProcesses--;
    }  
    pthread_mutex_unlock(&lock);    
}


/*
    controlla che proiettili e navi siano entrati in contatto e ne aggio
*/
/*bool checkContacts(struct proprietaOggetto *bersaglio, int numeroBersagli, struct proprietaOggetto *dardo, int numeroDardi, struct proprietaOggetto *scorte, int numeroScorte, int *fileDescriptor){
    int index=0;
    int i,k;
    for ( i = 0; i < numeroBersagli; i++)
    {   
        index = indexOfWhoIsSameLocationArray(&bersaglio[i],dardo, numeroDardi);
        if(index >=0){                
            bersaglio[i].vite--;
            if( bersaglio[i].vite <= 0){
                killIt(&bersaglio[i]);
                numeroNemici = numeroNemici-1;
                if( strcmp( bersaglio[i].segnaposto,SEGNAPOSTO_ALIENO)==0) {
                    for ( k = 0; k < numeroScorte; k++)
                    {            
                        setPersonaggio(&scorte[bersaglio[i].istanza*NUMERO_ALIENI_CATTIVI+k],SEGNAPOSTO_ALIENO_CATTIVO,bersaglio[i].x,bersaglio[i].y,0,VITE_ALIENI_CATTIVI,bersaglio[i].istanza*NUMERO_ALIENI_CATTIVI+k);                    
                        scorte[bersaglio[i].istanza*NUMERO_ALIENI_CATTIVI+k].oldX = bersaglio[i].oldX;
                        scorte[bersaglio[i].istanza*NUMERO_ALIENI_CATTIVI+k].oldY = bersaglio[i].oldY;
                        for (int a = 0; a < (numeroScorte-k-1)*(strlen(SEGNAPOSTO_ALIENO_CATTIVO)+1); a++)
                        {
                            spostamentoLineare(&scorte[bersaglio[i].istanza*NUMERO_ALIENI_CATTIVI+k],false);
                        }                        

                        myForkSwitch(&(scorte[bersaglio[i].istanza*NUMERO_ALIENI_CATTIVI+k]),fileDescriptor,alienoF);
                        numeroNemici = numeroNemici+1;
                    }                    
                }
                printEnemiesLeft(10, 0, numeroNemici);                
            }
            killIt(&dardo[index]);            
            return true;
        }
    }
    return false;
}*/


int checkContacts(struct proprietaOggetto *personaggioA, struct proprietaOggetto arrayPersonaggiB[], int numeroPersonaggiB){
    int index=0;
    index = indexOfWhoIsSameLocationArray(personaggioA,arrayPersonaggiB, numeroPersonaggiB);    
    if(index >=0){ 
        
        pthread_mutex_lock(&lifes);
         
        personaggioA->vite--;
        
        if (personaggioA->vite<=0){            
            pthread_mutex_unlock(&lifes);
            killIt(personaggioA);
        }
        
        pthread_mutex_lock(&lifes);
        
        arrayPersonaggiB[index].vite--;
         
        if (arrayPersonaggiB[index].vite<=0){
            pthread_mutex_unlock(&lifes);
            killIt(&arrayPersonaggiB[index]);
        }

        pthread_mutex_unlock(&lifes);
        return index;
    }    
    return index;
}