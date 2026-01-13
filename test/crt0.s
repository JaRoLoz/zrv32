.section .text.entry
.global _start

_start:
    /* initialize stack pointer */
    la sp, _stack_top

    /* clean bss using the memset hypercall */
    la a0, _bss_start
    la t0, _bss_end
    sub a2, t0, a0

    /* if the length is 0, jump to main */
    beqz a2, call_main

    li a1, 0
    li t1, 0xFFFFFFF0       /* memset hypercall address */

    /* memset hypercall */
    jalr ra, t1, 0

call_main:
    call main

exit_trap:
    li t1, 0xFFFFFFF8       /* exit hypercall address */
    jalr ra, t1, 0
    j exit_trap