#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "config.h"

#include <string.h>

#include <magick/MagickCore.h>
#include <magick/draw.h>

extern int init (
	Image** image,
	ImageInfo** image_info,
	ExceptionInfo* exception,
	const char* filename
);

extern void scale (
	Image** image,
	ImageInfo* image_info,
	ExceptionInfo* exception,
	int width,
	int height
);

extern void excerpt (
	Image** image,
	ImageInfo* image_info,
	ExceptionInfo* exception,
	int width,
	int height
);

extern void annotate(
	Image** image,
	ImageInfo* image_info,
	ExceptionInfo* exception,
	char* description,
	char* font
);

extern void save(
	Image* image,
	ImageInfo* image_info,
	const char* name
);

#endif // _IMAGE_H_
