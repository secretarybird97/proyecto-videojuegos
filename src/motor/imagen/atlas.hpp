#pragma once
#include "../../objetos_estaticos/ObjetoEstatico.hpp"
#include "../../utilidad/Estructuras.hpp"
#include <SDL2/SDL.h>
#include <string>

class Atlas {
public:
  Atlas(std::string atlas, Coordenadas offset = {0, 0});
  ~Atlas();
  bool cargar_textura(SDL_Renderer *r);
  void generar_mapa(SDL_Renderer *r, int idflip, int notidobjfisicos,
                    int scale);
  std::vector<Objeto *> get_objetos_fisicos() const { return objetos_fisicos; };

private:
  AtlasInfo info;
  Coordenadas pos;
  std::vector<Objeto *> objetos_fisicos;
  SDL_Texture *atlas_sheet;
};