#include "wrapper.h"

#include "lista.h"

int main() {
    int listenfd; // descrittore client

    struct sockaddr_in addr;
    //CLIENT
    NodoN * nuovo_nodo;
    nuovo_nodo = malloc(sizeof(NodoN));
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    int totale, numero_transazione, scelta;
    char ip[15];
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2123);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Connect(listenfd, addr, sizeof(addr));
    for (;;) {
        printf("Scegli un operazione\n");
        printf("1. Somma totale\n");
        printf("2. Transazione specifica da visualizzare\n");
        printf("3. Visualizzare gli ultimi N nodi\n");
        printf("4. Ip mittente o destinatario da trovare\n");

        scanf("%d", & scelta);
        if (scelta > 4) {
            printf("scelta sbagliata\n");
            exit(1);
        }
        FullWrite(listenfd, & scelta, sizeof(int));

        switch (scelta) {
        case 1:
            printf("\nQuesta è la somma totale \n");
            FullRead(listenfd, & totale, sizeof(int));
            printf("totale:%d\n", totale);

            break;

        case 2:
            printf("inserisci il numero della transazione che vuoi vedere\n");
            scanf("%d", & numero_transazione);
            FullWrite(listenfd, & numero_transazione, sizeof(int));
            FullRead(listenfd, nuovo_nodo, sizeof(NodoN));
            stampa(nuovo_nodo);

            break;

        case 3:
            printf("inserisci la transazione dalla quale vuoi vedere\n");
            scanf("%d", & numero_transazione);
            FullWrite(listenfd, & numero_transazione, sizeof(int));
            while (numero_transazione > 0) {
                nuovo_nodo = malloc(sizeof(NodoN));
                FullRead(listenfd, nuovo_nodo, sizeof(NodoN));

                stampa(nuovo_nodo);
                numero_transazione--;

            }
            break;

        case 4:
            printf("inserisci l'indirizzo ip mittente o destinatario che si vuole trovare\n");
            scanf("%s", ip);
            FullWrite(listenfd, & ip, sizeof(ip));
            FullRead(listenfd, nuovo_nodo, sizeof(NodoN));
            stampa(nuovo_nodo);
            break;

        }
        exit(0);
    }

}
