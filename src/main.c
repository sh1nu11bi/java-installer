#include <windows.h>
#include <tchar.h>
#include <stdio.h>
#include <curl/curl.h>

typedef BOOL (WINAPI *LPFN_ISWOW64PROCESS)(HANDLE, PBOOL);

LPFN_ISWOW64PROCESS fnIsWow64Process;

BOOL IsWow64() {
	BOOL bIsWow64 = FALSE;

	fnIsWow64Process = (LPFN_ISWOW64PROCESS) GetProcAddress(GetModuleHandle(TEXT("kernel32")), "IsWow64Process");

	if (NULL != fnIsWow64Process) {
		if (!fnIsWow64Process(GetCurrentProcess(), &bIsWow64)) {
			return FALSE;
		}
	}

	return bIsWow64;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
	size_t w = fwrite(ptr, size, nmemb, stream);
	return w;
}

void print(char* c) {
	printf(c);
	fflush(stdout);
}

int main(void) {
	CURL *curl;
	FILE *fp;
	char *url;
	char outfilename[FILENAME_MAX] = "jre.exe";

	if (IsWow64()) {
		url = "http://download.oracle.com/otn-pub/java/jdk/8u31-b13/jre-8u31-windows-x64.exe";
	} else {
		url = "http://download.oracle.com/otn-pub/java/jdk/8u31-b13/jre-8u31-windows-i586.exe";
	}

	print("Attempting download: ");
	print(url);
	print("\n");

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

		char *file = "jre.exe";

		print("Executing ");
		print(file);

		char *cmd = malloc(strlen(file) + strlen(" /s") + 1);
		strcpy(cmd, file);
		strcat(cmd, " /s");

		system(cmd);
	}
	return 0;
}
