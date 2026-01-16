#include "deps.h"
#include "helper.h"

static literal url_postfix[] = {
	"11.5/love-11.5-win64.zip",
	"11.5/love-11.5-x86_64.AppImage"
};

Dependency plat_to_dep(String *plat) {

	if (string_cmp_lit(plat, "win64")) {
		return WINDOWS_LATEST;
	} else if (string_cmp_lit(plat, "linux")) {
		return LINUX_LATEST;
	}

	return INVALID_DEP;
}

bool dep_check(Arena *arena, String *plat) {

	String full_path = cat(arena, "build/", lit(plat));

	if (!file_exists(lit(&full_path))) {
		printf("Missing dependency %s\n", lit(plat));
		return false;
	}

	return true;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int dep_get(Arena *arena, String *plat) {

	Dependency dep = plat_to_dep(plat);

	String url = cat(arena, DEP_BASE_URL, url_postfix[dep]);
	printf("DEP URL: %s\n", lit(&url));

	String dep_path = cat(arena, "build/", lit(plat));
	printf("DEP PATH: %s\n", lit(&dep_path));

	CURL *curl;
    FILE *fp;
    CURLcode res;

    curl = curl_easy_init();

    if (curl) {

        fp = fopen(lit(&dep_path),"wb");

        curl_easy_setopt(curl, CURLOPT_URL, lit(&url));
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

        res = curl_easy_perform(curl);

		if (res != CURLE_OK) {
			fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
		}

        curl_easy_cleanup(curl);
        fclose(fp);
    }

	return 0;
}
