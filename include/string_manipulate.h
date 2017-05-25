#ifndef _STRING_MANIPULATE_H_
#define _STRING_MANIPULATE_H_

#include "config.h"
#include "download.h"

#include <string.h>

extern void substr_to (
	const char* src,
	const char last,
	char* result
);

void get_url (
	struct MemoryStruct mem,
	char* image_url
);

extern void get_desc(
	struct MemoryStruct mem,
	char* description
);

#endif // _STRING_MANIPULATE_H_
