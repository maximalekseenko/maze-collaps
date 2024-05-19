#include "reader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

#include "utility/log.h"

// Mapdata* GetMapdataFromFile(const char* __mapdataFilePath)
// {
//     return
// }


int Reader::GetPixelValue(int __r, int __g, int __b)
{
    return __r + (__g << 8) + (__b << 16);
}

void Reader::ReadImage(const char *__imagePath, int *&__outImageData, int &__outImageWidth, int &__outImageHeight)
{
    // read image
    int _rawImageChannels;
    unsigned char *_rawImage = stbi_load(__imagePath, &__outImageWidth, &__outImageHeight, &_rawImageChannels, 0);
    if (_rawImage == NULL)
        Log::Out("Reader got an invalid image.", true);

    // set up output array
    if (__outImageData != nullptr)
        delete[] __outImageData;
    __outImageData = new int[__outImageWidth * __outImageHeight];

    // convert pixels
    for (int _iImage = 0; _iImage < __outImageWidth * __outImageHeight; _iImage++)
    {
        int r = static_cast<int>(_rawImage[_rawImageChannels * _iImage + 0]);
        int g = static_cast<int>(_rawImage[_rawImageChannels * _iImage + 1]);
        int b = static_cast<int>(_rawImage[_rawImageChannels * _iImage + 2]);

        __outImageData[_iImage] = GetPixelValue(r, g, b);
    }

    // clear
    stbi_image_free(_rawImage);
}