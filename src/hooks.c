#include "../Cub3D.h"


int	k_down(int k, t_game *g)
{
	if (k == K_ESC)
		end_game(g);
	else if (k == K_W)
		g->key->w = 1;
	else if (k == K_S)
		g->key->s = 1;
	else if (k == K_A)
		g->key->a = 1;
	else if (k == K_D)
		g->key->d = 1;
	else if (k == K_LF)
		g->key->l = 1;
	else if (k == K_RT)
		g->key->r = 1;
	return (0);
}

int	k_up(int k, t_game *g)
{
	if (k == K_W)
		g->key->w = 0;
	else if (k == K_S)
		g->key->s = 0;
	else if (k == K_A)
		g->key->a = 0;
	else if (k == K_D)
		g->key->d = 0;
	else if (k == K_LF)
		g->key->l = 0;
	else if (k == K_RT)
		g->key->r = 0;
	return (0);
}
