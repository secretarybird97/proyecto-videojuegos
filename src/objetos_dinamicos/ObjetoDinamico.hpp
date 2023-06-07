#pragma once
#include "../motor/KeyOyente.hpp"
#include "../motor/MouseOyente.hpp"
#include "../objetos/Objeto.hpp"
#include "../utilidad/FSMS/FSMJugador.hpp"
#include <SDL.h>
#include <string>
class FSMJugador; // forward declaration

class ObjetoDinamico : public Objeto {
public:
  virtual ~ObjetoDinamico(){};
  virtual void update(double dt) = 0;
  float get_dtgf() const { return dtgravedad; };
  void set_dtgf(float v) { dtgravedad = v; };

  // forma general de convertir cualquier puntero si se conoce la clase
  virtual void set_estado(void *estado) = 0;
  virtual void *get_estado() = 0;

  Coordenadas offsetoverlap;

protected:
  float dtgravedad{0};
};

class Jugador : public ObjetoDinamico {
public:
  virtual ~Jugador(){};
  Jugador(float vida, int x, int y, SDL_Color c);
  Jugador(std::string path_sprite, float vida, int x, int y, int w, int h,
          int sw, int sh, SDL_Color c);
  void update(double dt);
  void input_handle(KeyOyente &input, MouseOyente &mouse);
  std::string get_strEstado();
  Coordenadas get_piso() const { return piso; };
  void set_piso(Coordenadas p) { piso = p; };

  void set_estado(void *estado);
  void *get_estado();

  std::vector<Objeto *> get_objetos_mundo() const { return *objetos; };
  void set_objetos_mundo(std::vector<Objeto *> &o) { objetos = &o; };

  Figura *get_col_piso() const { return col_piso; };
  bool get_onPlataforma() const { return en_plataforma; };
  void set_onPlataforma(bool v);
  Coordenadas get_centro() const { return centro; };

  Sprite *get_sprite_salto() const { return sprite_salto; };
  void set_sprite_salto(Sprite *s) { sprite_salto = s; };

protected:
  Sprite *sprite_salto;

private:
  FSMJugador *estado_actual;
  Coordenadas piso;
  std::vector<Objeto *> *objetos;
  Figura *col_piso;
  bool en_plataforma{false};
  Coordenadas centro;
};

class BackgroundDinamico : public ObjetoDinamico {
public:
  BackgroundDinamico(std::string path_sprite, int w, int h);
  virtual ~BackgroundDinamico(){};
  void update(double dt);
  void set_estado(void *estado);
  void *get_estado();

private:
  Coordenadas limites;
  Coordenadas centro_cam;
};

class PlataformasDinamicas : public ObjetoDinamico {
public:
  PlataformasDinamicas(std::string sprite_path, int x, int y, int w, int h,
                       int sw, int sh, SDL_Color c);
  virtual ~PlataformasDinamicas(){};
  void update(double dt);
  void set_estado(void *estado);
  void *get_estado();
};