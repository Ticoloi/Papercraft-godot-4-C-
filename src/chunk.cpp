#include "chunk.h"

void Chunk::_bind_methods() {

}

Chunk::Chunk(){
    // this->blocks = NULL; // Removed, Array does not need to be set to NULL
    this->blocks.resize(CHUNK_SIZE.x * CHUNK_SIZE.y);

    for (int i = 0; i < CHUNK_SIZE.x; i++) {
        for (int j = 0; j < CHUNK_SIZE.y; j++) {
            this->blocks[i] = Variant();
        }
    }
    this->chunk_state = 0;
}

godot::Chunk::~Chunk()
{
    for (int i = 0; i < CHUNK_SIZE.x; i++) {
        for (int j = 0; j < CHUNK_SIZE.y; j++) {
            this->break_block(Vector2i(i,j));
        }
    }
}

void godot::Chunk::_ready()
{

}

Block* Chunk::get_block(const Vector2& pos) const {
    int index = pos.x + pos.y * CHUNK_SIZE.x;
    if(this->blocks[index] == Variant()) {
        return nullptr;
    }
    return Object::cast_to<Block>(this->blocks[index]);
}


void Chunk::break_block(const Vector2& pos){
    int index = pos.x + pos.y * CHUNK_SIZE.x;
    Block * rmv = Object::cast_to<Block>(this->blocks[index]);
    if(rmv != nullptr){
        // Eliminar la memòria
        memdelete(rmv);

        // CRÍTIC: Assignar nullptr o Variant() a l'array
        blocks[index] = Variant();  // ← AQUEST ÉS EL PAS QUE ET FALTABA
    }
}


void Chunk::set_block(const Vector2& pos, Block* new_block){
    int index = pos.x + pos.y*CHUNK_SIZE.x;
    if(get_block(pos) == Variant()){
        new_block->set_position(pos*BLOCK_SIZE);
        this->add_child(new_block);
        this->blocks[index] = new_block; // Només si block és Block* vàlid
    }else{
        this->break_block(pos);
        new_block->set_position(pos*BLOCK_SIZE);
        this->add_child(new_block);
        this->blocks[index] = new_block; // Només si block és Block* vàlid
    }
}

int godot::Chunk::get_state() const
{
    return this->chunk_state;
}

void godot::Chunk::set_state(const int &p_state)
{
    this->chunk_state = p_state;
}


//++++

//+x++

//++++
