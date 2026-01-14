#include "macros.hpp"

#include <cstdint>
#include <raylib.h>
#include "pow.hpp"

static constexpr int32_t TEXTURE_ADDRESS = ipow(2, 18); // 256 KB (0x40000)
static constexpr const size_t FB_WIDTH = 256;
static constexpr const size_t FB_HEIGHT = 256;

static constexpr const size_t WINDOW_WIDTH = 512;
static constexpr const size_t WINDOW_HEIGHT = 512;
static constexpr const char *title = "ZRV32 Emulator";

static Texture2D framebuffer_texture;

int32_t rv_init_window(CPU &cpu)
{
    SetTraceLogLevel(LOG_NONE);
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title);

    Image framebuffer_image = GenImageColor(FB_WIDTH, FB_HEIGHT, BLACK);
    ImageFormat(&framebuffer_image, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    framebuffer_texture = LoadTextureFromImage(framebuffer_image);
    UnloadImage(framebuffer_image);

    SetTextureFilter(framebuffer_texture, TEXTURE_FILTER_POINT);

    return 0;
}

int32_t rv_close_window(CPU &cpu)
{
    UnloadTexture(framebuffer_texture);
    CloseWindow();
    return 0;
}

int32_t rv_set_target_fps(CPU &cpu)
{
    int32_t fps = static_cast<int32_t>(cpu.get_reg(CPU::REG_X10));
    SetTargetFPS(fps);
    return 0;
}

int32_t rv_draw_frame(CPU &cpu)
{
    if (WindowShouldClose())
    {
        std::exit(0);
    }

    uint8_t *framebuffer_ptr = cpu.memory_ptr<uint8_t>(TEXTURE_ADDRESS);
    UpdateTexture(framebuffer_texture, framebuffer_ptr);

    BeginDrawing();
    ClearBackground(BLACK);

    DrawTexturePro(
        framebuffer_texture,
        Rectangle{0.0f, 0.0f, static_cast<float>(FB_WIDTH), static_cast<float>(FB_HEIGHT)},
        Rectangle{0.0f, 0.0f, static_cast<float>(WINDOW_WIDTH), static_cast<float>(WINDOW_HEIGHT)},
        Vector2{0.0f, 0.0f},
        0.0f,
        WHITE);

    EndDrawing();
    return 0;
}

HYPERCALL(0x80000010, rv_init_window)
HYPERCALL(0x80000014, rv_close_window)
HYPERCALL(0x80000018, rv_set_target_fps)
HYPERCALL(0x8000001C, rv_draw_frame)