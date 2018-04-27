#pragma once

#include <stdio.h>

#pragma pack(1)
struct bufhead {
    int len;
    int count;
    char buf[1];
};
#pragma pack()

typedef char *buf_t;
typedef struct bufhead bufhead;

buf_t new_buf(size_t len);
void free_buf(buf_t buf);

bufhead *get_bufhead(buf_t buf);
