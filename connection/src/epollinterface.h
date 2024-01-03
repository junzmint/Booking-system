// typedef union epoll_data
// {
//     void *ptr;
//     int fd;
//     uint32_t u32;
//     uint64_t u64;
// } epoll_data_t;

struct epoll_event_handler
{
    int fd; // The file descriptor it's associated with
    void (*handle)(struct epoll_event_handler *, uint32_t); 
    /*A callback function to handle an epoll event which takes a pointer to a epoll_event_handler structure,
    and a uint32_t which will hold the bitmask representing the events that need to be handled*/
    void *closure; // A place to store any data the callback function needs to do its job.
};

// create epoll file descriptor
extern void epoll_init();

// Add a file descriptor and an associated structure to an epoll FD
extern void epoll_add_handler(struct epoll_event_handler *handler, uint32_t event_mask);

// Delete a file descriptor and an associated structure out of an epoll FD
extern void epoll_remove_handler(struct epoll_event_handler *handler);

// Store and manage blocks of memory that are no longer in use, potentially for reuse later
extern void epoll_add_to_free_list(void *block);

// Wait for incoming events on the epoll FD, and when they arrive it extracts the appropriate handler, and calls its callback with its closure data
extern void epoll_do_reactor_loop();
