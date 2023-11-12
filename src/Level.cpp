#include "Level.h"
#include <ResourceLoader.hpp>
Level* Level::singleton = nullptr;



void Level::_register_methods()
{
	register_method("_ready", &Level::_ready);
	register_method("_process", &Level::_process);
	register_method("_physics_process", &Level::_physics_process);
	register_method("pull", &Level::pull);

	register_property("num_subs", &Level::num_subs, 0);
	register_property("num_mines", &Level::num_mines, 0);
	register_property("minefield_width", &Level::minefield_width, 0);
	register_property("mine_texture", &Level::mine_texture, Ref<Texture>());
	register_property("player_prefab", &Level::player_prefab, Ref<PackedScene>());
	register_property("size", &Level::size, Vector2());


}
Level* Level::get_singleton() {
	return singleton;

}
void Level::_init()
{
	singleton = this;
}

void Level::_ready()
{
	ResourceLoader* loader = ResourceLoader::get_singleton();
	Ref<PackedScene> player_prefab = loader->load("res://player.tscn");
	player = static_cast<Node2D*>(player_prefab->instance());
	tilemap = (TileMap*)get_node("TileMap");
	astarr = (AStarr*)get_node("Astarr");

	add_child(player);
	Godot::print("Hello World");

	initialize_tilemap(size);

}

void Level::_process(float delta)
{
}
void Level::_physics_process(float delta)
{
}

void Level::pull(Variant from, float magnitude)
{
}

void Level::initialize_tilemap(Vector2 size)
{
	for (int i = 0; i < size.x; i++)
	{
		for (int j = 0; j < size.y; j++)
		{
			tilemap->set_cellv(Vector2(i, j),0);
		}
	}
	//astarr->setup(witdh, height);

}

