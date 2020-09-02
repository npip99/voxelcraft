#include "world_gen.hpp"

extern int air_block;
extern int dirt_block;
extern int stone_block;
extern int log_block;
extern int leaf_block;

void generate_random_tree(World& world, ivec3 loc) {
    int r = rand() % 4;
    switch(r) {
    case 0:
        generate_tree_pyramid(world, loc);
        break;
    case 1:
        generate_tree_pyramid_truncated(world, loc);
        break;
    case 2:
        generate_tree_cute(world, loc);
        break;
    case 3:
        generate_tree_overhang(world, loc);
        break;
    }
}

void generate_tree_pyramid(World& world, ivec3 loc) {
    // Make stalk of tree
    world.set_block(loc.x, loc.y, loc.z, log_block);
    world.set_block(loc.x, loc.y+1, loc.z, log_block);

    // Make pyramid
    for(int dy = 2; dy <= 4; dy++) {
        int radius = dy == 2 ? 2 : (dy == 3 ? 1 : 0);
        for(int dx = -radius; dx <= radius; dx++) {
            for(int dz = -radius; dz <= radius; dz++) {
                world.set_block(loc.x + dx, loc.y + dy, loc.z + dz, leaf_block);
            }
        }
    }
    
    world.set_block(loc.x, loc.y+2, loc.z, log_block);
    world.set_block(loc.x, loc.y+3, loc.z, log_block);
}

void generate_tree_pyramid_truncated(World& world, ivec3 loc) {
    // Make stalk of tree
    world.set_block(loc.x, loc.y, loc.z, log_block);
    world.set_block(loc.x, loc.y+1, loc.z, log_block);

    // Make pyramid
    for(int dy = 2; dy <= 4; dy++) {
        int radius = dy == 2 ? 2 : (dy == 3 ? 1 : 0);
        for(int dx = -radius; dx <= radius; dx++) {
            for(int dz = -radius; dz <= radius; dz++) {
                if (radius > 0 && abs(dx) == radius && abs(dz) == radius) {
                    continue;
                }
                world.set_block(loc.x + dx, loc.y + dy, loc.z + dz, leaf_block);
            }
        }
    }
    
    world.set_block(loc.x, loc.y+2, loc.z, log_block);
    world.set_block(loc.x, loc.y+3, loc.z, log_block);
}

void generate_tree_cute(World& world, ivec3 loc) {
    world.set_block(loc.x, loc.y, loc.z, log_block);
    world.set_block(loc.x, loc.y+1, loc.z, log_block);

    for(int dx = -2; dx <= 2; dx++){
        for(int dz = -2; dz <= 2; dz++){
            for(int dy = 2; dy <= 3; dy++){
                world.set_block(loc.x + dx, loc.y + dy, loc.z + dz, leaf_block);
            }
        }
    }

    world.set_block(loc.x, loc.y + 4, loc.z, leaf_block);
}

void generate_tree_overhang(World& world, ivec3 loc) {
    for(int dy = 0; dy <= 2; dy++){
        world.set_block(loc.x, loc.y + dy, loc.z, log_block);
    }

    for(int dx = -2; dx <= 2; dx++){
        for(int dz = -2; dz <= 2; dz++){
            for(int dy = 3; dy <= 4; dy++){
                world.set_block(loc.x + dx, loc.y + dy, loc.z + dz, leaf_block);
            }
        }
    }

    for(int dy = 3; dy <= 4; dy++){
        for(int dx = -2; dx <= 2; dx += 4){
            for(int dz = -2; dz <= 2; dz += 4){
                world.set_block(loc.x + dx, loc.y + dy, loc.z + dz, air_block);
            }
        }
    }
    
    for(int dx = -1; dx <= 1; dx++){
        for(int dz = -1; dz <= 1; dz++){
            world.set_block(loc.x + dx, loc.y + 3, loc.z + dz, air_block);
        }
    }

    for(int dx = -1; dx <= 1; dx++){
        for(int dz = -1; dz <= 1; dz++){
            world.set_block(loc.x + dx, loc.y + 5, loc.z + dz, leaf_block);
        }
    }

    world.set_block(loc.x, loc.y + 3, loc.z, log_block);
}
