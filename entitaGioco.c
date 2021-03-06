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
    proprieta_personaggio->tid = 0;
    proprieta_personaggio->isAlive = true;
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
    copiante->isAlive=true;
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
        napms(VELOCITA_PERSONAGGI);
        c = customRandom(DESTRA, SINISTRA); 
        if((c==DESTRA && personaggio->x < getXfieldSize()-personaggio->lunghezzaSegnaposto-1) || (c==SINISTRA && personaggio->x > 1) ){
            passo(personaggio,c);
        }       
        
        if (customRandom(0,2)==0){
            personaggio->flag=BLANK_SPACE;
        }
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


void spara(struct proprietaOggetto proiettile[], struct proprietaOggetto *valore_letto){
    
    killIt(&proiettile[istanzaProiettile]);
    killIt(&proiettile[(istanzaProiettile+1)%NUMERO_MAX_PROIETTILI]);

    setPersonaggio(&proiettile[istanzaProiettile],SEGNAPOSTO_PROIETTILE,valore_letto->x+valore_letto->lunghezzaSegnaposto/2-1,valore_letto->y-1,0,1,istanzaProiettile);                    
    setPersonaggio(&proiettile[(istanzaProiettile+1)%NUMERO_MAX_PROIETTILI],SEGNAPOSTO_PROIETTILE,valore_letto->x+valore_letto->lunghezzaSegnaposto/2-1,valore_letto->y-1,0,1,(istanzaProiettile+1)%NUMERO_MAX_PROIETTILI);                    

    myThreadCreate(&(proiettile[istanzaProiettile]),proiettileSX);
    myThreadCreate(&(proiettile[(istanzaProiettile+1)%NUMERO_MAX_PROIETTILI]),proiettileDX);
    
    istanzaProiettile = (istanzaProiettile+2)%NUMERO_MAX_PROIETTILI;
}


void *proiettileSX(void* voidPersonaggio){
    int index =0;
    struct proprietaOggetto* personaggio = (struct proprietaOggetto*) voidPersonaggio;
    printStringIntDebugLog(DEBUGGING,"-> proiettileSX %d; \n", &personaggio->istanza);

    //proiettileF(personaggio,spostamentoProiettileSX);
    int counter =0;
    printStringIntDebugLog(DEBUGGING2,"-> proiettileF %d \n", &debugIndex);
    //scrivi(personaggio);

    while(personaggio->isAlive) {
        counter++;

        for (int i = 0; i < 1000000; i++)
        {
            {int i = 5+5;}
        }
        
        if(counter==100){
            counter=0;
            printStringCharDebugLog(DEBUGGING_NEEDED,"personaggio %c ",&personaggio->segnaposto[0]);
            printStringIntDebugLog(DEBUGGING,"con istanza %d: e ancora vivo\n",&personaggio->istanza);
        }

        spostamentoProiettileSX(personaggio);
        napms(VELOCITA_PROIETTILI);
        
       printStringIntDebugLog(DEBUGGING2, "proiettile istanza:%d; \n", &(personaggio->istanza));
       if(personaggio->y>=getYfieldSize()||personaggio->y<0){
            printStringCharDebugLog(DEBUGGING," !!!!!!!!\nERRORE il proiettile %c", &personaggio->segnaposto[0]);
            printStringIntDebugLog(DEBUGGING," con istanza  = %d  si e eliminato da solo\n!!!!!!!!!\n", &personaggio->istanza);
            //personaggio->flag=LOST;
            killIt(personaggio);
            break;
        }

        if (personaggio->vite<=0){            
            killIt(personaggio);
            break;
        }

        //aggiorno
        if (isOutOfBound(personaggio)){
            killIt(personaggio);
            break;
        }else{

            //controllo contatti e agisco di conseguenza
            index = checkContacts(personaggio,alienoCattivo,NUMERO_ALIENI*NUMERO_ALIENI_CATTIVI);
            
            //controllo contatti e agisco di conseguenza
            index = checkContacts(personaggio,alieno,NUMERO_ALIENI);
            if(0<=index){
                if (alieno[index].vite<=0){
                    controlloAlieno(&alieno[index], alienoCattivo);
                }
            }

            printPropietaOggetto(personaggio,personaggio->vite,apparenzaProiettile);
        }
    }
}

void *proiettileDX(void* voidPersonaggio){
    int index =0;
    struct proprietaOggetto* personaggio = (struct proprietaOggetto*) voidPersonaggio;
    printStringIntDebugLog(DEBUGGING,"-> proiettileDX %d; \n", &personaggio->istanza);

    //proiettileF(personaggio,spostamentoProiettileDX);

    int counter =0;
    printStringIntDebugLog(DEBUGGING2,"-> proiettileF %d \n", &debugIndex);
    //scrivi(personaggio);

    while(personaggio->isAlive) {
        counter++;

        for (int i = 0; i < 1000000; i++)
        {
            {int i = 5+5;}
        }
        
        if(counter==100){
            counter=0;
            printStringCharDebugLog(DEBUGGING_NEEDED,"personaggio %c ",&personaggio->segnaposto[0]);
            printStringIntDebugLog(DEBUGGING,"con istanza %d: e ancora vivo\n",&personaggio->istanza);
        }

        spostamentoProiettileDX(personaggio);
        napms(VELOCITA_PROIETTILI);
        
       printStringIntDebugLog(DEBUGGING2, "proiettile istanza:%d; \n", &(personaggio->istanza));
       if(personaggio->y>=getYfieldSize()||personaggio->y<0){
            printStringCharDebugLog(DEBUGGING," !!!!!!!!\nERRORE il proiettile %c", &personaggio->segnaposto[0]);
            printStringIntDebugLog(DEBUGGING," con istanza  = %d  si e eliminato da solo\n!!!!!!!!!\n", &personaggio->istanza);
            //personaggio->flag=LOST;
            break;
        }
        
        if (personaggio->vite<=0){            
            killIt(personaggio);
            break;
        }

        //aggiorno
        if (isOutOfBound(personaggio)){
            killIt(personaggio);
            break;
        }else{

            //controllo contatti e agisco di conseguenza
            index = checkContacts(personaggio,alienoCattivo,NUMERO_ALIENI*NUMERO_ALIENI_CATTIVI);
            
            //controllo contatti e agisco di conseguenza
            index = checkContacts(personaggio,alieno,NUMERO_ALIENI);
            if(0<=index){
                if (alieno[index].vite<=0){
                    controlloAlieno(&alieno[index], alienoCattivo);
                }
            }
            
            printPropietaOggetto(personaggio,personaggio->vite,apparenzaProiettile);
        }
    }
}

void *dropBombF(void* voidPersonaggio){
    struct proprietaOggetto* personaggio = (struct proprietaOggetto*) voidPersonaggio;
    int index=0;
    printStringIntDebugLog(DEBUGGING,"-> dropBombF %d; \n", &personaggio->istanza);

    //proiettileF(personaggio,spostamentoDropBomb);

    int counter =0;
    printStringIntDebugLog(DEBUGGING2,"-> proiettileF %d \n", &debugIndex);
    scrivi(personaggio);
    while(personaggio->isAlive) {
        counter++;

        for (int i = 0; i < 1000000; i++)
        {
            {int i = 5+5;}
        }
        
        if(counter==100){
            counter=0;
            printStringCharDebugLog(DEBUGGING_NEEDED,"personaggio %c ",&personaggio->segnaposto[0]);
            printStringIntDebugLog(DEBUGGING,"con istanza %d: e ancora vivo\n",&personaggio->istanza);
        }
        
        spostamentoDropBomb(personaggio);
        napms(VELOCITA_PROIETTILI);
        
       printStringIntDebugLog(DEBUGGING2, "proiettile istanza:%d; \n", &(personaggio->istanza));
       if(personaggio->y>=getYfieldSize()||personaggio->y<0){
            printStringCharDebugLog(DEBUGGING," !!!!!!!!\nERRORE il proiettile %c", &personaggio->segnaposto[0]);
            printStringIntDebugLog(DEBUGGING," con istanza  = %d  si e eliminato da solo\n!!!!!!!!!\n", &personaggio->istanza);
            //personaggio->flag=LOST;
            break;
        }
        
        if (personaggio->vite<=0){
            killIt(personaggio);
            break;
        }
          
                //controllo contatti e agisco di conseguenza
        index = checkContacts(personaggio,naveSpaziale,NUMERO_GIOCATORI);
        if(0<=index){
            printLifesLeft(1,0,naveSpaziale->vite);
            printStringIntDebugLog(DEBUGGING,"nave controllata drop %d\n",&personaggio->istanza);
        }

        if (isOutOfBound(personaggio)){
                killIt(personaggio);
                break;
        }       

        printPropietaOggetto(personaggio,personaggio->vite,apparenzaDropBomb);
    }
}



void *alienoF(void* voidPersonaggio){
    int bombDrop = customRandom(10,30);
    int counter =0;
    struct proprietaOggetto* personaggio = (struct proprietaOggetto*) voidPersonaggio;
    printStringIntDebugLog(DEBUGGING_NEEDED,"-> alienoF %d; \n", &personaggio->istanza);

    printStringIntDebugLog(DEBUGGING2,"creato singolo alieno() %d; \n", &debugIndex);
    printStringIntDebugLog(DEBUGGING2," alieno creato con istanza = %d \n", &personaggio->istanza);
    //srand((int)(personaggio->istanza*(int)time(0)^(1/5)));
    //personaggioF(personaggio,true,spostamentoLineare);

   

    waitTOJumpIn(personaggio);
    
    printStringIntDebugLog(DEBUGGING,"personaggioF sta per scrivere %d\n", &personaggio->istanza);
    fflush(NULL);
    if(personaggio->segnaposto[0]==SEGNAPOSTO_ALIENO[0]){
        printPropietaOggetto(personaggio,personaggio->vite,apparenzaAlieno); 
    }else{
        printPropietaOggetto(personaggio,personaggio->vite,apparenzaAlienoCattivo); 
    }  
    
    printStringIntDebugLog(DEBUGGING,"personaggioF ha scritto %d\n", &personaggio->istanza);
    fflush(NULL);  

    while(personaggio->isAlive&&personaggio->flag!=LOST) {
        counter++;
        if(counter==100){
            counter=0;
            printStringCharDebugLog(DEBUGGING_NEEDED,"personaggio %c ",&personaggio->segnaposto[0]);
            printStringIntDebugLog(DEBUGGING,"con istanza %d: e ancora vivo\n",&personaggio->istanza);
        }        
        
        napms(VELOCITA_PERSONAGGI);
        //mutexLock(&proprieta_personaggio->mutex,proprieta_personaggio->segnaposto);
        spostamentoLineare(personaggio, false);                      
        if (bombDrop==0){
            personaggio->flag=BLANK_SPACE;
            bombDrop = customRandom(30,90);
        }
        bombDrop--;
        
        
        printStringCharDebugLog(DEBUGGING2, "personaggio flag:%c; \n", &(personaggio->flag));
        printStringIntDebugLog(DEBUGGING2, "personaggio istanza:%d; \n", &(personaggio->istanza));
        if(personaggio->y>=getYfieldSize()-2||personaggio->y<=0){
           printStringIntDebugLog(DEBUGGING," !!!!!!!!\nERRORE il personaggio con istanza  = %d  si e eliminato da solo\n!!!!!!!!!\n", &personaggio->istanza);          
            personaggio->flag=LOST;
            scrivi(personaggio);           
        }

            
        //controllo contatti e agisco di conseguenza
        //mutexLock(&lifes,"vite");
        if(0<=checkContacts(personaggio,proiettile,NUMERO_MAX_PROIETTILI)){
            if (personaggio->vite<=0)
            {
                if(personaggio->segnaposto[0]==SEGNAPOSTO_ALIENO[0]){
                    controlloAlieno(personaggio, alienoCattivo);
                }
                personaggio->flag=='k';
                //mutexUnlock(&lifes,"vite");
                break;
            }              
        }
        //mutexUnlock(&lifes,"vite");
           
        //controllo spari e agisco
        if (personaggio->flag==BLANK_SPACE)
        {                   
            killIt(&dropBomb[istanzaDropBomb]);
            setPersonaggio(&dropBomb[istanzaDropBomb],SEGNAPOSTO_DROPBOMB,personaggio->x,personaggio->y+1,0,1,istanzaDropBomb);                    
            myThreadCreate(&(dropBomb[istanzaDropBomb]),dropBombF);
            //aliveProcesses++;
            istanzaDropBomb = (istanzaDropBomb+1)%NUMERO_MAX_PROIETTILI;
        }
        //stampo
        if(personaggio->segnaposto[0]==SEGNAPOSTO_ALIENO[0]){
            printPropietaOggetto(personaggio,personaggio->vite,apparenzaAlieno); 
        }else{
            printPropietaOggetto(personaggio,personaggio->vite,apparenzaAlienoCattivo); 
        }
               
    }
    numeroNemici--;
    printEnemiesLeft(10, 0, numeroNemici);
}



void *naveSpazialeF(void* voidPersonaggio){
    int counter=0;
    struct proprietaOggetto* personaggio = (struct proprietaOggetto*) voidPersonaggio;
    //srand((int)(personaggio->istanza*(int)time(0)^(1/5)));
    
    printStringIntDebugLog(DEBUGGING_NEEDED,"-> naveSpazialeF %d; \n", &personaggio->istanza);

    printStringIntDebugLog(DEBUGGING,"entrato dentro naveSpaziale() %d; \n", &debugIndex);
    printProprietaOggettoDebugLog(DEBUGGING, personaggio);
    printStringIntDebugLog(DEBUGGING2,"setPersonaggio() eseguito %d; \n", &debugIndex);
    //personaggioF(personaggio,false,spostamentoAPassiLaterali);   
    
    //scrivi(personaggio);
    //printPropietaOggetto(personaggio,personaggio->vite,apparenzaNaveSpaziale); 
    printPropietaOggetto(personaggio,personaggio->vite,apparenzaNaveSpaziale);

    while(personaggio->isAlive) {               
        
        numeroGiriCiclo++;
        //printStringIntDebugLog(true,"giro :%d\n",&numeroGiriCiclo);
        napms(VELOCITA_PERSONAGGI/10);
        spostamentoAPassiLaterali(personaggio, IS_AUTONOMUS);
        counter++;
        if(counter==100){
            counter=0;
            printStringCharDebugLog(DEBUGGING_NEEDED,"personaggio %c ",&personaggio->segnaposto[0]);
            printStringIntDebugLog(DEBUGGING,"con istanza %d: e ancora vivo\n",&personaggio->istanza);
        } 
        
        printStringCharDebugLog(DEBUGGING2, "personaggio flag:%c; \n", &(personaggio->flag));
        printStringIntDebugLog(DEBUGGING2, "personaggio istanza:%d; \n", &(personaggio->istanza));
        if(personaggio->y>=getYfieldSize()-2||personaggio->y<=0){
           printStringIntDebugLog(DEBUGGING," !!!!!!!!\nERRORE il personaggio con istanza  = %d  si e eliminato da solo\n!!!!!!!!!\n", &personaggio->istanza);
            printPropietaOggetto(personaggio,personaggio->vite,apparenzaNaveSpaziale); 
           personaggio->flag=LOST;
           scrivi(personaggio); 
           break;          
        }

        //controllo spari e agisco
        if (personaggio->flag==BLANK_SPACE)
        {
            spara(proiettile, personaggio);                               
        } 

        if (personaggio->flag==QUIT){
            scrivi(personaggio);
            break;
        }
                       

        //controlllo contatti e agisco di conseguenza
        if(0<=checkContacts(personaggio,dropBomb,NUMERO_MAX_PROIETTILI)){
            printLifesLeft(1,0,personaggio->vite);
        }
        
        
        //stampo
        printPropietaOggetto(personaggio,personaggio->vite,apparenzaNaveSpaziale); 
        if (personaggio->vite<=0){
            personaggio->flag=LOST;
            scrivi(personaggio);
            break;
        }
    }
}