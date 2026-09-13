.section .text.entry
.global angos_entry

angos_entry:
    call init
    cli
1:
    hlt
    jmp 1b