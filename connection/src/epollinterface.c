#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/epoll.h>

#include "epollinterface.h"
#include "logging.h"

int epoll_fd;

// create epoll file descriptor
void epoll_init()
{
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1)
    {
        rsp_log_error("Couldn't create epoll FD");
        exit(1);
    }
}

// Add a file descriptor and an associated structure to an epoll FD
void epoll_add_handler(struct epoll_event_handler *handler, uint32_t event_mask)
{
    struct epoll_event event;

    memset(&event, 0, sizeof(struct epoll_event));
    event.data.ptr = handler;
    event.events = event_mask;
    // epoll_ctl allows you to add, modify and delete file descriptors from the list that a particular epoll file descriptor is watching
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, handler->fd, &event) == -1)
    {
        rsp_log_error("Couldn't register server socket with epoll");
        exit(-1);
    }
}

void epoll_remove_handler(struct epoll_event_handler *handler)
{
    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, handler->fd, NULL);
}

struct free_list_entry
{
    void *block; //  A pointer to the memory block that this entry represents.
    struct free_list_entry *next; // A pointer to the next entry in the list, or NULL if this is the last entry
};

struct free_list_entry *free_list = NULL;

// Store and manage blocks of memory that are no longer in use, potentially for reuse later
void epoll_add_to_free_list(void *block)
{
    struct free_list_entry *entry = malloc(sizeof(struct free_list_entry));
    entry->block = block;
    entry->next = free_list;
    free_list = entry;
}

// Wait for incoming events on the epoll FD, and when they arrive it extracts the appropriate handler, and calls its callback with its closure data
void epoll_do_reactor_loop()
{
    struct epoll_event current_epoll_event;

    while (1)
    {
        struct epoll_event_handler *handler;

        // Block until events are available on any one of the file descriptors
        epoll_wait(epoll_fd, &current_epoll_event, 1, -1);
        handler = (struct epoll_event_handler *)current_epoll_event.data.ptr;
        handler->handle(handler, current_epoll_event.events);

        struct free_list_entry *temp;
        while (free_list != NULL)
        {
            free(free_list->block);
            temp = free_list->next;
            free(free_list);
            free_list = temp;
        }
    }
}
