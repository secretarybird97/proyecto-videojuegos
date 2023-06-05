#pragma once
#include "../sdlapp.hpp"

class SDLApp_AUX {
public:
  static void init_mundo();
  static std::vector<ObjetoEstatico *> get_mundo();
  static void update_mundo();
};