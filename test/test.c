#define PUTC_HYPERCALL_ADDR 0x80000000
#define MEMSET_HYPERCALL_ADDR 0x80000004
#define EXIT_HYPERCALL_ADDR 0x80000008
#define GET_STDIN_HYPERCALL_ADDR 0x8000000C
#define INIT_WINDOW_HYPERCALL_ADDR 0x80000010
#define CLOSE_WINDOW_HYPERCALL_ADDR 0x80000014
#define SET_TARGET_FPS_HYPERCALL_ADDR 0x80000018
#define DRAW_FRAME_HYPERCALL_ADDR 0x8000001C
#define FRAMEBUFFER_ADDRESS 0x00040000

volatile unsigned int *framebuffer = (volatile unsigned int *)FRAMEBUFFER_ADDRESS;

int putc(char c)
{
    void (*hypercall)(char) = (void (*)(char))PUTC_HYPERCALL_ADDR;
    hypercall(c);
    return 0;
}

int get_stdin(char *buffer, unsigned long max_length)
{
    int (*hypercall)(char *, unsigned long) = (int (*)(char *, unsigned long))GET_STDIN_HYPERCALL_ADDR;
    return hypercall(buffer, max_length);
}

unsigned long strlen(const char *str)
{
    unsigned long len = 0;
    while (str[len] != '\0')
    {
        len++;
    }
    return len;
}

void print(const char *str)
{
    void (*hypercall)(char) = (void (*)(char))PUTC_HYPERCALL_ADDR;

    unsigned long len = strlen(str);
    for (unsigned long i = 0; i < len; i++)
    {
        hypercall(str[i]);
    }
}

int memset(void *dest, unsigned char value, unsigned long length)
{
    int (*hypercall)(void *, unsigned char, unsigned long) = (int (*)(void *, unsigned char, unsigned long))MEMSET_HYPERCALL_ADDR;
    return hypercall(dest, value, length);
}

void exit()
{
    void (*hypercall)() = (void (*)())EXIT_HYPERCALL_ADDR;
    hypercall();
}

void init_window()
{
    void (*hypercall)() = (void (*)())INIT_WINDOW_HYPERCALL_ADDR;
    hypercall();
}

void close_window()
{
    void (*hypercall)() = (void (*)())CLOSE_WINDOW_HYPERCALL_ADDR;
    hypercall();
}

void set_target_fps(int fps)
{
    void (*hypercall)(int) = (void (*)(int))SET_TARGET_FPS_HYPERCALL_ADDR;
    hypercall(fps);
}

void draw_frame()
{
    void (*hypercall)() = (void (*)())DRAW_FRAME_HYPERCALL_ADDR;
    hypercall();
}

int main()
{
    char name[100] = {0};
    print("Hello, rv32 emulation!\n");
    print("Name: ");
    get_stdin(name, sizeof(name));
    print("Hello, ");
    print(name);
    putc('\n');

    init_window();
    float blue_level = 0.0f;
    float direction = 0.01f;

    while (1)
    {
        blue_level += direction;
        if (blue_level > 1.0f || blue_level < 0.0f)
        {
            direction = -direction;
        }

        for (int i = 0; i < 256 * 256; i++)
        {
            int x = i & 0xFF;
            int y = i >> 8;

            float r_float = (float)x / 255.0f;
            float g_float = (float)y / 255.0f;
            float b_float = blue_level;

            unsigned int r = (unsigned int)(r_float * 255.0f);
            unsigned int g = (unsigned int)(g_float * 255.0f);
            unsigned int b = (unsigned int)(b_float * 255.0f);

            framebuffer[i] = 0xFF000000 | (b << 16) | (g << 8) | r;
        }

        draw_frame();
    }

    return 0;
}