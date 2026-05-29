# Carnival Story

Carnival Story é um jogo desenvolvido em linguagem C utilizando a biblioteca Raylib. O projeto é executado por linha de comando e utiliza assets locais para imagens, músicas, animações e telas do jogo.

O jogo foi desenvolvido como projeto acadêmico e segue a proposta de implementação em C, com compilação via terminal. Para a entrega da disciplina, o ambiente recomendado é Linux, MacOS ou WSL. O Windows com MSYS2 também pode ser usado para desenvolvimento e testes.

# Vídeo de demonstração

O vídeo abaixo mostra o jogo sendo executado, incluindo a navegação inicial, as batalhas principais e a tela de vitória.

Link do vídeo no YouTube:

[Assista ao vídeo de demonstração aqui](LinkdoVideo)

# Artefatos da entrega

Este projeto possui os seguintes artefatos solicitados:

- Código fonte disponível no GitHub;
- Assets necessários para execução do jogo;
- Instruções de compilação e uso no README;
- Makefile ou linha de comando para compilação;
- Vídeo de demonstração do jogo no YouTube;
- Slides de apresentação enviados pelo Classroom.

Link do vídeo:

[COLE_AQUI_O_LINK_DO_YOUTUBE](COLE_AQUI_O_LINK_DO_YOUTUBE)

Link do repositório:

[COLE_AQUI_O_LINK_DO_GITHUB](https://github.com/juliatenoriocalado/PROJETO-PIF)

# Requisitos

Para compilar e executar o jogo, é necessário ter:

- GCC instalado;
- Raylib instalada;
- Make instalado;
- Terminal compatível;
- Pasta `assets` mantida junto aos arquivos do jogo.

# Estrutura esperada do projeto

O jogo deve ser executado a partir da pasta `CarnivalStory`, onde ficam os arquivos `.c`, `.h` e a pasta `assets`.

Exemplo de estrutura:

    PROJETO-PIF/
    └── CarnivalStory/
        ├── main.c
        ├── game.c
        ├── game.h
        ├── Makefile
        └── assets/
            ├── background/
            ├── boss1/
            ├── boss2/
            ├── gameover/
            ├── intro/
            ├── menu/
            ├── music/
            ├── rose/
            ├── transicao/
            └── victory/

É importante manter a pasta `assets` no mesmo local dos arquivos `.c`, pois o jogo carrega imagens, músicas e animações usando caminhos relativos.

Exemplos de caminhos usados pelo jogo:

    assets/background/background.png
    assets/music/track_menu.ogg
    assets/music/track_victory.ogg
    assets/victory/victory1.png

# Como baixar o projeto

Clone o repositório:

    git clone https://github.com/juliatenoriocalado/PROJETO-PIF.git

Entre na pasta do jogo:

    cd PROJETO-PIF/CarnivalStory

# Makefile do projeto

Dentro da pasta `CarnivalStory`, crie um arquivo chamado exatamente:

    Makefile

Sem extensão.

Conteúdo recomendado para o Makefile:

    CC = gcc
    SRC = *.c
    TARGET = jogo

    UNAME_S := $(shell uname -s)

    ifeq ($(UNAME_S),Darwin)
    	LIBS = -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    else
    	LIBS = -lraylib -lm -ldl -lpthread -lGL -lrt -lX11
    endif

    all:
    	$(CC) $(SRC) -o $(TARGET) $(LIBS)

    run: all
    	./$(TARGET)

    clean:
    	rm -f $(TARGET) jogo.exe

Atenção: no Makefile, as linhas abaixo de `all:`, `run:` e `clean:` precisam começar com TAB, não com espaços.

# Rodando no Linux

Entre na pasta do jogo:

    cd CarnivalStory

Compile com:

    make

Execute com:

    ./jogo

Também é possível compilar e executar diretamente com:

    make run

Para apagar o executável gerado:

    make clean

# Rodando no WSL

No Windows, abra o terminal do WSL e entre na pasta do projeto.

Exemplo:

    cd /mnt/c/Users/SEU_USUARIO/Documents/PROJETO-PIF/CarnivalStory

Compile com:

    make

Execute com:

    ./jogo

Ou compile e execute diretamente:

    make run

Observação: para rodar jogos gráficos no WSL, é necessário que o ambiente gráfico esteja funcionando corretamente. Em algumas máquinas, pode ser mais simples executar em Linux nativo, máquina virtual Linux ou Windows com MSYS2 para testes.

# Rodando no MacOS

Entre na pasta do jogo:

    cd CarnivalStory

Compile com:

    make

Execute com:

    ./jogo

Ou compile e execute diretamente:

    make run

# Linha de comando manual para Linux/WSL

Caso o Makefile não funcione, compile manualmente com:

    gcc *.c -o jogo -lraylib -lm -ldl -lpthread -lGL -lrt -lX11

Depois execute:

    ./jogo

# Linha de comando manual para MacOS

Caso o Makefile não funcione no MacOS, compile manualmente com:

    gcc *.c -o jogo -lraylib -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo

Depois execute:

    ./jogo

# Rodando no Windows com MSYS2/UCRT64

Embora o ambiente principal recomendado para a entrega seja Linux, MacOS ou WSL, o jogo também pode ser compilado no Windows usando MSYS2/UCRT64.

Abra o terminal MSYS2 UCRT64 e entre na pasta do jogo.

Exemplo:

    cd "/c/Users/SEU_USUARIO/Documents/PROJETO-PIF/CarnivalStory"

Compile com:

    gcc *.c -o jogo.exe -I/c/msys64/ucrt64/include -L/c/msys64/ucrt64/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lm

Execute com:

    ./jogo.exe

# Rodando no Windows com Raylib instalada em C:\raylib

Caso esteja usando a instalação tradicional da Raylib no Windows, também é possível compilar pelo PowerShell.

Entre na pasta do jogo:

    cd "C:\Users\SEU_USUARIO\Documents\PROJETO-PIF\CarnivalStory"

Compile com:

    & "C:\raylib\w64devkit\bin\gcc.exe" *.c -o jogo.exe -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm -lm

Execute com:

    .\jogo.exe

# Comandos do jogo

Durante a execução, os comandos principais são:

    ENTER - avançar telas, iniciar o jogo e confirmar transições
    Seta para a direita - mover a personagem para a direita
    Seta para a esquerda - mover a personagem para a esquerda
    ESPAÇO - pular
    A - atacar
    Q - aparar/rebater projéteis

# Objetivo do jogo

O jogador controla Rose em uma sequência de batalhas contra chefes. O objetivo é sobreviver aos ataques, usar o parry para rebater projéteis, derrotar os inimigos e alcançar a tela de vitória.

# Principais telas do jogo

O jogo possui:

- Menu inicial;
- Introdução;
- Batalha contra o primeiro chefe;
- Transição entre chefes;
- Introdução do segundo chefe;
- Batalha contra a Marionette;
- Tela de Game Over;
- Tela de vitória.

# Detalhes técnicos

O projeto foi estruturado em linguagem C com Raylib.

Alguns pontos principais da implementação:

- Controle de estados do jogo com enum;
- Separação entre atualização lógica e desenho na tela;
- Sistema de animações com struct própria;
- Carregamento de texturas, músicas e sprites por caminhos relativos;
- Sistema de batalha com dois chefes;
- Ataque da personagem;
- Sistema de parry;
- Projéteis rebatíveis;
- Lista encadeada para controle de projéteis;
- Matriz dinâmica para controle do dano progressivo do parry;
- Sistema de música por estado do jogo;
- Tela de vitória com imagens e música própria.

# Principais funções

Algumas das funções principais do projeto são:

    CarregarAssets()

Carrega imagens, músicas, sprites e animações utilizadas no jogo.

    DescarregarAssets()

Descarrega texturas, músicas e libera memória antes do encerramento do jogo.

    AtualizarJogo()

Controla a lógica principal do jogo de acordo com a tela atual.

    DesenharJogo()

Desenha na tela o menu, as batalhas, as transições, o game over e a vitória.

    Boss2()

Inicializa os dados da segunda batalha, como vida, posição do jogador, posição do chefe, projéteis e animações.

    AdicionarProjetil()

Adiciona um projétil à lista encadeada.

    LimparProjeteis()

Remove projéteis ativos e libera a memória usada pela lista encadeada.

    CriarMatrizDanoParry()

Cria dinamicamente a matriz usada para controlar o dano do parry.

    ObterDanoParry()

Retorna o dano atual do parry com base na posição atual da matriz.

    AumentarComboParry()

Avança o combo do parry após um acerto bem-sucedido.

    ReduzirComboParryAoTomarDano()

Reduz o combo do parry quando a personagem recebe dano.

# Problemas comuns

## O jogo não abre ou fecha imediatamente

Verifique se você está executando o jogo a partir da pasta `CarnivalStory`.

A pasta `assets` precisa estar no mesmo diretório do executável.

Estrutura correta no Linux/MacOS/WSL:

    CarnivalStory/
    ├── jogo
    └── assets/

Estrutura correta no Windows:

    CarnivalStory/
    ├── jogo.exe
    └── assets/

## Erro ao carregar textura

Se aparecer uma mensagem como:

    Erro ao carregar textura

verifique se o nome do arquivo e a extensão estão exatamente iguais ao caminho usado no código.

Exemplo:

    LoadTexture("assets/victory/victory1.png");

Nesse caso, o arquivo precisa estar exatamente em:

    assets/victory/victory1.png

Também confira se o arquivo está como `.png`, `.jpg` ou `.jpeg`, pois a extensão no código precisa bater com a extensão real do arquivo.

## Música não toca

Verifique se o arquivo de música está na pasta correta.

Exemplo:

    assets/music/track_victory.ogg

Também confira se o nome usado no código está igual ao nome do arquivo.

## Comando make não encontrado

No Linux, WSL ou MacOS, instale o `make` no ambiente.

No Windows/MSYS2, pode ser necessário usar:

    mingw32-make

em vez de:

    make

## Raylib não encontrada

Se aparecer erro dizendo que `raylib.h` não foi encontrado ou que `-lraylib` não existe, significa que a Raylib não está instalada ou não está no caminho esperado pelo compilador.

Nesse caso, instale a Raylib no ambiente utilizado ou ajuste os caminhos de include e library no comando de compilação.

# Observação sobre compatibilidade

O projeto foi implementado em C e pode ser compilado por linha de comando. Porém, não é possível garantir que funcione automaticamente em qualquer máquina sem configuração prévia, pois o jogo depende do GCC, da Raylib e do ambiente gráfico funcionando corretamente.

Para a entrega da disciplina, o ambiente recomendado é Linux, MacOS ou WSL. O Windows com MSYS2 foi incluído como alternativa para desenvolvimento e testes.

# Arquivos que não devem ser commitados

O executável gerado não precisa ser enviado ao GitHub.

Evite commitar arquivos como:

    jogo
    jogo.exe

Cada pessoa deve compilar o executável na própria máquina usando as instruções acima.

# Resumo rápido

Linux, WSL ou MacOS:

    cd CarnivalStory
    make
    ./jogo

Windows com MSYS2/UCRT64:

    cd "/c/Users/SEU_USUARIO/Documents/PROJETO-PIF/CarnivalStory"
    gcc *.c -o jogo.exe -I/c/msys64/ucrt64/include -L/c/msys64/ucrt64/lib -lraylib -lopengl32 -lgdi32 -lwinmm -lm
    ./jogo.exe

Windows com Raylib em C:\raylib:

    cd "C:\Users\SEU_USUARIO\Documents\PROJETO-PIF\CarnivalStory"
    & "C:\raylib\w64devkit\bin\gcc.exe" *.c -o jogo.exe -I"C:\raylib\raylib\src" -L"C:\raylib\raylib\src" -lraylib -lopengl32 -lgdi32 -lwinmm -lm
    .\jogo.exe
