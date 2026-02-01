#include "stdio.h"
#include "stdlib.h"
#include "ctype.h"
#include "string.h"

#include "raylib.h"
#include "raygui.h"
#include "../platform/platform_win32.h"
#include "ui_widgets.h"

void styling() {
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

void UiSlider(float *val, float min, float max, char text[], char righttext[], int row, Rectangle content, bool percentage_mode) {
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

void toggle_menu(bool *menu) {
    if (!IsKeyPressed(KEY_M)) return;
    *menu = !(*menu);
}


bool UiTextInput(Rectangle content, char *text, int textSize)
{
    static bool edit = false;

    if(GuiTextBox((Rectangle){content.x, content.y + 3 * 60, content.width, 20}, text, textSize, edit)) {
        edit = !edit;

        return true;
    }

    return false;
}

// NOTE: empty textbox snaps back to last value (UX tradeoff, acceptable for now)
bool UiFloatInput(float *value, UiFloatState *state, Rectangle content, int row, float min, float max) {
    state->dirty = false;

    if(state->buff[0] == '\0'){
        snprintf(state->buff, sizeof(state->buff), "%.3f", *value);
        state->last_value = *value;
    }


    if(GuiTextBox((Rectangle){content.x, content.y + (row + 4) * 60, content.width, 20}, state->buff, sizeof(state->buff), state->edit)) {
        state->edit = !state->edit;

        char *end;
        float parse_buff = strtof(state->buff, &end);

        // Bounds
        if(parse_buff < min) parse_buff = min;
        if(parse_buff > max) parse_buff = max;

        *value = parse_buff;

        if(parse_buff != state->last_value) {
            *value = parse_buff;
            state->last_value = parse_buff;
            state->dirty = true;
        }

    }

    return state->dirty;
}

// NOTE: empty textbox snaps back to last value (UX tradeoff, acceptable for now)
bool UiIntInput(int *value, UiIntState *state, Rectangle content, int row, int min, int max)
{
    state->dirty = false;

    if(state->buff[0] == '\0'){
        snprintf(state->buff, sizeof(state->buff), "%d", *value);
        state->last_value = *value;
    }


    if(GuiTextBox((Rectangle){content.x, content.y + (row + 4) * 60, content.width, 20}, state->buff, sizeof(state->buff), state->edit)) {
        state->edit = !state->edit;

        char *end;
        int parse_buff = strtol(state->buff, &end, 10);

        // Bounds
        if(parse_buff < min) parse_buff = min;
        if(parse_buff > max) parse_buff = max;

        *value = parse_buff;
        
        if(parse_buff != state->last_value) {
            *value = parse_buff;
            state->last_value = parse_buff;
            state->dirty = true;
        }

    }

    return state->dirty;

}

bool UiFilePicker(Rectangle content, const char *label, char *path, int path_size)
{
    if(GuiButton((Rectangle){content.x, content.y + 4 * 60, content.width, 20}, label)) {
        return platfrom_open_file(path, path_size);
    }
    return false;

}