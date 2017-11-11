#ifndef ERROR_H_
#define ERROR_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include "garbage.h"

void warning_msg(const char *fmt, ...);
void error_msg(const char *fmt, ...);

#endif /* ERROR_H_ */
