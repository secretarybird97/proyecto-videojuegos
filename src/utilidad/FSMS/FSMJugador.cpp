#include "FSMJugador.hpp"
#include "../../motor/Plotter.hpp"
#include "../../motor/camaras/ManejadorCamara.hpp"
#include "../../motor/fisica/MotorFisico.hpp"
#include "../../sdlapp_aux.hpp"
#include "../Func_aux.hpp"
#include <SDL.h>
#include <cmath>
#include <iostream>
#include <math.h>
#include <sstream>

/*
IDLE
*/
EstadoJugadorIDLE::EstadoJugadorIDLE() {
  strnombre = "IDLE";
  frames_actual_ani = 0;
  frames_maxim_ani = 10;
  dir = 1;
};

FSMJugador *EstadoJugadorIDLE::input_handle(KeyOyente &input,
                                            MouseOyente &mouse) {
/*if(input.estaPresionado(SDL_SCANCODE_D))
    return new EstadoJugadorDerecha();
if(input.estaPresionado(SDL_SCANCODE_A))
    return new EstadoJugadorIzquierda();*/
#ifdef BLINEAL
  if (input.estaPresionado(SDL_SCANCODE_SPACE))
    return new EstadoJugadorInitBrinco(150);
#elif defined(GRAVEDAD)
  if (input.estaPresionado(SDL_SCANCODE_SPACE))
    return new EstadoJugadorGravedadBrinco(-10);
#elif defined(CLERP)
  if (input.estaPresionado(SDL_SCANCODE_SPACE)) {
    return new EstadoJugadorLerpBrinco(250, dir);
  }
#endif

  return NULL;
};
void EstadoJugadorIDLE::entrar(Jugador &player) {
  frames_actual_ani = 0;
  frames_maxim_ani = 10;
  // DEBUGCOOR(player.get_piso());
  dir = (player.get_sprite()->get_flip()) ? -1 : 1;
  // printf("DIR: %d\n",dir);
};
void EstadoJugadorIDLE::salir(Jugador &player){

};
void EstadoJugadorIDLE::update(Jugador &player, double dt) {
  player.get_sprite()->play_frame(0, frames_actual_ani % frames_maxim_ani);
  if (frame_dt > 7) {
    frame_dt = 0;
    frames_actual_ani++;
  }
  frame_dt++;
};

/*
MOVER
*/
EstadoJugadorIzquierda::EstadoJugadorIzquierda() {
  frames_actual_ani = 0;
  frames_maxim_ani = 10; ////////////10
  strnombre = "IZQUIERDA";
};
void EstadoJugadorIzquierda::entrar(Jugador &player) {
  frames_actual_ani = 0;
  frames_maxim_ani = 10; ////////////10
  player.get_sprite()->set_flip(true);
};

void EstadoJugadorIzquierda::salir(Jugador &player){
    // player.get_sprite()->set_flip(false);
};

FSMJugador *EstadoJugadorIzquierda::input_handle(KeyOyente &input,
                                                 MouseOyente &mouse) {
  if (input.estaPresionado(SDL_SCANCODE_D))
    return new EstadoJugadorDerecha();
#ifdef BLINEAL
  if (input.estaPresionado(SDL_SCANCODE_SPACE))
    return new EstadoJugadorInitBrinco(150);
#elif defined(GRAVEDAD)
  if (input.estaPresionado(SDL_SCANCODE_SPACE))
    return new EstadoJugadorGravedadBrinco(-10);
#elif defined(CLERP)
  if (input.estaPresionado(SDL_SCANCODE_SPACE))
    return new EstadoJugadorLerpBrinco(250, -1);

#endif

  if (!(input.estaPresionado(SDL_SCANCODE_A)) &&
      !(input.estaPresionado(SDL_SCANCODE_D))) {
    return new EstadoJugadorIDLE();
  }

  return NULL;
};

void EstadoJugadorIzquierda::update(Jugador &player, double dt) {
  /*Coordenadas p = player.get_posicion_mundo();
  p.x +=(player.get_velocidad()*-1);
  player.set_posicion_mundo(p);
  printf("[izquierda] (%d,%d)\n",p.x,p.y);*/
  player.get_sprite()->play_frame(1, frames_actual_ani % frames_maxim_ani);

  if (frame_dt > 3) {
    frame_dt = 0;
    frames_actual_ani++;
  }
  frame_dt++;
};

//--
EstadoJugadorDerecha::EstadoJugadorDerecha() {
  frames_actual_ani = 0;
  frames_maxim_ani = 10;
  strnombre = "DERECHA";
};
void EstadoJugadorDerecha::entrar(Jugador &player) {
  frames_actual_ani = 0;
  frames_maxim_ani = 10;
  player.get_sprite()->set_flip(false);
  // ManejadorCamaras::unluck_objeto();
};

void EstadoJugadorDerecha::salir(Jugador &player){
    // player.get_sprite()->set_flip(false);
    // ManejadorCamaras::lock_objeto(player);
};

FSMJugador *EstadoJugadorDerecha::input_handle(KeyOyente &input,
                                               MouseOyente &mouse) {
  if (input.estaPresionado(SDL_SCANCODE_A))
    return new EstadoJugadorIzquierda();

#ifdef BLINEAL
  if (input.estaPresionado(SDL_SCANCODE_SPACE))
    return new EstadoJugadorInitBrinco(150);
#elif defined(GRAVEDAD)
  if (input.estaPresionado(SDL_SCANCODE_SPACE))
    return new EstadoJugadorGravedadBrinco(-10);
#elif defined(CLERP)
  if (input.estaPresionado(SDL_SCANCODE_SPACE))
    return new EstadoJugadorLerpBrinco(250, 1);
#endif

  if (!(input.estaPresionado(SDL_SCANCODE_A)) &&
      !(input.estaPresionado(SDL_SCANCODE_D))) {
    return new EstadoJugadorIDLE();
  }

  return NULL;
};

void EstadoJugadorDerecha::update(Jugador &player, double dt) {
  /*FSMCamara* ec = (FSMCamara*)ManejadorCamaras::get_estado();
  if(ec->strestado =="transicion")
  {
      Coordenadas p = player.get_posicion_mundo();
      p.x +=(player.get_velocidad());
      player.set_posicion_mundo(p);
  }*/
  player.get_sprite()->play_frame(1, frames_actual_ani % frames_maxim_ani);

  if (frame_dt > 3) {
    frame_dt = 0;
    frames_actual_ani++;
  }
  frame_dt++;
};

/*
-- BRINCO INIT
*/
EstadoJugadorInitBrinco::EstadoJugadorInitBrinco(int fbrinco) {
  fuerza.y = fbrinco;
  fuerza.x = 0;
};

void EstadoJugadorInitBrinco::entrar(Jugador &player) {
  // calcular la y final

  tope = player.get_piso();
  // asserrt que la fuerza sea por lo menos mas que la altura del sprite
  if (player.get_sprite()->get_sprite_sizes().y > fuerza.y)
    fuerza.y *= 2;
  tope.y -= fuerza.y;
  tope.x += fuerza.x;

  /* DEBUGPRINT("PISO:")
      DEBUGCOOR(player.get_piso())
      DEBUGPRINT("TOPE:")
      DEBUGCOOR(tope)
  */
};
void EstadoJugadorInitBrinco::update(Jugador &player, double dt){};
void EstadoJugadorInitBrinco::salir(Jugador &player){};
FSMJugador *EstadoJugadorInitBrinco::input_handle(KeyOyente &input,
                                                  MouseOyente &mouse) {
  return new EstadoJugadorSubeBrinco(tope);
};

/*
-- BRINCO SUBE
*/

EstadoJugadorSubeBrinco::EstadoJugadorSubeBrinco(Coordenadas max) {
  tope = max;
  frames_actual_ani = 0;
  frames_maxim_ani = 8;
  frames_actual = 0;
  frames_maximos = 20;
  llego = false;
};

FSMJugador *EstadoJugadorSubeBrinco::input_handle(KeyOyente &input,
                                                  MouseOyente &mouse) {
  if (llego)
    return new EstadoJugadorBajaBrinco(P2);
  return NULL;
};
void EstadoJugadorSubeBrinco::entrar(Jugador &player) {
  // calcular frames maximos?
  P1 = player.get_piso();
  P2 = tope;
  temp = P1;
};
void EstadoJugadorSubeBrinco::salir(Jugador &player){};
void EstadoJugadorSubeBrinco::update(Jugador &player, double dt) {
  if (frames_actual > frames_maximos) {
    llego = true;
    return;
  }

  float t = frames_actual / (float)frames_maximos;
  Coordenadas p = LERP(P1, P2, t);
  // temp.x = p.x -temp.x;
  temp.y -= p.y;

  // printf("P(%d,%d)\n",p.x,p.y);
  // printf("T(%d,%d)\n",temp.x,temp.y);

  Coordenadas actual = player.get_posicion_mundo();
  actual.y -= std::abs(temp.y);
  player.set_posicion_mundo(actual);
  // printf("A(%d,%d)\n",actual.x,actual.y);
  temp = p;

  player.get_sprite()->play_frame(2, frames_actual_ani % frames_maxim_ani);

  frames_actual++;
  if (frame_dt > (int)frames_maximos / 8) {
    frame_dt = 0;
    if (frames_actual_ani < frames_maxim_ani - 1)
      frames_actual_ani++;
  }
  frame_dt++;
};

/*
--- BRINCO BAJA
*/
EstadoJugadorBajaBrinco::EstadoJugadorBajaBrinco(Coordenadas tope) {
  P1 = tope;
  temp = P1;

  llego = false;
  frames_actual_ani = 7;
  frames_maxim_ani = 8;
  frames_actual = 0;
  frames_maximos = 20;
};

FSMJugador *EstadoJugadorBajaBrinco::input_handle(KeyOyente &input,
                                                  MouseOyente &mouse) {
  if (llego)
    return new EstadoJugadorIDLE();
  return NULL;
};
void EstadoJugadorBajaBrinco::entrar(Jugador &player) {
  P2 = player.get_piso();
};
void EstadoJugadorBajaBrinco::salir(Jugador &player){

};
void EstadoJugadorBajaBrinco::update(Jugador &player, double dt) {
  if (frames_actual > frames_maximos) {
    llego = true;
    return;
  }

  float t = frames_actual / (float)frames_maximos;
  Coordenadas p = LERP(P1, P2, t);
  // temp.x = p.x -temp.x;
  temp.y -= p.y;

  // printf("P(%d,%d)\n",p.x,p.y);
  // printf("T(%d,%d)\n",temp.x,temp.y);

  Coordenadas actual = player.get_posicion_mundo();
  actual.y += std::abs(temp.y);
  player.set_posicion_mundo(actual);
  // printf("A(%d,%d)\n",actual.x,actual.y);
  temp = p;

  player.get_sprite()->play_frame(2, frames_actual_ani % frames_maxim_ani);

  frames_actual++;

  if (frame_dt > (int)frames_maximos / 8) {
    frame_dt = 0;
    if (frames_actual_ani < frames_maxim_ani - 1)
      frames_actual_ani++;
  }
  frame_dt++;
};

/*
- BRINCO GRAVEDAD
*/
EstadoJugadorGravedadBrinco::EstadoJugadorGravedadBrinco(int fuerza) {
  f = fuerza;
  frames_actual_ani = 0;
  frames_maxim_ani = 8;
  strnombre = "BRINCAR";
  en_aire = true;
};

FSMJugador *EstadoJugadorGravedadBrinco::input_handle(
    KeyOyente &input,
    MouseOyente &mouse) { /////////////////////    NIVEL
  if (!en_aire)
    return new EstadoJugadorIDLE();
  if (input.estaPresionado(SDL_SCANCODE_D))
    if (P1.x < 935) {
      P1.x += (5 + (pow(SDLApp_AUX::get_nivel(), 2) - SDLApp_AUX::get_nivel()));
    } // nivel 1 = 11; nivel 2 = 7; nivel 3 = 5
  if (input.estaPresionado(SDL_SCANCODE_A))
    if (P1.x > 0) {
      P1.x -= 7;
    }

  return NULL;
};

void EstadoJugadorGravedadBrinco::entrar(Jugador &player) {
  player.set_dtgf(f);
  P1 = player.get_posicion_mundo(); // player.get_piso();
  en_aire = true;
  player.set_onPlataforma(false);

  // ManejadorCamaras::set_estado(new EstadoCamaraTransicion());
};
void EstadoJugadorGravedadBrinco::salir(Jugador &player){

};

void EstadoJugadorGravedadBrinco::update(Jugador &player, double dt) {

  player.get_sprite()->play_frame(2, frames_actual_ani % frames_maxim_ani);
  P1.y += player.get_dtgf();

  player.set_posicion_mundo(P1);
  if (player.get_onPlataforma())
    en_aire = false;

  if (frame_dt > (int)13) {
    frame_dt = 0;
    if (frames_actual_ani < frames_maxim_ani - 1)
      frames_actual_ani++;
  }
  frame_dt++;
};

/*LERP(LERP)*/
EstadoJugadorLerpBrinco::EstadoJugadorLerpBrinco(int max, int d) {
  pmax = max;
  dir = d;
};

void EstadoJugadorLerpBrinco::entrar(Jugador &player) {
  P1 = player.get_piso();
  P2 = P1;
  P2.x += player.get_velocidad() * 5 * dir;
  P2.y -= pmax;
  P3 = P2;
  P3.x += player.get_velocidad() * 10 * dir;
  P3.y = P1.y;

  temp = P1;

  en_aire = true;

  frames_actual_ani = 0;
  frames_maxim_ani = 8;

  frame_actual = 0;
  frame_maximo = 100;
};

FSMJugador *EstadoJugadorLerpBrinco::input_handle(KeyOyente &input,
                                                  MouseOyente &mouse) {

  if (!en_aire)
    return new EstadoJugadorIDLE();
  return NULL;
};

void EstadoJugadorLerpBrinco::salir(Jugador &player){

};
void EstadoJugadorLerpBrinco::update(Jugador &player, double dt) {
  if (frame_actual > frame_maximo) {
    en_aire = false;
    return;
  }

  float t = frame_actual / (float)frame_maximo;
  Coordenadas p1p2 = LERP(P1, P2, t);
  Coordenadas p2p3 = LERP(P2, P3, t);
  Coordenadas brinco = LERP(p1p2, p2p3, t);
  temp.x -= brinco.x;
  temp.y -= brinco.y;

  Coordenadas actual = player.get_posicion_mundo();
  actual.x -= temp.x;
  actual.y -= temp.y;

  player.set_posicion_mundo(actual);
  // DEBUGLINEA(temp,actual);
  temp = brinco;

  player.get_sprite()->play_frame(2, frames_actual_ani % frames_maxim_ani);
  frame_actual++;

  if (frame_dt > (int)3) {
    frame_dt = 0;
    if (frames_actual_ani < frames_maxim_ani - 1)
      frames_actual_ani++;
  }
  frame_dt++;
};