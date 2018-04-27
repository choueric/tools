#include "bufhead.h"
#include <string.h>
#include <stdlib.h>

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

void free_buf(buf_t buf)
{
	bufhead *pBufHead = get_bufhead(buf);
	free(pBufHead);
}

bufhead *get_bufhead(buf_t buf)
{
    return (bufhead *)(buf - sizeof(bufhead) + 1);
}
