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
            WHITE     = 7
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
        static void Run();

    public:
        static void RenderText(int __x, int __y, const char* __text, Color __colorF, Color __colorB);
};