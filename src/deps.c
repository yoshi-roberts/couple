#include "deps.h"
#include "helper.h"

static literal version_str[] = {
	"11.5"
};

static literal file_version_prefix[] = {
	"love-11.5-"
};

static literal file_name[] = {
	"win64",
	"x86_64"
};

static literal file_extension[] = {
	".zip",
	".AppImage"
};

DependencyPlatform str_to_plat(String *plat) {

	// Switch on the last char of the plat name, as it is unique.
	switch (string_get(plat, len(plat) - 1)) {
	
		case '4': return PLATFORM_WINDOWS; // win64
		case 'x': return PLATFORM_LINUX;   // linux
		default: return PLATFORM_INVALID;
	}

}

Dependency dep_init(Arena *arena, String *plat) {


	Dependency dep = {0};

	dep.version = VERSION_11_5;

	literal love_version = version_str[dep.version];
	literal version_prefix = file_version_prefix[dep.version];
	literal name = file_name[dep.version];
	literal ext = file_extension[dep.version];

	dep.platform = str_to_plat(plat);
	dep.love_version = str(love_version);
	dep.file_name = cat(arena, version_prefix, name);
	dep.url = cat(
		arena,
		DEP_BASE_URL,
		love_version,
		"/",
		version_prefix,
		name,
		ext
	);

	dep.full_path = cat(arena, "deps/", love_version, "/", version_prefix, name, ext);
	printf("DEP PATH: %s\n", lit(&dep.full_path));

	return dep;
}

bool dep_check(Arena *arena, Dependency *dep) {

	if (!file_exists(lit(&dep->full_path))) {
		printf("Missing dependency %s\n", lit(&dep->full_path));
		return false;
	}

	return true;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int dep_get(Arena *arena, Dependency *dep) {

	if(!directory_exists("deps")) {
		directory_make("deps");
	}

	String version_dir = cat(arena, "deps/", lit(&dep->love_version));

	if (!directory_exists(lit(&version_dir))) {
		directory_make(lit(&version_dir));
	}

	CURL *curl;
	FILE *fp;
	CURLcode res;

	curl = curl_easy_init();

	if (curl) {

		fp = fopen(lit(&dep->full_path),"wb");

		curl_easy_setopt(curl, CURLOPT_URL, lit(&dep->url));
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
