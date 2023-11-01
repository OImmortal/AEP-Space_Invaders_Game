#include "raylib.h"
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

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

struct EnemyBoss {
    Vector2 position;
    Vector2 size;
    int life;
};

struct BulletPlayer{
    Vector2 position;
    Rectangle size;
    bool active;
    int speed;
};

int main() {
    srand(time(NULL));
    
    int teste = rand() % 3;
    
    // Inicialização da janela
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Meu Jogo");

    // Carregamento da fonte
    Font font = LoadFontEx("resources/font.ttf", FONT_SIZE, 0, 0);
    SetTargetFPS(60);

    InitAudioDevice();
    Music musica = LoadMusicStream("SomGame.wav");
    Music musica1 = LoadMusicStream("SomClique.wav");
    
    // Struct do player
    struct Player player;
    player.size.x = PLAYER_SIZE_X;
    player.size.y = PLAYER_SIZE_Y;
    player.position.x = (SCREEN_WIDTH - player.size.x) / 2;
    
    //Struct do Boss
    struct EnemyBoss boss;
    boss.size.x = 80;
    boss.size.y = 20;
    boss.position.x = (SCREEN_WIDTH - boss.size.x) / 2;
    boss.position.y = 20;
    
    struct BulletPlayer bullet;
    bullet.active = false;
    
    // Variaveis de jogo
    /* Usadas para definir quando esta em game e quando está no menu */
    bool inGame = false;
    bool inMenu = true;
    
    //Posição do boss
    bool andar = true;
    int bossPosition = boss.position.x;
    
    // Loop principal do jogo
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        playMusic(musica, musica1);    
        PlayMusicStream(musica);
        
        //Voltar para o menu
        if(IsKeyPressed(KEY_TAB)) {
            inMenu = true;
            inGame = false;
        }
        
        if(inMenu == false && inGame == true) {
            //Aqui sera executado todo o código fora do menu
            
            //Movimentação do boss
            if(bossPosition == SCREEN_WIDTH - boss.size.x) andar = false;
            if(bossPosition == 0) andar = true;   
            if(andar == false) {
                bossPosition-=3;
            } else if(andar == true) {
                bossPosition+=3;
            }
            boss.position.x = bossPosition;
            
            //Movimentação do player
            if(IsKeyDown(KEY_RIGHT) && player.position.x < (SCREEN_WIDTH - player.size.x)) {
                player.position.x += 5;
            }
            
            if(IsKeyDown(KEY_LEFT) && player.position.x > 0) {
                player.position.x -= 5;
            }   


            
            
            //Game render
            DrawGame(player, boss);
        }
        
        if(inMenu == true && inGame == false) {
            
            UpDownMenuLogic();
            // Selecionar opção do menu
            if(IsKeyPressed(KEY_ENTER)) {
                PlayMusicStream(musica1);
                switch(selectedOption) {
                    case 0:
                        //Jogar
                        inMenu = false;
                        inGame = true;
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
            DrawMenu(font, musica);
        }
        
        // Para musica do clique
        float musicPosition = GetMusicTimePlayed(musica1);
        if(musicPosition >= 1.120000) {
            StopMusicStream(musica1);
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
void DrawGame(struct Player player, struct EnemyBoss boss) {
    //Desenho do jogo
    ClearBackground(BLACK);
    // Desenhar o player
    DrawRectangle(player.position.x, SCREEN_HEIGHT - (player.size.y * 2), player.size.x, player.size.y, RED);
    DrawRectangle(boss.position.x, boss.position.y, boss.size.x, boss.size.y, WHITE);
}