#pragma once
#include "../../objetos/Objeto.hpp"
#include "../../motor/Pipeline.hpp"
#include <SDL.h>

class PlataformasSpawner
{
    public:
        PlataformasSpawner(std::string sprite_path, int x, int y, int w, int h,int sw, int sh,SDL_Color c,Pipeline &p);

        void spawn(std::vector<Objeto*>*lista);
        void despawn(std::vector<Objeto*>*lista);
        void update(std::vector<Objeto*>*lista);
        void set_velocidad(int v);
    private:
        std::vector<Objeto*> objetos;
        std::string sprite_path;
        SDL_Color colordebug;
        int x; // pos x
        int y; // pos y
        int w; // ancho sprite
        int h; // alto sprite
        int sw;// ancho mostrar sprite
        int sh;// alto mostrar sprite
        int objetos_activos;
        int delay;
        double init_tiempo;
        int past_tiempo;
        bool check;
        Pipeline *pipeline;
        int velocidad;

};