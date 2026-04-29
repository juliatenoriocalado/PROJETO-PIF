#include "raylib.h"
#include <stdio.h>

#define LARGURA_TELA 800
#define ALTURA_TELA 600
#define COORDENADA_CHAO 500
#define INTENSIDADE_PULO -10.0f
#define GRAVIDADE 0.5f

//==============================================
// Definição de modos de jogo ; telas ; estados
// =============================================

typedef enum {
    tela_menu,
    tela_batalha,
    tela_animacaoIntro,
    tela_animacaoFinal,
    tela_GameOver
} Telas;

//Enum é para representar uma escolha fixa (é um tipo de struct)
//Nesse caso temos um enum (não é struct, mas parece) porque não se pode ter várias telas ao mesmo tempo, e sim uma por vez.
//Em um "enum", perceba, não colocamos ; (como struct)

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

    inimigo.vida = 500;
    inimigo.ativado = 1;

    //Em c não temos boleano, então a gente define 1 para estar ativo e 0 para desativado.

}

//Define vida, posição e movimentos do personagem e do inimigo básicos

void UpdateGame(){

    switch (ModoDoJogo){

        case tela_menu:
            if (IsKeyPressed(KEY_ENTER)){
                ModoDoJogo = tela_animacaoIntro;
            }
            
        break;

        case tela_animacao:
            if (IsKeyPressed(KEY_ENTER)){
                ModoDoJogo = tela_batalha;
            }

         case tela_batalha:

            if (IsKeyDown(KEY_RIGHT)) jogador.posicao.x += 2;
            if (IsKeyDown(KEY_LEFT)) jogador.posicao.x -= 2;

            if (IsKeyDown(KEY_SPACE) && jogador.no_chao){

                jogador.velocidade_pulo = INTENSIDADE_PULO;
                jogador.no_chao = 0;

                //Se eu pressionar a tecla de espaço e o jogador estiver no chão,
                //ele recebe velocidade para cima e não vai estar no chão, o famoso PULAAR

            }

            
            jogador.velocidade_pulo += GRAVIDADE; //A gravidade MUDA a velocidade vertical do jogador
            // <-----------      <---------
            jogador.posicao.y += jogador.velocidade_pulo; //A velocidade do jogador muda a posiçaõ do jogador
            // <-----------         <------------

            if (IsKeyDown(KEY_DOWN)) jogador.posicao.y -= 2;

            //X seria a coordenada que marca a horizontal (eixo das abcissas)
            //Y seria a coordenada que marca a vertical (eixo das ordenadas)
            //Quando a tecla da seta direita é pressionada, o personagem anda para a direita
            //Quando a tecla da seta esquerda é pressionada, o personagem dá uma recuada (por isso o negativo) e acaba indo para a esquerda

                if (jogador.posicao.y >= COORDENADA_CHAO){ //Se o jogador estiver no chão (sob a coordenada do eixo Y)
                jogador.posicao.y = COORDENADA_CHAO; //Trava o jogador no chão
                jogador.velocidade_pulo = 0; //Não vai estar pulando, para a queda
                jogador.no_chao = 1; //E a afirmação se torna verdadeira de que ele tá no chão, permitindo que ele pule de novo

            }

            break;

        case tela_batalha:

        //Inimigo ataca

            if (GetRandomValue(0,100) < 2){ //Quando um valor entre 0 e 100 é usado for menor que dois (entre 1 e 0) 
                jogador.vida--; //O jogador perde vida 
            }

        //Fim da batalha

            if (inimigo.vida <= 0){
                ModoDoJogo = tel
            }

    }


}
