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
#define PLAYER_SIZE_X 100
#define PLAYER_SIZE_Y 20

int selectedOption = 0; // Opção selecionada no menu

// Protótipo da função
//void DrawMenu(Font font, Music sound);
//void DrawCredits(Font font);

typedef struct {
    Vector2 position;
    Vector2 size;
    int score;
    int life;
}Player;

typedef struct{
    Vector2 position;
    Vector2 size;
    int life;
}EnemyBoss;

typedef struct {
    Vector2 position;
    Rectangle size;
    bool active;
} Enemy;

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
    Player player;
    player.size.x = PLAYER_SIZE_X;
    player.size.y = PLAYER_SIZE_Y;
    player.position.x = (SCREEN_WIDTH - player.size.x) / 2;
    player.position.y = SCREEN_HEIGHT - (player.size.y * 2);
    player.score = 0;
    
    //Código da bullet
    bool bulletActive = false;
    float bulletPositionY = player.position.y - 20;
    float bulletPositionX = player.position.x + (player.size.x / 2.2);
    Rectangle bullet;
    
    //Struct do Boss
    EnemyBoss boss;
    boss.size.x = 80;
    boss.size.y = 20;
    boss.position.x = (SCREEN_WIDTH - boss.size.x) / 2;
    boss.position.y = 50;
    //Posição do boss
    bool andarTras = true;
    int bossPosition = boss.position.x;
    //boss ataque
    bool bossAtack = false;
    
    // Código da bala do Boss
    bool enemyBullet = false;
    float enemyBulletPositionY = boss.position.y + 2;
    float enemyBulletPositionX = boss.position.x + (boss.size.x / 2.2);
    
    // Enemy
    //Quantidade total de inimigos
    int enemyTotal = 2;
    Enemy enemys[enemyTotal];
    //Ativar todos os inimigos
    for(int i = 0;i < enemyTotal;i++) {
        enemys[i].active = true;
    }
    int kk = 1;
    
    // Variaveis de jogo
    /* Usadas para definir quando esta em game e quando está no menu */
    bool inGame = false;
    bool inMenu = true;
    bool control = false;
    bool credit = false;
    
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
            control = false;
            credit = false;
        }
        
        if(inMenu == false && inGame == true) {
            //Aqui sera executado todo o código fora do menu
            
            
            if(player.score == (enemyTotal * 5) * kk) {
                kk++;
                for(int i = 0;i < enemyTotal;i++) {
                    if(enemys[i].active != true) {
                        enemys[i].active = true;
                    }
                }
            }
            
            //Movimentação do boss
            if(bossPosition == SCREEN_WIDTH - (boss.size.x)) andarTras = false;
            if(bossPosition == 0) andarTras = true;   
            if(andarTras == false) {
                bossPosition-=3;
            } else if(andarTras == true) {
                bossPosition+=3;
            }
            boss.position.x = bossPosition;
            
            
            //Logica do tiro do Boss
            if(enemyBullet > SCREEN_HEIGHT) {
                    enemyBullet = false;
                    bossAtack = false;
                }
            
            if(enemyBullet == true) {
                enemyBulletPositionY += 15;
                DrawRectangle(enemyBulletPositionX, enemyBulletPositionY, 20 ,20, RED);
            }
            
            
            //-------------------------------------------------------------------------
            
            //Movimentação do player
            if(IsKeyDown(KEY_RIGHT) && player.position.x < (SCREEN_WIDTH - player.size.x)) {
                player.position.x += 5;
            }
            if(IsKeyDown(KEY_D) && player.position.x < (SCREEN_WIDTH - player.size.x)) {
                player.position.x += 5;
            }
            
            if(IsKeyDown(KEY_LEFT) && player.position.x > 0) {
                player.position.x -= 5;
            } 
            
            if(IsKeyDown(KEY_A) && player.position.x > 0) {
                player.position.x -= 5;
            }
            //-------------------------------------------------------------------------
            
            //Logica da bala
            if(IsKeyPressed(KEY_SPACE)){
                bulletActive = true;
                bulletPositionX = player.position.x + (player.size.x / 2.2);
                bulletPositionY = player.position.y;
            }
            
            if(bulletActive == true) {
                
                if(bulletPositionY < 10) {
                    bulletActive = false;
                } else { 
                    bulletPositionY -= 10;
                }
                bullet = (Rectangle){bulletPositionX, bulletPositionY, 10, 10};
                DrawRectangleRec(bullet, RED);
                for(int h = 0;h < enemyTotal;h++) {
                    if(CheckCollisionRecs(enemys[h].size, bullet)) {
                        //Aumento no score
                        player.score += 5;
                        //Morte do inimigo
                        bulletActive = false;
                        enemys[h].active = false;
                        //Criação da bala do boss
                        enemyBullet = true;
                        enemyBulletPositionX = boss.position.x + (boss.size.x / 2.2);
                        enemyBulletPositionY = boss.position.y - 20;  
                    }
                }
            }
            //-------------------------------------------------------------------------
            
            //Logica para renderização dos inimigos
            int y = 120;
            for(int v = 0;v < enemyTotal;v++) { 
                
                if(v % 10 == 0) {
                    enemys[v].position.x = 110;
                    if(v != 0) {
                        y += 70;   
                    }
                } else {
                    if(v != 0) {
                        enemys[v].position.x = enemys[v - 1].position.x + 60;
                        enemys[v].position.y = y;    
                    }   
                }
                
                if(v == 0) {
                    enemys[0].position.y = 120;
                    enemys[10].position.y = enemys[0].position.y + 70;
                    enemys[20].position.y = enemys[10].position.y + 70;
                }
                
                if(enemys[v].active == true) {
                    enemys[v].size = (Rectangle){enemys[v].position.x, enemys[v].position.y, 40, 40};
                } else {
                    enemys[v].size = (Rectangle){1000, 1000, 0, 0};
                }
            }
            
            for(int j = 0;j< enemyTotal;j++) {
                if(enemys[j].active == true) {
                    DrawRectangleRec(enemys[j].size, BLUE);
                    DrawText(TextFormat("%.0f, %.0f, \n %d",enemys[j].position.x, enemys[j].position.y, j), enemys[j].position.x, enemys[j].position.y, 10, WHITE);
                }
            }
            //-------------------------------------------------------------------
            
            //Desenhar o Score
            DrawText(TextFormat("Score: %d", player.score), SCREEN_WIDTH - 130, 20, 20, RED);
            
            
            //Game render
            DrawPlayerAndBoss(player, boss);
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
                        control = true;
                        break;
                    case 2:
                        //Créditos
                        credit = true;
                        break;
                    case 3:
                        //Sair
                        CloseWindow();
                        break;
                }
            }
            if(control == true){
                DrawCmd();
                DrawText("Pressione TAB para Voltar", SCREEN_WIDTH / 2 - MeasureText("Pressione TAB para Voltar", FONT_SIZE) / 2, 500, FONT_SIZE, BLACK);
            }else if(credit == true){
                DrawCredits();
                DrawText("Pressione TAB para Voltar", SCREEN_WIDTH / 2 - MeasureText("Pressione TAB para Voltar", FONT_SIZE) / 2, 500, FONT_SIZE, BLACK);
            }else{
                DrawMenu(font, musica);
            }    
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
    DrawText("João Pedro - R.A: 23034350-2", SCREEN_WIDTH / 2 - MeasureText("João Pedro - R.A: 23034350-2", FONT_SIZE) / 2, 200, FONT_SIZE, BLACK);
    DrawText("Débora Reis - R.A:", SCREEN_WIDTH / 2 - MeasureText("Débora Reis - R.A:", FONT_SIZE) / 2, 250, FONT_SIZE, BLACK);
    DrawText("Pedro Alvaro - R.A: 23079477-2", SCREEN_WIDTH / 2 - MeasureText("Pedro Alvaro - R.A: 23079477-2", FONT_SIZE) / 2, 300, FONT_SIZE, BLACK);

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
void DrawPlayerAndBoss(Player player, EnemyBoss boss) {
    //Desenho do jogo
    ClearBackground(BLACK);
    // Desenhar o player
    DrawRectangle(player.position.x, player.position.y, player.size.x, player.size.y, RED);
    DrawRectangle(boss.position.x, boss.position.y, boss.size.x, boss.size.y, WHITE);
}