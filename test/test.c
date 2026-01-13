#define MEMSET_HYPERCALL_ADDR 0xFFFFFFF0
#define PUTC_HYPERCALL_ADDR 0xFFFFFFF4
#define EXIT_HYPERCALL_ADDR 0xFFFFFFF8

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

int memset(void *dest, int value, unsigned long length)
{
    int (*hypercall)(void *, int, unsigned long) = (int (*)(void *, int, unsigned long))MEMSET_HYPERCALL_ADDR;
    return hypercall(dest, value, length);
}

void exit()
{
    void (*hypercall)() = (void (*)())EXIT_HYPERCALL_ADDR;
    hypercall();
}

int main()
{
    print("Hello, rv32 emulation!\n");

    return 0;
}