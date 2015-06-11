#include <SDL.h>

#include <GL/gl.h>
#include <GL/glu.h>

#include <cmath>

#include "constantes.h"
#include "FreeFlyCamera_object.h"
#include "sdlglutils.h"
#include "dessin.h"
#include "scene.h"

FreeFlyCamera *camera;

void stop();

int main(int argc, char *argv[])
{
    atexit(stop);

    SDL_Init(SDL_INIT_VIDEO);

    bool continuer = true;
    SDL_Event event;
    SDL_Surface *ecran = SDL_SetVideoMode(LARGEUR_FENETRE, HAUTEUR_FENETRE, 32, SDL_OPENGL | SDL_HWACCEL | SDL_HWSURFACE | SDL_FULLSCREEN | SDL_DOUBLEBUF);
    Vector3D vec;
    Obj_3D cube;
    Scene scene;

    //scene.executer();

    if (cube.loadUsingFile((char*)"3D_Obj/R2D2/R2D2.obj") == false)
    {
        exit(0);
    }

    vec.set(0, -100, -50);

    int current_time = 0, elapsed_time = 0, last_time = 0;

    camera = new FreeFlyCamera(vec);

    // definition de la perspective
    glMatrixMode( GL_PROJECTION );
    glLoadIdentity( );
    gluPerspective(70,(double)LARGEUR_FENETRE/HAUTEUR_FENETRE,1,10000);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    GLfloat ambientColor[] = {1.0f, 1.0f, 1.0f, 1.0f};   // Color (0.2, 0.2, 0.2)

    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);

    glEnable(GL_LIGHT0);

    while(continuer)
    {
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_QUIT:
                exit(0);
                break;
                case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                    case SDLK_p:
                    takeScreenshot("test.bmp");
                    break;
                    case SDLK_ESCAPE:
                    exit(0);
                    break;
                    default : //on a utilisé la touche P et la touche ECHAP, le reste (en keydown) est donné à la caméra
                    camera->OnKeyboard(event.key);
                }
                break;
                case SDL_KEYUP: //on n'utilise pas de keyup, on donne donc tout à la caméra
                camera->OnKeyboard(event.key);
                break;
                case SDL_MOUSEMOTION: //la souris est bougée, ça n'intéresse que la caméra
                camera->OnMouseMotion(event.motion);
                break;
                case SDL_MOUSEBUTTONUP:
                case SDL_MOUSEBUTTONDOWN: //tous les évenements boutons (up ou down) sont donnés à la caméra
                camera->OnMouseButton(event.button);
                break;
            }
        }

        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();

        current_time = SDL_GetTicks();
        elapsed_time = current_time - last_time; //on calcule le temps écoulé depuis la dernière image
        last_time = current_time;

        camera->animate(elapsed_time); //et on fait bouger la caméra

        // Dessin
        dessiner(cube);
    }

    SDL_Quit();

    SDL_FreeSurface(ecran);

    return EXIT_SUCCESS;
}

void stop()
{
    delete camera;
    SDL_Quit();
}
