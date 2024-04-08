#ifndef __ENGINE_COLOR_H
#define __ENGINE_COLOR_H



/// @brief Numbers for colors.
enum Color {
    BLACK     = 0, BRIGHT_BLACK     = 8,
    RED       = 1, BRIGHT_RED       = 9,
    GREEN     = 2, BRIGHT_GREEN     = 10,
    YELLOW    = 3, BRIGHT_YELLOW    = 11,
    BLUE      = 4, BRIGHT_BLUE      = 12,
    MAGENTA   = 5, BRIGHT_MAGENTA   = 13,
    CYAN      = 6, BRIGHT_CYAN      = 14,
    WHITE     = 7, BRIGHT_WHITE     = 15
};



/// @brief Converts combination of font color and background color into pair's id.
/// @param __colorF Font color pair's part.
/// @param __colorB Background color pair's part.
/// @return Id of paired colors.
inline int GetColorPairId(int __colorF, int __colorB)
{
    // color pair is "Bbbbffff" byte
    // where B is font's boldness
    //     bbb is font's number
    //    ffff is background's number
    
    //      B          bbb                     ffff
    return ((1 << 7) | ((7 & __colorB) << 4) | (7 & __colorF));
}



#endif // __ENGINE_COLOR_H