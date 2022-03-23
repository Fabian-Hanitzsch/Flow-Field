tool
extends TileMap
class_name FlowField

var tile_map:TileMap

var rect_left_x = 0
var rect_top_y = 0
var rect_right_x = 0
var rect_bottom_y = 0

var offset_x = 0
var offset_y = 0

var eight_directions
var target
var maximum_id_of_a_tile = 100
var tiles = {}

onready var flow_field_module = preload("FlowField.gdns").new()

func _ready():
	update_play_field()

func update_play_field():
	flow_field_module.set_play_field(self, tiles)

func create_flow_field(x,y):
	flow_field_module.create_flow_field(x,y)
	
func get_direction_map():
	return flow_field_module.get_direction_map()

func get_distance_map():
	return flow_field_module.get_distance_map()



func _get_property_list():
	var array_to_return = []
	
	if true:
		var extra_properties = [{
		"name": "tile_map_path/target",
		"type": 1,
	},
	{
		"name": "tile_map_path/maximum_id_of_a_tile",
		"type": 2,
	},]
	
		for tile in tiles:

			extra_properties.append({
				"name": "tile_map2/" + str(tiles[tile].name),
				"type": 2,
			})
		for item in extra_properties:
			array_to_return.append(item)
	
	return array_to_return

func _set(property, value):
	if property == "aeqweasd":
		pass
	#	property_list_changed_notify()
	#	if not is_inside_tree():
	#		print("Not loaded")
	#		return
	#	tile_map = get_node(value)
#
#		if not tile_map.is_class("TileMap"):
#			print("STOP IT")
#			return
#		tile_map_path = value
#		set_tile_map_tiles_cost()
#		property_list_changed_notify()
	elif property == "tile_map_path/target":
		target = value
	elif property == "tile_map_path/maximum_id_of_a_tile":
		maximum_id_of_a_tile = value
		set_tile_map_tiles_cost()
		property_list_changed_notify()
	elif "tile_map2" in property:
		var name = property.substr(10)
		var nr = tile_set.find_tile_by_name(name)
		if value < 0:
			value = 0
		tiles[nr].cost = value

func _get(property):
	if property == "aedwq":
		return 21
	elif property == "tile_map_path/target":
		return target
	elif property == "tile_map_path/maximum_id_of_a_tile":
		return maximum_id_of_a_tile
	elif "tile_map2" in property:
		var name = property.substr(10)
		var nr = tile_set.find_tile_by_name(name)
		return tiles[nr].cost

func set_tile_map_tiles_cost():
	var updated_tiles = {}
	
	for cell in get_used_cells():
		var id = get_cell(cell.x, cell.y)
		var name = tile_set.tile_get_name(id)
		if not tiles.has(id):
			tiles[id] = {"name": name,
						"cost": 0}
		else:
			tiles[id].name = name
		updated_tiles[id] = true
	
	var ids_to_delete = {}
	for id in tiles:
		if not updated_tiles.has(id):
			ids_to_delete[id] = true
	
	for id in ids_to_delete:
		tiles.erase(id)
