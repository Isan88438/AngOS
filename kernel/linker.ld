ENTRY(angos_entry)

SECTIONS
{
    . = 0xFFFFFFFF80000000;

    .text ALIGN(0x1000) : ALIGN(0x1000)
    {
        KEEP(*(.tsbp))
        *(.text.entry)
        *(.text .text.*)
    }

    .rodata ALIGN(0x1000) :
    {
        *(.rodata .rodata.*)
    }

    .data ALIGN(0x1000) :
    {
        *(.data .data.*)
    }

    .bss ALIGN(0x1000) :
    {
        *(COMMON)
        *(.bss .bss.*)
    }

    /DISCARD/ :
    {
        *(.comment)
        *(.note*)
        *(.eh_frame*)
    }
}