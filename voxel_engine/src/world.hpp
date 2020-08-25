#ifndef _WORLD_HPP_
#define _WORLD_HPP_

#include "utils.hpp"
#include "aabb.hpp"
#include "chunk.hpp"

using fn_on_collide = std::function<void(vec3)>;

class World {
public:
    int world_id;

    vector<Chunk> chunks;

    World();

    void set_block(int x, int y, int z, Block* b);

    Block* get_block(int x, int y, int z);

    Chunk* get_chunk(int x, int y, int z);
    Chunk* make_chunk(int x, int y, int z);

    void render(mat4 &PV);

    bool is_in_block(vec3 position);

    optional<ivec3> raycast(vec3 position, vec3 direction, float max_distance, bool nextblock = false);

    void collide(AABB collision_box, fn_on_collide on_collide);
};

class Universe {
public:
    vector<Texture> textures;
    vector<Block> blocks;
    vector<World> worlds;
};

#endif
