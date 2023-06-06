#include "ObjetoDinamico.hpp"
#include "../motor/camaras/ManejadorCamara.hpp"
#include "../motor/fisica/MotorFisico.hpp"
#include "../objetos/Figuras.hpp"
#include "../utilidad/Func_aux.hpp"
#include <iostream>
#include <math.h>

Jugador::Jugador(float vida, int x, int y, SDL_Color c) : ObjetoDinamico{} {
  hp = vida;
  posicion_mundo.x = x;
  posicion_mundo.y = y;
  avatar = new Rectangulo(x, y, 75, 75, c);
  c.a = 150;
  color = c;
  avatar->set_rellenocolor(c);
  avatar->set_serellena(false);
  col_box = new Rectangulo(x, y, 75 + 10, 75 + 10, c);
  col_box->set_serellena(false);
  tiene_fisica = true;
  en_colision = false;
  estado_actual = new EstadoJugadorIDLE();
  piso = {x, y + 100}; // definir el piso en general
  sprite = nullptr;
  tile = nullptr;
  velocidad = 0;
  objetos = nullptr;
  area_plataforma = nullptr;
};

Jugador::Jugador(std::string path_sprite, float vida, int x, int y, int w,
                 int h, int sw, int sh, SDL_Color c)
    : ObjetoDinamico{} {
  hp = vida;
  posicion_mundo.x = x;
  posicion_mundo.y = y;
  posicion_camara = {0, 0};
  avatar = new Rectangulo(x, y, sw, sh, c);
  c.a = 150;
  color = c;
  avatar->set_rellenocolor(c);
  // avatar->set_serellena(false);
  col_box = new Rectangulo(x, y - 5, sw - 25, sh - 10, c);
  // col_box = new Rectangulo(x, y, sw, sh, c);

  col_box->set_serellena(true);
  tiene_fisica = true;
  en_colision = false;
  estado_actual = new EstadoJugadorIDLE();

  c.a = 255;
  col_piso =
      new Rectangulo(x + 100, y + sh, sw / 2 + 10, 10, {150, 200, 0, 255});

  piso = {x, y + sw}; // definir el piso en general
  sprite = new Sprite(path_sprite, posicion_mundo, w, h, sw, sh);
  tile = nullptr;
  objetos = nullptr;
  velocidad = 2;
  dtgravedad = 0;
  area_plataforma = col_piso;
  centro = {posicion_camara.x + sprite->get_sprite_sizes().x / 2,
            posicion_camara.y + sprite->get_sprite_sizes().y / 2};
};

std::string Jugador::get_strEstado() { return estado_actual->get_namestate(); };

void *Jugador::get_estado() { return estado_actual; };

void Jugador::set_estado(void *estado) {
  estado_actual->salir(*this);
  delete estado_actual;
  estado_actual = (FSMJugador *)estado;
  estado_actual->entrar(*this);
};

void Jugador::set_onPlataforma(bool v) { en_plataforma = v; };

void Jugador::update(double dt) {

  if (!estado_actual) // nulo
    return;

  centro = {posicion_camara.x + sprite->get_sprite_sizes().x / 2,
            posicion_camara.y + sprite->get_sprite_sizes().y / 2};
  if (area_plataforma) {
    area_plataforma->set_position(posicion_camara.x + 20,
                                  posicion_camara.y +
                                      sprite->get_sprite_sizes().y - 10);
    area_plataforma->update_vertices();
  }

  estado_actual->update(*this, dt);

  // reset

  if (en_plataforma) {
    set_dtgf(0);
  }
  set_onPlataforma(false);
};

void Jugador::input_handle(KeyOyente &input, MouseOyente &mouse) {
  if (!estado_actual) // nulo
    return;
  FSMJugador *estado = estado_actual->input_handle(input, mouse);
  if (estado) {
    set_estado(estado);
  }
};

/*
---
*/

BackGroundDinamico::BackGroundDinamico(std::string path_sprite, int w, int h) {
  posicion_mundo.x = -w / 2;
  posicion_mundo.y = 0;
  posicion_camara = posicion_mundo;
  avatar = nullptr;
  tile = nullptr;
  col_box = nullptr;
  // toda la imagen sprite?
  limites.x = w;
  limites.y = 0;
  sprite = new Sprite(path_sprite, posicion_mundo, w, h, w, h);
  tiene_fisica = false;
  en_colision = false;
  velocidad = 0;

  area_plataforma = nullptr;
  // crear la camara antes del background
  centro_cam = ManejadorCamaras::get_camara().get_posicion_centro();
};
void BackGroundDinamico::set_estado(void *estado){};
void *BackGroundDinamico::get_estado() { return NULL; }
void BackGroundDinamico::update(double dt) {
  // parallax/scroller reset
  float dist = std::sqrt(std::pow(posicion_mundo.x - limites.x / 2, 2) +
                         std::pow(posicion_mundo.y - limites.y, 2));
  // DEBUGPRINT(dist)
  if (dist < (limites.x / 2) + centro_cam.x - velocidad) // izquierda
  {
    // reset
    posicion_mundo.x = -(limites.x / 2) - centro_cam.x - velocidad;
  } else if (dist > limites.x + centro_cam.x + velocidad) // derecha
  {
    // reset
    posicion_mundo.x = -(limites.x / 2) + centro_cam.x + velocidad;
  }
};

/*
Plataformas Dinámicas
*/

PlataformasDinamicas::PlataformasDinamicas(std::string sprite_path, int x,
                                           int y, int w, int h, int sw, int sh,
                                           SDL_Color c)
    : ObjetoDinamico{} {
  posicion_mundo.x = x;
  posicion_mundo.y = y;
  posicion_camara = {0, 0};
  avatar = nullptr;
  tile = nullptr;
  // usar para la colision
  col_box = new Rectangulo(x, y, sw, sh, c);
  // col_box = new Rectangulo(x - 10, y, sw + 20, 5, {0, 0, 255, 255});
  // col_box = new Rectangulo(x - 10, y, sw + 20, 5, {0, 0, 255, 255});
  // col_box = nullptr;
  col_box->set_serellena(false);
  render_colbox = false;

  // usar para el area de piso de la plataforma
  area_plataforma = new Rectangulo(x - 10, y, sw + 20, 5, {0, 0, 255, 255});

  // area_plataforma = new Rectangulo(x, y, sw, sh, {0, 0, 255, 255});

  // toda la imagen sprite?
  sprite = new Sprite(sprite_path, posicion_mundo, w, h, sw, sh);
  tiene_fisica = false;
  en_colision = false;
  velocidad = 0;
};

void PlataformasDinamicas::update(double dt) {
  if (!area_plataforma)
    return;
  area_plataforma->set_position(posicion_camara.x - 10, posicion_camara.y);
  area_plataforma->update_vertices();
  // DEBUGCOOR(area_plataforma->get_posicion());
};
void PlataformasDinamicas::set_estado(void *estado){

};
void *PlataformasDinamicas::get_estado() { return NULL; };