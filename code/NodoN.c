#include "wrapper.h"

#include "lista.h"


int main(int argc, char ** argv) {
    int blocco = 0;
    NodoN * head = NULL;

    int connfd, listenfd, n;
    fd_set set;
    int tr = 1;
    struct sockaddr_in addr;
    //Creazione socket
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);

    // kill "Address already in use" error message
    if (setsockopt(listenfd, SOL_SOCKET, SO_REUSEADDR, & tr, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }
    //Dichiarazione Struttura Server
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(2059);
    //Assegnazione indirizzo,porta e famiglia di ip in questo caso ipv4
    Bind(listenfd, addr, sizeof(addr));
    //Coda max 1024 connessioni
    Listen(listenfd);
    

   /*in questa parte di codice usiamo la select per verificare quando un descrittore
   è pronto per la connessione,quando è pronto quindi significa che qualcuno si sta connettendo
  al nostro server*/
    FD_ZERO( & set);

    for (;;) {
        printf("Attendo connessioni\n");
        FD_SET(listenfd, & set);
        if (select(listenfd + 1, & set, NULL, NULL, NULL) < 0)
            exit(1);
        if (FD_ISSET(listenfd, & set)) {
            connfd = Accept(listenfd);
        }
close(listenfd);
    /*arrivata al connessione andremo ad inviare sempre blocchi fin quando c'è una connessione*/
        for (;;) {
            //quando viene effettuata la connessione genera e invia blocchi in tempo reale
            inserimento( & head, blocco++); //crea i blocchi 
            stampa(head); //stampa i blocchi

            FullWrite(connfd, head, sizeof(NodoN));

            sleep(5+rand()%11);
        }
    }

    close(connfd);
    
    exit(0);
}



