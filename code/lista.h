#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <signal.h>

typedef struct nodo{
int soldi;
int numero_blocco;
char ip_mittente[15];
char ip_destinatario[15];
struct nodo *next;
}NodoN;  


int genera_soldi()
{
int n=10+rand()%100;
return n;
}



char *genera_ip() {

int i;
char *s=(char*)malloc(6*sizeof(char));
char *a=(char*)malloc(6*sizeof(char));
for(i = 0; i < 3; i++) {
snprintf(a,10, "%d.", rand()%255);
strcat(s,a);}
snprintf(a,10,"%d",rand()%255);
strcat(s,a);

return s;
}


void inserimento(NodoN **head,int conteggio)
{
NodoN *nuovo_nodo;
nuovo_nodo=malloc(sizeof(NodoN));
nuovo_nodo->numero_blocco=conteggio;
nuovo_nodo->soldi=genera_soldi();
strcpy(nuovo_nodo->ip_mittente,genera_ip());
strcpy(nuovo_nodo->ip_destinatario,genera_ip());
nuovo_nodo->next=*head;
*head=nuovo_nodo;
}

void stampa(NodoN *head)
{
printf("il numero del blocco è:%d\n",head->numero_blocco);
printf("i soldi sono:%d\n",head->soldi);
printf("ip mittente è:%s\n",head->ip_mittente);
printf("ip destinatario è:%s\n",head->ip_destinatario);

}





