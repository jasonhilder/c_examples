#include <stdio.h>
#include <stdlib.h>

#define CUTE_TILED_NO_EXTERNAL_TILESET_WARNING
#define CUTE_TILED_IMPLEMENTATION
#include "libs/cute_tiled.h"

#define STB_DS_IMPLEMENTATION
#include "libs/stb_ds.h"

#include "raylib.h"

typedef struct {
	int LayerCount;
	int ObjectCount;
	Rectangle* CollisionObjects;
} Level;

int main(void) {

	Level level_data;
	level_data.LayerCount = 0;
	level_data.CollisionObjects = NULL;

	// Loading level data into a struct
	// -------------------------------------------------------

	cute_tiled_map_t *map = cute_tiled_load_map_from_file("./src/assets/map_data.json", 0);

	if(!map) {
		printf("File open map file");
		return EXIT_FAILURE;
	}

	cute_tiled_layer_t* layer = map->layers;
	while (layer)
	{
		cute_tiled_object_t* objects = layer->objects;
		int* data = layer->data;
		if(data != NULL) {
			// Increment the layer count after adding the new layer
			level_data.LayerCount++;
		}

		// TODO needs to be an array of Rectangles
		if(objects != NULL) {
			while(objects) {
				float w = objects->width;
				float h = objects->height;
				float x = objects->x;
				float y = objects->y;
				
				Rectangle r = {.height = h, .width = w, .x = x, .y = y};
				arrput(level_data.CollisionObjects, r);

				objects = objects->next;
				level_data.ObjectCount++;
			}
		}

		layer = layer->next;
	}

	// Finished Loading level data 
	// -------------------------------------------------------

	// Raylib test
	// -------------------------------------------------------

	// LoadImage tiles
	Image img = LoadImage("./src/assets/mossy_tiles.png");

	Color bg = {0x18, 0x18, 0x18, 0xff};

	InitWindow(3840, 2160, "WHAT THE FUCK");
	SetWindowState(FLAG_WINDOW_RESIZABLE);
	SetTargetFPS(250);

	Texture2D map_tiles = LoadTextureFromImage(img);
	UnloadImage(img);
	
	int tile_size = 512;
	bool debug = false;
	float scale_factor  = 0.250;

	cute_tiled_layer_t* layers = map->layers;
	
	while(!WindowShouldClose()) {

		if (IsKeyPressed(KEY_TAB)) {
			debug = !debug;
			printf("TAB key pressed, debug state: %s\n", debug ? "true" : "false");
		}

		BeginDrawing();

		ClearBackground(bg);

		int i = 0;
		while(true) {
			if(i == level_data.LayerCount) break;

			int* tile_data = layers[i].data;
			int data_count = layers[i].data_count;

			if(tile_data != NULL) {
				for (int index = 0; index < data_count; index++) {
					int tile_index = tile_data[index] - 1;

					if (tile_index == -1) {
						continue;
					}

					// Calculate the grid position (x, y)
					int x = index % map->width;
					int y = index / map->width;

					// Convert from grid position to pixel position
					x *= tile_size;
					y *= tile_size;

					// Get the source rectangle in the sprite sheet (tile index)
					int src_x = (tile_index % 7) * tile_size;
					int src_y = (tile_index / 7) * tile_size;

					// Define the source rectangle (tile region from the sprite sheet)
					Rectangle src_rect = { (float)src_x, (float)src_y, (float)tile_size, (float)tile_size };

					// Calculate scaled position and size
					float scaled_pos_x = (float)x * scale_factor;
					float scaled_pos_y = (float)y * scale_factor;
					float scaled_width = (float)tile_size * scale_factor;
					float scaled_height = (float)tile_size * scale_factor;

					// Define the destination rectangle (scaled position and size)
					Rectangle dest_rect = { scaled_pos_x, scaled_pos_y, scaled_width, scaled_height };

					// Draw the tile using the source and destination rectangles
					DrawTexturePro(map_tiles, src_rect, dest_rect, (Vector2){ 0, 0 }, 0.0f, WHITE);
				}
			}

			i++;
		}

		if(debug) {
			for(int j = 0; j <= level_data.ObjectCount - 1; j++) {
				int x = level_data.CollisionObjects[j].x * scale_factor;
				int y = level_data.CollisionObjects[j].y * scale_factor;

				int w = level_data.CollisionObjects[j].width * scale_factor;
				int h = level_data.CollisionObjects[j].height * scale_factor;

				// Create a rectangle for the collision box (scaled position and size)
				Rectangle bounds_rec = {.x = (float)x, .y = (float)y, .width = (float)w, .height = (float)h};

				// Draw the collision box with a green outline
				DrawRectangleLinesEx(bounds_rec, 5.0, RED);
			}
		}

		EndDrawing();
	}

	UnloadTexture(map_tiles);

	CloseWindow();

	// Raylib test end
	// -------------------------------------------------------


	// Clean up 
	// -------------------------------------------------------
	// No longer need the map data
	cute_tiled_free_map(map);
	arrfree(level_data.CollisionObjects);
	// -------------------------------------------------------

    return EXIT_SUCCESS;
}
