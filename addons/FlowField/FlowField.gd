tool
extends TileMap

var tile_map:TileMap
var allow_diagonals
var diagonals_need_neighbors
var tiles = {}
onready var flow_field_module = preload("res://addons/FlowField/FlowFieldModule.gdns").new()

func _ready():
	set_tile_map_tiles_cost()
	update_play_field()

func update_play_field() -> void:
	flow_field_module.set_play_field(self, tiles)

func create_flow_field(start_cell:Vector2, target_cells=[]) -> void:
	for cell in target_cells:
		if not typeof(cell) == 5:
			return
	flow_field_module.create_flow_field([start_cell],target_cells, allow_diagonals,diagonals_need_neighbors)

func create_flow_fieldv(start_cells:Array, target_cells=[]) -> void:
	for cell in start_cells:
		if not typeof(cell) == 5:
			return
	for cell in target_cells:
		if not typeof(cell) == 5:
			return

	flow_field_module.create_flow_field(start_cells,target_cells, allow_diagonals,diagonals_need_neighbors)

func get_cell_direction(cell:Vector2) -> Vector2:
	return flow_field_module.get_cell_direction(cell)

func get_cell_distance(cell:Vector2) -> int:
	return flow_field_module.get_cell_distance(cell)

func _get_property_list():
	var array_to_return = []
	set_tile_map_tiles_cost()
	property_list_changed_notify()
	
	if true:
		var extra_properties = [{
		"name": "allow_diagonals",
		"type": 1,
	},
	{
		"name": "diagonals_need_neighbors",
		"type": 1,
	},]

		for tile in tiles:
			extra_properties.append({
				"name": "tile_costs/" + str(tiles[tile].name),
				"type": 2,
			})
		for item in extra_properties:
			array_to_return.append(item)
	
	return array_to_return

func _set(property, value):
	set_tile_map_tiles_cost()
	if property == "allow_diagonals":
		allow_diagonals = value
	elif property == "diagonals_need_neighbors":
		diagonals_need_neighbors = value
	elif "tile_costs" in property:
		var name = property.substr(len("tile_costs") + 1)
		var nr = tile_set.find_tile_by_name(name)
		if value < 0:
			value = 0
		tiles[nr].cost = value

func _get(property):
	if property == "allow_diagonals":
		return allow_diagonals
	elif property == "diagonals_need_neighbors":
		return diagonals_need_neighbors
	elif "tile_costs" in property:
		var name = property.substr(len("tile_costs") + 1)
		var nr = tile_set.find_tile_by_name(name)
		return tiles[nr].cost

func set_tile_map_tiles_cost():
	var updated_tiles = {}
	
	for id in tile_set.get_tiles_ids():
		var name = tile_set.tile_get_name(id)
		if not tiles.has(id):
			tiles[id] = {"name": name,
						"cost": 1}
		else:
			tiles[id].name = name
		updated_tiles[id] = true
	
	var ids_to_delete = {}
	for id in tiles:
		if not updated_tiles.has(id):
			ids_to_delete[id] = true
	
	for id in ids_to_delete:
		tiles.erase(id)

