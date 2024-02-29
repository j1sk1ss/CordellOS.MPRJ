#include "../include/graphics.h"


//====================================================================
// Function put pixel by coordinates
// EBX - x
// ECX - y
// EDX - pixel data
void put_pixel(int x, int y, int color) {
    __asm__ volatile(
        "movl $28, %%eax\n"
        "movl %0, %%ebx\n"
        "movl %1, %%ecx\n"
        "movl %2, %%edx\n"
        "int $0x80\n"
        :
        : "r"(x), "r"(y), "r"(color)
        : "eax", "ebx", "ecx"
    );
}

//====================================================================
// Function directly in video memory put pixel by coordinates
// EBX - x
// ECX - y
// EDX - pixel data
void vput_pixel(int x, int y, int color) {
    __asm__ volatile(
        "movl $37, %%eax\n"
        "movl %0, %%ebx\n"
        "movl %1, %%ecx\n"
        "movl %2, %%edx\n"
        "int $0x80\n"
        :
        : "r"(x), "r"(y), "r"(color)
        : "eax", "ebx", "ecx"
    );
}

//====================================================================
// Function get pixel from framebuffer by coordinates
// EBX - x
// ECX - y
// EDX - result
int get_pixel(int x, int y) {
    int result;
    __asm__ volatile(
        "movl $29, %%eax\n"
        "movl %0, %%ebx\n"
        "movl %1, %%ecx\n"
        "movl %2, %%edx\n"
        "int $0x80\n"
        :
        : "r"(x), "r"(y), "r"(result)
        : "eax", "ebx", "ecx"
    );

    return result;
}

//====================================================================
// Function get pixel from framebuffer by coordinates
// EDX - result
int get_resolution_x() {
    int result;
    __asm__ volatile(
        "movl $31, %%eax\n"
        "movl $0, %%ebx\n"
        "movl $1, %%ecx\n"
        "movl %0, %%edx\n"
        "int $0x80\n"
        :
        : "r"(&result)
        : "eax", "ebx", "ecx"
    );

    return result;
}

//====================================================================
// Function get pixel from framebuffer by coordinates
// EDX - result
int get_resolution_y() {
    int result;
    __asm__ volatile(
        "movl $32, %%eax\n"
        "movl $0, %%ebx\n"
        "movl $1, %%ecx\n"
        "movl %0, %%edx\n"
        "int $0x80\n"
        :
        : "r"(&result)
        : "eax", "ebx", "ecx"
    );

    return result;
}

//====================================================================
// Function swipe video buffer with second buffer
void swipe_buffers() {
    __asm__ volatile(
        "movl $36, %%eax\n"
        "movl $0, %%ebx\n"
        "movl $1, %%ecx\n"
        "movl $0, %%edx\n"
        "int $0x80\n"
        :
        :
        : "eax", "ebx", "ecx"
    );
}

void display_gui_object(struct GUIobject* object) {
    if (object == NULL) return;
    for (int y = object->height - 1; y >= 0; y--)
        for (int x = 0; x < object->width; x++)
            vput_pixel(x + object->x, y + object->y, object->background_color);

    for (int i = 0; i < object->children_count; i++) 
        display_gui_object(object->childrens[i]);

    for (int i = 0; i < object->bitmap_count; i++) 
        BMP_display(object->bitmaps[i]);

    for (int i = 0; i < object->text_count; i++)
        put_text(object->texts[i]);
}

struct GUIobject* create_gui_object(int x, int y, int height, int width, uint32_t background) {
    struct GUIobject* newObject = (struct GUIobject*)malloc(sizeof(struct GUIobject));
    if (newObject != NULL) {
        newObject->x      = x;
        newObject->y      = y;
        newObject->prev_x = x;
        newObject->prev_y = y;

        newObject->height = height;
        newObject->width  = width;

        newObject->background_color = background;
        newObject->children_count   = 0;
        newObject->childrens        = NULL;
        newObject->bitmap_count     = 0;
        newObject->bitmaps          = NULL;
        newObject->text_count       = 0;
        newObject->texts            = NULL;
    }

    return newObject;
}

void object_move(struct GUIobject* object, int rel_x, int rel_y) {
    if (object == NULL) return;
    for (int i = 0; i < object->children_count; i++)
        object_move(object->childrens[i], rel_x, rel_y);

    for (int i = 0; i < object->bitmap_count; i++) {
        object->bitmaps[i]->x += rel_x;
        object->bitmaps[i]->y += rel_y;
    }

    for (int i = 0; i > object->text_count; i++) {
        object->texts[i]->x += rel_x;
        object->texts[i]->y += rel_y;
    }

    object->x += rel_x;
    object->y += rel_y;
}

struct GUIobject* object_add_children(struct GUIobject* object, struct GUIobject* children) {
    if (object != NULL && children != NULL) {
        object->childrens = (struct GUIobject**)realloc(object->childrens, (object->children_count + 1) * sizeof(struct GUIobject*));
        if (object->childrens != NULL) object->childrens[object->children_count++] = children;
    }

    return object;
}

struct GUIobject* object_add_bitmap(struct GUIobject* object, struct bitmap* bmp) {
    if (object != NULL && bmp != NULL) {
        object->bitmaps = (struct bitmap**)realloc(object->bitmaps, (object->bitmap_count + 1) * sizeof(struct bitmap*));
        if (object->bitmaps != NULL) object->bitmaps[object->bitmap_count++] = bmp;
    }

    return object;
}

struct GUIobject* object_add_text(struct GUIobject* object, struct text_object* text) {
    if (object != NULL && text != NULL) {
        object->texts = (struct text_object**)realloc(object->texts, (object->text_count + 1) * sizeof(struct text_object*));
        if (object->texts != NULL) object->texts[object->text_count++] = text;
    }

    return object;
}

void unload_gui_object(struct GUIobject* object) {
    if (object == NULL) return;

    for (int i = 0; i < object->children_count; i++) unload_gui_object(object->childrens[i]);
    for (int i = 0; i < object->bitmap_count; i++) BMP_unload(object->bitmaps[i]);
    for (int i = 0; i < object->text_count; i++) unload_text(object->texts[i]);

    free(object->childrens);
    free(object->bitmaps);
    free(object->texts);
    free(object);
}

struct text_object* create_text(int x, int y, char* text, uint32_t background_color) {
    struct text_object* object = malloc(sizeof(struct text_object));

    object->x = x;
    object->y = y;
    object->char_count = strlen(text);
    object->text = malloc(object->char_count + 1);
    object->bg_color = background_color;
    strncpy(object->text, text, object->char_count);

    return object;
}

void put_text(struct text_object* text) {
    if (text->x > get_resolution_x() || text->y > get_resolution_y() ||
        text->x < 0 || text->y < 0) {
        printf("\nWrong resolution");
        return NULL;
    }

    int cursor[2];
    cursor_get(cursor);

    int prev_x = cursor[0];
    int prev_y = cursor[1];

    cursor_set32(text->x, text->y);
    cprintf(text->bg_color, text->text);

    cursor_set(prev_x, prev_y);
    return NULL;
}

void unload_text(struct text_object* text)  {
    free(text->text);
    free(text);
}