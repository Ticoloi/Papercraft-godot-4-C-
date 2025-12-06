#ifndef CHUNK_MANAGER_H
#define CHUNK_MANAGER_H
#include <godot_cpp/classes/ref_counted.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/vector2i.hpp>
#include "chunk.h"

// O si utilitzes Vector2i:


namespace godot
{
    class Chunk;

    class ChunkManager : public godot::RefCounted {
        GDCLASS(ChunkManager,  godot::RefCounted)

    private:
        // Diccionari amb els chunks
        // O millor encara:
        Dictionary chunks;  // Godot 4

    protected:
        static void _bind_methods();

    public:
        // Constructor
        ChunkManager();

        // Destructor
        ~ChunkManager();

        // Gurada el chunk
        void save_chunk(const Vector2i& chunk_coords, const Chunk* chunk);

        // Carrega chunk de memoria, retorna el node chunk
        Chunk* load_chunk(const Vector2i& chunk_coords) const;

        // Existeix el chunk?
        bool chunk_exists(const Vector2i& chunk_coords) const;

        // Converteix variables
        Vector2i block_pos_to_chunk_pos(const Vector2i& world_coords) const;
    };
}
#endif // CHUNK_MANAGER_H
