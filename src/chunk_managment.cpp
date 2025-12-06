// chunk_managment.cpp
#include "chunk_managment.h"

void ChunkManager::_bind_methods() {
    ClassDB::bind_method(D_METHOD("save_chunk", "chunk_coords", "chunk_data"), &ChunkManager::save_chunk);
    ClassDB::bind_method(D_METHOD("load_chunk", "chunk_coords"), &ChunkManager::load_chunk);
    ClassDB::bind_method(D_METHOD("chunk_exists", "chunk_coords"), &ChunkManager::chunk_exists);
    ClassDB::bind_method(D_METHOD("world_to_chunk_pos", "world_coords"), &ChunkManager::block_pos_to_chunk_pos);
}

ChunkManager::ChunkManager() {
    this->chunks =  Dictionary();
}

ChunkManager::~ChunkManager() {
    // Iterar per tots els chunks i alliberar la memòria
    Array keys = chunks.keys();

    for (int i = 0; i < keys.size(); i++)
    {
        Variant key = keys[i];
        Variant v = chunks.get(keys[i], Variant());
        if (v.get_type() == Variant::OBJECT) {
            Object *obj = (Object*)v;                       // extract contained Object*
            Chunk *chunk = Object::cast_to<Chunk>(obj);    // safe downcast to Chunk*
            if (chunk) {
                memdelete(chunk);
                chunk = nullptr;
            }
        }
    }
    chunks.clear();

}
void ChunkManager::save_chunk(const Vector2i& chunk_coords, const Chunk* chunk) {
    chunks[chunk_coords] = chunk;
}

Chunk* ChunkManager::load_chunk(const Vector2i& chunk_coords) const{
    Variant v = chunks.get(chunk_coords, Variant());
    if (v.get_type() == Variant::OBJECT) {
        Object *obj = (Object*)v;                       // extract contained Object*
        Chunk *chunk = Object::cast_to<Chunk>(obj);    // safe downcast to Chunk*
        if (chunk) {
            return chunk;
        }
    }
    return nullptr;
}

bool ChunkManager::chunk_exists(const Vector2i& chunk_coords) const{
    return chunks.has(chunk_coords);
}

Vector2i ChunkManager::block_pos_to_chunk_pos(const Vector2i& world_coords) const{
    return Vector2i(
        Math::floor((double)world_coords.x / CHUNK_SIZE.x),
        Math::floor((double)world_coords.y / CHUNK_SIZE.y)
    );
}
