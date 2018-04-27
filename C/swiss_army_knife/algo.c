#include <stdio.h>

typedef int(iterator_func_t)(int offset, int len, void *data);

/*
 * iterate from @start in the step of @page_size.
 * call @func to do actual work in each iteration.
 * if @func return < 0, it will return with failure immediately.
 * in each iteration, if the return value of @func is >= 0, iterate will
 * add the return value together, and when iterate ends, return the sum.
 * @return: how many bytes have done from @count, if fails, return value < 0
 */
int page_iterate(int start, int page_size, size_t count, iterator_func_t func, void *data)
{
	int left = 0;
	int offset = 0;
	int len = 0;
	int next_page_offset = 0;
	int sum = 0;
	int ret = 0;

	left = count;
	offset = start;
	next_page_offset = (offset / page_size + 1) * page_size;
	while (left > 0) {
		len = left > page_size ? next_page_offset - offset : left;

		if ( (ret = func(offset, len, data)) < 0) {
			printf("iterator func return failure, ret = %d\n", ret);
			return -1;
		}

		offset += len;
		left -= len;
		next_page_offset += page_size;
		sum += ret;
	}

	return sum;
}

