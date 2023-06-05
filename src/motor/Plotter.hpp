#pragma once
#include "../objetos/Figuras.hpp"
#include "../utilidad/Estructuras.hpp"
#include <SDL2/SDL.h>

class Plotter {
private:
  Plotter();
  // singleton
  static Plotter *instancia;

public:
  static Plotter &get();
  static void render_pixel(SDL_Renderer *r, Coordenadas p, SDL_Color color);
  static void render_linea(SDL_Renderer *r, Coordenadas p1, Coordenadas p2,
                           SDL_Color color);
  static void rellenoscandline(SDL_Renderer *r, Figura *fig);
  static void pintar_textura(SDL_Renderer *r, SDL_Texture *textura,
                             SDL_Rect &src, SDL_Rect &dest, Coordenadas size);
};