#include "header.h"


int customRandom(int min, int max){
    return min + (rand() % (1+max-min));
}




//debug /////////////////////////////////////////////////////////////



void createDebugLog(bool isDebugging){
    if(isDebugging){
        fptr = fopen("debugLog.txt","w");
        debugIndex = 0;
        if(fptr == NULL){
            //printf("Error loading debug");   
            exit(1);             
        }
        fclose(fptr);
    }
}

void printDebugLog(bool isDebugging){
    printStringIntDebugLog(isDebugging,"%d; \n", &debugIndex);
}

void printStringIntDebugLog(bool isDebugging, char* string, int *something){
    if (isDebugging){
        mutexLock(&debug,"debug");
        fptr = fopen("debugLog.txt","a");
        if(fptr == NULL){
            //printf("Error in printStringDebugLog");   
            exit(1);             
        }
        fprintf(fptr,string, *something);
        fclose(fptr);
        debugIndex++;
        mutexUnlock(&debug,"debug");
    }    
}
void printStringCharDebugLog(bool isDebugging, char* string,char *something){
    if (isDebugging){
        mutexLock(&debug,"debug");
        fptr = fopen("debugLog.txt","a");
        if(fptr == NULL){
            //printf("Error in printStringDebugLog");   
            exit(1);             
        }
        fprintf(fptr,string, *something);
        fclose(fptr);
        debugIndex++;
        mutexUnlock(&debug,"debug");
    }
}

void printStringStringDebugLog(bool isDebugging, char* string,char something[]){
    if (isDebugging){
        mutexLock(&debug,"debug");
        fptr = fopen("debugLog.txt","a");
        if(fptr == NULL){
            //printf("Error in printStringDebugLog");   
            exit(1);             
        }
        fprintf(fptr,string, something);
        fclose(fptr);
        debugIndex++;
        mutexUnlock(&debug,"debug");
    }
}

void printProprietaOggettoDebugLog(bool isDebugging,struct proprietaOggetto *personaggio){
    if (isDebugging){
        mutexLock(&debug,"debug");
        fptr = fopen("debugLog.txt","a");
        if(fptr == NULL){
            //printf("Error in printStringDebugLog");   
            exit(1);             
        }
        fprintf(fptr,"{\n\t");
        fprintf(fptr,"%s",personaggio->segnaposto);
        fprintf(fptr,"\n\tlunghezza segnaposto: %d\n",personaggio->lunghezzaSegnaposto);
        fprintf(fptr,"\tistanza: %d\n",personaggio->istanza);
        //fprintf(fptr,"\tpid: %d\n",personaggio->pid);
        fprintf(fptr,"\ttid: %d\n",(int) personaggio->tid);
        fprintf(fptr,"\tflag: %c\n",personaggio->flag);
        fprintf(fptr,"\tvite: %d\n",personaggio->vite);
        fprintf(fptr,"\tX: %d\n",personaggio->x);
        fprintf(fptr,"\tY: %d\n",personaggio->y);
        fprintf(fptr,"\toldX: %d\n",personaggio->oldX);
        fprintf(fptr,"\toldY: %d\n",personaggio->oldY);
        fprintf(fptr,"}\n");
        fclose(fptr);
        fflush(NULL);
        debugIndex++;
        mutexUnlock(&debug,"debug");
    }   
}

void printMutexDebugLog(bool isDebugging, char nomeMutex[], char statoMutex[]){
    if (isDebugging){
        fptr = fopen("debugLog.txt","a");
        if(fptr == NULL){
            //printf("Error in printStringDebugLog");   
            exit(1);             
        }
        fprintf(fptr,"mutex:%s;  stato:%s;\n", nomeMutex,statoMutex);
        fclose(fptr);
        debugIndex++;
    }    
}
