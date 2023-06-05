#include "sdl_aux.hpp"

void SDLApp_AUX::init_mundo() {
  // 02_lineas_figuras
  // SDLApp::get().mapa.push_back(
  // new Atlas("assets/sprites/mundo/ids/3x3.txt", {-500, -500}));

  SDLApp::get().mapa.push_back(
      new Atlas("assets/sprites/mundo/ids/test.txt", {500, 500}));

  /* SDLApp::get().mapa.push_back(
      new Atlas("assets/sprites/mundo/ids/base.txt", {30, 620}));

  SDLApp::get().mapa.push_back(
      new Atlas("assets/sprites/mundo/ids/3x3.txt", {200, 200}));

  SDLApp::get().mapa.push_back(
      new Atlas("assets/sprites/mundo/ids/3x3.txt", {600, 400}));

  SDLApp::get().mapa.push_back(
      new Atlas("assets/sprites/mundo/ids/4x4.txt", {400, 100}));*/

  for (auto atlas : SDLApp::get().mapa) {
    atlas->generar_mapa(SDLApp::get().render, 2, 0, SDLApp::SCALE);
  }
};

std::vector<ObjetoEstatico *> SDLApp_AUX::get_mundo() {
  std::vector<ObjetoEstatico *> temp;
  for (auto atlas : SDLApp::get().mapa) {
    for (auto obj : atlas->get_objetos_fisicos()) {
      temp.push_back(obj);
    }
  }

  return temp;
};

void SDLApp_AUX::update_mundo() {
  for (auto atlas : SDLApp::get().mapa) {
  }
};