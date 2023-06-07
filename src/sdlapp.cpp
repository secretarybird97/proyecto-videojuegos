#include "sdlapp.hpp"
#include "motor/KeyOyente.hpp"
#include "motor/MouseOyente.hpp"
#include "motor/Plotter.hpp"
#include "motor/fisica/MotorFisico.hpp"
#include "utilidad/Estructuras.hpp"
#include "utilidad/FSMS/FSMJugador.hpp"
#include "utilidad/Func_aux.hpp"
#include "utilidad/RenderTexto.hpp"
#include "utilidad/Tiempo.hpp"
#include <SDL_ttf.h>
#include <algorithm>
#include <iostream>
#include <math.h>

// #define RECTANGULOS
// #define TRIANGULOS
// #define CIRCULOS

SDLApp *SDLApp::instancia = 0;


SDLApp::SDLApp() {
  vnt = nullptr;
  vntsurf = nullptr;
  render = nullptr;
  ensamble = nullptr;
  bg_color.r = 255; // rojo
  bg_color.g = 255; // verde
  bg_color.b = 255; // azul
  bg_color.a = 255; // alpha

  esta_corriendo = true;
  WIDTH = 1024;
  HEIGHT = 720;
  maxFPS = 60;
  fps = 0;
};

// Como solo queremos una instancia del esta clase, entonces se hace una clase
// singleton
SDLApp &SDLApp::get() {
  if (!instancia)
    instancia = new SDLApp();
  return *instancia;
};

bool SDLApp::on_init() {
  // revisar que SDL inicio bien
  if (SDL_Init(SDL_INIT_EVERYTHING) < 0) {
    return false;
  };
  // crear la ventana
  get().vnt =
      SDL_CreateWindow("Juego",                 // Titulo de la ventana
                       SDL_WINDOWPOS_UNDEFINED, // posicion X de la pantalla
                       SDL_WINDOWPOS_UNDEFINED, // posicion Y de la pantalla
                       get().WIDTH,             // ancho de la ventana
                       get().HEIGHT,            // alto de la ventana
                       SDL_WINDOW_OPENGL);      // que use OPENGL

  TTF_Init();
  IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);
  // revisar que se creo bien la ventana
  if (get().vnt == NULL) {
    printf("No se creo la ventana por: %s", SDL_GetError());
    return false;
  }
  // la forma de procesar en GPU
  SDL_SetHint(SDL_HINT_RENDER_BATCHING, "opengl");
  // creamos el 'canvas'
  get().render = SDL_CreateRenderer(
      get().vnt,                                             // la ventana
      -1,                                                    // driver
      SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC); // flags driver
  // revisamos si se creo correctamente
  if (get().render == NULL) {
    printf("No se creo el renderer por: %s", SDL_GetError());
    return false;
  }

  Plotter::get().set_renderer(*get().render);
  // si se creo correcto lo agregamos al Pipeline
  get().ensamble = new Pipeline(*get().render);

  // 06_Camaras

  get().camara_principal =
      new Camara(0, 0, get().WIDTH, get().HEIGHT, *get().render);

  ManejadorCamaras::get().set_camara(*get().camara_principal);

  // platspawn = new
  // PlataformasSpawner("assets/sprites/mundo/bg/pared_sprite.png", 1024, 670,
  // 216, 72, 100, 72, {0, 255, 0, 255}, *get().ensamble);

  platspawn = new PlataformasSpawner("assets/sprites/mundo/bg/pared_sprite.png",
                                     500, 875, 216, 72, 100, 72,
                                     {0, 255, 0, 255}, *get().ensamble);

  platspawn->set_velocidad(5);

  // 08 tiles
  mapa = new Atlas("assets/sprites/mundo/ids/mundo_ids.txt");
  mapa->generar_mapa(get().render, 2, 0);
  // mapa->generar_mapa(get().render, 2, 0);
  //  05
  player = new Jugador("assets/sprites/heroe/gato_sheet.png",
                       //      hp , x , y, sW,sH , vW,vH ,color
                       100, (int)(get().WIDTH / 2), (int)(get().HEIGHT), 500,
                       520, 100, 100, {255, 0, 255, 255});
  get().ensamble->cargar_texturas(player->get_sprite());
  int nivel = 1;
  player->set_velocidad(nivel*5);
  
  printf("Se creo el player\n");

  plataformas = mapa->get_objetos_fisicos();

  PlataformasDinamicas *test =
      new PlataformasDinamicas("assets/sprites/mundo/bg/pared_sprite.png", 700,
                               670, 216, 72, 100, 72, {0, 255, 0, 255});

  test->set_velocidad(5);
  get().ensamble->cargar_texturas(test->get_sprite());

  // plataformadinamicas.push_back(test);

  // lock camara
  ManejadorCamaras::get().lock_objeto(*player);
  ManejadorCamaras::get().set_estado(new EstadoCamaraTransicion());

  // 09 parallax
  backgrounds.push_back(new Background("assets/sprites/backgrounds/layer1v.png",
                                       0, 0, 1080, 3840));

  // ESTRELLAS PEQUEÑAS
  backgrounds.push_back(new Background("assets/sprites/backgrounds/layer2v.png",
                                       0, 0, 1080, 3840));

  for (auto bg : backgrounds) {
    get().ensamble->cargar_texturas(bg->get_sprite());
    objetos.push_back(bg);
  }

  for (int i = 0; i < plataformas.size(); i++) {
    // agregar todos los objetos en una lista para la camara
    plataformas[i]->set_velocidad(5);
    objetos.push_back(plataformas[i]);
  }
  objetos.push_back(test);

  player->set_objetos_mundo(objetos);

  objetos.push_back(player);

  printf("\nSe crearon los objetos exitosamente\n");

  // agregamos el color del background del frame
  SDL_SetRenderDrawColor(get().render,           // canvas
                         get().bg_color.r,       // rojo
                         get().bg_color.g,       // verde
                         get().bg_color.b,       // azul
                         SDL_ALPHA_TRANSPARENT); // como usar el alpha

  return true;
};

void SDLApp::on_evento(SDL_Event *evento) {
  if (evento->type == SDL_QUIT) {
    get().esta_corriendo = false;
  }
  // eventos del mouse
  MouseOyente::get().evento_movimiento(evento);
  MouseOyente::get().evento_presionado(evento);
  MouseOyente::get().evento_scroll(evento);
  // que esta presionado
  KeyOyente::get().keyPresionado(evento);
};

void SDLApp::on_fisicaupdate(double dt) {

  // Camara Lock UnLock
  if (ManejadorCamaras::get().get_camara().get_obj_lock()) {
  }

  player->input_handle(KeyOyente::get(), MouseOyente::get());
  platspawn->update(&objetos);
  // printf("update_fisica1\n");
  bool pplat = false;
  for (auto &p : objetos) {
    p->update(dt);
    /*if(!p->render_colbox && p->get_colbox())
        p->render_colbox=true;*/

    if (p != player && p->get_colbox()) {

      // MotorFisico2D::get().diag_overlap(*player, *p); //hace que lo empuje
      bool pc = MotorFisico2D::get().aabb_colision(*player->get_colbox(),
                                                   *p->get_colbox());
      player->en_colision |= pc;

      if (p->area_plataforma) {
        pplat |= MotorFisico2D::get().aabb_colision(*player->area_plataforma,
                                                    *p->area_plataforma);
        // printf("en plataforma [%d]\n",pplat);
        if (pplat) {
          MotorFisico2D::get().diag_overlap(*player, *p);
          player->set_onPlataforma(pplat);

          // poner coordenada exacta en plataforma del jugador en y

          // player->en_colision=false;
        }
      }
    }
  }
  //  printf("update_fisica2\n");
#ifdef GRAVEDAD
  MotorFisico2D::get().gravedad({player});
#endif
  /*CAMARA al final para actualizar la proyeción de los objetos*/
  ManejadorCamaras::get().input_handle(KeyOyente::get(), MouseOyente::get());
  ManejadorCamaras::get().update(objetos);
  // printf("update_fisica3\n");

  /*if(muerto)
  {
    get().on_limpiar8();\\
  }*/
};

void SDLApp::on_frameupdate(double dt) {
  // limpiar frame
  SDL_RenderClear(get().render);

  // Renderizar todo a través de la camara
  ManejadorCamaras::get().renderizar(objetos);
  camara_principal->render_cross();

  // printf("update_frame\n");

  Coordenadas pm = player->get_posicion_mundo();
  Coordenadas pc = player->get_posicion_camara();

  std::string spm =
      "mundo ( " + std::to_string(pm.x) + ", " + std::to_string(pm.y) + ")";

  RenderTexto::get().render_texto(get().render, 100, 100, spm, 150, 50,
                                  {0, 0, 0, 255});

  std::string spc =
      "camara ( " + std::to_string(pc.x) + ", " + std::to_string(pc.y) + ")";

  RenderTexto::get().render_texto(get().render, 800, 100, spc, 150, 50,
                                  {0, 0, 0, 255});

  // get().ensamble->figuras(player->get_col_piso());
  FSMJugador *e = (FSMJugador *)player->get_estado();
  RenderTexto::get().render_texto(get().render, 600, 600, e->get_namestate(),
                                  100, 50, {255, 0, 255, 255});

  FSMCamara *ce = (FSMCamara *)ManejadorCamaras::get().get_estado();
  RenderTexto::get().render_texto(get().render, 100, 600, ce->strestado, 100,
                                  50, {255, 0, 255, 255});

  // Actualizar
  SDL_RenderPresent(get().render);

  // resetear color del frame
  SDL_SetRenderDrawColor(get().render, get().bg_color.r, get().bg_color.g,
                         get().bg_color.b, SDL_ALPHA_OPAQUE);
};

void SDLApp::on_limpiar() {
  SDL_DestroyRenderer(get().render);
  SDL_FreeSurface(get().vntsurf);
  SDL_DestroyWindow(get().vnt);

  for (std::vector<Objeto *>::iterator it = objetos.begin();
       it != objetos.end(); it++) {
    Objeto *p = *it;
    delete p;
  }
  // objetos.clear();
  delete ensamble;
  // delete player;
  delete platspawn;
  SDL_Quit();
};

int SDLApp::on_correr() {
  // revisar que todo se inicializo bien
  if (get().on_init() == false) {
    return -1;
  }

  SDL_Event eventos;
  double dt = 0;
  double frecuencia = 1 / get().maxFPS; // 1 frame a 60fps
  get().msfrecuencia = frecuencia * 1000;

  while (get().estaCorriendo()) {
    // double start = SDL_GetTicks();
    double inicio = Tiempo::get_tiempo();

    // printf("%lf <> %d\n",Tiempo::get_tiempo(),SDL_GetTicks());
    // captura eventos
    while (SDL_PollEvent(&eventos)) {
      get().on_evento(&eventos);
    }

    // actualizamos si inicia o hay una diferencia de tiempo

    get().on_fisicaupdate(dt);
    get().on_frameupdate(dt);

    // calculamos el tiempo del frame
    dt = (Tiempo::get_tiempo() - inicio) / (frecuencia * 1000);

    inicio = dt; // el nuevo frame inicia en este tiempo
    // printf("<%d>[%lf][%d]\n",get().fps,dt,(int)Tiempo::get_tiempo());
    if (dt > get().msfrecuencia) {
      SDL_Delay(floor(get().msfrecuencia - dt));
    }
    // get().fps++;
  }
  // liberamos memoria
  get().on_limpiar();
  return 0;
};
