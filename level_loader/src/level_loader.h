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

#endif /* LEVEL_LOADER_H */
