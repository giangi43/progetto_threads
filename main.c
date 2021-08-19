#include "header.h"

int main(){
    numeroGiriCiclo=0;
    numeroClick=0;
    

    deltatime.tv_sec=0;
    deltatime.tv_nsec=100000;


    //pid_t pid_alieni, pid_naveSpaziale;
    int comand;
    NUMERO_ALIENI = D_NUMERO_ALIENI;
    NUMERO_ALIENI_CATTIVI = D_NUMERO_ALIENI_CATTIVI;
    NUMERO_GIOCATORI = D_NUMERO_GIOCATORI;
    NUMERO_MAX_PROIETTILI = D_NUMERO_MAX_PROIETTILI;
    VITE_NAVE = D_VITE_NAVE;
    VITE_ALIENI = D_VITE_ALIENI;
    VITE_ALIENI_CATTIVI = D_VITE_ALIENI_CATTIVI;
    PASSI_IN_VERTICALE = D_PASSI_IN_VERTICALE;
    VELOCITA_PROIETTILI = D_VELOCITA_PROIETTILI;
    VELOCITA_PERSONAGGI = D_VELOCITA_PERSONAGGI;
    IS_WITH_THREAD = THREAD_ON;

    struct proprietaOggetto talieno[NUMERO_ALIENI];
    struct proprietaOggetto talienoCattivo[NUMERO_ALIENI*NUMERO_ALIENI_CATTIVI];
    struct proprietaOggetto tnaveSpaziale[NUMERO_GIOCATORI];
    //struct proprietaOggetto valore_letto;
    struct proprietaOggetto tdropBomb[NUMERO_MAX_PROIETTILI];
    struct proprietaOggetto tproiettile[NUMERO_MAX_PROIETTILI];
    alieno = talieno;
    alienoCattivo = talienoCattivo;
    naveSpaziale = tnaveSpaziale;
    //valore_letto = &tvalore_letto;
    dropBomb= tdropBomb;
    proiettile=tproiettile;

    char mainMenu[][25]={"PLAY","OPTION","QUIT"};
    char optionMenu[][25]={ "numero alieni",
                            "numero alieni cattivi",                        
                            "numero max proiettili",
                            "vite nave",
                            "vite alieni",
                            "vite alieni cattivi",
                            "passi in verticale",
                            "velocita proiettili",
                            "velocita personaggi",
                            "usa thread"};
    int* interazioni[] ={   &NUMERO_ALIENI,
                            &NUMERO_ALIENI_CATTIVI,
                            &NUMERO_MAX_PROIETTILI,
                            &VITE_NAVE,
                            &VITE_ALIENI,
                            &VITE_ALIENI_CATTIVI,
                            &PASSI_IN_VERTICALE,
                            &VELOCITA_PROIETTILI,
                            &VELOCITA_PERSONAGGI,
                            &IS_WITH_THREAD};

    //printf("%d",*interazioni[0]);
    srand((int)((int)time(0)^(1/5)));
    
    
    createDebugLog(true);
    printDebugLog(true);
    printDebugLog(DEBUGGING);
    
    initscr();
    
    noecho();

    start_color();
      init_pair(1, COLOR_RED, COLOR_BLACK);
      init_pair(2, COLOR_YELLOW, COLOR_BLACK);
      init_pair(3, COLOR_GREEN, COLOR_BLACK);
      init_pair(4, COLOR_BLUE, COLOR_BLACK);
      init_pair(5, COLOR_WHITE, COLOR_BLACK);
      init_pair(6, COLOR_BLACK, COLOR_WHITE);
      init_pair(7, COLOR_RED, COLOR_WHITE);
      init_pair(8, COLOR_BLUE, COLOR_WHITE);

    curs_set(0);
    setActualFieldSize();

    //pipeCeck(fileDescriptor);    

    do{
        comand = customMenu("SPACE INVADER",mainMenu,NULL,3);
        if (comand == 1){
            customMenu("Option Menu",optionMenu,interazioni,10);
        }            
    }while (comand ==1);
    
    if (comand == 0){

            posizioneCoda=0;
            numeroElementiAttualiCoda=0;
            
            attrset(A_NORMAL);
            printStringIntDebugLog(DEBUGGING_NEEDED,"-> controllo %d; \n", &debugIndex);
            controllo (NULL);
            printStringIntDebugLog(DEBUGGING_NEEDED,"controllo ->%d; \n", &debugIndex);

        }

    
    endwin();
    /**/
    //getchar();
    return 0;
}


void controllo (){
printStringIntDebugLog(DEBUGGING,"entrato dentro controllo() %d; ", &debugIndex);
    struct proprietaOggetto valore_letto;
    istanzaProiettile=0;
    istanzaDropBomb=0;
    int viteTotali=NUMERO_GIOCATORI*VITE_NAVE;
    numeroNemici=NUMERO_ALIENI;
    clockStart = clock();
    int FPScounter = 0;
    aliveProcesses = 0;
    int i =0, j=0, index =0;
    //printStringIntDebugLog(DEBUGGING," pipeout = %d \n",&fileDescriptor[1]);

    resetField(0, 0, getXfieldSize(), getYfieldSize());
    
    buildFieldBorders(getXfieldSize(), getYfieldSize()-1);

    printLifesLeft(1,0,VITE_NAVE);
    //printFPS(1, getXfieldSize()-7, &FPScounter);
    printEnemiesLeft(10, 0, numeroNemici);
    curs_set(0);

    printStringIntDebugLog(DEBUGGING_NEEDED,"-> settaggio personaggi (controllo)  %d; \n", &debugIndex);

    setPersonaggio(&valore_letto,SEGNAPOSTO_NAVE,getXfieldSize()/2,getYfieldSize()-3,0,VITE_NAVE,0);
    inizializzaPersonaggi(&valore_letto,naveSpaziale,NUMERO_GIOCATORI);
    creaGruppoPersonaggi(naveSpaziale, naveSpazialeF, NUMERO_GIOCATORI);
    //aliveProcesses+= NUMERO_GIOCATORI ;

    printStringIntDebugLog(DEBUGGING," creata nave %d \n", &debugIndex);

    setPersonaggio(&valore_letto,SEGNAPOSTO_ALIENO,1,1,0,VITE_ALIENI,0);
    inizializzaPersonaggi(&valore_letto,alieno,NUMERO_ALIENI);
    creaGruppoPersonaggi(alieno, alienoF, NUMERO_ALIENI);
    //aliveProcesses+= NUMERO_ALIENI ;

    printStringIntDebugLog(DEBUGGING," creati alieni = %d \n",&debugIndex);
    fflush(NULL);

    setPersonaggio(&valore_letto,SEGNAPOSTO_ALIENO_CATTIVO,1,1,0,VITE_ALIENI_CATTIVI,0);
    inizializzaPersonaggi(&valore_letto,alienoCattivo,NUMERO_ALIENI*NUMERO_ALIENI_CATTIVI);

    printStringIntDebugLog(DEBUGGING," inizializzati alieni cattivi = %d \n", &debugIndex);
    fflush(NULL);


    setPersonaggio(&valore_letto,SEGNAPOSTO_PROIETTILE,1,1,0,1,0);
    inizializzaPersonaggi(&valore_letto,proiettile,NUMERO_MAX_PROIETTILI);

    setPersonaggio(&valore_letto,SEGNAPOSTO_DROPBOMB,1,1,0,1,0);
    inizializzaPersonaggi(&valore_letto,dropBomb,NUMERO_MAX_PROIETTILI);
    printStringIntDebugLog(DEBUGGING_NEEDED,"settaggio personaggi (controllo) -> %d; \n", &debugIndex);
    
        
    refresh();

    printStringIntDebugLog(DEBUGGING_NEEDED,"-> ciclo (controllo) %d; \n", &debugIndex);

    do { 
        //printStringIntDebugLog(DEBUGGING,"entrato %d\n", &debugIndex);
        //fflush(NULL);

        //read(pipein,&valore_letto,sizeof(valore_letto)); // leggo dalla pipe //
        leggi (&valore_letto);
        if(valore_letto.flag!='v'){
            //mutexLock(&valore_letto.mutex, valore_letto.segnaposto);
            printStringIntDebugLog(DEBUGGING,"inizio %d\n", &debugIndex);
            printProprietaOggettoDebugLog(DEBUGGING, &valore_letto);
            //fflush(NULL);
        }
        if(valore_letto.flag==LOST){
            break;
        }
        
        
        /*
            gestisco il caso di un errore
        */ 
        else if( valore_letto.flag!='v'){
            printStringIntDebugLog(DEBUGGING,"qualcosa è andato storto dentro a controllo %d\n",&debugIndex);
            printProprietaOggettoDebugLog(DEBUGGING,&valore_letto);
        }
        
        
    } while (viteTotali >0 && valore_letto.flag!=QUIT && numeroNemici>0 && valore_letto.flag!=LOST); // ciclo fino al verificarsi di una collisione alieni/naveSpaziale //

    killThemAll(alieno, NUMERO_ALIENI);
    killThemAll(naveSpaziale, NUMERO_GIOCATORI);
    killThemAll(dropBomb, NUMERO_MAX_PROIETTILI);
    killThemAll(proiettile, NUMERO_MAX_PROIETTILI);
    killThemAll(alienoCattivo, NUMERO_ALIENI*NUMERO_ALIENI_CATTIVI);


    printStringIntDebugLog(DEBUGGING2," uscente da killThemAll %d\n", &debugIndex);
    /**/
    
    if (naveSpaziale[0].vite<1 || valore_letto.flag==LOST){
        gameOver(getXfieldSize(), getYfieldSize());
    }
    else if (numeroNemici==0 ){
        youWon(getXfieldSize(),getYfieldSize());
    }

    //freeTheBuffer(NULL,valore_letto);
    printStringIntDebugLog(DEBUGGING2," uscente da freeTheBuffer %d\n", &debugIndex);

    printStringIntDebugLog(DEBUGGING2," uscente da controllo %d\n", &debugIndex);
}





/*
    gestisce la parte relativa a come appare l'alieno sullo schermo 
    e come si deve comportare lo stesso quando viene ucciso
*/
void controlloAlieno( struct proprietaOggetto *alieno, struct proprietaOggetto alienoCattivo[]){
    int i,j;
    //if(alieno->tid==0){
        //numeroNemici = numeroNemici-1;
        inizializzaPersonaggi(alieno,&alienoCattivo[alieno->istanza*NUMERO_ALIENI_CATTIVI],NUMERO_ALIENI_CATTIVI);
        //aliveProcesses+= NUMERO_ALIENI_CATTIVI ;
        for ( i = 0; i < NUMERO_ALIENI_CATTIVI; i++)
        {   
            alienoCattivo[alieno->istanza*NUMERO_ALIENI_CATTIVI+i].istanza=alieno->istanza*NUMERO_ALIENI_CATTIVI+i;
            alienoCattivo[alieno->istanza*NUMERO_ALIENI_CATTIVI+i].vite=VITE_ALIENI_CATTIVI;
            alienoCattivo[alieno->istanza*NUMERO_ALIENI_CATTIVI+i].lunghezzaSegnaposto = strlen(SEGNAPOSTO_ALIENO_CATTIVO);
            strncpy(alienoCattivo[alieno->istanza*NUMERO_ALIENI_CATTIVI+i].segnaposto,SEGNAPOSTO_ALIENO_CATTIVO,MAX_STRING_SIZE);
            for (j = 0; j < (NUMERO_ALIENI_CATTIVI-i-1)*(strlen(SEGNAPOSTO_ALIENO_CATTIVO)+2); j++)
            {
                spostamentoLineare(&alienoCattivo[alieno->istanza*NUMERO_ALIENI_CATTIVI+i],false);
            }     
            myThreadCreate(&alienoCattivo[alieno->istanza*NUMERO_ALIENI_CATTIVI+i],alienoF);
            //printProprietaOggettoDebugLog(DEBUGGING,&alienoCattivo[alieno->istanza*NUMERO_ALIENI_CATTIVI+i]);

            numeroNemici++;
        }  
        //printEnemiesLeft(10, 0, numeroNemici);
    //}
}


int push(struct proprietaOggetto coda[],struct proprietaOggetto *oggetto){
    char str[20];
    strcpy(str,"scrittura ");
    strcat(str, oggetto->segnaposto);
    mutexLock(&lock, str);
    if(numeroElementiAttualiCoda>=LUNGHEZZA_CODA){
        return 0;
    }else{
        coda[posizioneCoda]=*oggetto;
        posizioneCoda=(posizioneCoda+1)%LUNGHEZZA_CODA;
        numeroElementiAttualiCoda++;
    }
    mutexUnlock(&lock,str);
    return 1;
}

struct proprietaOggetto pop(struct proprietaOggetto coda[]){
    if(numeroElementiAttualiCoda>0){
        char str[20];
        mutexLock(&lock,"lettura");
        numeroElementiAttualiCoda--;
        struct proprietaOggetto tmp = coda[(posizioneCoda+(LUNGHEZZA_CODA-numeroElementiAttualiCoda-1))%LUNGHEZZA_CODA];
        strcpy(str,"lettura ");
        strcat(str, tmp.segnaposto);
        mutexUnlock(&lock,str);
        return tmp;
    }
    struct proprietaOggetto vuoto ;
    vuoto.flag='v';
    return vuoto;
}
