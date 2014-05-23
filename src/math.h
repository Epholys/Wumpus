#ifndef MATH_H
#define MATH_H

#include<cstdlib>

int randint(int max) { return rand()%max; }

int randint(int min, int max) { return randint(max-min)+min; }


#endif
