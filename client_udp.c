#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h> // Ajout pour memcpy
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

#define TAILLEMAXMSG 20
#define PORT 9600

int main(int argc, char *argv[]) {

    int sockfd;
    struct sockaddr_in server_addr;
    struct hostent *server;
    char message[TAILLEMAXMSG];

    if (argc < 2) {
        fprintf(stderr, "Arguments manquants.");
        exit(0);
    }

    // 1. Ouverture du socket
    sockfd = socket(PF_INET, SOCK_DGRAM, 0);

    server = gethostbyname(argv[1]);

    // 2. Remplissage de la structure d'adresse du serveur :
    server_addr.sin_family = PF_INET;
    // on affecte l'adresse IP du serveur récupérée par gethostbyname à notre structure d'adresse dédiée à l'aide de memcpy car<
    // les deux types sont incompatibles (h_addr est un pointeur vers un caractère et server_addr.sin_addr.s_addr est un pointeur vers un entier de 32 bits non-signé)
    // de plus, memcpy copie réellement les données au lieu de seulement réassigner un pointeur
    memcpy((char *)&server_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
    server_addr.sin_port = PORT; 

    printf("Entrez le message : ");
    memset(message, 0, TAILLEMAXMSG); // sert à effacer le buffer message en le comblant de zéros pour éviter un problème de données résiduelles
    fgets(message, TAILLEMAXMSG, stdin);

    // 3. Envoi du message au serveur
    // strlen sert à envoyer seulement les octets utilisés
    sendto(sockfd, message, strlen(message), 0, (struct sockaddr *)&server_addr, sizeof(server_addr));

    // 4. Fermeture de la connexion
    close(sockfd);

    return 0;
}
