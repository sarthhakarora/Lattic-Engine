#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "raylib.h"
#include "ctype.h"
#include "stdio.h"
#include "stdlib.h"
#include "../platform/platform_win32.h"

#include "ui_widgets.h"
#include "ui.h"
#include "../world.h"
#include "../planet.h"

#define GUI_TEXT_MAX_LENGTH 255

typedef enum {
    PANEL_GLOBAL,
    PANEL_PLANET,
    PANEL_CREATE
} UiPanel;

bool check_validation_text(const char *s, size_t max_len)
{
    // Null check
    if (!s) return false;

    // Length check
    size_t len = strlen(s);
    if (len == 0 || len >= max_len) return false;

    for (; *s; s++) {
        // Printable check
        if (!isprint((unsigned char)*s)) return false;

        // Whitespace only check
        if (!isspace((unsigned char)*s)) return true;

    }

    return false;
}

void ui(World *world){
    // ui
    styling(); //for now
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
        static bool createMode = false;
        
        static UiPanel currentPanel = PANEL_GLOBAL;
        static int selectedPlanet = -1;
        if (GuiButton((Rectangle){content.x, content.y, content.width, 30}, "Global Settings")) {
            currentPanel = PANEL_GLOBAL;
        }

        float y = content.y + 40;
        for (int i = 0; i < world->planet_count; i++) {
            if (GuiButton((Rectangle){content.x, y, content.width, 30}, world->planets[i]->name)) {
                currentPanel = PANEL_PLANET; selectedPlanet = i; 

            } 
            y += 35; 
            } 
            if(currentPanel == PANEL_GLOBAL) { 
                createMode = false;
                static UiSliderState gravity = {0};
                static UiSliderState time = {0};

                UiSlider(&gravity, &world->gravity_strength, 30.0f, 100.0f, "Gravity", NULL, 3, content, false);
                UiSlider(&time, &world->timeScale, 0.0f, 2.0f, "Time Speed", NULL, 4, content, true); 

                if(GuiButton((Rectangle){content.x, content.y + 5 * 60, content.width, 20}, "Create Planet")) { 
                    currentPanel = PANEL_CREATE; 
                } 
            } 
            if(currentPanel == PANEL_CREATE) { 
                createMode = true; // Name and Texture path
            static unsigned char name[255] = "";
            static unsigned char texturepath[1024] = "";

            // Properties
            static int rings = 0;
            static int slices = 0;
            static float mass = 0;
            static float radius = 0;
            static bool has_gravity = false;

            // Planet position
            static float x = 0.0f;
            static float y = 0.0f;
            static float z = 0.0f;

            // State handeling
            static UiFloatState x_ui = {0};
            static UiFloatState y_ui = {0};
            static UiFloatState z_ui = {0};
            static UiIntState rings_ui = {0};
            static UiIntState slices_ui = {0};
            static UiFloatState radius_ui = {0};
            static UiFloatState mass_ui = {0};

            // Name
            if(UiTextInput(content, name, 255)) {
                if(!check_validation_text(name, 255 - 1)) return;
                printf("---------------------\ninput given -> %s\n---------------------", name);
            }
            
            // Texture Path
            UiFilePicker(content, "Select Texture", texturepath, 1024);
            
            // Postion Vector
            UiFloatInput(&x, &x_ui, content, 1, -1000, 1000);
            UiFloatInput(&y, &y_ui, content, 2, -1000, 1000);
            UiFloatInput(&z, &z_ui, content, 3, -1000, 1000);

            // Quality -> in rings and slices
            UiIntInput(&rings, &rings_ui, content, 4, 1, 256);
            UiIntInput(&slices, &slices_ui, content, 5, 1, 256);

            // Radius
            UiFloatInput(&radius, &radius_ui, content, 6, 1, 1000);

            // Mass
            UiFloatInput(&mass, &mass_ui, content, 7, 1, 1000);

            if(IsKeyPressed(KEY_G)) {
                Planet *buffer = malloc(sizeof(Planet));
                if(!buffer) {
                    platform_throw_error("Out Of Memory", "Memory error", PLATFORM_SYSTEM_MODAL|PLATFORM_MSG_OK);
                }
                *buffer = planet_create(name, texturepath, radius, (Vector3){x, y, z}, rings, slices, mass, has_gravity);

             
                if(!world_add_planet(buffer, world)) {
                    platform_throw_error("Ran out of memory while trying to make planet", "Memory error", PLATFORM_SYSTEM_MODAL|PLATFORM_MSG_OK);
                    return;
                }
            }
          
        }

        if(currentPanel == PANEL_PLANET && selectedPlanet != -1) {
            Planet *p = world->planets[selectedPlanet];
            createMode = false;

            float last_radius = p->radius;

            static UiSliderState planet_mass = {0};
            static UiSliderState planet_radius = {0};

            UiSlider(&planet_mass, &p->mass, 0.0f, 10000.0f, p->name, "mass", 3, content, false);
            UiSlider(&planet_radius, &p->radius, 0.0f, 1000.0f, p->name, "radius", 4, content, false);

            if(last_radius != p->radius) {
                remake_model(p);
            }
        }
    }
}