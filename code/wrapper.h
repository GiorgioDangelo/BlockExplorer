#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <sys/select.h>
#include <sys/time.h>
#include <pthread.h>


// CLIENT
int Socket(int family, int type, int protocol)
{
  int listenfd;
    if((listenfd = socket(family, type, protocol)) < 0)
    {
      perror("socket");
      exit(1);
    }
    return listenfd;
};
void Connect(int listenfd, struct sockaddr_in servaddr, int size_servaddr)
{
  if (connect(listenfd, (struct sockaddr *) &servaddr, size_servaddr) < 0)
  {
  	perror("errore connessione");
  	exit(1);
  }
}
// SERVER
// SOCKET(int family,int type,int protocolo)

void Bind(int listenfd, struct sockaddr_in servaddr, int size_servaddr)
{
  if (bind(listenfd, (struct sockaddr *) &servaddr, size_servaddr) < 0) {
    perror("bind");
    exit(1);
  }
};

void Listen(int listenfd)
{
  if ( listen(listenfd, 1024) < 0)
  {
    perror("listen");
    exit(1);
  }
};

int Accept(int listenfd)
{
  int connfd;
  if (( connfd= accept(listenfd, (struct sockaddr *) NULL, NULL) ) < 0)
  {
    perror("accept");
    exit(1);
  }
  return connfd;
}

//assicurare la scrittura anche in presenza di interrupt da parte del sistema operativo
ssize_t FullWrite(int fd, const void *buf, size_t count)
{
size_t nleft;
ssize_t nwritten;
nleft = count;
while (nleft > 0) {

if ( (nwritten = write(fd, buf, nleft)) < 0) {
if (errno == EINTR) { /* if interrupted by system call */
continue;
/* repeat the loop */
} else {
exit(nwritten); /* otherwise exit with error */
}
}
nleft -= nwritten;
/* set left to write */
buf +=nwritten;
/* set pointer */
}
return (nleft);
}

ssize_t FullRead(int fd, void *buf, size_t count)
{
size_t nleft;
ssize_t nread;
nleft = count;
while (nleft > 0) {
/* repeat until no left */
if ( (nread = read(fd, buf, nleft)) < 0) {
if (errno == EINTR) { /* if interrupted by system call */
continue;
/* repeat the loop */
} else {
exit(nread);
/* otherwise exit */
}
} else if (nread == 0) { /* EOF */
break;
/* break loop here */
}
nleft -= nread;
/* set left to read */
buf +=nread;
/* set pointer */
}
buf=0;
return (nleft);
}


