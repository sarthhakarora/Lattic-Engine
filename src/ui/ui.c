#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#include "../platform/platform_win32.h"
#include "ctype.h"
#include "raylib.h"
#include "stdio.h"
#include "stdlib.h"

#include "../planet.h"
#include "../world.h"
#include "ui.h"
#include "ui_widgets.h"

#define BUTTON_H 28
#define BUTTON_GAP 6

// ------------------------------------------------------------
// simple validation
// ------------------------------------------------------------
static bool valid_text(const char *s) {
  if (!s)
    return false;
  if (strlen(s) == 0)
    return false;
  for (; *s; s++) {
    if (!isprint((unsigned char)*s))
      return false;
  }
  return true;
}

// ------------------------------------------------------------
// layout helper
// ------------------------------------------------------------
typedef struct {
  Rectangle bounds;
  float y;
} Layout;

static void layout_begin(Layout *l, Rectangle area) {
  l->bounds = area;
  l->y = area.y;
}

static Rectangle layout_row(Layout *l, float h) {
  Rectangle r = {l->bounds.x, l->y, l->bounds.width, h};
  l->y += h + BUTTON_GAP;
  return r;
}

static void ui_slider(Layout *l, const char *label, float *value, float min,
                      float max) {
  GuiLabel(layout_row(l, 18), TextFormat("%s = %.2f", label, *value));
  GuiSlider(layout_row(l, 20), NULL, NULL, value, min, max);
}

static void ui_slider_int(Layout *l, const char *label, int *value, int min,
                          int max) {
  float v = (float)*value;
  GuiLabel(layout_row(l, 18), TextFormat("%s = %d", label, *value));
  GuiSlider(layout_row(l, 20), NULL, NULL, &v, min, max);
  *value = (int)v;
}

// ------------------------------------------------------------
// UI
// ------------------------------------------------------------
static void ui_planet_inspector(Layout *layout, World *world) {
  static int selected = -1;
  static bool dropdownOpen = false;
  static char search[64] = "";

  if (!world || world->planet_count == 0)
    return;

  if (selected < 0)
    selected = 0;
  if (selected >= world->planet_count)
    selected = world->planet_count - 1;

  Planet *current = world->planets[selected];
  if (!current)
    return;

  GuiLabel(layout_row(layout, 18), "Select Planet");

  // --- SELECTOR ---
  Rectangle row = layout_row(layout, 24);

  float btnW = 40;
  float gap = 4;

  Rectangle prevBtn = {row.x, row.y, btnW, row.height};
  Rectangle nextBtn = {row.x + btnW + gap, row.y, btnW, row.height};
  Rectangle nameBtn = {row.x + 2 * (btnW + gap), row.y,
                       row.width - 2 * (btnW + gap), row.height};

  // prev
  if (GuiButton(prevBtn, "<")) {
    selected--;
    if (selected < 0)
      selected = world->planet_count - 1;
  }

  // next
  if (GuiButton(nextBtn, ">")) {
    selected = (selected + 1) % world->planet_count;
  }

  // dropdown toggle
  if (GuiButton(nameBtn, world->planets[selected]->name)) {
    dropdownOpen = !dropdownOpen;
  }

  // --- SEARCH BAR ---

  static bool editSearch = false;

  Rectangle searchRect = layout_row(layout, 24);

  if (GuiTextBox(searchRect, search, 64, editSearch)) {
    editSearch = !editSearch;
  }

  if (editSearch && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
      !CheckCollisionPointRec(GetMousePosition(), searchRect)) {
    editSearch = false;
  }

  // --- DROPDOWN LIST ---
  if (dropdownOpen) {
    int shown = 0;

    for (int i = 0; i < world->planet_count; i++) {
      Planet *p = world->planets[i];
      if (!p)
        continue;

      // filter by search
      if (strlen(search) > 0 && !strstr(p->name, search)) {
        continue;
      }

      // cap list (prevents UI explosion)
      if (shown >= 10)
        break;

      if (GuiButton(layout_row(layout, 22), p->name)) {
        selected = i;
        dropdownOpen = false;
      }

      shown++;
    }

    // optional: show hint if truncated
    if (shown >= 10) {
      GuiLabel(layout_row(layout, 18), "...more results");
    }
  }

  // --- INSPECTOR ---
  Planet *p = world->planets[selected];
  if (!p)
    return;

  GuiLabel(layout_row(layout, 18), "Edit Planet");

  ui_slider(layout, "Mass", &p->mass, 1, 10000);

  float old_radius = p->radius;
  ui_slider(layout, "Radius", &p->radius, 1, 500);
  if (old_radius != p->radius) {
    remake_model(p);
  }

  ui_slider(layout, "Pos X", &p->position.x, -500, 500);
  ui_slider(layout, "Pos Y", &p->position.y, -500, 500);
  ui_slider(layout, "Pos Z", &p->position.z, -500, 500);
}

void ui(World *world) {
  styling();

  static bool texturePickerOpen = false;

  if (texturePickerOpen) {
    GuiLock();
    DisableCursor();
  }

  Rectangle window = {GetScreenWidth() - 320, 0, 320, GetScreenHeight()};

  static bool windowOpen = true;
  static bool menuActive = true;

  toggle_menu(&menuActive);

  if (!menuActive) {
    if (texturePickerOpen) {
      GuiUnlock();
    }
    return;
  }
  if (!windowOpen)
    menuActive = false;

  windowOpen = !GuiWindowBox(window, "Simulation Controls");

  Rectangle content = {window.x + 10, window.y + 35, window.width - 20,
                       window.height - 45};

  Layout layout;
  layout_begin(&layout, content);

  // ------------------------------------------------------------
  // GLOBAL
  // ------------------------------------------------------------

  GuiLabel(layout_row(&layout, 18),
           TextFormat("Gravity = %.2f", world->gravity_strength));
  GuiSlider(layout_row(&layout, 20), NULL, NULL, &world->gravity_strength,
            30.0f, 100.0f);

  GuiLabel(layout_row(&layout, 18),
           TextFormat("Time Scale = %.2f", world->timeScale));
  GuiSlider(layout_row(&layout, 20), NULL, NULL, &world->timeScale, 0.0f, 3.0f);

  // ------------------------------------------------------------
  // RENDER
  // ------------------------------------------------------------
  GuiLabel(layout_row(&layout, 18), "Link Mode");

  int current = world->linkMode;

  if (GuiButton(layout_row(&layout, 24),
                current == LINKS_OFF ? "> Off" : "Off")) {
    world->linkMode = LINKS_OFF;
  }
  if (GuiButton(layout_row(&layout, 24),
                current == LINKS_NEAREST ? "> Nearest" : "Nearest")) {
    world->linkMode = LINKS_NEAREST;
  }

  if (GuiButton(layout_row(&layout, 24),
                current == LINKS_STRONGEST ? "> Strongest" : "Strongest")) {
    world->linkMode = LINKS_STRONGEST;
  }

  if (GuiButton(layout_row(&layout, 24),
                current == LINKS_ALL ? "> All" : "All")) {
    world->linkMode = LINKS_ALL;
  }
  // ------------------------------------------------------------
  // CREATE PLANET
  // ------------------------------------------------------------

  GuiLabel(layout_row(&layout, 20), "Create Planet");

  static char name[128] = "planet";
  static char texture[256] = "";

  static float px = 0, py = 0, pz = 0;
  static float mass = 10.0f;
  static float radius = 10.0f;
  static int rings = 8;
  static int slices = 16;
  static bool has_gravity = false;

  static bool editName = false;
  static Rectangle nameRect;

  nameRect = layout_row(&layout, 24);

  if (GuiTextBox(nameRect, name, 128, editName)) {
    editName = !editName;
  }

  if (editName && IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
      !CheckCollisionPointRec(GetMousePosition(), nameRect)) {
    editName = false;
  }
  layout_row(&layout, 8); // spacing

  GuiLabel(layout_row(&layout, 18), "Texture");

  Rectangle fileRect = layout_row(&layout, 28); // not 19

  if (GuiButton(fileRect, texture[0] ? texture : "Select Texture")) {
    texturePickerOpen = true;
  }

  // position

  layout_row(&layout, 10);
  GuiLabel(layout_row(&layout, 18), "Position");

  ui_slider(&layout, "X", &px, -500, 500);
  ui_slider(&layout, "Y", &py, -500, 500);
  ui_slider(&layout, "Z", &pz, -500, 500);

  ui_slider(&layout, "Mass", &mass, 1, 10000);
  ui_slider(&layout, "Radius", &radius, 1, 500);

  ui_slider_int(&layout, "Rings", &rings, 3, 64);
  ui_slider_int(&layout, "Slices", &slices, 3, 64);

  if (GuiButton(layout_row(&layout, 24),
                has_gravity ? "Gravity: ON" : "Gravity: OFF")) {
    has_gravity = !has_gravity;
  }
  // ------------------------------------------------------------
  // CREATE BUTTON
  // ------------------------------------------------------------

  if (GuiButton(layout_row(&layout, 28), "Spawn Planet")) {

    if (!valid_text(name))
      return;

    Planet *p = (Planet *)malloc(sizeof(Planet));
    if (!p) {
      platform_throw_error("Out Of Memory", "Memory error",
                           PLATFORM_SYSTEM_MODAL | PLATFORM_MSG_OK);
      return;
    }

    *p = planet_create(name, texture, radius, (Vector3){px, py, pz}, rings,
                       slices, mass, has_gravity);

    if (!world_add_planet(p, world)) {
      platform_throw_error("Failed to add planet", "Error",
                           PLATFORM_SYSTEM_MODAL | PLATFORM_MSG_OK);
      free(p);
    }
  }

  // ------------------------------------------------------------
  // CREATE BUTTON
  // ------------------------------------------------------------

  layout_row(&layout, 10);
  GuiLine(layout_row(&layout, 2), NULL);
  layout_row(&layout, 10);

  ui_planet_inspector(&layout, world);

  // ------------------------------------------------------------
  // DEBUG INFO
  // ------------------------------------------------------------

  char buf[128];
  snprintf(buf, sizeof(buf), "Planets: %d", world->planet_count);
  DrawText(buf, window.x + 10, window.y + window.height - 30, 12, GRAY);

  if (texturePickerOpen) {
    Rectangle popup = {window.x - 300, window.y + 30, 300, 420};

    DrawRectangleRec(popup, (Color){15, 17, 21, 240});
    DrawRectangleLinesEx(popup, 1, (Color){0xA7, 0x8B, 0xFA, 255});

    if (UiFilePicker(popup, "Select Texture", (unsigned char *)texture, 256)) {
      texturePickerOpen = false;
    }

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) &&
        !CheckCollisionPointRec(GetMousePosition(), popup)) {
      texturePickerOpen = false;
    }

    GuiUnlock();
    EnableCursor();
  }
}
