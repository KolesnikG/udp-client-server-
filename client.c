#include "prototypes.h" 


int main(int argc, char **argv) {
    int sockfd, port, n;
    int serverlen;
    struct sockaddr_in serveraddr;
    struct hostent *server;
    char *hostname;
    char buf[BUFSIZE];

    if (argc != 3) {
       fprintf(stderr,"Введите: %s <hostname> <port>\n", argv[0]);
       exit(0);
    }

    hostname = argv[1];
    port = atoi(argv[2]);

    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) 
        error("Ошибка открытия сокета");

    server = gethostbyname(hostname);
    if (server == NULL) {
        fprintf(stderr,"Нет такого адреса: %s\n", hostname);
        exit(0);
    }



    //установка адреса сервера
    bzero((char *) &serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
    (char *)&serveraddr.sin_addr.s_addr, server->h_length);
    serveraddr.sin_port = htons(port);

    // ввод сообщения
    bzero(buf, BUFSIZE);
    printf("Введите сообщение: ");
    fgets(buf, BUFSIZE, stdin);

    // отправка сообщения серверу
    serverlen = sizeof(serveraddr);
    n = sendto(sockfd, buf, strlen(buf), 0, &serveraddr, serverlen);
    if (n < 0) 
      error("Ошибка отправки");
    
    // Ответ сервера
    n = recvfrom(sockfd, buf, strlen(buf), 0, &serveraddr, &serverlen);
    if (n < 0) 
      error("Ошибка получения ответа");
    printf("сообщение от сервера: %s", buf);
    return 0;
}

void error_messege(char *inf) {
  perror(inf);
  exit(1);
}