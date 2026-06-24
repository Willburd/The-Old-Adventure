#ifndef __TEXT_LOADING_HEADER__
#define __TEXT_LOADING_HEADER__

#include <stdint.h>

#define MAX_TEXT_ENTRIES 4096
#define MAX_KEY_LEN 128
#define MAX_ENTRY_CHARS 2048

#define TEXT_ID(x) "ID_"#x"_"

struct hashmap* loaded_text;

int text_compare(const void* a, const void* b, void* udata);
uint64_t text_hash(const void* item, uint64_t seed0, uint64_t seed1);
void text_free(void* item);

void LoadBuiltinText();
void LoadTextData(char* text_file_path);
void DumpTextData();
char* GetText(char* string_id);

#endif