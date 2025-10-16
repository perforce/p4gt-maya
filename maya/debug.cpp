#include <stdio.h>

#include "debug.h"

#include <time.h>

static FILE *f=0;


void DBG_open() {
	if (!f)
		f = fopen("c:\\p4maya.log", "a");
}

void DBG_output(char *msg) {
char	buff[32];
	_strtime(buff);
	fprintf(f, buff);
	fprintf(f, "\t");
	fprintf(f, msg);
	fprintf(f, "\n");
	fflush(f);
}

void DBG_close() {
	fclose(f);
}