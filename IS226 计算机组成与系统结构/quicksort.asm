; multi-segment executable file template.

data_seg segment
    ; add your data here!
    a dw 49H,48H,47H,46H,45H,44H,43H,42H,41H,40H,39H,38H,37H,36H,35H,34H,33H,32H,31H,30H,29H,28H,27H,26H,25H,24H,23H,22H,21H,20H,19H,18H,17H,16H,15H,14H,13H,12H,11H,10H,9H,8H,7H,6H,5H,4H,3H,2H,1H,0H
    p dw 2
    q dw ?
    low dw ?
    high dw ?
data_seg ends

stack_seg segment stack
    ST db   128  dup(0)  
    top equ 128
stack_seg ends

code_seg segment
assume cs: code_seg, ds: data_seg, ss:stack_seg
start:
; set segment registers:
    mov ax, data_seg
    mov ds, ax
    mov ax, stack_seg
    mov ss, ax 
    mov sp, top

    ; add your code here
    mov dx,a
    mov [1200h],sp
    push 0
    push 49        
       
    divide:
        pop [high]
        pop [low]
        push [low]
        push [high]
        mov bp,sp
        mov ax,ss[bp+2]
        mov bx,ss[bp]
        cmp ax,bx
        jge lt
        mov ax,ss[bp+2]
        mul [p]
        mov bx,ax
        mov ax,a[bx]
        mov [q],ax
        mov bx,[q]
        ;临时变量
        loop1: mov bx,ss[bp+2]
        mov cx,ss[bp]
        cmp bx,cx
        jge do1
        mov ax,cx
        mul [p]
        mov bx,ax
        mov cx,a[bx]
        cmp cx,[q]
        jnge do1
        sub ss[bp],0001h
        jmp loop1;第一个while循环
        do1: mov bx,ss[bp+2]
        mov cx,ss[bp]
        cmp bx,cx
        jge loop2
        mov ax,ss[bp+2]
        mul [p]
        mov di,ax
        mov ax,ss[bp]
        mul [p]
        mov si,ax
        mov ax,a[si]
        mov cx,a[di]
        mov a[di],ax
        add ss[bp+2],1;第一个if语句
        loop2: mov bx,ss[bp+2]
        mov cx,ss[bp]
        cmp bx,cx
        jge do2
        mov ax,ss[bp+2]
        mul [p]
        mov si,ax
        mov cx,a[si]
        mov bx,[q]
        cmp cx,bx
        jg do2
        add ss[bp+2],1
        jmp loop2;第二个while循环
        do2:  mov bx,ss[bp+2]
        mov cx,ss[bp]
        cmp bx,cx
        jge l2
        mov ax,ss[bp+2]
        mul [p]
        mov di,ax
        mov ax,ss[bp]
        mul [p]
        mov si,ax
        mov ax,a[di]
        mov a[si],ax
        sub ss[bp],1;第二个if语句
        l2: mov bx,ss[bp+2]
        mov cx,ss[bp]
        cmp bx,cx
        jnz loop1
        mov ax,ss[bp+2]
        mul [p]
        mov si,ax
        mov ax,[q]
        mov a[si],ax
        
        return:pop ax
        pop bx
        add ax,1
        push ax
        mov cx,[high]
        push cx
        sub bx,1
        mov dx,[low]
        push dx
        push bx
        jmp divide
        

         
        
        lt:
        pop ax
        pop bx
        rec:cmp sp,[1200h]
        je end
        jmp divide
        
        
        
        
        
        
        
        
        
        
    
    end:
    mov ax,a
    mov bx,a[14]
    mov cx,a[16]
    mov dx,a[18]
    mov ax, 4c00h ; exit to operating system.
    int 21h    
code_seg ends

end start ; set entry point and stop the assembler.