# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: cub3d <cub3d@student.42.fr>                +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/19 00:00:00 by cub3d             #+#    #+#              #
#    Updated: 2025/11/19 00:00:00 by cub3d            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= cub3D

CC			= gcc
CFLAGS		= -Wall -Wextra -Werror -g
RM			= rm -f

# Directories
SRC_DIR		= src
OBJ_DIR		= obj
LIBFT_DIR	= libft
MLX_DIR		= minilibx-linux

# Libraries
LIBFT		= $(LIBFT_DIR)/libft.a
MLX			= $(MLX_DIR)/libmlx.a
MLX_FLAGS	= -L$(MLX_DIR) -lmlx -L/usr/lib -lXext -lX11 -lm -lz

# Source files
SRC_FILES	= main.c \
			  map_parser.c \
			  map_validation.c \
			  map_check.c \
			  flood_fill.c \
			  get_next_line.c \
			  init_game.c \
			  hooks.c \
			  render.c \
			  texture_loader.c \
			  config_parser.c \
			  movement.c

SRC			= $(addprefix $(SRC_DIR)/, $(SRC_FILES))
OBJ			= $(SRC:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

# Colors
GREEN		= \033[0;32m
RED			= \033[0;31m
YELLOW		= \033[0;33m
BLUE		= \033[0;34m
RESET		= \033[0m

# Rules
all: $(LIBFT) $(MLX) $(NAME)

$(NAME): $(OBJ)
	@echo "$(BLUE)Linking $(NAME)...$(RESET)"
	@$(CC) $(CFLAGS) $(OBJ) $(LIBFT) $(MLX_FLAGS) -o $(NAME)
	@echo "$(GREEN)✓ $(NAME) created successfully!$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	@echo "$(YELLOW)Compiling $<...$(RESET)"
	@$(CC) $(CFLAGS) -I$(LIBFT_DIR) -I$(MLX_DIR) -c $< -o $@

$(LIBFT):
	@echo "$(BLUE)Building libft...$(RESET)"
	@make -C $(LIBFT_DIR)
	@echo "$(GREEN)✓ libft compiled!$(RESET)"

$(MLX):
	@echo "$(BLUE)Building minilibx...$(RESET)"
	@make -C $(MLX_DIR)
	@echo "$(GREEN)✓ minilibx compiled!$(RESET)"

clean:
	@echo "$(RED)Cleaning object files...$(RESET)"
	@$(RM) -r $(OBJ_DIR)
	@make -C $(LIBFT_DIR) clean
	@make -C $(MLX_DIR) clean
	@echo "$(GREEN)✓ Clean completed!$(RESET)"

fclean: clean
	@echo "$(RED)Removing $(NAME)...$(RESET)"
	@$(RM) $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@echo "$(GREEN)✓ Full clean completed!$(RESET)"

re: fclean all

.PHONY: all clean fclean re
