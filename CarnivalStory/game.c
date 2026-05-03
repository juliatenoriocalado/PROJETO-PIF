//Controle dos estados do jogo

//==============================================
//             Variáveis globais
//==============================================

#include "game.h"
#include <stdio.h>

Telas ModoDoJogo = tela_menu;

int IndiceCenaFinal = 0; //Contador para marcas as cenas sendo passadas

const char *textos_final[] = { //Vetor de textos //Ponteiro para caractere (marcar o começo de uma string)
    "No final, Rose acorda...",
    "E percebe que, no fim, tudo não passava de um 'sonho'.",
    "Ela se levantou, e finalmente adquiriu a coragem que precisava para o show..."
};

//Aqui eu estou criando uma variável do tipo enum "Telas" para marcar o início ao menu do jogo

Jogador jogador;
Inimigo inimigo;
Projetil_Inimigo projetil_inimigo;
float tempo_sem_receber_dano = 0; //Para o jogador não receber dano várias vezes com um mesmp projétil
float cooldown_projetil = 0;
Rectangle ataque_jogador;
float tempo_ataque = 0;
int atacando = 0; //Se o jogador está atacando
int aparando = 0; //Se o jogador está aparando
float tempo_parry; //Quanto tempo para o parry acabar
float cooldown_parry = 0; //Quanto tempo para o parry poder ser usado de novo
float tempo_texto_parry = 0; //Tempo em que o texto "parry!" é exibido
int avisando_ataque_inimigo = 0;
float tempo_aviso_ataque_inimigo = 0;
float tempo_recebendo_dano_jogador = 0;
float tempo_recebendo_dano_inimigo = 0;
float tempo_texto_dano = 0;
int contador_ataques_inimigo = 0;
int tiros_rajada_restantes = 0; //Controla para saber se falta tiros de rajada extra faltando

//Aqui estamos criando as variáveis tanto para personagem quanto para os nossos inimigos (cada um com seus respectivos structs, os de letra maiúscula, para marcar do formato para o objeto)

//==============================================================================================================================================================================================
//                                                                            Inicalização do jogo! 
//==============================================================================================================================================================================================

void InitGame(){ //Aqui é onde inicializamos todas as variáveis globais, por exemplo, para o jogo poder começar de fato

    jogador.posicao = (Vector2){400, COORDENADA_CHAO}; //X = 400 (posição horizontal) e Y = COORDENADA_CHAO (começa no)
    inimigo.posicao = (Vector2){650, COORDENADA_CHAO - 100};

    //Coordenadas do jogador

    jogador.vida = VIDA_MAX_JOGADOR;
    jogador.no_chao = 1;
    jogador.velocidade_pulo = 0;

    ataque_jogador = (Rectangle){0,0,LARGURA_ATAQUE,ALTURA_ATAQUE};
    tempo_ataque = 0;
    atacando = 0;

    aparando = 0;
    tempo_parry = 0;
    cooldown_parry = 0;
    tempo_texto_parry = 0;

    tempo_recebendo_dano_jogador = 0;
    tempo_recebendo_dano_inimigo = 0;
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
    cooldown_projetil = TEMPO_COOLDOWN_PROJETIL_FASE1;

    tempo_sem_receber_dano = 0;

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

            //Limite o jogador dentro da tela

            if (jogador.posicao.x < 20) jogador.posicao.x = 20;
            if (jogador.posicao.x > LARGURA_TELA - 20) jogador.posicao.x = LARGURA_TELA - 20;



            //Se eu estou pressionando A e eu não estava atacando...

            if (IsKeyPressed(KEY_A) && !atacando){
                atacando = 1;
                tempo_ataque = TEMPO_ATAQUE;

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

            if (IsKeyPressed(KEY_Q) && cooldown_parry <= 0 && !aparando){
                aparando = 1;
                tempo_parry = TEMPO_PARRY;
                cooldown_parry = COOLDOWN_PARRY;
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

                int ataque_de_rajada = 0;

                if (tiros_rajada_restantes > 0){
                    tiros_rajada_restantes--;
                    ataque_de_rajada = 1;
                }

                else if (inimigo.vida <= VIDA_MAX_INIMIGO / 2){ //Ativa configuração para o modo 2 (que é o modo de quando o inimigo fica com metade da vida)
                    contador_ataques_inimigo++; //Começa a contar a quantidade de ataques

                    if (contador_ataques_inimigo >= QUANTIDADE_DE_ATAQUES_PARA_RAJADA){ //Se a quantidade de ataques for igual a quantidade de ataques para a rajada 
                        tiros_rajada_restantes = 1; //Ele conta a rajada
                        contador_ataques_inimigo = 0; //Zera o contador para contar de novo
                    }
                }

                avisando_ataque_inimigo = 1;

                if (ataque_de_rajada){
                    tempo_aviso_ataque_inimigo = 0.08f;
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
                jogador.vida -= 10;
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
                ModoDoJogo = tela_GameOver;
            }

            else if (inimigo.vida <= 0){
                IndiceCenaFinal = 0;
                ModoDoJogo = tela_animacaoFinal;
            }

            break;

            //Fim da batalha

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

            Color corJogador = BLUE;

            if (tempo_recebendo_dano_jogador > 0){
                corJogador = RED;
            }

            DrawCircleV (jogador.posicao, 20, corJogador);

            if (atacando){
                DrawRectangleLinesEx(ataque_jogador,2,PINK);
            }

            Color corInimigo = RED;

            if (tempo_recebendo_dano_inimigo > 0){
                corInimigo = WHITE;
            }

            DrawRectangleV(inimigo.posicao, (Vector2){80,100}, corInimigo);

            if (tempo_texto_dano > 0){
                DrawText("ATACOU!", inimigo.posicao.x + 10, inimigo.posicao.y - 30, 20, YELLOW);
            }

            if (avisando_ataque_inimigo){
                DrawText("!", inimigo.posicao.x + 35, inimigo.posicao.y - 35, 40, YELLOW); //Mostra a exclamação antes do palhaço atacar 
                DrawRectangleLines(inimigo.posicao.x - 5, inimigo.posicao.y - 5, 90, 110, YELLOW); //Mostra uma aura amarela antes de atacar
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