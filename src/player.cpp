#include "player.h"
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/scene_tree.hpp>

using namespace godot;

void Player::_bind_methods() {

	ClassDB::bind_method(D_METHOD("get_speed"), &Player::get_speed);
	ClassDB::bind_method(D_METHOD("set_speed", "p_speed"), &Player::set_speed);

    ADD_PROPERTY(
        PropertyInfo(
            Variant::FLOAT,
            "speed"
        ),
        "set_speed",
        "get_speed"
    );

	ClassDB::bind_method(D_METHOD("get_jump_height"), &Player::get_jump_height);
	ClassDB::bind_method(D_METHOD("set_jump_height", "p_jump_height"), &Player::set_jump_height);

    ADD_PROPERTY(
        PropertyInfo(
            Variant::FLOAT,
            "jump_height"
        ),
        "set_jump_height",
        "get_jump_height"
    );

	ClassDB::bind_method(D_METHOD("get_selected_block"), &Player::get_selected_block);
	ClassDB::bind_method(D_METHOD("set_selected_block", "p_selected_block"), &Player::set_selected_block);

    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "selected_block"
        ),
        "set_selected_block",
        "get_selected_block"
    );

    ClassDB::bind_method(D_METHOD("get_chunk_loading_radius"), &Player::get_chunk_loading_radius);
	ClassDB::bind_method(D_METHOD("set_chunk_loading_radius", "p_chunk_loading_radius"), &Player::set_chunk_loading_radius);

    ADD_PROPERTY(
        PropertyInfo(
            Variant::INT,
            "chunk_loading_radius"
        ),
        "set_chunk_loading_radius",
        "get_chunk_loading_radius"
    );

    // Senyals
    ADD_SIGNAL(MethodInfo("direction_changed", PropertyInfo(Variant::VECTOR2I, "direction")));
    ADD_SIGNAL(MethodInfo("moving_changed", PropertyInfo(Variant::BOOL, "moving")));
}

Player::Player()
{
    this->speed = 70;
    this->jump_height = 50;
    this->m_input = Input::get_singleton();
    this->world = nullptr;
    this->chunk_loading_radius = 8;
    selected_block = World::DIRT;
    this->fall_speed = 0;


}

void Player::_ready()
{
    this->world = Object::cast_to<World>(this->get_parent());
    direction = Vector2i(0,0);
    emit_signal("direction_changed", direction);
    moving = false;
    this->add_to_group("ChunkLoading");
}



void Player::_physics_process(double delta)
{
	//var dir_x = Input.get_action_strength("ui_right") - 1*Input.get_action_strength("ui_left")
	//var dir_y = Input.get_action_strength("ui_down") - 1*Input.get_action_strength("ui_up")

    double dir_x = m_input->get_action_strength("ui_right") -  m_input->get_action_strength("ui_left");
    Vector2i vector_direction(int(dir_x), 0);

    if(direction != vector_direction){
        direction = vector_direction;
        emit_signal("direction_changed", direction);
    }

    if(dir_x == 0 and moving){
        moving = false;
        emit_signal("moving_changed", moving);
    }else if(dir_x != 0 and !moving){
        moving = true;
        emit_signal("moving_changed", moving);
    }

    // Tema gravetat
    if(is_on_floor() or is_on_ceiling()){
        fall_speed = 9.81;
    }else{
        fall_speed = fall_speed + 9.81;
    }
    if(m_input->is_action_just_pressed("ui_accept")){
        if(is_on_floor() or is_on_ceiling()){
            fall_speed = fall_speed - this->get_jump_height();
        }
    }

    move_and_collide(Vector2(dir_x*speed*delta,0));
    set_velocity(Vector2(0,fall_speed));
    move_and_slide();

    // Tema ratoli
    if(m_input->is_mouse_button_pressed(MOUSE_BUTTON_RIGHT)){
        Vector2 mouse_pos = get_global_mouse_position();
        Vector2i tile_pos = Vector2i(floor(mouse_pos.x/16), floor(mouse_pos.y/16));
        this->world->set_block(tile_pos, get_selected_block());
    }
    if(m_input->is_mouse_button_pressed(MOUSE_BUTTON_LEFT)){
        Vector2 mouse_pos = get_global_mouse_position();
        Vector2i tile_pos = Vector2i(floor(mouse_pos.x/16), floor(mouse_pos.y/16));
        this->world->destroy_block(tile_pos);
    }

}


void Player::set_speed(const double &p_speed)
{
    this->speed = p_speed;
}

void Player::set_jump_height(const double &p_jump_height)
{
    this->jump_height = p_jump_height;
}

double Player::get_speed() const
{
    return this->speed;
}

double Player::get_jump_height() const
{
    return this->jump_height;
}

void godot::Player::set_selected_block(const int &p_selected_block)
{
    this->selected_block = p_selected_block;
}

int godot::Player::get_selected_block() const
{
    return this->selected_block;
}

int godot::Player::get_chunk_loading_radius() const
{
    return this->chunk_loading_radius;
}

void godot::Player::set_chunk_loading_radius(const int &p_chunk_loading_radius)
{
    this->chunk_loading_radius = p_chunk_loading_radius;
}
