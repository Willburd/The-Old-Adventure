#ifndef __TEXT_LOADING_HEADER__
#define __TEXT_LOADING_HEADER__

#include <stdint.h>

typedef struct {
    char* key;
    char* data;
} TextEntry;

#define MAX_TEXT_ENTRIES 4096
#define MAX_TEXT_ENTRY_LENGTH 4096

struct hashmap* loaded_text;

int text_compare(const void* a, const void* b, void* udata);
uint64_t text_hash(const void* item, uint64_t seed0, uint64_t seed1);
void text_free(void* item);

void LoadBuiltinText();
void LoadTextData(char* text_id);
void DumpTextData();
char* GetText(char* string_id);

#endif