#include "helper.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// -- Arena --

void* arena_alloc(Arena *arena, usize size) {

	ArenaBlock *it, *prev;
	usize old = size;

	size = ARENA_ALIGN_UP(size, ARENA_MAX_ALIGN);
	it = prev = arena->current;

	// find the first block with enough space
	assert(it ? it->end >= it->ptr : 1);
	while (it && size > (usize)(it->end - it->ptr)) {
		prev = it, it = it->next;
	}

	if (it) {
		arena->current = it;
		it->ptr += size;
	} else {
		// Needs to be allocated
		usize n = sizeof *it + size + ARENA_BLOCK_SIZE;
		usize an = ARENA_ALIGN_UP(n, ARENA_MAX_ALIGN);
		it = malloc(an);

		if (arena->current) {
			arena->current = prev->next = it;
		} else  {
			arena->current = arena->blocks = it;
		}

		it->next = 0;
		it->first = (u8*)it + sizeof *it + (an - n);
		it->ptr = it->first + size;
		it->end = (u8*)it + an;
	}

	return it->ptr - old;
}

void arena_dealloc(Arena *arena) {
	ArenaBlock *it = arena->blocks;

	while (it) {
		it->ptr = it->first;
		it = it->next;
	}

	arena->current = arena->blocks;
}

void arena_free(Arena *arena) {

	ArenaBlock *it = arena->blocks;

	while (it) {
		ArenaBlock *b = it;
		it = it->next;
		free(b);
	}

	arena->current = arena->blocks = 0;
}

// -- Array --

Array _array_new(void *items, usize capacity, usize item_size) {
	
	Array array = {0};

	array.length = capacity;
	array.capacity = capacity;
	array.item_size = item_size;
	array.items = items;

	return array;
}

Array _array_make(Arena *arena, usize capacity, usize item_size) {
	
	Array array = {0};

	array.length = 0;
	array.capacity = capacity;
	array.item_size = item_size;
	array.items = arena_alloc(arena, capacity * item_size);

	return array;
}

void _array_push(Array *array, void *item) {

	if (array->length < array->capacity) {

		void* target = (i8*)array->items + array->length * array->item_size;
		memcpy(target, item, array->item_size);
		array->length++;
	}
}

void _array_set(Array *array, void *item, usize index) {

	if (array->length < array->capacity) {

		void* target = (i8*)array->items + index * array->item_size;
		memcpy(target, item, array->item_size);
		array->length++;
	}
}

void* _array_get(Array *array, usize index) {

	if (index >= 0 && index < array->length) {
			return (i8*)array->items + index * array->item_size;
	}

	return NULL;
}

// -- String --

String string_new(literal string_lit) {

	usize src_len = _string_len(string_lit);

	String string = {
		.chars = (char*)string_lit,
		.length = src_len,
		.capacity = src_len
	};

	return string;
}

String string_make(Arena *arena, usize capacity) {

	String string = {
		.chars = arena_alloc(arena, sizeof(char) * capacity),
		.length = 0,
		.capacity = capacity
	};

	return string;
}

String string_copy(Arena *arena, String *original) {

	String copy = {
		.chars = arena_alloc(arena, sizeof(char) * original->length),
		.length = original->length
	};

	for (int i = 0; i <= original->length; ++i) {
		copy.chars[i] = original->chars[i];
	}
	copy.chars[copy.length] = '\0';

	return copy;
}

void string_write(String *string, char *string_lit) {

	usize len = _string_len(string_lit);
	
	if (len > string->capacity) {
		error("Literal too long");
	}

	string->length = len;

	for (int i = 0; i <= string->length; ++i) {
		string->chars[i] = string_lit[i];
	}
}

String string_cat(Arena *arena, Array literals) {

	// NOTE: Probably a better way to do this.

	usize total_len = 0;

	for (usize i = 0; i < len(&literals); i++) {

		literal l = array_get(literal, &literals, i);
		total_len += _string_len(l);
	}

	String new = {
		.chars = arena_alloc(arena, sizeof(char) * (total_len)),
		.length = total_len,
	};

	for (usize i = 0; i < len(&literals); i++) {

		literal l = array_get(literal, &literals, i);
		strcat(new.chars, l);
	}

	return new;
}

i8 string_get(String *string, usize index) {

	if (index >= 0 && index < string->length) {
			return *(string->chars + index * sizeof(i8));
	}

	return '\0';
}

bool string_cmp(String *a, String *b) {
	if (strcmp(lit(a), lit(b)) == 0) {
		return true;
	}

	return false;
}

bool string_cmp_lit(String *a, literal b) {
	if (strcmp(lit(a), b) == 0) {
		return true;
	}

	return false;
}

usize _string_len(const char *chars) {
	usize len = 0;

	for(int i = 0; chars[i] != '\0'; i++) {
		len++;	
	}

	return len;
}

// -- File --

File file_read(Arena *arena, const char *file_path) {

	File file = {0};

	file.path = string_new((char*)file_path);
	file.fptr = fopen(file_path, "r");

	if (!file.fptr) {
		error("Could not open file.");
	}

	// Read contents
	
	fseek(file.fptr, 0, SEEK_END); 
	usize size = ftell(file.fptr);
	fseek(file.fptr, 0, SEEK_SET);

	file.contents = string_make(arena, size);
	fread(file.contents.chars, size, 1, file.fptr);
	
	fclose(file.fptr);

	return file;
}

File file_write(literal file_path, literal contents) {

	File file = {0};

	file.path = string_new((char*)file_path);
	file.fptr = fopen(file_path, "w");

	if (!file.fptr) {
		error("Could not open file.");
	}

	fprintf(file.fptr, "%s", contents);
	
	fclose(file.fptr);

	return file;
}

bool file_exists(literal file_path) {
	return access(file_path, F_OK) == 0;
}

int directory_make(const char *path) {
	return mkdir(path, 0755);
}

bool directory_exists(const char *path) {

	DIR *dir = opendir(path);

    if (dir) {
        closedir(dir);
        return true;
    }

	closedir(dir);
	return false;
}
