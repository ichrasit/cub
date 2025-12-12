#include "../Cub3D.h"

int	key_press(int keycode, t_game *game)
{
	if (keycode == KEY_ESC)
		close_game(game);
	else if (keycode == KEY_W)
		game->keys->w = 1;
	else if (keycode == KEY_S)
		game->keys->s = 1;
	else if (keycode == KEY_A)
		game->keys->a = 1;
	else if (keycode == KEY_D)
		game->keys->d = 1;
	else if (keycode == KEY_LEFT)
		game->keys->left = 1;
	else if (keycode == KEY_RIGHT)
		game->keys->right = 1;
	return (0);
}

int	key_release(int keycode, t_game *game)
{
	if (keycode == KEY_W)
		game->keys->w = 0;
	else if (keycode == KEY_S)
		game->keys->s = 0;
	else if (keycode == KEY_A)
		game->keys->a = 0;
	else if (keycode == KEY_D)
		game->keys->d = 0;
	else if (keycode == KEY_LEFT)
		game->keys->left = 0;
	else if (keycode == KEY_RIGHT)
		game->keys->right = 0;
	return (0);
}

int	close_game(t_game *game)
{
	game->running = 0;
	free_game(game);
	exit(0);
	return (0);
}
