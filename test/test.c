#define PUTC_HYPERCALL_ADDR 0x80000000
#define MEMSET_HYPERCALL_ADDR 0x80000004
#define EXIT_HYPERCALL_ADDR 0x80000008
#define MALLOC_HYPERCALL_ADDR 0x8000000C
#define FREE_HYPERCALL_ADDR 0x80000010
#define GET_STDIN_HYPERCALL_ADDR 0x80000014

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

void *malloc(unsigned long size)
{
    void *(*hypercall)(unsigned long) = (void *(*)(unsigned long))MALLOC_HYPERCALL_ADDR;
    return hypercall(size);
}

void free(void *ptr)
{
    void (*hypercall)(void *) = (void (*)(void *))FREE_HYPERCALL_ADDR;
    hypercall(ptr);
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
    char name[100] = {0};
    print("Hello, rv32 emulation!\n");
    print("Name: ");
    get_stdin(name, sizeof(name));
    print("Hello, ");
    print(name);
    putc('\n');

    return 0;
}