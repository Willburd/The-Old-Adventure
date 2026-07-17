#include <stdio.h>
#include "models.h"

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