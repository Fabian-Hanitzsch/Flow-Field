#include "FlowField.hpp"




void FlowField::_register_methods() {
	godot::register_method("get_distance_map", &FlowField::get_distance_map);
	godot::register_method("set_play_field", &FlowField::set_play_field);
	godot::register_method("create_flow_field", &FlowField::create_flow_field);
	godot::register_method("get_direction_map", &FlowField::get_direction_map);
	godot::register_method("get_cell_direction", &FlowField::get_cell_direction);
	godot::register_method("get_cell_distance", &FlowField::get_cell_distance);
	

	

}



godot::Array FlowField::get_direction_map () {
	godot::Array ret;

	for (size_t i = 0; i < direction_map.size(); i++)
	{
		godot::Array row;
		for (size_t j = 0; j < direction_map[i].size(); j++)
		{
			godot::Vector2 cell;
			cell.x = direction_map[i][j][0];
			cell.y = direction_map[i][j][1];
			row.append(cell);
		}
		ret.append(row);
		
	}

	return ret;
}

godot::Array FlowField::get_distance_map () {
	godot::Array ret;
	for (size_t i = 0; i < distance_map.size(); i++)
	{
		godot::Array row;
		for (size_t j = 0; j < distance_map[i].size(); j++)
		{
			row.append(distance_map[i][j]);
		}
		ret.append(row);
		
	}

	return ret;
}


void FlowField::create_flow_field(godot::Array start_cells, godot::Array target_cells, bool diagonals_allowed, bool diagonals_need_neighbors) {
		for (size_t i = 0; i < distance_map.size(); i++)
	{
		for (size_t j = 0; j < distance_map[i].size(); j++)
		{
			distance_map[i][j] = max_distance;
		}
		
	}
	
	
	int current_cost_pointer = 0;
	std::vector<int> nr_of_cells(max_cost_of_a_tile, 0);
	std::vector<std::vector<std::vector<int>>> all_interesting_cells(max_cost_of_a_tile, std::vector<std::vector<int>>(10,std::vector<int>(2,0)));
	int interesting_cells_left = 0;
	
	for (size_t i = 0; i < start_cells.size(); i++)
	{
		godot::Vector2 cell = start_cells[i];
		cell.x += offset_x;
		cell.y += offset_y;

		int cell_as_array[2];
		cell_as_array[0] = cell.x;
		cell_as_array[1] = cell.y;

		if (!is_in_play_field(cell_as_array))
		{
			continue;
		}
		distance_map[cell.x][cell.y] = 0;
		std::vector<int> cell_as_vector = std::vector<int>(2,0);
		cell_as_vector[0] = cell.x;
		cell_as_vector[1] = cell.y;

		if (interesting_cells_left>=all_interesting_cells[0].size())
		{
			all_interesting_cells[0].push_back(cell_as_vector);
		}
		else
		{
			all_interesting_cells[0][interesting_cells_left] = cell_as_vector;
		}
		interesting_cells_left++;
	}
	nr_of_cells[0] = interesting_cells_left;
	
	
	int target_cells_left = 0;
	bool early_stop = false;

	for (size_t i = 0; i < target_cells.size(); i++)
	{
		godot::Vector2 cell = target_cells[i];
		cell.x += offset_x;
		cell.y += offset_y;

		int cell_as_array[2];
		cell_as_array[0] = cell.x;
		cell_as_array[1] = cell.y;

		if (!is_in_play_field(cell_as_array))
		{
			continue;
		}
		if (!cells_to_visit[cell.x][cell.y])
		{
			cells_to_visit[cell.x][cell.y] = true;
			target_cells_left += 1;
		}
		
	}

	if (target_cells_left == 0)
	{
		target_cells_left = 1;
		early_stop = false;
	}
	
	

	int current_neighbor[2];
	int current_neighbor_side_check1[2];
	int current_neighbor_side_check2[2];

	int current_nr_of_cells_to_check = 0;

	while (interesting_cells_left > 0 && target_cells_left > 0)
	{
		current_nr_of_cells_to_check = nr_of_cells[current_cost_pointer];
		for (size_t i = 0; i < current_nr_of_cells_to_check; i++)
		{

			nr_of_cells[current_cost_pointer] -= 1;
			interesting_cells_left -= 1;

			for (size_t k = 0; k < four_directions.size(); k++){
				current_neighbor[0] = all_interesting_cells[current_cost_pointer][i][0] + four_directions[k][0];
				current_neighbor[1] = all_interesting_cells[current_cost_pointer][i][1] + four_directions[k][1];
				if (!is_in_play_field(current_neighbor))
				{
					continue;
				}
				if (cost_map[current_neighbor[0]][current_neighbor[1]] == wall_cost){
					continue;
				}
				int new_distance_to_cell = distance_map[all_interesting_cells[current_cost_pointer][i][0]][all_interesting_cells[current_cost_pointer][i][1]] + cost_map[current_neighbor[0]][current_neighbor[1]];
				if (new_distance_to_cell < distance_map[current_neighbor[0]][current_neighbor[1]]){
					int new_distance_as_cost_pointer = new_distance_to_cell % max_cost_of_a_tile;
					distance_map[current_neighbor[0]][current_neighbor[1]] = new_distance_to_cell;
					interesting_cells_left += 1;
					direction_map[current_neighbor[0]][current_neighbor[1]][0] =  all_interesting_cells[current_cost_pointer][i][0]  - current_neighbor[0];
					direction_map[current_neighbor[0]][current_neighbor[1]][1] =  all_interesting_cells[current_cost_pointer][i][1]  - current_neighbor[1];
					if (nr_of_cells[new_distance_as_cost_pointer] >= all_interesting_cells[new_distance_as_cost_pointer].size())
					{
						all_interesting_cells[new_distance_as_cost_pointer].resize(all_interesting_cells[new_distance_as_cost_pointer].size() * 2, {0,0});
					}
					all_interesting_cells[new_distance_as_cost_pointer][nr_of_cells[new_distance_as_cost_pointer]][0] =  current_neighbor[0];
					all_interesting_cells[new_distance_as_cost_pointer][nr_of_cells[new_distance_as_cost_pointer]][1] =  current_neighbor[1];
					direction_map[current_neighbor[0]][current_neighbor[1]][0] = all_interesting_cells[current_cost_pointer][i][0]  - current_neighbor[0];
					direction_map[current_neighbor[0]][current_neighbor[1]][1] =all_interesting_cells[current_cost_pointer][i][1]  - current_neighbor[1];
					nr_of_cells[new_distance_as_cost_pointer] += 1;

					if (cells_to_visit[current_neighbor[0]][current_neighbor[1]])
					{
						cells_to_visit[current_neighbor[0]][current_neighbor[1]] = false;
						target_cells_left--;
					}
					



				}
		
			}
			
			// Almost the same code as above, just that it might need to check if the x and y Neighbor are both reachable Cells
			if (diagonals_allowed)
			{
				for (size_t k = 0; k < four_diagonals.size(); k++){
					current_neighbor[0] = all_interesting_cells[current_cost_pointer][i][0] + four_diagonals[k][0];
					current_neighbor[1] = all_interesting_cells[current_cost_pointer][i][1] + four_diagonals[k][1];
					if (!is_in_play_field(current_neighbor))
					{
						continue;
					}
					if (cost_map[current_neighbor[0]][current_neighbor[1]] == wall_cost){
						continue;
					}

					if (diagonals_need_neighbors)
					{
						current_neighbor_side_check1[0] = all_interesting_cells[current_cost_pointer][i][0] + four_diagonals[k][0];
						current_neighbor_side_check1[1] = all_interesting_cells[current_cost_pointer][i][1];

						current_neighbor_side_check2[0] = all_interesting_cells[current_cost_pointer][i][0];
						current_neighbor_side_check2[1] = all_interesting_cells[current_cost_pointer][i][1] + four_diagonals[k][1];


						if (!is_in_play_field(current_neighbor_side_check1))
						{
							continue;
						}
						if (cost_map[current_neighbor_side_check1[0]][current_neighbor_side_check1[1]] == wall_cost){
							continue;
						}
						if (!is_in_play_field(current_neighbor_side_check2))
						{
							continue;
						}
						if (cost_map[current_neighbor_side_check2[0]][current_neighbor_side_check2[1]] == wall_cost){
							continue;
						}
					}			
					int new_distance_to_cell = distance_map[all_interesting_cells[current_cost_pointer][i][0]][all_interesting_cells[current_cost_pointer][i][1]] + cost_map[current_neighbor[0]][current_neighbor[1]];
					if (new_distance_to_cell < distance_map[current_neighbor[0]][current_neighbor[1]]){
						int new_distance_as_cost_pointer = new_distance_to_cell % max_cost_of_a_tile;
						distance_map[current_neighbor[0]][current_neighbor[1]] = new_distance_to_cell;
						interesting_cells_left += 1;
						direction_map[current_neighbor[0]][current_neighbor[1]][0] =  all_interesting_cells[current_cost_pointer][i][0]  - current_neighbor[0];
						direction_map[current_neighbor[0]][current_neighbor[1]][1] =  all_interesting_cells[current_cost_pointer][i][1]  - current_neighbor[1];
						if (nr_of_cells[new_distance_as_cost_pointer] >= all_interesting_cells[new_distance_as_cost_pointer].size())
						{
							all_interesting_cells[new_distance_as_cost_pointer].resize(all_interesting_cells[new_distance_as_cost_pointer].size() * 2, {0,0});
						}
						all_interesting_cells[new_distance_as_cost_pointer][nr_of_cells[new_distance_as_cost_pointer]][0] =  current_neighbor[0];
						all_interesting_cells[new_distance_as_cost_pointer][nr_of_cells[new_distance_as_cost_pointer]][1] =  current_neighbor[1];
						direction_map[current_neighbor[0]][current_neighbor[1]][0] = all_interesting_cells[current_cost_pointer][i][0]  - current_neighbor[0];
						direction_map[current_neighbor[0]][current_neighbor[1]][1] = all_interesting_cells[current_cost_pointer][i][1]  - current_neighbor[1];
						nr_of_cells[new_distance_as_cost_pointer] += 1;
						if (cells_to_visit[current_neighbor[0]][current_neighbor[1]])
						{
						cells_to_visit[current_neighbor[0]][current_neighbor[1]] = false;
						target_cells_left--;
						}
					}
				}
			}	
		}
		
		current_cost_pointer = (current_cost_pointer + 1) % max_cost_of_a_tile;
		
	}

	if (target_cells_left > 0)
	{
		for (size_t i = 0; i < target_cells.size(); i++)
		{
			godot::Vector2 cell = target_cells[i];
			cell.x += offset_x;
			cell.y += offset_y;

			int cell_as_array[2];
			cell_as_array[0] = cell.x;
			cell_as_array[1] = cell.y;

			if (!is_in_play_field(cell_as_array))
			{
				continue;
			}
			cells_to_visit[cell.x][cell.y] = true;
		}
		
	}
	
	
}

bool FlowField::is_in_play_field(int cell[]){
		if ( cell[0]>= distance_map.size() || cell[0]<0)
	{
		return false;
	}

	if ( cell[1]<0 || cell[1] >= distance_map[cell[0]].size())
	{
		return false;
	}
	
	return true;


}

void FlowField::set_play_field(godot::TileMap* tile_map, godot::Dictionary cost_of_tiles){
	godot::Rect2 used_rect = tile_map->get_used_rect();
	godot::Array used_cells = tile_map->get_used_cells();

	max_cost_of_a_tile = 2;

	if (used_rect.position.x > (used_rect.position.x + used_rect.size.x)){
		offset_x = -(used_rect.position.x + used_rect.size.x);
	}
	else {
		offset_x = -used_rect.position.x;
	}

	if (used_rect.position.y > (used_rect.position.y + used_rect.size.y)){
		offset_y = -(used_rect.position.y + used_rect.size.y);
	}
	else {
		offset_y = -used_rect.position.y;
	}

	distance_map = std::vector<std::vector<int>>(used_rect.size.x, std::vector<int>(used_rect.size.y,max_distance));
	direction_map = std::vector<std::vector<std::vector<int>>>(used_rect.size.x, std::vector<std::vector<int>>(used_rect.size.y, std::vector<int>(2,0)));
	cost_map = std::vector<std::vector<int>>(used_rect.size.x, std::vector<int>(used_rect.size.y,wall_cost));
	cells_to_visit = std::vector<std::vector<bool>>(used_rect.size.x, std::vector<bool>(used_rect.size.y,false));
	int cost_of_the_tile = 0;
	godot::Dictionary tmp;

	for (size_t i = 0; i <used_cells.size(); i++)
	{
		tmp = cost_of_tiles[tile_map->get_cell(godot::Vector2(used_cells[i]).x, godot::Vector2(used_cells[i]).y)];
		cost_of_the_tile = tmp["cost"];

		if ((cost_of_the_tile+1) > max_cost_of_a_tile)
		{
			max_cost_of_a_tile = cost_of_the_tile + 1;
		}
		

		if (cost_of_the_tile == 0)
		{
			cost_of_the_tile = 255;
		}
		
		
		cost_map[offset_x + godot::Vector2(used_cells[i]).x][offset_y + godot::Vector2(used_cells[i]).y] = cost_of_the_tile;
		
	}


}

godot::Vector2 FlowField::get_cell_direction(godot::Vector2 cell){
	godot::Vector2 direction;
	cell.x += offset_x;
	cell.y += offset_y;
	int cell_as_array[2];
	cell_as_array[0] = cell.x;
	cell_as_array[1] = cell.y;

	if (!is_in_play_field(cell_as_array))
	{
		return direction;
	}
		direction.x = direction_map[cell.x][cell.y][0];
		direction.y = direction_map[cell.x][cell.y][1];
		return direction;
}

int FlowField::get_cell_distance(godot::Vector2 cell){
	int distance;
	cell.x += offset_x;
	cell.y += offset_y;
	int cell_as_array[2];
	cell_as_array[0] = cell.x;
	cell_as_array[1] = cell.y;

	if (!is_in_play_field(cell_as_array))
	{
		return -1;
	}

		return distance_map[cell.x][cell.y];
}
