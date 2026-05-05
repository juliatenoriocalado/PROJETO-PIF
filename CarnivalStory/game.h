//EM C, quando um projeto é muito grande a gente geralmente separa um arquivo chamado algo.h 
// para saber quais funções/structs existem em outro. {Declarações gerais do jogo}

#ifndef GAME_H //Se algo ou um trabalho ou enfim, se ALGO ainda não estiver definido, execute o que está em baixo ("ANOTE O NÚMERO")
#define GAME_H 

// #define GAME_H Etiqueta para definir a configuração (tipo um nome)

#include "raylib.h"
#include <stdio.h>

//Constantes

#define LARGURA_TELA 800 //Largura da tela que vai ser aberta para jogar
#define ALTURA_TELA 600 //Altura da tela que o jogo vai abrir
#define COORDENADA_CHAO 500 //Coordenada que o personagem vai iniciar no jogo (em Raylib, o eixo Y começa do 0, no TOPO da tela e vai descendo para baixo, por isso que é 500, porque a tela é 600)
#define INTENSIDADE_PULO -10.0f //Intensidade com que o jogador pula
#define GRAVIDADE 0.5f //Gravidade definida (cair mais lento mais rápido...)
#define TOTAL_CENAS_FINAIS 3 //Quantidade de cenas que serão exibidas no final, quando o jogo acabar)
#define LARGURA_PROJETIL 45 //largura ou grossura do projétil, tamanho pra os lados)
#define ALTURA_PROJETIL 30 
#define ALTURA_PROJETIL_RASTEIRO (COORDENADA_CHAO - ALTURA_PROJETIL) //Para o primeiro iniciar bem perto do chão e forçar ele a pular
#define VIDA_MAX_INIMIGO 500 
#define VIDA_MAX_JOGADOR 100
#define TEMPO_COOLDOWN_PROJETIL 0.6f 
#define ALTURA_PROJETIL_MIN (COORDENADA_CHAO - 35)
#define ALTURA_PROJETIL_MAX (COORDENADA_CHAO - ALTURA_PROJETIL)
#define LARGURA_ATAQUE 60  //Largura do ataque do jogador
#define ALTURA_ATAQUE 40 //Altura do ataque do jogador
#define TEMPO_ATAQUE 0.15f //Tempo que marca quando a Rose recebe dano (é para mostrar visualmente e desenhar na hora)
#define TEMPO_PARRY 0.18f //Tempo para acertar o parry
#define COOLDOWN_PARRY 0.7f //Tempo de recarregar o parry
#define DANO_PARRY 5 //Dano que o boss leva ao ser aparado
#define AREA_PARRY 30 //Tamanho visual do parry (quanto maior, mais fácil de acertar, porque ele mede a colisão entre o projétil e o jogador)
#define VELOCIDADE_PROJETIL_FASE1 8.0f
#define VELOCIDADE_PROJETIL_FASE2 15.0f
#define TEMPO_COOLDOWN_PROJETIL_FASE1 0.6f
#define TEMPO_COOLDOWN_PROJETIL_FASE2 0.45f
#define TEMPO_AVISO_ATAQUE_INIMIGO 0.35f //Duração do aviso antes de o inimigo atacar (antes de ele atacar)
#define QUANTIDADE_DE_ATAQUES_PARA_RAJADA 3 //A cada três ataques na fase dois 
#define INTERVALO_RAJADA_BOSS 0.001f //Tempo de espera entre um tiro e o próximo configurado como rajada


//==============================================
// Definição de modos de jogo e telas
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
    Vector2 posicao;
    int vida;
    int ativado;
} Inimigo;

typedef struct {
    Rectangle corpo;
    float velocidade;
    int ativo;
    int rebatido;
} Projetil_Inimigo;

//Protótipo das funções do jogo

void InitGame();
void AtualizarJogo();
void DesenharJogo();

#endif
