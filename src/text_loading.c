#include <stdio.h>
#include <string.h>
#include "text_loading.h"
#include "hashmap.h"
#include "tools.h"

typedef struct {
    char* key;
    char* value;
} TextEntry;

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
    RELEASE(entry->value);
}

static void AddEntry(char* string_id, char* string_data)
{
    MALLOC(TextEntry, entry, NULL);
    CHAR_STR_COPY(entry->key, string_id, NULL);
    CHAR_STR_COPY(entry->value, string_data, NULL);
    hashmap_set(loaded_text, entry);
    printf("Added text: %s, %s\n", entry->key, entry->value);
}

void LoadBuiltinText()
{
	AddEntry(TEXT_ID(empty), " ");
	AddEntry(TEXT_ID(test), "Test");
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
        printf("%s: %s\n", text->key, text->value);
    }
}

char* GetText(char* string_id)
{
    TextEntry* entry = hashmap_get(loaded_text, &(const TextEntry){.key = string_id });
    if (entry == NULL || entry->value == NULL)
        return "?";
    return entry->value;
}