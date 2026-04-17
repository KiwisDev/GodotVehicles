extends GVVehicle


func _physics_process(delta: float) -> void:
	var steer_input = Input.get_axis("ui_right", "ui_left")
	var accel_input = Input.get_axis("ui_down", "ui_up")
	
	self.steering_angle = steer_input * 0.5
	self.engine_force = accel_input * 5000
