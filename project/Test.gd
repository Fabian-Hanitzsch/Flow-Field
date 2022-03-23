extends Node2D

var random_field = []
var all_arrows = {}

onready var flow_field = $FlowField
onready var label_time_update = $"CanvasLayer/Control/VBoxContainer/update Play Field/Time"
onready var label_time_create = $"CanvasLayer/Control/VBoxContainer/create Flow Field/Time"
onready var label_nr_of_cells = $"CanvasLayer/Control/VBoxContainer/Number of Cells/Time"
onready var label_access_time = $"CanvasLayer/Control/VBoxContainer/Access/Time"

onready var arrow_scene = preload("res://Arrow.tscn")

func _ready():
	create_arrows()
	
	
func update_flow_field(cell):
	var begin_time = OS.get_ticks_usec()
	
	flow_field.update_play_field()
	var end_time = OS.get_ticks_usec()
	
	label_time_update.text = str((end_time - begin_time) / 1000.0)
	
	begin_time = OS.get_ticks_usec()
	flow_field.create_flow_field(cell)
	end_time = OS.get_ticks_usec()
	
	label_time_create.text = str((end_time - begin_time) / 1000.0)
	label_nr_of_cells.text = str(flow_field.get_used_cells().size())
	
	
	
	update_arrows()


func create_arrows():
	flow_field.get_used_cells()
	for cell in flow_field.get_used_cells():
		var arrow = arrow_scene.instance()
		arrow.position = (cell + Vector2(0.5,0.5)) * flow_field.cell_size
		add_child(arrow)
		all_arrows[cell] = arrow

func update_arrows():
	for cell in all_arrows:
		all_arrows[cell].rotation = 0
		all_arrows[cell].rotate(flow_field.get_cell_direction(cell).angle() + deg2rad(90) )

func measure_access_speed():
	var i = 0
	var begin_time = OS.get_ticks_usec()
	while (i < 10000):
		i+= 1
		var direction = flow_field.get_cell_direction(Vector2(0,0))
	
	var end_time = OS.get_ticks_usec()
	label_access_time.text = str((end_time - begin_time) / 1000.0)
	
	
func _process(delta):
	if Input.is_action_just_pressed("ui_accept"):
		var mous_position = get_global_mouse_position()
		var cell:Vector2 = mous_position / flow_field.cell_size
		cell = cell.floor()
		update_flow_field(cell)
		measure_access_speed()
		
