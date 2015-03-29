#ifndef FREEFLY

    #define FREEFLY

    #include <SDL.h>

    #include <GL/gl.h>
    #include <GL/glu.h>
    #include <map>
    #include <string>
    #include "vector3d.h"

    #include <cmath>

    #include "constantes.h"
    #include "sdlglutils.h"

    class FreeFlyCamera
    {
        public:

            FreeFlyCamera(const Vector3D & position = Vector3D(0,0,0));

            virtual void OnMouseMotion(const SDL_MouseMotionEvent & event);
            virtual void OnMouseButton(const SDL_MouseButtonEvent & event);
            virtual void OnKeyboard(const SDL_KeyboardEvent & event);

            virtual void animate(Uint32 timestep);
            virtual void setSpeed(double speed);
            virtual void setSensivity(double sensivity);

            virtual void setPosition(const Vector3D & position);

            virtual void look();

            virtual ~FreeFlyCamera();

        protected:

            double _speed;
            double _sensivity;

            Uint32 _timeBeforeStoppingVerticalMotion;
            bool _verticalMotionActive;
            int _verticalMotionDirection;

            typedef std::map<SDLKey,bool> KeyStates;
            KeyStates _keystates;
            typedef std::map<std::string,SDLKey> Keyconf;
            Keyconf _keyconf;

            Vector3D _position;
            Vector3D _target;
            Vector3D _forward;
            Vector3D _left;
            double _theta;
            double _phi;

            void VectorsFromAngles();
    };

#endif
