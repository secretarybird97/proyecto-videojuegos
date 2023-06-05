#pragma once
#include "../../objetos/Objeto.hpp"
#include "../../objetos_dinamicos/ObjetoDinamico.hpp"
#include "../../objetos_estaticos/ObjetoEstatico.hpp"
#include <vector>

class MotorFisico2D {
public:
  static MotorFisico2D &get();
  static bool aabb_colision(Figura &A, Figura &B);
  static bool diag_colision(Figura &A, Figura &B);
  static bool diag_overlap(Objeto &A, Objeto &B);
  static void sat_colision(ObjetoDinamico &p,
                           std::vector<ObjetoEstatico *> obj);
  static void sortandsweep_colision(std::vector<ObjetoDinamico *> objs);

  static void gravedad(std::vector<ObjetoDinamico *> objs);

private:
  bool sat(Objeto &A, Objeto &B);
  bool aabb(Objeto &A, Objeto &B);

private:
  MotorFisico2D(){};
  static MotorFisico2D *instancia;
  float gforce{0.09};
  float dtgravedad{0};
};
