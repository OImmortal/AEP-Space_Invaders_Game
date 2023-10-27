#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MENU_TITLE "Meu Jogo"
#define MENU_OPTION_PLAY "Jogar"
#define MENU_OPTION_EXIT "Sair"
#define FONT_SIZE 40

int selectedOption = 0; // Opção selecionada no menu

int main(void) {
    // Inicialização da janela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Meu Jogo");

    // Carregamento da fonte
    Font font = LoadFontEx("resources/font.ttf", FONT_SIZE, 0, 0);
    SetTargetFPS(60);
    
    // Loop principal do jogo
    while (!WindowShouldClose()) {
        if (IsKeyPressed(KEY_UP)) {
            selectedOption = (selectedOption - 1 + 2) % 2;
        } else if (IsKeyPressed(KEY_DOWN)) {
            selectedOption = (selectedOption + 1) % 2;
        }

    // Desenha o menu
        DrawMenu(font);
    }

    // Liberação de recursos
    UnloadFont(font);
    CloseWindow();

    return 0;
}
void DrawMenu(Font font) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Desenhe o título
    DrawText(MENU_TITLE, SCREEN_WIDTH/2 - MeasureText(MENU_TITLE, FONT_SIZE)/2, 80, FONT_SIZE, BLACK);

    // Desenhe as opções do menu
    for (int i = 0; i < 2; i++) {
        if (i == selectedOption) {
            DrawText(TextFormat("> %s <", (i == 0) ? MENU_OPTION_PLAY : MENU_OPTION_EXIT),
                     SCREEN_WIDTH/2 - MeasureText(MENU_OPTION_PLAY, FONT_SIZE)/2, 200 + i * 80, FONT_SIZE, MAROON);
        } else {
            DrawText(TextFormat("%s", (i == 0) ? MENU_OPTION_PLAY : MENU_OPTION_EXIT),
                     SCREEN_WIDTH/2 - MeasureText(MENU_OPTION_PLAY, FONT_SIZE)/2, 200 + i * 80, FONT_SIZE, BLACK);
        }
    }

    EndDrawing();
}