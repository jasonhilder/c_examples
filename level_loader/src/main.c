#include <stdio.h>
#include <stdlib.h>

#define CUTE_TILED_NO_EXTERNAL_TILESET_WARNING
#define CUTE_TILED_IMPLEMENTATION
#include "libs/cute_tiled.h"

#define STB_DS_IMPLEMENTATION
#include "libs/stb_ds.h"

#include "raylib.h"

typedef struct {
	char Width;
	char Height;
	int **Layers;
	int LayerCount;
	Rectangle *Objects;	
} Level;

int main(void) {

	Level level_data;
	level_data.Layers = NULL;
	level_data.LayerCount = 0;

	// Loading level data into a struct
	// -------------------------------------------------------

	cute_tiled_map_t *map = cute_tiled_load_map_from_file("./assets/map_data.json", 0);

	if(!map) {
		printf("File open map file");
		return EXIT_FAILURE;
	}

	level_data.Height = map->height;
	level_data.Width = map->width;

	cute_tiled_layer_t* layer = map->layers;
	while (layer)
	{
		int* data = layer->data;
		int data_count = layer->data_count;
		if(data != NULL) {
			
			// create new array dimension
 			int **new_layers = (int**)realloc(level_data.Layers, (level_data.LayerCount + 1) * sizeof(int*));
            if (new_layers == NULL) {
                printf("Memory reallocation failed for rows.\n");
                return EXIT_FAILURE;
            }
			level_data.Layers = new_layers;

			// Allocate memory for the new layer and copy the data
			level_data.Layers[level_data.LayerCount] = (int*)malloc(data_count * sizeof(int));
			if (level_data.Layers[level_data.LayerCount] == NULL) {
				printf("Memory allocation failed for new layer.\n");
                return EXIT_FAILURE;
			}

			// Add data to the new dimension created
			for(int i = 0; i < data_count; i++) {
 				level_data.Layers[level_data.LayerCount][i] = data[i];
			}

			// Increment the layer count after adding the new layer
			level_data.LayerCount++;
		}

		/*
		cute_tiled_object_t* objects = layer->objects;
		if(objects != NULL) {
			printf("object layer\n");
			// ignore this section of code
		}
		*/

		layer = layer->next;
	}

	// No longer need the map data
	cute_tiled_free_map(map);

	// Finished Loading level data 
	// -------------------------------------------------------

	
	// Raylib test
	// -------------------------------------------------------

	// LoadImage tiles

	Color bg = {0x18, 0x18, 0x18, 0xff};

	InitWindow(1920, 1080, "WHAT THE FUCK");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(250);
	
	// map_tiles texture
	// UnloadImage
	int tile_size = 512;

	bool debug = false;
	
	while(!WindowShouldClose()) {
		BeginDrawing();
		
		ClearBackground(bg);

		EndDrawing();
	}

	CloseWindow();

	// Raylib test end
	// -------------------------------------------------------


	// Clean up 
	// -------------------------------------------------------
    for (int i = 0; i < level_data.LayerCount; i++) {
        free(level_data.Layers[i]);
    }
    free(level_data.Layers);

	// -------------------------------------------------------

    return EXIT_SUCCESS;
}

