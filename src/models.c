#include <stdio.h>
#include "assets.h"
#include "models.h"
#include "tools.h"

// Extracts json from .glb file 
cJSON* ParseGLTFModel(char* path)
{
	// Load material define file
	FILE* fptr = fopen(path, "r");
	char cur_line[4096] = { '\0'};
	char final_data[4096] = { '\0' };
	if (fptr == NULL) {
		printf("Asset: failed to open model file: %s\n", path);
		return NULL;
	}

	unsigned int depth = 0;
	unsigned int read_index = 0;
	unsigned int write_index = 0;
	while (fgets(cur_line, 4096, fptr)) {
		while (read_index < 4096)
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
	return cJSON_Parse(final_data);
}

int GetMeshIndex(cJSON* model_json, char* mesh_name)
{
	cJSON* mesh_array = cJSON_GetObjectItem(model_json, "nodes");
	for (int i = 0; i < cJSON_GetArraySize(mesh_array); i++)
	{
		cJSON* mesh_entry = cJSON_GetArrayItem(mesh_array, i);
		
		if (STRMATCH(cJSON_GetObjectItem(mesh_entry, "name")->valuestring, mesh_name))
			return cJSON_GetObjectItem(mesh_entry, "mesh")->valueint;
	}
	printf("MESH FIND ERROR: Unable to locate %s, meshes are: \n", mesh_name);
	for (int i = 0; i < cJSON_GetArraySize(mesh_array); i++)
	{
		cJSON* mesh_entry = cJSON_GetArrayItem(mesh_array, i);
		printf("  -%s \n", cJSON_GetObjectItem(mesh_entry, "name")->valuestring);
	}
	return 0;
}