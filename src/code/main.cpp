#include <iostream>
#include "random.h"
#include "wave.h"


// image lib
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


int main()
{
    int* PATTERN = new int[16]{
        0, 1, 0, 0,
        0, 1, 0, 0,
        1, 1, 1, 1,
        0, 1, 0, 0};
    // Wave(12, 12, 4, 4, 3, PATTERN);
    Wave(12, 12, 3, 4, 4, PATTERN);

}
