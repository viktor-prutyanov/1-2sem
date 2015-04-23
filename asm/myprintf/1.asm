
test:     формат файла elf64-x86-64


Дизассемблирование раздела .init:

00000000004003a8 <_init>:
  4003a8:	48 83 ec 08          	sub    rsp,0x8
  4003ac:	48 8b 05 45 0c 20 00 	mov    rax,QWORD PTR [rip+0x200c45]        # 600ff8 <_DYNAMIC+0x1d0>
  4003b3:	48 85 c0             	test   rax,rax
  4003b6:	74 05                	je     4003bd <_init+0x15>
  4003b8:	e8 33 00 00 00       	call   4003f0 <__gmon_start__@plt>
  4003bd:	48 83 c4 08          	add    rsp,0x8
  4003c1:	c3                   	ret    

Дизассемблирование раздела .plt:

00000000004003d0 <__libc_start_main@plt-0x10>:
  4003d0:	ff 35 32 0c 20 00    	push   QWORD PTR [rip+0x200c32]        # 601008 <_GLOBAL_OFFSET_TABLE_+0x8>
  4003d6:	ff 25 34 0c 20 00    	jmp    QWORD PTR [rip+0x200c34]        # 601010 <_GLOBAL_OFFSET_TABLE_+0x10>
  4003dc:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

00000000004003e0 <__libc_start_main@plt>:
  4003e0:	ff 25 32 0c 20 00    	jmp    QWORD PTR [rip+0x200c32]        # 601018 <_GLOBAL_OFFSET_TABLE_+0x18>
  4003e6:	68 00 00 00 00       	push   0x0
  4003eb:	e9 e0 ff ff ff       	jmp    4003d0 <_init+0x28>

00000000004003f0 <__gmon_start__@plt>:
  4003f0:	ff 25 2a 0c 20 00    	jmp    QWORD PTR [rip+0x200c2a]        # 601020 <_GLOBAL_OFFSET_TABLE_+0x20>
  4003f6:	68 01 00 00 00       	push   0x1
  4003fb:	e9 d0 ff ff ff       	jmp    4003d0 <_init+0x28>

Дизассемблирование раздела .text:

0000000000400400 <_start>:
  400400:	31 ed                	xor    ebp,ebp
  400402:	49 89 d1             	mov    r9,rdx
  400405:	5e                   	pop    rsi
  400406:	48 89 e2             	mov    rdx,rsp
  400409:	48 83 e4 f0          	and    rsp,0xfffffffffffffff0
  40040d:	50                   	push   rax
  40040e:	54                   	push   rsp
  40040f:	49 c7 c0 50 08 40 00 	mov    r8,0x400850
  400416:	48 c7 c1 e0 07 40 00 	mov    rcx,0x4007e0
  40041d:	48 c7 c7 ed 04 40 00 	mov    rdi,0x4004ed
  400424:	e8 b7 ff ff ff       	call   4003e0 <__libc_start_main@plt>
  400429:	f4                   	hlt    
  40042a:	66 0f 1f 44 00 00    	nop    WORD PTR [rax+rax*1+0x0]

0000000000400430 <deregister_tm_clones>:
  400430:	b8 3f 11 60 00       	mov    eax,0x60113f
  400435:	55                   	push   rbp
  400436:	48 2d 38 11 60 00    	sub    rax,0x601138
  40043c:	48 83 f8 0e          	cmp    rax,0xe
  400440:	48 89 e5             	mov    rbp,rsp
  400443:	77 02                	ja     400447 <deregister_tm_clones+0x17>
  400445:	5d                   	pop    rbp
  400446:	c3                   	ret    
  400447:	b8 00 00 00 00       	mov    eax,0x0
  40044c:	48 85 c0             	test   rax,rax
  40044f:	74 f4                	je     400445 <deregister_tm_clones+0x15>
  400451:	5d                   	pop    rbp
  400452:	bf 38 11 60 00       	mov    edi,0x601138
  400457:	ff e0                	jmp    rax
  400459:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]

0000000000400460 <register_tm_clones>:
  400460:	b8 38 11 60 00       	mov    eax,0x601138
  400465:	55                   	push   rbp
  400466:	48 2d 38 11 60 00    	sub    rax,0x601138
  40046c:	48 c1 f8 03          	sar    rax,0x3
  400470:	48 89 e5             	mov    rbp,rsp
  400473:	48 89 c2             	mov    rdx,rax
  400476:	48 c1 ea 3f          	shr    rdx,0x3f
  40047a:	48 01 d0             	add    rax,rdx
  40047d:	48 d1 f8             	sar    rax,1
  400480:	75 02                	jne    400484 <register_tm_clones+0x24>
  400482:	5d                   	pop    rbp
  400483:	c3                   	ret    
  400484:	ba 00 00 00 00       	mov    edx,0x0
  400489:	48 85 d2             	test   rdx,rdx
  40048c:	74 f4                	je     400482 <register_tm_clones+0x22>
  40048e:	5d                   	pop    rbp
  40048f:	48 89 c6             	mov    rsi,rax
  400492:	bf 38 11 60 00       	mov    edi,0x601138
  400497:	ff e2                	jmp    rdx
  400499:	0f 1f 80 00 00 00 00 	nop    DWORD PTR [rax+0x0]

00000000004004a0 <__do_global_dtors_aux>:
  4004a0:	80 3d 90 0c 20 00 00 	cmp    BYTE PTR [rip+0x200c90],0x0        # 601137 <_edata>
  4004a7:	75 11                	jne    4004ba <__do_global_dtors_aux+0x1a>
  4004a9:	55                   	push   rbp
  4004aa:	48 89 e5             	mov    rbp,rsp
  4004ad:	e8 7e ff ff ff       	call   400430 <deregister_tm_clones>
  4004b2:	5d                   	pop    rbp
  4004b3:	c6 05 7d 0c 20 00 01 	mov    BYTE PTR [rip+0x200c7d],0x1        # 601137 <_edata>
  4004ba:	f3 c3                	repz ret 
  4004bc:	0f 1f 40 00          	nop    DWORD PTR [rax+0x0]

00000000004004c0 <frame_dummy>:
  4004c0:	48 83 3d 58 09 20 00 	cmp    QWORD PTR [rip+0x200958],0x0        # 600e20 <__JCR_END__>
  4004c7:	00 
  4004c8:	74 1e                	je     4004e8 <frame_dummy+0x28>
  4004ca:	b8 00 00 00 00       	mov    eax,0x0
  4004cf:	48 85 c0             	test   rax,rax
  4004d2:	74 14                	je     4004e8 <frame_dummy+0x28>
  4004d4:	55                   	push   rbp
  4004d5:	bf 20 0e 60 00       	mov    edi,0x600e20
  4004da:	48 89 e5             	mov    rbp,rsp
  4004dd:	ff d0                	call   rax
  4004df:	5d                   	pop    rbp
  4004e0:	e9 7b ff ff ff       	jmp    400460 <register_tm_clones>
  4004e5:	0f 1f 00             	nop    DWORD PTR [rax]
  4004e8:	e9 73 ff ff ff       	jmp    400460 <register_tm_clones>

00000000004004ed <main>:
  4004ed:	55                   	push   rbp
  4004ee:	48 89 e5             	mov    rbp,rsp
  4004f1:	48 83 ec 30          	sub    rsp,0x30
  4004f5:	c7 44 24 20 da 0e 00 	mov    DWORD PTR [rsp+0x20],0xeda
  4004fc:	00 
  4004fd:	c7 44 24 18 03 00 00 	mov    DWORD PTR [rsp+0x18],0x3
  400504:	00 
  400505:	48 c7 44 24 10 da 0e 	mov    QWORD PTR [rsp+0x10],0xeda
  40050c:	00 00 
  40050e:	48 c7 44 24 08 b1 08 	mov    QWORD PTR [rsp+0x8],0x4008b1
  400515:	40 00 
  400517:	48 c7 04 24 39 05 00 	mov    QWORD PTR [rsp],0x539
  40051e:	00 
  40051f:	41 b9 64 00 00 00    	mov    r9d,0x64
  400525:	41 b8 39 05 00 00    	mov    r8d,0x539
  40052b:	b9 68 08 40 00       	mov    ecx,0x400868
  400530:	ba 66 00 00 00       	mov    edx,0x66
  400535:	be 6d 08 40 00       	mov    esi,0x40086d
  40053a:	bf 78 08 40 00       	mov    edi,0x400878
  40053f:	b8 00 00 00 00       	mov    eax,0x0
  400544:	e8 07 00 00 00       	call   400550 <myprintf>
  400549:	b8 00 00 00 00       	mov    eax,0x0
  40054e:	c9                   	leave  
  40054f:	c3                   	ret    

0000000000400550 <myprintf>:
  400550:	55                   	push   rbp
  400551:	48 89 e5             	mov    rbp,rsp
  400554:	57                   	push   rdi
  400555:	56                   	push   rsi
  400556:	52                   	push   rdx
  400557:	51                   	push   rcx
  400558:	41 50                	push   r8
  40055a:	41 51                	push   r9
  40055c:	4d 31 f6             	xor    r14,r14
  40055f:	48 8b 75 f8          	mov    rsi,QWORD PTR [rbp-0x8]
  400563:	48 89 f7             	mov    rdi,rsi
  400566:	b0 00                	mov    al,0x0
  400568:	b9 ff 00 00 00       	mov    ecx,0xff
  40056d:	f2 ae                	repnz scas al,BYTE PTR es:[rdi]
  40056f:	41 bf ff 00 00 00    	mov    r15d,0xff
  400575:	49 29 cf             	sub    r15,rcx
  400578:	48 8b 7d f8          	mov    rdi,QWORD PTR [rbp-0x8]

000000000040057c <next_percent>:
  40057c:	48 89 fe             	mov    rsi,rdi
  40057f:	b0 25                	mov    al,0x25
  400581:	4c 89 f9             	mov    rcx,r15
  400584:	f2 ae                	repnz scas al,BYTE PTR es:[rdi]
  400586:	4c 89 fa             	mov    rdx,r15
  400589:	48 29 ca             	sub    rdx,rcx
  40058c:	48 ff ca             	dec    rdx
  40058f:	48 89 fb             	mov    rbx,rdi
  400592:	b8 01 00 00 00       	mov    eax,0x1
  400597:	bf 01 00 00 00       	mov    edi,0x1
  40059c:	0f 05                	syscall 
  40059e:	49 29 d7             	sub    r15,rdx
  4005a1:	48 89 df             	mov    rdi,rbx
  4005a4:	49 ff c6             	inc    r14
  4005a7:	80 3f 00             	cmp    BYTE PTR [rdi],0x0
  4005aa:	74 41                	je     4005ed <return>
  4005ac:	80 3f 73             	cmp    BYTE PTR [rdi],0x73
  4005af:	74 78                	je     400629 <printf_s>
  4005b1:	80 3f 64             	cmp    BYTE PTR [rdi],0x64
  4005b4:	0f 84 ad 01 00 00    	je     400767 <printf_d>
  4005ba:	80 3f 63             	cmp    BYTE PTR [rdi],0x63
  4005bd:	0f 84 53 01 00 00    	je     400716 <printf_c>
  4005c3:	80 3f 62             	cmp    BYTE PTR [rdi],0x62
  4005c6:	0f 84 ac 00 00 00    	je     400678 <printf_b>
  4005cc:	80 3f 6f             	cmp    BYTE PTR [rdi],0x6f
  4005cf:	0f 84 aa 00 00 00    	je     40067f <printf_o>
  4005d5:	80 3f 78             	cmp    BYTE PTR [rdi],0x78
  4005d8:	0f 84 a8 00 00 00    	je     400686 <printf_x>
  4005de:	80 3f 25             	cmp    BYTE PTR [rdi],0x25
  4005e1:	74 14                	je     4005f7 <printf_prcnt>

00000000004005e3 <next>:
  4005e3:	48 ff c7             	inc    rdi
  4005e6:	80 3f 00             	cmp    BYTE PTR [rdi],0x0
  4005e9:	74 02                	je     4005ed <return>
  4005eb:	eb 8f                	jmp    40057c <next_percent>

00000000004005ed <return>:
  4005ed:	41 59                	pop    r9
  4005ef:	41 58                	pop    r8
  4005f1:	59                   	pop    rcx
  4005f2:	5a                   	pop    rdx
  4005f3:	5e                   	pop    rsi
  4005f4:	5f                   	pop    rdi
  4005f5:	5d                   	pop    rbp
  4005f6:	c3                   	ret    

00000000004005f7 <printf_prcnt>:
  4005f7:	49 ff ce             	dec    r14
  4005fa:	57                   	push   rdi
  4005fb:	56                   	push   rsi
  4005fc:	52                   	push   rdx
  4005fd:	49 83 ef 02          	sub    r15,0x2
  400601:	c6 04 25 38 10 60 00 	mov    BYTE PTR ds:0x601038,0x25
  400608:	25 
  400609:	b8 01 00 00 00       	mov    eax,0x1
  40060e:	bf 01 00 00 00       	mov    edi,0x1
  400613:	48 be 38 10 60 00 00 	movabs rsi,0x601038
  40061a:	00 00 00 
  40061d:	ba 01 00 00 00       	mov    edx,0x1
  400622:	0f 05                	syscall 
  400624:	5a                   	pop    rdx
  400625:	5e                   	pop    rsi
  400626:	5f                   	pop    rdi
  400627:	eb ba                	jmp    4005e3 <next>

0000000000400629 <printf_s>:
  400629:	57                   	push   rdi
  40062a:	56                   	push   rsi
  40062b:	52                   	push   rdx
  40062c:	49 83 ef 02          	sub    r15,0x2
  400630:	4d 89 f5             	mov    r13,r14
  400633:	49 c1 e5 03          	shl    r13,0x3
  400637:	49 83 c5 08          	add    r13,0x8
  40063b:	49 89 ec             	mov    r12,rbp
  40063e:	4d 29 ec             	sub    r12,r13
  400641:	49 8b 34 24          	mov    rsi,QWORD PTR [r12]
  400645:	49 83 fe 06          	cmp    r14,0x6
  400649:	72 05                	jb     400650 <through_registers3>
  40064b:	4a 8b 74 f5 e0       	mov    rsi,QWORD PTR [rbp+r14*8-0x20]

0000000000400650 <through_registers3>:
  400650:	48 89 f7             	mov    rdi,rsi
  400653:	b0 00                	mov    al,0x0
  400655:	b9 ff 00 00 00       	mov    ecx,0xff
  40065a:	f2 ae                	repnz scas al,BYTE PTR es:[rdi]
  40065c:	ba fe 00 00 00       	mov    edx,0xfe
  400661:	48 29 ca             	sub    rdx,rcx
  400664:	b8 01 00 00 00       	mov    eax,0x1
  400669:	bf 01 00 00 00       	mov    edi,0x1
  40066e:	0f 05                	syscall 
  400670:	5a                   	pop    rdx
  400671:	5e                   	pop    rsi
  400672:	5f                   	pop    rdi
  400673:	e9 6b ff ff ff       	jmp    4005e3 <next>

0000000000400678 <printf_b>:
  400678:	b9 01 00 00 00       	mov    ecx,0x1
  40067d:	eb 0c                	jmp    40068b <printf_bx>

000000000040067f <printf_o>:
  40067f:	b9 03 00 00 00       	mov    ecx,0x3
  400684:	eb 05                	jmp    40068b <printf_bx>

0000000000400686 <printf_x>:
  400686:	b9 04 00 00 00       	mov    ecx,0x4

000000000040068b <printf_bx>:
  40068b:	57                   	push   rdi
  40068c:	56                   	push   rsi
  40068d:	52                   	push   rdx
  40068e:	49 83 ef 02          	sub    r15,0x2
  400692:	48 31 db             	xor    rbx,rbx
  400695:	4d 89 f5             	mov    r13,r14
  400698:	49 c1 e5 03          	shl    r13,0x3
  40069c:	49 83 c5 08          	add    r13,0x8
  4006a0:	49 89 ec             	mov    r12,rbp
  4006a3:	4d 29 ec             	sub    r12,r13
  4006a6:	49 8b 04 24          	mov    rax,QWORD PTR [r12]
  4006aa:	49 83 fe 06          	cmp    r14,0x6
  4006ae:	72 05                	jb     4006b5 <next_div_b>
  4006b0:	4a 8b 44 f5 e0       	mov    rax,QWORD PTR [rbp+r14*8-0x20]

00000000004006b5 <next_div_b>:
  4006b5:	48 81 fb ff 00 00 00 	cmp    rbx,0xff
  4006bc:	74 2c                	je     4006ea <next_digit_b>
  4006be:	ba 01 00 00 00       	mov    edx,0x1
  4006c3:	48 d3 e2             	shl    rdx,cl
  4006c6:	48 83 ea 01          	sub    rdx,0x1
  4006ca:	48 21 c2             	and    rdx,rax
  4006cd:	48 d3 e8             	shr    rax,cl
  4006d0:	48 ff c3             	inc    rbx
  4006d3:	52                   	push   rdx
  4006d4:	48 83 f8 00          	cmp    rax,0x0
  4006d8:	75 db                	jne    4006b5 <next_div_b>
  4006da:	48 be 38 10 60 00 00 	movabs rsi,0x601038
  4006e1:	00 00 00 
  4006e4:	48 89 f7             	mov    rdi,rsi
  4006e7:	48 89 d9             	mov    rcx,rbx

00000000004006ea <next_digit_b>:
  4006ea:	58                   	pop    rax
  4006eb:	48 83 f8 0a          	cmp    rax,0xa
  4006ef:	72 04                	jb     4006f5 <digit>
  4006f1:	48 83 c0 07          	add    rax,0x7

00000000004006f5 <digit>:
  4006f5:	48 83 c0 30          	add    rax,0x30
  4006f9:	aa                   	stos   BYTE PTR es:[rdi],al
  4006fa:	e2 ee                	loop   4006ea <next_digit_b>
  4006fc:	48 89 fa             	mov    rdx,rdi
  4006ff:	48 29 f2             	sub    rdx,rsi
  400702:	b8 01 00 00 00       	mov    eax,0x1
  400707:	bf 01 00 00 00       	mov    edi,0x1
  40070c:	0f 05                	syscall 
  40070e:	5a                   	pop    rdx
  40070f:	5e                   	pop    rsi
  400710:	5f                   	pop    rdi
  400711:	e9 cd fe ff ff       	jmp    4005e3 <next>

0000000000400716 <printf_c>:
  400716:	57                   	push   rdi
  400717:	56                   	push   rsi
  400718:	52                   	push   rdx
  400719:	49 83 ef 02          	sub    r15,0x2
  40071d:	4d 89 f5             	mov    r13,r14
  400720:	49 c1 e5 03          	shl    r13,0x3
  400724:	49 83 c5 08          	add    r13,0x8
  400728:	49 89 ec             	mov    r12,rbp
  40072b:	4d 29 ec             	sub    r12,r13
  40072e:	49 8b 04 24          	mov    rax,QWORD PTR [r12]
  400732:	49 83 fe 06          	cmp    r14,0x6
  400736:	72 05                	jb     40073d <through_registers4>
  400738:	4a 8b 44 f5 e0       	mov    rax,QWORD PTR [rbp+r14*8-0x20]

000000000040073d <through_registers4>:
  40073d:	88 04 25 38 10 60 00 	mov    BYTE PTR ds:0x601038,al
  400744:	b8 01 00 00 00       	mov    eax,0x1
  400749:	bf 01 00 00 00       	mov    edi,0x1
  40074e:	48 be 38 10 60 00 00 	movabs rsi,0x601038
  400755:	00 00 00 
  400758:	ba 01 00 00 00       	mov    edx,0x1
  40075d:	0f 05                	syscall 
  40075f:	5a                   	pop    rdx
  400760:	5e                   	pop    rsi
  400761:	5f                   	pop    rdi
  400762:	e9 7c fe ff ff       	jmp    4005e3 <next>

0000000000400767 <printf_d>:
  400767:	57                   	push   rdi
  400768:	56                   	push   rsi
  400769:	52                   	push   rdx
  40076a:	49 83 ef 02          	sub    r15,0x2
  40076e:	48 31 c9             	xor    rcx,rcx
  400771:	4d 89 f5             	mov    r13,r14
  400774:	49 c1 e5 03          	shl    r13,0x3
  400778:	49 83 c5 08          	add    r13,0x8
  40077c:	49 89 ec             	mov    r12,rbp
  40077f:	4d 29 ec             	sub    r12,r13
  400782:	49 8b 04 24          	mov    rax,QWORD PTR [r12]
  400786:	49 83 fe 06          	cmp    r14,0x6
  40078a:	72 05                	jb     400791 <through_registers1>
  40078c:	4a 8b 44 f5 e0       	mov    rax,QWORD PTR [rbp+r14*8-0x20]

0000000000400791 <through_registers1>:
  400791:	bb 0a 00 00 00       	mov    ebx,0xa

0000000000400796 <next_div>:
  400796:	48 81 f9 ff 00 00 00 	cmp    rcx,0xff
  40079d:	74 1d                	je     4007bc <next_digit_d>
  40079f:	48 ff c1             	inc    rcx
  4007a2:	48 31 d2             	xor    rdx,rdx
  4007a5:	48 f7 f3             	div    rbx
  4007a8:	52                   	push   rdx
  4007a9:	48 83 f8 00          	cmp    rax,0x0
  4007ad:	75 e7                	jne    400796 <next_div>
  4007af:	48 be 38 10 60 00 00 	movabs rsi,0x601038
  4007b6:	00 00 00 
  4007b9:	48 89 f7             	mov    rdi,rsi

00000000004007bc <next_digit_d>:
  4007bc:	58                   	pop    rax
  4007bd:	48 83 c0 30          	add    rax,0x30
  4007c1:	aa                   	stos   BYTE PTR es:[rdi],al
  4007c2:	e2 f8                	loop   4007bc <next_digit_d>
  4007c4:	48 89 fa             	mov    rdx,rdi
  4007c7:	48 29 f2             	sub    rdx,rsi
  4007ca:	b8 01 00 00 00       	mov    eax,0x1
  4007cf:	bf 01 00 00 00       	mov    edi,0x1
  4007d4:	0f 05                	syscall 
  4007d6:	5a                   	pop    rdx
  4007d7:	5e                   	pop    rsi
  4007d8:	5f                   	pop    rdi
  4007d9:	e9 05 fe ff ff       	jmp    4005e3 <next>
  4007de:	66 90                	xchg   ax,ax

00000000004007e0 <__libc_csu_init>:
  4007e0:	41 57                	push   r15
  4007e2:	41 89 ff             	mov    r15d,edi
  4007e5:	41 56                	push   r14
  4007e7:	49 89 f6             	mov    r14,rsi
  4007ea:	41 55                	push   r13
  4007ec:	49 89 d5             	mov    r13,rdx
  4007ef:	41 54                	push   r12
  4007f1:	4c 8d 25 18 06 20 00 	lea    r12,[rip+0x200618]        # 600e10 <__frame_dummy_init_array_entry>
  4007f8:	55                   	push   rbp
  4007f9:	48 8d 2d 18 06 20 00 	lea    rbp,[rip+0x200618]        # 600e18 <__init_array_end>
  400800:	53                   	push   rbx
  400801:	4c 29 e5             	sub    rbp,r12
  400804:	31 db                	xor    ebx,ebx
  400806:	48 c1 fd 03          	sar    rbp,0x3
  40080a:	48 83 ec 08          	sub    rsp,0x8
  40080e:	e8 95 fb ff ff       	call   4003a8 <_init>
  400813:	48 85 ed             	test   rbp,rbp
  400816:	74 1e                	je     400836 <__libc_csu_init+0x56>
  400818:	0f 1f 84 00 00 00 00 	nop    DWORD PTR [rax+rax*1+0x0]
  40081f:	00 
  400820:	4c 89 ea             	mov    rdx,r13
  400823:	4c 89 f6             	mov    rsi,r14
  400826:	44 89 ff             	mov    edi,r15d
  400829:	41 ff 14 dc          	call   QWORD PTR [r12+rbx*8]
  40082d:	48 83 c3 01          	add    rbx,0x1
  400831:	48 39 eb             	cmp    rbx,rbp
  400834:	75 ea                	jne    400820 <__libc_csu_init+0x40>
  400836:	48 83 c4 08          	add    rsp,0x8
  40083a:	5b                   	pop    rbx
  40083b:	5d                   	pop    rbp
  40083c:	41 5c                	pop    r12
  40083e:	41 5d                	pop    r13
  400840:	41 5e                	pop    r14
  400842:	41 5f                	pop    r15
  400844:	c3                   	ret    
  400845:	66 66 2e 0f 1f 84 00 	data32 nop WORD PTR cs:[rax+rax*1+0x0]
  40084c:	00 00 00 00 

0000000000400850 <__libc_csu_fini>:
  400850:	f3 c3                	repz ret 

Дизассемблирование раздела .fini:

0000000000400854 <_fini>:
  400854:	48 83 ec 08          	sub    rsp,0x8
  400858:	48 83 c4 08          	add    rsp,0x8
  40085c:	c3                   	ret    
