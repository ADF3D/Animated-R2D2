#ifndef WATER

    #define WATER

    #include <GL/gl.h>
    #include <SDL.h>

    #include "FreeFlyCamera_object.h"

    class Water
    {
        private :

            GLuint texture;
            bool isAnimed;
            double anim;

        public :

            Water(GLuint Itexture , bool IisAnimed = false);

            GLuint getTexture();
            double getAnim();

            ~Water();
    };

#endif
