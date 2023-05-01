#define STB_IMAGE_IMPLEMENTATION


#include <iostream>
#include "image.h"
#include "wave.h"
#include "array2d.h"
#include <string>
int main() {
    std::cout << Image::GetPixelValue(0x10, 0x10, 0x10) << " ";
    std::cout << Image::GetPixelValue(0xa0, 0xa0, 0xa0) << " ";
    std::cout << Image::GetPixelValue(0xa0, 0x10, 0x10) << "\n";


    Array2D<int> I = GenerateMap("dungeon1.png", 3, 12, 12);

    Array2D<std::string> A(I.X, I.Y);
    for (int i = 0; i < A.len; i ++){
        switch (I.get(i)){
            case 1052688:  A.set(i, "."); break;
            case 10526880: A.set(i, "█"); break;
            case 1052832:  A.set(i, "◙"); break;
            default: break;
        }
    }

    A.out();
    


    // Array2D<std::string> B(20, 20);
    // for (int i = 0; i < B.len; i ++) B.set(i, "?");
    // if(Wave::Gen(A, 3, B))
    //     B.out();
}

