# PROJETO-PIF
Desenvolvimento de jogo para a cadeira de Programação Imperativa Funcional

gcc main.c game.c -o jogo.exe -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm

👉 História estilo Undertale
👉 Combate estilo Cuphead

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

Boss 2 - Marionete

testar reflexo
testar parry
testar leitura de padrão
parecer mais assustadora e imprevisível

Ela pode ter:

projéteis em alturas diferentes
ataque falso
sequência de 3 projéteis
cooldown menor
fase 2 mais agressiva

DIFERENÇA ENTRE OS BOSSES

Boss 1 / Palhaço:
- 1 projétil por vez
- velocidade média/alta
- parry fácil
- fase 2 com cooldown menor

Boss 2 / Marionete:
- 2 ou 3 projéteis
- alturas mais variadas
- ataques em sequência
- parry mais necessário
- talvez projétil falso ou lento/rápido alternando
