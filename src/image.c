#include "image.h"

int init(Image** image, ImageInfo** image_info, ExceptionInfo** exception, const char* filename){
	*exception = AcquireExceptionInfo();
	*image_info = CloneImageInfo((ImageInfo *) NULL);

	(void) strcpy((*image_info)->filename, filename);
	*image = ReadImage(*image_info, *exception);
	if ((*exception)->severity != UndefinedException)
		CatchException(*exception);
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
