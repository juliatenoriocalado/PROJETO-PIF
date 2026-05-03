#include "raylib.h"
#include "game.h"

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