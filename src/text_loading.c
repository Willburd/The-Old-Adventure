#include <stdio.h>
#include <string.h>
#include "text_loading.h"
#include "hashmap.h"
#include "tools.h"

int text_compare(const void* a, const void* b, void* udata) {
    TextEntry* ua = a;
    TextEntry* ub = b;
    return strcmp(ua->key, ub->key);
}

uint64_t text_hash(const void* item, uint64_t seed0, uint64_t seed1) {
    const TextEntry* entry = item;
    return hashmap_sip(entry->key, strlen(entry->key), seed0, seed1);
}

void text_free(void* item) { 
    TextEntry* entry = item;
    RELEASE(entry->key);
    RELEASE(entry->data);
}

static void AddEntry(char* string_id, char* string_data)
{
    MALLOC(TextEntry, entry, NULL);
    CHAR_STR_COPY(entry->key, string_id, NULL);
    CHAR_STR_COPY(entry->data, string_data, NULL);
    hashmap_set(loaded_text, entry);
}

void LoadBuiltinText()
{
	AddEntry(TEXT_ID(debug), "The Old Adventure");
    AddEntry(TEXT_ID(test_arabic), "أبجدية عربية");
    AddEntry(TEXT_ID(test_russian), "Кириллица");
}

void LoadTextData(char* text_file_path)
{

}

void DumpTextData()
{
    size_t iter = 0;
    void* item;
    while (hashmap_iter(loaded_text, &iter, &item)) {
        TextEntry* text = item;
        printf("%s: %s\n", text->key, text->data);
    }
}

char* GetText(char* string_id)
{
    TextEntry* entry = hashmap_get(loaded_text, &(const TextEntry){.key = string_id });
    if (entry == NULL || entry->data == NULL)
        return "?";
    return entry->data;
}