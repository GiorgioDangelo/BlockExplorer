#include "wrapper.h"

#include "lista.h"


int main(int argc, char ** argv) {

    int listenfd; // descrittore client
    int socket, nuovo_socket; //descrittore server
    int ready, figlio, scelta, nipote;
    int fd[2];
    pipe(fd);
    NodoN * head;
    head = NULL;
    NodoN * new_nodo;
    int numero_transazione, i;
    char ip[15];
    struct sockaddr_in addr;
    //CLIENT
    listenfd = Socket(AF_INET, SOCK_STREAM, 0);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(2059);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    Connect(listenfd, addr, sizeof(addr));

    //Server
    socket = Socket(AF_INET, SOCK_STREAM, 0);
    int tr = 1; //flag address already in use
    // kill "Address already in use" error message

    if (setsockopt(socket, SOL_SOCKET, SO_REUSEADDR, & tr, sizeof(int)) == -1) {
        perror("setsockopt");
        exit(1);
    }

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(2123);
    Bind(socket, addr, sizeof(addr));
    int flag = 0;

    //socket non bloccante
    int flags = fcntl(socket, F_GETFL);
    fcntl(socket, F_SETFL, flags | O_NONBLOCK);
    //Coda max 1024 connessioni
    Listen(socket);
    figlio = fork();

    if (figlio == 0) //sono il figlio
    {
        for (;;) {
            close(socket);
            close(fd[0]);
            new_nodo = malloc(sizeof(NodoN));
            FullRead(listenfd, new_nodo, sizeof(NodoN));

            if (new_nodo -> soldi == 0) raise(SIGUSR1);

            FullWrite(fd[1], new_nodo, sizeof(NodoN));
            new_nodo -> next = head;
            head = new_nodo;

            printf("ricevuto\n");

        }
    }
    if (figlio != 0) { //sono il padre
     
        NodoN * testa = NULL;
        NodoN * nuovo;
        for (;;) {
            nuovo_socket = accept(socket, NULL, NULL);
            if (nuovo_socket == -1) {
                if (errno == EWOULDBLOCK) //Praticamente sarà in uno stato di polling fin quando non ci sarà qualcuno a connettersi.
                {
                    nuovo = malloc(sizeof(NodoN));
                    FullRead(fd[0], nuovo, sizeof(NodoN));
                    nuovo -> next = testa;
                    testa = nuovo;

                }
            } else {     //in questa parte il nipote gestirà il client connesso.
                nipote = fork();
                if (nipote == 0) {
                    close(socket);
                    close(listenfd);

                    printf("connesso\n");
                    FullRead(nuovo_socket, & scelta, sizeof(scelta));

                    switch (scelta) {
                    case 1:
                        printf("E' stata selezionata la scelta 1\n");  /*qui calcola i soldi della transazioni*/
                        NodoN * ok = testa;

                        int totale = 0;
                        while (ok != NULL) {
                            totale = totale + ok -> soldi;
                            ok = ok -> next;
                        }
                        FullWrite(nuovo_socket, & totale, sizeof(int));

                        close(nuovo_socket);
                        break;

                    case 2:
                        printf("E' stata selezionata la scelta 2\n");              /*va a cercare una transazione specifica*/

                        FullRead(nuovo_socket, & numero_transazione, sizeof(int));

                        NodoN * ok1 = testa;

                        while (ok1 != NULL) {

                            if (ok1 -> numero_blocco == numero_transazione) {
                                FullWrite(nuovo_socket, ok1, sizeof(NodoN));
                                break;
                            }
                            ok1 = ok1 -> next;

                        }
                        close(nuovo_socket);
                        break;

                    case 3:
                        printf("E' stata selezionata la scelta 3\n");          /*scorre la lista dalla testa a partire da un numero */
                        sleep(1);
                        NodoN * ok2 = testa;
                        FullRead(nuovo_socket, & numero_transazione, sizeof(int));

                        while (numero_transazione > 0) {
                            ok2 = ok2 -> next;
                            FullWrite(nuovo_socket, ok2, sizeof(NodoN));
                            numero_transazione--;
                        }

                        close(nuovo_socket);
                        break;

                    case 4:
                        printf("E'stata selezionata la scelta 4\n");        /*cerca l'indirizzo ip richiesto*/
                        NodoN * ok3 = testa;
                        FullRead(nuovo_socket, & ip, sizeof(ip));
                        while (ok3 != NULL) {
                            if ((strcmp(ok3 -> ip_mittente, ip) == 0) || (strcmp(ok3 -> ip_destinatario, ip) == 0)) {
                                FullWrite(nuovo_socket, ok3, sizeof(NodoN));
                                break;
                            }
                            ok3 = ok3 -> next;
                        }
                        close(nuovo_socket);
                        break;

                    }
                    exit(1);
                }

            }
        }
    }
}














