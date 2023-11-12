#include "AStarr.h"
#include "Level.h"

void AStarr::_register_methods()
{
	register_method("_ready", &AStarr::_ready);
	register_method("_redraw_tilemap", &AStarr::_redraw_tilemap);
	register_method("_process", &AStarr::_process);
	register_method("_init", &AStarr::_init);

	register_signal<AStarr>((char*)"updated_tilemap");


}


void AStarr::_process(float delta)
{
	Input* input = Input::get_singleton();
	ERR_FAIL_NULL(input);
	int nNodeSize = 50;
	int nNodeBorder = 2;
	Vector2 size = Level::get_singleton()->size;
	if (input->is_action_just_pressed("ui_click"))
	{
		tilemap = (TileMap*)(get_parent()->get_node("TileMap"));
		ERR_FAIL_NULL(tilemap);

		Vector2 pos = tilemap->world_to_map(get_global_mouse_position());
		int nSelectedNodeX = pos.x;
		int nSelectedNodeY = pos.y;//joy_btn->get_global_position().y / nNodeSize;
		if (cells != nullptr) {
			if (input->is_action_pressed("shift")) {
			
				nodeStart = &cells[nSelectedNodeY * int(size.x) + nSelectedNodeX];
			}
			else 	if (input->is_action_pressed("control")) {
				nodeEnd = &cells[nSelectedNodeY * int(size.x) + nSelectedNodeX];

			}

			else
				cells[nSelectedNodeY * int(size.x) + nSelectedNodeX].bObstacle = !cells[nSelectedNodeY * int(size.x) + nSelectedNodeX].bObstacle;
		}


		solve_astar();

		_redraw_tilemap();
		emit_signal("updated_tilemap");
	}


}




void AStarr::_redraw_tilemap() {

	Vector2 size = Level::get_singleton()->size;


	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			tilemap->set_cellv(Vector2(x, y), cells[y *int( size.x) + x].bObstacle);
			if (cells[y * int(size.x) + x].bVisited)
				tilemap->set_cellv(Vector2(x, y), 4);
			if(&cells[y * int(size.x) + x]==nodeStart)
				tilemap->set_cellv(Vector2(x, y), 2);
			if (&cells[y * int(size.x) + x] == nodeEnd)
				tilemap->set_cellv(Vector2(x, y), 3);
			
		}
	}


	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			int line_number = 0;
			for (auto n : cells[y * int(size.x) + x].vecNeighbours)
			{
				Line2D* line = cells[y * int(size.x) + x].lines[line_number];
				
				line->clear_points();
				Vector2 point_global_position_0 = tilemap->map_to_world(Vector2(cells[y * int(size.x) + x].x, cells[y * int(size.x) + x].y));
				Vector2 point_global_position_1 = tilemap->map_to_world(Vector2(n->x, n->y));
				
			
				line->add_point(point_global_position_0);
				line->add_point(point_global_position_1);

				line_number++;

			}
		}
	}

	for (auto line : lines) {
		line->queue_free();
	}
	lines.clear();


	if (nodeEnd != nullptr) {
		cell* p = nodeEnd;
		while (p->parent != nullptr) {
			Line2D* line = Line2D::_new();
			line->set_global_position(Vector2(25, 25));
			line->set_default_color(Color(0, 1, 0, 1));
			line->set_width(5);
			add_child(line);

			Vector2 point_global_position_0 = tilemap->map_to_world(Vector2(p->x, p->y));
			Vector2 point_global_position_1 = tilemap->map_to_world(Vector2(p->parent->x, p->parent->y));
			lines.push_back(line);

			line->add_point(point_global_position_0);
			line->add_point(point_global_position_1);

			p = p->parent;
		}
	}


}

void AStarr::setup()
{
	Vector2 size = Level::get_singleton()->size;

	cells = new cell[size.x * size.y];


	ERR_FAIL_NULL(tilemap);


	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			cell _cell;
			_cell.x = x;
			_cell.y = y;
			_cell.bObstacle = false;
			_cell.parent = nullptr;
			_cell.bVisited = false;


			cells[y * int(size.x) + x] = _cell;
		
		}
	}
	//connect neighbours

	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			if (y>0)
				cells[y * int(size.x) + x].vecNeighbours.push_back(&cells[(y - 1) * int(size.x) + (x + 0)]);
			if (y < size.y-1)
				cells[y * int(size.x) + x].vecNeighbours.push_back(&cells[(y + 1) * int(size.x) + (x + 0)]);
			if (x > 0)
				cells[y * int(size.x) + x].vecNeighbours.push_back(&cells[(y + 0) * int(size.x) + (x - 1)]);
			if (x < size.x - 1)
				cells[y * int(size.x) + x].vecNeighbours.push_back(&cells[(y +0) * int(size.x) + (x + 1)]);
			
		}
	}


	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
			for (auto n : cells[y * int(size.x) + x].vecNeighbours)
			{

				Line2D* line = Line2D::_new();
				line->set_global_position(Vector2(25, 25));
				line->set_default_color(Color(1, 1, 1, 1));
				line->set_width(5);
				add_child(line);
				cells[y * int(size.x) + x].lines.push_back( line);
			}

		}
	}

	nodeStart = &cells[int(size.y / 2) * int(size.x) + 1];
	nodeEnd = &cells[int(size.y / 2) * int(size.x) + int(size.x) - 2];


	solve_astar();
	_redraw_tilemap();
}





void AStarr::solve_astar() {
	Vector2 size = Level::get_singleton()->size;



	for (int x = 0; x < size.x; x++)
	{
		for (int y = 0; y < size.y; y++)
		{
		
		
			cells[y * int(size.x) + x].parent = nullptr;
			cells[y * int(size.x) + x].fGlobalGoal = INFINITY;
			cells[y * int(size.x) + x].fLocalGoal = INFINITY;
			cells[y * int(size.x) + x].bVisited = false;
		}
	}
	auto distance = [](cell* a, cell* b) {
		return sqrtf((a->x - b->x) * (a->x - b->x) + (a->y - b->y) * (a->y - b->y));
		};
	auto heuristic = [distance](cell* a, cell* b) {
		return distance(a, b);
		};

	//setup starting conditions
	cell* current = nodeStart;
	nodeStart->fLocalGoal = 0.0f;
	nodeStart->fGlobalGoal = heuristic(nodeStart,nodeEnd);

	list<cell*> listNotTestedNodes;
	listNotTestedNodes.push_back(nodeStart);

	while (!listNotTestedNodes.empty() && current != nodeEnd) {
		listNotTestedNodes.sort([](const cell* lhs, const cell* rhs) {
			return lhs->fGlobalGoal< rhs->fGlobalGoal;
			});

		while (!listNotTestedNodes.empty() && listNotTestedNodes.front()->bVisited)
			listNotTestedNodes.pop_front();
		if (listNotTestedNodes.empty())
			break;
		current = listNotTestedNodes.front();
		current->bVisited = true;

		for (auto nodeNeighbour : current->vecNeighbours) {
			if (!nodeNeighbour->bVisited && nodeNeighbour->bObstacle == 0)
				listNotTestedNodes.push_back(nodeNeighbour);

			float fPossiblyLowerGoal = current->fLocalGoal + distance(current, nodeNeighbour);



			if (fPossiblyLowerGoal < nodeNeighbour->fLocalGoal) {
				nodeNeighbour->parent = current;
				nodeNeighbour->fLocalGoal = fPossiblyLowerGoal;




				nodeNeighbour->fGlobalGoal = nodeNeighbour->fLocalGoal + heuristic(nodeNeighbour, nodeEnd);
			}
		}

	}





}

void AStarr::_ready()
{
	tilemap = (TileMap*)(get_parent()->get_node("TileMap"));
	//Godot::print(String::num_int64(my_array.size()));
	
	setup();
}

void AStarr::_init()
{
}

