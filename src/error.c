/*
 * error.c
 *
 *  Created on: Mar 18, 2017
 *      Author:		Erik Pitko(xpitko00), FIT
 */

#include "error.h"

void qWarning(const char *form, va_list args, int fatal){
	fprintf(stderr, "%s", "ERROR: ");
	vfprintf(stderr, form, args);
	fprintf(stderr, "\n");

	if(fatal){
		garbageFree();
		exit(1);
	}
}

void warning_msg(const char *form, ...){
	va_list args;
	va_start(args, form);

	qWarning(form, args, 0);

	va_end(args);
}

void error_msg(const char *form, ...){
	va_list args;
	va_start(args, form);

	qWarning(form, args, 1);

	va_end(args);
}
