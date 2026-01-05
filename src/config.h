#ifndef CONFIG_H
#define CONFIG_H

#include "types.h"

/*

[package]
name = "Name"
description = "Description"
author = "Author"
version = "0.1"

[directories]
src = "src"
build = "build"

*/

typedef struct {
	String file_path;
} config;

config config_load(String file_path);

#endif // !CONFIG_H
