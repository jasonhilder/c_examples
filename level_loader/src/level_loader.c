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

