#include "PlataformasSpawner.hpp"
#include "../Tiempo.hpp"
#include "../Func_aux.hpp"
#include "../../objetos_dinamicos/ObjetoDinamico.hpp"
#include<iostream>
#include<cstdlib>

PlataformasSpawner::PlataformasSpawner(std::string sprite_path, int x, int y, int w, int h,int sw, int sh,SDL_Color c,Pipeline &p)
{
    this->sprite_path=sprite_path;
    colordebug=c;
    this->x=x; // pos x
    this->y=y; // pos y
    this->w=w; // ancho sprite
    this->h=h; // alto sprite
    this->sw=sw;// ancho mostrar sprite
    this->sh=sh;// alto mostrar sprite
    objetos_activos=0;
    delay=3; 
    init_tiempo=Tiempo::get_tiempo();
    check=false;
    pipeline = &p;
    velocidad=0;
};

void PlataformasSpawner::spawn(std::vector<Objeto*>*lista)
{
    //random
    /*srand((unsigned) time(NULL));
    int sx = x+rand()%100;
    int sy = y+rand()%200;
    int nw = sw+rand()%100;
    int nh = sh+rand()%100;*/

    x+=(sw*2);

    PlataformasDinamicas * nuevo = new PlataformasDinamicas(
        sprite_path,x,y,w,h,sw,sh,colordebug);
    nuevo->set_velocidad(velocidad);
    pipeline->cargar_texturas(nuevo->get_sprite());
    lista->push_back(nuevo);
    objetos_activos++;
};
void PlataformasSpawner::set_velocidad(int v)
{
    velocidad=v;
}
void PlataformasSpawner::despawn(std::vector<Objeto*>*lista)
{
    int id = lista->size()-objetos_activos;
    delete lista->at(id);
    objetos_activos--;
    lista->erase(std::next(lista->begin()+id-1));
};
void PlataformasSpawner::update(std::vector<Objeto*>*lista)
{
    double dt =  Tiempo::get_tiempo() - init_tiempo;

    //DEBUGPRINT(dt)
    //DEBUGPRINT(past_tiempo)
    if((int)dt!=0 && ((int)dt)%delay == 0 && check==false && objetos_activos<5)
    {
        
        spawn(lista);
        past_tiempo=dt;
        check=true;
        DEBUGPRINT(std::to_string(dt)+" SPWAN "+std::to_string(objetos_activos))
        DEBUGCOOR(lista->at(lista->size()-1)->get_posicion_mundo());
    }
    
    
    if((int)dt!=0 && (int)dt%(delay*2) == 0 && !check)
    {
        DEBUGPRINT("DESPWAN")
        despawn(lista);
        //check=true;
    }

    
    
    //DEBUGPRINT(past_tiempo)
    //DEBUGPRINT((int)dt)
    if((int)dt>past_tiempo)
    {    
        check=false;
    }

};