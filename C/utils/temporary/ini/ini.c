#include "ini.h"

/*
 * parse ini-format file. "key = val" pair in each line.
 * key and val strings shouldn't have space characters.
 * can strip space characters around key string and val string.
 * # is comment line
 */
static int parse_line(char *line, int n, char **key, char **val)
{
    char *equal = NULL, *i = NULL;
	char *end = line + n;
	char *s = NULL;

    equal = strstr(line, "=");
    if (equal == NULL)
        return -1;
    i = strstr(equal + 1, "=");
    if (i != NULL)
        return -1;

    /* skip space characters at front of line */
	for (s = line; s < end; s++)
		if (*s != ' ' && *s != '\t')
			break;

    /* skip blank line or comment line or no key string line*/
    if (s == end || *s == '#' || *s == '=')
        return 2;

    /* store key until find space character or '=' */
    for (i = equal - 1; *i == ' ' || *i == '\t'; --i)
        ;

	*(i + 1) = '\0';
	*key = s;

    /* skip space characters before val string */
    for (s = equal + 1; s != end; s++)
        if (*s != ' ' && *s != '\t')
            break;
    if (s == end)  // return if no val string
        return 3;

    /* find last no-space characters */
    for (i = end - 1; *i == ' ' || *i == '\t' || *i == '\n'; --i)
        ;
		
	*(i + 1) = '\0';
	*val = s;

    return 0;
}

static int parse_lines(FILE *pf, parsefunc_t func, void *arg)
{
	char *line = NULL;
	ssize_t read;
	size_t len = 0;
	char *key = NULL;
	char *val = NULL;

	while ((read = getline(&line, &len, pf)) != -1) {
		if (parse_line(line, strlen(line), &key, &val))
			continue;
		func(key, val, arg);
	}

	free(line);

	return 0;
}

/*-----------------------------------------------------------------------*/

int ParseIni(char *filename, parsefunc_t func, void *arg)
{
	FILE *pf = NULL;

    if (filename == NULL || func == NULL)
        return -1;
    
	pf = fopen(filename, "r");
	if (pf == NULL) {
		perror("cant' open ini file");
		return -1;
	}

	parse_lines(pf, func, arg);

	fclose(pf);

	return 0;
}
