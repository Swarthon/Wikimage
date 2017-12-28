/**
  *  __        ___ _    _
  *  \ \      / (_) | _(_)_ __ ___   __ _  __ _  ___
  *   \ \ /\ / /| | |/ / | '_ ` _ \ / _` |/ _` |/ _ \
  *    \ V  V / | |   <| | | | | | | (_| | (_| |  __/
  *     \_/\_/  |_|_|\_\_|_| |_| |_|\__,_|\__, |\___|
  *					  |___/
  *
  * File :	wikimage.c
  *
  * Date :	2017
  * Software :	Wikimage
  *
  * Summary :
  *
  * This program is used to define a Wikimedia image as background image.
  * It downloads, resize and crop to fit to your screen.
  * It also adds a description of what it is.
  *
  */

#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#ifdef USE_X11
#include <X11/Xlib.h>
#endif // USE_X11

#include "image.h"
#include "download.h"
#include "string_manipulate.h"

struct option long_options[] ={
	{"url", required_argument, 0, 'u'},
	{"width", required_argument, 0, 'w'},
	{"height", required_argument, 0, 'h'},
	{"font", required_argument, 0, 'f'},
	{"loop", optional_argument, 0, 'l'},
	{0, 0, 0, 0}
};

int main (int argc, char** argv) {
	struct MemoryStruct mem;
	int res;
	char* url = "http://commons.wikimedia.org/wiki/Special:Random/File";
	char description[500];
	char image_url[500];
	ExceptionInfo *exception;
	Image *image = NULL;
	ImageInfo *image_info = NULL;
	FILE* file;
	int c;
	char* font = NULL;
	int loop_time = -1;

#ifdef USE_X11
	Display* d = XOpenDisplay(NULL);
	Screen*  s = DefaultScreenOfDisplay(d);
	int width = s->width, height = s->height;
#else
	int width = WIDTH, height = HEIGHT;
#endif

	while((c = getopt_long (argc, argv, "u:w:h:f:l:",long_options, 0)) != -1)
		switch (c) {
			case 'u':
				url = optarg;
				break;
			case 'w':
				width = atoi(optarg);
				break;
			case 'h':
				height = atoi(optarg);
				break;
			case 'f':
				font = optarg;
				break;
			case 'l':
				loop_time = atoi(optarg);
				break;
			}

	do {
		res = download(&mem, url);
		if (res != 0)
			continue;
		if(!(file = fopen("File.html", "wb"))){
			printf("Could not open file\n");
			exit(1);
		}

		fwrite(mem.memory, mem.size, 1, file);
		fclose(file);

		get_desc(mem, description);
		get_url(mem, image_url);
		res = download(&mem, image_url);
		if (res != 0)
			continue;
		if(!(file = fopen("Image.png", "wb"))){
			printf("Could not open file\n");
			exit(1);
		}
		fwrite(mem.memory, mem.size, 1, file);
		fclose(file);

		if(!init(&image, &image_info, &exception, "Image.png"))
			continue;
		scale(&image, image_info, exception, width, height);
		save(image, image_info, "Scaled_image.png");
		excerpt(&image, image_info, exception, width, height);
		save(image, image_info, "Croped_image.png");
		annotate(&image, image_info, exception, description, font);
		save(image, image_info, "Text_image.png");

		if(loop_time != -1)
			sleep(loop_time);
	} while(loop_time != -1);

	return 0;
}
