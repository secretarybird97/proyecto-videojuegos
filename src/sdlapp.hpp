#pragma once
#include "motor/Pipeline.hpp"
#include "motor/camaras/Camaras.hpp"
#include "motor/imagen/atlas.hpp"
#include "objetos_dinamicos/ObjetoDinamico.hpp"
#include "objetos_estaticos/ObjetoEstatico.hpp"
#include "utilidad/sdl_aux.hpp"
#include <SDL2/SDL.h>
#include <string>
#include <vector>

class SDLApp {
private:
  SDLApp();
  bool on_init();
  void on_evento(SDL_Event *evento);
  void on_fisicaupdate(double dt);
  void on_frameupdate(double dt);
  void on_limpiar();
  void init_plataformas();

  bool esta_corriendo;
  int WIDTH;
  int HEIGHT;

  SDL_Window *vnt;
  SDL_Surface *vntsurf;
  SDL_Renderer *render;

  // singleton
  static SDLApp *instancia;

  // 02_lineas_figuras
  Pipeline *ensamble;

  // a03_Maquinas_estados.
  double maxFPS;
  double msfrecuencia;
  int fps;

  // 05_Colisiones
  Jugador *player;
  std::vector<ObjetoEstatico *> plataformas;

  // 06_Camaras
  Camara *camara_principal;
  std::vector<Objeto *> objetos;

  // 07 Background
  std::vector<Background *> backgrounds;

  // 08 tiles
  std::vector<Atlas *> mapa;

public:
  SDL_Color bg_color;
  static int on_correr();
  static void set_bgcolor(Uint8 rojo, Uint8 verde, Uint8 azul);
  static SDLApp &get();
  static bool estaCorriendo() { return get().esta_corriendo; };
  friend class SDLApp_AUX; // Clase auxiliar
  static const int BG_WIDTH = 1080;
  static const int BG_HEIGHT = 17280;
  static const int SCALE = 3;
};