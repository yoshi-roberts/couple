#include "types.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

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

Array _array_new(void *items, usize capacity, usize item_size) {
	
	Array array = {0};

	array.length = capacity;
	array.capacity = capacity;
	array.item_size = item_size;
	array.items = items;

	return array;
}

Array _array_new_empty(Arena *arena, usize capacity, usize item_size) {
	
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

void* _array_get(Array *array, usize index) {

	if (index >= 0 && index < array->length) {
			return (i8*)array->items + index * array->item_size;
	}

	return NULL;
}

String string_new(char *str) {

	String string = {
		.chars = str,
		.len = _string_len(str) 
	};

	return string;
}

i32 _string_len(char *chars) {
	i32 len = 0;

	for(int i = 0; chars[i] != '\0'; i++) {
		len++;	
	}

	return len;
}
