# ultimateTicTacToe
Ultimate TicTacToe Game

Foi utilizada a biblioteca Raylib
Segue as instruções para baixá-la (Para Linux): 
    1) sudo apt install build-essential git
        sudo apt install libasound2-dev mesa-common-dev libx11-dev libxrandr-dev libxi-dev xorg-dev libgl1-mesa-dev libglu1-mesa-dev
    2) git clone https://github.com/raysan5/raylib.git raylib
        cd raylib/src/
    3) make PLATFORM=PLATFORM_DESKTOP
        sudo make install

Instruções para executar corretamente o código:
    4)  Ao jogar contra o robô, o humano sempre será o “X”

    5) Para salvar o jogo é necessário clicar no “x” para sair da tela onde o jogo está 
       acontecendo e digitar “S” no terminal (aparecerá uma mensagem perguntando 
       se deseja salvar o jogo)
    
    6) O comando de execução é: 
       1)g++ ./estruturagrafica.cpp ./logic.cpp ./io_arquivo.cpp -o jogo -lraylib -lm -ldl –
         lpthread
       2)./jogo
    
    7) É feito com nível de integração 3 (Salva e carrega estado de jogo/app (save/load) de forma consistente)

    8) O robô (do modo jogar contra o robô) não joga aleatóriamente, existindo um sistema de "confiança" de jogada

    9) Otimização dos sistemas que envolvem possibilidades de vitória

    10) Interface gráfica com efeitos de vitória e telas alternativas (WELCOME, GAME E END)

