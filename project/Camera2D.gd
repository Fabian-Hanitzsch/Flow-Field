extends Camera2D


# Declare member variables here. Examples:
# var a = 2
# var b = "text"
var speed = 1000

# Called when the node enters the scene tree for the first time.
func _ready():
	pass # Replace with function body.

func _process(delta):
	if Input.is_action_pressed("ui_left"):
		self.position += Vector2.LEFT * delta * speed
	if Input.is_action_pressed("ui_right"):
		self.position += Vector2.RIGHT * delta * speed
	if Input.is_action_pressed("ui_up"):
		self.position += Vector2.UP * delta * speed
	if Input.is_action_pressed("ui_down"):
		self.position += Vector2.DOWN * delta * speed
