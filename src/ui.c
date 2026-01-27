#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "raylib.h"

#include "ui.h"
#include "world.h"
#include "planet.h"

typedef enum {
    PANEL_GLOBAL,
    PANEL_PLANET,
    PANEL_CREATE
} UiPanel;

static void styling() {
    // Global
    GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x1E1E1ECC);
    GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0x25252FF);
    GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0x25252FF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xDADADAFF);
    GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED, 0x7A7A7AFF);

    // Window / status bar
    GuiSetStyle(STATUSBAR, BACKGROUND_COLOR, 0x2A2A2AFF);
    GuiSetStyle(STATUSBAR, TEXT_COLOR_NORMAL, 0xFFFFFFFF);
    GuiSetStyle(STATUSBAR, BORDER_WIDTH, 1);

    // Buttons
    GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x313131FF);
    GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x3E3E3EFF);
    GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0x4A4A4AFF);
    GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0x5A5A5AFF);
    GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xFFFFFFFF);

    // Sliders
    GuiSetStyle(SLIDER, BASE_COLOR_NORMAL, 0x313131FF);
    GuiSetStyle(SLIDER, BASE_COLOR_FOCUSED, 0x3E3E3EFF);
    GuiSetStyle(SLIDER, BORDER_COLOR_NORMAL, 0x5A5A5AFF);

    // Textboxes
    GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, 0x1E1E1EFF);
    GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, 0x5A5A5AFF);
    GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, 0xFFFFFFFF);

    // Checkboxes
    GuiSetStyle(CHECKBOX, BASE_COLOR_NORMAL, 0x313131FF);
    GuiSetStyle(CHECKBOX, BORDER_COLOR_NORMAL, 0x5A5A5AFF);

    // General spacing
    GuiSetStyle(DEFAULT, BORDER_WIDTH, 1);
    GuiSetStyle(DEFAULT, TEXT_PADDING, 6);

}

static void slider(float *val, float min, float max, char text[], char righttext[], int row, Rectangle content, bool percentage_mode) {
    char label[128];
    if(percentage_mode) {
        snprintf(label, sizeof(label),"%s = %.2f%%", text, (*val) * 100);
    }
    if(!percentage_mode) {
        snprintf(label, sizeof(label),"%s = %.2f", text, (*val));
    }
    char right_label[128] = "";
    if(righttext != NULL) {
        snprintf(right_label, sizeof(right_label),"%s ", righttext);
    }

    float y = content.y + row * 60;
    
    GuiLabel((Rectangle){content.x, y, 300, 20}, label);
    GuiSlider((Rectangle){content.x, y + 25, content.width, 20}, right_label, NULL, val, min, max);

}

static void toggle_menu(bool *menu) {
    if (!IsKeyPressed(KEY_M)) return;
    *menu = !(*menu);
}

void ui(World *world){
    styling();

    Rectangle window = {
        GetScreenWidth() - 300,
        0, 
        300, 
        GetScreenHeight()
    };

    static bool windowOpen = true;
    static bool menuActive = true;

    toggle_menu(&menuActive);

    Rectangle content = {
        window.x + 10,
        window.y + 30,
        window.width - 20,
        window.height - 40
    };
    if(menuActive) {
        if(!windowOpen) menuActive = false;
        windowOpen = !GuiWindowBox(window, "Simulation Contorls");
        
        static UiPanel currentPanel = PANEL_GLOBAL;
        static int selectedPlanet = -1;
        if (GuiButton((Rectangle){content.x, content.y, content.width, 30}, "Global Settings")) {
            currentPanel = PANEL_GLOBAL;
        }
        if (GuiButton((Rectangle){content.x, content.y, content.width, 30}, "Global Settings")) {
            currentPanel = PANEL_GLOBAL;
        }
        float y = content.y + 40;
        for (int i = 0; i < world->planet_count; i++) {
            if (GuiButton((Rectangle){content.x, y, content.width, 30}, world->planets[i].name)) {
                currentPanel = PANEL_PLANET;
                selectedPlanet = i;
            }
            y += 35;
        }

        if(currentPanel == PANEL_GLOBAL) {
            slider(&world->gravity_strength, 30.0f, 100.0f, "Gravity", NULL, 3, content, false);
            slider(&world->deltaTime, 0.0f, 2.0f, "Time Speed", NULL, 4, content, true);
            
        }

        world->count_g = 0;
        world->count_ng = 0;

        for (int i = 0; i < world->planet_count; i++) {
            planet_update(&world->planets[i]); // This is visual only
            if(world->planets[i].has_gravity == true) {
                world->gravity_planet_indexs[world->count_g++] = i;
            }

            if(world->planets[i].has_gravity == false) {
                world->non_gravity_planet_indexs[world->count_ng++] = i;
            }
        }

        if(currentPanel == PANEL_PLANET && selectedPlanet != -1) {
            Planet *p = &world->planets[selectedPlanet];

            float last_radius = p->radius;

            slider(&p->mass, 0.0f, 10000.0f, p->name, "mass", 3, content, false);
            slider(&p->radius, 0.0f, 1000.0f, p->name, "radius", 4, content, false);

            if(last_radius != p->radius) {
                remake_model(p);
            }
        }
    }
}