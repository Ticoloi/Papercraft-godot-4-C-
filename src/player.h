#ifndef PLAYER_H
#define PLAYER_H

#include <godot_cpp/classes/character_body2d.hpp>
#include <godot_cpp/classes/input.hpp>
#include "world.h"


namespace godot
{
    class Player : public godot::CharacterBody2D
    {
        GDCLASS(Player, CharacterBody2D);

    private:


        // Variables
        double speed;

        double jump_height;

        int selected_block;

        int chunk_loading_radius;

        Input *m_input;

        World *world;

        Vector2i direction;

        bool moving;

        float fall_speed;

    protected:
        static void _bind_methods();

    public:

    Player();

    // Inici jugador
    void _ready() override;
    // process jugador
    void _physics_process(double delta) override;

    // defineix velocitat
    void set_speed(const double& p_speed);

    // defineix altura salt
    void set_jump_height(const double &p_jump_height);

    // obtenir velocitat
    double get_speed() const;

    // obtenir altura salt
    double get_jump_height() const;

    // escriure block seleccionat
    void set_selected_block(const int& p_selected_block);

    // obtenir block seleccionat
    int get_selected_block() const;

    int get_chunk_loading_radius() const;

    void set_chunk_loading_radius(const int& p_chunk_loading_radius);

    };
}

#endif
