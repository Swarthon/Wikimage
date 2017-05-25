#include "string_manipulate.h"

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
