#include "dessin.h"

extern FreeFlyCamera *camera;

void dessiner(Obj_3D cube)
{
    GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f};    // Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = {15.0f, 15.0f, 15.0f, 0.0f};      // Positioned at (4, 0, 8)

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode( GL_MODELVIEW );

    // Ajout lumière positionnelle L0
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);        // lumière diffuse
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);         // position
    glColor3ub(255, 255, 255);

    glPushMatrix();
    glTranslated(lightPos0[0], lightColor0[1], lightPos0[2]);
    glBegin(GL_QUADS);
        glVertex3d(1, 1, 1);
        glVertex3d(-1, 1, 1);
        glVertex3d(-1, -1, 1);
        glVertex3d(1, -1, 1);
        glVertex3d(1, 1, 1);
        glVertex3d(-1, 1, 1);
        glVertex3d(-1, 1, -1);
        glVertex3d(1, 1, -1);
        glVertex3d(1, 1, 1);
        glVertex3d(1, -1, 1);
        glVertex3d(1, -1, -1);
        glVertex3d(1, 1, -1);
    glEnd();
    glPopMatrix();

    glLoadIdentity();

    camera->look();

    glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, lightPos0);
    cube.draw();

    glFlush();
    SDL_GL_SwapBuffers();
}
