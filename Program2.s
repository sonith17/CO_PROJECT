.data
.word 0xf, 0xe, 0xd, 0xc, 0xb, 0xa, 0x9, 0x8, 0x7, 0x6, 0x5, 0x4, 0x3, 0x2, 0x1
base: .word 0x00000100
.text
addi x1 x0 256
addi x2 x0 0
addi x2 x2 16
selection_sort:
    addi x3 x0 0
    addi x4 x0 0
    addi x5 x0 0
    for_loop_1:
        addi x6 x2 -1
        bge x3 x6 end_loop_1
        add x5 x0 x3
        addi x4 x3 1
        for_loop_2:
            bge x4 x2 end_loop_2
            slli x7 x4 2
            add x8 x1 x7
            lw x9 0(x8)
            slli x10 x5 2
            add x11 x1 x10
            lw x12 0(x11)
            if_1:
                bge x9 x12 end_if_1
                add x5 x0 x4
                addi x4 x4 1
                j for_loop_2
        if_2:
            beq x5 x3 end_if_2
            slli x7 x3 2
            add x8 x1 x7
            lw x9 0(x8) 
            slli x10 x5 2
            add x11 x1 x10
            lw x12 0(x11)
            sw x9 0(x11)
            sw x12 0(x8)
            addi x3 x3 1
            j for_loop_1
end_if_2:
    addi x3 x3 1
    j for_loop_1
end_if_1:
    addi x4 x4 1
    j for_loop_2
end_loop_2:
    j if_2
    addi x0 x0 0
end_loop_1:  
     addi x0 x0 0