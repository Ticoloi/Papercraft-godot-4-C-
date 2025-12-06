#ifndef BLOCK_H
#define BLOCK_H

#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/classes/static_body2d.hpp>
#include <godot_cpp/classes/collision_shape2d.hpp>
#include <godot_cpp/classes/sprite2d.hpp>
#include <godot_cpp/classes/input.hpp>

#include "world.h"

const int BLOCK_SIZE = 16;

namespace godot
{
    class World; // Forward declaration

    class Block : public godot::StaticBody2D
    {
        GDCLASS(Block, StaticBody2D);

    private:

        String id;

    protected:
        static void _bind_methods();

    public:



    Block();

    void _ready() override;

    void _process(double delta) override;

    World* get_world() const;

    // Funcio per obtenir la poscio del mon
    Vector2i get_block_position() const;

    // Funcio per obtenir el block
    Block* get_block_at(const Vector2i position);

    String get_id() const;

    void set_id(const String &p_id);

    };
}



#endif
