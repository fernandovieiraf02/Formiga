 	/**
 	-------------------------
 		FORMIGA
 		versão 2.0
 	-------------------------
     Fernando Angelo Z Vieira
     Labirinto_formiga
     Engenharia de Computação
     UFGD - FACET
    -------------------------
    */

    #include<stdio.h>
    #include<stdlib.h>
    #include<time.h>
    #include<SDL.h>
    #include<SDL_image.h>
    #include<sys\timeb.h>
    #include"TAD.c"

    #define NORTH 1
    #define EAST 2
    #define SOUTH 3
    #define WEST 4

    #define TAM_X 16
    #define TAM_Y 16

    #define ON 1
    #define OFF 0


	FILE *way_exit = fopen("way_exit.doc", "w");
    Queue *track_way;//listas de adjacenssias
    int **lab;//struct para salva posiçoes de coordenadas.
    int TAM = 0, TAM2 = 0, SCREEN_WIDTH = 0, SCREEN_HEIGHT = 0; /**Algumas variaveis para salvar o tamanho da tela e das imagens.*/

    SDL_Window *main_window = NULL; /** Criando a variavel que armazena um ponteiro para uma janela.*/
    SDL_Renderer *main_render = NULL;/** Criando a variavel que vai renderisa a tela final.*/
	/**variaveis para carregarem as imagens.*/
    SDL_Surface *s_arrow = NULL, *s_grass = NULL, *s_floor = NULL, *s_ant = NULL, *s_door = NULL, *s_win = NULL, *s_wall = NULL;
    /**variaveis para texturizar as imagens.*/
    SDL_Texture *t_arrow = NULL, *t_grass = NULL, *t_floor = NULL, *t_ant = NULL, *t_door = NULL, *t_win = NULL, *t_wall = NULL;
    //salva as coordenadas de entrada e saida do labirinto
    SDL_Rect inicio, stop;


    void Lab_aleatory(void);/** função para gerar o labirinto aleatorio*/

    void read_track(int, int);/**função para ler e listar os caminhos adjacentes*/

    void Percent(char*);/**animação de porcentagem*/

    void wall_lab(void);/**Gera as paredes da borda da janela*/

    void print_lab(void);/**imprime o labirinto no console*/

    int win(SDL_Rect point); /**verifica se o jogador ganhou */

    void graphic(int);/**função que le um arquivo e gera os graficos*/

    int user_walker(SDL_Event*, FILE*);/** função que faz o usuario percorrer o labirinto*/

    void closed(void);/** função que fecha os arquivos, janelas, funçoes da SDL e desaloca ponteiros*/

    int colision(SDL_Rect, int);/** verifica se há colisão com parede em determinada posição da matriz*/

    int Way_Aleatory(int x, int y);/** Sorteia uma direção aleátoria que esteja livre ao redor da posição (x,y)*/

    void load(void);/** função que carrega as imagens, cria a janela e inicializa variaveis*/

    void graphic_pos(SDL_Rect, SDL_Texture*, int);/** função para renderiza um ponto determinado*/

    void graphic_pos_float(SDL_Rect , SDL_Texture*, int);/** função para renderiza um ponto determinado e especifico*/

    int find_exit(SDL_Rect, FILE*, int);/** função que acha uma saida sozinha. O utimo parâmetro serve para informar, se o procedimento deve salvar a saída encontrada em um arquivo.*/


    int main(int argc, char* args[])
    {
        int diff, i, j, minutos, way_cont = 0, speed;
        struct timeb start, end; //para contar o tempo
        Stack* saida;

        SDL_Event event;//variavel para captar os eventos do usuario.
        //arquivo que vai salva a matriz
        FILE *way_user = fopen("way_user.txt", "w");


        printf("\nInforme o tamanho do labirinto, (m x n): ");
        scanf("%d%*[x]%d", &TAM, &TAM2);//salva o tamanho da matriz
        printf("\nInforme a posicao da entrada: ");
        scanf("%d%*[x]%d", &inicio.y, &inicio.x);
        printf("\nInforme a posicao da saida: ");
        scanf("%d%*[x]%d", &stop.y, &stop.x);
        printf("\ntempo (>=1)ms: ");
        scanf("%d", &speed);

        Percent("load...");
        load();//inicializa as funcoes e variaveis.
        srand((unsigned)time(NULL));// alimenta a função com milesimos do relogio para gerar numeros aleatorio.

		//Gera as paredes e chão de fundo
        wall_lab();

        //Gera o labirinto
        Lab_aleatory();

        //Gera a fila de adjacência
        read_track(inicio.y, inicio.x);

        //Renderiza os graficos
        graphic(speed);
        graphic_pos(inicio, t_door, ON);
        graphic_pos(stop, t_door, ON);

		/**-----------TEMPO DE JOGO-----------------------------------*/
        ftime(&start);//inicializa o cronômetro.
        way_cont = user_walker(&event, way_user);
        ftime(&end);//Para o cronômetro.

        diff = (int)((end.time - start.time));

		minutos = 0;
        if(diff>60) minutos = diff/60;
        /**-----------------------------------------------------------*/
        /**--------------ENCONTRA A SAÍDA-----------------------------*/
        Init_Stack(&saida);
        Push_Stack(&saida, inicio.x, inicio.y);
        find_exit(inicio, way_exit, ON);
        /**-----------------------------------------------------------*/

        printf("\nseu tempo de jogo foi %dm:%2ds", minutos, (diff - minutos*60));
        printf("\no numero de movimentos foi de %d\n", way_cont);
        fprintf(way_user,"\nTempo da partida: %dm:%2ds\n",  minutos, (diff - minutos*60));

        SDL_Delay(2000);

        closed();//fecha tudo.
        return 0;
    }

    void Percent(char* text)
    {
        for(int i=0; i<101; i++)
        {
            printf("\n%s %d%%", text, i);
            SDL_Delay(1);
        }
    }

    void wall_lab(void)//Gera as paredes e o chão de fundo.
    {
    	int loading = 0;
        SDL_Rect pos;
        pos.w = TAM_X;
        pos.h = TAM_Y;

        for(int i=0; i<TAM; i++)
            for(int j=0; j<TAM2; j++)
            {
            	printf("\nLoading Wall... step %d\n", ++loading);
                if(i==0 || j==0 || i==TAM-1 || j==TAM2-1)/*Toda vez que estiver na ultima linha ou coluna ira colocar uma parede*/
                {
                	lab[j][i] = -1;
                    pos.x = j;
                    pos.y = i;
                    graphic_pos(pos ,t_wall, ON);
                }
                else//Caso contrário ira colocar mostrar a imagem do chão de fundo.
                {
                    pos.x = j;
                    pos.y = i;
                    graphic_pos(pos ,t_floor, ON);
                }
            }
    }

    int Way_Aleatory(int x, int y)
    {
        ///Esta função verifica quantos caminhos possiveis podem ser sorteados
        ///e retorna uma direçao aleatoria, caso nao haja caminho livre retorna zero
        int i, way[4] = {}, cont = OFF;

        if(lab[x][y-1] == OFF)//se estiver livre a posição NORTE.
        {
            way[0] = NORTH;
            cont++;//sinaliza que há pelo menos um caminho livre
        }
        if(lab[x+1][y] == OFF)//se estiver livre a posição LESTE.
        {
            way[1] = EAST;
            cont++;//sinaliza que há pelo menos um caminho livre
        }
        if(lab[x][y+1] == OFF)//se estiver livre a posição SUL.
        {
            way[2] = SOUTH;
            cont++;//sinaliza que há pelo menos um caminho livre
        }
        if(lab[x-1][y] == OFF)//se estiver livre a posição OESTE.
        {
            way[3] = WEST;
            cont++;//sinaliza que há pelo menos um caminho livre
        }

        while(cont)//enquanto existir um caminho para seguir
        for(i=0; i<4; i++)
            if(way[i] != OFF)//se way na posição 'i' existir um caminho livre
                if(rand()%4+1 == way[i])//se a direção gerada aleatoriamente for igual ao caminho livre, salvo na posiçãp 'i'
                    return way[i];//retorne a direção do caminho livre

        return cont;//caso nao haja nenhum caminho para seguir ele retorna zero
    }

    void Lab_aleatory(void)
    {
        /** o labirinto e gerado a partir da posição que o usuario escolhe
            O algoritmo so para de geras os caminhos ate que nao
            haja mais caminhos para seguir.
            Cada celula de caminho gerado e incrementado 1 para cada
            caminho diferente*/
        int way, x = inicio.x, y = inicio.y, sucess = OFF, dir = ON, loading = 0;

        Stack *pilha;//pilha para armazenar as coordenadas
        Init_Stack(&pilha);//inicializa a pilha
        Push_Stack(&pilha, x, y);

        while(Void_Stack(pilha))//verifica se a pilha nao esta vazia
        {
            printf("\nLoading Lab_aleatory... step %d\n", ++loading);
            while(sucess == OFF)
            {
                way = Way_Aleatory(x,y);//direção aleatoria
                if(x==stop.x && y==stop.y)//se estiver na posição da saída do labirinto
                    way = OFF;//sinaliza a flag

                if(lab[x][y] == OFF)//serve para sinalizar o começo dos caminhos do labirinto
                {
                    lab[x][y] = dir;
                    dir++;//incrementa o contador
                }
                if(way == OFF)//verifica se nao esta preso em uma posição
                {
                    Pop_Stack_Double(pilha, &x, &y);
                    sucess = ON;//sinaliza para sair de while e verificar se a pilha de coordenadas nao esta vazia
                }
                else sucess = ON;//caso não houve nenhuma restrição com o caminho gerado
            }
            sucess = OFF;//reseta a flag para o próximo laço
            switch(way)
            {
                case NORTH://NORTE
                        lab[x][--y] = dir;//sinaliza a direção na matriz
                        dir++;//incrementa o contador de caminhos
                        Push_Stack(&pilha, x, y);//salva as coordenada na pilha
                        break;

                case EAST://LESTE
                        lab[++x][y] = dir;//sinaliza a direção na matriz
                        dir++;
                        Push_Stack(&pilha, x, y);//salva as coordenada na pilha
                        break;

                case SOUTH://SUL
                        lab[x][++y] = dir;//sinaliza a direção na matriz
                        dir++;
                        Push_Stack(&pilha, x, y);//salva as coordenada na pilha
                        break;

                case WEST://OESTE
                        lab[--x][y] = dir;//sinaliza a direção na matriz
                        dir++;
                        Push_Stack(&pilha, x, y);//salva as coordenada na pilha
                        break;
            }
        }
    }

    void read_track(int y, int x)
    {
        int cont = ON, dont_back = OFF, start = OFF, vx, vy, i = ON, loading = 0;
        Stack *control;

        Init_Stack(&control);
        Push_Stack(&control, x, y);//Adciona a posição inicial do labirinto a pilha.

        Init_Queue(&track_way);

        while(Void_Stack(control))
        {
            printf("\nLoading read_track... step %d\n", ++loading);
            dont_back = OFF;

            if(lab[x][y-1] == cont+1)//NORTE
            {
                if(start == ON)//se estiver na recursão da pilha. A flag 'start' estará ativada.
                {
                    i = lab[x][y];
                    Push_Queue(&track_way, i, cont+1);//adiciona na fila os caminhos adjacentes.
                    Push_Queue(&track_way, cont+1, i);
                }
                y--;//Atualiza a posição atual em relação ao eixo 'y'.
                Push_Stack(&control, x, y);//Adiciona a pilha sua posição atual.

                start = OFF;//Reseta a flag. Assim o algoritmo sabe, que ja não está em uma recursão mais.
                dont_back++;//Ativa a flag 'dont_back', para informar ao algoritmo que pelo menos um caminho foi encontrado.
				//incrementa os contadores.
                cont++;
            }
            if(lab[x+1][y] == cont+1)//LESTE
            {
               if(start == ON)//se estiver na recursão da pilha. A flag 'start' estará ativada.
                {
                    i = lab[x][y];
                    Push_Queue(&track_way, i, cont+1);//adiciona na fila os caminhos adjacentes.
                    Push_Queue(&track_way, cont+1, i);
                }

                x++;//Atualiza a posição atual em relação ao eixo 'x'.
                Push_Stack(&control, x, y);//Adiciona a pilha sua posição atual.

                start = OFF;//Reseta a flag. Assim o algoritmo sabe, que ja não está em uma recursão mais.
                dont_back++;//Ativa a flag 'dont_back', para informar ao algoritmo que pelo menos um caminho foi encontrado.
				//incrementa os contadores
                cont++;
            }
            if(lab[x][y+1] == cont+1)//SUL
            {
               if(start == ON)//se estiver na recursão da pilha. A flag 'start' estará ativada.
                {
                    i = lab[x][y];
                    Push_Queue(&track_way, i, cont+1);//adiciona na fila os caminhos adjacentes.
                    Push_Queue(&track_way, cont+1, i);
                }

                y++;//Atualiza a posição atual em relação ao eixo 'y'.
                Push_Stack(&control, x, y);//Adiciona a pilha sua posição atual.

                start = OFF;//Reseta a flag. Assim o algoritmo sabe, que ja não está em uma recursão mais.
                dont_back++;//Ativa a flag 'dont_back', para informar ao algoritmo que pelo menos um caminho foi encontrado.
				//incrementa os contadores
                cont++;
            }
            if(lab[x-1][y] == cont+1)//OESTE
            {
                if(start == ON)//se estiver na recursão da pilha. A flag 'start' estará ativada.
                {
                    i = lab[x][y];
                    Push_Queue(&track_way, i, cont+1);//adiciona na fila os caminhos adjacentes.
                    Push_Queue(&track_way, cont+1, i);
                }

                x--;//Atualiza a posição atual em relação ao eixo 'x'.
                Push_Stack(&control, x, y);//Adiciona a pilha sua posição atual.

                start = OFF;//Reseta a flag. Assim o algoritmo sabe, que ja não está em uma recursão mais.
                dont_back++;//Ativa a flag 'dont_back', para informar ao algoritmo que pelo menos um caminho foi encontrado.
				//incrementa os contadores
                cont++;
            }
            if(dont_back == OFF)//Caso não ache nenhum caminho, então certamente está preso em um caminho sem adjacentes.
            {
                Pop_Stack_Double(control, &x, &y);//decrementa os valores da Pila
                start = ON;/**Ativa a flag, para informar ao algoritmo que está em uma recursão e nesse caso, algumas condições mudam seus comportamentos.*/
            }
        }
    }

    void print_lab(void)// mostra a matriz
    {
        int i,j;

        for(i=0;i<TAM;i++)
        {
            for(j=0;j<TAM2;j++)
                printf("%3.d", lab[j][i]);
            printf("\n\n");
        }
    }

    void load(void)//inicializa as funções
    {
        int i;
        TAM+=2, TAM2+=2;//uma linha e uma coluna para as paredes

        inicio.w = TAM_X;
        inicio.h = TAM_Y;

        stop.w = TAM_X;
        stop.h = TAM_Y;

 		///Aqui foi alocado uma matriz para salvar os caminhos
        lab = (int**)calloc(sizeof(int), TAM2);//matriz para salvar as posições
        for(i=0; i<TAM2; i++)
            lab[i] = (int*)calloc(sizeof(int), TAM);

        SDL_Init(SDL_INIT_EVERYTHING);//inicializa a SDL

        SCREEN_HEIGHT = TAM_Y * TAM;//tamanho da tela em relação a altura
        SCREEN_WIDTH = TAM_X * TAM2;//tamanho da tela em relação a largura.

        main_window = SDL_CreateWindow("Labirinto", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_RESIZABLE);// cria a janela
        main_render = SDL_CreateRenderer( main_window, -1, SDL_RENDERER_ACCELERATED);//gera uma tela de renderização para a janela

        s_arrow = IMG_Load("arrow.png");//carrega imagem das flechas
        s_grass = IMG_Load("grass.png");//carrega a imagem da parede
        s_floor = IMG_Load("floor.png");//carrega a imagem do chao
        s_ant = IMG_Load("ant.png");//carrega a imagem da formiga
        s_door = IMG_Load("door.png");//carrega imagem das portas
        s_win = IMG_Load("win.png");//carrega a imagem win
        s_wall = IMG_Load("wall.png");//carrega a imagem da parede externa

		// texturiza as imagens
        t_grass = SDL_CreateTextureFromSurface(main_render, s_grass);
        t_floor = SDL_CreateTextureFromSurface(main_render, s_floor);
        t_ant = SDL_CreateTextureFromSurface(main_render, s_ant);
        t_arrow = SDL_CreateTextureFromSurface(main_render, s_arrow);
        t_door = SDL_CreateTextureFromSurface(main_render, s_door);
        t_win = SDL_CreateTextureFromSurface(main_render, s_win);
        t_wall = SDL_CreateTextureFromSurface(main_render, s_wall);

		//desaloca os ponteiros
        SDL_FreeSurface(s_arrow);
        SDL_FreeSurface(s_grass);
        SDL_FreeSurface(s_floor);
        SDL_FreeSurface(s_ant);
        SDL_FreeSurface(s_door);
        SDL_FreeSurface(s_win);
        SDL_FreeSurface(s_wall);
    }

    int win(SDL_Rect point)
    {
        if(point.x == stop.x && point.y == stop.y)
        {
            SDL_RenderCopy(main_render, t_win, NULL, NULL);
            SDL_RenderPresent(main_render);
            return 1;
        }
        return 0;
    }

    void graphic_pos(SDL_Rect pos, SDL_Texture* texture, int flag)/**Essa função serve para quando não e necessario uma posição especifica na tela  */
    {
        pos.x *= pos.h, pos.y *= pos.w;
        SDL_RenderCopy(main_render, texture, NULL, &pos);//renderiza uma imagem
        if(flag)
            SDL_RenderPresent(main_render);//mostra na tela
    }

    void graphic_pos_float(SDL_Rect pos, SDL_Texture* texture, int flag)/**Essa função serve para quando precise de posições especificas da tela*/
    {
        SDL_RenderCopy(main_render, texture, NULL, &pos);//renderiza uma imagem
        if(flag)
            SDL_RenderPresent(main_render);//mostra na tela
    }

    void graphic(int speed)//gera os graficos dos caminhos, recebe com parâmetro o tempo de delay de cada laço
    {
        int cont = ON, dont_back = OFF, x = inicio.x, y = inicio.y, loading = 0;
        SDL_Rect pos;//para salvar poder passar os agurmentos para as funções
        Stack *pos_graphic;//pilha para controle das posições

        pos.h = TAM_Y, pos.w = TAM_X;
        ///Sera usado uma pilha para controle das posição da matriz
        ///afim de evitar a alocação de outra matriz auxiliar

        Init_Stack(&pos_graphic);
        Push_Stack(&pos_graphic, x, y);
        ///inicializa a pilha e logo em seguida
        ///adiciona a posição de entrada na pilha

        while(Void_Stack(pos_graphic))//enquanto a pilha não estiver vazia
        {
            printf("\nLoading Graphic... step %d\n", ++loading);

            dont_back = OFF;//reseta a flag a cada laço
            if(lab[x][y-1] == cont+1)//NORTE
            {
            	//sobre põe os caminhos gerando uma ligação entre eles
                pos.y = y*TAM_Y - TAM_Y/2, pos.x = x*TAM_X;
                graphic_pos_float(pos, t_grass, ON);

                pos.y = y*TAM_Y, pos.x = x*TAM_X;
                graphic_pos_float(pos, t_grass, ON);
                y--;//decrementa a posição em relação ao eixo 'y'

                Push_Stack(&pos_graphic, x, y);//adiciona na pilha a posição atual
                cont++;//incrementa o contador que é usado para procurar os valores sucessores ao da posição atual

                dont_back++;//sinaliza que foi incontrado um caminho
                SDL_Delay(speed);//delay para animação
            }
            else if(lab[x+1][y] == cont+1)//LESTE
            {
           		//idem
                pos.y = y*TAM_Y, pos.x = x*TAM_X + TAM_X/2;
                graphic_pos_float(pos, t_grass, ON);

                pos.y = y*TAM_Y, pos.x = x*TAM_X;
                graphic_pos_float(pos, t_grass, ON);
                x++;//incrementa a posição em relação ao eixo 'x'

                Push_Stack(&pos_graphic, x, y);//idem
                cont++;//idem

                dont_back++;//idem
                SDL_Delay(speed);//idemgraphic_pos(input, t_ant, ON);
            }
            else if(lab[x][y+1] == cont+1)//SUL
            {
            	//idem
                pos.y = y*TAM_Y + TAM_Y/2, pos.x = x*TAM_X;
                graphic_pos_float(pos, t_grass, ON);

                pos.y = y*TAM_Y, pos.x = x*TAM_X;
                graphic_pos_float(pos, t_grass, ON);
                y++;//incrementa a posição em relação ao eixo 'y'

                Push_Stack(&pos_graphic, x, y);//idem
                cont++;//idem

                dont_back++;//idem
                SDL_Delay(speed);//idem
            }
            else if(lab[x-1][y] == cont+1)//OESTE
            {
            	//idem
                pos.y = y*TAM_Y, pos.x = x*TAM_X - TAM_X/2;
                graphic_pos_float(pos, t_grass, ON);

                pos.y = y*TAM_Y, pos.x = x*TAM_X;
                graphic_pos_float(pos, t_grass, ON);
                x--;//decrementa em relação ao eixo 'x'

                Push_Stack(&pos_graphic, x, y);//idem
                cont++;//idem

                dont_back++;//idem
                SDL_Delay(speed);//idem
            }
            if(dont_back == OFF)//essa condição serve para o caso de estar 'preso' em uma posição do labirinto
                Pop_Stack_Double(pos_graphic, &x, &y);//decrementa da pilha de posições

        }
    }

    int colision(SDL_Rect point, int way)//Essa função serve para encontrar colisões entre as paredes do labirinto
    {
        int track, aux = lab[point.x][point.y];

        switch(way)
        {
            case NORTH:
                track = lab[point.x][point.y - 1];
                break;

            case EAST:
                track = lab[point.x + 1][point.y];
                break;

            case SOUTH:
                track = lab[point.x][point.y + 1];
                break;

            case WEST:
                track = lab[point.x - 1][point.y];
                break;
        }
		/**Verifica se o caminho para qual deseja ir, é sucessor ou antecessor da minha posição atual ou se está na lista de caminhos adjacentes, gerados pela função 'Read_track'*/
        if( aux == (track-1) || aux == (track+1) || (Find_Queue_Double(track_way, aux, track)) )
            return ON;

        return OFF;
    }

    int user_walker(SDL_Event* event, FILE* way_user)//faz o usuario controla a formiga
    {
        SDL_Rect input = inicio;
        input.w = TAM_X, input.h = TAM_Y;

        graphic_pos(input, t_ant, ON);
        int way, sucess = ON, passos = 0;

        while(SDL_PollEvent(event)!= OFF || sucess == ON)//enquanto houver um evento do usuario ou flag sucess estiver ativada
        {
            if( event->type == SDL_KEYDOWN)//verifica se uma tecla foi digitada
            {
                switch(event->key.keysym.sym)//verifica qual tecla foi digitada
                {
                    case SDLK_UP://faz a formiga subir

                        printf("\nYOU PRESS UP");
                        way = 1;
                        if(colision(input, way))
                        {
                            if(win(input)) return passos;
                            graphic_pos(input, t_grass, OFF);

                            if((input.x == inicio.x && input.y == inicio.y))
                                graphic_pos(input, t_door, ON);

                            input.y--;

                            graphic_pos(input, t_grass, OFF);
                            graphic_pos(input, t_ant, ON);

                            fprintf(way_user,"WAY POSITION: %d,%d\n", input.x, input.y);
                            passos++;
                        }
                        break;

                    case SDLK_RIGHT://faz formiga ir para a direita

                        printf("\nYOU PRESS RIGHT");
                        way = 2;
                        if(colision(input, way))
                        {
                            if(win(input)) return passos;
                            graphic_pos(input, t_grass, OFF);

                           if((input.x == inicio.x && input.y == inicio.y))
                                graphic_pos(input, t_door, ON);
                            input.x++;

                            graphic_pos(input, t_grass, OFF);
                            graphic_pos(input, t_ant, ON);

                            fprintf(way_user,"WAY POSITION: %d,%d\n", input.x, input.y);
                            passos++;
                        }
                        break;

                    case SDLK_DOWN://faz formiga ir para baixo

                        printf("\nYOU PRESS DOWN");
                        way = 3;
                        if(colision(input, way))
                        {
                            if(win(input)) return passos;
                            graphic_pos(input, t_grass, OFF);

                            if((input.x == inicio.x && input.y == inicio.y))
                                graphic_pos(input, t_door, ON);
                            input.y++;

                            graphic_pos(input, t_grass, OFF);
                            graphic_pos(input, t_ant, ON);

                            fprintf(way_user,"WAY POSITION: %d,%d\n", input.x, input.y);
                            passos++;
                        }
                        break;

                    case SDLK_LEFT://faz a formiga ir para a esquerda

                        printf("\nYOU PRESS LEFT");
                        way = 4;
                        if(colision(input, way))
                        {
                            if(win(input)) return passos;
                            graphic_pos(input, t_grass, OFF);

                            if((input.x == inicio.x && input.y == inicio.y))
                                graphic_pos(input, t_door, ON);

                            input.x--;

                            graphic_pos(input, t_grass, OFF);
                            graphic_pos(input, t_ant, ON);

                            fprintf(way_user,"WAY POSITION: %d,%d\n", input.x, input.y);
                            passos++;
                        }
                        break;

                    case SDLK_h://tecla 'h'. Opção 'I need so help'.

                        find_exit(input, way_user, OFF);//chama a função 'find_exit'
                        break;

                    case SDLK_q://tecla 'q'. Opção 'let go'.

                        printf("\nEXIT");
                        sucess = OFF;//sinaliza a flag
                        break;

                    case SDLK_w://tecla 'w'. Opção "WHAT A FUCKY".

                        printf("\nWTF...\n");
                        print_lab();//mostra a matriz do labirinto no terminal
                        break;
                    case SDLK_b://tecla 'b'. Opção "Get over here".

                        Show_Queue(track_way);
                        break;
                }
            }
            SDL_Delay(35);//delay 35 ms
        }
        return passos;
    }

    int find_exit(SDL_Rect start, FILE* way, int save)//procura a saída, a partir da posição passa pelo 1º parâmetro
    {
        int cont = lab[start.x][start.y], dont_back = OFF, loading = 0;

        SDL_Rect pos;//para salvar poder passar os agurmentos para as funções
        pos.x = start.x, pos.y = start.y;
        Stack *pos_control;//pilha para controle das posições

        pos.h = TAM_Y, pos.w = TAM_X;
        ///Sera usado uma pilha para controle das posição da matriz
        ///afim de evitar a alocação de outra matriz auxiliar

        Init_Stack(&pos_control);
        Push_Stack(&pos_control, pos.x, pos.y);
        ///inicializa a pilha e logo em seguida
        ///adiciona a posição de entrada na pilha

        while(1)
        {
            printf("\nLoading Find_exit... step %d\n", ++loading);

            dont_back = OFF;//reseta a flag a cada laço
            if(pos.x == stop.x && pos.y == stop.y)
            {
                graphic_pos(pos, t_door, ON);
                if(save == ON) fprintf(way,"WAY EXIT: %d,%d\n", pos.x, pos.y);
                return lab[pos.x][pos.y] - lab[start.x][start.y];
            }
            if(lab[pos.x][pos.y-1] == cont+1)//NORTE
            {
                if(save == ON) fprintf(way,"WAY : %d,%d\n", pos.x, pos.y);
                graphic_pos(pos, t_arrow, ON);
                pos.y--;//decrementa a posição em relação ao eixo 'y'

                Push_Stack(&pos_control, pos.x, pos.y);//adiciona na pilha a posição atual
                cont++;//incrementa o contador que é usado para procurar os valores sucessores ao da posição atual

                dont_back++;//sinaliza que foi incontrado um caminho
                SDL_Delay(5);//delay para animação
            }
            else if(lab[pos.x+1][pos.y] == cont+1)//LESTE
            {
                if(save == ON) fprintf(way,"WAY : %d,%d\n", pos.x, pos.y);
           		graphic_pos(pos, t_arrow, ON);
                pos.x++;//decrementa a posição em relação ao eixo 'y'

                Push_Stack(&pos_control, pos.x, pos.y);//adiciona na pilha a posição atual
                cont++;//incrementa o contador que é usado para procurar os valores sucessores ao da posição atual

                dont_back++;//sinaliza que foi incontrado um caminho
                SDL_Delay(5);//delay para animação
            }
            else if(lab[pos.x][pos.y+1] == cont+1)//SUL
            {
                if(save == ON) fprintf(way,"WAY : %d,%d\n", pos.x, pos.y);
            	graphic_pos(pos, t_arrow, ON);
                pos.y++;//decrementa a posição em relação ao eixo 'y'

                Push_Stack(&pos_control, pos.x, pos.y);//adiciona na pilha a posição atual
                cont++;//incrementa o contador que é usado para procurar os valores sucessores ao da posição atual

                dont_back++;//sinaliza que foi incontrado um caminho
                SDL_Delay(5);//delay para animação
            }
            else if(lab[pos.x-1][pos.y] == cont+1)//OESTE
            {
                if(save == ON) fprintf(way,"WAY : %d,%d\n", pos.x, pos.y);
            	graphic_pos(pos, t_arrow, ON);
                pos.x--;//decrementa a posição em relação ao eixo 'y'

                Push_Stack(&pos_control, pos.x, pos.y);//adiciona na pilha a posição atual
                cont++;//incrementa o contador que é usado para procurar os valores sucessores ao da posição atual

                dont_back++;//sinaliza que foi incontrado um caminho
                SDL_Delay(5);//delay para animação
            }
            if(dont_back == OFF)//essa condição serve para o caso de estar 'preso' em uma posição do labirinto
            {
                graphic_pos(pos, t_grass, ON);
                Pop_Stack_Double(pos_control, &pos.x, &pos.y);//decrementa da pilha de posições
            }
        }
    }

    void closed(void)//destroi a janela, desaloca ponteiros, fecha a SDL, entre outros...
    {
        SDL_DestroyWindow(main_window);
        main_render = NULL;

        SDL_DestroyTexture(t_arrow);
        t_arrow = NULL;

        SDL_DestroyTexture(t_grass);
        t_grass = NULL;

        SDL_DestroyTexture(t_floor);
        t_floor = NULL;

        SDL_DestroyTexture(t_ant);
        t_ant = NULL;

        SDL_DestroyTexture(t_door);
        t_door = NULL;

        SDL_DestroyTexture(t_win);
        t_win = NULL;

        SDL_DestroyTexture(t_wall);
        t_wall = NULL;

        SDL_DestroyRenderer(main_render);
        main_render = NULL;


        for(int j=0; j<TAM2; j++)
        {
            free(lab[j]);
            lab[j] = NULL;
        }

        SDL_Quit();
    }
