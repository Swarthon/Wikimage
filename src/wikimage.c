#include "config.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <curl/curl.h>

#include <getopt.h>

#include <magick/MagickCore.h>
#include <magick/draw.h>

#ifdef USE_X11
#include <X11/Xlib.h>
#endif // USE_X11

struct MemoryStruct {
	char* memory;
	size_t size;
};

/* Functions to download & write data*/
size_t write_data(void* buffer, size_t size, size_t nmemb, void* userp) {
	size_t realsize = size * nmemb;
	struct MemoryStruct* mem = (struct MemoryStruct*) userp;
	mem->memory = realloc(mem->memory, mem->size + realsize + 1);
	if(mem->memory == NULL){
		printf("Not enough memory\n");
		return 0;
	}
	memcpy(&(mem->memory[mem->size]), buffer, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;
	return realsize;
}
int download(struct MemoryStruct* mem, char* url){
	CURL* easy;

	mem->size = 0;
	mem->memory = malloc(1);
	easy = curl_easy_init();
	curl_easy_setopt(easy, CURLOPT_WRITEDATA, mem);
	curl_easy_setopt(easy, CURLOPT_WRITEFUNCTION, write_data);
	curl_easy_setopt(easy, CURLOPT_URL, url);
	curl_easy_setopt(easy, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
	curl_easy_setopt(easy, CURLOPT_FOLLOWLOCATION, 1);
	curl_easy_setopt(easy, CURLOPT_NOPROGRESS, 0L);

#ifdef DEBUG
	curl_easy_setopt(easy, CURLOPT_VERBOSE, 1L);
	curl_easy_setopt(easy, CURLOPT_DEBUGFUNCTION, trace);
#endif

	CURLcode res = curl_easy_perform(easy);
	if(res != CURLE_OK){
	      fprintf(stderr, "%s\n", curl_easy_strerror(res));
	      return 1;
	}

	curl_easy_cleanup(easy);
	return 0;
}

/* Functions to manipulate strings */
void substr_to(const char* src, const char last, char* result) {
	int i = strchr(src, last) - src;
	memcpy(result, src, i);
	result[i] = '\0';
}
void get_url(struct MemoryStruct mem, char* image_url){
	char* fullMedia = strstr(mem.memory, "fullMedia");	/* Search specific string */
	fullMedia += 20;					/* Move at the beginning of the URL */
	substr_to(fullMedia, '"', image_url);
}
void get_desc(struct MemoryStruct mem, char* description){
	char *string = strstr(mem.memory, "escription ");
	if (string) {
		string = strstr(string, "/span>");
		string += 6;
		substr_to(string, '<', description);
		if (strlen(description) < 5)
			string = NULL;
	}
	if (!string) {
		string = strstr(mem.memory, "File:");
		string += 5;
		substr_to(string, '.', description);
	}
}

/* Image description */
int init(Image** image, ImageInfo** image_info, ExceptionInfo* exception, const char* filename){
	GetExceptionInfo(exception);
	*image_info = CloneImageInfo((ImageInfo *) NULL);

	(void) strcpy((*image_info)->filename, filename);
	*image = ReadImage(*image_info, exception);
	if (exception->severity != UndefinedException)
		CatchException(exception);
	if (*image == (Image*) NULL)
		return 0;
	return 1;
}
void scale(Image** image, ImageInfo* image_info, ExceptionInfo* exception, int width, int height){
	float scale_x = (float)((*image)->columns) / (float)(width);
	float scale_y = (float)((*image)->rows) / (float)(height);
	if(scale_x < scale_y)
		*image = AdaptiveResizeImage(*image, width, (*image)->rows / scale_x, exception);
	else
		*image = AdaptiveResizeImage(*image, (*image)->columns / scale_y, height, exception);
}
void excerpt(Image** image, ImageInfo* image_info, ExceptionInfo* exception, int width, int height){
	RectangleInfo rectangle;
	rectangle.width = width;					rectangle.height = height;
	rectangle.x	= ((*image)->columns - width) / 2;	rectangle.y = ((*image)->rows - height) / 2;
	*image = ExcerptImage(*image, &rectangle, exception);
}
void annotate(Image** image, ImageInfo* image_info, ExceptionInfo* exception, char* description, char* font){
	DrawInfo *draw_info;
	draw_info = CloneDrawInfo(image_info, (DrawInfo*)NULL);
	draw_info->undercolor.blue = 20000;
	draw_info->undercolor.green = 20000;
	draw_info->undercolor.red = 20000;
	draw_info->undercolor.opacity = 10000;
	draw_info->fill.blue = 65535;
	draw_info->fill.red = 65535;
	draw_info->fill.green = 65535;
	draw_info->stroke_width = 10;
	draw_info->text = description;
	draw_info->pointsize = 35;
	draw_info->gravity = SouthEastGravity;
	draw_info->font = font;
	AnnotateImage(*image, draw_info);
}
void save(Image* image, ImageInfo* image_info, const char* name){
	(void) strcpy(image->filename,name);
	WriteImage(image_info, image);
}

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
	ExceptionInfo exception;
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
	int width = HEIGHT, height = HEIGHT;
#endif

	while((c = getopt_long (argc, argv, "u:w:h:f:l",long_options, 0)) != -1)
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
		scale(&image, image_info, &exception, width, height);
		save(image, image_info, "Scaled_image.png");
		excerpt(&image, image_info, &exception, width, height);
		save(image, image_info, "Croped_image.png");
		annotate(&image, image_info, &exception, description, font);
		save(image, image_info, "Text_image.png");

		if(loop_time != -1)
			sleep(loop_time);
	} while(loop_time != -1);

	return 0;
}
