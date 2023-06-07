#include "sdlapp_aux.hpp"
#include "sdlapp.hpp"

int SDLApp_AUX::get_nivel() { return SDLApp::get().nivel; };

void SDLApp_AUX::set_nivel(int n) { SDLApp::get().nivel = n; };