#ifndef LISTOFCITIES_H
#define LISTOFCITIES_H

#include <stddef.h>

typedef struct { const char *city; const char *tz; } CityTZ;

extern const CityTZ cities[];

#endif