#ifndef CHUNK_H
#define CHUNK_H


#include "block.h"
#include <godot_cpp/classes/node2d.hpp>
#include <godot_cpp/godot.hpp>

#include <godot_cpp/variant/typed_array.hpp>

using namespace godot;


const Vector2 CHUNK_SIZE = Vector2(16,16);


namespace godot
{
    class Block; // Forward declaration

    class Chunk : public godot::Node2D
    {
        GDCLASS(Chunk, Node2D);

    private:
        // El estat del chunk
        int chunk_state;

        TypedArray<Block> blocks;

    protected:

        static void _bind_methods();

    public:

    Chunk();
    ~Chunk();


    void _ready() override;

    // Aquesta funcio es la que elimina la tile del array de blocs
    void break_block(const Vector2& pos);

    // Pre: La coordenada del cchunk
    // Post: Opte el bloc
    Block* get_block(const Vector2& pos) const;

    // Pre: La coordenada del cchunk i un bloc
    // Post: Afegeix el bloc
    void set_block(const Vector2& pos, Block* new_block);

    // Retorna chunk_state
    int get_state() const;

    // Esriu state
    void set_state(const int& p_state);

    };
}

#endif
