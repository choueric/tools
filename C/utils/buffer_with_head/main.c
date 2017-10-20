#include "bufhead.h"

int main(int argc, char **argv)
{
	buf_t pBuf = new_buf(10);
	bufhead *pBufHead = get_bufhead(pBuf);
	printf("buffer count = %d, size = %d\n", pBufHead->count, pBufHead->len);

	free_buf(pBuf);

	return 0;
}
