//Sent an email to Tychonovich regarding this assignment being late for around half an hour.

#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h> //memset
#include <stdlib.h> //atoi
#include <poll.h>


int main(int argc , char *argv[]){
//    printf("%i", argc);
    int connection;
    if(argc == 1) { // server
        srandom(getpid());
        int port = 0xc000 | (random()&0x3fff); // random element of 49152–65535

        struct sockaddr_in ipOfServer;
        memset(&ipOfServer, 0, sizeof(struct sockaddr_in));
        ipOfServer.sin_family = AF_INET;
        ipOfServer.sin_addr.s_addr = htonl(INADDR_ANY);
        ipOfServer.sin_port = htons(port);

        int listener = socket(AF_INET, SOCK_STREAM, 0);

        bind(listener, (struct sockaddr*)&ipOfServer , sizeof(ipOfServer));

        listen(listener, 20);

        system("host $HOSTNAME"); // display all this computer's IP addresses
        printf("The server is now listening on port %d\n", port);

        printf("Waiting for a connection\n");
        // get a connection socket (this call will wait for one to connect)
        connection = accept(listener, (struct sockaddr*)NULL, NULL);
        close(listener);
        printf("Connection made");

    struct pollfd pollstruct[2];
    pollstruct[0].fd = connection;
    pollstruct[0].events = POLLIN;
    pollstruct[1].fd = 0;
    pollstruct[1].events = POLLIN;
    int status;
    char buffer[4097];
    while(1) {
        status = poll(pollstruct, 2, 6000);
        if(status < 0) return 0;
        if(pollstruct[1].revents & POLLIN) {
             read(0, buffer, 4096);
             write(connection, buffer, strlen(buffer));
             memset(buffer, 0, sizeof(buffer));
        }
        if(pollstruct[0].revents & POLLIN) {
             read(connection, buffer, 4096);
             write(0, buffer, strlen(buffer));
             memset(buffer, 0, sizeof(buffer));

           }
    }

    close(connection);
    } else { // client
//printf("hereelse");
        int port = atoi(argv[2]);
        int address = inet_addr(argv[1]);

        struct sockaddr_in clientIP;
        memset(&clientIP, 0, sizeof(struct sockaddr_in));
        clientIP.sin_family = AF_INET;
        clientIP.sin_addr.s_addr = address;
        clientIP.sin_port = htons(port);
  //      printf("here1");
        //Create Socket  (Remember to check the return value to see if an error occured)
        int connection = socket(AF_INET, SOCK_STREAM, 0);
        if(connection == -1) printf("No work");
    //    printf("here");
        connect(connection, (struct sockaddr *)&clientIP, sizeof(clientIP));
printf("Connection made");

    struct pollfd pollstruct[2];
    pollstruct[0].fd = connection;
    pollstruct[0].events = POLLIN;
    pollstruct[1].fd = 0;
    pollstruct[1].events = POLLIN;
    int status;
    char buffer[4097];
    while(1) {
        status = poll(pollstruct, 2, 6000);
        if(status < 0) return 0;
        if(pollstruct[1].revents & POLLIN) {
             read(0, buffer, 4096);
             write(connection, buffer, strlen(buffer));
        }
        if(pollstruct[0].revents & POLLIN) {
             read(connection, buffer, 4096);
             write(0, buffer, strlen(buffer));
        }
    }

    close(connection);
    }
}