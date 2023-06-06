#include "ObjetoEstatico.hpp"

Plataformas::Plataformas(int x, int y, int w, int h, SDL_Color color)
    : ObjetoEstatico{} // llamar constructor padre
{
  posicion_mundo.x = x;
  posicion_mundo.y = y;
  avatar = new Rectangulo(x, y, w, h, color);
  avatar->set_rellenocolor(color);
  avatar->set_serellena(false);
  col_box = new Rectangulo(x, y, w + 5, h + 5, {0, 255, 0, 255});
  tiene_fisica = false;
  en_colision = false;
  sprite = nullptr;
};

Plataformas::Plataformas(TileInfo tiles, bool colision) {
  avatar = nullptr;
  sprite = nullptr;
  posicion_mundo = tiles.pos;
  col_box = new Rectangulo(posicion_mundo.x - (tiles.width / 2),
                           posicion_mundo.y - (tiles.height / 2), tiles.width,
                           tiles.height, {0, 255, 0, 255});
  tiene_fisica = colision;
  en_colision = false;
  tile = new Tile(tiles);
};

void Plataformas::update(double dt) {
  // if Plataforma is out of screen delete it
  // printf("\n%f %f", posicion_mundo.x, posicion_mundo.y);
  // printf(" %f %f", posicion_camara.x, posicion_camara.y);
  if (posicion_mundo.x < -100 || posicion_mundo.x > 1000) {
    this->~Plataformas();
  }
};

Background::Background(std::string path_sprite, int x, int y, int w, int h)
    : ObjetoEstatico{} {
  posicion_mundo.x = x;
  posicion_mundo.y = y;
  avatar = new Rectangulo(x, y, w, h, color);
  avatar->set_rellenocolor(color);
  avatar->set_serellena(true);
  col_box = new Rectangulo(x, y, w + 5, h + 5, {0, 255, 0, 255});
  sprite = new Sprite(path_sprite, posicion_mundo, w, h, w, h);
  tiene_fisica = false;
  en_colision = false;
  velocidad = 2; // modificar variable para cambiar velocidad de fondo
};

void Background::update(double dt) {
  // cambiar los valores de la condicion de acuerdo al tamaño del fondo
  if (posicion_mundo.y >= 2280) {
    this->set_posicion_mundo({get_posx(), (int)(720 / 2)});
  } else if (posicion_mundo.y <= -1560) {
    this->set_posicion_mundo({get_posx(), (int)(720 / 2)});
  }
};