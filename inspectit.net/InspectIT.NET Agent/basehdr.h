#pragma once

#define _WIN32_DCOM

#include "stdio.h"
#include "stdlib.h"
#include "malloc.h"
#include "stdarg.h"
#include "stdint.h"

#include "cor.h"
#include "corhdr.h"
#include "corhlpr.h"
#include "corerror.h"
#include "corpub.h"
#include "corprof.h"
#include "cordebug.h"

#define COM_METHOD( TYPE ) TYPE STDMETHODCALLTYPE

#define MAX_LENGTH 256
#define MAX_BUFFERSIZE 1024

#define JAVA_INT int32_t
#define JAVA_LONG int64_t

#define METHOD_ID unsigned int