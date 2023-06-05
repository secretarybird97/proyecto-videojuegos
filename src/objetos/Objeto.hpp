#pragma once
#include "../motor/imagen/sprites.hpp"
#include "../motor/imagen/tiles.hpp"
#include "../utilidad/Estructuras.hpp"
#include "Figuras.hpp"
#include <SDL2/SDL.h>

class Objeto {
protected:
  Figura *avatar;
  Sprite *sprite;
  Tile *tile;
  Figura *col_box;
  float hp;
  Coordenadas posicion_mundo;
  Coordenadas posicion_camara;
  bool tiene_fisica;
  SDL_Color color;
  int velocidad = 1;

public:
  bool en_colision;
  bool render_colbox;

public:
  virtual ~Objeto(){};
  virtual void update(double dt) = 0;
  void render(SDL_Renderer *r);
  Figura *get_avatar() const { return avatar; };
  Figura *get_colbox() const { return col_box; };
  float get_hp() const { return hp; };
  void set_hp(float v) { hp = v; };
  Coordenadas get_posicion_mundo() const { return posicion_mundo; };
  Coordenadas get_posicion_camara() const { return posicion_camara; };
  int get_posx() const { return posicion_mundo.x; };
  int get_posy() const { return posicion_mundo.y; };
  int get_velocidad() const { return velocidad; };
  void set_posicion_mundo(Coordenadas p);
  void set_posicion_camara(Coordenadas p);
  void set_posx(int x);
  void set_posy(int y);
  void set_velocidad(int v) { velocidad = v; };
  SDL_Color get_color() const { return color; };
  void set_color(SDL_Color c) { color = c; };
  bool get_tiene_fisica() const { return tiene_fisica; };

  Sprite *get_sprite() const { return sprite; };
  Tile *get_tile() const { return tile; };
};