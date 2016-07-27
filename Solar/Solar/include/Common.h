#ifndef COMMON_H
#define COMMON_H

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <SOIL/SOIL.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <vector>

#define SOLAR_PLANET_NUMBERS 8

#define SOLAR_PLANET_SCALE 200
#define SOLAR_SUN_SCALE 40
#define SOLAR_AXIS_SCALE 50
#define SOLAR_AU 149600000

#define SOLAR_PLANET_MERCURY 0
#define SOLAR_PLANET_VENUS 1
#define SOLAR_PLANET_EARTH 2
#define SOLAR_PLANET_MARS 3
#define SOLAR_PLANET_JUPITER 4
#define SOLAR_PLANET_SATURN 5
#define SOLAR_PLANET_URANUS 6
#define SOLAR_PLANET_NEPTUNE 7

#define SOLAR_BRIGHTNESS_SUN 10
#define SOLAR_BRIGHTNESS_SKY 4
#define SOLAR_BRIGHTNESS_LIGHT 4

#endif