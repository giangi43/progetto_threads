#include "header.h"

void setPersonaggio (struct proprietaOggetto *proprieta_personaggio, char characterPlaceHolder[], int startingX, int startingY, pid_t pidToAssign, int viteIniziali, int istanza){
    printStringIntDebugLog(DEBUGGING2,"setPersonaggio() %d\n; ", &debugIndex);

    strncpy(proprieta_personaggio->segnaposto,characterPlaceHolder,MAX_STRING_SIZE);
    proprieta_personaggio->lunghezzaSegnaposto=strlen(proprieta_personaggio->segnaposto);

    proprieta_personaggio->x=startingX;
    proprieta_personaggio->y=startingY;
    proprieta_personaggio->oldX=-1;
    proprieta_personaggio->oldY=-1;
    proprieta_personaggio->istanza=istanza;
    //proprieta_personaggio->pid = pidToAssign;          
    proprieta_personaggio->flag = EMPTY;    
    proprieta_personaggio->vite = viteIniziali; 
    proprieta_personaggio->tid=0;
}


void copyPersonaggio (struct proprietaOggetto *copiante,struct proprietaOggetto *copiato){
    strncpy(copiante->segnaposto,copiato->segnaposto,MAX_STRING_SIZE);
    copiante->lunghezzaSegnaposto = copiato->lunghezzaSegnaposto;
    copiante->istanza=copiato->istanza;
    copiante->x=copiato->x;
    copiante->y=copiato->y;
    copiante->oldX=copiato->oldX;
    copiante->oldY=copiato->oldY;
    //copiante->pid=copiato->pid;
    copiante->flag=copiato->flag;
    copiante->vite=copiato->vite;
    copiante->tid=copiato->tid;
}

void inizializzaPersonaggi(struct proprietaOggetto *daCopiare, struct proprietaOggetto Personaggio[], int numeroPersonaggi){
    int i;
    for ( i = 0; i < numeroPersonaggi; i++)
    {
        copyPersonaggio(&Personaggio[i],daCopiare);
        Personaggio[i].istanza=i;
        printProprietaOggettoDebugLog(DEBUGGING2,&Personaggio[i]);
    }
}

void personaggioF (struct proprietaOggetto *proprieta_personaggio, int isAutonomus, char (*spostamento)(struct proprietaOggetto*,bool)){    
    int bombDrop = customRandom(10,30);
    int counter =0;

    waitTOJumpIn(proprieta_personaggio);
    
    printStringIntDebugLog(DEBUGGING,"personaggioF sta per scrivere %d\n", &proprieta_personaggio->istanza);
    fflush(NULL);
    scrivi(proprieta_personaggio);  
    
    printStringIntDebugLog(DEBUGGING,"personaggioF ha scritto %d\n", &proprieta_personaggio->istanza);
    fflush(NULL);  

    while(proprieta_personaggio->flag!='l') {
        counter++;
        if(counter==100){
            counter=0;
            printStringCharDebugLog(DEBUGGING,"personaggio %c ",&proprieta_personaggio->segnaposto[0]);
            printStringIntDebugLog(DEBUGGING,"con istanza %d: e ancora vivo\n",&proprieta_personaggio->istanza);
        }        
        if(isAutonomus){
            napms(VELOCITA_PERSONAGGI);
            spostamento(proprieta_personaggio, false);                      
            if (bombDrop==0)
            {
                proprieta_personaggio->flag=BLANK_SPACE;
                bombDrop = customRandom(30,90);
            }
            bombDrop--;
        }
        else{
            numeroGiriCiclo++;
            //printStringIntDebugLog(true,"giro :%d\n",&numeroGiriCiclo);
            spostamento(proprieta_personaggio, false);
        }
        printStringCharDebugLog(DEBUGGING2, "personaggio flag:%c; \n", &(proprieta_personaggio->flag));
        printStringIntDebugLog(DEBUGGING2, "personaggio istanza:%d; \n", &(proprieta_personaggio->istanza));
        if(proprieta_personaggio->y>=getYfieldSize()-2||proprieta_personaggio->y<=0){
           printStringIntDebugLog(DEBUGGING," !!!!!!!!\nERRORE il personaggio con istanza  = %d  si e eliminato da solo\n!!!!!!!!!\n", &proprieta_personaggio->istanza);
           printPropietaOggetto(proprieta_personaggio);
           proprieta_personaggio->flag=LOST;
           scrivi(proprieta_personaggio);           
        }

        
        pthread_mutex_lock(&lifes);
        if (proprieta_personaggio->vite<=0){
            break;
        }
        pthread_mutex_unlock(&lifes);
        

        scrivi(proprieta_personaggio); 
    }
}

void waitTOJumpIn(struct proprietaOggetto *proprieta_personaggio){
    int whaitingCount =0;
    int i, j;
    if (strcmp(proprieta_personaggio->segnaposto,SEGNAPOSTO_ALIENO)==0){
        whaitingCount = (strlen(SEGNAPOSTO_ALIENO_CATTIVO)+1)*(NUMERO_ALIENI_CATTIVI+1);
        whaitingCount += strlen(SEGNAPOSTO_ALIENO)+3;
        
        for ( i = 0; i < proprieta_personaggio->istanza; i++)
        {
            for ( j = 0; j <whaitingCount+2; j++)
            {
                napms(VELOCITA_PERSONAGGI);
            }        
        }
    }
}

char spostamentoAPassi (struct proprietaOggetto *personaggio, bool isRandom){
    char c;
    if (isRandom){
        c = customRandom(MIN_RANDOM_STEP, MAX_RANDOM_STEP);
    }
    else{
        c=getch();
    }
    return passo(personaggio,c);
}

char spostamentoAPassiLaterali (struct proprietaOggetto *personaggio, bool isRandom){
    char c = 'n';
    if (isRandom){
        c = customRandom(DESTRA, SINISTRA);
        passo(personaggio,c);
    }
    else{
        c=getch();
        numeroClick++;
        //printStringIntDebugLog(true,"click :%d\n",&numeroClick);

        if(personaggio->istanza==0){
            if(c==DESTRA && personaggio->x < getXfieldSize()-personaggio->lunghezzaSegnaposto-1 ){
                passo(personaggio,c);
            }else if (c==SINISTRA && personaggio->x > 1)        {
                passo(personaggio,c);
            }else if(c == BLANK_SPACE || c==QUIT){
                passo(personaggio,c);
            }else{
                passo(personaggio, EMPTY);
            }
        }
        else{
            if(c=='d' && personaggio->x < getXfieldSize()-personaggio->lunghezzaSegnaposto-1 ){
                passo(personaggio,DESTRA);
            }else if (c=='a' && personaggio->x > 1)        {
                passo(personaggio,SINISTRA);
            }else if(c == 'w' ){
                passo(personaggio,BLANK_SPACE);
            }else if( c==QUIT){
                passo(personaggio,QUIT);
            }else{
                passo(personaggio, EMPTY);
            }

        }
        
    }
    return c;
}

char spostamentoLineare (struct proprietaOggetto *personaggio, bool isRandom){
    char c='x';
    int i=0;
    if (isRandom){
        c = customRandom(MIN_RANDOM_STEP, MAX_RANDOM_STEP);        
    }
    else{
        if (personaggio->y >= getYfieldSize()-3)
        {
            return passo(personaggio,LOST);
        }
        
        if(personaggio->oldX!=-1 && personaggio->oldY != -1 && personaggio->x != personaggio->oldX && (personaggio->x <=1 ||personaggio->x + strlen(SEGNAPOSTO_ALIENO) >= getXfieldSize()-1)){
            c=GIU;           
        }
        else{
            if(personaggio->x <=1 || personaggio->x > personaggio->oldX){
                c=DESTRA;                
            }
            else{
                c=SINISTRA;
            } 
        }
    }
    c=passo(personaggio,c);
    if(c==GIU&&PASSI_IN_VERTICALE>2){
        personaggio->y=personaggio->y+PASSI_IN_VERTICALE-1;        
    }
    return c;
    
}

char passo(struct proprietaOggetto *personaggio, char c){
    personaggio->flag = EMPTY;      
    switch(c) {
        case SU:
            if(personaggio->y>1)
            personaggio->oldY=personaggio->y;
            personaggio->oldX=personaggio->x;
            personaggio->y-=1;
            break;
        case GIU:
            if(personaggio->y<getYfieldSize()-3)
            personaggio->oldY=personaggio->y;
            personaggio->oldX=personaggio->x;
            personaggio->y+=1;
            break;
        case SINISTRA:
            if(personaggio->x>1)
            personaggio->oldY=personaggio->y;
            personaggio->oldX=personaggio->x;
            personaggio->x-=1;
            break;
        case DESTRA:
            if(personaggio->x<getXfieldSize()-personaggio->lunghezzaSegnaposto)
            personaggio->oldY=personaggio->y;
            personaggio->oldX=personaggio->x;
            personaggio->x+=1;
            break;
        case QUIT:
            personaggio->flag = QUIT;
            break;
        case BLANK_SPACE:
            personaggio->flag = BLANK_SPACE;
            break;
        case LOST:
            personaggio->flag = LOST;
            break;
    }
    return c;
}


void createRandomLocation(struct proprietaOggetto *el){
    el->x=customRandom(2,getXfieldSize()-2);
    el->y=customRandom(2,getYfieldSize()-2);
}

bool isSameLocation(struct proprietaOggetto *elA, struct proprietaOggetto *elB){
    if (elA->tid != 0 && elB->tid != 0 &&(( elA->x <= elB->x && elA->x+elA->lunghezzaSegnaposto-1 >= elB->x)||( elB->x <= elA->x && elB->x+elB->lunghezzaSegnaposto-1 >= elA->x)) && elA->y == elB->y)
    {
        return true;
    }    
    return false;
}

bool isSameLocationArray(struct proprietaOggetto *elA, struct proprietaOggetto elB[], int arrayLenght){
    int i;
    for ( i = 0; i < arrayLenght; i++)
    {
        if(isSameLocation(elA,&elB[i])){return true;}
    }
    
    return false;
}

int indexOfWhoIsSameLocationArray(struct proprietaOggetto *elA, struct proprietaOggetto elB[], int arrayLenght){
    int i;
    if (elA->x !=1 || elA->y!=1){
        for ( i = 0; i < arrayLenght; i++)
        {
            if (elB[i].x !=1 || elB[i].y!=1){
                if(isSameLocation(elA,&elB[i])){return i;}
            }
        }
    }    
    return -1;
}

bool isOutOfBound(struct proprietaOggetto *elA){
    if (elA->tid != 0 && elA->x >=2 && elA->y >=2 && elA->x < getXfieldSize()-2 && elA->y <=getYfieldSize()-4)
    {
        return false;
    }    
    return true;
}


void updateProprietaOggetto(struct proprietaOggetto *daSovrascrivere, struct proprietaOggetto *daCopiare ){
    daSovrascrivere->oldX = daCopiare->oldX;
    daSovrascrivere->oldY = daCopiare->oldY;

    daSovrascrivere->x = daCopiare->x;
    daSovrascrivere->y = daCopiare->y;
    
    if (daCopiare->tid != 0){daSovrascrivere->tid = daCopiare->tid;}
              
    daSovrascrivere->flag = EMPTY; 

    if (daSovrascrivere->vite < 0 ){daSovrascrivere->vite = daCopiare->vite;}
}


void proiettileF ( struct proprietaOggetto *proprieta_proiettile, char (*spostamento)(struct proprietaOggetto*)){    
    int counter =0;
    printStringIntDebugLog(DEBUGGING2,"-> proiettileF %d \n", &debugIndex);
    scrivi(proprieta_proiettile);
    while(proprieta_proiettile->flag!=LOST) {
        counter++;

        for (int i = 0; i < 10000; i++)
        {
            {int i = 5+5;}
        }
        
        if(counter==100){
            counter=0;
            printStringCharDebugLog(DEBUGGING,"personaggio %c ",&proprieta_proiettile->segnaposto[0]);
            printStringIntDebugLog(DEBUGGING,"con istanza %d: e ancora vivo\n",&proprieta_proiettile->istanza);
        }

        spostamento(proprieta_proiettile);
        napms(VELOCITA_PROIETTILI);
        
       printStringIntDebugLog(DEBUGGING2, "proiettile istanza:%d; \n", &(proprieta_proiettile->istanza));
       if(proprieta_proiettile->y>=getYfieldSize()||proprieta_proiettile->y<0){
            printStringCharDebugLog(DEBUGGING," !!!!!!!!\nERRORE il proiettile %c", &proprieta_proiettile->segnaposto[0]);
            printStringIntDebugLog(DEBUGGING," con istanza  = %d  si e eliminato da solo\n!!!!!!!!!\n", &proprieta_proiettile->istanza);
            proprieta_proiettile->flag=LOST;
            scrivi(proprieta_proiettile);
        }
        
        // pthread_mutex_lock(&lifes);
        // if (proprieta_proiettile->vite<=0){
        //     scrivi(proprieta_proiettile);
        //     break;
        // }
        // pthread_mutex_unlock(&lifes);
        
        scrivi(proprieta_proiettile);
    }
}

char spostamentoProiettileSX (struct proprietaOggetto *proiettile){
    char c = passo(proiettile,SINISTRA);
    proiettile->y=proiettile->y-1;
    return c;
}

char spostamentoProiettileDX (struct proprietaOggetto *proiettile){
    char c = passo(proiettile,DESTRA);
    proiettile->y=proiettile->y-1;
    return c;
}

char spostamentoDropBomb (struct proprietaOggetto *proiettile){
    char c = GIU;
    return passo(proiettile,c);
}


void spara(struct proprietaOggetto proiettile[], struct proprietaOggetto *valore_letto, int fileDescriptor[],int istanzaProiettile){
    aliveProcesses+=2;
    killIt(&proiettile[istanzaProiettile]);
    killIt(&proiettile[(istanzaProiettile+1)%NUMERO_MAX_PROIETTILI]);

    setPersonaggio(&proiettile[istanzaProiettile],SEGNAPOSTO_PROIETTILE,valore_letto->x+valore_letto->lunghezzaSegnaposto/2-1,valore_letto->y-1,0,proiettile[istanzaProiettile].vite,istanzaProiettile);                    
    setPersonaggio(&proiettile[(istanzaProiettile+1)%NUMERO_MAX_PROIETTILI],SEGNAPOSTO_PROIETTILE,valore_letto->x+valore_letto->lunghezzaSegnaposto/2-1,valore_letto->y-1,0,proiettile[(istanzaProiettile+1)%NUMERO_MAX_PROIETTILI].vite,(istanzaProiettile+1)%NUMERO_MAX_PROIETTILI);                    

    myThreadCreate(&(proiettile[istanzaProiettile]),proiettileSX);
    myThreadCreate(&(proiettile[(istanzaProiettile+1)%NUMERO_MAX_PROIETTILI]),proiettileDX);
}


void *proiettileSX(void* voidPersonaggio){
    struct proprietaOggetto* personaggio = (struct proprietaOggetto*) voidPersonaggio;
    printStringIntDebugLog(DEBUGGING,"-> proiettileSX %d; \n", &personaggio->istanza);

    proiettileF(personaggio,spostamentoProiettileSX);
}

void *proiettileDX(void* voidPersonaggio){
    struct proprietaOggetto* personaggio = (struct proprietaOggetto*) voidPersonaggio;
    printStringIntDebugLog(DEBUGGING,"-> proiettileDX %d; \n", &personaggio->istanza);

    proiettileF(personaggio,spostamentoProiettileDX);
}

void *dropBombF(void* voidPersonaggio){
    struct proprietaOggetto* personaggio = (struct proprietaOggetto*) voidPersonaggio;
    printStringIntDebugLog(DEBUGGING,"-> dropBombF %d; \n", &personaggio->istanza);

    proiettileF(personaggio,spostamentoDropBomb);
}



void *alienoF(void* voidPersonaggio){
    struct proprietaOggetto* personaggio = (struct proprietaOggetto*) voidPersonaggio;
    printStringIntDebugLog(true,"-> alienoF %d; \n", &personaggio->istanza);

    printStringIntDebugLog(DEBUGGING2,"creato singolo alieno() %d; \n", &debugIndex);
    printStringIntDebugLog(DEBUGGING2," alieno creato con istanza = %d \n", &personaggio->istanza);
    srand((int)(personaggio->istanza*(int)time(0)^(1/5)));
    personaggioF(personaggio,true,spostamentoLineare);
}


void *naveSpazialeF(void* voidPersonaggio){
    struct proprietaOggetto* personaggio = (struct proprietaOggetto*) voidPersonaggio;
    printStringIntDebugLog(true,"-> naveSpazialeF %d; \n", &personaggio->istanza);

    printStringIntDebugLog(DEBUGGING,"entrato dentro naveSpaziale() %d; \n", &debugIndex);
    printProprietaOggettoDebugLog(DEBUGGING, personaggio);
    printStringIntDebugLog(DEBUGGING2,"setPersonaggio() eseguito %d; \n", &debugIndex);
    personaggioF(personaggio,false,spostamentoAPassiLaterali);
}