#ifndef SCENE_H_INCLUDED
#define SCENE_H_INCLUDED

#include <map>
#include <GL/gl.h>
#include <GL/glu.h>
#include <string>
#include "objet3DStatique.h"


class Scene
{

    private:
        typedef std::map<std::string, GLuint> Textures;

        bool continuer;

        Objet3DStatique* skybox;

        void gererEvenements(void);
        void animer(void);
        void dessiner(void);
        void afficher(void);

    public:

        Scene();
        ~Scene();
        void executer();
};

#endif // SCENE_H_INCLUDED

