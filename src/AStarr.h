#pragma once
#include "NativeLib.h"
#include <Node.hpp>
#include <vector>

#include <Input.hpp>
#include <InputEventMouseMotion.hpp>
#include <InputEventMouseButton.hpp>
#include <TileMap.hpp>
#include <unordered_map>

#include <Line2D.hpp>

struct cell {
	bool bObstacle = false;
	bool bVisited = false;
	float fGlobalGoal;
	float fLocalGoal;
	int x;
	int y;
	vector<cell*> vecNeighbours;
	cell* parent;
	vector< Line2D*> lines;

};
class AStarr: public CanvasItem
{

	GODOT_CLASS(AStarr, CanvasItem);
	Vector2 mousepos;
	cell* cells;

	TileMap* tilemap;
	cell* nodeStart;
	cell* nodeEnd;
	vector< Line2D*> lines;


public:
	
	static void _register_methods();
	void setup();
	void solve_astar();
	void _redraw_tilemap();
	void _ready();
	void _init();
	void _process(float delta);

};

