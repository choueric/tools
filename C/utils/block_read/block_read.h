#pragma once

#include <stdint.h>

int block_read(int fd, void *data, size_t *p_nbytes, int timeout_ms);

