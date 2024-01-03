#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <netdb.h>
#include <errno.h>

#include "logging.h"
#include "netutils.h"

void make_socket_non_blocking(int socket_fd)
{
    int flags;

    flags = fcntl(socket_fd, F_GETFL, 0);
    if (flags == -1)
    {
        rsp_log_error("Couldn't get socket flags");
        exit(1);
    }

    flags |= O_NONBLOCK;
    if (fcntl(socket_fd, F_SETFL, flags) == -1)
    {
        rsp_log_error("Couldn't set socket flags");
        exit(-1);
    }
}

int connect_to_backend(char *backend_host,
                       char *backend_port_str)
{
    struct addrinfo hints; // What kind of thing we're interested in hearing about
    // Set up hints
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;     // Happy with either IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM; // Supports streaming sockets

    int getaddrinfo_error;
    struct addrinfo *addrs; // Pointer to the first item in a linked list of possible addresses

    // Convert the hostname/service descriptor
    getaddrinfo_error = getaddrinfo(backend_host, backend_port_str, &hints, &addrs);
    if (getaddrinfo_error != 0)
    {
        if (getaddrinfo_error == EAI_SYSTEM)
        {
            rsp_log_error("Couldn't find backend");
        }
        else
        {
            rsp_log("Couldn't find backend: %s", gai_strerror(getaddrinfo_error));
        }
        exit(1);
    }

    int backend_socket_fd;
    struct addrinfo *addrs_iter;
    // Loop through the connect socket
    for (addrs_iter = addrs;
         addrs_iter != NULL;
         addrs_iter = addrs_iter->ai_next)
    {
        backend_socket_fd = socket(addrs_iter->ai_family,
                                   addrs_iter->ai_socktype,
                                   addrs_iter->ai_protocol);
        if (backend_socket_fd == -1)
        {
            continue;
        }
        // If connect succeeds, we break the loop
        if (connect(backend_socket_fd,
                    addrs_iter->ai_addr,
                    addrs_iter->ai_addrlen) != -1)
        {
            break;
        }
        // Else close the socket and move to another address
        close(backend_socket_fd);
    }

    // Check if we ever managed to do a successful socket creation and connect
    if (addrs_iter == NULL)
    {
        rsp_log("Couldn't connect to backend");
        exit(1);
    }

    // Free the arrays of addresses
    freeaddrinfo(addrs);

    return backend_socket_fd;
}
