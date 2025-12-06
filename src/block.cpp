#include "block.h"
#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/rectangle_shape2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>


using namespace godot;

void Block::_bind_methods() {

    	ClassDB::bind_method(D_METHOD("get_world"), &Block::get_world);
        ClassDB::bind_method(D_METHOD("get_block_position"), &Block::get_block_position);
        ClassDB::bind_method(D_METHOD("get_block_at"), &Block::get_block_at);

    ADD_SIGNAL(MethodInfo("destroyed", PropertyInfo(Variant::VECTOR2, "position")));

	ClassDB::bind_method(D_METHOD("get_id"), &Block::get_id);
	ClassDB::bind_method(D_METHOD("set_id", "p_id"), &Block::set_id);

    ADD_PROPERTY(
        PropertyInfo(
            Variant::STRING,
            "id"
        ),
        "set_id",
        "get_id"
    );

}

Block::Block()
{
    this->id = "null";

}

void Block::_ready()
{

}

void Block::_process(double delta)
{

}

World* Block::get_world() const {
    return Object::cast_to<World>(this->get_parent()->get_parent());
}

Vector2i godot::Block::get_block_position() const
{
        Vector2 block_pos = get_global_position();
        Vector2i tile_pos = Vector2i(floor(block_pos.x/16), floor(block_pos.y/16));
        return tile_pos;
}

Block *godot::Block::get_block_at(const Vector2i position)
{
     World * world = get_world();
     if(world){
        return world->get_block(position);
     }else{
        return &*this;
     }

}

String Block::get_id() const{
    return this->id;
}

void Block::set_id(const String &p_id){
    this->id = p_id;
}
