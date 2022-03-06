.global start

# Necessary parameters
magic = 0x1BADB002
flags = 0
checksum = -0x1BADB002

# Standard multiboot header
.section header
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
    */
    
    # VGA
    movl $0x2f422f50, 0xB8000 # PB
    movl $0x2f302f32, 0xB8004 # 20
    movl $0x2f302f30, 0xB8008 # 00
    movl $0x2f302f30, 0xB800C # 00
    movl $0x2f312f36, 0xB8010 # 61

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