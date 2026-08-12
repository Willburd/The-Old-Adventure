#include <stdio.h>
#include <string.h>
#include "assets.h"
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
    free(entry->resource_ptr);
}

static int AddEntry(char* string_id, char* string_data)
{
    MALLOC(TextEntry, entry, ERR_NOALLOC);
    CHAR_STR_COPY(entry->key, string_id, ERR_NOALLOC);
    CHAR_STR_COPY(entry->data, string_data, ERR_NOALLOC);
    entry->resource_ptr = entry;
    hashmap_set(loaded_text, entry);
    return 0;
}

void LoadBuiltinText()
{
	AddEntry("debug", "The Old Adventure");
    AddEntry("test_arabic", "أبجدية عربية");
    AddEntry("test_russian", "Кириллица");
}

void LoadTextData(char* text_id)
{
    // Solve the language folder
    char* file_path;
    switch (current_game_language)
    {
    default:
    case lang_EN:
        file_path = TextFormat("%s/EN/%s.txt", ASSET_TEXT, text_id);
        break;
    }

    // Get the entire file as a single long string
    FILE* fptr = fopen(file_path, "r");
    char cur_line[256];
    if (fptr == NULL) {
        printf("Asset: failed to open text data definition: %s\n", file_path);
        return;
    }
    unsigned int index = 0;
    char final_string[MAX_TEXT_ENTRY_LENGTH] = { '\0'};
    while (fgets(cur_line, 256, fptr)) {
        for (int i = 0; i < strlen(cur_line); i++)
        {
            final_string[index++] = cur_line[i];
        }
    }
    fclose(fptr);

    // Append the string to the database of text entries
    AddEntry(text_id, final_string);
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