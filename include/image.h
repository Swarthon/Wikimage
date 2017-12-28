/**
  * File:	image.h
  *
  * Date:	2017
  * Software: 	Wikimage
  *
  * Summary of File:
  *
  *   This file includes the functions related to image manipulation
  *
  */

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include "config.h"

#include <string.h>

/* The include path is different between version 6 and 7 of ImageMagick */
#ifdef HAVE_MAGICK_6 
#include <magick/MagickCore.h>
#include <magick/draw.h>
#else
#include <MagickCore/MagickCore.h>
#include <MagickCore/draw.h>
#endif

extern int init (
	Image** image,
	ImageInfo** image_info,
	ExceptionInfo** exception,
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
