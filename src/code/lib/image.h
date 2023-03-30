#pragma once
#include "stb_image.h"

#include <vector>

#include "array2d.h"



class Image {
    public:
        static Array2D<int> ParseImage(const char *filename) {
            int width, height, channels;
            unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);

            if (image == NULL) throw;

            Array2D<int> return_array(width, height);

            for (int x = 0; x < width; x ++) {
                for (int y = 0; y < height; y ++) {
                    int r = static_cast<int>(image[channels * (x + y * width)] + 0);
                    int g = static_cast<int>(image[channels * (x + y * width)] + 1);
                    int b = static_cast<int>(image[channels * (x + y * width)] + 2);
                    
                    return_array.set(x, y, GetPixelValue(r, g, b));
                }
            }

            stbi_image_free(image);

            return return_array;
        }


        static int GetPixelValue(int r, int g, int b) {
            return r + g*256 + b*256*256;
        }
};