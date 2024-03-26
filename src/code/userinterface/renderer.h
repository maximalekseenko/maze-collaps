#pragma once


class Renderer {
    public:
        enum Color {
            BLACK     = 0,
            RED       = 1,
            GREEN     = 2,
            YELLOW    = 3,
            BLUE      = 4,
            MAGENTA   = 5,
            CYAN      = 6,
            WHITE     = 7,
            BRIGHT_BLACK     = 8,
            BRIGHT_RED       = 9,
            BRIGHT_GREEN     = 10,
            BRIGHT_YELLOW    = 11,
            BRIGHT_BLUE      = 12,
            BRIGHT_MAGENTA   = 13,
            BRIGHT_CYAN      = 14,
            BRIGHT_WHITE     = 15
        };
        #define RENDERER_COLOR_AMOUNT 8
        
    public: // +++ INITIALIZATION +++ //
        static void Init();
    private:
        static bool initialized;
        static void InitColors();

    public: // +++ UPDATING / RENDERING +++ //
        static void CallForUpdate(int __hash);
    private:
        static int update_hash;
        static void Update();

    public:
        static bool is_running;
        static void Run();

    public:
        static void RenderText(int __x, int __y, const char* __text, Color __colorF, Color __colorB);
};