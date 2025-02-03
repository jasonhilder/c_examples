/* level_loader.h - v0.01 - Jason Hilder

Heavily inspired and designed from:
* https://github.com/nothings/stb/

This is a single header file that adds functions around: 
https://github.com/RandyGaul/cute_headers/blob/master/cute_tiled.h

To load a tiled level editor json file into a game leveldata struct.

To use this library, do this in *one* C or C++ file:
	#define LEVEL_LOADER_H_IMPLEMENTATION
	#include "level_loader.h"
*/

#ifndef LEVEL_LOADER_H
#define LEVEL_LOADER_H

#include "libs/cute_tiled.h"

enum CollisionType {
	RECTANGLE,
	CIRCLE,
	POINT,
	POLYGON
};

typedef struct {
	int type;
	float width;
	float height;
	float x;
	float y;
} CollisionObject;

typedef struct {
	// Map data is a map object from cute_tiled.h
	// Json object with all data needed for level map. 
	cute_tiled_map_t* map_data;

	// Objects could be rectangles, circles, polys
	// These are created in Tiled as an Object layer.
	// Use them as you will for collisions etc
	CollisionObject* collision_objects;

	// Helper to loop over layers easier
	int layer_count;

	// Helper to loop over shapes easier
	int shape_count;
} LevelData;

// Requires an unintialized pointer to a LevelData struct.
// Will init the default values and load in the map_data
int load_level(LevelData* LevelData, char* map_file_path);

// Free's the map_data and the shapes array
void unload_level(LevelData level_data);

// -------------------------------------- 
// implementation Below
// -------------------------------------- 

#ifdef LEVEL_LOADER_H_IMPLEMENTATION

#define CUTE_TILED_NO_EXTERNAL_TILESET_WARNING
#define CUTE_TILED_IMPLEMENTATION
#include "libs/cute_tiled.h"

#include "level_loader.h"

#include <stdio.h>
#include "libs/stb_ds.h"

int load_level(LevelData* level_data, char* map_file_path) {
	level_data->layer_count = 0;
	level_data->shape_count = 0;
	level_data->collision_objects = NULL;

	// Loading level data into a struct
	// -------------------------------------------------------

	cute_tiled_map_t *map = cute_tiled_load_map_from_file(map_file_path, 0);

	if(!map) {
		printf("File open map file");
		return 1;
	}

	cute_tiled_layer_t* layer = map->layers;
	while (layer)
	{
		cute_tiled_object_t* objects = layer->objects;
		int* data = layer->data;
		if(data != NULL) {
			level_data->layer_count++;
		}

		if(objects != NULL) {
			while(objects) {
				// TODO: Need to figure out storing polygon.

				CollisionObject co = {
					.width = objects->width,
					.height = objects->height,
					.x = objects->x,
					.y = objects->y
				};

				if(objects->ellipse) {
					co.type = CIRCLE;
				} else if(objects->point) {
					co.type = POINT;
				} else {
					co.type = RECTANGLE;
				}
				
				arrput(level_data->collision_objects, co);

				objects = objects->next;
				level_data->shape_count++;
			}
		}

		layer = layer->next;
	}

	level_data->map_data = map;

	return 0;
}

void unload_level(LevelData level_data) {
	arrfree(level_data.collision_objects);
	cute_tiled_free_map(level_data.map_data);
}

#endif /* LEVEL_LOADER_H_IMPLEMENTATION */
#endif /* LEVEL_LOADER_H */

