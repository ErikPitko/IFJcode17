/*
 * error.c
 *
 *  Created on: Mar 18, 2017
 *      Author:		Erik Pitko(xpitko00), FIT
 */

#include "error.h"

void qWarning(const char *form, va_list args, int errno){
	fprintf(stderr, "%s", "ERROR: ");
	vfprintf(stderr, form, args);
	fprintf(stderr, "\n");

	if(errno){
		garbageFree();
		exit(errno);
	}
}

void warning_msg(const char *form, ...){
	va_list args;
	va_start(args, form);

	qWarning(form, args, 0);

	va_end(args);
}

void error_msg(int errno, const char *form, ...){
	va_list args;
	va_start(args, form);

	qWarning(form, args, errno);

	va_end(args);
}
