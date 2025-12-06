#ifndef WORLD_H
#define WORLD_H

#include <godot_cpp/classes/node2d.hpp>
#include "chunk.h"
#include "chunk_managment.h"
#include <godot_cpp/variant/typed_array.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

#include "block.h"

namespace godot
{
    class Chunk; // Forward declaration
    class Block; // Forward declaration
    class ChunkManager; // Forward declaration

    class World : public godot::Node2D
    {
        GDCLASS(World, Node2D);

    private:

        // Aquest fara el managment de chunks
        ChunkManager* manager;

        // Pre: Una poscio de block
        // Post: La poscicio del block al chunk
         Vector2i block_chunk_pos(Vector2i pos) const;

        // Pre: Una poscio de block
        // Post: La del chunk
         Vector2i chunk_coords(Vector2i pos) const;


        // Entitats que careguen
        // chunks
        TypedArray<Node2D> chunk_loading_nodes;



        // Preload dels blocs
        TypedArray<PackedScene> loaded_blocks;

        // Obte la instancia del block
        // Com a punter a partir de
        // el seu ID enter
        Block* get_block_instance(const int& BLOCK_ID) const;

        // Pre: Node afegit nou a escena
        // Post: En cas que sigui un node de cargar chunks,
        // serà afegit al vector chunk_loading_nodes
        void on_node_added(Node *p_node);

        // Pre: Node *eliminat* de escena
        // Post: En cas que sigui un node de cargar chunks,
        // serà ELIMINAT al vector chunk_loading_nodes
        void on_node_removed(Node *p_node);

        // Genera el chunk,
        // Passali les coords de chunk
        // i la seva referencia :)
        void generate_chunk(const Vector2i& chunk_coords,  Chunk *chunk);

        // Crea el chunk
        // retorna el punter al chunk (dons esta dins la escena)
        Chunk* create_new_chunk(const Vector2i& chunk_coords);

        double gravity;

    protected:

        static void _bind_methods();

    public:

    void _process(double delta) override;

    enum BlockType {
        COBBLESTONE = 0,
        DIRT = 1,
        MUD = 2,
        STONE = 3,
        SANDSTONE = 4

    // ... afegeix més tipus
    };

    World();

    ~World();

    void _ready() override;

    // Aquesta funcio es la que elimina la tile del array de blocs
    void tile_break(const Vector2& pos);

    // Pre : Les coordenades del mon
    // Post: Retorna el bloc
    Block* get_block(const Vector2& pos) const;

    // Pre : Les coordenades del mon i un ID
    // Post: Afegeix el block al chunk
    void set_block(const Vector2& pos, const int& id);

    // Pre : Les coordenades del mon
    // Post: Destrueix block
    void destroy_block(const Vector2& pos);

    };
}

VARIANT_ENUM_CAST(World::BlockType);

// Ensure the header guard is properly closed
#endif // WORLD_H
