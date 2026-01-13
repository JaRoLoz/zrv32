ENTRY(_start)

MEMORY
{
    RAM (rwx) : ORIGIN = 0x00000000, LENGTH = 1M
}

SECTIONS
{
    .text : {
        KEEP(*(.text.entry)) 
        *(.text)
        *(.text.*)
    } > RAM

    .data : {
        *(.data)
    } > RAM

    .bss : {
        _bss_start = .;
        *(.bss)
        *(COMMON)
        _bss_end = .;
    } > RAM

    /* ORIGIN(RAM) + LENGTH(RAM) = 0 + 1MB = 0x100000 */
    /* Align to 16 bytes, will be important for the future */
    _stack_top = ORIGIN(RAM) + LENGTH(RAM) - 16;
}