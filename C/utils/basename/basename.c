#include <string.h>
#include <stdlib.h>

#define PATH_MAX 128
#define DEFAULT_RESULT_DOT "."
#define DEFAULT_RESULT_SLASH "/"

/*
 * @path: NULL, use current directory ".".
 */
char *basename_r(char *path, char *buf, int buflen)
{
    char i_fixed_buf[PATH_MAX + 1];
    const int i_fixed_buf_len = sizeof(i_fixed_buf) / sizeof(char);
	int ajusted_path_len = 0;

    char *result = buf;
    char *i_buf = NULL;
    int i_buf_len = 0;

    int adjusted_path_len = 0;
    int path_len = 0;

    int i, j;

	if (buf == NULL || buflen <= 0) {
		return NULL;
	}

    if (path == NULL) {
        path = DEFAULT_RESULT_DOT;
    }
    
    path_len = strlen(path);
    if ((path_len + 1) > i_fixed_buf_len) {
        i_buf_len = path_len + 1;
        i_buf = (char *)malloc(i_buf_len * sizeof(char));
    } else {
        i_buf_len = i_fixed_buf_len;
        i_buf = i_fixed_buf;
    }

    strcpy(i_buf, path);
    adjusted_path_len = path_len;

    if (adjusted_path_len > 1) {
        while (i_buf[adjusted_path_len - 1] == '/') {
            if (adjusted_path_len != 1) {
                adjusted_path_len--;
            } else {
                break;
            }
        }
        i_buf[adjusted_path_len] = '\0';
    }

    if (adjusted_path_len > 1) {
        for (i = (adjusted_path_len - 1), j = 0; i >= 0; i--) {
            if (j == 0) {
                if (i_buf[i] == '/')
                    j = i;
            } else {
                if (i_buf[i] != '/') {
                    i++;
                    break;
                }
            }
        }
    }

    if (j != 0 && i < j) {
        strcpy(i_buf + i, i_buf + j);
    }

    ajusted_path_len -= (j - i);

    for (i = 0, j = -1; i < adjusted_path_len; i++) {
        if (i_buf[i] == '/')
            j = i;
    }

    if (j >= 0) {
        if (ajusted_path_len == 1) {
            if (2 > buflen)
                return NULL;
             else
                strcpy(result, DEFAULT_RESULT_SLASH);
        } else {
            if ((adjusted_path_len - j) > buflen)
                result = NULL;
            else
                strcpy(result, (i_buf + j + 1));
        }
    } else {
        if (adjusted_path_len == 0) {
            if (2 > buflen)
                return NULL;
            else
                strcpy(result, DEFAULT_RESULT_DOT);
        } else {
            if ((adjusted_path_len + 1) > buflen)
                result = NULL;
            else
                strcpy(result, i_buf);
        }
    }

    if (i_buf_len != i_fixed_buf_len) {
        free(i_buf);
        i_buf = NULL;
    }

    return result;
}
