/**
  * File:	download.h
  *
  * Date:	2017
  * Software: 	Wikimage
  *
  * Summary of File:
  *
  *   This file includes the functions related to download
  *
  */

#ifndef _DOWNLOAD_H_
#define _DOWNLOAD_H_

#include "config.h"

#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

struct MemoryStruct {
	char* memory;
	size_t size;
};

extern size_t write_data (
	void* buffer,
	size_t size,
	size_t nmemb,
	void* userp
);

extern int download (
	struct MemoryStruct* mem,
	char* url
);

#endif // _DOWNLOAD_H_
