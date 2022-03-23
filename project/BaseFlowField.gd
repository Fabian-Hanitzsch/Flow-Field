extends Node
class_name FlowField2

var play_field = []
var four_directions = [Vector2(1,0), Vector2(-1,0), Vector2(0,1), Vector2(0,-1)]
var four_diagonal_directions = [Vector2(1,1), Vector2(-1,1), Vector2(1,-1), Vector2(-1,-1)]
var all_eight_directions = [Vector2(1,0), Vector2(-1,0), Vector2(0,1), Vector2(0,-1), 
							Vector2(1,1), Vector2(-1,1), Vector2(1,-1), Vector2(-1,-1)]

var cost_map
var distance_map
var direction_map


func reset_paths():
	direction_map = null
	cost_map = null
	distance_map = null

func reset():
	play_field = null
	cost_map = null
	distance_map = null
	direction_map = null

func is_cell_in_play_field(cell) -> bool:
	if cell.x < 0 or cell.x >= len(play_field) or cell.y < 0 or cell.y >= len(play_field[0]):
		return false
	return true

func prepare_maps():
	cost_map = []
	distance_map = []
	direction_map = []
	
	for row in play_field:
		var cost_map_row = []
		var distance_map_row = []
		var direction_map_row = []
		for cell in row:
			if cell >= 1:
				cost_map_row.append(1)
			else:
				cost_map_row.append(255)
			distance_map_row.append(65555)
			direction_map_row.append(Vector2(0,0))
	
		cost_map.append(cost_map_row)
		distance_map.append(distance_map_row)
		direction_map.append(direction_map_row)
		

func set_play_field(new_play_field):
	play_field = new_play_field
	prepare_maps()
	
func create_direction_map(target_cell):
	create_distance_map(target_cell)
	for x in len(distance_map):
		for y in len(distance_map[0]):
			direction_map[x][y] = get_vector_to_lowest_distance_neighbor(Vector2(x,y))
	
	return direction_map

func get_vector_to_lowest_distance_neighbor(cell):
	var lowest_distance = 65555
	var vector_to_lowest_distance_neighbor = Vector2.ZERO
	for direction in four_directions:
		var target_cell = cell + direction
		if not is_cell_in_play_field(target_cell):
			continue
		var distance_on_this_cell = distance_map[target_cell.x][target_cell.y]
		if distance_on_this_cell < lowest_distance :
			lowest_distance = distance_on_this_cell
			vector_to_lowest_distance_neighbor = target_cell - cell

	return vector_to_lowest_distance_neighbor

func create_distance_map(target_cell):
	if not is_cell_in_play_field(target_cell):
		return
	distance_map[target_cell.x][target_cell.y] = 0
	var interesting_cells = get_neighbors(target_cell)

	while not len(interesting_cells) == 0:
		var next_round_interesting_cells = []
		for cell in interesting_cells:
			var neighbors = get_neighbors(cell)
			for neighbor in neighbors:
				next_round_interesting_cells.append(neighbor)

		
		interesting_cells = next_round_interesting_cells
	
	print("DONE")
	
func get_neighbors(cell):
	var neighbors = []
	var distance_cell = distance_map[cell.x][cell.y]
	for direction in four_directions:
		var target_cell = cell + direction
		if not is_cell_in_play_field(target_cell):
			continue
		#if cost_map[target_cell.x][target_cell.y] == 255:
			#continue
			
		var distance_to_cell = distance_cell + cost_map[target_cell.x][target_cell.y]
		if distance_to_cell < distance_map[target_cell.x][target_cell.y]:
			neighbors.append(target_cell)
			distance_map[target_cell.x][target_cell.y] = distance_to_cell

	return neighbors

func get_cell_direction(cell):
	if not is_cell_in_play_field(cell):
		return Vector2.ZERO
	
	return direction_map[cell.x][cell.y]

