#ifndef OBJET3DSTATIQUE_H_INCLUDED
#define OBJET3DSTATIQUE_H_INCLUDED

#include <string>
#include <map>
#include <GL/gl.h>
#include <GL/glu.h>

class Objet3DStatique
{
    private:
        typedef std::map<std::string, GLuint> Textures;

        Textures textures; // Les textures de l'objet
        GLuint listeAffichage;
        GLuint listeRST;

    public:
        // Position de l'objet
        float positionX;
        float positionY;
        float positionZ;

        // Angle horizontal avec la verticale en z
        float angleHorizontal;

        std::string nom;

        Objet3DStatique(std::string nomFichier);
        ~Objet3DStatique();
        void dessiner3D();
};

#endif // OBJET3DSTATIQUE_H_INCLUDED
