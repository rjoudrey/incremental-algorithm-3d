#include "Color.h"
#include "Math.h"

#ifndef LIGHT_H
#define LIGHT_H

class Light  {
public:
	Light();
	Color ambientColor, diffuseColor, specularColor;
	Vec3f position;
};

#endif
