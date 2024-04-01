.data
array: .word 0x12f 0x12c 0x13 0x12 0x11 0x10 0xf 0xe 0xd 0xc 0xb 0xa 0x9 0x8 0x7 0x6 0x5 0x4 0x3 0x2 0x1
.text
 addi x16 x16 1024
 addi x25 x0 21
 addi x26 x0 -1 #jfdgjd
 outer_loop:
     addi x26 x26 1
     beq x25 x26 exit
     addi x27 x26 -20
     sub x27 x0 x27
     addi x28 x0 0
     addi x31 x16 0
     inner_loop:
         beq x27 x28 outer_loop
         lw x29 0(x31)
         lw x30 4(x31)
         blt x29 x30 noswap
         sw x29 4(x31)
         sw x30 0(x31)
         addi x28 x28 1 
         addi x31 x31 4 
         j inner_loop
         noswap:
             addi x28 x28 1 
             addi x31 x31 4 
             j inner_loop                
exit:
    addi x31 x16 0
    addi x20 x20 -24
