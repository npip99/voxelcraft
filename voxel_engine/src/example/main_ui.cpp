#include "main_ui.hpp"

int crosshair_texture;
int main_button_texture;
int button_texture;
int button_selection_texture;
int hotbar_texture;
int hotbar_button_texture;
int hotbar_selected_texture;

MainUI::MainUI(Game* game) {
    // Initialization of main UI stuff
    crosshair_texture = get_universe()->register_texture("assets/images/crosshair.bmp", ivec3(255, 0, 255));
    main_button_texture = get_universe()->register_texture("assets/images/boxes_test.bmp", ivec3(255, 0, 255));
    button_texture = get_universe()->register_texture("assets/images/save_button.bmp", ivec3(255, 0, 255));
    button_selection_texture = get_universe()->register_texture("assets/images/button_selected.bmp", ivec3(255, 0, 255));
    hotbar_texture = get_universe()->register_texture("assets/images/hotbar.bmp", ivec3(255, 0, 255));
    hotbar_button_texture = get_universe()->register_texture("assets/images/hotbar_button.bmp", ivec3(255, 0, 255));
    hotbar_selected_texture = get_universe()->register_texture("assets/images/hotbar_selected.bmp", ivec3(255, 0, 255));

    int dirt_texture = get_universe()->register_texture("assets/images/dirt.bmp");
    int stone_texture = get_universe()->register_texture("assets/images/stone.bmp");
    int log_side_texture = get_universe()->register_texture("assets/images/log_side.bmp");

    this->game = game;
    this->crosshair = UI_Element(crosshair_texture);
    crosshair.size = ivec2(25);
    hotbar_selected = UI_Element(hotbar_selected_texture);
    hotbar_menu.background = UI_Element(hotbar_texture);
    hotbar_menu.background.value().size = ivec2(163, 21) * 5;

    vector<int> hotbar_textures = {dirt_texture, stone_texture, log_side_texture};
    for(int i = 4; i <= 9; i++) {
        hotbar_textures.push_back(hotbar_button_texture);
    }
    for(int i = 1; i <= 9; i++) {
        hotbar_menu.buttons.push_back(Button(UI_Element(hotbar_textures[i - 1]), "", [](){}));
    }

    main_menu.font = &font;
    save_menu.font = &font;
    load_menu.font = &font;
    hotbar_menu.font = &font;

    main_menu.buttons = {
        Button(UI_Element(main_button_texture), "Play", [this]() {
            this->game->paused = false;
        }),
        Button(UI_Element(button_texture), "Save Game", [this]() {
            this->menu = MenuState::SaveMenu;
            this->save_selected = this->save_loaded;
        }),
        Button(UI_Element(button_texture), "Load Game", [this]() {
            this->menu = MenuState::LoadMenu;
            this->save_selected = this->save_loaded;
        }),
        Button(UI_Element(button_texture), "New Game", [this]() {
            this->game->restart_world();
            this->game->paused = false;
            this->save_loaded = -1;
        }),
        Button(UI_Element(button_texture), "Exit", [this]() {
            this->exiting = true;
        })
    };

    save_menu.buttons = {
        Button(UI_Element(main_button_texture), "Back", [this]() {
            this->menu = MenuState::MainMenu;
        }),
        Button(UI_Element(button_texture), "Game 1", [this]() {
            this->save_selected = 1;
        }),
        Button(UI_Element(button_texture), "Game 2", [this]() {
            this->save_selected = 2;
        }),
        Button(UI_Element(button_texture), "Game 3", [this]() {
            this->save_selected = 3;
        }),
        Button(UI_Element(button_texture), "Save", [this]() {
            if (this->save_selected > 0) {
                char filename[128];
                snprintf(filename, sizeof(filename), "saves/save_%d.save", this->save_selected);
                this->game->save_world(filename);
                this->save_loaded = this->save_selected;
                this->menu = MenuState::MainMenu;
                this->game->paused = false;
            }
        })
    };

    load_menu.buttons = {
        Button(UI_Element(main_button_texture), "Back", [this]() {
            this->menu = MenuState::MainMenu;
        }),
        Button(UI_Element(button_texture), "Game 1", [this]() {
            this->save_selected = 1;
        }),
        Button(UI_Element(button_texture), "Game 2", [this]() {
            this->save_selected = 2;
        }),
        Button(UI_Element(button_texture), "Game 3", [this]() {
            this->save_selected = 3;
        }),
        Button(UI_Element(button_texture), "Load", [this]() {
            if (this->save_selected > 0) {
                char filename[128];
                snprintf(filename, sizeof(filename), "saves/save_%d.save", this->save_selected);
                this->game->load_world(filename);
                this->save_loaded = this->save_selected;
                this->menu = MenuState::MainMenu;
                this->game->paused = false;
            }
        })
    };

    menu = MenuState::MainMenu;
}

void MainUI::iterate(InputState& input, int width, int height) {
    // Set crosshair position to center of screen
    crosshair.location = ivec2(width/2, height/2) - crosshair.size / 2;
    int hotbar_size_multiple = (int)(width/(float)163/2 + 0.5);
    UI_Element& hotbar = hotbar_menu.background.value();
    hotbar.size = ivec2(163, 21) * hotbar_size_multiple;
    hotbar.location = ivec2(width/2 - hotbar.size.x/2, height - 15 - hotbar.size.y);

    for(int i = 0; i <= 8; i++){
        UI_Element& elem = hotbar_menu.buttons[i].elem;
        elem.size = ivec2(8, 8)*hotbar_size_multiple;
        elem.location = hotbar.location + ivec2(6 + 18*i, 6)*hotbar_size_multiple;
    }

    hotbar_selected.size = ivec2(22, 21)*hotbar_size_multiple;
    hotbar_selected.location = hotbar_menu.buttons[game->player.hand].elem.location - ivec2(7, 7)*hotbar_size_multiple;

    auto position_page = [width, height](PageUI& page) {
        // Position the buttons of the given page
        ivec2 button_size = ivec2(500, 65);
        ivec2 top_button = ivec2(width/2, height/4) - button_size/2;
        for(int i = 0; i < (int)page.buttons.size(); i++) {
            UI_Element& elem = page.buttons.at(i).elem;
            elem.location = top_button + i*ivec2(0, button_size.y + 35);
            elem.size = button_size;
        }
    };

    // Get and position visible page
    PageUI& visible_page = menu == MenuState::MainMenu ? main_menu
                        : (menu == MenuState::SaveMenu ? save_menu
                        : load_menu);
    position_page(visible_page);

    if (input.left_mouse == InputButtonState::PRESS && game->paused == true) {
        visible_page.click(input.mouse_pos);
    }

    // Page may have changed after click event, so position the new page
    PageUI& new_visible_page = menu == MenuState::MainMenu ? main_menu
                : (menu == MenuState::SaveMenu ? save_menu
                : load_menu);
    position_page(new_visible_page);

    if (menu == MenuState::SaveMenu || menu == MenuState::LoadMenu) {
        new_visible_page.buttons[1].elem.texture = button_texture;
        new_visible_page.buttons[2].elem.texture = button_texture;
        new_visible_page.buttons[3].elem.texture = button_texture;
        if (save_selected > 0) {
            new_visible_page.buttons[save_selected].elem.texture = button_selection_texture;
        }
    }
    
    // Save screen width and height
    screen = ivec2(width, height);
}

void MainUI::render() {
    crosshair.render();
	TextureRenderer::render_text(font, ivec2(screen.x / 80, screen.y - screen.y / 80), 0.3, "VoxelCraft v0.1.0", ivec3(240, 0, 0));
   
    // Render buttons on the visible page
    if(game->paused) {
        PageUI& visible_page = menu == MenuState::MainMenu ? main_menu
                            : (menu == MenuState::SaveMenu ? save_menu
                            : load_menu);
        visible_page.render();
    } else {
        hotbar_menu.background.value().render();
        hotbar_selected.render();
        hotbar_menu.render();
    }
}
