bits 64
;ADD: number at stack top will be popped and add new top
    pop rax
    add [rsp], rax