#include <stdio.h>
#include <sys/socket.h>
#include <string.h>    //strlen
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
//#include <pthread.h>
#include <netdb.h> //for getnameinfo(), but it isn't using soo...
#include <netinet/in.h>
#include <sys/types.h> // for .html (i guess)
#include <arpa/inet.h>
#include <stdlib.h>

#include <stdbool.h> // for the interface()
#define SIZE 1024
#define BACKLOG 10 // Passed to listen()

void report(struct sockaddr_in *servAddr);
void setHttpHeader(char httpHeader[]);
void seticon(char icon[]);
void handler(int client_sock, char httpHeader[]);
void interface();
int parse(const char *line);
void MeineHeader(char httpHeader[], char req1[]);

int main()
{

    

    int s, new_s, count = 0; //new_s is the same as client_socket, but i choose this name

    struct sockaddr_in serv;

    char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";

    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0)
    {
        puts("Creating socket failed");
    }
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(666);

    if (bind(s, (struct sockaddr *)&serv, sizeof(serv)) < 0)
    {
        puts("Bind failed");
    }

    listen(s, SOMAXCONN); //somaxconn =128  btw
    printf("\n\t\"ext\" to exit the program\r\n\t\"xd\" to  do smth\r\n");

   /////////////// interface();

    puts("\n\tWaiting for incoming connections...");

    report(&serv); // Custom report function
    //setHttpHeader(httpHeader); // Custom function to set header

    while (1)
    {
        char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";
        new_s = accept(s, 0, 0);

        if (new_s < 0)
        {
            perror("Accept failed");
        }

        count++;

        printf("\r\n\n\r\tRequest accepted - %d\n\r\n", count);

        handler(new_s, httpHeader);

        send(new_s, httpHeader, sizeof(httpHeader), 0);

        close(new_s);
    }

    puts("Press enter to cose the programm");
    getchar();

    return 0;
}

void handler(int client_sock, char httpHeader[])
{

    char req[500];
    char *resp;
    read(client_sock, req, 500);

    //printf("%s", req); //prints the full request

    char *req1, http[] = "home";
    printf("\n\r %s\n", strtok(req, " /"));
    req1 = strtok(NULL, " /");

    printf("%s", req1);

    if (strcmp(req1, http) == 0)
    {
        setHttpHeader(httpHeader);
    }
    else
    {
        MeineHeader(httpHeader, req1);
    }
}
void MeineHeader(char httpHeader[], char req1[250])
{
    char resp[100] = "<html><head/><body><h1>You requested: ", resp1[100] = "</h1></body></html>";
    strcat(resp, req1);
    strcat(resp, resp1);
    strcat(httpHeader, resp);
}

void setHttpHeader(char httpHeader[])
{
    // File object to return
    FILE *htmlData = fopen("site.html", "r");

    char line[1000];
    char responseData[8000];
    while (fgets(line, 1000, htmlData) != 0)
    {
        strcat(responseData, line);
    }
    // char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";
    strcat(httpHeader, responseData);
}

void report(struct sockaddr_in *servAddr)
{
    char hostBuffer[INET6_ADDRSTRLEN];
    char serviceBuffer[NI_MAXSERV]; // defined in `<netdb.h>`
    socklen_t addr_len = sizeof(*servAddr);
    int err = getnameinfo(
        (struct sockaddr *)servAddr,
        addr_len,
        hostBuffer,
        sizeof(hostBuffer),
        serviceBuffer,
        sizeof(serviceBuffer),
        NI_NUMERICHOST);
    if (err != 0)
    {
        printf("It's not working!!\n");
    }
    printf("\n\tServer listening on http://%s:%s (localhost)\n\n", hostBuffer, serviceBuffer);
}

void interface()
{
    bool exit_check = false;

    char str[100], comp1[] = "ext", comp2[] = "xd", comp3[] = "start", comp4[] = "reboot";
    while (69)
    {
        if (exit_check == true)
        {
            return;
        }

        scanf("%s", str);
        if (strcmp(comp1, str) == 0)
        {
            exit(0);
        }

        if (strcmp(comp2, str) == 0)
        {
            printf("\nMaybe it rly did smth i dunno lol\r\n");
            // gotta_ext = TRUE;
        }
        if (strcmp(comp3, str) == 0)
        {
            printf("\nServer started!\r\n");
            //print report and turn into accept mode
            exit_check = true;
        }
    }
}


