#include "FreeFlyCamera_object.h"

FreeFlyCamera::FreeFlyCamera(const Vector3D & position)
{
    _position = position; //si aucune position n'est définie on reçoit quand même (0,0,0) en paramètre
    _phi = 0;
    _theta = 0;
    VectorsFromAngles(); //décrit un peu plus loin

    _speed = 0.01;
    _sensivity = 0.2;
    _verticalMotionActive = false;

    //Initialisation de la configuration des touches
    _keyconf["forward"] = SDLK_w;
    _keyconf["backward"] = SDLK_s;
    _keyconf["strafe_left"] = SDLK_q;
    _keyconf["strafe_right"] = SDLK_d;
    _keyconf["boost"] = SDLK_LSHIFT;

    //Initialisation des KeyStates
    _keystates[_keyconf["forward"]] = false;
    _keystates[_keyconf["backward"]] = false;
    _keystates[_keyconf["strafe_left"]] = false;
    _keystates[_keyconf["strafe_right"]] = false;
    _keystates[_keyconf["boost"]] = false;

    // empeche la sortie de la souris de l'ecran et gere le deplacement relatif
    SDL_WM_GrabInput(SDL_GRAB_ON);
    SDL_ShowCursor(SDL_DISABLE);
}

void FreeFlyCamera::OnMouseMotion(const SDL_MouseMotionEvent & event)
{
    // soustraction car angles en radians
    _theta -= event.xrel*_sensivity;
    _phi -= event.yrel*_sensivity;
    VectorsFromAngles();
    printf("%.2lf, %.2lf\n", _theta, _phi);
}

void FreeFlyCamera::VectorsFromAngles()
{
    static const Vector3D up(0,0,1); //une constante, le vecteur vertical du monde, utilisé dans les calculs
//On limite les valeurs de _phi, on vole certes, mais on en fait pas de loopings :p
    if (_phi > 89)
        _phi = 89;
    else if (_phi < -89)
        _phi = -89;
//passage des coordonnées sphériques aux coordonnées cartésiennes
    double r_temp = cos(_phi*M_PI/180);
    _forward.z = sin(_phi*M_PI/180);
    _forward.x = r_temp*cos(_theta*M_PI/180);
    _forward.y = r_temp*sin(_theta*M_PI/180);
//diantre mais que fait ce passage ?
    _left.x = up.y*_forward.z - _forward.y*up.z;
    _left.y = _forward.x*up.z - up.x*_forward.z;
    _left.z = up.x*_forward.y - up.y*_forward.x;
    //_left = up.crossProduct(_forward);
    _left.normalize();

//avec la position de la caméra et la direction du regard, on calcule facilement ce que regarde la caméra (la cible)
    _target = _position + _forward;
}

void FreeFlyCamera::OnMouseButton(const SDL_MouseButtonEvent & event)
{
    if ((event.button == SDL_BUTTON_WHEELUP)&&(event.type == SDL_MOUSEBUTTONDOWN)) //coup de molette vers le haut
    {
        _verticalMotionActive = true; //on demande à activer le mouvement vertical
        _timeBeforeStoppingVerticalMotion = 250; //pendant 250 ms
        _verticalMotionDirection = 1; //et vers le haut

    }
    else if ((event.button == SDL_BUTTON_WHEELDOWN)&&(event.type == SDL_MOUSEBUTTONDOWN)) //coup de molette vers le bas
    {
        _verticalMotionActive = true; //on demande à activer le mouvement vertical
        _timeBeforeStoppingVerticalMotion = 250; //pendant 250 ms
        _verticalMotionDirection = -1; //et vers le bas
    }
}

void FreeFlyCamera::OnKeyboard(const SDL_KeyboardEvent & event)
{
//on parcourt tous les keystates actuels
    for (KeyStates::iterator it = _keystates.begin();it != _keystates.end();
            it++)
    {
        if (event.keysym.sym == it->first) //est-ce que la touche responsable de l'événement est celle du keystate ?
        {
            it->second = (event.type == SDL_KEYDOWN); //true si enfoncé, false si relâché
            break; //la touche responsable de l'événement a été utilisée, on quitte le for
        }
    }
}

void FreeFlyCamera::animate(Uint32 timestep)
{
//la vitesse réelle du déplacement est soit la vitesse de croisière, soit 10*la vitesse, en fonction
//de l'état enfoncé ou non de la touche correspondant à l'action "boost"
    double realspeed = (_keystates[_keyconf["boost"]])?10*_speed:_speed;
    if (_keystates[_keyconf["forward"]])
        _position += _forward * (realspeed * timestep); //on avance
    if (_keystates[_keyconf["backward"]])
        _position -= _forward * (realspeed * timestep); //on recule
    if (_keystates[_keyconf["strafe_left"]])
        _position += _left * (realspeed * timestep); //on se déplace sur la gauche
    if (_keystates[_keyconf["strafe_right"]])
        _position -= _left * (realspeed * timestep); //on se déplace sur la droite
    if (_verticalMotionActive)
    {
        if (timestep > _timeBeforeStoppingVerticalMotion)
            _verticalMotionActive = false;
        else
            _timeBeforeStoppingVerticalMotion -= timestep;
        _position += Vector3D(0,0,_verticalMotionDirection*realspeed*timestep); //on monte ou on descend, en fonction de la valeur de _verticalMotionDirection
    }
    _target = _position + _forward; //comme on a bougé, on recalcule la cible fixée par la caméra

}

void FreeFlyCamera::look()
{
    gluLookAt(_position.x,_position.y,_position.z,
              _target.x,_target.y,_target.z,
              0,0,1);
}

void FreeFlyCamera::setSpeed(double speed)
{
    _speed = speed;
}

void FreeFlyCamera::setSensivity(double sensivity)
{
    _sensivity = sensivity;
}

void FreeFlyCamera::setPosition(const Vector3D & position)
{
    _position = position;
}

FreeFlyCamera::~FreeFlyCamera()
{
    SDL_WM_GrabInput(SDL_GRAB_OFF);
    SDL_ShowCursor(SDL_ENABLE);
}
