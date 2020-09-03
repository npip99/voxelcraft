#ifndef _MAIN_GAME_HPP_
#define _MAIN_GAME_HPP_

#include "../utils.hpp"
#include "../world.hpp"
#include "../player.hpp"
#include "../input.hpp"

class Game {
public:
    Game();
    ~Game();
    void iterate(InputState& input);
    void render();
    Player player;

    void restart_world();
    void load_world(const char* filename);
    void save_world(const char* filename);
    
    bool paused = false;
private:
    World world;
    InputState input;
    double last_save = -1.0f;

    void do_something();
    double last_time;
    float flying_speed;
    vec2 get_mouse_rotation();
    vec3 get_keyboard_movement();
    bool mining_block(float time);
    void place_block(int block);
    void handle_player_movement(double currentTime, float deltaTime);
};

#endif
