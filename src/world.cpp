#include "world.h"

#include <godot_cpp/godot.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/node.hpp>

#include <godot_cpp/classes/editor_file_system_directory.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/packed_scene.hpp>

#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/dir_access.hpp>
#include <godot_cpp/classes/file_access.hpp>
#include <godot_cpp/classes/json.hpp>

using namespace godot;

void World::_bind_methods()
{
    ClassDB::bind_method(D_METHOD("get_block", "pos"), &World::get_block);

    ClassDB::bind_method(D_METHOD("destroy_block", "pos"), &World::destroy_block);


    ClassDB::bind_method(D_METHOD("set_block", "pos", "id"), &World::set_block);

    ClassDB::bind_method(D_METHOD("on_node_added", "node"), &World::on_node_added);
    ClassDB::bind_method(D_METHOD("on_node_removed", "node"), &World::on_node_removed);

    BIND_ENUM_CONSTANT(COBBLESTONE);
    BIND_ENUM_CONSTANT(DIRT);
    BIND_ENUM_CONSTANT(MUD);
    BIND_ENUM_CONSTANT(STONE);
    BIND_ENUM_CONSTANT(SANDSTONE);
}

World::World()
{
    this->gravity = 9.81;
    this->chunk_loading_nodes = TypedArray<Node2D>();
    this->manager = nullptr;
    this->loaded_blocks = TypedArray<PackedScene>();
}

godot::World::~World()
{
    delete manager;
    manager = nullptr;
    loaded_blocks.clear();
    chunk_loading_nodes.clear();
}

void godot::World::_ready()
{
    this->manager = memnew(ChunkManager);
    if (get_tree()) {
        get_tree()->connect("node_added", Callable(this, "on_node_added"));
        get_tree()->connect("node_removed", Callable(this, "on_node_removed"));
    }

    // Ok, anem a descobrir els blocs amb json i carpetes
    String path = "res://papercraft/blocks/";

    this->chunk_loading_nodes.append(this->find_child("Player"));
    //UtilityFunctions::print(find_child("Player").get_name());

    // Ara obrim el json
    String json_path = "res://papercraft/tiles.json";

    TypedArray<String>  blocks;

    Ref<FileAccess> json_file = FileAccess::open(json_path, FileAccess::ModeFlags::READ);

    // Llegint el json_file
    if (json_file->is_open()) {
        String json_text = json_file->get_as_text();
        Ref<JSON> json_object;
        UtilityFunctions::print("{p:2}");
        Variant result = json_object->parse_string(json_text);
        UtilityFunctions::print(String("Found file: ") , result);
        if (result.get_type() == Variant::DICTIONARY) {
            Dictionary json_dict = result;
            if(json_dict.has("BLOCKS")){
                blocks = TypedArray<String>(json_dict["BLOCKS"]);
            }
        }
         json_file->close();

    }
    UtilityFunctions::print(blocks);

    // Aqui estic guardant els blocks a la RAM.
    for(int i = 0; i < blocks.size(); i++){
        String file_path = path + String(blocks[i]);
        // Et preguntes, perque obres el fitxer. ve aixo es perque
        // si no existeix llavors passa de tot.
        Ref<FileAccess> file = FileAccess::open(file_path, FileAccess::ModeFlags::READ);
        if(file->is_open()){
            UtilityFunctions::print(String("Found file: ") + file_path);
            Ref<PackedScene> packet_block =
                    ResourceLoader::get_singleton()->load(file_path);
            this->loaded_blocks.append(packet_block);
            file->close();
        }
    }
}

void godot::World::_process(double delta)
{
    for(int i = 0; i < chunk_loading_nodes.size(); i++){
        Node2D* loading_node = Object::cast_to<Node2D>(chunk_loading_nodes[i]);
        if(loading_node){
            Vector2i position = chunk_coords(Vector2i(loading_node->get_position()/BLOCK_SIZE));
            for(int j = -2; j <= 2; j ++){
                for(int k = -2; k <= 2; k ++){
                    Vector2i this_chunk_pos = Vector2i(position.x + j, position.y + k);
                    Chunk* chunk = manager->load_chunk(this_chunk_pos);
                    if(chunk ==  nullptr){
                        chunk = create_new_chunk(this_chunk_pos);
                    }
                    // Ara sí, podem començar a fer la
                    // generacio de terreny (nomes en cas que chunk state sigui 1!)
                    if(chunk->get_state() == 0){
                        chunk->set_state(1);
                    }else if(chunk->get_state() == 1){
                        chunk->set_state(2);
                        generate_chunk(this_chunk_pos , chunk);
                    }
                }
            }
        }
    }
}

Vector2i godot::World::block_chunk_pos( Vector2i pos) const
{
    return Vector2i(pos.x - (chunk_coords(pos).x * CHUNK_SIZE.x),pos.y - (chunk_coords(pos).y * CHUNK_SIZE.y));
}


int floor_div(int a, int b) {
    return (a >= 0) ? (a / b) : ((a - b + 1) / b);
}

Vector2i godot::World::chunk_coords(Vector2i pos) const
{

    return Vector2i(floor_div(pos.x, CHUNK_SIZE.x), floor_div(pos.y, CHUNK_SIZE.y));
}

Block* World::get_block(const Vector2 &pos) const
{
    Chunk* chunk = this->manager->load_chunk(chunk_coords(pos));
    if(chunk != nullptr){
        return chunk->get_block(pos - chunk_coords(pos)*CHUNK_SIZE);
    }else{
        return nullptr;
    }

}


  Block *World::get_block_instance(const int& BLOCK_ID) const{
    Ref<PackedScene> packed_scene = this->loaded_blocks[BLOCK_ID];
    if (!packed_scene.is_valid()) {
        UtilityFunctions::print("PackedScene no vàlid!");
        return nullptr;
    }
    return Object::cast_to<Block>(packed_scene->instantiate());
  }



void World::set_block(const Vector2 &pos, const int &id)
{
    // Obte id block
    int n = id;
    if (id >= this->loaded_blocks.size() || id < 0) {
        n = 0;
    }
    Block *new_block = get_block_instance(id);
    if (!new_block) {
        UtilityFunctions::print("No s'ha pogut instanciar el bloc!");
    }

    // Colocar block
    Chunk* chunk = manager->load_chunk(chunk_coords(pos));
    if (chunk != nullptr) {
        if(chunk->get_block(block_chunk_pos(pos)) == nullptr){
            chunk->set_block(block_chunk_pos(pos), new_block);
        }
    }
}

void godot::World::destroy_block(const Vector2 &pos)
{

    // Colocar block
    Chunk* chunk = manager->load_chunk(chunk_coords(pos));
    if (chunk != nullptr) {
        if(chunk->get_block(block_chunk_pos(pos)) != nullptr){
            //UtilityFunctions::print(String("Ok 3"));
            chunk->break_block(block_chunk_pos(pos));
        }
    }
}


void World::on_node_added(Node *p_node)
{
    if (!p_node) return;
    //UtilityFunctions::print("Node added: ", p_node->get_name());
    if(p_node->is_in_group("ChunkLoading")){
        this->chunk_loading_nodes.append(p_node);
    }
}

void World::on_node_removed(Node *p_node)
{
    if (!p_node) return;
    //UtilityFunctions::print("Node added: ", p_node->get_name());
    if(p_node->is_in_group("ChunkLoading")){
        int pos = -1;
        pos = this->chunk_loading_nodes.find(p_node);
        if(pos != -1){
            this->chunk_loading_nodes.erase(pos);
        }
    }
}

// Funcio per crear els chunks del mon
void World::generate_chunk(const Vector2i& chunk_coords,  Chunk *chunk){
    for(int i = 0; i < CHUNK_SIZE.x; i++){
        for(int j = 0; j < CHUNK_SIZE.y; j++){
            if(j*i > 16*16){
                UtilityFunctions::print("WAIT");
            }
           Vector2i block_position = Vector2i(j+chunk_coords.x*CHUNK_SIZE.x,
                i+chunk_coords.y*CHUNK_SIZE.y);
            if(cos(block_position.x) > sin(block_position.y)){
                if(tan(block_position.y) > 0){
                    if(tan(block_position.x) > 0){
                        chunk->set_block(Vector2i(j,i), get_block_instance(SANDSTONE));
                    }else{
                        chunk->set_block(Vector2i(j,i), get_block_instance(STONE));
                    }
                }
            }else{
                if(asin(block_position.x) >= asin(block_position.x) or tan(block_position.x) > tan(block_position.y)){
                    chunk->set_block(Vector2i(j,i), get_block_instance(DIRT));
                }else{
                    chunk->set_block(Vector2i(j,i), get_block_instance(MUD));
                }
            }
        }
    }
}

Chunk *godot::World::create_new_chunk(const Vector2i &chunk_coords)
{
        Chunk*  new_chunk = memnew(Chunk);
        this->add_child(new_chunk);
        new_chunk->set_position(chunk_coords * CHUNK_SIZE * BLOCK_SIZE);
        this->manager->save_chunk(chunk_coords, new_chunk);
        return new_chunk;
}
