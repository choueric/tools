#include "ini.h"

/*
 * find the first character @c in string @str from index number @s.
 * The length of string is @n.
 * search range is [s, n-1]
 * if found, return the character's index; if not found, return @n
 * if parameters are invalid, return @n, which means failure.
 */
static int str_find(char c, const char *str, int s, int n)
{
    if (s >= n || str == NULL)
        return n;

    while (s < n && str[s++] != c)
        ;

    return s;
}

/*
 * parse ini-format file. "key = val" pair in each line.
 * key and val strings shouldn't have space characters.
 * can strip space characters around key string and val string.
 * # is comment line
 * @line: the string line which are parsed.
 * @n: length of @line
 * @key: store the "key" in line, should be big enough
 * @val: store the "val" in line, should be big enough
 * return: 0, succeed; otherwise, failure.
 */
static int parse_line(const char *line, int n, char *key, char *val)
{
    int index = 0;
    int equal, r;

    equal = str_find('=', line, 0, n);
    if (equal == n)
        return 1;
    r = str_find('=', line, equal + 1, n);
    if (r != n)
        return 1;

    /* skip space characters at front of line */
    for (index = 0; index < n; index++) {
        if (line[index] != ' ' && line[index] != '\t')
            break;
    }
    /* skip blank line or comment line or no key string line*/
    if (index == n || line[index] == '#' || line[index] == '=')
        return 2;

	equal--;
    /* store key until find space character or '=' */
    for (r = equal - 1; line[r] == ' ' || line[r] == '\t'; --r)
        ;
    memcpy(key, line + index, r - index + 1);

    /* skip space characters before val string */
    for (index = equal + 1; index != n; ++index) {
        if (line[index] != ' ' && line[index] != '\t')
            break;
    }
    if (index == n)  // return if no val string
        return 3;

    /* find last no-space characters */
    for (r = n - 1; line[r] == ' ' || line[r] == '\t'; --r)
        ;
    memcpy(val, line + index, r - index);

    return 0;
}

/***************************************************/


FILE *OpenIniFile(const char *filename)
{
	FILE *pf = NULL;

	if (filename == NULL)
		return NULL;

	pf = fopen(filename, "r");
	return pf;
}

int CloseIniFile(FILE *pf)
{
	fclose(pf);

	return 0;
}

int ParseIniLines(FILE *pf, parsefunc_t func, void *arg)
{
	char *line = NULL;
	ssize_t read;
	size_t len = 0;
	char key[256];
	char val[256];

	if (pf == NULL || func == NULL)
		return 1;

	rewind(pf);

	while ((read = getline(&line, &len, pf)) != -1) {
		memset(key, 0, 256);
		memset(val, 0, 256);
		if (parse_line(line, strlen(line), key, val))
			continue;
		func(key, val, arg);
	}

	free(line);

	return 0;
}


int ParseIni(const char *filename, parsefunc_t func, void *arg)
{
	FILE *pf = OpenIniFile(filename);
	if (pf == NULL) {
		printf("cant' open ini file\n");
		return -1;
	}

	ParseIniLines(pf, func, arg);

	CloseIniFile(pf);

	return 0;
}
