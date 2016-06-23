#include <arpa/inet.h>
#include "prototypes.h" 


int main(int argc, char **argv) {
  int sockfd; 
  int portno; 
  int clientlen;
  struct sockaddr_in serveraddr; 
  struct sockaddr_in clientaddr; 
  struct hostent *hostp; /* client host info */
  char buf[BUFSIZE]; 
  char *hostaddrp; 
  int optval; 
  int n; 

  
  if (argc != 2) {
    fprintf(stderr, "usage: %s <port>\n", argv[0]);
    exit(1);
  }
  
  portno = atoi(argv[1]);

   
  //Создание сокета  
  sockfd = socket(AF_INET, SOCK_DGRAM, 0);
  if (sockfd < 0) 
    error("Ошибка открытия сокета");

  /* 
  Устанавливает пераметр сокета SO_REUSEADDR на уровне SOL_SOCKET(API).
  Ножно для перезапуска удаленного(killed) процесса по тому же адресу.
  используеться из-за того, что порт переходит в ждущий режим для гарантии передачи данных.
   */
  optval = 1;
  setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optval , sizeof(int));

  // установка интернет адреса сервера
  bzero(&serveraddr, sizeof(serveraddr));
  serveraddr.sin_family = AF_INET;
  serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
  serveraddr.sin_port = htons((unsigned short)portno);

  //связка сокета и порта 
  if (bind(sockfd, (struct sockaddr*) &serveraddr, sizeof(serveraddr)) < 0) 
    error("Ошибка связывания сокета и порта");

  //ожидание датаграммы
  clientlen = sizeof(clientaddr);
  while (1) {

    bzero(buf, BUFSIZE);
    //получения длины сообщения из сокета
    n = recvfrom(sockfd, buf, BUFSIZE, 0,
         (struct sockaddr *) &clientaddr, &clientlen);
    if (n < 0)
      error("Ошибка получения");

    
    //Отправка данных клиенту
    n = sendto(sockfd, buf, strlen(buf), 0, (struct sockaddr *) &clientaddr, clientlen);
    if (n < 0) 
      error("Ошибка при отправке");
    else
      // нет сообщений о получении каких либо данных, вместо этого полученные данные просто 
      //  отправляються клиенту с выводом его адреса
      printf("Датаграмма отправлена по адресу: %s: %d\n", inet_ntoa(clientaddr.sin_addr),
                                                          (int) ntohs(clientaddr.sin_port));
                                    
}}


void error_messege(char *inf) {
  perror(inf);
  exit(1);
}