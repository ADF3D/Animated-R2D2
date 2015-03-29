#include "3D_obj.h"

double ang = 0;

Obj_3D::Obj_3D()
{

}

Obj_3D::Obj_3D(char *filename)
{
    loadUsingFile(filename, 0);
}

bool Obj_3D::loadUsingFile(char *filename, unsigned char simplifyTimes)
{
    FILE *obj = fopen(filename, "r");
    char categ[10] = "";
    double tab[3] = {0};
    Face_3D f;
    char str[100] = "";
    char grp[10][100] = {""};
    int cnt_grp = 0;
    int cnt = 0;
    GLuint tex;

    if (obj == NULL)
        return false;

    while(fscanf(obj, "%s", categ) && !feof(obj))
    {
        cnt = 0;
        if(strcmp(categ, "v") == 0) // VERTEX
        {
            fscanf(obj, "%lf %lf %lf", &(tab[0]), &(tab[2]), &(tab[1]));
            vertices.push_back((Pos3D){tab[0], tab[1], tab[2]});
        }

        else if(strcmp(categ, "vn") == 0) // NORMAL
        {
            fscanf(obj, "%lf %lf %lf", &(tab[0]), &(tab[2]), &(tab[1]));
            normals.push_back((Pos3D){tab[0], tab[1], tab[2]});
        }

        else if(strcmp(categ, "vt") == 0) // TEXTURE POS
        {
            fscanf(obj, "%lf %lf", &(tab[0]), &(tab[1]));
            texPos.push_back((Pos2D){tab[0], tab[1]});
        }

        else if (strcmp(categ, "f") == 0) // FACE
        {
            char *ptr = str + 1;
            char *ptr2 = str + 1;
            char strtmp[100] = "";
            int counter = 0;
            fgets(str, 100, obj);

            while((ptr2 = strpbrk(ptr, " /\n")) != NULL)
            {
                strncpy(strtmp, ptr, ptr2 - ptr);

                strtmp[ptr2 - ptr] = '\0';

                ptr = ptr2 + 1;

                switch(counter)
                {
                    case 0:
                        f.v[cnt] = atoi(strtmp);
                        break;

                    case 1:
                        f.n[cnt] = atoi(strtmp);
                        f.t[cnt] = atoi(strtmp);
                        break;

                    case 2:
                        f.t[cnt] = atoi(strtmp);
                        break;
                }

                counter++;

                if(*ptr2 == ' ')
                {
                    cnt++;
                    counter = 0;
                }
            }

            f.cnt = cnt + 1;
            f.tex = tex;
            strcpy(f.g, grp[0]);

            faces.push_back(f);
        }

        else if(strcmp(categ, "usemtl") == 0) // TEXTURE POS
        {
            fscanf(obj, "%s", str);
            tex = loadTexture(str, true);
        }

        else if(strcmp(categ, "g") == 0)
        {
            char *ptr = str + 1;
            char *ptr2 = str + 1;
            cnt_grp = 0;
            fgets(str, 100, obj);

            while((ptr2 = strpbrk(ptr, " \n")) != NULL)
            {
                strncpy(grp[cnt_grp], ptr, ptr2 - ptr);

                grp[cnt_grp][ptr2 - ptr] = '\0';

                ptr = ptr2 + 1;

                cnt_grp++;
            }
        }
    }

    for (int i = 0 ; i < simplifyTimes ; i++)
    {
        simplify();
    }

    return true;
}

void Obj_3D::simplify()
{
    vector<Face_3D> f;
    Face_3D tmp;

    for(unsigned int i = 0 ; i < faces.size() - 1 ; i+=2)
    {
        unsigned int sz = faces[i].cnt;
        tmp.cnt = 4;
        tmp.tex = faces[i].tex;
        strcpy(tmp.g, faces[i].g);

        if (sz < faces[i+1].cnt)
            sz = faces[i+1].cnt;

        for(unsigned int j = 0 ; j < 4 ; j++)
        {
            tmp.v[j] = faces[i + (j > 1)].v[j % sz];
            tmp.t[j] = faces[i + (j > 1)].v[j % sz];
            tmp.n[j] = faces[i + (j > 1)].v[j % sz];
        }

        f.push_back(tmp);
    }

    while(faces.size() > 0)
        faces.pop_back();

    while(f.size() > 0)
    {
        faces.push_back(f[f.size() - 1]);
        f.pop_back();
    }
}

void Obj_3D::draw()
{
    GLfloat lightColor0[] = {1.0f, 1.0f, 1.0f, 1.0f};    // Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = {15.0f, 15.0f, 15.0f, 0.0f};      // Positioned at (4, 0, 8)

    // Ajout lumière positionnelle L0
    glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);        // lumière diffuse
    glLightf(GL_LIGHT0, GL_QUADRATIC_ATTENUATION, 1.0);
    glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);         // position
    glColor3ub(255, 255, 255);
    ang = 0.01 * SDL_GetTicks();

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
    for (unsigned int i = 0 ; i < faces.size() ; i++)
    {
        glBindTexture(GL_TEXTURE_2D, faces[i].tex);

        glPushMatrix();

        if (strcmp(faces[i].g, "head") == 0)
        {
            glTranslated(vertices[0].x, vertices[0].y, vertices[0].z);
            glRotated(ang , 0, 0, 1);
        }

        else if (strcmp(faces[i].g, "body") == 0)
        {
            /*glRotated(ang , 0, 0, 1);
            glTranslated(vertices[0].x, vertices[0].y, vertices[0].z);*/
        }

        else if (strcmp(faces[i].g, "left_arm_axis") == 0)
        {
            glRotated(ang + 90 , 1, 0, 0);
            glTranslated(vertices[0].x, vertices[0].y, vertices[0].z);
        }

        else if (strcmp(faces[i].g, "right_arm_axis") == 0)
        {
            glRotated(ang + 180 , 1, 0, 0);
            glTranslated(vertices[0].x, vertices[0].y, vertices[0].z);
        }

        else if (strcmp(faces[i].g, "foot_axis") == 0)
        {
            glRotated(ang , 1, 0, 0);
            glTranslated(vertices[0].x, vertices[0].y, vertices[0].z);
        }

        else
        {
            glTranslated(vertices[0].x, vertices[0].y, vertices[0].z);
        }

        if(faces[i].cnt == 3)
            glBegin(GL_LINE_LOOP);

        else
            glBegin(GL_QUADS);

        for (unsigned int j = 0 ; j < faces[i].cnt ; j++)
        {
            if (faces[i].t[j] > 0 && texPos.size() > faces[i].t[j])
                glTexCoord2d(texPos[faces[i].t[j] - 1].x, texPos[faces[i].t[j] - 1].y);

            if (faces[i].t[j] > 0 && normals.size() > faces[i].t[j])
                glNormal3d(normals[faces[i].t[j] - 1].x, normals[faces[i].t[j] - 1].y, normals[faces[i].t[j] - 1].z);

            glVertex3d(vertices[faces[i].v[j] - 1].x - vertices[0].x, vertices[faces[i].v[j] - 1].y  - vertices[0].y, vertices[faces[i].v[j] - 1].z - vertices[0].z);
        }

        glEnd();

        glPopMatrix();

        /*glFlush();
        SDL_GL_SwapBuffers();*/
    }

    glPopMatrix();
}

vector <Pos3D> Obj_3D::getVertices()
{
    return vertices;
}

vector <Pos2D> Obj_3D::getTexPos()
{
    return texPos;
}

vector <Pos3D> Obj_3D::getNormals()
{
    return normals;
}

vector <Face_3D> Obj_3D::getFaces()
{
    return faces;
}

void Obj_3D::setFaces(vector <Face_3D> _f)
{
    faces = _f;
}

void Obj_3D::setVertices(vector <Pos3D> _v)
{
    vertices = _v;
}

void Obj_3D::setTexPos(vector <Pos2D> _t)
{
    texPos = _t;
}

void Obj_3D::setNormals(vector <Pos3D> _n)
{
    normals = _n;
}

Obj_3D::~Obj_3D()
{

}
