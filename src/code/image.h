#pragma once
#include "stb_image.h"

#include "array2d.h"

#include <vector>


class Image {
    public:
        static int* ParseImage(const char *filename, int& width, int& height) {
            int channels;
            unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);

            if (image == NULL) throw;

            int* parsedImage = new int[width * height];

            for (int iImage = 0; iImage < width * height; iImage ++) {
                int r = static_cast<int>(image[channels * iImage + 0]);
                int g = static_cast<int>(image[channels * iImage + 1]);
                int b = static_cast<int>(image[channels * iImage + 2]);
                    
                parsedImage[iImage] = GetPixelValue(r, g, b);
            }

            stbi_image_free(image);

            return parsedImage;
        }


        static Array2D<int> ParseImage(const char *filename) {
            int width, height, channels;
            unsigned char* image = stbi_load(filename, &width, &height, &channels, 0);

            if (image == NULL) throw;

            Array2D<int> return_array(width, height);

            for (int x = 0; x < width; x ++) {
                for (int y = 0; y < height; y ++) {
                    int r = static_cast<int>(image[channels * (x + y * width) + 0]);
                    int g = static_cast<int>(image[channels * (x + y * width) + 1]);
                    int b = static_cast<int>(image[channels * (x + y * width) + 2]);
                    
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