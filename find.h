#ifndef FIND_H
#define FIND_H

#include "listofcities.h"

const CityTZ *find_city(const char *query, int *out_dist);
void strip_spaces(const char *src, char *dst, size_t max);
int parse_offset(const char *query, int *out_hours);

#endif