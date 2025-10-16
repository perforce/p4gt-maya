#include <stdio.h>

#include "debug.h"

#include <time.h>

static FILE *f=NULL;


void DBG_open()
{
	if (f == NULL)
		f = fopen("c:/temp/p4plugin.log", "a");
}

void DBG_output(char *msg)
{
	char	buff[32];

	if (f != NULL)
	{
		_strtime(buff);
		fprintf(f, "%s\t", buff);
		fprintf(f, "msg=%s\n", msg);
		fflush(f);
	}
}

void DBG_woutput(wchar_t *wmsg)
{
	char	buff[32];

	if (f != NULL)
	{
		_strtime(buff);
		fprintf(f, "%s\t", buff);
		fwprintf(f, L"wmsg=%s\n", wmsg);
		fflush(f);
	}
}

void DBG_close()
{
	if (f != NULL)
		fclose(f);
	f=NULL;
}

void DBG_log(char *msg)
{
	DBG_open();
	DBG_output(msg);
	DBG_close();
}

void DBG_wlog(wchar_t *wmsg)
{
	DBG_open();
	DBG_woutput(wmsg);
	DBG_close();
}

