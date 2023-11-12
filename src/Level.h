#pragma once
#include "NativeLib.h"
#include "AStarr.h"
#include <Node2D.hpp>
#include <Texture.hpp>
#include <PackedScene.hpp>
#include <TileMap.hpp>

class Level:public Node2D
{
	GODOT_CLASS(Level, Node2D);

	int num_subs = 0;
	int num_mines = 0;
	int minefield_width = 0;
	Ref<Texture> mine_texture;
	Ref<PackedScene> player_prefab;
	Node2D* player;
	AStarr* astarr;
	const godot_string* message;
	static Level* singleton ;
public:
	TileMap* tilemap;
	Vector2 size;

	static void _register_methods();
	void _init();

	void _ready();
	void _process(float delta);
	void _physics_process(float delta);
	void pull(Variant from, float magnitude);
	void initialize_tilemap(Vector2 size);
	static Level* get_singleton();

};

