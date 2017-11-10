#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#ifndef ERROR_H_
#define ERROR_H_

void warning_msg(const char *fmt, ...);
void error_msg(const char *fmt, ...);

#endif /* ERROR_H_ */
