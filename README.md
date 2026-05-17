# PROJETO-PIF
Desenvolvimento de jogo para a cadeira de Programação Imperativa Funcional

gcc main.c game.c -o jogo.exe -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm

Manual do Projeto:
https://docs.google.com/document/d/13pf4KvmaYNXZTb2gA_sq3NhMEbgtJOwITAk_XaqN4Sc/edit?usp=sharing

To do com tarefas concluídas, pendentes e em progresso:
https://app.clickup.com/90171191673/v/b/2kz9ycbt-457

👉 História estilo Undertale
👉 Combate estilo Cuphead

------------------------------------------------- SEMANA 1 -------------------------------------------------

Boss 1 - Palhaço

ensinar movimentação
ensinar ataque
ensinar desvio
ensinar parry
dar sensação de primeira vitória

projétil horizontal
cooldown previsível
parry possível
vida visual
fase 2 simples quando chegar em metade da vida

vida 250 até 100:
- projétil normal
- velocidade média
- cooldown 1s

vida 100 até 0:
- projétil mais rápido
- cooldown 0.7s
- talvez às vezes dispara 2 projéteis seguidos

Boss 2 fase 1
velocidade de fase 2 do Boss 1
cooldown de fase 2 do Boss 1
rajada com 2 ou 3 tiros
ataque especial a cada 4 ataques

Boss 2 fase 2
ataque especial a cada 3 ataques
rajada mais frequente
talvez projétil um pouco mais rápido

DIFERENÇA ENTRE OS BOSSES

Boss 1 / Palhaço:
- 1 projétil por vez
- velocidade média/alta
- parry fácil
- fase 2 com cooldown menor

Boss 2 / Marionete:
- Soltar um ataque especial que, quando acertado no jogador, deixa-o incapaz de atacar e rebater por alguns segundos
- 2 ou 3 projéteis
- alturas mais variadas
- ataques em sequência
- parry mais necessário
- talvez projétil falso ou lento/rápido alternando
