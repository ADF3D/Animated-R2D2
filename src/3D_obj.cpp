#include "3D_obj.h"

Obj_3D::Obj_3D()
{

}

Obj_3D::Obj_3D(char *filename)
{
    loadUsingFile(filename);
}

bool Obj_3D::loadUsingFile(char *filename)
{
    FILE *obj = fopen(filename, "r");
    char categ[10] = "";
    double tab[3] = {0};
    Face_3D f;
    char str[100] = "";
    char grp[100] = "";
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
                        f.t[cnt] = atoi(strtmp);
                        break;

                    case 2:
                        f.n[cnt] = atoi(strtmp);
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
            strcpy(f.g, grp);

            faces.push_back(f);
        }

        else if(strcmp(categ, "usemtl") == 0) // TEXTURE POS
        {
            fscanf(obj, "%s", str);
            tex = loadTexture(str, true);
        }

        else if(strcmp(categ, "g") == 0 || strcmp(categ, "o") == 0)
        {
            fscanf(obj, "%s", grp);

            if (strcmp(grp, "head") == 0)
            {
                grabPoint[grp] = (Pos3D){0, 1, 0};
            }

            else if (strcmp(grp, "body") == 0)
            {
                grabPoint[grp] = (Pos3D){0, 0, 0};
            }

            else if (strcmp(grp, "left_arm_axis") == 0)
            {
                grabPoint[grp] = (Pos3D){-14, 1, 18};
            }

            else if (strcmp(grp, "right_arm_axis") == 0)
            {
                grabPoint[grp] = (Pos3D){14, 1, 18};
            }

            else if (strcmp(grp, "foot_axis") == 0)
            {
                grabPoint[grp] = (Pos3D){0, 0, 6};
            }

        }
    }

    return true;
}

void Obj_3D::draw()
{

    static double ang = 0;
    register unsigned int i = 0, j = 0;

    ang+= 5;

    glPushMatrix();
    glTranslated(pos.x, pos.y, pos.z);

    glBindTexture(GL_TEXTURE_2D, faces[0].tex);

    for (i = 0 ; i < faces.size() ; i++)
    {
        glPushMatrix();

        glTranslated(grabPoint[faces[i].g].x, grabPoint[faces[i].g].y, grabPoint[faces[i].g].z);

        glBegin(GL_TRIANGLES);

        if(strcmp(faces[i].g, "head") == 0)
        {
            glRotated(ang_head, 0, 0, 1);
        }

        else if(strcmp(faces[i].g, "left_arm_axis") == 0)
        {
            glRotated(ang_left_arm, 0, 0, 1);
        }

        else if(strcmp(faces[i].g, "right_arm_axis") == 0)
        {
            glRotated(ang_right_arm, 0, 0, 1);
        }

        else if(strcmp(faces[i].g, "foot_axis") == 0)
        {
            glRotated(ang_foot, 0, 0, 1);
        }


        for (j = 0 ; j < faces[i].cnt ; j++)
        {
            glTexCoord2d(texPos[faces[i].t[j] - 1].x,texPos[faces[i].t[j] - 1].y);
            glNormal3d(normals[faces[i].n[j] - 1].x, normals[faces[i].n[j] - 1].y, normals[faces[i].n[j] - 1].z);
            glVertex3d(vertices[faces[i].v[j] - 1].x - grabPoint[faces[i].g].x, vertices[faces[i].v[j] - 1].y  - grabPoint[faces[i].g].y, vertices[faces[i].v[j] - 1].z - grabPoint[faces[i].g].z);
        }

        glEnd();

        glPopMatrix();
    }

    glPopMatrix();
}

void Obj_3D::setHeadAng(double ang)
{
    ang_head = ang;
}

void Obj_3D::setLeftArmAng(double ang)
{
    ang_left_arm = ang;
}

void Obj_3D::setRightArmAng(double ang)
{
    ang_right_arm = ang;
}

void Obj_3D::setFootAng(double ang)
{
    ang_foot = ang;
}

double Obj_3D::getHeadAng()
{
    return ang_head;
}

double Obj_3D::getLeftArmAng()
{
    return ang_left_arm;
}

double Obj_3D::getRightArmAng()
{
    return ang_right_arm;
}

double Obj_3D::getFootAng()
{
    return ang_foot;
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
