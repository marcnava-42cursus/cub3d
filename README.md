*This project has been created as part of the 42 curriculum by marcnava, ivmirand.*

# Cub3D

## Description
Cub3D is a small raycasting engine inspired by the classic Wolfenstein 3D style.
It parses a `.cub` configuration file that defines textures, floor/ceiling colors,
and a 2D map layout, then renders a pseudo-3D view with basic movement and
collision using MLX42.

The project goal is to implement a full rendering loop, robust map validation,
and texture mapping while keeping the codebase in C and compatible with 42
requirements.

## Instructions

### Requirements
- C compiler (gcc/clang)
- make, cmake, git
- GLFW development libraries and OpenGL/X11 (Linux) or the system equivalents

### Build
```bash
make
```
The first build will clone and compile the dependencies into `libs/`.

### Run
```bash
./cub3D maps/example.cub
```

### Bonus build
```bash
make bonus
./cub3D_bonus maps/example.cub
```

### Useful targets and options
```bash
make clean        # remove objects
make fclean       # remove objects and binaries
make re           # full rebuild
make reload       # build and run example map
make reload_bonus # build and run bonus example
```
```bash
make DEBUG=1      # debug symbols
make DEBUG=2      # debug symbols + address sanitizer
make VSYNC=off    # disable vsync (or VSYNC=0)
```

### Controls
- W/A/S/D: move
- Left/Right arrows: rotate
- Mouse: rotate view
- ESC: quit

### Map format (.cub)
A `.cub` file typically includes texture paths (NO/SO/WE/EA), floor/ceiling RGB
colors (F/C), and the map grid. See `maps/example.cub` for a reference.

## Resources
- 42 Cub3D subject (project specification)
- Lode's Computer Graphics Tutorial: Raycasting
- Permadi's Ray-Casting Tutorial
- MLX42 documentation (Codam)
- GLFW documentation

AI usage:
- OpenAI Codex was used to draft and structure this README (documentation only).
