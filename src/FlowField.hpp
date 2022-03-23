#ifndef FlowField_H
#define FlowField_H

#include <Node2D.hpp>
#include <TileMap.hpp>
#include <Godot.hpp>
#include <vector>

class FlowField : public godot::Node2D {
	GODOT_CLASS(FlowField, godot::Node2D)
	std::vector<std::vector<int>> distance_map;
	std::vector<std::vector<int>> cost_map;
	std::vector<std::vector<std::vector<int>>> direction_map;
	std::vector<std::vector<bool>> cells_to_visit;
	const std::vector<std::vector<int>> four_directions = {{1,0}, {-1,0}, {0,1}, {0,-1}};
	const std::vector<std::vector<int>> four_diagonals = {{1,1}, {1,-1}, {-1,1}, {-1,-1}};
	const int wall_cost = 255;
	const int max_distance = 65535;
	int max_cost_of_a_tile = 2;
	int offset_x = 0;
	int offset_y = 0;

public:
	void _init() {}
	godot::Array get_distance_map();
	godot::Array get_direction_map();
	godot::Vector2 get_cell_direction(godot::Vector2);
	int get_cell_distance(godot::Vector2);
	void create_flow_field(godot::Array start_cells, godot::Array target_cells, bool diagonals_allowed, bool diagonals_need_neighbors);
	static void _register_methods();
	void set_play_field(godot::TileMap*, godot::Dictionary);
	

private:
	bool is_in_play_field(int[]);
};




#endif // FlowField_H
