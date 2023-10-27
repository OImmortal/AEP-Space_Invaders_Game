#include "raylib.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MENU_TITLE "Space Invaders"
#define MENU_OPTION_PLAY "Jogar"
#define MENU_OPTION_CONTROLS "Controles"
#define MENU_OPTION_CREDITS "Créditos"
#define MENU_OPTION_EXIT "Sair"
#define FONT_SIZE 40

int selectedOption = 0; // Opção selecionada no menu

// Protótipo da função
//void DrawMenu(Font font, Music sound);
//void DrawCredits(Font font);

int main() {
    // Inicialização da janela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Meu Jogo");

    // Carregamento da fonte
    Font font = LoadFontEx("resources/font.ttf", FONT_SIZE, 0, 0);
    SetTargetFPS(60);

    InitAudioDevice();
    Music musica = LoadMusicStream("SomGame.wav");
    Music musica1 = LoadMusicStream("SomClique.wav");

    bool showCredits = false; // Variável para controlar se os créditos estão sendo exibidos
    bool showCmd = false;
    bool showGame = false;
    // Loop principal do jogo
    while (!WindowShouldClose()) {
        if(!showCmd){
            if (!showCredits) {
                playMusic(musica, musica1);
                UpDownMenuLogic();
                ChangePagetoSelectedOption(selectedOption);
                    
            } else {
                // Lógica dos créditos 
                while(!IsKeyPressed(KEY_ENTER)) {
                    // Voltar para o menu principal
                    UpdateMusicStream(musica);
                    PlayMusicStream(musica);
                    showCredits = false;
                    DrawCredits(font);
                    DrawText("Pressione Enter para Voltar", SCREEN_WIDTH / 2 - MeasureText("Pressione Enter para Voltar", FONT_SIZE) / 2, 500, FONT_SIZE, BLACK);
                    // Pausa a música enquanto retorna ao menu principal  
                }
            }
        }else{
            //Lógica de controles
            while(!IsKeyPressed(KEY_ENTER)) {
                // Voltar para o menu principal
                UpdateMusicStream(musica);
                PlayMusicStream(musica);
                showCmd = false;
                DrawCmd(font);
                DrawText("Pressione Enter para Voltar", SCREEN_WIDTH / 2 - MeasureText("Pressione Enter para Voltar", FONT_SIZE) / 2, 500, FONT_SIZE, BLACK);
            }
        }        
        // Desenha o menu
        DrawMenu(font, musica);
    }

    // Liberação de recursos
    UnloadMusicStream(musica);
    UnloadMusicStream(musica1);
    CloseAudioDevice();
    UnloadFont(font);
    CloseWindow();

    return 0;
}
//-------playMusic
void playMusic(Music musica, Music musica1) {
    // TODO: Alterar função, função provisória
    UpdateMusicStream(musica);
    UpdateMusicStream(musica1);
    PlayMusicStream(musica);
    StopMusicStream(musica1);
}

//-------UpDownMenuLogic
void UpDownMenuLogic() {
    //Essa função é utilizada para logica dos botoes no menu
    //Ela funciona da seguinte maneira
    /*
        Se a seta para cima for precionada e a opção selecionada
        seja igual a 0 então a opção selecionada seja igual a 3.
        Caso ao contrario e a seta para cima for precionada, desconte
        1 da variavel opção selecionada
        
        Esse código é utilizado para que os botões do menu fique em loop. 
        Quando a opção "JOGAR" estiver selecionada, ao clicar o botão, a 
        proxima opção selecionada será "SAIR"
        
        Esse processo acontece para os dois ifs, mas invertido
    */
    if (IsKeyPressed(KEY_UP) && selectedOption == 0)
        selectedOption = 3;
    else if (IsKeyPressed(KEY_UP))
        selectedOption -= 1;
                
    if(IsKeyPressed(KEY_DOWN) && selectedOption == 3)
        selectedOption = 0;
    else if(IsKeyPressed(KEY_DOWN))
        selectedOption += 1;

}

//---------------------------------------------
void ChangePagetoSelectedOption(int opcaoSelecionada) {
    
    if(IsKeyPressed(KEY_ENTER)) {
        switch(opcaoSelecionada) {
            case 0:
                //Jogar
                DrawText("Jogar", 50,50, 20, BLACK);
                break;
            case 1:
                DrawText("Controles", 50,50, 20, BLACK);
                break;
            case 2:
                DrawText("Créditos", 50,50, 20, BLACK);
                break;
            case 3:
                DrawText("SAIR", 50,50, 20, BLACK);
                break;
        }
    }
    
}

//-------DrawMenu
void DrawMenu(Font font, Music sound) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    // Desenhe o título
    DrawText(MENU_TITLE, SCREEN_WIDTH / 2 - MeasureText(MENU_TITLE, FONT_SIZE) / 2, 80, FONT_SIZE, BLACK);

    // Desenhe as opções do menu
    for (int i = 0; i < 4; i++) {
        if (i == selectedOption) {
            DrawText(TextFormat(" %s ", (i == 0) ? MENU_OPTION_PLAY : (i == 1) ? MENU_OPTION_CONTROLS : (i == 2) ? MENU_OPTION_CREDITS : MENU_OPTION_EXIT),
                     SCREEN_WIDTH - SCREEN_WIDTH, 200 + i * 80, FONT_SIZE, MAROON);
        } else {
            DrawText(TextFormat(" %s ", (i == 0) ? MENU_OPTION_PLAY : (i == 1) ? MENU_OPTION_CONTROLS : (i == 2) ? MENU_OPTION_CREDITS : MENU_OPTION_EXIT),
                     SCREEN_WIDTH - SCREEN_WIDTH, 200 + i * 80, FONT_SIZE, (i == selectedOption) ? MAROON : BLACK);
        }
    }

    EndDrawing();
}

//-------DrawCredits
void DrawCredits(Font font) {
    // Esta função desenha a tela de créditos
    // Você pode personalizar essa função para exibir os nomes dos desenvolvedores, agradecimentos, etc.
    BeginDrawing();
    ClearBackground(RAYWHITE);
    

    // Exemplo: Desenha os nomes dos desenvolvedores
    DrawText("Créditos", SCREEN_WIDTH / 2 - MeasureText("Créditos", FONT_SIZE) / 2, 80, FONT_SIZE, BLACK);
    DrawText("Desenvolvido por:", SCREEN_WIDTH / 2 - MeasureText("Desenvolvido por:", FONT_SIZE) / 2, 150, FONT_SIZE, BLACK);
    DrawText("Seu Nome", SCREEN_WIDTH / 2 - MeasureText("Seu Nome", FONT_SIZE) / 2, 200, FONT_SIZE, BLACK);

    // Adicione mais linhas para outros desenvolvedores ou agradecimentos
    EndDrawing();
}
//-------DrawCmd
void DrawCmd(Font font){
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawText("Controles:", 10, 10, 20, BLACK);
    DrawText("- Setas do Teclado para mover", 10, 40, 20, BLACK);
    DrawText("- Barra de Espaço para ação", 10, 70, 20, BLACK);
    EndDrawing();
} 
//-------------------------
void DrawGame() {
    
}