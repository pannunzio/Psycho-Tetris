#include <ncurses.h>
#include <time.h>
#define SERV_ENGINE
#include "engine.h"
#define SERV_TELA
#include "tela.h"
#define SERV_PECA
#include "peca.h"
#include "game.h"

#define ENTER 10
#define ESC 27

int endgame(Tela* t)
{
	int i;
	int res = 1;

	for(i=0;i<TELA_LARGURA;++i)
	{
		if(t->bloco[0][i].tipo)
			res*=0;
	}

	return !res;
}

EXT_MOD_GAME void start_game(int difficulty)
{
	Tela *tela;
	Peca *peca;
	int end=0, in;
	WINDOW* main;

	srand(time(NULL));
	tela = nova_tela();

	while(!end)
	{
		clock_t ini;
		peca = nova_peca(tela, TELA_LARGURA/2 - 1, 0);

		if(peca_touching(peca, tela, 's'))
		{
			deleta_peca(peca);
			break;
		}
		
		ini = clock();
		while(!end)
		{
			clock_t now;

			mostra_tela(tela);
			mostra_peca(peca);
			refresh();

			in = pega_input(1);
			switch(in)
			{
				case 'S':
				case 's':
					if(!peca_touching(peca, tela, 's'))
						peca_move_y(peca, 1);		
				break;

				case 'A':
				case 'a':
					if(!peca_touching(peca, tela, 'a'))
						peca_move_x(peca, -1);
				break;

				case 'D':
				case 'd':
					if (!peca_touching(peca, tela, 'd'))
						peca_move_x(peca, 1);
				break;

				case ENTER:
					prende_peca(peca, tela);
				break;
				
				case 'q': 
				case 'Q':
					end = 1;
				break;

				case ESC:
					set_color(COR_TELA);
					mvaddstr(tela->y + TELA_ALTURA/2 -1, tela->x + TELA_LARGURA/2 + 2, "PAUSED");
					do {in = getch();}
					while(in != ESC && in!='q' && in!='Q');

					if(in == 'q' || in == 'Q')
					{
						end = 1;
						deleta_peca(peca);
					}
				break;		
			}
			clear();
			if(end) break;
			now = clock();
			if(now - ini > 5)
			{
				peca_move_y(peca, 1);
				ini = clock();
			}
			if(peca_touching(peca, tela, 's'))
			{
				prende_peca(peca, tela);
				deleta_peca(peca);
				break;
			}
		}

		if(endgame(tela))
		{
			end = 1;
			deleta_tela(tela);
		}
	}

}

EXT_MOD_GAME void game_over(char* gameover)
{
	set_color(COR_TELA);
	mvaddstr(0,0,gameover);
	refresh();
	getchar();
}
