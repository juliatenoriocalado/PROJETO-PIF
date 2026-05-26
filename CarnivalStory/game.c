//Controle dos estados do jogo

#include "game.h"
#include <stdio.h>

//==============================================
//             Variáveis globais
//==============================================

Telas ModoDoJogo = tela_menu;
//Aqui eu estou criando uma variável do tipo enum "Telas" para marcar o início ao menu do jogo

int IndiceCenaFinal = 0; //Contador para marcar e contar as cenas que estão sendo passadas ou exibidas no momento
//Essas cenas do final são as que vão ser exibidas após o jogador vencer o jogo

int IndiceCenaTransicaoBoss = 0;

float opacidade_fade_final = 0.0f;

int fazendo_fade_final = 0;

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

// ANIMAÇÕES //
animacao boss1_idle;
animacao boss1_attack;
animacao boss1_hurt;

int animacao_atual_boss1 = 0;

animacao boss2_idle;
animacao boss2_attack;
animacao boss2_special;
animacao boss2_hurt;
animacao boss2_dying;

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
Projetil_Inimigo projetil_inimigo;
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

//Aqui estamos criando as variáveis tanto para personagem quanto para os nossos inimigos (cada um com seus respectivos structs, os de letra maiúscula, para marcar do formato para o objeto)

//==============================================================================================================================================================================================
//                                                                            Inicalização do jogo! 
//==============================================================================================================================================================================================
void ReiniciarAnimacao(animacao *animacao);
void Boss2(){

    //Aqui a gente tá reiniciando tudo tudo tudo, porque se não fizermos isso o jogador vai chegar com a vida que sobrou do Boss 1, por exemplo, e outras configurações já ativadas que podem prejudicar o controle do jogador

    jogador.vida = VIDA_MAX_JOGADOR;
    jogador.posicao = (Vector2){200, COORDENADA_CHAO};
    inimigo.vida = VIDA_MAX_INIMIGO_BOSS2;
    inimigo.posicao = (Vector2){640, COORDENADA_CHAO - 120};

    opacidade_fade_final = 0.0f;
    fazendo_fade_final = 0;

    animacao_atual_rose = 0;
    ReiniciarAnimacao(&rose_idle);
    ReiniciarAnimacao(&rose_walk);
    ReiniciarAnimacao(&rose_jump);
    ReiniciarAnimacao(&rose_parry);
    ReiniciarAnimacao(&rose_dying);
    ReiniciarAnimacao(&rose_attack);

    projetil_inimigo.ativo = 0;
    projetil_inimigo.rebatido = 0;
    projetil_inimigo.especial = 0;

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

    LoadAnimacao(&boss1_idle, "assets/boss1/idle", "idle", FRAMES_ANIMACAO, 0.12f, 1);
    LoadAnimacao(&boss1_attack, "assets/boss1/attack", "attack", FRAMES_ANIMACAO, 0.08f, 0);
    LoadAnimacao(&boss1_hurt, "assets/boss1/hurt", "hurt", FRAMES_ANIMACAO, 0.08f, 0);

    LoadAnimacao(&boss2_idle, "assets/boss2/idle", "idle", 9, 0.12f, 1);
    LoadAnimacao(&boss2_attack, "assets/boss2/attack", "attack", 9, 0.08f, 0);
    LoadAnimacao(&boss2_special, "assets/boss2/special", "special", 9, 0.08f, 0);
    LoadAnimacao(&boss2_hurt, "assets/boss2/hurt", "hurt", 9, 0.08f, 0);
    LoadAnimacao(&boss2_dying, "assets/boss2/dying", "death", 9, 0.10f, 0);

    LoadAnimacao(&rose_idle, "assets/rose/idle", "idle", 3, 0.18f, 1);
    LoadAnimacao(&rose_walk, "assets/rose/walk", "walk", 3, 0.12f, 1);
    LoadAnimacao(&rose_jump, "assets/rose/jump", "jump", 5, 0.10f, 0);
    LoadAnimacao(&rose_parry, "assets/rose/parry", "parry", 3, 0.11f, 0);
    LoadAnimacao(&rose_dying, "assets/rose/dying", "dying", 9, 0.10f, 0);
    LoadAnimacao(&rose_attack, "assets/rose/attack", "attack", 9, 0.06f, 0);
    
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

    UnloadAnimacao(&boss2_idle);
    UnloadAnimacao(&boss2_attack);
    UnloadAnimacao(&boss2_special);
    UnloadAnimacao(&boss2_hurt);
    UnloadAnimacao(&boss2_dying);

    UnloadAnimacao(&rose_idle);
    UnloadAnimacao(&rose_walk);
    UnloadAnimacao(&rose_jump);
    UnloadAnimacao(&rose_parry);
    UnloadAnimacao(&rose_dying);
    UnloadAnimacao(&rose_attack);

    UnloadTexture(background);
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

void InitGame(){ //Aqui é onde inicializamos todas as variáveis globais, por exemplo, para o jogo poder começar de fato

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

    projetil_inimigo.corpo = (Rectangle){
        inimigo.posicao.x,
        ALTURA_PROJETIL_RASTEIRO,
        LARGURA_PROJETIL,
        ALTURA_PROJETIL
    };
    
    //A ideia é que o Boss não comece atirando, ele espera um tempo e começa a atirar
    
    projetil_inimigo.velocidade = VELOCIDADE_PROJETIL_FASE1;
    projetil_inimigo.ativo = 0;
    projetil_inimigo.rebatido = 0;
    projetil_inimigo.especial = 0;
    cooldown_projetil = TEMPO_COOLDOWN_PROJETIL_FASE1;

    tempo_sem_receber_dano = 0;
    
    animacao_atual_boss1 = 0;
    ReiniciarAnimacao(&boss1_idle);
    ReiniciarAnimacao(&boss1_attack);
    ReiniciarAnimacao(&boss1_hurt); //aqui reiniciamos a animação para idle para n ter conflito ao iniciar de novo

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
                ModoDoJogo = tela_batalha_boss1;
            }

            break;

         case tela_batalha_boss1:{

            if (jogador.vida <= 0) {
                jogador.vida = 0;

                TrocarAnimacaoRose(4);
                AtualizarAnimacao(&rose_dying);

                if (rose_dying.terminou) {
                    ModoDoJogo = tela_GameOver;
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

            if (!projetil_inimigo.ativo && !avisando_ataque_inimigo && cooldown_projetil <= 0){ //Se acabou o cooldown e o projétil não está ativo, então ele ativa e o Boss dispara

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
                    projetil_inimigo.ativo = 1;
                    projetil_inimigo.rebatido = 0;

                    projetil_inimigo.corpo.x = inimigo.posicao.x;
                    projetil_inimigo.corpo.y = GetRandomValue(ALTURA_PROJETIL_MIN, ALTURA_PROJETIL_MAX);
                }
            }

            //Se o projétil está ativo, ele anda para a esquerda (eixo x negativo)

            if (projetil_inimigo.ativo){

                if (projetil_inimigo.rebatido){
                    projetil_inimigo.corpo.x += velocidade_atual_projetil; //Vai da esquerda para a direita
                }else{
                    projetil_inimigo.corpo.x -= velocidade_atual_projetil; //Vai da direita para a esquerda
                }

                if (!projetil_inimigo.rebatido && projetil_inimigo.corpo.x < -50){
                    projetil_inimigo.ativo = 0;
                    projetil_inimigo.rebatido = 0;
                
                    if (tiros_rajada_restantes > 0){
                        cooldown_projetil = INTERVALO_RAJADA_BOSS;
                    }else{
                        cooldown_projetil = tempo_cooldown_atual_projetil;
                    }

                    projetil_inimigo.corpo.x = inimigo.posicao.x;
                    projetil_inimigo.corpo.y = GetRandomValue(ALTURA_PROJETIL_MIN, ALTURA_PROJETIL_MAX);
                }

                if (projetil_inimigo.rebatido && projetil_inimigo.corpo.x > LARGURA_TELA + 50){
                    projetil_inimigo.ativo = 0;
                    projetil_inimigo.rebatido = 0;

                    if (tiros_rajada_restantes > 0){
                        cooldown_projetil = INTERVALO_RAJADA_BOSS;
                    }else{
                        cooldown_projetil = tempo_cooldown_atual_projetil;
                    }

                    projetil_inimigo.corpo.x = inimigo.posicao.x;
                    projetil_inimigo.corpo.y = GetRandomValue(ALTURA_PROJETIL_MIN, ALTURA_PROJETIL_MAX);

                }
            }

            //Tempo invencível após receber o dano

            if (tempo_sem_receber_dano > 0){
                tempo_sem_receber_dano -= GetFrameTime(); //GetFrameTime é da Raylib, e representa o tempo entre um frame e outro, nesse caso é para marcar o tempo de receber dano apenas quando enconsta

            }

           
            //Lógica da colisão do projétil com o parry, testa se o jogador aparou

            if (aparando && projetil_inimigo.ativo && !projetil_inimigo.rebatido && CheckCollisionCircleRec(jogador.posicao, AREA_PARRY, projetil_inimigo.corpo)){
                projetil_inimigo.rebatido = 1;
                tempo_texto_parry = 0.4f;
                aparando = 0;
            }

            Rectangle corpo_inimigo_rebatido = {
                inimigo.posicao.x,
                inimigo.posicao.y,
                80,
                100
            };

            //Quando o projétil rebatido acerta o boss

            if (projetil_inimigo.ativo && projetil_inimigo.rebatido && CheckCollisionRecs(projetil_inimigo.corpo, corpo_inimigo_rebatido)){
                inimigo.vida -= 20;
                tempo_recebendo_dano_inimigo = 0.15f;
                tempo_texto_dano = 0.3f;
                TrocarAnimacaoBoss1(2);

                projetil_inimigo.ativo = 0;
                projetil_inimigo.rebatido = 0;

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

                projetil_inimigo.corpo.x = inimigo.posicao.x;
                projetil_inimigo.corpo.y = GetRandomValue(ALTURA_PROJETIL_MIN, ALTURA_PROJETIL_MAX);
            }

            //Testa se o jogador tomou dano ou Quando a Rose tomar dano

            else if (projetil_inimigo.ativo && !projetil_inimigo.rebatido && CheckCollisionCircleRec(jogador.posicao, 20, projetil_inimigo.corpo) && tempo_sem_receber_dano <=0){
                jogador.vida -= 18;
                tempo_sem_receber_dano = 1.0f;
                tempo_recebendo_dano_jogador = 0.2f;

                projetil_inimigo.ativo = 0;
                projetil_inimigo.rebatido = 0;

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

                projetil_inimigo.corpo.x = inimigo.posicao.x;
                projetil_inimigo.corpo.y = GetRandomValue(ALTURA_PROJETIL_MIN, ALTURA_PROJETIL_MAX);

            }

            //Inimigo perdeu ou Jogador perdeu

            if (jogador.vida <= 0){
                jogador.vida = 0;
                atacando = 0;
                aparando = 0;
                projetil_inimigo.ativo = 0;
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

        case tela_batalha_boss2:{
            if (jogador.vida <= 0) {
                jogador.vida = 0;

                TrocarAnimacaoRose(4);
                AtualizarAnimacao(&rose_dying);

                if (rose_dying.terminou) {
                    ModoDoJogo = tela_GameOver;
                }

                break;
            }
            if (inimigo.vida <= 0) {
                inimigo.vida = 0;

                atacando = 0;
                aparando = 0;
                projetil_inimigo.ativo = 0;
                avisando_ataque_inimigo = 0;

                if (animacao_atual_boss2 != 4) {
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
                        ModoDoJogo = tela_animacaoFinal;
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
                    inimigo.posicao.x,
                    inimigo.posicao.y,
                    80,
                    100
                };

                if (CheckCollisionRecs(ataque_jogador, corpo_inimigo)){
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

            if (!projetil_inimigo.ativo && !avisando_ataque_inimigo && cooldown_projetil <= 0){
                
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
                    projetil_inimigo.ativo = 1;
                    projetil_inimigo.rebatido = 0;
                    projetil_inimigo.especial = proximo_projetil_especial;

                    projetil_inimigo.corpo.x = inimigo.posicao.x;
                    projetil_inimigo.corpo.y = GetRandomValue(ALTURA_PROJETIL_MIN, ALTURA_PROJETIL_MAX);
                }
            }

            //Se o projétil está ativo, ele anda para a esquerda (eixo x negativo)

            if (projetil_inimigo.ativo){

                if (projetil_inimigo.rebatido){
                    projetil_inimigo.corpo.x += velocidade_atual_projetil; //Vai da esquerda para a direita
                }else{
                    projetil_inimigo.corpo.x -= velocidade_atual_projetil; //Vai da direita para a esquerda
                }

                if (!projetil_inimigo.rebatido && projetil_inimigo.corpo.x < -50){
                    projetil_inimigo.ativo = 0;
                    projetil_inimigo.rebatido = 0;
                
                    if (tiros_rajada_restantes > 0){
                        cooldown_projetil = INTERVALO_RAJADA_BOSS;
                    }else{
                        cooldown_projetil = tempo_cooldown_atual_projetil;
                    }

                    projetil_inimigo.corpo.x = inimigo.posicao.x;
                    projetil_inimigo.corpo.y = GetRandomValue(ALTURA_PROJETIL_MIN, ALTURA_PROJETIL_MAX);
                }

                if (projetil_inimigo.rebatido && projetil_inimigo.corpo.x > LARGURA_TELA + 50){
                    projetil_inimigo.ativo = 0;
                    projetil_inimigo.rebatido = 0;

                    if (tiros_rajada_restantes > 0){
                        cooldown_projetil = INTERVALO_RAJADA_BOSS;
                    }else{
                        cooldown_projetil = tempo_cooldown_atual_projetil;
                    }

                    projetil_inimigo.corpo.x = inimigo.posicao.x;
                    projetil_inimigo.corpo.y = GetRandomValue(ALTURA_PROJETIL_MIN, ALTURA_PROJETIL_MAX);

                }
            }

            //Tempo invencível após receber o dano

            if (tempo_sem_receber_dano > 0){
                tempo_sem_receber_dano -= GetFrameTime(); //GetFrameTime é da Raylib, e representa o tempo entre um frame e outro, nesse caso é para marcar o tempo de receber dano apenas quando enconsta

            }

           
            //Lógica da colisão do projétil com o parry, testa se o jogador aparou

            if (aparando && projetil_inimigo.ativo && !projetil_inimigo.rebatido && CheckCollisionCircleRec(jogador.posicao, AREA_PARRY, projetil_inimigo.corpo)){
                projetil_inimigo.rebatido = 1;
                tempo_texto_parry = 0.4f;
                aparando = 0;
            }

            Rectangle corpo_inimigo_rebatido = {
                inimigo.posicao.x,
                inimigo.posicao.y,
                80,
                100
            };

            //Quando o projétil rebatido acerta o boss

            if (projetil_inimigo.ativo && projetil_inimigo.rebatido && CheckCollisionRecs(projetil_inimigo.corpo, corpo_inimigo_rebatido)){
                inimigo.vida -= 20;
                tempo_recebendo_dano_inimigo = 0.15f;
                tempo_texto_dano = 0.3f;
                TrocarAnimacaoBoss2(3);
                

                projetil_inimigo.ativo = 0;
                projetil_inimigo.rebatido = 0;

                if (tiros_rajada_restantes > 0){
                    cooldown_projetil = INTERVALO_RAJADA_BOSS;
                }else{
                    cooldown_projetil = tempo_cooldown_atual_projetil;
                }

                projetil_inimigo.corpo.x = inimigo.posicao.x;
                projetil_inimigo.corpo.y = GetRandomValue(ALTURA_PROJETIL_MIN, ALTURA_PROJETIL_MAX);
            }

            //Testa se o jogador tomou dano ou Quando a Rose tomar dano

            else if (projetil_inimigo.ativo && !projetil_inimigo.rebatido && CheckCollisionCircleRec(jogador.posicao, 20, projetil_inimigo.corpo) && tempo_sem_receber_dano <=0){
               
                if (projetil_inimigo.especial){
                    jogador.vida -= 5;
                    tempo_incapacitado_jogador = TEMPO_INCAPACITADO_JOGADOR;
                }else{
                    jogador.vida -= 10;
                }

                tempo_sem_receber_dano = 1.0f;
                tempo_recebendo_dano_jogador = 0.2f;

                projetil_inimigo.ativo = 0;
                projetil_inimigo.rebatido = 0;

                if (tiros_rajada_restantes > 0){
                    cooldown_projetil = INTERVALO_RAJADA_BOSS;
                }else{
                    cooldown_projetil = tempo_cooldown_atual_projetil;
                }

                projetil_inimigo.corpo.x = inimigo.posicao.x;
                projetil_inimigo.corpo.y = GetRandomValue(ALTURA_PROJETIL_MIN, ALTURA_PROJETIL_MAX);

            }

            //Inimigo perdeu ou Jogador perdeu

            if (jogador.vida <= 0){
                jogador.vida = 0;
                atacando = 0;
                aparando = 0;
                projetil_inimigo.ativo = 0;
                TrocarAnimacaoRose(4);
            }

            else if (inimigo.vida <= 0){
                inimigo.vida = 0;

                atacando = 0;
                aparando = 0;
                projetil_inimigo.ativo = 0;
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
                ModoDoJogo = tela_menu;
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
            
        case tela_transicao_boss:

            if (IsKeyPressed(KEY_ENTER)){
                IndiceCenaTransicaoBoss++;
            }

            if (IndiceCenaTransicaoBoss >= TOTAL_CENAS_TRANSICAO_BOSS){
                Boss2();
                ModoDoJogo = tela_batalha_boss2;
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

            // Color corInimigo = RED;

            // if (tempo_recebendo_dano_inimigo > 0){
            //     corInimigo = WHITE;
            // }

            // DrawRectangleV(inimigo.posicao, (Vector2){80,100}, corInimigo);
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
                inimigo.posicao.x - 60,
                inimigo.posicao.y - 120,
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

                DrawRectangleLines(
                    destinoBoss.x,
                    destinoBoss.y,
                    destinoBoss.width,
                    destinoBoss.height,
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

            if (projetil_inimigo.ativo){
                if (projetil_inimigo.rebatido){
                    DrawRectangleRec(projetil_inimigo.corpo, SKYBLUE);
                }else{
                    DrawRectangleRec(projetil_inimigo.corpo, ORANGE);
                }

            }

            DrawText(TextFormat("Jogador HP: %d", jogador.vida), 50,100,20,WHITE);
            DrawText(TextFormat("Inimigo HP: %d", inimigo.vida), 50,130,20,RED);

            if (inimigo.vida <= VIDA_MAX_INIMIGO / 2){
                DrawText("FASE 2",650,50,20,RED);
            }

            if (tiros_rajada_restantes > 0){
                DrawText("ENFURECIDO!", 640,80,20,YELLOW);
            }

            DrawRectangle(50,160,300,20,DARKGRAY);
            float larguraVidaBoss = ((float)inimigo.vida / VIDA_MAX_INIMIGO) * 300;
            if (larguraVidaBoss < 0){
                larguraVidaBoss = 0;
            }

            DrawRectangle(50,160,larguraVidaBoss,20,RED);
            DrawRectangleLines(50,160,300,20,WHITE);

            DrawText("Pressione A para atacar", 50,200,20,WHITE);
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
                inimigo.posicao.x - 70,
                inimigo.posicao.y - 100,
                250,
                300
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
                    destinoBoss2.x + 20,
                    destinoBoss2.y + 35,
                    80,
                    YELLOW
                );

                DrawRectangleLines(
                    destinoBoss2.x,
                    destinoBoss2.y,
                    destinoBoss2.width,
                    destinoBoss2.height,
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

            if (projetil_inimigo.ativo){

                if (projetil_inimigo.rebatido){
                    DrawRectangleRec(projetil_inimigo.corpo, SKYBLUE); //Projétil rebatido

                }else if (projetil_inimigo.especial){
                    DrawRectangleRec(projetil_inimigo.corpo, PURPLE); //Projétil especial

                }else{
                    DrawRectangleRec(projetil_inimigo.corpo, ORANGE); //Projétil normal
                }

            }

            if (tempo_incapacitado_jogador > 0){
                DrawText("SILENCIADO!", jogador.posicao.x - 35, jogador.posicao.y - 70,20,PURPLE); 
            }

            DrawText(TextFormat("Jogador HP: %d", jogador.vida), 50,100,20,WHITE);
            DrawText(TextFormat("Inimigo HP: %d", inimigo.vida), 50,130,20,RED);

            if (inimigo.vida <= VIDA_MAX_INIMIGO_BOSS2 / 2){
                DrawText("FASE 2",650,50,20,RED);
            }

            if (tiros_rajada_restantes > 0){
                DrawText("ENFURECIDO!", 640,80,20,YELLOW);
            }

            DrawRectangle(50,160,300,20,DARKGRAY);
            float larguraVidaBoss = ((float)inimigo.vida / VIDA_MAX_INIMIGO_BOSS2) * 300;
            if (larguraVidaBoss < 0){
                larguraVidaBoss = 0;
            }

            DrawRectangle(50,160,larguraVidaBoss,20,RED);
            DrawRectangleLines(50,160,300,20,WHITE);

            DrawText("Pressione A para atacar", 50,200,20,WHITE);
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

        case tela_transicao_boss:{

            DrawText("BOSS 2 - A MARIONETE",250,80,40,RED);

            DrawRectangle(80,140,640,280,BLACK);
            DrawRectangleLines(80,140,640,280,WHITE);

            DrawText(textos_transicao_boss[IndiceCenaTransicaoBoss],110,450,20,WHITE);
            DrawText("ENTER para continuar",180,520,20,GRAY);

            break;

        }

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
            DrawText("VOCÊ PERDEU",260,250,60,RED);
            DrawText("Pressione ENTER para tentar novamente",160,330,20,WHITE);

            break;

    }

}