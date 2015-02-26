#include <stdio.h>
#include <curl/curl.h>

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t w = fwrite(ptr, size, nmemb, stream);
    return w;
}

int main(void) {
    CURL *curl;
    FILE *fp;
    char *url = "http://download.oracle.com/otn-pub/java/jdk/8u31-b13/jre-8u31-windows-i586.exe";
    char outfilename[FILENAME_MAX] = "jre.exe";

    struct curl_slist *cookies = curl_slist_append(NULL, "Cookie: oraclelicense=accept-securebackup-cookie");
    curl = curl_easy_init();

    if (curl) {
        fp = fopen(outfilename, "wb");
        curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, cookies);
        curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);

        system("jre.exe");
    }
    return 0;
}
