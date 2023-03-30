/*
MIT License

Copyright (c) 2023 NecroMax

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#ifndef ARRAY2D_H
#define ARRAY2D_H


#ifndef _LIBCPP_INITIALIZER_LIST
#include <initializer_list>
#endif // _LIBCPP_INITIALIZER_LIST

#ifndef _LIBCPP_ALGORITHM
#include <algorithm>
#endif // _LIBCPP_ALGORITHM


/** Two-dimentional array.
 * @author @maximalekseenko 
 * 
 * @tparam T type of element in array
 * @tparam X amount of columns in array
 * @tparam Y amount of rows in array
 * 
 * @see Array2D::x
 * @see Array2D::y
 * @see Array2D::get
 * @see Array2D::set
 */
template <typename T>
class Array2D {
    public: // ----- public Members -----
        /// @brief Amount of all elements in array2d.
        const int len;

        /// @brief Amount of cols in array2d.
        const int X;

        /// @brief Amount of rows in array2d.
        const int Y;


    private: // ----- private Members -----
        T *__values;

        int a;


    public: // ----- Constructors and Destructors -----
        Array2D(const Array2D<char> &obj) : Array2D(obj.X, obj.Y) {
            obj.clone(*this);
        }
        
        /**
         * @brief Constructs Array2D filled with default values for type T.
         */
        Array2D(int x, int y) : X(x), Y(y), len(x * y), __values(new T[x * y]) {}

        /** 
         * @brief Constructs Array2D and fills it with provided values in init.
         * 
         * @param init initializer list vith inital values.
         * 
         * @overload
         */
        Array2D(int x, int y, std::initializer_list<T> init) : X(x), Y(y), len(x * y), __values(new T[x * y]) {
            int i = 0;
            for (T v : init) __values[i++] = v;
        }

        /**
         * @brief Deconstructs Array2D
         */
        ~Array2D(){ delete[] __values; }

    public:  // ----- operator Overloads -----
        Array2D<T> &operator=(Array2D<T> obj){
            obj.clone(*this);
            return *this;
        }

        bool operator==(Array2D<T> &obj){
            for (int i = 0; i < len; i ++)
                if (__values[i] != obj.get(i)) return false;
            return true;
        }


    public: // ----- public Functions -----

        /** 
         * @brief Gets an element at i.
         * 
         * @param i index of an element.
         * 
         * @return element, stored in array.
         * 
         * @see Array2D::set
         */
        T& get(int i) const { return __values[i]; }

        /** 
         * @brief Gets an element at [x, y].
         * 
         * @param x collumn of an element.
         * @param y row of an element.
         * 
         * @return element, stored in array.
         * 
         * @overload
         * 
         * @see Array2D::set
         */
        T& get(int x, int y) const { return __values[x + y * X]; }

        /** 
         * @brief Sets an element at i to v.
         * 
         * @param i index of an element.
         * @param v new value of an element.
         * 
         * @see Array2D::get
         */
        void set(int i, T v) { __values[i] = v; }

        /** 
         * @brief Sets an element at [x, y] to v.
         * 
         * @param x collumn of an element.
         * @param y row of an element.
         * @param v new value of an element.
         * 
         * @overload
         * 
         * @see Array2D::get
         */
        void set(int x, int y, T v) { __values[x + y * X] = v; }

        /**
         * @brief Mirrors array elements.
         * 
         * @param axis Axis for mirroring:
         *      0 - horizontal,
         *      1 - vertical,
         *      2 - bottomleft-topright diagonal,
         *      3 - topleft-bottomright diagonal.
         * 
         * @throw If axis is diagonal and array's X is not equal to Y.
         * 
         * @see Array2D::mirrorred
         * 
         * @note For diagonal axis array's X MUST be equal to Y.
         */
        void mirror(int axis){
            if ((axis == 2 || axis == 3) && (X != Y)) throw;

            switch (axis) {
                case 0: // horizontal
                    for (int y = 0; y < Y / 2; y ++) // other half already mirrored
                        for (int x = 0; x < X; x ++) // swap [x, y] with [x, -y]
                            std::swap(__values[x + y * X], __values[x + (Y - 1 - y) * X]);
                    break;

                case 1: // vertical
                    for (int x = 0; x < X / 2; x ++) // other half already mirrored
                        for (int y = 0; y < Y; y ++) // swap [x, y] with [-x, y]
                            std::swap(__values[x + y * X], __values[(X - 1 - x) + y * X]);
                    break;

                case 2: // bottomleft-topright diagonal
                    for (int y = 0; y < Y; y ++) // swap [x, y] with [-y, -x]
                        for (int x = 0; x < X - y - 1; x ++) // other half already mirrored
                            std::swap(__values[x + y * X], __values[(Y - 1 - y) + (X - 1 - x) * X]);
                    break;

                case 3: // topleft-bottomright diagonal
                    for (int y = Y - 1; y > 0; y --) // other half already mirrored
                        for (int x = 0; x < X - y; x ++) // swap [x, y] with [y, x]
                            std::swap(__values[x + y * X], __values[y + x * X]);
                    break;
                
                default: throw;
            }
        };

        /**
         * @brief 
         * 
         * @param axis Axis for mirroring:
         *      0 - horizontal,
         *      1 - vertical,
         *      2 - bottomleft-topright diagonal,
         *      3 - topleft-bottomright diagonal.
         * 
         * @return Mirrorred array.
         * 
         * @throw If axis is diagonal and array's X is not equal to Y.
         * 
         * @see Array2D::mirror
         * 
         * @note For diagonal axis array's X MUST be equal to Y.
         */
        Array2D mirrorred(int axis){
            Array2D newArray = clone();
            newArray.mirror(axis);
            return newArray;
        };

        /**
         * @brief Rotate array clockwise
         * 
         * @param amount amount of rotations, can be negative
         * 
         * @see Array2D::rotated
         */
        void rotate(int amount){
            if (X != Y) throw;
            // optimize by rotation amount
            if (amount >= 4) amount %= 4;
            if (amount < 0) amount = (amount % 4 + 4) % 4;


            int capY = Y % 2 == 0 ? Y / 2 : Y / 2 + 1; // half w center
            int capX = X % 2 == 0 ? X / 2 : X / 2 + 1; // half w/o center(handeled in Y)

            switch (amount){
            case 0: break;

            case 1:
                for (int y = 0; y < capY; y ++)
                    for (int x = 0; x < capX; x ++){
                        std::swap(
                            __values[x           + y           * X],  // 1
                            __values[(X - 1 - y) + x           * X]); // 2
                        std::swap(
                            __values[y           + (Y - 1 - x) * X],  // 3
                            __values[(X - 1 - x) + (Y - 1 - y) * X]); // 4
                        std::swap(
                            __values[x           + y           * X],  // 1
                            __values[(X - 1 - x) + (Y - 1 - y) * X]); // 4
                } break;

            case 2:
                for (int y = 0; y < capY; y ++)
                    for (int x = 0; x < capX; x ++){
                        std::swap(
                            __values[x           + y           * X],  // 1
                            __values[(X - 1 - x) + (Y - 1 - y) * X]); // 4
                        std::swap(
                            __values[(X - 1 - y) + x           * X],  // 2
                            __values[y           + (Y - 1 - x) * X]); // 3
                        std::swap(
                            __values[x           + y           * X],  // 1
                            __values[(X - 1 - y) + x           * X]); // 2
                } break;

            case 3:
                for (int y = 0; y < capY; y ++)
                    for (int x = 0; x < capX; x ++){
                        std::swap(
                            __values[x           + y           * X],  // 1
                            __values[y           + (Y - 1 - x) * X]); // 3
                        std::swap(
                            __values[(X - 1 - y) + x           * X],  // 2
                            __values[(X - 1 - x) + (Y - 1 - y) * X]); // 4
                        std::swap(
                            __values[x           + y           * X],  // 1
                            __values[(X - 1 - x) + (Y - 1 - y) * X]); // 4
                } break;
            
            default:
                break;
            }
        };

        /**
         * @brief Clones this array.
         * 
         * @return Clonned array.
         * 
         * @warning If array values are pointers, function will clone pointers, not data where they point.s
         */
        Array2D clone() const {
            Array2D<T> newArray(X, Y);
            for (int i = 0; i < len; i ++) newArray.set(i, get(i)); 
            return newArray;
        };

        /**
         * @brief Clones this array.
         * 
         * @param[out] Array to clone to.
         * 
         * @overload
         * 
         * @warning If array values are pointers, function will clone pointers, not data where they point.s
         */
        void clone(Array2D<T> &cloneto) const {
            for (int i = 0; i < len; i ++) cloneto.set(i, get(i)); 
        };

        #ifdef _LIBCPP_IOSTREAM
        /** 
         * @brief Output array using std::cout.
         * 
         * @note Requires iostream to be included BEFORE array2d.h.
         */
        void out() {
            for (int y = 0; y < Y; y ++){
                for (int x = 0; x < X; x ++)
                    std::cout << __values[x + y * X];
                std::cout << std::endl;
            }
        }
        #endif // _LIBCPP_IOSTREAM
};


#endif // ARRAY2D_H