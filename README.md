*This project has been created as part of the 42 curriculum by muhaoz and htekdemi*

## Description

**Cub3D** is a 42 School project that implements a 3D maze game engine using raycasting techniques. The project creates a first-person perspective game where players navigate through a textured maze, similar to classic games like Wolfenstein 3D.

### Goal

The main objectives of this project are:
- Implement raycasting algorithms to render 3D graphics from 2D maps
- Parse and validate .cub configuration files for game maps and textures
- Load and display XPM format textures
- Handle real-time player input and movement
- Implement collision detection with maze walls
- Create an interactive graphics window using MiniLibX

### Technical Overview

Cub3D reads a .cub file containing:
- Texture paths (North, South, East, West walls)
- Floor and ceiling colors (RGB format)
- 2D maze map with player spawn position and direction

The engine uses the DDA (Digital Differential Analyzer) algorithm for raycasting and renders the 3D view in real-time.

## Instructions

### Requirements

- Linux environment (tested on Linux)
- GCC compiler
- MiniLibX library (included in project)
- Make

### Compilation

```bash
make        # Compile the project
make clean  # Remove object files
make fclean # Remove object files and executable
make re     # Clean and recompile
```

### Execution

```bash
./cub3D <path_to_map.cub>
```

Example:
```bash
./cub3D maps/basic.cub
```

### Controls

- **W** - Move forward
- **A** - Strafe left
- **S** - Move backward
- **D** - Strafe right
- **← →** - Rotate view left/right
- **ESC** - Exit program

### Map File Format

A valid .cub file must contain:

```
NO <path_to_north_texture.xpm>
SO <path_to_south_texture.xpm>
WE <path_to_west_texture.xpm>
EA <path_to_east_texture.xpm>
F <red>,<green>,<blue>
C <red>,<green>,<blue>

1111111111
1000000001
100N000001
1111111111
```

**Rules:**
- Each texture and color must be defined exactly once
- RGB values must be 0-255
- Map must be surrounded by walls (1s)
- Player spawn marked with N, S, E, or W (direction)
- Only one player allowed
- Map must be at least 3x3 in size
- No trailing commas or extra characters

## Resources

### Raycasting Algorithm
- [Raycasting Basics](https://en.wikipedia.org/wiki/Ray_casting)
- [DDA Algorithm](https://en.wikipedia.org/wiki/Digital_differential_analyzer)
- [Wolfenstein 3D Technique](https://www.youtube.com/results?search_query=wolfenstein+raycasting)

### Graphics & Game Development
- [MiniLibX Documentation](https://harm-smits.github.io/42docs/libs/minilibx)
- XPM Image Format (included in project)
- Real-time rendering optimization techniques

### Documentation & References
- C file I/O operations
- Configuration file format standards
- Input validation techniques

## Project Structure

```
cub3D/
├── src/
│   ├── config_parser.c           # Main configuration parsing
│   ├── config_parser_utils.c     # Path and RGB parsing utilities
│   ├── config_parser_check.c     # Validation and duplicate checking
│   ├── map_parser.c              # Map file parsing
│   ├── map_validation.c          # Map validation logic
│   ├── map_check.c               # Map wall and boundary checking
│   ├── map_contiguous.c          # Contiguous map block verification
│   ├── map_grid.c                # Map grid allocation
│   ├── map_dim.c                 # Map dimension calculation
│   ├── flood_fill.c              # Flood fill validation
│   ├── raycast.c                 # Raycasting algorithm
│   ├── render.c                  # Frame rendering
│   ├── hooks.c                   # Input event handling
│   ├── movement.c                # Player movement logic
│   ├── init_game.c               # Game initialization
│   ├── texture_loader.c          # Texture loading
│   ├── cleanup.c                 # Resource deallocation
│   ├── get_next_line.c           # Line reading utility
│   └── main.c                    # Entry point
├── libft/                        # 42 libft library
├── minilibx-linux/               # MiniLibX graphics library
├── maps/                         # Example map files
├── assets/                       # Game textures
├── Cub3D.h                       # Main header file
├── Makefile                      # Build configuration
└── README.md                     # This file
```

## Notes

- The project strictly follows the 42 School norm (norminette)
- All mandatory functions pass strict parsing validation
- Memory management is carefully implemented to prevent leaks
- The raycasting engine provides smooth real-time rendering
- Textures must be valid XPM format files

## License

This project is part of the 42 School curriculum.
