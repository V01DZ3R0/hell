#include <stdio.h>
#include <sys/socket.h> //for sock_ets (thx cap) && alzo for shutdown(2)
#include <string.h>     //for strlen, coz sizeof()-1 does not work here
#include <arpa/inet.h>  //for inet_addr
#include <unistd.h>     //write && for fork
//forforforfork
#include <netdb.h> //for getnameinfo() and serviceBuffer[NI_MAXSERV]
#include <netinet/in.h>
#include <sys/types.h> // for .html (i guess)
#include <arpa/inet.h>
#include <stdlib.h>
#include <signal.h> //for #define SIGKILL 9

//IMPORTANT
//all commented printf() down below are logging requests into terminal, can be rewrited to file or any other logs

#define SIZE 1024
#define BACKLOG 10 // Passed to listen()

void report(struct sockaddr_in *servAddr);
void setHttpHeader(char httpHeader[]);
void seticon(char icon[]);
void servecycle(int sockfd);
void handler(int client_sock, char httpHeader[]);
void interface(pid_t pid);
void FooHeader(char httpHeader[], char req1[250]);

int main()
{

    pid_t pid = 0;

    int s; //"main" socket

    struct sockaddr_in serv;

    char httpHeader[8000] = "HTTP/1.1 200 OK\r\n\n";

    s = socket(AF_INET, SOCK_STREAM, 0);

    if (s < 0)
    {
        puts("\tCreating socket failed");
    }
    serv.sin_family = AF_INET;
    serv.sin_addr.s_addr = INADDR_ANY;
    serv.sin_port = htons(1666);

    if (bind(s, (struct sockaddr *)&serv, sizeof(serv)) < 0)
    {
        puts("\tBind failed");
    }

    listen(s, SOMAXCONN); //somaxconn == 128  btw

    printf("\n\t\"ext\" to exit the program\r\n\t\"xd\"  to  do smth\r\n\t\"rst\" to restart the server\r\n");

    puts("\n\tWaiting for incoming connections...");

    report(&serv); // Custom report function

    pid = fork();

    if (pid < 0)
    {
        puts("\n\tFork failed. Sad enough.");
    }

    if (pid > 0)
    {
        printf("\n\tServer Demon PID: %d\n", pid);

        char str[100], comp1[] = "ext", comp2[] = "xd", comp3[] = "rst";
        while (69)
        {
            scanf("%s", str);
            if (strcmp(comp1, str) == 0)
            {

                shutdown(s, SHUT_RDWR); //to awoid TIME_WAIT
                close(s);
                kill(pid, SIGKILL); //SIGKILL defined as 9
                puts("Exit");
                return 0;
            }

            if (strcmp(comp2, str) == 0)
            {
                puts("Maybe it rly did smth i dunno lol\r\n");
            }
            if (strcmp(comp3, str) == 0)
            {

                kill(pid, SIGKILL);
                pid = fork();
                if (pid < 0)
                {
                    puts("\n\tFork failed. Sad enough.");
                }
                if (pid == 0)
                {
                    puts("Restarted");
                    break;
                }
            }
        }
    }

    if (pid == 0)
    {
        servecycle(s);
    }
}

void servecycle(int sockfd)
{
    int count = 0;
    int conn = 0; // client connection

    while (1)
    {
        char buf[8000] = "HTTP/1.1 200 OK\n\n";
        conn = accept(sockfd, 0, 0);
        if (conn < 0)
        {
            perror("Accept failed");
        }
        count++;
        //printf("\r\n\n\r\tRequest accepted - %d\n\r\n", count);
        handler(conn, buf);
        send(conn, buf, sizeof(buf), 0);
        close(conn);
    }
}

void handler(int conn, char out[])
{
    char req[500];
    char *resp;

    read(conn, req, 500);
    printf("%s", req); //prints the full request
    char *req1, http[] = "home";
    strtok(req, " /");
    req1 = strtok(NULL, " /");
    /* 
        printf("\n\r %s\n", req); //simple method and request log
        printf("%s", req1); 
    */
    if (strcmp(req1, http) == 0)
    {
        setHttpHeader(out);
    }
    else
    {
        FooHeader(out, req1);
    }
}

void FooHeader(char httpHeader[], char req1[250]) //kinda like 404 HTTP err 
{
      if (strcmp(req1, "HTTP") == 0)
    {
        req1="Literally nothing...";
    }

    char resp[100] = "<html><head/><body><h1>You requested: ", resp1[100] = "</h1><p><a target=\"_self\" href=\"/home\">Homepage</a></p></body></html>";
    strcat(resp, req1);
    strcat(resp, resp1);
    strcat(httpHeader, resp);
}

void setHttpHeader(char httpHeader[])
{
    char responseData[8000] = "";
    char line[1000] = "";

    // file object to return
    FILE *htmlData = fopen("site.html", "r");
    while (fgets(line, sizeof(line), htmlData) != 0)
    {
        strcat(responseData, line);
    }
    strcat(httpHeader, responseData);
    fclose(htmlData);
}

void report(struct sockaddr_in *servAddr)
{
    char hostBuffer[INET6_ADDRSTRLEN];
    char serviceBuffer[NI_MAXSERV];
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
    printf("\n\tServer listening on http://%s:%s (localhost)\n", hostBuffer, serviceBuffer);
}
