#pragma pack(1)
struct bufhead {
    int len;
    int count;
    char buf[1];
};
#pragma pack()

typedef char *buf_t;

buf_t new_buf(size_t len)
{
    struct bufhead *head = NULL;
    
    if (len <= 0)
        return NULL;

    head = calloc(1, sizeof(struct bufhead) + len - 1);
    if (head == NULL) {
        perror("malloc failed");
        return NULL;
    }
    
    head->len = len;
    head->count = 0;
    
    return head->buf;
}

struct bufhead *get_bufhead(buf_t buf)
{
    return (struct bufhead *)(buf - sizeof(struct bufhead) + 1);
}
