#include <stdio.h>
#include <stdlib.h>

#define CUTE_TILED_NO_EXTERNAL_TILESET_WARNING
#define CUTE_TILED_IMPLEMENTATION
#include "libs/cute_tiled.h"

#define STB_DS_IMPLEMENTATION
#include "libs/stb_ds.h"

#include "raylib.h"

#include "level_loader.h"

typedef struct {
	LevelData level_data;
} Game;

int main(void) {

	Game game;

	// Loading level data into a struct
	// -------------------------------------------------------

	int map_err = load_level(&game.level_data, "./src/assets/map_data.json");
	if(map_err > 0) {
		printf("Failed to load map");
		return EXIT_FAILURE;
	}
	
	// Finished Loading level data 
	// -------------------------------------------------------

	// EXAMPLE!! Using Raylib 
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
	bool debug_mode = false;
	float scale_factor  = 0.250;

	cute_tiled_layer_t* layers =  game.level_data.map_data->layers;
	
	while(!WindowShouldClose()) {

		if (IsKeyPressed(KEY_TAB)) {
			debug_mode = !debug_mode;
		}

		BeginDrawing();

		ClearBackground(bg);

		// Draw tiles from map data and sprite sheet
		// -----------------------------------------
		int i = 0;
		while(true) {
			if(i == game.level_data.layer_count) break;

			int* tile_data = layers[i].data;
			int data_count = layers[i].data_count;

			if(tile_data != NULL) {
				for (int index = 0; index < data_count; index++) {
					int tile_index = tile_data[index] - 1;

					if (tile_index == -1) {
						continue;
					}

					// Calculate the grid position (x, y)
					int x = index % game.level_data.map_data->width;
					int y = index / game.level_data.map_data->width;

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

		// -----------------------------------------

		// Debug mode
		// -----------------------------------------
		if(debug_mode) {
			for(int j = 0; j <= game.level_data.shape_count - 1; j++) {
				int x = game.level_data.collision_objects[j].x * scale_factor;
				int y = game.level_data.collision_objects[j].y * scale_factor;

				int w = game.level_data.collision_objects[j].width * scale_factor;
				int h = game.level_data.collision_objects[j].height * scale_factor;
				
				if(game.level_data.collision_objects[j].type == RECTANGLE) {
					// Create a rectangle for the collision box (scaled position and size)
					Rectangle bounds_rec = {.x = (float)x, .y = (float)y, .width = (float)w, .height = (float)h};

					// Draw the collision box with a green outline
					DrawRectangleLinesEx(bounds_rec, 5.0, RED);
				}
			}
		}
		// -----------------------------------------

		EndDrawing();
	}

	UnloadTexture(map_tiles);
	CloseWindow();

	// EXAMPLE!! Using Raylib END
	// -------------------------------------------------------

	unload_level(game.level_data);
	// -------------------------------------------------------
    return EXIT_SUCCESS;
}



