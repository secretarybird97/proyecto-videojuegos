#include "PlataformasSpawner.hpp"
#include "../../objetos_dinamicos/ObjetoDinamico.hpp"
#include "../../sdlapp_aux.hpp"
#include "../Func_aux.hpp"
#include "../Tiempo.hpp"
#include <cstdlib>
#include <iostream>

PlataformasSpawner::PlataformasSpawner(std::string sprite_path, int x, int y,
                                       int w, int h, int sw, int sh,
                                       SDL_Color c, Pipeline &p) {
  this->sprite_path = sprite_path;
  colordebug = c;
  this->x = x;   // pos x
  this->y = y;   // pos y
  this->w = w;   // ancho sprite
  this->h = h;   // alto sprite
  this->sw = sw; // ancho mostrar sprite
  this->sh = sh; // alto mostrar sprite
  objetos_activos = 0;
  delay = 1;
  init_tiempo = Tiempo::get_tiempo();
  check = false;
  pipeline = &p;
  velocidad = 0;
};

void PlataformasSpawner::spawn(std::vector<Objeto *> *lista) {
  // random
  // srand((unsigned)time(NULL));
  /*int sx = x + rand() % 100;
  int sy = y + rand() % 200;
  int nw = sw + rand() % 100;
  int nh = sh + rand() % 100;*/

  int min = 0 + sw;
  int max = 935 - sw;

  int x = min + (std::rand() % (max - min + 1));
  // y -= (sh * 4 - (100 / SDLApp_AUX::get_nivel()));

  y -= (sh * (6 - SDLApp_AUX::get_nivel()) -
        (100 / 6 - SDLApp_AUX::get_nivel()));

  PlataformasDinamicas *nuevo = new PlataformasDinamicas(
      sprite_path, x, y, w, h, sw / SDLApp_AUX::get_nivel(),
      sh / SDLApp_AUX::get_nivel(), colordebug);
  nuevo->set_velocidad(velocidad);
  pipeline->cargar_texturas(nuevo->get_sprite());
  lista->insert(lista->end() - 1, nuevo); // push_back
  objetos_activos++;
};
void PlataformasSpawner::set_velocidad(int v) { velocidad = v; }
void PlataformasSpawner::despawn(std::vector<Objeto *> *lista) {
  int id = lista->size() - objetos_activos;

  delete lista->at(id - 1);

  objetos_activos--;
  std::cout << (objetos_activos) << std::endl;
  lista->erase(lista->end() - (21 - (SDLApp_AUX::get_nivel() * 5)));
};
void PlataformasSpawner::update(std::vector<Objeto *> *lista) {
  double dt = Tiempo::get_tiempo() - init_tiempo;

  // DEBUGPRINT(dt)
  // DEBUGPRINT(past_tiempo)
  if ((int)dt != 0 && ((int)dt) % delay == 0 && check == false &&
      objetos_activos < (20 - SDLApp_AUX::get_nivel() * 5)) {

    spawn(lista);
    past_tiempo = dt;
    check = true;
    DEBUGPRINT(std::to_string(dt) + " SPWAN " + std::to_string(objetos_activos))
    DEBUGCOOR(lista->at(lista->size() - 1)->get_posicion_mundo());
  }

  if ((int)dt != 0 && (int)dt % (delay * (4 - SDLApp_AUX::get_nivel())) == 0 &&
      !check) {
    DEBUGPRINT("DESPWAN")
    despawn(lista);
    // check=true;
  }

  // DEBUGPRINT(past_tiempo)
  // DEBUGPRINT((int)dt)
  if ((int)dt > past_tiempo) {
    check = false;
  }
};