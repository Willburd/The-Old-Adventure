#include <stdio.h>
#include "assets.h"
#include "models.h"
#include "tools.h"

// Hashmap for model info
int meshdata_compare(const void* a, const void* b, void* udata) {
	const MeshInfo* ua = a;
	const MeshInfo* ub = b;
	return strcmp(ua->mesh_name, ub->mesh_name);
}

uint64_t meshdata_hash(const void* item, uint64_t seed0, uint64_t seed1) {
	const MeshInfo* mesh_inf = item;
	return hashmap_sip(mesh_inf->mesh_name, strlen(mesh_inf->mesh_name), seed0, seed1);
}

void meshdata_free(void* item) {
	MeshInfo* mesh_inf = item;
	RELEASE(mesh_inf->mesh_name); // Allocated string
	free(mesh_inf->resource_ptr);
}

// Half a MB of space
#define MAX_JSON_CHARS 524288
// Extracts json from .glb file 
cJSON* ParseGLTFModel(char* path)
{
	// Load material define file
	FILE* fptr = fopen(path, "r");
	if (fptr == NULL) {
		printf("Asset: failed to open model file: %s\n", path);
		return NULL;
	}
	char* cur_line = malloc(sizeof(char) * MAX_JSON_CHARS);
	char* final_data = malloc(sizeof(char) * MAX_JSON_CHARS);

	unsigned int depth = 0;
	unsigned int read_index = 0;
	unsigned int write_index = 0;
	while (fgets(cur_line, MAX_JSON_CHARS, fptr)) {
		while (read_index < MAX_JSON_CHARS)
		{
			char current = cur_line[read_index++];
			if (current == '{')
				depth++;
			if (current == '}')
				depth--;
			if (depth < 1)
			{
				if (write_index > 0) // Reached the end
				{
					final_data[write_index++] = '}';
					final_data[write_index++] = '\0';
					break;
				}
				continue;
			}
			final_data[write_index++] = current;
		}
		// Next line
		read_index = 0;
	}
	fclose(fptr);
	free(cur_line);
	cJSON* parsed_data = cJSON_Parse(final_data);
	free(final_data);
	return parsed_data;
}
#undef MAX_JSON_CHARS

int GetMeshIndex(struct hashmap* mesh_data, char* mesh_name)
{
	const MeshInfo* mesh_inf = hashmap_get(mesh_data, &(const MeshInfo){.mesh_name = mesh_name });
	if (!mesh_inf)
		return 0;
	return mesh_inf->mesh_index;
}