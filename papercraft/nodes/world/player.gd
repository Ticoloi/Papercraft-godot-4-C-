extends Player


func _input(event):
	if event is InputEventKey:
		if event.as_text_keycode() == "5":
			selected_block = World.SANDSTONE
		if event.as_text_keycode() == "4":
			selected_block = World.STONE
		if event.as_text_keycode() == "3":
			selected_block = World.COBBLESTONE
		elif event.as_text_keycode() == "2":
			selected_block = World.MUD
		elif event.as_text_keycode() == "1":
			selected_block = World.DIRT
	 
 

func _on_direction_changed(direction: Vector2i) -> void:
	if(direction == Vector2i(0,0)):
		$AnimatedSprite2D.play("idle")
	else:
		$AnimatedSprite2D.play("walk")
		if(direction.x > 0):
			$AnimatedSprite2D.flip_h = true
		else:
			$AnimatedSprite2D.flip_h = false
