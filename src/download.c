#include "download.h"

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
