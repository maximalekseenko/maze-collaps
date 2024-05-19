#ifndef __MAP_READER_H
#define __MAP_READER_H

#include "./mapdata.h"

namespace Reader
{
    Mapdata* GetMapdataFromFile(const char* __mapdataFilePath);

    /// @brief Reads pixel data from image.
    /// @param __image_path Path to the image.
    /// @param __out_image_data Reference to pointer on array, where image data will be stored.
    /// @param __out_image_width Reference to where to store image width.
    /// @param __out_image_height Reference to where to store image height.
    void ReadImage(const char* __imagePath, int*& __outImageData, int& __outImageWidth, int& __outImageHeight);

    /// @brief Returns hash code of a pixel, based on it's color
    /// @param __r Pixel color value on red chanel.
    /// @param __g Pixel color value on green chanel.
    /// @param __b Pixel color value on blue channel.
    /// @return Number, that represents this pixel's unique color.
    int GetPixelValue(int __r, int __g, int __b);
}

#endif // __MAP_READER_H