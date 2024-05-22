#include "reader.h"

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

#include <algorithm>
#include <string>
#include "utility/log.h"

std::vector<Mapdata *> hashLoadedMapdatas;

Mapdata *Reader::GetMapdata(const char *__mapdataPath)
{
    // find if mapdata is already hashed
    auto _hashLoadedMapdatasIterator = std::find_if(
        hashLoadedMapdatas.begin(),
        hashLoadedMapdatas.end(),
        [__mapdataPath](Mapdata *_hashedMapdata)
        { return std::strcmp(__mapdataPath, _hashedMapdata->path); });
    if (_hashLoadedMapdatasIterator != hashLoadedMapdatas.end())
        return *_hashLoadedMapdatasIterator.base();

    // load mapdata
    return Reader::LoadMapdata(__mapdataPath);
}

Mapdata *Reader::LoadMapdata(const char *__mapdataPath)
{
    Mapdata *_newMapdataToLoad = new Mapdata(__mapdataPath);

    // get raw data for mapdata
    /// @note _rawImageData is cleared after setting patterns in mapdata.
    int *_rawImageData, _rawImageWidth, _rawImageHeight;
    Reader::ReadImage(__mapdataPath, _rawImageData, _rawImageWidth, _rawImageHeight);

    // define and process mapdata
    _newMapdataToLoad->SetPatterns(_rawImageData, _rawImageWidth, _rawImageHeight);

    // clear
    delete[] _rawImageData;

    // delete existing mapdata
    auto _hashLoadedMapdatasIterator = std::find_if(
        hashLoadedMapdatas.begin(),
        hashLoadedMapdatas.end(),
        [__mapdataPath](Mapdata *_hashedMapdata)
        { return std::strcmp(__mapdataPath, _hashedMapdata->path); });
    if (_hashLoadedMapdatasIterator != hashLoadedMapdatas.end())
        Reader::UnloadMapdata(__mapdataPath);

    // hash new
    hashLoadedMapdatas.push_back(_newMapdataToLoad);

    // return
    return _newMapdataToLoad;
}

void Reader::UnloadMapdata(const char *__mapdataPath)
{
    // find position
    auto _hashLoadedMapdatasIterator = std::find_if(
        hashLoadedMapdatas.begin(),
        hashLoadedMapdatas.end(),
        [__mapdataPath](Mapdata *_hashedMapdata)
        { return std::strcmp(__mapdataPath, _hashedMapdata->path); });

    // check that target mapdata is found
    if (_hashLoadedMapdatasIterator == hashLoadedMapdatas.end())
    {
        Log::Out("Reader unloads mapdata that does not exist.", true);
        return;
    }

    // delete
    if (_hashLoadedMapdatasIterator != hashLoadedMapdatas.end())
    {
        delete *_hashLoadedMapdatasIterator.base();
        hashLoadedMapdatas.erase(_hashLoadedMapdatasIterator);
    }
}

int Reader::GetPixelValue(int __r, int __g, int __b) { return __r + (__g << 8) + (__b << 16); }

void Reader::ReadImage(const char *__imagePath, int *&__outImageData, int &__outImageWidth, int &__outImageHeight)
{
    // read image
    int _rawImageChannels;
    unsigned char *_rawImage = stbi_load(__imagePath, &__outImageWidth, &__outImageHeight, &_rawImageChannels, 0);
    if (_rawImage == NULL)
        Log::Out("Reader got an invalid image.", true);

    // set up output array
    __outImageData = new int[__outImageWidth * __outImageHeight];

    // convert pixels
    for (int _imageI = 0; _imageI < __outImageWidth * __outImageHeight; _imageI++)
    {
        int r = static_cast<int>(_rawImage[_rawImageChannels * _imageI + 0]);
        int g = static_cast<int>(_rawImage[_rawImageChannels * _imageI + 1]);
        int b = static_cast<int>(_rawImage[_rawImageChannels * _imageI + 2]);

        __outImageData[_imageI] = GetPixelValue(r, g, b);
    }

    // clear
    stbi_image_free(_rawImage);
}