#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <conio.h>
#include "Map.h"
typedef struct Operatiune{
    char codBanca[6];
    char codClient[11];
    char dataOperatiune[11];
    double sumaOperatiune;
    struct Operatiune *next;
}Operatiune;

void copyUntilSeparator(const char *source, char *destination, int destinationSize, char separator){
    char *pozitieSeparator = strchr(source, separator);

    if (pozitieSeparator != NULL) {
        // Găsim separatorul în șir
        int indexSeparator = pozitieSeparator - source;
        if(indexSeparator<destinationSize){
            memmove(destination, source,  indexSeparator);
            destination[indexSeparator]='\0';
        }
        else{
            /*Dacă dimensiunea destinației e mai mica decât șirul până la separator
             * nu copiem nimic pentru a nu cauza deformarea în memorie a șirului inițial
             */
        }
    }
    else {
        // Dacă separatorul nu este găsit, șirul rămâne neschimbat
    }
}
void removeUntilSeparator(char *sir, char separator) {
    char *pozitieSeparator = strchr(sir, separator);
    if (pozitieSeparator != NULL) {
        // Găsim separatorul în șir
        int indexSeparator = pozitieSeparator - sir;
        memmove(sir, sir + indexSeparator + 1, strlen(sir) - indexSeparator);
    }
    else {
        // Dacă separatorul nu este găsit, șirul rămâne neschimbat
    }
}
void createOperation(const char codBanca[6], const char codClient[11], const  char dataOperatiune[11], const double sumaOperatiune, Operatiune **head) {
    Operatiune *node = malloc(sizeof(Operatiune));
    if (node == NULL) {
        fputs("Error: Out of memory\n", stderr);
        exit(1);
    } else {
        strcpy(node->codBanca, codBanca);
        strcpy(node->codClient, codClient);
        strcpy(node->dataOperatiune, dataOperatiune);
        node->sumaOperatiune = sumaOperatiune;
        if (*head == NULL) {
            node->next = *head;
            *head = node;
        } else {
            Operatiune *iterr = *(head);
            while (iterr->next != NULL)
                iterr = iterr->next;
            iterr->next = node;
            node->next = NULL;


        }
    }
}
int parseFile(Operatiune **head){
    FILE *fptr;
    fptr = fopen("operations.txt","r");
    char operation[46];
    if(fptr == NULL)
    {
        printf("Error!");
        getch();
        exit(1);
    }
    char codBanca[6];
    char codClient[11];
    char dataOperatiune[11];
    double sumaOperatiune;
    //Citim toate operațiunile din fișier
    while (fgets(operation, sizeof(operation), fptr)){
        if (operation[0] == '\r' || operation[0]=='\n') {
            continue; // Ignoră caracterul '\r' si '\n'
        }
        //Citim fiecare operațiune rând cu rând  în variabila operation apoi extragem din ea datele
        copyUntilSeparator(operation,codBanca,6,';');
        removeUntilSeparator(operation,';');
        copyUntilSeparator(operation,codClient,11,';');
        removeUntilSeparator(operation,';');
        copyUntilSeparator(operation,dataOperatiune,11,';');
        removeUntilSeparator(operation,';');
        //Facem replace la ',' în '.' pentru a putea parsa suma operațiunii
        char *pozitieSeparator = strchr(operation, ',');
        if (pozitieSeparator != NULL) {
            *pozitieSeparator='.';
        }
        sumaOperatiune = strtod(operation, '\0');
        createOperation(codBanca,codClient,dataOperatiune,sumaOperatiune,head);

    }
    fclose(fptr);
    return 0;

}
void print(Operatiune * head){
    int i=0;
    while(head!=NULL){
        printf("\n Opratiune %i ",++i);
        printf("\n Cod banca %s ",head->codBanca);
        printf("\n Cod client %s ",head->codClient);
        printf("\n Data operatiune %s ",head->dataOperatiune);
        printf("\n Suma operatiune %.3f ",head->sumaOperatiune);
        printf("\n-----------------------------------------------------");
        head=head->next;
    }
}

void Swap(Operatiune * operatiune1, Operatiune *operatiune2)
{
    Operatiune aux;
    Operatiune *next1, *next2;
    next1=operatiune1->next;
    next2=operatiune2->next;
    aux=*operatiune1;
    *operatiune1=*operatiune2;
    *operatiune2=aux;
    operatiune1->next=next1;
    operatiune2->next=next2;
    return;
}
void sortByClientAndBankAndDate(Operatiune * head){
    Operatiune *current = head, *index = NULL;
    if(head == NULL) {
        return ;
    }
    else {
        while(current != NULL) {
            index = current->next;
            while(index != NULL) {
                char lunaOperatiune[8];
                char lunaOperatiune1[8];
                /*Copiem în vriabile luna operațiunii până întâlnim separatorul '-' și transmitem în funcție
                 sursa +3 pentru a omite primele 3 caractere care indică ziua operațiunii
                 */
                copyUntilSeparator(current->dataOperatiune+3, lunaOperatiune, 8, '-');
                copyUntilSeparator(index->dataOperatiune+3, lunaOperatiune1, 8, '-');
                if(strcmp(current->codClient ,index->codClient)>0 || strcmp(current->codBanca,index->codBanca)>0 ||
                   strcmp(lunaOperatiune, lunaOperatiune1) > 0) {
                    Swap(current,index);
                }
                index = index->next;
            }
            current = current->next;
        }
    }
}
double calculateComision(const double suma){
    return suma*0.5/100.0;
}
void writeToFileAllComisions(Operatiune *head){
    FILE *fptr;
    fptr = fopen("output.txt","w");
    double sumComision=0;
    Map *venitTotal=new();
    if(fptr == NULL)
    {
        printf("Error!");
        getch();
        exit(1);
    }
    while(head!=NULL){
        if(head->sumaOperatiune>5000) {
            sumComision+= calculateComision(head->sumaOperatiune);
        }

        char lunaOperatiune1[3];
        char lunaOperatiune[3];

        copyUntilSeparator(head->dataOperatiune+3,lunaOperatiune,3,'-');
        if(head->next!=NULL){
        copyUntilSeparator(head->next->dataOperatiune+3,lunaOperatiune1,3,'-');
        }


        if(head->next!=NULL && strcmp(lunaOperatiune,lunaOperatiune1)!=0  && sumComision>0){
            venitTotal->insert(head->codBanca, sumComision, venitTotal);
             fprintf(fptr,"%s;%s;%s;%012.3f\n",head->codBanca,head->codClient,lunaOperatiune,sumComision);
            sumComision=0;
        }

        else if(head->next!=NULL && strcmp(head->codBanca,head->next->codBanca)!=0 && sumComision>0 ){
            venitTotal->insert(head->codBanca, sumComision, venitTotal);
            fprintf(fptr,"%s;%s;%s;%012.3f\n",head->codBanca,head->codClient,lunaOperatiune,sumComision);
            sumComision=0;
        }
        else if(head->next!=NULL && strcmp(head->codClient,head->next->codClient)!=0 && sumComision>0){
            venitTotal->insert(head->codBanca, sumComision, venitTotal);
            fprintf(fptr,"%s;%s;%s;%012.3f\n",head->codBanca,head->codClient,lunaOperatiune,sumComision);
            sumComision=0;
        }

        else if(head->next==NULL && sumComision>0){
            venitTotal->insert(head->codBanca, sumComision, venitTotal);
            fprintf(fptr,"%s;%s;%s;%012.3f\n",head->codBanca,head->codClient,lunaOperatiune,sumComision);

        }


        head=head->next;
    }
    venitTotal->print(venitTotal);
    fclose(fptr);
}

int main() {
    Operatiune *head=NULL;
    parseFile(&head);
    sortByClientAndBankAndDate(head);
    writeToFileAllComisions(head);
    getch();
    return 0;
}
