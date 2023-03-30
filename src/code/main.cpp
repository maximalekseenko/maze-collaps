#define STB_IMAGE_IMPLEMENTATION


#include <iostream>
#include "lib/image.h"
// #include "wave.h"
int main() {

    auto A = Image::ParseImage("dungeon1.png");

    A.out();

    // auto B = Wave::Gen(A, B, 4)


    for (int y = 0; y < A.Y; y ++){
        for (int x = 0; x < A.X; x ++){
            int value = A.get(x, y);
            if (value == 1184016) std::cout << '.';
            if (value == 10658208) std::cout << '#';
        } std::cout << std::endl;
    }


    // auto result = Wave::Gen(image, 4, 10, 10);
}