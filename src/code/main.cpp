#define STB_IMAGE_IMPLEMENTATION


#include <iostream>
#include "lib/image.h"
#include "lib/wave.h"
int main() {
    std::cout << Image::GetPixelValue(0x10, 0x10, 0x10) << " ";
    std::cout << Image::GetPixelValue(0xa0, 0xa0, 0xa0) << " ";
    std::cout << Image::GetPixelValue(0xa0, 0x10, 0x10) << "\n";

    auto A = Image::ParseImage("dungeon4.png");
    Array2D<int> B(25, 25);
    if(Wave::Gen(A, 4, B))
        for (int y = 0; y < B.Y; y ++){
            for (int x = 0; x < B.X; x ++){
                int value = B.get(x, y);
                if (value == 1052688) std::cout << '.';
                if (value == 10526880) std::cout << "█";
                if (value == 1052832) std::cout << "◙";
                // std::cout << value << " ";
            } std::cout << std::endl;
        }
}

