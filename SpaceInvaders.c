#include "raylib.h"

// Constantes de menu do jogo
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define MENU_TITLE "Space Invaders"
#define MENU_OPTION_PLAY "Jogar"
#define MENU_OPTION_CONTROLS "Controles"
#define MENU_OPTION_CREDITS "Créditos"
#define MENU_OPTION_EXIT "Sair"
#define FONT_SIZE 40

// Constantes do Player
#define PLAYER_SIZE_X 80
#define PLAYER_SIZE_Y 20

int selectedOption = 0; // Opção selecionada no menu

// Protótipo da função
//void DrawMenu(Font font, Music sound);
//void DrawCredits(Font font);

struct Player{
    Vector2 position;
    Vector2 size;
    int life;
};

int main() {
    // Inicialização da janela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Meu Jogo");

    // Carregamento da fonte
    Font font = LoadFontEx("resources/font.ttf", FONT_SIZE, 0, 0);
    SetTargetFPS(60);

    InitAudioDevice();
    Music musica = LoadMusicStream("SomGame.wav");
    Music musica1 = LoadMusicStream("SomClique.wav");
    
    struct Player player;
    player.size.x = PLAYER_SIZE_X;
    player.size.y = PLAYER_SIZE_Y;
    player.position.x = (SCREEN_WIDTH - player.size.x) / 2;
    
    
    // Variaveis de jogo
    /* Usadas para definir quando esta em game e quando está no menu */
    bool inGame = true;
    bool inMenu = false;
    
    // Loop principal do jogo
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        playMusic(musica, musica1);    
        PlayMusicStream(musica);
        
        if(inMenu == false && inGame == true) {
            DrawGame(player);
        }
        
        if(inMenu == true && inGame == false) {
            
            UpDownMenuLogic();
            ChangePagetoSelectedOption(selectedOption, musica1);
            // Desenha o menu
            DrawMenu(font, musica);
        }
        EndDrawing();
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

//-------ChangePagetoSelectedOption
void ChangePagetoSelectedOption(int opcaoSelecionada, Music musica1) {
    // Funçao com objetivo de selecionar uma opção
    // TODO: Em construção - aceitando mudanças
    float posicao = GetMusicTimePlayed(musica1);
    bool showCr = false;
    if(posicao >= 1.120000) {
        StopMusicStream(musica1);
    }
    if(IsKeyPressed(KEY_ENTER)) {
        PlayMusicStream(musica1);
        switch(opcaoSelecionada) {
            case 0:
                //Jogar
                break;
            case 1:
                //Controles
                break;
            case 2:
                //Créditos
                break;
            case 3:
                //Sair
                break;
        }
    }
    
}

//-------DrawMenu
void DrawMenu() {
    
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
    
}

//-------DrawCredits
void DrawCredits() {
    // Esta função desenha a tela de créditos
    // Você pode personalizar essa função para exibir os nomes dos desenvolvedores, agradecimentos, etc.
    
    

    // Exemplo: Desenha os nomes dos desenvolvedores
    DrawText("Créditos", SCREEN_WIDTH / 2 - MeasureText("Créditos", FONT_SIZE) / 2, 80, FONT_SIZE, BLACK);
    DrawText("Desenvolvido por:", SCREEN_WIDTH / 2 - MeasureText("Desenvolvido por:", FONT_SIZE) / 2, 150, FONT_SIZE, BLACK);
    DrawText("Seu Nome", SCREEN_WIDTH / 2 - MeasureText("Seu Nome", FONT_SIZE) / 2, 200, FONT_SIZE, BLACK);

    // Adicione mais linhas para outros desenvolvedores ou agradecimentos
    
}
//-------DrawCmd
void DrawCmd(Font font){
    // Desenho de texto informando os controlers
    DrawText("Controles:", 10, 10, 20, BLACK);
    DrawText("- Setas do Teclado para mover", 10, 40, 20, BLACK);
    DrawText("- Barra de Espaço para ação", 10, 70, 20, BLACK);
} 
//-------------------------
void DrawGame(struct Player player) {
    //Desenho do jogo
    ClearBackground(BLACK);
    DrawRectangle(player.position.x, SCREEN_HEIGHT - (player.size.y * 2), player.size.x, player.size.y, RED);
}