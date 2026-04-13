#include "ctype.h"
#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#include "../platform/platform_win32.h"
#include "raygui.h"
#include "raylib.h"
#include "ui_widgets.h"

void styling() {
  // base
  GuiSetStyle(DEFAULT, BACKGROUND_COLOR, 0x0F1115FF);
  GuiSetStyle(DEFAULT, BASE_COLOR_NORMAL, 0x1A1D23FF);
  GuiSetStyle(DEFAULT, BASE_COLOR_FOCUSED, 0x22262EFF);
  GuiSetStyle(DEFAULT, BASE_COLOR_PRESSED, 0x2A2F3AFF);
  GuiSetStyle(DEFAULT, BORDER_COLOR_NORMAL, 0x2C2F38FF);
  GuiSetStyle(DEFAULT, BORDER_COLOR_FOCUSED, 0xA78BFAFF);
  GuiSetStyle(DEFAULT, TEXT_COLOR_NORMAL, 0xE6E6E6FF);
  GuiSetStyle(DEFAULT, TEXT_COLOR_FOCUSED, 0xFFFFFFFF);
  GuiSetStyle(DEFAULT, TEXT_COLOR_DISABLED, 0x555A66FF);
  GuiSetStyle(DEFAULT, BORDER_WIDTH, 1);
  GuiSetStyle(DEFAULT, TEXT_PADDING, 6);
  GuiSetStyle(DEFAULT, TEXT_SIZE, 13);

  // status bar / window title
  GuiSetStyle(STATUSBAR, BACKGROUND_COLOR, 0x13161BFF);
  GuiSetStyle(STATUSBAR, TEXT_COLOR_NORMAL, 0xA78BFAFF);
  GuiSetStyle(STATUSBAR, BORDER_WIDTH, 1);

  // buttons — flat, minimal
  GuiSetStyle(BUTTON, BASE_COLOR_NORMAL, 0x1E2128FF);
  GuiSetStyle(BUTTON, BASE_COLOR_FOCUSED, 0x252932FF);
  GuiSetStyle(BUTTON, BASE_COLOR_PRESSED, 0xA78BFA33);
  GuiSetStyle(BUTTON, BORDER_COLOR_NORMAL, 0x2C2F38FF);
  GuiSetStyle(BUTTON, BORDER_COLOR_FOCUSED, 0xA78BFAFF);
  GuiSetStyle(BUTTON, TEXT_COLOR_NORMAL, 0xC4C4C4FF);
  GuiSetStyle(BUTTON, TEXT_COLOR_FOCUSED, 0xFFFFFFFF);
  GuiSetStyle(BUTTON, TEXT_COLOR_PRESSED, 0xA78BFAFF);

  // sliders — accent on active
  GuiSetStyle(SLIDER, BASE_COLOR_NORMAL, 0x1E2128FF);
  GuiSetStyle(SLIDER, BASE_COLOR_FOCUSED, 0xA78BFA55);
  GuiSetStyle(SLIDER, BASE_COLOR_PRESSED, 0xA78BFAFF);
  GuiSetStyle(SLIDER, BORDER_COLOR_NORMAL, 0x2C2F38FF);
  GuiSetStyle(SLIDER, BORDER_COLOR_FOCUSED, 0xA78BFAFF);

  // textboxes
  GuiSetStyle(TEXTBOX, BASE_COLOR_NORMAL, 0x13161BFF);
  GuiSetStyle(TEXTBOX, BASE_COLOR_FOCUSED, 0x1A1D23FF);
  GuiSetStyle(TEXTBOX, BORDER_COLOR_NORMAL, 0x2C2F38FF);
  GuiSetStyle(TEXTBOX, BORDER_COLOR_FOCUSED, 0xA78BFAFF);
  GuiSetStyle(TEXTBOX, TEXT_COLOR_NORMAL, 0xE6E6E6FF);

  // checkboxes
  GuiSetStyle(CHECKBOX, BASE_COLOR_NORMAL, 0x1E2128FF);
  GuiSetStyle(CHECKBOX, BASE_COLOR_FOCUSED, 0x252932FF);
  GuiSetStyle(CHECKBOX, BASE_COLOR_PRESSED, 0xA78BFA33);

  GuiSetStyle(CHECKBOX, BORDER_COLOR_NORMAL, 0x2C2F38FF);
  GuiSetStyle(CHECKBOX, BORDER_COLOR_FOCUSED, 0xA78BFAFF);

  GuiSetStyle(CHECKBOX, TEXT_COLOR_NORMAL, 0xA78BFAFF);
  GuiSetStyle(CHECKBOX, TEXT_COLOR_FOCUSED, 0xC4B5FDFF);
  GuiSetStyle(CHECKBOX, TEXT_COLOR_PRESSED, 0xA78BFAFF);
}
void UiSlider(UiSliderState *state, float *val, float min, float max,
              const char *text, const char *righttext, int row,
              Rectangle content, bool percentage_mode) {
  if (percentage_mode) {
    snprintf(state->label, sizeof(state->label), "%s = %.2f%%", text,
             (*val) * 100.0f);
  }
  if (!percentage_mode) {
    snprintf(state->label, sizeof(state->label), "%s = %.2f", text, (*val));
  }
  if (righttext != NULL) {
    snprintf(state->right_label, sizeof(state->right_label), "%s ", righttext);
  } else {
    state->right_label[0] = '\0';
  }

  float y = content.y + row * 60;

  GuiLabel((Rectangle){content.x, y, 300, 20}, state->label);
  GuiSlider((Rectangle){content.x, y + 25, content.width, 20},
            state->right_label, NULL, val, min, max);
}

void toggle_menu(bool *menu) {
  if (!IsKeyPressed(KEY_M))
    return;
  *menu = !(*menu);
}

bool UiTextInput(Rectangle content, char *text, int textSize) {
  static bool edit = false;

  if (GuiTextBox((Rectangle){content.x, content.y + 3 * 60, content.width, 20},
                 text, textSize, edit)) {
    edit = !edit;

    return true;
  }

  return false;
}

// NOTE: empty textbox snaps back to last value (UX tradeoff, acceptable for
// now)
bool UiFloatInput(float *value, UiFloatState *state, Rectangle content, int row,
                  float min, float max) {
  state->dirty = false;

  if (state->buff[0] == '\0') {
    snprintf(state->buff, sizeof(state->buff), "%.3f", *value);
    state->last_value = *value;
  }

  if (GuiTextBox(
          (Rectangle){content.x, content.y + (row + 4) * 60, content.width, 20},
          state->buff, sizeof(state->buff), state->edit)) {
    state->edit = !state->edit;

    char *end;
    float parse_buff = strtof(state->buff, &end);

    // Bounds
    if (parse_buff < min)
      parse_buff = min;
    if (parse_buff > max)
      parse_buff = max;

    *value = parse_buff;

    if (parse_buff != state->last_value) {
      *value = parse_buff;
      state->last_value = parse_buff;
      state->dirty = true;
    }
  }

  return state->dirty;
}

// NOTE: empty textbox snaps back to last value (UX tradeoff, acceptable for
// now)
bool UiIntInput(int *value, UiIntState *state, Rectangle content, int row,
                int min, int max) {
  state->dirty = false;

  if (state->buff[0] == '\0') {
    snprintf(state->buff, sizeof(state->buff), "%d", *value);
    state->last_value = *value;
  }

  if (GuiTextBox(
          (Rectangle){content.x, content.y + (row + 4) * 60, content.width, 20},
          state->buff, sizeof(state->buff), state->edit)) {
    state->edit = !state->edit;

    char *end;
    int parse_buff = strtol(state->buff, &end, 10);

    // Bounds
    if (parse_buff < min)
      parse_buff = min;
    if (parse_buff > max)
      parse_buff = max;

    *value = parse_buff;

    if (parse_buff != state->last_value) {
      *value = parse_buff;
      state->last_value = parse_buff;
      state->dirty = true;
    }
  }

  return state->dirty;
}

bool UiFilePicker(Rectangle content, const char *label, char *path,
                  int path_size) {
  if (GuiButton((Rectangle){content.x, content.y + 4 * 60, content.width, 20},
                label)) {
    return platfrom_open_file(path, path_size);
  }
  return false;
}
