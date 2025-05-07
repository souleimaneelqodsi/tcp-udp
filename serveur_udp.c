#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#define TAILLEMAXMSG 20
#define PORT 9600

int main() {
    
    int sockfd;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_len = sizeof(client_addr);
    char buffer[TAILLEMAXMSG];

    // 1. Ouverture du socket
    sockfd = socket(PF_INET, SOCK_DGRAM, 0);

    // 2. Remplissage de la structure d'adresse du serveur
    server_addr.sin_family = PF_INET;
    // INADDR_ANY offre une plus grande flexibilité et un code plus "modulaire"
    // même si dans notre cas ce sera seulement l'adresse locale (127.0.0.1) qui transmettra des messages
    server_addr.sin_addr.s_addr = INADDR_ANY; 
    server_addr.sin_port = PORT; 

    // 3. Bind
    bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 4. Boucle du serveur
    int keeplistening = 1;
    while (keeplistening) {
        ssize_t msglen = recvfrom(sockfd, buffer, TAILLEMAXMSG, 0, (struct sockaddr *)&client_addr, &client_len);
        buffer[msglen] = '\0';
        printf("Message reçu : %s\n", buffer);
        printf("Voulez-vous fermer la connexion ? (Y/N) ");
        char stop = getchar(); 

        // Consomme les caractères supplémentaires (notamment le '\n') jusqu'à la prochaine ligne
        int ch;
        while ((ch = getchar()) != '\n' && ch != EOF);

        if (stop == 'Y' || stop == 'y') keeplistening = 0;
    }

    // 5. fermeture de la connexion
    close(sockfd);

    return 0;
}