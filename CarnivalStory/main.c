#include "raylib.h"
#include "game.h"

//Lógica que gosto para explicar:
//BeginDrawing() Peguei uma folha de papel para desenhar
//ClearBackground() pintei a folha inteira com um fundo, exemplo: um céu azul
//DesenharJogo() Desenhei meus personagens e detalhes do desenho
//EndDrawing() Mostrei a folha com o desenho pronto na tela

int main(){

    InitWindow(LARGURA_TELA,ALTURA_TELA,"Carnival Story");
    SetTargetFPS(60); //Função da Raylib que define os frames por segundo (FPS), sem isso, o while poderia rodar rápido demais (dependendo do pc), o que poderia prejudicar o desempenho do computador (se for alto demais).
    InitGame(); //Marca o início do jogo

    while (!WindowShouldClose()){ //Enquanto a janela NÃO estiver fechada, continue rodando o jogo nesse lopíng

        AtualizarJogo(); //Lógica: teclas, movimentos, vida, colisão

        BeginDrawing(); //Função especial que marca o começo da parte do desenho da tela (desenha o frame)

        ClearBackground(DARKGRAY); //Limpar tela dos frames desenhador pelo AtualizarJogo
        //Pode ser explicado, também como "pintar o cenário" antes de desenhar

        DesenharJogo(); //Desenha o menu, player, inimigo, textos etc.

        EndDrawing(); //Função especial para terminar o desenho e mostrar a janela (termina de desenhar o frame para finalmente mostrar na tela)

        //Tanto o BeginDrawing e o EndDrawing são necessários, porque o jogo desenha a tela várias vezez por segundo, tipo 60 FPS
        //Tudo que for visual fica entre eles
    }

    CloseWindow(); //Após executar o looping eu fecho a janela
    return 0; //O programa terminou com sucesso

}
