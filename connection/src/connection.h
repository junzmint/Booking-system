struct connection_closure
{
    void (*on_read)(void *closure, char *buffer, int len);
    void *on_read_closure;

    void (*on_close)(void *closure);
    void *on_close_closure;

    struct data_buffer_entry *write_buffer;
};

// Thêm dữ liệu vào write buffer và thực hiện gửi nếu có thể
extern void connection_write(struct epoll_event_handler *self, char *data, int len);

// Bắt đầu quá trình đóng kết nối, dọn dẹp và giải phóng tài nguyên
extern void connection_close(struct epoll_event_handler *self);

// Khởi tạo và cấu hình một kết nối mới
extern struct epoll_event_handler *create_connection(int connection_fd);
