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
#define FONT_SIZE 30

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
    Rectangle playerRec;
    int score;
    int speed;
    int life;
}Player;

typedef struct{
    Vector2 position;
    Vector2 size;
    Rectangle bossRec;
    int speed;
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
    Sound powerUpSound = LoadSound("powerUp.wav");
    Sound hitEnemy = LoadSound("hitHurt.wav");
    Sound BossDie = LoadSound("explosion.wav");
    
    // Struct do player
    Player player;
    player.size.x = PLAYER_SIZE_X;
    player.size.y = PLAYER_SIZE_Y;
    player.position.x = (SCREEN_WIDTH - player.size.x) / 2;
    player.position.y = SCREEN_HEIGHT - (player.size.y * 2);
    player.score = 0;
    player.speed = 5;
    
    //Código da bullet
    bool bulletActive = false;
    float bulletPositionY = player.position.y - 20;
    float bulletPositionX = player.position.x + (player.size.x / 2.2);
    Rectangle bullet;
    int bulletPlayerSpeed = 10;
    
    int lastScore = 0;
    
    //Struct do Boss
    EnemyBoss boss;
    boss.size.x = 80;
    boss.size.y = 20;
    boss.position.x = (SCREEN_WIDTH - boss.size.x) / 2;
    boss.position.y = 50;
    boss.speed = 3;
    boss.life = 4;
    //Posição do boss
    bool andarTras = true;
    int bossPosition = boss.position.x;
    //boss ataque
    bool bossAtack = false;
    bool finalBattle = false;
    

    
    // Código da bala do Boss
    bool enemyBullet = false;
    float enemyBulletPositionY = boss.position.y + 2;
    float enemyBulletPositionX = boss.position.x + (boss.size.x / 2.2);
    Rectangle enemyBulletRec; // Bala do inimigo
    
    // Enemy
    //Quantidade total de inimigos
    int enemyTotal = 40;
    Enemy enemys[enemyTotal];
    //Ativar todos os inimigos
    for(int i = 0;i < enemyTotal;i++) {
        enemys[i].active = true;
    }
    int jogoReiniciado = 1;

    
    // Variaveis de jogo
    /* Usadas para definir quando esta em game e quando está no menu */
    bool inGame = false;
    bool inMenu = true;
    bool control = false;
    bool credit = false;
    bool theEnd = false;
    bool theWin = false;
    
    Enemy enemyInitialPositions[enemyTotal];
    for (int i = 0; i < enemyTotal; i++) {
        enemyInitialPositions[i].position = enemys[i].position;
    }
    
    //PowerUp
    int powerUp = rand() % 100;
    bool takePowerUp = false;
    Rectangle powerUpRec;
    
    // Reiniciar Jogo / Voltar ao menu
    void voltarAoMenu() {
        if(IsKeyPressed(KEY_TAB)) {
            inMenu = true; 
            inGame = false;
            control = false;
            credit = false;
            theEnd = false;
            theWin = false;
            player.score = 0;
            ReiniciarInimigos(enemys, enemyInitialPositions, enemyTotal);
            //boss
            boss.position.x = (SCREEN_WIDTH - boss.size.x) / 2;
            boss.position.y = 50;
            boss.life = 4;
            boss.speed = 3;
            //Bala do inimigo
            bossAtack = false;
            enemyBullet = false;
            //player
            player.position.x = (SCREEN_WIDTH - player.size.x) / 2;
            player.position.y = SCREEN_HEIGHT - (player.size.y * 2);
            //Bala do player
            bulletActive = false;  
        }
    }
    
    // Loop principal do jogo
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        playMusic(musica, musica1);    
        PlayMusicStream(musica);
        
        //Voltar para o menu
        
        voltarAoMenu();
        
        // Manter valores
        if(player.speed < 5) {
            player.speed = 5;
        }
        
        if(bulletPlayerSpeed < 10) {
            bulletPlayerSpeed = 10;
        }
        
        // PowerUp logica
        if(powerUp == 1) {
            powerUpRec = (Rectangle){SCREEN_WIDTH - 110, player.position.y, 20, 20};
            DrawText(("Vida Extra!"), SCREEN_WIDTH - 155, SCREEN_HEIGHT - 70, 20, GREEN);
            DrawRectangleRec(powerUpRec, RED);
            takePowerUp = true;
        }
        
        if(powerUp == 2) {
            powerUpRec = (Rectangle){SCREEN_WIDTH - 110, player.position.y, 20, 20};
            DrawText(("Bônus Velocidade!"), SCREEN_WIDTH - 195, SCREEN_HEIGHT - 70, 20, GREEN);
            DrawRectangleRec(powerUpRec, BLUE);
            takePowerUp = true;
        }
        
        if(theEnd == true) {
            DrawEnd(player.score);
            takePowerUp = false;
            powerUpRec = (Rectangle){0};
            powerUp = 0;
            
        }else if(theWin == true){
            DrawWin(player.score);
            takePowerUp = false;
            powerUpRec = (Rectangle){0};
            powerUp = 0;   
        }
        
        if(player.score > lastScore){
            lastScore = player.score;
        }
        
        if(inMenu == false && inGame == true) {
            //Aqui sera executado todo o código fora do menu
            
            // Perder jogo
            if(player.life <= 0) {
                theEnd = true;
                inGame = false;
            }
            
            if(boss.life >= 1) {
                boss.bossRec = (Rectangle){boss.position.x, boss.position.y, boss.size.x, boss.size.y};
            } else {
                if(boss.life <= 0) {
                    PlaySound(BossDie);
                    boss.bossRec = (Rectangle){0};
                    theWin = true;
                    inGame = false;
                }
            }
            
            //Movimentação do boss
            if(bossPosition >= SCREEN_WIDTH - (boss.size.x)) andarTras = false;
            if(bossPosition <= 0) andarTras = true;   
            if(andarTras == false) {
                bossPosition-=boss.speed;
            } else if(andarTras == true) {
                bossPosition+=boss.speed;
            }
            boss.position.x = bossPosition;
            
            
            //Logica do tiro do Boss
            if(enemyBulletPositionY > SCREEN_HEIGHT) {
                    enemyBullet = false;
                    bossAtack = false;
            }
            
            if(enemyBullet == true) {
                enemyBulletPositionY += 25;
                enemyBulletRec = (Rectangle){enemyBulletPositionX, enemyBulletPositionY, 20, 20}; // Bala do inimigo
                DrawRectangleRec(enemyBulletRec, RED);
            }
            
            
            //-------------------------------------------------------------------------
            
            //Movimentação do player
            if(IsKeyDown(KEY_RIGHT) && player.position.x < (SCREEN_WIDTH - player.size.x)) {
                player.position.x += 5; // Para direita
            }
            if(IsKeyDown(KEY_D) && player.position.x < (SCREEN_WIDTH - player.size.x)) {
                player.position.x += 5; // Para direita
            }
            
            if(IsKeyDown(KEY_LEFT) && player.position.x > 0) {
                player.position.x -= 5; // Para esquerda
            } 
            
            if(IsKeyDown(KEY_A) && player.position.x > 0) {
                player.position.x -= 5; // Para esquerda
            }
            
            if(CheckCollisionRecs(player.playerRec, powerUpRec) && powerUp == 1) {
                takePowerUp = false;
                powerUp = 0;
                if(player.life <= 3) {
                    player.life = player.life + 1;
                }
                PlaySound(powerUpSound);
            }
            
            if(CheckCollisionRecs(player.playerRec, powerUpRec) && powerUp == 2) {
                takePowerUp = false;
                powerUp = 0;
                if(player.speed < 7 && bulletPlayerSpeed < 20) {
                    bulletPlayerSpeed += 15;
                    player.speed += 5;
                }
                PlaySound(powerUpSound);
            } 
            
            // Se a vida do player for maior que zero, ele é criado
            // Caso ao contrario ele é apagado
            if(player.life > 0) {
                player.playerRec = (Rectangle){player.position.x, player.position.y, player.size.x, player.size.y};
            } else {
                player.playerRec = (Rectangle){0};
            }
            
            // Verifica se a bala do inimigo está colidindo com o player
            if(CheckCollisionRecs(player.playerRec, enemyBulletRec) == true) {
                player.life -= 1;
                PlaySound(hitEnemy);
            } 
            
            
            //-------------------------------------------------------------------------
            
            //Logica da bala do player
            // Criação do objeto bala do player
            if(IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_C)){
                bulletActive = true;
                bulletPositionX = player.position.x + (player.size.x / 2.2);
                bulletPositionY = player.position.y;
            }
            
            // Tiro iniciado
            if(bulletActive == true) {
                
                if(bulletPositionY < 10) {
                    bulletActive = false; // Desativa a bala do player ao atravessar o mapa
                } else { 
                    bulletPositionY -= bulletPlayerSpeed; // Volta a bala na estaca 0
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
                        PlaySound(hitEnemy);
                        if(takePowerUp == false) {
                            if(player.life == 1) {
                                powerUp = rand() % 6;
                            } else { 
                                powerUp = rand() % 25;
                            }
                        }
                        
                        if(player.speed > 5) {
                            player.speed-=2;
                        }
                        
                        if(bulletPlayerSpeed > 10) {
                            bulletPlayerSpeed-=5;
                        }
                    }
                }
            }
            //-------------------------------------------------------------------------
                
                // Briga final
                int quantidadeDeMortos = 0;
                for(int x = 0;x < enemyTotal;x++) {
                    if(enemys[x].active == false) {
                        quantidadeDeMortos++;
                    }
                }
                
                if(quantidadeDeMortos == enemyTotal) {
                    finalBattle = true;
                }
                
                if(finalBattle == true) {
                    if(CheckCollisionRecs(bullet, boss.bossRec)) {
                        boss.life -= 1;
                        boss.speed+=2;
                        finalBattle = false;
                        ReiniciarInimigos(enemys, enemyInitialPositions, enemyTotal);
                        bulletPlayerSpeed+=2;
                        PlaySound(hitEnemy);
                    }
                }
                
                DrawText(TextFormat("Player Speed: %d", player.speed), 100, SCREEN_HEIGHT - 30, 10, WHITE);
                DrawText(TextFormat("Vida do boss: %d", boss.life), 15, SCREEN_HEIGHT - 30, 10, WHITE);
                
                DrawText(TextFormat("Bullet Speed: %d", bulletPlayerSpeed), 140, SCREEN_HEIGHT - 20, 10, WHITE);
                DrawText(TextFormat("Power Up: %d", powerUp), 70, SCREEN_HEIGHT - 20, 10, WHITE);
                DrawText(TextFormat("Mortes: %d", quantidadeDeMortos), 15, SCREEN_HEIGHT - 20, 10, WHITE);
                
            //-------------------------------------------------------------------------
           
            //Logica para renderização dos inimigos
            int y = 120;
            for(int v = 0;v < enemyTotal;v++) { 
                
                // Posiciona os inimigos em seus lugares
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
                
                // Um bug acontecia com os valores que terminavam com zeros, ess código foi usado
                // para resolve-lo
                if(v % 10 == 0) {
                    if(v == 0) {
                        enemys[0].position.y = 120;
                    } else {
                        enemys[v].position.y = enemys[v - 1].position.y + 70;
                    }
                }
                // Criar rectangle do inimigo
                if(enemys[v].active == true) {
                    enemys[v].size = (Rectangle){enemys[v].position.x, enemys[v].position.y, 40, 40};
                } else {
                    enemys[v].size = (Rectangle){0};
                }
            }
            
            for(int j = 0;j< enemyTotal;j++) {
                if(enemys[j].active == true) {
                    DrawRectangleRec(enemys[j].size, BLUE);
                }
            }
            //-------------------------------------------------------------------
            
            //Desenhar o Score
            DrawText(TextFormat("Score: %d", player.score), SCREEN_WIDTH - 130, 20, 20, RED);
            DrawText(TextFormat("Recorde Score: %d", lastScore), SCREEN_WIDTH - 350, 20, 20, BLUE);
            DrawText(TextFormat("Life: %d", player.life), 30, 20, 20, WHITE);
            
            //Game render
            DrawPlayerAndBoss(player, boss, enemyBulletRec);
        }
        
        if(inMenu == true && inGame == false) {
            
            UpDownMenuLogic();
            
            DrawText(TextFormat("Recorde Score: %d", lastScore), SCREEN_WIDTH - 350, SCREEN_HEIGHT - 40, FONT_SIZE, BLACK);
            
            // Selecionar opção do menu
            if(IsKeyPressed(KEY_ENTER)) {
                PlayMusicStream(musica1);
                switch(selectedOption) {
                    case 0:
                        //Jogar
                        inMenu = false;
                        inGame = true;
                        theEnd = false;
                        player.life = 4;
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

void ReiniciarInimigos(Enemy enemys[], const Enemy initialPositions[], int enemyTotal) {
    for (int i = 0; i < enemyTotal; i++) {
        enemys[i].active = true;
        enemys[i].position = initialPositions[i].position;
    }
}

//-------DrawMenu
void DrawMenu() {
    
    // Desenhe o título
    DrawText(MENU_TITLE, SCREEN_WIDTH / 2 - MeasureText(MENU_TITLE, FONT_SIZE) / 2, 100, FONT_SIZE, BLACK);

    // Desenhe as opções do menu
    for (int i = 0; i < 4; i++) {
        if (i == selectedOption) {
            DrawText(TextFormat(" %s ", (i == 0) ? MENU_OPTION_PLAY : (i == 1) ? MENU_OPTION_CONTROLS : (i == 2) ? MENU_OPTION_CREDITS : MENU_OPTION_EXIT),
                     SCREEN_WIDTH - SCREEN_WIDTH, 200 + i * 80, FONT_SIZE + 10, MAROON);
        } else {
            DrawText(TextFormat(" %s ", (i == 0) ? MENU_OPTION_PLAY : (i == 1) ? MENU_OPTION_CONTROLS : (i == 2) ? MENU_OPTION_CREDITS : MENU_OPTION_EXIT),
                     SCREEN_WIDTH - SCREEN_WIDTH, 200 + i * 80, FONT_SIZE + 10, (i == selectedOption) ? MAROON : BLACK);
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
    DrawText("- D ou -> para direita", 10, 40, 20, BLACK);
    DrawText("- A ou <- para esquerda", 10, 70, 20, BLACK);
    DrawText("- Barra de Espaço para ação", 10, 100, 20, BLACK);
} 
//-------------------------
//-------DrawEnd
void DrawEnd(int score) {
    char scoreText[20];
    DrawText("Que pena, você Perdeu!", SCREEN_WIDTH / 2 - MeasureText("Que pena, você perdeu!", FONT_SIZE) / 2, 190, 30, BLACK);
    sprintf(scoreText, "SCORE : %i", score);
    DrawText(scoreText, SCREEN_WIDTH / 2 - MeasureText(scoreText, FONT_SIZE) / 2, 250, 30, BLACK);
    DrawText("Pressione TAB para Voltar", SCREEN_WIDTH / 2 - MeasureText("Pressione TAB para Voltar", FONT_SIZE) / 2, 500, FONT_SIZE, BLACK);
}
//-------DrawWin
void DrawWin(int score) {
    char scoreText[20];
    DrawText("Parabéns, Você Venceu!", SCREEN_WIDTH / 2 - MeasureText("Parabéns, Você Venceu!!", FONT_SIZE) / 2, 190, 30, BLACK);
    sprintf(scoreText, "SCORE: %i", score);
    DrawText(scoreText, SCREEN_WIDTH / 2 - MeasureText(scoreText, FONT_SIZE) / 2, 250, 30, BLACK);
    DrawText("Pressione TAB para Voltar", SCREEN_WIDTH / 2 - MeasureText("Pressione TAB para Voltar", FONT_SIZE) / 2, 500, FONT_SIZE, BLACK);
}
void DrawPlayerAndBoss(Player player, EnemyBoss boss, Rectangle enemyBulletRec) {
    //Desenho do jogo
    ClearBackground(DARKGRAY);
    // Desenhar o player
    if(CheckCollisionRecs(player.playerRec, enemyBulletRec) == true) {
        DrawRectangleRec(player.playerRec, RED);
    } else {
        DrawRectangleRec(player.playerRec, GREEN);
    }
    DrawRectangleRec(boss.bossRec, WHITE);
}