// Instruct the assembler to generate thumb code
// for the cortex-M3 core
.cpu cortex-m3
.thumb

// end of 20k RAM
.word 0x20005000
.word _reset
.thumb_func
_reset:
    bl main
    b .
