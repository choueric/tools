#include "clog.h"

static void test()
{
	error("error msg.");
	warn("warn msg.");
	info("info msg.");
	debug("debug msg.");
	trace("trace msg.");
	printf("\n");
}

int main(int argc, char **argv)
{
	clog_set_level(CLOG_NONE);
	test();
	clog_set_level(CLOG_ERROR);
	test();
	clog_set_level(CLOG_WARNING);
	test();
	clog_set_level(CLOG_INFO);
	test();
	clog_set_level(CLOG_DEBUG);
	test();
	clog_set_level(CLOG_TRACE);
	test();
    return 0;
}
