#ifndef __TEXT_LOADING_HEADER__
#define __TEXT_LOADING_HEADER__

#include <stdint.h>

typedef struct {
    void* resource_ptr;
    char* key;
    char* data;
} TextEntry;

#define MAX_TEXT_ENTRIES 4096
#define MAX_TEXT_ENTRY_LENGTH 4096

void TextHashmapCreate();
void TextHashmapDestroy();
void TextHashmapClear();

void LoadBuiltinText();
void LoadTextData(char* text_id);
void DumpTextData();
char* GetText(char* string_id);

#endif