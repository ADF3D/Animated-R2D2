#include "water_object.h"

Water::Water(GLuint Itexture, bool IisAnimed)
{
    texture = Itexture;
    isAnimed = IisAnimed;
}

GLuint Water::getTexture()
{
    return texture;
}

double Water::getAnim()
{
    anim += 0.5;

    return anim;
}

Water::~Water()
{

}
