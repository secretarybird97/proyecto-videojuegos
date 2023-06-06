#include "MotorFisico.hpp"
#include "../../utilidad/Estructuras.hpp"
#include "../../utilidad/Func_aux.hpp"
#include<iostream>
#include<cmath>
#include<algorithm>



MotorFisico2D* MotorFisico2D::instancia = 0;
float MotorFisico2D::gforce = .15f;

MotorFisico2D& MotorFisico2D::get()
{
    if(!instancia) 
        instancia = new MotorFisico2D();
    return *instancia;
};

bool MotorFisico2D::aabb_colision(Figura& A, Figura& B)
{
    //    A.-----.D
    //     |     |
    //    B.-----.C

    std::vector<Coordenadas> vA = A.get_vertices();
    std::vector<Coordenadas> vB = B.get_vertices();

    return  (vA[0].x < vB[2].x && vA[2].x > vB[0].x) &&
            (vA[0].y < vB[2].y && vA[2].y > vB[0].y);
    
};




bool MotorFisico2D::diag_colision(Figura& A, Figura& B)
{
    //revisar ambas figuras en un solo ciclo
    Figura* TA=&A;
    Figura* TB=&B;

    for(int i=0;i<2;i++)
    {
        if(i==1)
        {
            TA=&B;
            TB=&A;
        }
        /*
        (A) .--------. (D)
            |\       |
            |   .(t) |
            |     \  | 
        (B) .--------. (C)
        */

        std::vector<Coordenadas> vA = TA->get_vertices();
        Coordenadas Ac = {(vA[0].x+vA[3].x)/2,(vA[0].y+vA[1].y)/2};
        std::vector<Coordenadas> vB = TB->get_vertices();
        Coordenadas Bc = {(vB[0].x+vB[3].x)/2,(vB[0].y+vB[1].y)/2};

        //revisar las diagonales con AABB?
        for(int n = 0;n<vA.size();n++)
        {
            Coordenadas lineaA_inicio = Ac;
            Coordenadas lineaA_fin = vA[n];
            for(int m=0;m<vB.size();m++)
            {
                Coordenadas lineaB_inicio = Bc;
                Coordenadas lineaB_fin = vB[(m+1)%vB.size()];

                //intersección de las lineas
                //(x4-x3)*(y1-y2)-(x1-x2)*(y4-y3)
                float h = (lineaB_fin.x - lineaB_inicio.x) * (lineaA_inicio.y - lineaA_fin.y) -(lineaA_inicio.x-lineaA_fin.x)*(lineaB_fin.y-lineaB_inicio.y);
                //[(y3-y4)*(x1-x3)+(x4-x3)*(y1-y3)]/[(x4-x3)*(y1-y2)-(x1-x2)*(y4-y3)]
                float t1 = ((lineaB_inicio.y - lineaB_fin.y) *(lineaA_inicio.x-lineaB_inicio.x)+(lineaB_fin.x-lineaB_inicio.x)*(lineaA_inicio.y-lineaB_inicio.y))/h;
                //[(y1-y2)*(x1-x3)+(x2-x1)*(y1-y3)]/[(x4-x3)*(y1-y2)-(x1-x2)*(y4-y3)]
                float t2 = ((lineaA_inicio.y-lineaA_fin.y)*(lineaA_inicio.x-lineaB_inicio.x)+(lineaA_fin.x-lineaA_inicio.x)*(lineaA_inicio.y-lineaB_inicio.y))/h;

                //verificamos la intersección
                if(t1>=0.0f && t1<1.0f && t2>=0.0f && t2<1.0f)
                {
                    return true;
                }
            }
        }
    }
    return false;
};

bool MotorFisico2D::diag_overlap(Objeto& A, Objeto& B)
{
    //revisar ambas figuras en un solo ciclo
    Objeto* TA=&A;
    Objeto* TB=&B;

    for(int i=0;i<2;i++)
    {
        if(i==1)
        {
            TA=&B;
            TB=&A;
        }
        std::vector<Coordenadas> vA = TA->get_colbox()->get_vertices();
        Coordenadas Ac = {(vA[0].x+vA[3].x)/2,(vA[0].y+vA[1].y)/2};
        std::vector<Coordenadas> vB = TB->get_colbox()->get_vertices();
        Coordenadas Bc = {(vB[0].x+vB[3].x)/2,(vB[0].y+vB[1].y)/2};

        //revisar las diagonales con AABB?
        for(int n = 0;n<vA.size();n++)
        {
            Coordenadas lineaA_inicio = Ac;
            Coordenadas lineaA_fin = vA[n];

            Coordenadas offset ={0,0};
            for(int m=0;m<vB.size();m++)
            {
                Coordenadas lineaB_inicio = Bc;
                Coordenadas lineaB_fin = vB[(m+1)%vB.size()];

                //intersección de las lineas
                //(x4-x3)*(y1-y2)-(x1-x2)*(y4-y3)
                float h = (lineaB_fin.x - lineaB_inicio.x) * (lineaA_inicio.y - lineaA_fin.y) -(lineaA_inicio.x-lineaA_fin.x)*(lineaB_fin.y-lineaB_inicio.y);

                //[(y3-y4)*(x1-x3)+(x4-x3)*(y1-y3)]/[(x4-x3)*(y1-y2)-(x1-x2)*(y4-y3)]
                float t1 = ((lineaB_inicio.y - lineaB_fin.y) *(lineaA_inicio.x-lineaB_inicio.x)+(lineaB_fin.x-lineaB_inicio.x)*(lineaA_inicio.y-lineaB_inicio.y))/h;
                
                //[(y1-y2)*(x1-x3)+(x2-x1)*(y1-y3)]/[(x4-x3)*(y1-y2)-(x1-x2)*(y4-y3)]
                float t2 = ((lineaA_inicio.y-lineaA_fin.y)*(lineaA_inicio.x-lineaB_inicio.x)+(lineaA_fin.x-lineaA_inicio.x)*(lineaA_inicio.y-lineaB_inicio.y))/h;

                //verificamos la intersección
                if(t1>=0.0f && t1<1.0f && t2>=0.0f && t2<1.0f)
                {
                    //cuanto se sobrelapo
                    offset.x += (1.0f-t1)*(lineaA_fin.x-lineaA_inicio.x);
                    offset.y += (1.0f-t1)*(lineaA_fin.y-lineaA_inicio.y);
                }
            }
            //mover el objeto
            Coordenadas pos = A.get_posicion_mundo();
            pos.x+=offset.x*(i==0 ?-1:1);
            pos.y+=offset.y*(i==0 ?-1:1);
            A.set_posicion_mundo(pos);
        }
    }
    return false;
};

void MotorFisico2D::sat_colision(ObjetoDinamico& p, std::vector<ObjetoEstatico*>obj)
{
    for(auto& o:obj)
    {
        if(o->get_tiene_fisica())
            p.en_colision |= get().sat(p,*o);
    }
};

bool MotorFisico2D::sat(Objeto &A, Objeto& B)
{
    //para no iterar tanto copiar y flipearlos
    Objeto *objA = &A;
    Objeto *objB = &B;
    
    //dos comparaciones a la vez
    for(int fig=0;fig<2;fig++)
    {
        if(fig==1)
        {
            //flip para no iterar nuevamente
            objA = &B;
            objB = &A;
        }

        //proyectar el axis en figura 1
        std::vector<Coordenadas> vertA = objA->get_colbox()->get_vertices();
        for(int i=0;i<vertA.size();i++)
        {
            int j = (i+1)%vertA.size(); //el siguiente punto
            //como son ortogonales -y,x da la proyection, 
            float axis_x = -(vertA[j].y-vertA[i].y);
            float axis_y = vertA[j].x - vertA[i].x;
            //normalizar
            float magnitud = std::sqrt(axis_x*axis_x + axis_y*axis_y);
            axis_x /=magnitud;
            axis_y /=magnitud;

            //proyectamos todos los puntos en el axis y calculamos los limites
            float a_min = INFINITY, a_max = -INFINITY;
            for(int a=0; a<vertA.size();a++)
            {
                //producto punto nos da la escala del reflejo
                float q = vertA[a].x*axis_x+vertA[a].y*axis_y;
                a_min = std::min(a_min,q);
                a_max = std::max(a_max,q);
            }

            //lo mismo para la segunda figura
            float b_min = INFINITY, b_max = -INFINITY;
            std::vector<Coordenadas> vertB = objB->get_colbox()->get_vertices();
            for(int b=0; b<vertB.size();b++)
            {
                //producto punto nos da la escala del reflejo
                float m = vertB[b].x*axis_x+vertB[b].y*axis_y;
                b_min = std::min(b_min,m);
                b_max = std::max(b_max,m);
            }

            //AABB si no sobrelapan en algun axis regresar falso
            if(!(b_max>=a_min && a_max >=b_min))
            {
                return false;
            }

        }
    }
    // si llega aquí significa que un axis hay sobrelapo
    return true;

};


void MotorFisico2D::sortandsweep_colision(std::vector<ObjetoDinamico*> objs)
{
    
};

void MotorFisico2D::gravedad(std::vector<ObjetoDinamico*> objs)
{
    for(auto& o: objs)
    {
        if(!o->get_tiene_fisica())
            continue;
        int cy = o->get_posy();
        //if(o->get_dtgf()!=0)
        if(o->get_dtgf()<=5) //cap 
            o->set_dtgf(o->get_dtgf()+get().gforce);
        int y = cy+o->get_dtgf();
        o->set_posy(y);
    }
};
