//Controle dos estados do jogo

#include "game.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>

//==============================================
//             Variáveis globais
//==============================================

Telas ModoDoJogo = tela_menu;
//Aqui eu estou criando uma variável do tipo enum "Telas" para marcar o início ao menu do jogo

int IndiceCenaFinal = 0; //Contador para marcar e contar as cenas que estão sendo passadas ou exibidas no momento
//Essas cenas do final são as que vão ser exibidas após o jogador vencer o jogo

int IndiceCenaTransicaoBoss = 0;

float opacidade_fade_intro = 0.0f;
int fase_fade_intro = 0;
int destino_fade_intro = 0;

float opacidade_fade_final = 0.0f;
int fazendo_fade_final = 0;

float opacidade_fade_intro_boss2 = 1.0f;
float tempo_intro_boss2 = 0.0f;

int efeito_morte_rose = 0;
float raio_luz_morte_rose = 700.0f;

int qtd_linhas_combo_parry = 3;
int qtd_colunas_combo_parry = 4;

int **matriz_dano_parry = NULL;

int linha_combo_parry = 0;
int coluna_combo_parry = 0;

const char *textos_final[] = { //Vetor de textos //Ponteiro para caractere (marcar o começo de uma string)
    "No final, Rose acorda...",
    "E percebe que, no fim, tudo não passava de um 'sonho'.",
    "Ela se levantou, e finalmente adquiriu a coragem que precisava para o show..."
};

const char *textos_transicao_boss[] = {
    "Com muito esforço, o palhaço é derrotado...",
    "Entretanto, uma força maior se aproximava, mais forte, mais assutadora...",
    "'Será que sou capaz disso?...' pensou Rose."
};
// BACKGROUND //
Texture2D background;
Texture2D gameover;
Texture2D menu;
Texture2D intro[6];
int indice_tela_intro = 0;
Texture2D transicao_boss[3];
Texture2D victory[3];
int indice_tela_vitoria = 0;

Music track_menu;
Music track_intro;
Music track_boss1;
Music track_boss2;
Music track_gameover;
Music track_victory;

int musica_atual = 0;

Sound som_clown;
Sound som_hand;
Sound som_marionette;
Sound som_spider;
Sound som_parry;
Sound som_hit_rose;
Sound som_hit_boss;

float cooldown_som_clown = 0.0f;
float cooldown_som_hand = 0.0f;
float cooldown_som_spider = 0.0f;
float cooldown_som_parry = 0.0f;
float cooldown_som_hit_rose = 0.0f;
float cooldown_som_hit_boss = 0.0f;

int contador_som_boss1 = 0;
int intervalo_som_clown = 4;

int som_morte_marionette_tocado = 0;

// ANIMAÇÕES //
animacao boss1_idle;
animacao boss1_attack;
animacao boss1_hurt;
animacao projetil_palhaco;
animacao projetil_palhaco_parry;


int animacao_atual_boss1 = 0;

animacao boss2_idle;
animacao boss2_attack;
animacao boss2_special;
animacao boss2_hurt;
animacao boss2_dying;
animacao boss2_intro;
animacao projetil_marionette;
animacao projetil_marionette_especial;


int animacao_atual_boss2 = 0;

animacao rose_idle;
animacao rose_walk;
animacao rose_jump;
animacao rose_parry;
animacao rose_dying;
animacao rose_attack;

int animacao_atual_rose = 0;


Jogador jogador;
Rectangle ataque_jogador; //Cria uma variável do tipo Rectangle para definir o dano do jogador (Por enquanto...)
float tempo_ataque = 0; //Tempo que o ataque da Rose fica ativo depois de apertar A
float tempo_sem_receber_dano = 0; //Para o jogador não receber dano várias vezes com um mesmp projétil
float tempo_recebendo_dano_jogador = 0; //Tempo, segundos, por isso float, que o jogador fica com o EFEITO VISUAL de dano
int atacando = 0; //Se o jogador está atacando
int aparando = 0; //Se o jogador está aparando
float tempo_incapacitado_jogador = 0; //Tempo em que a Rose fica sem poder atacar nem aparar

Inimigo inimigo;
Projetil *head = NULL;
int avisando_ataque_inimigo = 0; //Confirma se o Boss está atacando (Sim ou não = 1 ou 0), por isso é do tipo int
float tempo_aviso_ataque_inimigo = 0; //Por quanto tempo avisa que o Boss vai atacar, por isso float, segundos
float tempo_recebendo_dano_inimigo = 0; //Tempo, por isso float, que marca o EFEITO VISUAL do inimigo recebendo dano
float tempo_texto_dano = 0; //Tempo em que o Boss leva dano
float cooldown_projetil = 0;
int contador_ataques_inimigo = 0; //Para soltar uma possível "rajada"
int tiros_rajada_restantes = 0; //Controla para saber se falta tiros de rajada extra faltando, exemplo: 1 (tem tiro extra), 0 (não tem tiro extra)
int contador_ataques_especiais_boss2 = 0; //Conta quantos ataques o Boss 2 fez
int proximo_projetil_especial = 0; //Marca se o próximo projétil será normal ou especial

float tempo_parry; //Quanto tempo para o parry dura
float cooldown_parry = 0; //Quanto tempo para o parry poder ser usado de novo
float tempo_texto_parry = 0; //Tempo em que o texto "APAROU!" é exibido

Projetil *AdicionarProjetil(Projetil *head, Projetil_Inimigo dados) {
    Projetil *novo = malloc(sizeof(Projetil));

    if (novo == NULL) {
        printf("Erro ao alocar memoria para projetil.\n");
        return head;
    }

    novo->dados = dados;
    novo->next = NULL;

    if (head == NULL) {
        return novo;
    }

    Projetil *temp = head;

    while (temp->next != NULL) {
        temp = temp->next;
    }

    temp->next = novo;

    return head;
}

void LimparProjeteis(Projetil **head) {
    Projetil *temp = *head;

    while (temp != NULL) {
        Projetil *remover = temp;
        temp = temp->next;
        free(remover);
    }

    *head = NULL;
}

Projetil_Inimigo *PegarProjetilAtual(Projetil *head) {
    if (head == NULL) {
        return NULL;
    }

    return &head->dados;
}
void CriarMatrizDanoParry() {
    if (matriz_dano_parry != NULL) {
        return;
    }

    matriz_dano_parry = malloc(qtd_linhas_combo_parry * sizeof(int *));

    if (matriz_dano_parry == NULL) {
        printf("Erro ao alocar linhas da matriz de dano do parry.\n");
        return;
    }

    for (int i = 0; i < qtd_linhas_combo_parry; i++) {
        *(matriz_dano_parry + i) = malloc(qtd_colunas_combo_parry * sizeof(int));

        if (*(matriz_dano_parry + i) == NULL) {
            printf("Erro ao alocar colunas da matriz de dano do parry.\n");

            for (int j = 0; j < i; j++) {
                free(*(matriz_dano_parry + j));
            }

            free(matriz_dano_parry);
            matriz_dano_parry = NULL;
            return;
        }
    }

    matriz_dano_parry[0][0] = 8;
    matriz_dano_parry[0][1] = 10;
    matriz_dano_parry[0][2] = 12;
    matriz_dano_parry[0][3] = 14;

    matriz_dano_parry[1][0] = 16;
    matriz_dano_parry[1][1] = 18;
    matriz_dano_parry[1][2] = 20;
    matriz_dano_parry[1][3] = 21;

    matriz_dano_parry[2][0] = 22;
    matriz_dano_parry[2][1] = 23;
    matriz_dano_parry[2][2] = 24;
    matriz_dano_parry[2][3] = 25;
}

void LiberarMatrizDanoParry() {
    if (matriz_dano_parry == NULL) {
        return;
    }

    for (int i = 0; i < qtd_linhas_combo_parry; i++) {
        free(matriz_dano_parry[i]);
    }

    free(matriz_dano_parry);
    matriz_dano_parry = NULL;
}
int ObterDanoParry() {
    if (matriz_dano_parry == NULL) {
        return 20;
    }

    return matriz_dano_parry[linha_combo_parry][coluna_combo_parry];
}

void AumentarComboParry() {
    if (coluna_combo_parry < qtd_colunas_combo_parry - 1) {
        coluna_combo_parry++;
    }
    else if (linha_combo_parry < qtd_linhas_combo_parry - 1) {
        linha_combo_parry++;
        coluna_combo_parry = 0;
    }
}

void ReduzirComboParryAoTomarDano() {
    coluna_combo_parry = 0;
}

//Aqui estamos criando as variáveis tanto para personagem quanto para os nossos inimigos (cada um com seus respectivos structs, os de letra maiúscula, para marcar do formato para o objeto)

//==============================================================================================================================================================================================
//                                                                            Inicalização do jogo! 
//==============================================================================================================================================================================================
void ReiniciarAnimacao(animacao *animacao);
void TrocarMusica(int nova_musica);
void IniciarFadeIntro(int destino);
void AtualizarAnimacaoProjetilInimigo();
void AtualizarFadeIntro();
void AtualizarMusicaJogo();
void Boss2(){

    //Aqui a gente tá reiniciando tudo tudo tudo, porque se não fizermos isso o jogador vai chegar com a vida que sobrou do Boss 1, por exemplo, e outras configurações já ativadas que podem prejudicar o controle do jogador

    efeito_morte_rose = 0;
    raio_luz_morte_rose = 700.0f;

    jogador.vida = VIDA_MAX_JOGADOR;
    jogador.posicao = (Vector2){200, COORDENADA_CHAO};
    inimigo.vida = VIDA_MAX_INIMIGO_BOSS2;
    inimigo.posicao = (Vector2){640, COORDENADA_CHAO - 120};

    opacidade_fade_final = 0.0f;
    fazendo_fade_final = 0;

    opacidade_fade_intro_boss2 = 1.0f;
    tempo_intro_boss2 = 0.0f;

    animacao_atual_rose = 0;
    ReiniciarAnimacao(&rose_idle);
    ReiniciarAnimacao(&rose_walk);
    ReiniciarAnimacao(&rose_jump);
    ReiniciarAnimacao(&rose_parry);
    ReiniciarAnimacao(&rose_dying);
    ReiniciarAnimacao(&rose_attack);

    som_morte_marionette_tocado = 0;

    LimparProjeteis(&head);
    linha_combo_parry = 0;
    coluna_combo_parry = 0;

    cooldown_projetil = 0;
    contador_ataques_inimigo = 0;
    tiros_rajada_restantes = 0;

    contador_ataques_especiais_boss2 = 0;
    proximo_projetil_especial = 0;

    tempo_sem_receber_dano = 0;
    tempo_recebendo_dano_jogador = 0;
    tempo_incapacitado_jogador = 0;
    
    animacao_atual_boss2 = 0;
    ReiniciarAnimacao(&boss2_idle);
    ReiniciarAnimacao(&boss2_attack);
    ReiniciarAnimacao(&boss2_special);
    ReiniciarAnimacao(&boss2_hurt);
    ReiniciarAnimacao(&boss2_dying);
    ReiniciarAnimacao(&boss2_intro);
}

void LoadAnimacao (animacao *animacao, const char *pasta, const char *prefixo, int frames_totais, float intervalo, int repetindo){
    animacao -> frames_totais = frames_totais;
    animacao -> frame_atual = 0;
    animacao -> tempo =0;
    animacao -> intervalo = intervalo;
    animacao->repetindo = repetindo;
    animacao->terminou = 0;

    for (int i = 0; i < frames_totais; i++){
        char caminho [200];

        sprintf (caminho, "%s/%s%d.png", pasta, prefixo, i+1);
        animacao -> frames[i] = LoadTexture (caminho);

        if (animacao->frames[i].id == 0) {
            printf("Erro ao carregar textura: %s\n", caminho);
        }

    }
}

void CarregarAssets() {
    background = LoadTexture("assets/background/background.png");
    gameover = LoadTexture("assets/gameover/gameover.png");
    menu = LoadTexture("assets/menu/menu.png");  
    intro[0] = LoadTexture("assets/intro/tela1.png");
    intro[1] = LoadTexture("assets/intro/tela2.png");
    intro[2] = LoadTexture("assets/intro/tela3.png");
    intro[3] = LoadTexture("assets/intro/tela4.png");
    intro[4] = LoadTexture("assets/intro/tela5.png");
    intro[5] = LoadTexture("assets/intro/tela6.png");

    transicao_boss[0] = LoadTexture("assets/transicao/transicao1.png");
    transicao_boss[1] = LoadTexture("assets/transicao/transicao2.png");
    transicao_boss[2] = LoadTexture("assets/transicao/transicao3.png");

    victory[0] = LoadTexture("assets/victory/victory1.png");
    victory[1] = LoadTexture("assets/victory/victory2.png");
    victory[2] = LoadTexture("assets/victory/victory3.png");

    track_menu = LoadMusicStream("assets/music/track_menu.ogg");
    track_intro = LoadMusicStream("assets/music/track_intro.ogg");
    track_boss1 = LoadMusicStream("assets/music/track_boss1.ogg");
    track_boss2 = LoadMusicStream("assets/music/track_boss2.ogg");
    track_gameover = LoadMusicStream("assets/music/track_gameover.ogg");
    track_victory = LoadMusicStream("assets/music/track_victory.ogg");

    track_menu.looping = true;
    track_intro.looping = true;
    track_boss1.looping = true;
    track_boss2.looping = true;
    track_gameover.looping = true;
    track_victory.looping = true;

    som_clown = LoadSound("assets/sfx/clown.ogg");
    som_hand = LoadSound("assets/sfx/hand.ogg");
    som_marionette = LoadSound("assets/sfx/marionette.ogg");
    som_spider = LoadSound("assets/sfx/spider.ogg");
    som_parry = LoadSound("assets/sfx/parry.ogg");
    som_hit_rose = LoadSound("assets/sfx/hit_rose.ogg");
    som_hit_boss = LoadSound("assets/sfx/hit_boss.ogg");

    LoadAnimacao(&boss1_idle, "assets/boss1/idle", "idle", 9, 0.12f, 1);
    LoadAnimacao(&boss1_attack, "assets/boss1/attack", "attack", 9, 0.08f, 0);
    LoadAnimacao(&boss1_hurt, "assets/boss1/hurt", "hurt", 9, 0.08f, 0);
    LoadAnimacao(&projetil_palhaco, "assets/boss1/projectile", "projectile_clown", 2, 0.08f, 1);
    LoadAnimacao(&projetil_palhaco_parry, "assets/boss1/projectile_parry", "p_parry", 5, 0.08f, 1);

    LoadAnimacao(&boss2_idle, "assets/boss2/idle", "idle", 4, 0.12f, 1);
    LoadAnimacao(&boss2_attack, "assets/boss2/attack", "attack", 9, 0.08f, 0);
    LoadAnimacao(&boss2_special, "assets/boss2/special", "special", 9, 0.08f, 0);
    LoadAnimacao(&boss2_hurt, "assets/boss2/hurt", "hurt", 9, 0.08f, 0);
    LoadAnimacao(&boss2_dying, "assets/boss2/dying", "death", 9, 0.10f, 0);
    LoadAnimacao(&boss2_intro, "assets/boss2/intro", "intro", 24, 0.08f, 0);
    LoadAnimacao(&projetil_marionette, "assets/boss2/projectile", "hand", 9, 0.07f, 1);
    LoadAnimacao(&projetil_marionette_especial, "assets/boss2/specialhand", "specialhand", 9, 0.07f, 1);

    LoadAnimacao(&rose_idle, "assets/rose/idle", "idle", 3, 0.18f, 1);
    LoadAnimacao(&rose_walk, "assets/rose/walk", "walk", 3, 0.12f, 1);
    LoadAnimacao(&rose_jump, "assets/rose/jump", "jump", 5, 0.10f, 0);
    LoadAnimacao(&rose_parry, "assets/rose/parry", "parry", 3, 0.11f, 0);
    LoadAnimacao(&rose_dying, "assets/rose/dying", "dying", 9, 0.10f, 0);
    LoadAnimacao(&rose_attack, "assets/rose/attack", "attack", 9, 0.06f, 0);
    
    TrocarMusica(1);
}


void UnloadAnimacao(animacao *animacao) {
    for (int i = 0; i < animacao->frames_totais; i++) {
        UnloadTexture(animacao->frames[i]);
    }
}

void DescarregarAssets() {
    UnloadAnimacao(&boss1_idle);
    UnloadAnimacao(&boss1_attack);
    UnloadAnimacao(&boss1_hurt);
    UnloadAnimacao(&projetil_palhaco);
    UnloadAnimacao(&projetil_palhaco_parry);

    UnloadAnimacao(&boss2_idle);
    UnloadAnimacao(&boss2_attack);
    UnloadAnimacao(&boss2_special);
    UnloadAnimacao(&boss2_hurt);
    UnloadAnimacao(&boss2_dying);
    UnloadAnimacao(&boss2_intro);
    UnloadAnimacao(&projetil_marionette);
    UnloadAnimacao(&projetil_marionette_especial);

    UnloadAnimacao(&rose_idle);
    UnloadAnimacao(&rose_walk);
    UnloadAnimacao(&rose_jump);
    UnloadAnimacao(&rose_parry);
    UnloadAnimacao(&rose_dying);
    UnloadAnimacao(&rose_attack);

    UnloadTexture(background);
    UnloadTexture(gameover);
    UnloadTexture(menu);
    for (int i = 0; i < 6; i++) {
    UnloadTexture(intro[i]);
    for (int i = 0; i < 3; i++) {
    UnloadTexture(transicao_boss[i]);
    for (int i = 0; i < 3; i++) {
    UnloadTexture(victory[i]);
}
}

    UnloadMusicStream(track_menu);
    UnloadMusicStream(track_intro);
    UnloadMusicStream(track_boss1);
    UnloadMusicStream(track_boss2);
    UnloadMusicStream(track_gameover);
    UnloadMusicStream(track_victory);
    UnloadSound(som_clown);
    UnloadSound(som_hand);
    UnloadSound(som_marionette);
    UnloadSound(som_spider);
    UnloadSound(som_parry);
    UnloadSound(som_hit_rose);
    UnloadSound(som_hit_boss);

    LiberarMatrizDanoParry();
}
}

void AtualizarAnimacao(animacao *animacao) {
    if (animacao->terminou) {
        return;
    }

    animacao->tempo += GetFrameTime();

    if (animacao->tempo >= animacao->intervalo) {
        animacao->tempo = 0;
        animacao->frame_atual++;

        if (animacao->frame_atual >= animacao->frames_totais) {
            if (animacao->repetindo) {
                animacao->frame_atual = 0;
            } else {
                animacao->frame_atual = animacao->frames_totais - 1;
                animacao->terminou = 1;
            }
        }
    }
}

void ReiniciarAnimacao(animacao *animacao) {
    animacao->frame_atual = 0;
    animacao->tempo = 0;
    animacao->terminou = 0;
}

void TrocarAnimacaoBoss1(int nova_animacao) {

    // Se o boss está atacando, o hurt não pode interromper
    if (animacao_atual_boss1 == 1 && nova_animacao == 2) {
        return;
    }

    // Se já está na mesma animação
    if (animacao_atual_boss1 == nova_animacao) {

        // Mas se for outro ataque, reinicia o ataque
        if (nova_animacao == 1) {
            ReiniciarAnimacao(&boss1_attack);
        }

        return;
    }

    animacao_atual_boss1 = nova_animacao;

    if (nova_animacao == 1) {
        ReiniciarAnimacao(&boss1_attack);
    }
    else if (nova_animacao == 2) {
        ReiniciarAnimacao(&boss1_hurt);
    }
}

void TrocarAnimacaoBoss2(int nova_animacao) {

    // Se o Boss 2 está atacando, hurt não interrompe
    if ((animacao_atual_boss2 == 1 || animacao_atual_boss2 == 2) && nova_animacao == 3) {
        return;
    }

    if (animacao_atual_boss2 == nova_animacao) {

        if (nova_animacao == 1) {
            ReiniciarAnimacao(&boss2_attack);
        }
        else if (nova_animacao == 2) {
            ReiniciarAnimacao(&boss2_special);
        }
        else if (nova_animacao == 3) {
            ReiniciarAnimacao(&boss2_hurt);
        }

        return;
    }

    animacao_atual_boss2 = nova_animacao;

    if (nova_animacao == 1) {
        ReiniciarAnimacao(&boss2_attack);
    }
    else if (nova_animacao == 2) {
        ReiniciarAnimacao(&boss2_special);
    }
    else if (nova_animacao == 3) {
        ReiniciarAnimacao(&boss2_hurt);
    }
    else if (nova_animacao == 4) {
        ReiniciarAnimacao(&boss2_dying);
    }
}

void TrocarAnimacaoRose(int nova_animacao) {

    
    if (animacao_atual_rose == 5 && !rose_attack.terminou && nova_animacao != 4) {
        return;
    }

  
    if (animacao_atual_rose == 3 && !rose_parry.terminou && nova_animacao != 4 && nova_animacao != 5) {
        return;
    }

    if (animacao_atual_rose == nova_animacao) {
        return;
    }

    animacao_atual_rose = nova_animacao;

    if (nova_animacao == 0) {
        ReiniciarAnimacao(&rose_idle);
    }
    else if (nova_animacao == 1) {
        ReiniciarAnimacao(&rose_walk);
    }
    else if (nova_animacao == 2) {
        ReiniciarAnimacao(&rose_jump);
    }
    else if (nova_animacao == 3) {
        ReiniciarAnimacao(&rose_parry);
    }
    else if (nova_animacao == 4) {
        ReiniciarAnimacao(&rose_dying);
    }
    else if (nova_animacao == 5) {
        ReiniciarAnimacao(&rose_attack);
    }
}

void TocarSomComCooldown(Sound som, float *cooldown, float tempo_cooldown) {
    if (*cooldown <= 0.0f) {
        if (IsSoundPlaying(som)) {
            StopSound(som);
        }

        PlaySound(som);
        *cooldown = tempo_cooldown;
    }
}

void TocarSomClown() {
    TocarSomComCooldown(som_clown, &cooldown_som_clown, 0.12f);
}

void TocarSomHand() {
    TocarSomComCooldown(som_hand, &cooldown_som_hand, 0.10f);
}

void TocarSomSpider() {
    TocarSomComCooldown(som_spider, &cooldown_som_spider, 0.10f);
}

void TocarSomParry() {
    TocarSomComCooldown(som_parry, &cooldown_som_parry, 0.10f);
}

void TocarSomHitRose() {
    TocarSomComCooldown(som_hit_rose, &cooldown_som_hit_rose, 0.08f);
}

void TocarSomHitBoss() {
    TocarSomComCooldown(som_hit_boss, &cooldown_som_hit_boss, 0.12f);
}

void TocarSomAtaqueBoss1() {
    contador_som_boss1++;

    if (contador_som_boss1 >= intervalo_som_clown) {
        TocarSomClown();

        contador_som_boss1 = 0;
        intervalo_som_clown = GetRandomValue(3, 4);
    }
    else {
        TocarSomSpider();
    }
}

void AtualizarCooldownsSons() {
    float dt = GetFrameTime();

    if (cooldown_som_clown > 0.0f) cooldown_som_clown -= dt;
    if (cooldown_som_hand > 0.0f) cooldown_som_hand -= dt;
    if (cooldown_som_spider > 0.0f) cooldown_som_spider -= dt;
    if (cooldown_som_parry > 0.0f) cooldown_som_parry -= dt;
    if (cooldown_som_hit_rose > 0.0f) cooldown_som_hit_rose -= dt;
    if (cooldown_som_hit_boss > 0.0f) cooldown_som_hit_boss -= dt;
}

void InitGame(){ //Aqui é onde inicializamos todas as variáveis globais, por exemplo, para o jogo poder começar de fato

    CriarMatrizDanoParry();

    linha_combo_parry = 0;
    coluna_combo_parry = 0;
    
    efeito_morte_rose = 0;
    raio_luz_morte_rose = 700.0f;

    jogador.posicao = (Vector2){400, COORDENADA_CHAO}; //X = 400 (posição horizontal) e Y = COORDENADA_CHAO (começa no)
    inimigo.posicao = (Vector2){650, COORDENADA_CHAO - 100};

    //Coordenadas do jogador

    jogador.vida = VIDA_MAX_JOGADOR;
    jogador.no_chao = 1;
    jogador.velocidade_pulo = 0;

    animacao_atual_rose = 0;
    ReiniciarAnimacao(&rose_idle);
    ReiniciarAnimacao(&rose_walk);
    ReiniciarAnimacao(&rose_jump);
    ReiniciarAnimacao(&rose_parry);
    ReiniciarAnimacao(&rose_dying);
    ReiniciarAnimacao(&rose_attack);

    ataque_jogador = (Rectangle){0,0,LARGURA_ATAQUE,ALTURA_ATAQUE};
    tempo_ataque = 0;
    atacando = 0;

    aparando = 0;
    tempo_parry = 0;
    cooldown_parry = 0;
    tempo_texto_parry = 0;

    tempo_recebendo_dano_jogador = 0;
    tempo_recebendo_dano_inimigo = 0;
    tempo_incapacitado_jogador = 0;
    tempo_texto_dano = 0;

    avisando_ataque_inimigo = 0;
    tempo_aviso_ataque_inimigo = 0;

    contador_ataques_inimigo = 0;
    tiros_rajada_restantes = 0;

    inimigo.vida = VIDA_MAX_INIMIGO;
    inimigo.ativado = 1;

    contador_som_boss1 = 0;
    intervalo_som_clown = GetRandomValue(3, 4);

    LimparProjeteis(&head);
    
    //A ideia é que o Boss não comece atirando, ele espera um tempo e começa a atirar

    LimparProjeteis(&head);
    cooldown_projetil = TEMPO_COOLDOWN_PROJETIL_FASE1;

    tempo_sem_receber_dano = 0;
    
    animacao_atual_boss1 = 0;
    ReiniciarAnimacao(&boss1_idle);
    ReiniciarAnimacao(&boss1_attack);
    ReiniciarAnimacao(&boss1_hurt); //aqui reiniciamos a animação para idle para n ter conflito ao iniciar de novo

    //Em c não temos boleano, então a gente define 1 para estar ativo e 0 para desativado.

}

//Define vida, posição e movimentos do personagem e do inimigo básicos

void AtualizarMorteRoseComLuz() {
    jogador.vida = 0;

    atacando = 0;
    aparando = 0;
    LimparProjeteis(&head);
    avisando_ataque_inimigo = 0;

    if (!efeito_morte_rose) {
        efeito_morte_rose = 1;
        raio_luz_morte_rose = 700.0f;
    }

    TrocarAnimacaoRose(4);
    AtualizarAnimacao(&rose_dying);

    raio_luz_morte_rose -= GetFrameTime() * 260.0f;

    if (raio_luz_morte_rose < 65.0f) {
        raio_luz_morte_rose = 65.0f;
    }

    if (rose_dying.terminou && raio_luz_morte_rose <= 70.0f) {
        efeito_morte_rose = 0;
        TrocarMusica(4);
        ModoDoJogo = tela_GameOver;
    }
}

void TrocarMusica(int nova_musica) {
    if (musica_atual == nova_musica) {
        return;
    }

    if (musica_atual == 1) {
        StopMusicStream(track_menu);
    }
    else if (musica_atual == 2) {
        StopMusicStream(track_boss1);
    }
    else if (musica_atual == 3) {
        StopMusicStream(track_boss2);
    }
    else if (musica_atual == 4) {
        StopMusicStream(track_gameover);
    }
    else if (musica_atual == 5) {
        StopMusicStream(track_intro);
    }
    else if (musica_atual == 6) {
        StopMusicStream(track_victory);
    }


    musica_atual = nova_musica;

    if (musica_atual == 1) {
        PlayMusicStream(track_menu);
    }
    else if (musica_atual == 2) {
        PlayMusicStream(track_boss1);
    }
    else if (musica_atual == 3) {
        PlayMusicStream(track_boss2);
    }
    else if (musica_atual == 4) {
        PlayMusicStream(track_gameover);
    }
    else if (musica_atual == 5) {
        PlayMusicStream(track_intro);
    }
    else if (musica_atual == 6) {
        PlayMusicStream(track_victory);
    }
}

void AtualizarMusicaJogo() {
    if (musica_atual == 1) {
        UpdateMusicStream(track_menu);
    }
    else if (musica_atual == 2) {
        UpdateMusicStream(track_boss1);
    }
    else if (musica_atual == 3) {
        UpdateMusicStream(track_boss2);
    }
    else if (musica_atual == 4) {
        UpdateMusicStream(track_gameover);
    }
    else if (musica_atual == 5) {
        UpdateMusicStream(track_intro);
    }
    else if (musica_atual == 6) {
        UpdateMusicStream(track_victory);
    }
}

void IniciarFadeIntro(int destino) {
    if (fase_fade_intro != 0) {
        return;
    }

    destino_fade_intro = destino;
    fase_fade_intro = 1;
    opacidade_fade_intro = 0.0f;
}

void AtualizarFadeIntro() {
    if (fase_fade_intro == 0) {
        return;
    }

    if (fase_fade_intro == 1) {
        opacidade_fade_intro += GetFrameTime() / 0.45f;

        if (opacidade_fade_intro >= 1.0f) {
            opacidade_fade_intro = 1.0f;

            if (destino_fade_intro == 1) {
                indice_tela_intro = 0;
                TrocarMusica(5);
                ModoDoJogo = tela_animacaoIntro;
            }
            else if (destino_fade_intro == 2) {
                if (indice_tela_intro < 5) {
                    indice_tela_intro++;
                }
            }
            else if (destino_fade_intro == 3) {
                InitGame();
                TrocarMusica(2);
                ModoDoJogo = tela_batalha_boss1;
            }
            else if (destino_fade_intro == 4) {
                if (IndiceCenaTransicaoBoss < TOTAL_CENAS_TRANSICAO_BOSS - 1) {
                    IndiceCenaTransicaoBoss++;
                }
            }
            else if (destino_fade_intro == 5) {
                Boss2();
                TrocarMusica(3);
                ModoDoJogo = tela_intro_boss2;
            }
            else if (destino_fade_intro == 6) {
                if (indice_tela_vitoria < 2) {
                    indice_tela_vitoria++;
                }
                else {
                    InitGame();
                    TrocarMusica(1);
                    indice_tela_vitoria = 0;
                    ModoDoJogo = tela_menu;
                }
            }

            fase_fade_intro = 2;
        }
    }
    else if (fase_fade_intro == 2) {
        opacidade_fade_intro -= GetFrameTime() / 0.45f;

        if (opacidade_fade_intro <= 0.0f) {
            opacidade_fade_intro = 0.0f;
            fase_fade_intro = 0;
            destino_fade_intro = 0;
        }
    }
}

void AtualizarAnimacaoProjetilInimigo() {
    Projetil_Inimigo *p = PegarProjetilAtual(head);

    if (p == NULL) {
        return;
    }

    if (ModoDoJogo == tela_batalha_boss1) {
        if (p->rebatido) {
            AtualizarAnimacao(&projetil_palhaco_parry);
        }
        else {
            AtualizarAnimacao(&projetil_palhaco);
        }
    }
    else if (ModoDoJogo == tela_batalha_boss2) {
        if (p->especial) {
            AtualizarAnimacao(&projetil_marionette_especial);
        }
        else {
            AtualizarAnimacao(&projetil_marionette);
        }
    }
}

void AtualizarJogo(){
    AtualizarAnimacaoProjetilInimigo();
    AtualizarMusicaJogo();
    AtualizarFadeIntro();
    AtualizarCooldownsSons();

    if (fase_fade_intro != 0) {
        return;
    }

    switch (ModoDoJogo){

        case tela_menu:
            if (IsKeyPressed(KEY_ENTER)){
                IniciarFadeIntro(1);
            }

            break;

        case tela_animacaoIntro:
            if (IsKeyPressed(KEY_ENTER)){
                if (indice_tela_intro < 5){
                    IniciarFadeIntro(2);
                }
                else {
                    IniciarFadeIntro(3);
                }
            }

            break;

         case tela_batalha_boss1:{

            if (jogador.vida <= 0) {
                AtualizarMorteRoseComLuz();
                break;
            }

            int rose_esta_andando = 0;

            if (IsKeyDown(KEY_RIGHT)) {
                jogador.posicao.x += 2;
                rose_esta_andando = 1;
            }

            if (IsKeyDown(KEY_LEFT)) {
                jogador.posicao.x -= 2;
                rose_esta_andando = 1;
            }

            //Limite o jogador dentro da tela

            if (jogador.posicao.x < 20) jogador.posicao.x = 20;
            if (jogador.posicao.x > LARGURA_TELA - 20) jogador.posicao.x = LARGURA_TELA - 20;



            //Se eu estou pressionando A e eu não estava atacando...

            if (IsKeyPressed(KEY_A) && !atacando && !aparando && tempo_incapacitado_jogador <= 0){
                atacando = 1;
                tempo_ataque = TEMPO_ATAQUE;
                TrocarAnimacaoRose(5);

                ataque_jogador = (Rectangle){
                    jogador.posicao.x + 20,
                    jogador.posicao.y - 30,
                    LARGURA_ATAQUE,
                    ALTURA_ATAQUE
                };

                Rectangle corpo_inimigo = {
                    inimigo.posicao.x,
                    inimigo.posicao.y,
                    80,
                    100
                };

                if (CheckCollisionRecs(ataque_jogador, corpo_inimigo)){
                    TocarSomHitRose();
                    inimigo.vida -= 10;
                    tempo_recebendo_dano_inimigo = 0.15f;
                    tempo_texto_dano = 0.3f;
                    TrocarAnimacaoBoss1(2);
                
                }
            }

            if (atacando){ //Se o jogador estiver atacando
                tempo_ataque -= GetFrameTime(); //Vai diminuindo o tempo restante do ataque a cada frame para durar exatamente o TEMPO_ATAQUE da constante

                if (tempo_ataque <= 0){ //Se o tempo do ataque for menor ou igual a zero, ou seja, se o jogador NÃO estiver atacando
                    atacando = 0; //O ataque zera novamente
                }
            }

            if (IsKeyPressed(KEY_SPACE) && jogador.no_chao){

                jogador.velocidade_pulo = INTENSIDADE_PULO;
                jogador.no_chao = 0;

                //Se eu pressionar a tecla de espaço e o jogador estiver no chão,
                //ele recebe velocidade para cima e não vai estar no chão, o famoso PULAAR
                TrocarAnimacaoRose(2);
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
            if (!jogador.no_chao) {
                TrocarAnimacaoRose(2);
            }
            else if (rose_esta_andando) {
                TrocarAnimacaoRose(1);
            }
            else {
                TrocarAnimacaoRose(0);
            }

            //Cooldown do parry

            if (cooldown_parry > 0){
                cooldown_parry -= GetFrameTime();
            }

            //Tempo texto "APAROU!"

            if (tempo_texto_parry > 0){
                tempo_texto_parry -= GetFrameTime();
            }

            //Tempo recebendo dano no jogador

            if (tempo_recebendo_dano_jogador > 0){
                tempo_recebendo_dano_jogador -= GetFrameTime();
            }

            //Tempo do texto "ATACOU!"

            if (tempo_texto_dano > 0){
                tempo_texto_dano -= GetFrameTime();
            }

            //Tempo visual de dano no inimigo

            if (tempo_recebendo_dano_inimigo > 0){
                tempo_recebendo_dano_inimigo -= GetFrameTime();
            }

            //Ativar parry com Q

            if (IsKeyPressed(KEY_Q) && cooldown_parry <= 0 && !aparando && !atacando){
                aparando = 1;
                tempo_parry = TEMPO_PARRY;
                cooldown_parry = COOLDOWN_PARRY;
                TrocarAnimacaoRose(3);
            }

            //Tempo ativo do parry

            if (aparando){
                tempo_parry -= GetFrameTime();

                if (tempo_parry <=0){
                    aparando = 0;
                }
            }

            float velocidade_atual_projetil = VELOCIDADE_PROJETIL_FASE1;
            float tempo_cooldown_atual_projetil = TEMPO_COOLDOWN_PROJETIL_FASE1;

            if (inimigo.vida <= VIDA_MAX_INIMIGO / 2){
                velocidade_atual_projetil = VELOCIDADE_PROJETIL_FASE2;
                tempo_cooldown_atual_projetil = TEMPO_COOLDOWN_PROJETIL_FASE2;
            }

            //Cooldown do Boss

            if (cooldown_projetil > 0){
                cooldown_projetil -= GetFrameTime();
            }

            //Lógica projétil

            if (head == NULL && !avisando_ataque_inimigo && cooldown_projetil <= 0){ //Se acabou o cooldown e o projétil não está ativo, então ele ativa e o Boss dispara

                int ataque_de_rajada = 0; //Aqui é só uma marcação ou variável temporária para dizer que o ataque que está por vir não é do tipo rajada
                // = 0, ataque normal. Se fosse = 1, seria ataque rajada, mas por enquanto é normal porque falta a condição para coemçar os rajadas.

                if (tiros_rajada_restantes > 0){ //Se existe rajada restante, guardada ou esperando
                    tiros_rajada_restantes--; //Usa o tiro rajada
                    ataque_de_rajada = 1; //Simboliza ou marca que esse ataque é rajada
                } //Se não tem rajada, conta ataques normais

                else if (inimigo.vida <= VIDA_MAX_INIMIGO / 2){ 
                    contador_ataques_inimigo++; //Começa a contar a quantidade de ataques

                    if (contador_ataques_inimigo >= QUANTIDADE_DE_ATAQUES_PARA_RAJADA_BOSS1){ //Se a quantidade de ataques for igual a quantidade de ataques para a rajada 
                        tiros_rajada_restantes = 1; //Ele marca o ataque como rajada
                        contador_ataques_inimigo = 0; //Zera o contador para contar de novo
                    }
                }

                avisando_ataque_inimigo = 1;
                TrocarAnimacaoBoss1(1);

                if (ataque_de_rajada){
                    tempo_aviso_ataque_inimigo = INTERVALO_RAJADA_BOSS;
                }else{
                    tempo_aviso_ataque_inimigo = TEMPO_AVISO_ATAQUE_INIMIGO;
                }
            }

            if (avisando_ataque_inimigo){
                tempo_aviso_ataque_inimigo -= GetFrameTime();

                if (tempo_aviso_ataque_inimigo <= 0){
                    avisando_ataque_inimigo = 0;

                    Projetil_Inimigo novo_projetil;

                    novo_projetil.ativo = 1;
                    novo_projetil.rebatido = 0;
                    novo_projetil.especial = 0;
                    novo_projetil.velocidade = velocidade_atual_projetil;

                    novo_projetil.corpo = (Rectangle){
                        inimigo.posicao.x,
                        GetRandomValue(ALTURA_PROJETIL_MIN, ALTURA_PROJETIL_MAX),
                        LARGURA_PROJETIL,
                        ALTURA_PROJETIL
                    };

                    head = AdicionarProjetil(head, novo_projetil);
                    TocarSomAtaqueBoss1();
                    ReiniciarAnimacao(&projetil_palhaco);
                }
            }

            //Se o projétil está ativo, ele anda para a esquerda (eixo x negativo)

            Projetil_Inimigo *p = PegarProjetilAtual(head);

            if (p != NULL){

                if (p->rebatido){
                    p->corpo.x += velocidade_atual_projetil;
                }else{
                    p->corpo.x -= velocidade_atual_projetil;
                }

                if (!p->rebatido && p->corpo.x < -50){
                    LimparProjeteis(&head);

                    if (tiros_rajada_restantes > 0){
                        cooldown_projetil = INTERVALO_RAJADA_BOSS;
                    }else{
                        cooldown_projetil = tempo_cooldown_atual_projetil;
                    }
                }
                else if (p->rebatido && p->corpo.x > LARGURA_TELA + 50){
                    LimparProjeteis(&head);

                    if (tiros_rajada_restantes > 0){
                        cooldown_projetil = INTERVALO_RAJADA_BOSS;
                    }else{
                        cooldown_projetil = tempo_cooldown_atual_projetil;
                    }
                }
            }

            //Tempo invencível após receber o dano

            if (tempo_sem_receber_dano > 0){
                tempo_sem_receber_dano -= GetFrameTime(); //GetFrameTime é da Raylib, e representa o tempo entre um frame e outro, nesse caso é para marcar o tempo de receber dano apenas quando enconsta

            }

           
            //Lógica da colisão do projétil com o parry, testa se o jogador aparou

            Projetil_Inimigo *p_parry = PegarProjetilAtual(head);

            if (p_parry != NULL && aparando && !p_parry->rebatido && CheckCollisionCircleRec(jogador.posicao, AREA_PARRY, p_parry->corpo)){
                p_parry->rebatido = 1;
                TocarSomParry();
                ReiniciarAnimacao(&projetil_palhaco_parry);
                tempo_texto_parry = 0.4f;
                aparando = 0;
            }

            Rectangle corpo_inimigo_rebatido = {
                inimigo.posicao.x,
                inimigo.posicao.y,
                80,
                100
            };

            Projetil_Inimigo *p_colisao = PegarProjetilAtual(head);

            //Quando o projétil rebatido acerta o boss
            if (p_colisao != NULL && p_colisao->rebatido && CheckCollisionRecs(p_colisao->corpo, corpo_inimigo_rebatido)){
                inimigo.vida -= 20;
                TocarSomHitRose();
                AumentarComboParry();
                tempo_recebendo_dano_inimigo = 0.15f;
                tempo_texto_dano = 0.3f;
                TrocarAnimacaoBoss1(2);

                LimparProjeteis(&head);

                float distancia_horizontal_boss = inimigo.posicao.x - jogador.posicao.x;

                if (distancia_horizontal_boss < 0) {
                    distancia_horizontal_boss = distancia_horizontal_boss * -1;
                }

                if (tiros_rajada_restantes > 0){
                    cooldown_projetil = INTERVALO_RAJADA_BOSS;
                }
                else if (distancia_horizontal_boss < 180){
                    cooldown_projetil = tempo_cooldown_atual_projetil + 0.5f;
                }
                else{
                    cooldown_projetil = tempo_cooldown_atual_projetil;
                }
            }

            //Testa se o jogador tomou dano ou Quando a Rose tomar dano
            else if (p_colisao != NULL && !p_colisao->rebatido && CheckCollisionCircleRec(jogador.posicao, 20, p_colisao->corpo) && tempo_sem_receber_dano <=0){
                TocarSomHitBoss();
                jogador.vida -= 12;
                ReduzirComboParryAoTomarDano();
                tempo_sem_receber_dano = 1.0f;
                tempo_recebendo_dano_jogador = 0.2f;

                LimparProjeteis(&head);

                float distancia_horizontal_boss = inimigo.posicao.x - jogador.posicao.x;

                if (distancia_horizontal_boss < 0) {
                    distancia_horizontal_boss = distancia_horizontal_boss * -1;
                }

                if (tiros_rajada_restantes > 0){
                    cooldown_projetil = INTERVALO_RAJADA_BOSS;
                }
                else if (distancia_horizontal_boss < 180){
                    cooldown_projetil = tempo_cooldown_atual_projetil + 0.6f;
                }
                else{
                    cooldown_projetil = tempo_cooldown_atual_projetil;
                }
            }

            //Inimigo perdeu ou Jogador perdeu

            if (jogador.vida <= 0){
                jogador.vida = 0;
                atacando = 0;
                aparando = 0;
                LimparProjeteis(&head);
                TrocarAnimacaoRose(4);
            }

            else if (inimigo.vida <= 0){
                IndiceCenaTransicaoBoss = 0;
                ModoDoJogo = tela_transicao_boss;
            }
            
            
            if (animacao_atual_rose == 1) {
                AtualizarAnimacao(&rose_walk);
            }
            else if (animacao_atual_rose == 2) {
                AtualizarAnimacao(&rose_jump);
            }
            else if (animacao_atual_rose == 3) {
                AtualizarAnimacao(&rose_parry);
            }
            else if (animacao_atual_rose == 4) {
                AtualizarAnimacao(&rose_dying);
            }
            else if (animacao_atual_rose == 5) {
                AtualizarAnimacao(&rose_attack);
            }
            else {
                AtualizarAnimacao(&rose_idle);
            }

            
            if (animacao_atual_boss1 == 1) {
                AtualizarAnimacao(&boss1_attack);

                if (boss1_attack.terminou) {
                    animacao_atual_boss1 = 0;
                }
            }
            else if (animacao_atual_boss1 == 2) {
                AtualizarAnimacao(&boss1_hurt);

                if (boss1_hurt.terminou) {
                    animacao_atual_boss1 = 0;
                }
            }
            else {
                AtualizarAnimacao(&boss1_idle);
            }
            break;
        
         }

            //Fim da batalha
        case tela_intro_boss2:{

            AtualizarAnimacao(&rose_idle);
            AtualizarAnimacao(&boss2_intro);

            tempo_intro_boss2 += GetFrameTime();

            if (opacidade_fade_intro_boss2 > 0.0f) {
                opacidade_fade_intro_boss2 -= GetFrameTime() / 1.2f;

                if (opacidade_fade_intro_boss2 < 0.0f) {
                    opacidade_fade_intro_boss2 = 0.0f;
                }
            }
            
            if ((boss2_intro.terminou && opacidade_fade_intro_boss2 <= 0.0f) || IsKeyPressed(KEY_ENTER)) {
                opacidade_fade_intro_boss2 = 0.0f;
                tempo_intro_boss2 = 0.0f;

                cooldown_projetil = TEMPO_COOLDOWN_BOSS2_FASE1;

                ModoDoJogo = tela_batalha_boss2;
            }

            break;
        }



        case tela_batalha_boss2:{
            
            if (jogador.vida <= 0) {
                AtualizarMorteRoseComLuz();
                break;
            }

            if (inimigo.vida <= 0) {
                inimigo.vida = 0;

                atacando = 0;
                aparando = 0;
                LimparProjeteis(&head);
                avisando_ataque_inimigo = 0;

                if (animacao_atual_boss2 != 4) {
                    if (!som_morte_marionette_tocado) {
                        PlaySound(som_marionette);
                        som_morte_marionette_tocado = 1;
                    }

                    TrocarAnimacaoBoss2(4);
                }

                if (!boss2_dying.terminou) {
                    AtualizarAnimacao(&boss2_dying);
                }
                else {
                    fazendo_fade_final = 1;
                }

                if (fazendo_fade_final) {
                    opacidade_fade_final += GetFrameTime() / 1.5f;

                    if (opacidade_fade_final >= 1.0f) {
                        opacidade_fade_final = 0.0f;
                        fazendo_fade_final = 0;

                        IndiceCenaFinal = 0;
                        TrocarMusica(6);
                        ModoDoJogo = tela_vitoria;
                    }
                }

                break;
            }

            int rose_esta_andando = 0;

            if (IsKeyDown(KEY_RIGHT)) {
                jogador.posicao.x += 2;
                rose_esta_andando = 1;
            }

            if (IsKeyDown(KEY_LEFT)) {
                jogador.posicao.x -= 2;
                rose_esta_andando = 1;
            }

            //Limite o jogador dentro da tela

            if (jogador.posicao.x < 20) jogador.posicao.x = 20;
            if (jogador.posicao.x > LARGURA_TELA - 20) jogador.posicao.x = LARGURA_TELA - 20;

            if (tempo_incapacitado_jogador > 0){
                tempo_incapacitado_jogador -= GetFrameTime();

                if (tempo_incapacitado_jogador < 0){
                    tempo_incapacitado_jogador = 0;
                }
            } 

            //Se eu estou pressionando A e eu não estava atacando...

            if (IsKeyPressed(KEY_A) && !atacando && !aparando && tempo_incapacitado_jogador <= 0){
                atacando = 1;
                tempo_ataque = TEMPO_ATAQUE;
                TrocarAnimacaoRose(5);

                ataque_jogador = (Rectangle){
                    jogador.posicao.x + 20,
                    jogador.posicao.y - 30,
                    LARGURA_ATAQUE,
                    ALTURA_ATAQUE
                };

                Rectangle corpo_inimigo = {
                    inimigo.posicao.x - 90,
                    inimigo.posicao.y + 20,
                    120,
                    180
                };

                if (CheckCollisionRecs(ataque_jogador, corpo_inimigo)){
                    TocarSomHitRose();
                    inimigo.vida -= 10;
                    tempo_recebendo_dano_inimigo = 0.15f;
                    tempo_texto_dano = 0.3f;
                    TrocarAnimacaoBoss2(3);
                }
            }

            if (atacando){ //Se o jogador estiver atacando
                tempo_ataque -= GetFrameTime(); //Vai diminuindo o tempo restante do ataque a cada frame para durar exatamente o TEMPO_ATAQUE da constante

                if (tempo_ataque <= 0){ //Se o tempo do ataque for menor ou igual a zero, ou seja, se o jogador NÃO estiver atacando
                    atacando = 0; //O ataque zera novamente
                }
            }

            if (IsKeyPressed(KEY_SPACE) && jogador.no_chao){

                jogador.velocidade_pulo = INTENSIDADE_PULO;
                jogador.no_chao = 0;

                //Se eu pressionar a tecla de espaço e o jogador estiver no chão,
                //ele recebe velocidade para cima e não vai estar no chão, o famoso PULAAR
                TrocarAnimacaoRose(2);

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
            if (!jogador.no_chao) {
                TrocarAnimacaoRose(2);
            }
            else if (rose_esta_andando) {
                TrocarAnimacaoRose(1);
            }
            else {
                TrocarAnimacaoRose(0);
            }

            //Cooldown do parry

            if (cooldown_parry > 0){
                cooldown_parry -= GetFrameTime();
            }

            //Tempo texto "APAROU!"

            if (tempo_texto_parry > 0){
                tempo_texto_parry -= GetFrameTime();
            }

            //Tempo recebendo dano no jogador

            if (tempo_recebendo_dano_jogador > 0){
                tempo_recebendo_dano_jogador -= GetFrameTime();
            }

            //Tempo do texto "ATACOU!"

            if (tempo_texto_dano > 0){
                tempo_texto_dano -= GetFrameTime();
            }

            //Tempo visual de dano no inimigo

            if (tempo_recebendo_dano_inimigo > 0){
                tempo_recebendo_dano_inimigo -= GetFrameTime();
            }

            //Ativar parry com Q

            if (IsKeyPressed(KEY_Q) && cooldown_parry <= 0 && !aparando && !atacando && tempo_incapacitado_jogador <= 0){
                aparando = 1;
                tempo_parry = TEMPO_PARRY;
                cooldown_parry = COOLDOWN_PARRY;
                TrocarAnimacaoRose(3);
            }

            //Tempo ativo do parry

            if (aparando){
                tempo_parry -= GetFrameTime();

                if (tempo_parry <=0){
                    aparando = 0;
                }
            }

            float velocidade_atual_projetil = VELOCIDADE_PROJETIL_FASE1_BOSS2;
            float tempo_cooldown_atual_projetil = TEMPO_COOLDOWN_BOSS2_FASE1;

            int quantidade_ataques_para_rajada_atual = QUANTIDADE_DE_ATAQUES_PARA_RAJADA_BOSS2_FASE1;
            int tiros_extras_rajada_atual = TIROS_EXTRAS_RAJADA_BOSS2_FASE1;

            if (inimigo.vida <= VIDA_MAX_INIMIGO_BOSS2 / 2){
                velocidade_atual_projetil = VELOCIDADE_PROJETIL_FASE2_BOSS2;
                tempo_cooldown_atual_projetil = TEMPO_COOLDOWN_BOSS2_FASE2;

                quantidade_ataques_para_rajada_atual = QUANTIDADE_DE_ATAQUES_PARA_RAJADA_BOSS2_FASE2;
                tiros_extras_rajada_atual = TIROS_EXTRAS_RAJADA_BOSS2_FASE2;
            }

            //Cooldown do Boss

            if (cooldown_projetil > 0){
                cooldown_projetil -= GetFrameTime();
            }

            //Lógica projétil

            if (head == NULL && !avisando_ataque_inimigo && cooldown_projetil <= 0){
                
                int ataque_de_rajada = 0;

                if (tiros_rajada_restantes > 0){
                    tiros_rajada_restantes--;
                    ataque_de_rajada = 1;
                }
                else{
                    contador_ataques_inimigo++;

                    if (contador_ataques_inimigo >= quantidade_ataques_para_rajada_atual){
                        tiros_rajada_restantes = tiros_extras_rajada_atual;
                        contador_ataques_inimigo = 0;
                    }
                }

                //Ataque de silenciamento do Boss 2
                contador_ataques_especiais_boss2++;

                if (contador_ataques_especiais_boss2 >= QUANTIDADE_ATAQUES_PARA_ESPECIAL_BOSS2){
                    proximo_projetil_especial = 1;
                    contador_ataques_especiais_boss2 = 0;
                }else{
                    proximo_projetil_especial = 0;
                }

                avisando_ataque_inimigo = 1;

                if (proximo_projetil_especial) {
                    if (ataque_de_rajada) {
                        boss2_special.intervalo = 0.025f; // especial acelerado na rajada surpresa
                    }
                    else {
                        boss2_special.intervalo = 0.08f; // velocidade normal do especial
                    }

                    TrocarAnimacaoBoss2(2);
                }
                else {
                    TrocarAnimacaoBoss2(1);
                }

                if (ataque_de_rajada){
                    tempo_aviso_ataque_inimigo = INTERVALO_RAJADA_BOSS;
                }
                else if (proximo_projetil_especial){
                    tempo_aviso_ataque_inimigo = 0.75f;
                }
                else{
                    tempo_aviso_ataque_inimigo = 0.50f;
                }

            }

            //PROJÉTIL SURGE!

            if (avisando_ataque_inimigo){
                tempo_aviso_ataque_inimigo -= GetFrameTime();

                if (tempo_aviso_ataque_inimigo <= 0){
                    avisando_ataque_inimigo = 0;

                    Projetil_Inimigo novo_projetil;

                    novo_projetil.ativo = 1;
                    novo_projetil.rebatido = 0;
                    novo_projetil.especial = proximo_projetil_especial;
                    novo_projetil.velocidade = velocidade_atual_projetil;

                    novo_projetil.corpo = (Rectangle){
                        inimigo.posicao.x,
                        GetRandomValue(ALTURA_PROJETIL_MIN, ALTURA_PROJETIL_MAX),
                        LARGURA_PROJETIL,
                        ALTURA_PROJETIL
                    };

                    head = AdicionarProjetil(head, novo_projetil);

                    TocarSomHand();

                    if (novo_projetil.especial) {
                        ReiniciarAnimacao(&projetil_marionette_especial);
                    }
                    else {
                        ReiniciarAnimacao(&projetil_marionette);
                    }
                }
            }

            //Se o projétil está ativo, ele anda para a esquerda (eixo x negativo)

            Projetil_Inimigo *p2_mov = PegarProjetilAtual(head);

            if (p2_mov != NULL){

                if (p2_mov->rebatido){
                    p2_mov->corpo.x += velocidade_atual_projetil;
                }else{
                    p2_mov->corpo.x -= velocidade_atual_projetil;
                }

                if (!p2_mov->rebatido && p2_mov->corpo.x < -50){
                    LimparProjeteis(&head);

                    if (tiros_rajada_restantes > 0){
                        cooldown_projetil = INTERVALO_RAJADA_BOSS;
                    }else{
                        cooldown_projetil = tempo_cooldown_atual_projetil;
                    }
                }
                else if (p2_mov->rebatido && p2_mov->corpo.x > LARGURA_TELA + 50){
                    LimparProjeteis(&head);

                    if (tiros_rajada_restantes > 0){
                        cooldown_projetil = INTERVALO_RAJADA_BOSS;
                    }else{
                        cooldown_projetil = tempo_cooldown_atual_projetil;
                    }
                }
            }

            //Tempo invencível após receber o dano

            if (tempo_sem_receber_dano > 0){
                tempo_sem_receber_dano -= GetFrameTime(); //GetFrameTime é da Raylib, e representa o tempo entre um frame e outro, nesse caso é para marcar o tempo de receber dano apenas quando enconsta

            }

           
            //Lógica da colisão do projétil com o parry, testa se o jogador aparou

            Projetil_Inimigo *p2_parry = PegarProjetilAtual(head);

            if (p2_parry != NULL && aparando && !p2_parry->rebatido && CheckCollisionCircleRec(jogador.posicao, AREA_PARRY, p2_parry->corpo)){
                p2_parry->rebatido = 1;
                TocarSomParry();

                if (p2_parry->especial) {
                    ReiniciarAnimacao(&projetil_marionette_especial);
                }
                else {
                    ReiniciarAnimacao(&projetil_marionette);
                }

                tempo_texto_parry = 0.4f;
                aparando = 0;
            }

            Rectangle corpo_inimigo_rebatido = {
                inimigo.posicao.x,
                inimigo.posicao.y,
                80,
                100
            };

            

            Projetil_Inimigo *p2_colisao = PegarProjetilAtual(head);

            //Quando o projétil rebatido acerta o boss
            if (p2_colisao != NULL && p2_colisao->rebatido && CheckCollisionRecs(p2_colisao->corpo, corpo_inimigo_rebatido)){
                inimigo.vida -= ObterDanoParry();
                TocarSomHitRose();
                AumentarComboParry();
                tempo_recebendo_dano_inimigo = 0.15f;
                tempo_texto_dano = 0.3f;
                TrocarAnimacaoBoss2(3);

                LimparProjeteis(&head);

                if (tiros_rajada_restantes > 0){
                    cooldown_projetil = INTERVALO_RAJADA_BOSS;
                }else{
                    cooldown_projetil = tempo_cooldown_atual_projetil;
                }
            }

            //Testa se o jogador tomou dano ou Quando a Rose tomar dano
            else if (p2_colisao != NULL && !p2_colisao->rebatido && CheckCollisionCircleRec(jogador.posicao, 20, p2_colisao->corpo) && tempo_sem_receber_dano <=0){
                
                TocarSomHitBoss();

                if (p2_colisao->especial){
                    jogador.vida -= 5;
                    tempo_incapacitado_jogador = TEMPO_INCAPACITADO_JOGADOR;
                }else{
                    jogador.vida -= 10;
                }
                
                ReduzirComboParryAoTomarDano();

                tempo_sem_receber_dano = 1.0f;
                tempo_recebendo_dano_jogador = 0.2f;

                LimparProjeteis(&head);

                if (tiros_rajada_restantes > 0){
                    cooldown_projetil = INTERVALO_RAJADA_BOSS;
                }else{
                    cooldown_projetil = tempo_cooldown_atual_projetil;
                }
            }

            //Inimigo perdeu ou Jogador perdeu

            if (jogador.vida <= 0){
                jogador.vida = 0;
                atacando = 0;
                aparando = 0;
                LimparProjeteis(&head);
                TrocarAnimacaoRose(4);
            }

            else if (inimigo.vida <= 0){
                inimigo.vida = 0;

                atacando = 0;
                aparando = 0;
                LimparProjeteis(&head);
                avisando_ataque_inimigo = 0;

                TrocarAnimacaoBoss2(4);
            }
            
            if (animacao_atual_rose == 1) {
                AtualizarAnimacao(&rose_walk);
            }
            else if (animacao_atual_rose == 2) {
                AtualizarAnimacao(&rose_jump);
            }
            else if (animacao_atual_rose == 3) {
                AtualizarAnimacao(&rose_parry);
            }
            else if (animacao_atual_rose == 4) {
                AtualizarAnimacao(&rose_dying);
            }
            else if (animacao_atual_rose == 5) {
                AtualizarAnimacao(&rose_attack);
            }
            else {
                AtualizarAnimacao(&rose_idle);
            }

            if (animacao_atual_boss2 == 1) {
                AtualizarAnimacao(&boss2_attack);

                if (boss2_attack.terminou) {
                    animacao_atual_boss2 = 0;
                }
            }
            else if (animacao_atual_boss2 == 2) {
                AtualizarAnimacao(&boss2_special);

                if (boss2_special.terminou) {
                    animacao_atual_boss2 = 0;
                }
            }
            else if (animacao_atual_boss2 == 3) {
                AtualizarAnimacao(&boss2_hurt);

                if (boss2_hurt.terminou) {
                    animacao_atual_boss2 = 0;
                }
            }
            else if (animacao_atual_boss2 == 4) {
                AtualizarAnimacao(&boss2_dying);
            }
            else {
                AtualizarAnimacao(&boss2_idle);
            }


            break;

            //Fim da batalha

        }

        case tela_GameOver:
            if (IsKeyPressed(KEY_ENTER)){
                InitGame();
                TrocarMusica(1);
                ModoDoJogo = tela_menu;
            }

            break;

        case tela_animacaoFinal:
            
            if (IsKeyPressed(KEY_ENTER)){
                IndiceCenaFinal++;

                if (IndiceCenaFinal >= TOTAL_CENAS_FINAIS){
                    InitGame();
                    TrocarMusica(1);
                    IndiceCenaFinal = 0; //Vai começar no indice zero
                    ModoDoJogo = tela_menu;

                }

            }

            break;
         
        case tela_transicao_boss:

            if (IsKeyPressed(KEY_ENTER)){
                if (IndiceCenaTransicaoBoss < TOTAL_CENAS_TRANSICAO_BOSS - 1) {
                    IniciarFadeIntro(4);
                }
                else {
                    IniciarFadeIntro(5);
                }
            }

            break;
        
        case tela_vitoria:

            if (IsKeyPressed(KEY_ENTER)){
                IniciarFadeIntro(6);
            }

            break;

            }

}
void DesenharLuzMorteRose(Vector2 centro, float raio) {
    Color escuro = Fade(BLACK, 0.96f);
    Color borda = Fade(BLACK, 0.55f);

    float suavidade = 55.0f;
    float raio_interno = raio - suavidade;

    if (raio_interno < 0) {
        raio_interno = 0;
    }

    float passo = 3.0f;

    for (float y = 0; y < ALTURA_TELA; y += passo) {
        float centro_y_linha = y + passo / 2.0f;
        float dy = centro_y_linha - centro.y;

        if (dy < -raio || dy > raio) {
            DrawRectangleRec(
                (Rectangle){0, y, LARGURA_TELA, passo},
                escuro
            );
        }
        else {
            float dx_externo = sqrtf(raio * raio - dy * dy);

            float esquerda_externa = centro.x - dx_externo;
            float direita_externa = centro.x + dx_externo;

            if (esquerda_externa > 0) {
                DrawRectangleRec(
                    (Rectangle){0, y, esquerda_externa, passo},
                    escuro
                );
            }

            if (direita_externa < LARGURA_TELA) {
                DrawRectangleRec(
                    (Rectangle){direita_externa, y, LARGURA_TELA - direita_externa, passo},
                    escuro
                );
            }

            if (raio_interno > 0 && dy >= -raio_interno && dy <= raio_interno) {
                float dx_interno = sqrtf(raio_interno * raio_interno - dy * dy);

                float esquerda_interna = centro.x - dx_interno;
                float direita_interna = centro.x + dx_interno;

                if (esquerda_interna > esquerda_externa) {
                    DrawRectangleRec(
                        (Rectangle){esquerda_externa, y, esquerda_interna - esquerda_externa, passo},
                        borda
                    );
                }

                if (direita_externa > direita_interna) {
                    DrawRectangleRec(
                        (Rectangle){direita_interna, y, direita_externa - direita_interna, passo},
                        borda
                    );
                }
            }
        }
    }
}
void DesenharJogo(){

    switch (ModoDoJogo){
        case tela_menu:
            DrawTexturePro(
                menu,
                (Rectangle){0, 0, menu.width, menu.height},
                (Rectangle){0, 0, LARGURA_TELA, ALTURA_TELA},
                (Vector2){0, 0},
                0,
                WHITE
            );

            break;

        case tela_animacaoIntro:
            DrawTexturePro(
                intro[indice_tela_intro],
                (Rectangle){
                    0,
                    0,
                    intro[indice_tela_intro].width,
                    intro[indice_tela_intro].height
                },
                (Rectangle){0, 0, LARGURA_TELA, ALTURA_TELA},
                (Vector2){0, 0},
                0,
                WHITE
            );

            break;

        case tela_batalha_boss1:{
            DrawTexturePro(
                background,
                (Rectangle){0, 0, background.width, background.height},
                (Rectangle){0, 0, LARGURA_TELA, ALTURA_TELA + 55},
                (Vector2){0, 0},
                0,
                WHITE
            );
            DrawText("LUTE!", 350,50,30,RED);

            Color corRose = WHITE;

            if (tempo_recebendo_dano_jogador > 0){
                corRose = RED;
            }

            animacao *animacaoRose = &rose_idle;

            if (animacao_atual_rose == 1) {
                animacaoRose = &rose_walk;
            }
            else if (animacao_atual_rose == 2) {
                animacaoRose = &rose_jump;
            }
            else if (animacao_atual_rose == 3) {
                animacaoRose = &rose_parry;
            }
            else if (animacao_atual_rose == 4) {
                animacaoRose = &rose_dying;
            }
            else if (animacao_atual_rose == 5) {
                animacaoRose = &rose_attack;
            }

            Texture2D frameRose = animacaoRose->frames[animacaoRose->frame_atual];

            Rectangle origemRose = {
                frameRose.width,
                0,
                -frameRose.width,
                frameRose.height
            };

            Rectangle destinoRose;

            if (animacao_atual_rose == 1) { // walk
                destinoRose = (Rectangle){
                    jogador.posicao.x - 55,
                    jogador.posicao.y - 105,
                    110,
                    130
                };
            }
            else if (animacao_atual_rose == 2) { // jump
                destinoRose = (Rectangle){
                    jogador.posicao.x - 105,
                    jogador.posicao.y - 160,
                    215,
                    245
                };
            }
            else if (animacao_atual_rose == 3) { // parry
                destinoRose = (Rectangle){
                    jogador.posicao.x - 55,
                    jogador.posicao.y - 105,
                    110,
                    130
                };
            }
            else if (animacao_atual_rose == 5) { // attack
                destinoRose = (Rectangle){
                    jogador.posicao.x - 105,
                    jogador.posicao.y - 160,
                    215,
                    245
                };
            }
            else if (animacao_atual_rose == 4) { // dying
                destinoRose = (Rectangle){
                    jogador.posicao.x - 110,
                    jogador.posicao.y - 155,
                    220,
                    250
                };
            }
            else { // idle
                destinoRose = (Rectangle){
                    jogador.posicao.x - 105,
                    jogador.posicao.y - 160,
                    215,
                    245
                };
            }

            DrawTexturePro(
                frameRose,
                origemRose,
                destinoRose,
                (Vector2){0, 0},
                0,
                corRose
            );

            if (atacando){
                DrawRectangleLinesEx(ataque_jogador,2,PINK);
            }

            animacao *animacaoBoss = &boss1_idle;

            if (animacao_atual_boss1 == 1) {
                animacaoBoss = &boss1_attack;
            }
            else if (animacao_atual_boss1 == 2) {
                animacaoBoss = &boss1_hurt;
            }

            Texture2D frameBoss = animacaoBoss->frames[animacaoBoss->frame_atual];

            Rectangle origemBoss = {
                0,
                0,
                frameBoss.width,
                frameBoss.height
            };

            Rectangle destinoBoss = {
                inimigo.posicao.x - 75,
                inimigo.posicao.y - 135,
                240,
                280
            };

            DrawTexturePro(
                frameBoss,
                origemBoss,
                destinoBoss,
                (Vector2){0, 0},
                0,
                WHITE
            );

            if (tempo_texto_dano > 0){
                DrawText("ATACOU!", inimigo.posicao.x + 10, inimigo.posicao.y - 30, 20, YELLOW);
            }

            if (avisando_ataque_inimigo){
                DrawText(
                    "!",
                    destinoBoss.x + 20,
                    destinoBoss.y + 25,
                    80,
                    YELLOW
                );
            }

            if (aparando){
                DrawCircleLines(jogador.posicao.x, jogador.posicao.y, AREA_PARRY, SKYBLUE);
                DrawCircleLines(jogador.posicao.x, jogador.posicao.y, AREA_PARRY -5, BLUE);
            }

            if (tempo_texto_parry > 0){
                DrawText("APAROU!", jogador.posicao.x - 35, jogador.posicao.y - 70, 20, SKYBLUE);
            }

            Projetil_Inimigo *p_desenho = PegarProjetilAtual(head);

            if (p_desenho != NULL) {
                Texture2D frameProjetil;

                if (p_desenho->rebatido) {
                    frameProjetil = projetil_palhaco_parry.frames[projetil_palhaco_parry.frame_atual];
                }
                else {
                    frameProjetil = projetil_palhaco.frames[projetil_palhaco.frame_atual];
                }

                Rectangle destinoProjetil = {
                    p_desenho->corpo.x - 8,
                    p_desenho->corpo.y - 5,
                    p_desenho->corpo.width * 2.2f,
                    p_desenho->corpo.height * 2.2f
                };

                DrawTexturePro(
                    frameProjetil,
                    (Rectangle){0, 0, frameProjetil.width, frameProjetil.height},
                    destinoProjetil,
                    (Vector2){0, 0},
                    0,
                    WHITE
                );
            }

            DrawText(TextFormat("ROSE HP: %d", jogador.vida), 50, 100, 20, WHITE);

            DrawRectangle(50, 130, 300, 14, BLACK);
            DrawRectangle(51, 131, 298, 12, DARKGRAY);

            float larguraVidaJogador = ((float)jogador.vida / VIDA_MAX_JOGADOR) * 298;

            if (larguraVidaJogador < 0){
                larguraVidaJogador = 0;
            }

            DrawRectangle(51, 131, (int)larguraVidaJogador, 12, GREEN);
            DrawRectangleLines(50, 130, 300, 14, WHITE);

            // Textos de estado do boss
            if (inimigo.vida <= VIDA_MAX_INIMIGO / 2){
                DrawText("FASE 2",650,50,20,RED);
            }

            if (tiros_rajada_restantes > 0){
                DrawText("ENFURECIDO!", 640,80,20,YELLOW);
            }

            // Barra de vida do Pagliacci
            int xBarraBoss = 120;
            int yBarraBoss = ALTURA_TELA - 55;
            int larguraBarraBoss = 560;
            int alturaBarraBoss = 14;

            DrawText("PAGLIACCI", xBarraBoss, yBarraBoss - 24, 18, WHITE);

            DrawRectangle(xBarraBoss, yBarraBoss, larguraBarraBoss, alturaBarraBoss, BLACK);
            DrawRectangle(xBarraBoss + 1, yBarraBoss + 1, larguraBarraBoss - 2, alturaBarraBoss - 2, DARKGRAY);

            float larguraVidaBoss = ((float)inimigo.vida / VIDA_MAX_INIMIGO) * (larguraBarraBoss - 2);

            if (larguraVidaBoss < 0){
                larguraVidaBoss = 0;
            }

            DrawRectangle(
                xBarraBoss + 1,
                yBarraBoss + 1,
                (int)larguraVidaBoss,
                alturaBarraBoss - 2,
                RED
            );

            DrawRectangleLines(xBarraBoss, yBarraBoss, larguraBarraBoss, alturaBarraBoss, WHITE);

            DrawText("Pressione A para atacar", 50,200,20,WHITE);
            
            if (efeito_morte_rose) {
                Vector2 centro_luz_rose = {
                jogador.posicao.x + 10,
                jogador.posicao.y - 25
            };

            DesenharLuzMorteRose(centro_luz_rose, raio_luz_morte_rose);
            }

            break;

        }
        
        case tela_intro_boss2:{

            DrawTexturePro(
                background,
                (Rectangle){0, 0, background.width, background.height},
                (Rectangle){0, 0, LARGURA_TELA, ALTURA_TELA + 55},
                (Vector2){0, 0},
                0,
                WHITE
            );

            Texture2D frameRose = rose_idle.frames[rose_idle.frame_atual];

            Rectangle origemRose = {
                frameRose.width,
                0,
                -frameRose.width,
                frameRose.height
            };

            Rectangle destinoRose = {
                jogador.posicao.x - 105,
                jogador.posicao.y - 160,
                215,
                245
            };

            DrawTexturePro(
                frameRose,
                origemRose,
                destinoRose,
                (Vector2){0, 0},
                0,
                WHITE
            );

            Texture2D frameBoss2 = boss2_intro.frames[boss2_intro.frame_atual];

            Rectangle origemBoss2 = {
                0,
                0,
                frameBoss2.width,
                frameBoss2.height
            };

            Rectangle destinoBoss2 = {
                inimigo.posicao.x - 215,
                inimigo.posicao.y - 220,
                420,
                500
            };

            DrawTexturePro(
                frameBoss2,
                origemBoss2,
                destinoBoss2,
                (Vector2){0, 0},
                0,
                WHITE
            );

            DrawText("A MARIONETE", 255, 70, 40, RED);

            if (tempo_intro_boss2 > 1.2f) {
                DrawText("Prepare-se...", 315, 120, 20, WHITE);
            }

            DrawRectangle(
                0,
                0,
                LARGURA_TELA,
                ALTURA_TELA,
                Fade(BLACK, opacidade_fade_intro_boss2)
            );

            break;
        }

        case tela_batalha_boss2:{

            DrawTexturePro(
                background,
                (Rectangle){0, 0, background.width, background.height},
                (Rectangle){0, 0, LARGURA_TELA, ALTURA_TELA + 55},
                (Vector2){0, 0},
                0,
                WHITE
            );
            
            DrawText("LUTE!", 350,50,30,RED);

            Color corRose = WHITE;

            if (tempo_recebendo_dano_jogador > 0){
                corRose = RED;
            }

            animacao *animacaoRose = &rose_idle;

            if (animacao_atual_rose == 1) {
                animacaoRose = &rose_walk;
            }
            else if (animacao_atual_rose == 2) {
                animacaoRose = &rose_jump;
            }
            else if (animacao_atual_rose == 3) {
                animacaoRose = &rose_parry;
            }
            else if (animacao_atual_rose == 4) {
                animacaoRose = &rose_dying;
            }
            else if (animacao_atual_rose == 5) {
                animacaoRose = &rose_attack;
            }

            Texture2D frameRose = animacaoRose->frames[animacaoRose->frame_atual];

            Rectangle origemRose = {
                frameRose.width,
                0,
                -frameRose.width,
                frameRose.height
            };

            Rectangle destinoRose;

            if (animacao_atual_rose == 1) { // walk
                destinoRose = (Rectangle){
                    jogador.posicao.x - 55,
                    jogador.posicao.y - 105,
                    110,
                    130
                };
            }
            else if (animacao_atual_rose == 2) { // jump
                destinoRose = (Rectangle){
                    jogador.posicao.x - 105,
                    jogador.posicao.y - 160,
                    215,
                    245
                };
            }
            else if (animacao_atual_rose == 3) { // parry
                destinoRose = (Rectangle){
                    jogador.posicao.x - 55,
                    jogador.posicao.y - 105,
                    110,
                    130
                };
            }
            else if (animacao_atual_rose == 5) { // attack
                destinoRose = (Rectangle){
                    jogador.posicao.x - 105,
                    jogador.posicao.y - 160,
                    215,
                    245
                };
            }
            else if (animacao_atual_rose == 4) { // dying
                destinoRose = (Rectangle){
                    jogador.posicao.x - 110,
                    jogador.posicao.y - 155,
                    220,
                    250
                };
            }
            else { // idle
                destinoRose = (Rectangle){
                    jogador.posicao.x - 105,
                    jogador.posicao.y - 160,
                    215,
                    245
                };
            }

            DrawTexturePro(
                frameRose,
                origemRose,
                destinoRose,
                (Vector2){0, 0},
                0,
                corRose
            );

            if (atacando){
                DrawRectangleLinesEx(ataque_jogador,2,PINK);
            }

            animacao *animacaoBoss2 = &boss2_idle;

            if (animacao_atual_boss2 == 1) {
                animacaoBoss2 = &boss2_attack;
            }
            else if (animacao_atual_boss2 == 2) {
                animacaoBoss2 = &boss2_special;
            }
            else if (animacao_atual_boss2 == 3) {
                animacaoBoss2 = &boss2_hurt;
            }
            else if (animacao_atual_boss2 == 4) {
                animacaoBoss2 = &boss2_dying;
            }

            Texture2D frameBoss2 = animacaoBoss2->frames[animacaoBoss2->frame_atual];

            Rectangle origemBoss2 = {
                0,
                0,
                frameBoss2.width,
                frameBoss2.height
            };

            Rectangle destinoBoss2 = {
                inimigo.posicao.x - 215,
                inimigo.posicao.y - 220,
                420,
                500
            };

            DrawTexturePro(
                frameBoss2,
                origemBoss2,
                destinoBoss2,
                (Vector2){0, 0},
                0,
                WHITE
            );

            if (tempo_texto_dano > 0){
                DrawText("ATACOU!", inimigo.posicao.x + 10, inimigo.posicao.y - 30, 20, YELLOW);
            }

            if (avisando_ataque_inimigo){
                DrawText(
                    "!",
                    destinoBoss2.x + 95,
                    destinoBoss2.y + 75,
                    80,
                    YELLOW
                );
            }

            if (aparando){
                DrawCircleLines(jogador.posicao.x, jogador.posicao.y, AREA_PARRY, SKYBLUE);
                DrawCircleLines(jogador.posicao.x, jogador.posicao.y, AREA_PARRY -5, BLUE);
            }

            if (tempo_texto_parry > 0){
                DrawText("APAROU!", jogador.posicao.x - 35, jogador.posicao.y - 70, 20, SKYBLUE);
            }

            Projetil_Inimigo *p2_desenho = PegarProjetilAtual(head);

            if (p2_desenho != NULL) {
                animacao *animacaoProjetil;

                if (p2_desenho->especial) {
                    animacaoProjetil = &projetil_marionette_especial;
                }
                else {
                    animacaoProjetil = &projetil_marionette;
                }

                Texture2D frameProjetil = animacaoProjetil->frames[animacaoProjetil->frame_atual];

                Rectangle origemProjetil;

                if (!p2_desenho->rebatido) {
                    origemProjetil = (Rectangle){
                        frameProjetil.width,
                        0,
                        -frameProjetil.width,
                        frameProjetil.height
                    };
                }
                else {
                    origemProjetil = (Rectangle){
                        0,
                        0,
                        frameProjetil.width,
                        frameProjetil.height
                    };
                }

                Rectangle destinoProjetil = {
                    p2_desenho->corpo.x - 12,
                    p2_desenho->corpo.y - 8,
                    p2_desenho->corpo.width * 2.6f,
                    p2_desenho->corpo.height * 2.6f
                };

                DrawTexturePro(
                    frameProjetil,
                    origemProjetil,
                    destinoProjetil,
                    (Vector2){0, 0},
                    0,
                    WHITE
                );
            }

            if (tempo_incapacitado_jogador > 0){
                DrawText("SILENCIADO!", jogador.posicao.x - 35, jogador.posicao.y - 70,20,PURPLE); 
            }

            // Vida da Rose
            DrawText(TextFormat("ROSE HP: %d", jogador.vida), 50, 100, 20, WHITE);

            DrawRectangle(50, 130, 300, 14, BLACK);
            DrawRectangle(51, 131, 298, 12, DARKGRAY);

            float larguraVidaJogador = ((float)jogador.vida / VIDA_MAX_JOGADOR) * 298;

            if (larguraVidaJogador < 0){
                larguraVidaJogador = 0;
            }

            DrawRectangle(51, 131, (int)larguraVidaJogador, 12, GREEN);
            DrawRectangleLines(50, 130, 300, 14, WHITE);

            // Textos de estado do boss
            if (inimigo.vida <= VIDA_MAX_INIMIGO_BOSS2 / 2){
                DrawText("FASE 2",650,50,20,RED);
            }

            if (tiros_rajada_restantes > 0){
                DrawText("ENFURECIDO!", 640,80,20,YELLOW);
            }

            // Barra de vida da Marionette
            int xBarraBoss = 120;
            int yBarraBoss = ALTURA_TELA - 55;
            int larguraBarraBoss = 560;
            int alturaBarraBoss = 14;

            DrawText("MARIONETTE", xBarraBoss, yBarraBoss - 24, 18, WHITE);

            DrawRectangle(xBarraBoss, yBarraBoss, larguraBarraBoss, alturaBarraBoss, BLACK);
            DrawRectangle(xBarraBoss + 1, yBarraBoss + 1, larguraBarraBoss - 2, alturaBarraBoss - 2, DARKGRAY);

            float larguraVidaBoss = ((float)inimigo.vida / VIDA_MAX_INIMIGO_BOSS2) * (larguraBarraBoss - 2);

            if (larguraVidaBoss < 0){
                larguraVidaBoss = 0;
            }

            DrawRectangle(
                xBarraBoss + 1,
                yBarraBoss + 1,
                (int)larguraVidaBoss,
                alturaBarraBoss - 2,
                RED
            );

            DrawRectangleLines(xBarraBoss, yBarraBoss, larguraBarraBoss, alturaBarraBoss, WHITE);

            DrawText("Pressione A para atacar", 50,200,20,WHITE);

            if (efeito_morte_rose) {
                Vector2 centro_luz_rose = {
                    jogador.posicao.x + 10,
                    jogador.posicao.y - 25
                };

                DesenharLuzMorteRose(centro_luz_rose, raio_luz_morte_rose);
            }

            if (fazendo_fade_final) {
                DrawRectangle(
                    0,
                    0,
                    LARGURA_TELA,
                    ALTURA_TELA,
                    Fade(BLACK, opacidade_fade_final)
                );
            }

            break;
        }


        case tela_transicao_boss:
            DrawTexturePro(
                transicao_boss[IndiceCenaTransicaoBoss],
                (Rectangle){
                    0,
                    0,
                    transicao_boss[IndiceCenaTransicaoBoss].width,
                    transicao_boss[IndiceCenaTransicaoBoss].height
                },
                (Rectangle){0, 0, LARGURA_TELA, ALTURA_TELA},
                (Vector2){0, 0},
                0,
                WHITE
            );

    break;

        case tela_vitoria:
            DrawTexturePro(
                victory[indice_tela_vitoria],
                (Rectangle){0, 0, victory[indice_tela_vitoria].width, victory[indice_tela_vitoria].height},
                (Rectangle){0, 0, LARGURA_TELA, ALTURA_TELA},
                (Vector2){0, 0},
                0,
                WHITE
            );

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
               

        case tela_GameOver:
            DrawTexturePro(
                gameover,
                (Rectangle){0, 0, gameover.width, gameover.height},
                (Rectangle){0, 0, LARGURA_TELA, ALTURA_TELA},
                (Vector2){0, 0},
                0,
                WHITE
            );

            break;
    }

    if (fase_fade_intro != 0) {
        DrawRectangle(
            0,
            0,
            LARGURA_TELA,
            ALTURA_TELA,
            Fade(BLACK, opacidade_fade_intro)
        );
    }

}
// fim do código