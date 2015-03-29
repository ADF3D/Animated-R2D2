#include "dessin.h"

extern FreeFlyCamera *camera;
extern GLuint eau;

void dessiner(Obj_3D cube)
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

    glMatrixMode( GL_MODELVIEW );
    glLoadIdentity( );

    camera->look();

    cube.draw();

    glFlush();
    SDL_GL_SwapBuffers();
}
