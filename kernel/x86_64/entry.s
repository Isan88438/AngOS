# Tosaithe boot protocol header
.section .tsbp,"a"

.global angos_tosaithe_header

angos_tosaithe_header:
    .long 0x50425354      # "TSBP"
    .long 1               # protocol version
    .long 1               # minimum required version
    .long 1               # requires framebuffer
    .quad angos_stack_top

# Kernel entry point
.section .text.entry

.global angos_entry
.extern init
.extern kernel_main

angos_entry:
    call init
    call kernel_main

1:
    hlt
    jmp 1b