#include "raylib.h"
#include <stdio.h>

//Constantes

#define LARGURA_TELA 800
#define ALTURA_TELA 600
#define COORDENADA_CHAO 500
#define INTENSIDADE_PULO -10.0f
#define GRAVIDADE 0.5f
#define TOTAL_CENAS_FINAIS 3

//==============================================
// Definição de modos de jogo ; telas ; estados
// =============================================

typedef enum { //Controlar que tela está ativa
    tela_menu,
    tela_batalha,
    tela_animacaoIntro,
    tela_animacaoFinal,
    tela_GameOver
} Telas;

//Enum é para representar uma escolha fixa (é um tipo de struct)
//Nesse caso temos um enum (não é struct, mas parece) porque não se pode ter várias telas ao mesmo tempo, e sim uma por vez.
// Com typedef enum, colocamos ; depois do nome do tipo, igual na struct.

//==============================================
//             Estruturas básicas
//==============================================

typedef struct {
    Vector2 posicao;
    int vida;
    int no_chao;
    float velocidade_pulo;
} Jogador;

//Float para velocidade para ter uma melhor precisão
//Vector2 é uma palavra especial na bibliotec Raylib, ela permite criar uma variável que vai definir as coordenadas do personagem
//De forma resumida, Vector2 é um tipo de variável que marca coordenadas

typedef struct {
    int vida;
    int ativado;
} Inimigo;

int IndiceCenaFinal = 0; //Contador para marcas as cenas sendo passadas

const char *textos_final[] = { //Vetor de textos //Ponteiro para caractere (marcar o começo de uma string)
    "No final, Rose acorda...",
    "E percebe que, no fim, tudo não passava de um 'sonho'.",
    "Ela se levantou, e finalmente adquiriu a coragem que precisava para o show..."
};

//==============================================
//             Variáveis globais
//==============================================

Telas ModoDoJogo = tela_menu;

//Aqui eu estou criando uma variável do tipo enum "Telas" para marcar o início ao menu do jogo

Jogador jogador;
Inimigo inimigo;

//Aqui estamos criando as variáveis tanto para personagem quanto para os nossos inimigos (cada um com seus respectivos structs, os de letra maiúscula, para marcar do formato para o objeto)

//==============================================================================================================================================================================================
//                                                                            Inicalização do jogo! 
//==============================================================================================================================================================================================

void InitGame(){

    jogador.posicao = (Vector2){400, COORDENADA_CHAO}; //X = 400 (posição horizontal) e Y = COORDENADA_CHAO (começa no)

    //Coordenadas do jogador

    jogador.vida = 100;
    jogador.no_chao = 1;
    jogador.velocidade_pulo = 0;

    inimigo.vida = 500;
    inimigo.ativado = 1;

    //Em c não temos boleano, então a gente define 1 para estar ativo e 0 para desativado.

}

//Define vida, posição e movimentos do personagem e do inimigo básicos

void AtualizarJogo(){

    switch (ModoDoJogo){

        case tela_menu:
            if (IsKeyPressed(KEY_ENTER)){
                ModoDoJogo = tela_animacaoIntro;
            }
            
            break;

        case tela_animacaoIntro:
            if (IsKeyPressed(KEY_ENTER)){
                ModoDoJogo = tela_batalha;
            }

            break;

         case tela_batalha:

            if (IsKeyDown(KEY_RIGHT)) jogador.posicao.x += 2;
            if (IsKeyDown(KEY_LEFT)) jogador.posicao.x -= 2;

            if (IsKeyPressed(KEY_A)){
                inimigo.vida -= 10;
            }

            if (IsKeyPressed(KEY_SPACE) && jogador.no_chao){

                jogador.velocidade_pulo = INTENSIDADE_PULO;
                jogador.no_chao = 0;

                //Se eu pressionar a tecla de espaço e o jogador estiver no chão,
                //ele recebe velocidade para cima e não vai estar no chão, o famoso PULAAR

            }

            
            jogador.velocidade_pulo += GRAVIDADE; //A gravidade MUDA a velocidade vertical do jogador
            // <-----------      <---------
            jogador.posicao.y += jogador.velocidade_pulo; //A velocidade do jogador muda a posiçaõ do jogador
            // <-----------         <------------

            //X seria a coordenada que marca a horizontal (eixo das abcissas)
            //Y seria a coordenada que marca a vertical (eixo das ordenadas)
            //Quando a tecla da seta direita é pressionada, o personagem anda para a direita
            //Quando a tecla da seta esquerda é pressionada, o personagem dá uma recuada (por isso o negativo) e acaba indo para a esquerda

                if (jogador.posicao.y >= COORDENADA_CHAO){ //Se o jogador estiver no chão (sob a coordenada do eixo Y)
                    jogador.posicao.y = COORDENADA_CHAO; //Trava o jogador no chão
                    jogador.velocidade_pulo = 0; //Não vai estar pulando, para a queda
                    jogador.no_chao = 1; //E a afirmação se torna verdadeira de que ele tá no chão, permitindo que ele pule de novo

            }

            //Inimigo ataca

            if (GetRandomValue(0,100) < 2){ //Quando um valor entre 0 e 100 é usado for menor que dois (entre 1 e 0) 
                jogador.vida--; //O jogador perde vida 
            }

            //Inimigo perdeu

            if (inimigo.vida <= 0){
                IndiceCenaFinal = 0;
                ModoDoJogo = tela_animacaoFinal;
            }

            //Jogador perdeu

            if (jogador.vida <= 0){
                IndiceCenaFinal = 0;
                ModoDoJogo
            }

            break;

            //Fim da batalha

            case tela_GameOver:
                if (jogador.vida <= 0){
                    ModoDoJogo = tela_GameOver;
                }

                break;

            case tela_animacaoFinal:
            
                if (IsKeyPressed(KEY_ENTER)){
                    IndiceCenaFinal++;

                    if (IndiceCenaFinal >= TOTAL_CENAS_FINAIS){
                        InitGame();
                        IndiceCenaFinal = 0; //Vai começar no indice zero
                        ModoDoJogo = tela_menu;

                    }

                }

                break;

    }

}

void DesenharJogo(){

    switch (ModoDoJogo){
        case tela_menu:
            DrawText("PRESSIONE ENTER PARA COMEÇAR", 180,300,20,WHITE);
            break;

        case tela_animacaoIntro:
            DrawText("CARNIVAL STORY", 250,80,40, RED);

            DrawRectangle(80,140,640,280,BLACK);
            DrawRectangleLines(80,140,640,280,WHITE);

            DrawText("Rose é uma menina gentil, cheia de sonhos...", 110,450,20,WHITE);
            DrawText("ENTER", 180,520,20,GRAY); //Para iniciar a batalha
            break;

        case tela_batalha:
            DrawText("LUTE!", 350,50,30,RED);

            DrawCircleV(jogador.posicao,20,BLUE);

            DrawText(TextFormat("Jogador HP: %d", jogador.vida), 50,100,20,WHITE);
            DrawText(TextFormat("Inimigo HP: %d", inimigo.vida), 50,130,20,RED);

            DrawText("Pressione A para atacar", 50,200,20,WHITE);
            break;

        case tela_animacaoFinal:
            DrawText("CARNIVAL STORY", 250,80,40,RED);

            DrawRectangle(80,140,640,280,BLACK);
            DrawRectangleLines(80,140,640,280,WHITE);

            DrawText(textos_final[IndiceCenaFinal], 110,450,20,WHITE); //Acessar o índice do vetor de cenas

                if (IndiceCenaFinal < TOTAL_CENAS_FINAIS - 1){ //Se o índice da cena final chegou no total de cenas, então acabaram as cenas finais.
                        DrawText("ENTER para continuar", 180,520,20,GRAY);
                }else{
                    DrawText("ENTER para retornar ao menu", 180,520,20,GRAY);
                }

            break;

    }

}

//========================================================================================================
//                                       Looping Principal!
//========================================================================================================

int main(){

    InitWindow(LARGURA_TELA,ALTURA_TELA,"Carnival Story");
    SetTargetFPS(60); //Função do Raylib que define os frames por segundo, sem isso, poderia rodar rápido demais (dependendo do pc)

    InitGame();

    while (!WindowShouldClose()){ //Enquanto a janela NÃO estiver fechada, continue rodando o jogo

        AtualizarJogo(); //Lógica: teclas, movimentos, vida, colisão

        BeginDrawing(); //Função especial que marca o começo da parte do desenho da tela (desenha o frame)

        ClearBackground(DARKGRAY); //Limpar tela

        DesenharJogo(); //Desenha o menu, player, inimigo, textos etc.

        EndDrawing(); //Função especial para terminar o desenho e mostrar a janela (termina de desenhar o frame para finalmente mostrar na tela)

        //Tanto o BeginDrawing e o EndDrawing são necessários, porque o jogo desenha a tela várias vezez por segundo, tipo 60 FPS
        //Tudo que for visual fica entre eles
    }

    CloseWindow();
    return 0;

}