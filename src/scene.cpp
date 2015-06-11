#include "scene.h"
#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "sdlglutils.h"

Scene::Scene()
{
    this->skybox = new Objet3DStatique("skybox.m3s");
}

Scene::~Scene()
{
    delete this->skybox;
}

void Scene::executer()
{
    this->continuer = true;

    while(this->continuer)
    {
        gererEvenements();
        animer();
        dessiner();
        afficher();
    }
}

void Scene::animer(void)
{
    //this->skybox->angleHorizontal += 0.3;
}

void Scene::dessiner(void)
{
    // Vidage de l'écran
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Place la camera
    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity();

    gluLookAt(3,4,-2,0,0,0,0,0,1);

    // Dessin de la skybox
    this->skybox->dessiner3D();
}

void Scene::afficher(void)
{
    glFlush();
    SDL_GL_SwapBuffers();
}

void Scene::gererEvenements(void)
{
    SDL_Event evenement;
    while (SDL_PollEvent(&evenement))
    {
        switch(evenement.type)
        {
            case SDL_QUIT:
                this->continuer = false;
        }
    }
}
