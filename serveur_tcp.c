#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define PORT 9600
#define TAILLEMAXMSG 20

int main() {
    
    int sockfd, socketClient;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[TAILLEMAXMSG];

    // 1. Ouverture du socket
    sockfd = socket(PF_INET, SOCK_STREAM, 0);

    // 2. Remplissage de la structure d'adresse du serveur
    server_addr.sin_family = PF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = PORT;

    // 3. Bind
    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 4. Mise en mode écoute du socket
    listen(sockfd, 1);

    // 5. Acceptation de la connexion client
    socketClient = accept(sockfd, (struct sockaddr *)&client_addr, &client_len);

    // 6. Réception des données envoyées par le client
    read(socketClient, buffer, TAILLEMAXMSG);

    printf("Message reçu : %s\n", buffer);

    // 7. Envoi d'une réponse au client 
    char ack[] = "Message reçu.";
    write(socketClient, ack, sizeof(ack));

    // 8. Fermeture de la connexion 
    close(socketClient);

    // 9. Fermeture du socket du serveur
    close(sockfd);

    return 0;
}