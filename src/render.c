#include "../Cub3D.h"

void	mlx_put_pixel(t_img *img, int x, int y, int color)
{
	char	*dst;

	if (x < 0 || x >= img->width || y < 0 || y >= img->height)
		return ;
	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	*(unsigned int *)dst = color;
}

int	get_pixel_color(t_img *img, int x, int y)
{
	char	*dst;

	if (x < 0 || x >= img->width || y < 0 || y >= img->height)
		return (0);
	dst = img->addr + (y * img->line_length + x * (img->bits_per_pixel / 8));
	return (*(unsigned int *)dst);
}

static void	draw_ceiling_floor(t_game *game)
{
	int	x;
	int	y;

	y = 0;
	while (y < WINDOW_HEIGHT / 2)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			mlx_put_pixel(game->screen, x, y, game->map->ceiling_color);
			x++;
		}
		y++;
	}
	while (y < WINDOW_HEIGHT)
	{
		x = 0;
		while (x < WINDOW_WIDTH)
		{
			mlx_put_pixel(game->screen, x, y, game->map->floor_color);
			x++;
		}
		y++;
	}
}

static void	cast_ray(t_game *game, int x)
{
	t_ray	*ray;

	ray = game->ray;
	ray->camera_x = 2 * x / (double)WINDOW_WIDTH - 1;
	ray->ray_dir_x = game->player->dir_x + game->player->plane_x * ray->camera_x;
	ray->ray_dir_y = game->player->dir_y + game->player->plane_y * ray->camera_x;
	ray->map_x = (int)game->player->pos_x;
	ray->map_y = (int)game->player->pos_y;
	ray->delta_dist_x = fabs(1 / ray->ray_dir_x);
	ray->delta_dist_y = fabs(1 / ray->ray_dir_y);
	if (ray->ray_dir_x < 0)
	{
		ray->step_x = -1;
		ray->side_dist_x = (game->player->pos_x - ray->map_x) * ray->delta_dist_x;
	}
	else
	{
		ray->step_x = 1;
		ray->side_dist_x = (ray->map_x + 1.0 - game->player->pos_x) * ray->delta_dist_x;
	}
	if (ray->ray_dir_y < 0)
	{
		ray->step_y = -1;
		ray->side_dist_y = (game->player->pos_y - ray->map_y) * ray->delta_dist_y;
	}
	else
	{
		ray->step_y = 1;
		ray->side_dist_y = (ray->map_y + 1.0 - game->player->pos_y) * ray->delta_dist_y;
	}
}

static void	perform_dda(t_game *game)
{
	t_ray	*ray;

	ray = game->ray;
	ray->hit = 0;
	while (ray->hit == 0)
	{
		if (ray->side_dist_x < ray->side_dist_y)
		{
			ray->side_dist_x += ray->delta_dist_x;
			ray->map_x += ray->step_x;
			ray->side = 0;
		}
		else
		{
			ray->side_dist_y += ray->delta_dist_y;
			ray->map_y += ray->step_y;
			ray->side = 1;
		}
		if (ray->map_x < 0 || ray->map_x >= game->map->width ||
			ray->map_y < 0 || ray->map_y >= game->map->height ||
			game->map->grid[ray->map_y][ray->map_x] == '1')
			ray->hit = 1;
	}
}

static t_img	*get_wall_texture(t_game *game)
{
	t_ray	*ray;

	ray = game->ray;
	if (ray->side == 0)
	{
		if (ray->step_x > 0)
			return (game->texture->east);
		else
			return (game->texture->west);
	}
	else
	{
		if (ray->step_y > 0)
			return (game->texture->south);
		else
			return (game->texture->north);
	}
}

static void	calculate_wall_distance(t_game *game)
{
	t_ray	*ray;

	ray = game->ray;
	if (ray->side == 0)
		ray->perp_wall_dist = (ray->map_x - game->player->pos_x + 
			(1 - ray->step_x) / 2) / ray->ray_dir_x;
	else
		ray->perp_wall_dist = (ray->map_y - game->player->pos_y + 
			(1 - ray->step_y) / 2) / ray->ray_dir_y;
	ray->line_height = (int)(WINDOW_HEIGHT / ray->perp_wall_dist);
	ray->draw_start = -ray->line_height / 2 + WINDOW_HEIGHT / 2;
	if (ray->draw_start < 0)
		ray->draw_start = 0;
	ray->draw_end = ray->line_height / 2 + WINDOW_HEIGHT / 2;
	if (ray->draw_end >= WINDOW_HEIGHT)
		ray->draw_end = WINDOW_HEIGHT - 1;
}

static void	draw_textured_wall(t_game *game, int x)
{
	t_img	*texture;
	int		y;
	int		color;

	calculate_wall_distance(game);
	texture = get_wall_texture(game);
	if (game->ray->side == 0)
		game->ray->wall_x = game->player->pos_y + game->ray->perp_wall_dist * game->ray->ray_dir_y;
	else
		game->ray->wall_x = game->player->pos_x + game->ray->perp_wall_dist * game->ray->ray_dir_x;
	game->ray->wall_x -= floor(game->ray->wall_x);
	game->ray->tex_x = (int)(game->ray->wall_x * (double)texture->width);
	if (game->ray->side == 0 && game->ray->ray_dir_x > 0)
		game->ray->tex_x = texture->width - game->ray->tex_x - 1;
	if (game->ray->side == 1 && game->ray->ray_dir_y < 0)
		game->ray->tex_x = texture->width - game->ray->tex_x - 1;
	game->ray->step = 1.0 * texture->height / game->ray->line_height;
	game->ray->tex_pos = (game->ray->draw_start - WINDOW_HEIGHT / 2 + 
		game->ray->line_height / 2) * game->ray->step;
	y = game->ray->draw_start;
	while (y < game->ray->draw_end)
	{
		game->ray->tex_y = (int)game->ray->tex_pos & (texture->height - 1);
		game->ray->tex_pos += game->ray->step;
		color = get_pixel_color(texture, game->ray->tex_x, game->ray->tex_y);
		if (game->ray->side == 1)
			color = (color >> 1) & 8355711;
		mlx_put_pixel(game->screen, x, y, color);
		y++;
	}
}

static void	raycasting(t_game *game)
{
	int	x;

	x = 0;
	while (x < WINDOW_WIDTH)
	{
		cast_ray(game, x);
		perform_dda(game);
		draw_textured_wall(game, x);
		x++;
	}
}

int	render_frame(t_game *game)
{
	draw_ceiling_floor(game);
	raycasting(game);
	mlx_put_image_to_window(game->mlx, game->win, game->screen->img, 0, 0);
	return (0);
}

int	loop_hook(t_game *game)
{
	if (!game->running)
		return (0);
	process_movement(game);
	render_frame(game);
	return (0);
}
