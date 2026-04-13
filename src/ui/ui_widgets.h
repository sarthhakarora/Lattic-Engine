#pragma once

typedef struct {
    char buff[32];
    bool edit;      // Note: edit is for when the user clicks on the textbox
    bool dirty;     // Note: dirty is for when the user changes the value 
                    //       it is called dirty because the engine has not reacted yet
                    //       and dirty implies that is is temp and needs handeling
    float last_value;
} UiFloatState;

typedef struct {
    char buff[32];
    bool edit;      // Note: edit is for when the user clicks on the textbox
    bool dirty;     // Note: dirty is for when the user changes the value 
                    //       it is called dirty because the engine has not reacted yet
                    //       and dirty implies that is is temp and needs handeling
    int last_value;
} UiIntState;

typedef struct {
    char label[128];
    char right_label[128];
} UiSliderState;


void styling();
void UiSlider(UiSliderState *state, float *val, float min, float max, const char* text, const char* righttext, int row, Rectangle content, bool percentage_mode);
void toggle_menu(bool *menu);
bool check_validation_text(const char *s, size_t max_len);
bool UiTextInput(Rectangle content, char *text, int textSize);
bool UiFloatInput(float *value, UiFloatState *state, Rectangle content, int row, float min, float max);
bool UiIntInput(int *value, UiIntState *state, Rectangle content, int row, int min, int max);
bool UiFilePicker(Rectangle content, const char* label, char* path, int path_size);