#define STB_IMAGE_IMPLEMENTATION


#include <typeinfo>
#include <iostream>
#include <type_traits>
// #include "lib/image.h"
// #include "lib/wave.h"
#include "lib/array2d.h"
#include <vector>
int main() {

    // int AA[16] = 
    Array2D<char> A(4, 4, {
        '0','1','2','3',
        '4','5','6','7',
        '8','9','a','b',
        'c','d','e','f'});

    
    
    // out original
    A.out();
    std::cout << std::endl;

    // rotate once
    A.rotate(1);
    A.out();
    std::cout << std::endl;

    // rotate back and mirror
    A.rotate(-1);
    A.mirror(2);
    A.out();
    std::cout << std::endl;

    auto B(A);

    // make vector
    // std::vector<Array2D<char>> B;
    // B.push_back(A.clone());


    // auto A = Image::ParseImage("dungeon1.png");

    // // A.out();

    // Array2D<int> B(25,25);
    // Wave::Gen<int>(A, &B, 4);


    // for (int y = 0; y < A.Y; y ++){
    //     for (int x = 0; x < A.X; x ++){
    //         int value = A.get(x, y);
    //         if (value == 1184016) std::cout << '.';
    //         if (value == 10658208) std::cout << '#';
    //     } std::cout << std::endl;
    // }


    // auto result = Wave::Gen(image, 4, 10, 10);
}