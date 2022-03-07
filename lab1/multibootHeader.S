.global start

# Necessary parameters
magic = 0x1BADB002
flags = 0
checksum = -0x1BADB002

# Standard multiboot header
.section .multiboot_header
.long magic
.long flags
.long checksum

# Display required information
.text
.code32
start:
    /*
        ASCII table:
            P   0x50
            B   0x42
            2   0x32
            0   0x30
            6   0x36
            1   0x31
            h   0x68
            e   0x65
            l   0x6c
            o   0x6f
            w   0x77
            r   0x72
            d   0x64
            ,   0x2c
                0x20
            C   0x43
            a   0x61
            G   0x47
            x   0x78
            i   0x69
            n   0x6e
            g   0x67
            _   0x5f
    */
    
    # VGA
    movl $0x2f652f68, 0xB8000 # he
    movl $0x2f6c2f6c, 0xB8004 # ll
    movl $0x2f772f6f, 0xB8008 # ow
    movl $0x2f722f6f, 0xB800C # or
    movl $0x2f642f6c, 0xB8010 # ld
    movl $0x2f202f2c, 0xB8014 # , 
    movl $0x2f422f50, 0xB8018 # PB
    movl $0x2f302f32, 0xB801C # 20
    movl $0x2f302f30, 0xB8020 # 00
    movl $0x2f302f30, 0xB8024 # 00
    movl $0x2f312f36, 0xB8028 # 61
    movl $0x2f432f5f, 0xB802C # _C
    movl $0x2f6f2f61, 0xB8030 # ao
    movl $0x2f612f47, 0xB8034 # Ga
    movl $0x2f782f6f, 0xB8038 # ox
    movl $0x2f612f69, 0xB803C # ia
    movl $0x2f672f6e, 0xB8040 # ng

    # UART
    movw $0x3f8, %dx

    # P
    movb $0x50, %al
    outb %al, %dx

    # B
    movb $0x42, %al
    outb %al, %dx

    # 2
    movb $0x32, %al
    outb %al, %dx

    # 0
    movb $0x30, %al
    outb %al, %dx

    # 0
    movb $0x30, %al
    outb %al, %dx

    # 0
    movb $0x30, %al
    outb %al, %dx

    # 0
    movb $0x30, %al
    outb %al, %dx

    # 0
    movb $0x30, %al
    outb %al, %dx

    # 6
    movb $0x36, %al
    outb %al, %dx

    # 1
    movb $0x31, %al
    outb %al, %dx

    hlt
