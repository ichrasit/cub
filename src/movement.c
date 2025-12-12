#include "../Cub3D.h"

static int	check_collision(t_game *game, double new_x, double new_y)
{
	int	map_x;
	int	map_y;

	map_x = (int)new_x;
	map_y = (int)new_y;
	if (map_x < 0 || map_x >= game->map->width || 
		map_y < 0 || map_y >= game->map->height)
		return (1);
	if (game->map->grid[map_y][map_x] == '1')
		return (1);
	return (0);
}

void	move_forward(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player->pos_x + game->player->dir_x * MOVE_SPEED;
	new_y = game->player->pos_y + game->player->dir_y * MOVE_SPEED;
	if (!check_collision(game, new_x, game->player->pos_y))
		game->player->pos_x = new_x;
	if (!check_collision(game, game->player->pos_x, new_y))
		game->player->pos_y = new_y;
}

void	move_backward(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player->pos_x - game->player->dir_x * MOVE_SPEED;
	new_y = game->player->pos_y - game->player->dir_y * MOVE_SPEED;
	if (!check_collision(game, new_x, game->player->pos_y))
		game->player->pos_x = new_x;
	if (!check_collision(game, game->player->pos_x, new_y))
		game->player->pos_y = new_y;
}

void	move_left(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player->pos_x - game->player->plane_x * MOVE_SPEED;
	new_y = game->player->pos_y - game->player->plane_y * MOVE_SPEED;
	if (!check_collision(game, new_x, game->player->pos_y))
		game->player->pos_x = new_x;
	if (!check_collision(game, game->player->pos_x, new_y))
		game->player->pos_y = new_y;
}

void	move_right(t_game *game)
{
	double	new_x;
	double	new_y;

	new_x = game->player->pos_x + game->player->plane_x * MOVE_SPEED;
	new_y = game->player->pos_y + game->player->plane_y * MOVE_SPEED;
	if (!check_collision(game, new_x, game->player->pos_y))
		game->player->pos_x = new_x;
	if (!check_collision(game, game->player->pos_x, new_y))
		game->player->pos_y = new_y;
}

void	rotate_left(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player->dir_x;
	game->player->dir_x = game->player->dir_x * cos(-ROT_SPEED) - 
		game->player->dir_y * sin(-ROT_SPEED);
	game->player->dir_y = old_dir_x * sin(-ROT_SPEED) + 
		game->player->dir_y * cos(-ROT_SPEED);
	old_plane_x = game->player->plane_x;
	game->player->plane_x = game->player->plane_x * cos(-ROT_SPEED) - 
		game->player->plane_y * sin(-ROT_SPEED);
	game->player->plane_y = old_plane_x * sin(-ROT_SPEED) + 
		game->player->plane_y * cos(-ROT_SPEED);
}

void	rotate_right(t_game *game)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = game->player->dir_x;
	game->player->dir_x = game->player->dir_x * cos(ROT_SPEED) - 
		game->player->dir_y * sin(ROT_SPEED);
	game->player->dir_y = old_dir_x * sin(ROT_SPEED) + 
		game->player->dir_y * cos(ROT_SPEED);
	old_plane_x = game->player->plane_x;
	game->player->plane_x = game->player->plane_x * cos(ROT_SPEED) - 
		game->player->plane_y * sin(ROT_SPEED);
	game->player->plane_y = old_plane_x * sin(ROT_SPEED) + 
		game->player->plane_y * cos(ROT_SPEED);
}

void	process_movement(t_game *game)
{
	if (game->keys->w)
		move_forward(game);
	if (game->keys->s)
		move_backward(game);
	if (game->keys->a)
		move_left(game);
	if (game->keys->d)
		move_right(game);
	if (game->keys->left)
		rotate_left(game);
	if (game->keys->right)
		rotate_right(game);
}
