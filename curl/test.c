#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>

typedef struct HTTPResponseHandler
{
    char *data;
    size_t size;
} HTTPResponseHandler;

size_t Write_Data(void *data_src, size_t size, size_t nmemb, void *data_dst);

int main(int argc, char **argv)
{
    CURL *curl = NULL;
    CURLcode result;
    HTTPResponseHandler response = {0};
    response.data = malloc(1);

    curl = curl_easy_init();
    if (!curl)
    {
        fprintf(stderr, "curl easy init failed\n");
        return -1; 
    }

    curl_easy_setopt(curl, CURLOPT_URL, "https://www.google.com/search?q=cats");
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, Write_Data);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void*)&response);

    result = curl_easy_perform(curl);
    if (result != CURLE_OK)
    {
        fprintf(stderr, "Error: %s\n", curl_easy_strerror(result));
        return -1;
    }

    printf("Response were succesfull\n");
    FILE *result_textfile = fopen("result.txt", "w");
    if (!result_textfile)
    {   
        fprintf(stderr, "Error opening result test file\n");
        return 1;
    }
    fprintf(result_textfile, "%s", response.data);
    fclose(result_textfile);

    free(response.data);
    response.data = NULL;
    curl_easy_cleanup(curl);
    return 0;
}

size_t Write_Data(void *data_src, size_t size, size_t nmemb, void *data_dst)
{
    size_t realsize = size * nmemb;
    HTTPResponseHandler *response = (HTTPResponseHandler*)data_dst;

    char *ptr_newchunk = realloc(response->data, response->size + realsize + 1);
    if (!ptr_newchunk)
    {
        return CURL_WRITEFUNC_ERROR;
    }

    response->data = ptr_newchunk;
    memcpy(&response->data[response->size], data_src, realsize);
    response->size += realsize;
    response->data[response->size] = '\0';

    return realsize;
}