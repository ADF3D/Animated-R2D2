#ifndef OBJ_3D_H

    #define OBJ_3D_H

    #include <cstdlib>
    #include <cstdio>
    #include <cstring>
    #include <vector>
    #include <map>
    #include <string>
    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <cmath>

    #include "sdlglutils.h"

    /// POSITIONNING STRUCTS
    typedef struct
    {
        double x;
        double y;
        double z;
    }Pos3D;

    typedef struct
    {
        double x;
        double y;
    }Pos2D;

    /// FACES STRUCT
    typedef struct
    {
        unsigned int v[5];
        unsigned int t[5];
        unsigned int n[5];
        unsigned int cnt;
        char g[100];
        GLuint tex;
    }Face_3D;

    using namespace std;

    class Obj_3D
    {
        protected:
            vector <Pos3D> vertices;
            vector <Pos2D> texPos;
            vector <Pos3D> normals;
            vector <Face_3D> faces;
            map <string, Pos3D> grabPoint;
            Pos3D pos;
            double ang_head;
            double ang_left_arm;
            double ang_right_arm;
            double ang_foot;

        public:
            Obj_3D();
            Obj_3D(char *filename);

            bool loadUsingFile(char *filename);
            void draw();

            vector <Pos3D> getVertices();
            vector <Pos2D> getTexPos();
            vector <Pos3D> getNormals();
            vector <Face_3D> getFaces();
            void setFaces(vector <Face_3D> _f);
            void setVertices(vector <Pos3D> _v);
            void setTexPos(vector <Pos2D> _t);
            void setNormals(vector <Pos3D> _n);
            void setHeadAng(double ang);
            void setLeftArmAng(double ang);
            void setRightArmAng(double ang);
            void setFootAng(double ang);
            double getHeadAng();
            double getLeftArmAng();
            double getRightArmAng();
            double getFootAng();

            ~Obj_3D();
    };

#endif
