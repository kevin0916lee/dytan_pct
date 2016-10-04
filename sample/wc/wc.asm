
wc:     file format elf64-x86-64


Disassembly of section .init:

0000000000400488 <_init>:
  400488:	48 83 ec 08          	sub    $0x8,%rsp
  40048c:	48 8b 05 65 0b 20 00 	mov    0x200b65(%rip),%rax        # 600ff8 <_DYNAMIC+0x1d0>
  400493:	48 85 c0             	test   %rax,%rax
  400496:	74 05                	je     40049d <_init+0x15>
  400498:	e8 63 00 00 00       	callq  400500 <__gmon_start__@plt>
  40049d:	48 83 c4 08          	add    $0x8,%rsp
  4004a1:	c3                   	retq   

Disassembly of section .plt:

00000000004004b0 <fclose@plt-0x10>:
  4004b0:	ff 35 52 0b 20 00    	pushq  0x200b52(%rip)        # 601008 <_GLOBAL_OFFSET_TABLE_+0x8>
  4004b6:	ff 25 54 0b 20 00    	jmpq   *0x200b54(%rip)        # 601010 <_GLOBAL_OFFSET_TABLE_+0x10>
  4004bc:	0f 1f 40 00          	nopl   0x0(%rax)

00000000004004c0 <fclose@plt>:
  4004c0:	ff 25 52 0b 20 00    	jmpq   *0x200b52(%rip)        # 601018 <_GLOBAL_OFFSET_TABLE_+0x18>
  4004c6:	68 00 00 00 00       	pushq  $0x0
  4004cb:	e9 e0 ff ff ff       	jmpq   4004b0 <_init+0x28>

00000000004004d0 <printf@plt>:
  4004d0:	ff 25 4a 0b 20 00    	jmpq   *0x200b4a(%rip)        # 601020 <_GLOBAL_OFFSET_TABLE_+0x20>
  4004d6:	68 01 00 00 00       	pushq  $0x1
  4004db:	e9 d0 ff ff ff       	jmpq   4004b0 <_init+0x28>

00000000004004e0 <fgetc@plt>:
  4004e0:	ff 25 42 0b 20 00    	jmpq   *0x200b42(%rip)        # 601028 <_GLOBAL_OFFSET_TABLE_+0x28>
  4004e6:	68 02 00 00 00       	pushq  $0x2
  4004eb:	e9 c0 ff ff ff       	jmpq   4004b0 <_init+0x28>

00000000004004f0 <__libc_start_main@plt>:
  4004f0:	ff 25 3a 0b 20 00    	jmpq   *0x200b3a(%rip)        # 601030 <_GLOBAL_OFFSET_TABLE_+0x30>
  4004f6:	68 03 00 00 00       	pushq  $0x3
  4004fb:	e9 b0 ff ff ff       	jmpq   4004b0 <_init+0x28>

0000000000400500 <__gmon_start__@plt>:
  400500:	ff 25 32 0b 20 00    	jmpq   *0x200b32(%rip)        # 601038 <_GLOBAL_OFFSET_TABLE_+0x38>
  400506:	68 04 00 00 00       	pushq  $0x4
  40050b:	e9 a0 ff ff ff       	jmpq   4004b0 <_init+0x28>

0000000000400510 <fopen@plt>:
  400510:	ff 25 2a 0b 20 00    	jmpq   *0x200b2a(%rip)        # 601040 <_GLOBAL_OFFSET_TABLE_+0x40>
  400516:	68 05 00 00 00       	pushq  $0x5
  40051b:	e9 90 ff ff ff       	jmpq   4004b0 <_init+0x28>

Disassembly of section .text:

0000000000400520 <_start>:
  400520:	31 ed                	xor    %ebp,%ebp
  400522:	49 89 d1             	mov    %rdx,%r9
  400525:	5e                   	pop    %rsi
  400526:	48 89 e2             	mov    %rsp,%rdx
  400529:	48 83 e4 f0          	and    $0xfffffffffffffff0,%rsp
  40052d:	50                   	push   %rax
  40052e:	54                   	push   %rsp
  40052f:	49 c7 c0 00 08 40 00 	mov    $0x400800,%r8
  400536:	48 c7 c1 90 07 40 00 	mov    $0x400790,%rcx
  40053d:	48 c7 c7 77 06 40 00 	mov    $0x400677,%rdi
  400544:	e8 a7 ff ff ff       	callq  4004f0 <__libc_start_main@plt>
  400549:	f4                   	hlt    
  40054a:	66 0f 1f 44 00 00    	nopw   0x0(%rax,%rax,1)

0000000000400550 <deregister_tm_clones>:
  400550:	b8 5f 10 60 00       	mov    $0x60105f,%eax
  400555:	55                   	push   %rbp
  400556:	48 2d 58 10 60 00    	sub    $0x601058,%rax
  40055c:	48 83 f8 0e          	cmp    $0xe,%rax
  400560:	48 89 e5             	mov    %rsp,%rbp
  400563:	77 02                	ja     400567 <deregister_tm_clones+0x17>
  400565:	5d                   	pop    %rbp
  400566:	c3                   	retq   
  400567:	b8 00 00 00 00       	mov    $0x0,%eax
  40056c:	48 85 c0             	test   %rax,%rax
  40056f:	74 f4                	je     400565 <deregister_tm_clones+0x15>
  400571:	5d                   	pop    %rbp
  400572:	bf 58 10 60 00       	mov    $0x601058,%edi
  400577:	ff e0                	jmpq   *%rax
  400579:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

0000000000400580 <register_tm_clones>:
  400580:	b8 58 10 60 00       	mov    $0x601058,%eax
  400585:	55                   	push   %rbp
  400586:	48 2d 58 10 60 00    	sub    $0x601058,%rax
  40058c:	48 c1 f8 03          	sar    $0x3,%rax
  400590:	48 89 e5             	mov    %rsp,%rbp
  400593:	48 89 c2             	mov    %rax,%rdx
  400596:	48 c1 ea 3f          	shr    $0x3f,%rdx
  40059a:	48 01 d0             	add    %rdx,%rax
  40059d:	48 d1 f8             	sar    %rax
  4005a0:	75 02                	jne    4005a4 <register_tm_clones+0x24>
  4005a2:	5d                   	pop    %rbp
  4005a3:	c3                   	retq   
  4005a4:	ba 00 00 00 00       	mov    $0x0,%edx
  4005a9:	48 85 d2             	test   %rdx,%rdx
  4005ac:	74 f4                	je     4005a2 <register_tm_clones+0x22>
  4005ae:	5d                   	pop    %rbp
  4005af:	48 89 c6             	mov    %rax,%rsi
  4005b2:	bf 58 10 60 00       	mov    $0x601058,%edi
  4005b7:	ff e2                	jmpq   *%rdx
  4005b9:	0f 1f 80 00 00 00 00 	nopl   0x0(%rax)

00000000004005c0 <__do_global_dtors_aux>:
  4005c0:	80 3d 91 0a 20 00 00 	cmpb   $0x0,0x200a91(%rip)        # 601058 <__TMC_END__>
  4005c7:	75 11                	jne    4005da <__do_global_dtors_aux+0x1a>
  4005c9:	55                   	push   %rbp
  4005ca:	48 89 e5             	mov    %rsp,%rbp
  4005cd:	e8 7e ff ff ff       	callq  400550 <deregister_tm_clones>
  4005d2:	5d                   	pop    %rbp
  4005d3:	c6 05 7e 0a 20 00 01 	movb   $0x1,0x200a7e(%rip)        # 601058 <__TMC_END__>
  4005da:	f3 c3                	repz retq 
  4005dc:	0f 1f 40 00          	nopl   0x0(%rax)

00000000004005e0 <frame_dummy>:
  4005e0:	48 83 3d 38 08 20 00 	cmpq   $0x0,0x200838(%rip)        # 600e20 <__JCR_END__>
  4005e7:	00 
  4005e8:	74 1e                	je     400608 <frame_dummy+0x28>
  4005ea:	b8 00 00 00 00       	mov    $0x0,%eax
  4005ef:	48 85 c0             	test   %rax,%rax
  4005f2:	74 14                	je     400608 <frame_dummy+0x28>
  4005f4:	55                   	push   %rbp
  4005f5:	bf 20 0e 60 00       	mov    $0x600e20,%edi
  4005fa:	48 89 e5             	mov    %rsp,%rbp
  4005fd:	ff d0                	callq  *%rax
  4005ff:	5d                   	pop    %rbp
  400600:	e9 7b ff ff ff       	jmpq   400580 <register_tm_clones>
  400605:	0f 1f 00             	nopl   (%rax)
  400608:	e9 73 ff ff ff       	jmpq   400580 <register_tm_clones>

000000000040060d <DYTAN_tag>:
  40060d:	55                   	push   %rbp
  40060e:	48 89 e5             	mov    %rsp,%rbp
  400611:	48 89 7d f8          	mov    %rdi,-0x8(%rbp)
  400615:	48 89 75 f0          	mov    %rsi,-0x10(%rbp)
  400619:	48 89 55 e8          	mov    %rdx,-0x18(%rbp)
  40061d:	5d                   	pop    %rbp
  40061e:	c3                   	retq   

000000000040061f <DYTAN_display>:
  40061f:	55                   	push   %rbp
  400620:	48 89 e5             	mov    %rsp,%rbp
  400623:	48 83 ec 50          	sub    $0x50,%rsp
  400627:	48 89 bd 48 ff ff ff 	mov    %rdi,-0xb8(%rbp)
  40062e:	48 89 b5 40 ff ff ff 	mov    %rsi,-0xc0(%rbp)
  400635:	48 89 8d 68 ff ff ff 	mov    %rcx,-0x98(%rbp)
  40063c:	4c 89 85 70 ff ff ff 	mov    %r8,-0x90(%rbp)
  400643:	4c 89 8d 78 ff ff ff 	mov    %r9,-0x88(%rbp)
  40064a:	84 c0                	test   %al,%al
  40064c:	74 20                	je     40066e <DYTAN_display+0x4f>
  40064e:	0f 29 45 80          	movaps %xmm0,-0x80(%rbp)
  400652:	0f 29 4d 90          	movaps %xmm1,-0x70(%rbp)
  400656:	0f 29 55 a0          	movaps %xmm2,-0x60(%rbp)
  40065a:	0f 29 5d b0          	movaps %xmm3,-0x50(%rbp)
  40065e:	0f 29 65 c0          	movaps %xmm4,-0x40(%rbp)
  400662:	0f 29 6d d0          	movaps %xmm5,-0x30(%rbp)
  400666:	0f 29 75 e0          	movaps %xmm6,-0x20(%rbp)
  40066a:	0f 29 7d f0          	movaps %xmm7,-0x10(%rbp)
  40066e:	48 89 95 38 ff ff ff 	mov    %rdx,-0xc8(%rbp)
  400675:	c9                   	leaveq 
  400676:	c3                   	retq   

0000000000400677 <main>:
  400677:	55                   	push   %rbp
  400678:	48 89 e5             	mov    %rsp,%rbp
  40067b:	48 83 ec 30          	sub    $0x30,%rsp
  40067f:	89 7d dc             	mov    %edi,-0x24(%rbp)
  400682:	48 89 75 d0          	mov    %rsi,-0x30(%rbp)
  400686:	c7 45 f4 00 00 00 00 	movl   $0x0,-0xc(%rbp)
  40068d:	c7 45 f0 01 00 00 00 	movl   $0x1,-0x10(%rbp)
  400694:	e9 cd 00 00 00       	jmpq   400766 <main+0xef>
  400699:	c7 45 ec 00 00 00 00 	movl   $0x0,-0x14(%rbp)
  4006a0:	8b 45 f0             	mov    -0x10(%rbp),%eax
  4006a3:	48 98                	cltq   
  4006a5:	48 8d 14 c5 00 00 00 	lea    0x0(,%rax,8),%rdx
  4006ac:	00 
  4006ad:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
  4006b1:	48 01 d0             	add    %rdx,%rax
  4006b4:	48 8b 00             	mov    (%rax),%rax
  4006b7:	be 14 08 40 00       	mov    $0x400814,%esi
  4006bc:	48 89 c7             	mov    %rax,%rdi
  4006bf:	e8 4c fe ff ff       	callq  400510 <fopen@plt>
  4006c4:	48 89 45 f8          	mov    %rax,-0x8(%rbp)
  4006c8:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4006cc:	48 89 c7             	mov    %rax,%rdi
  4006cf:	e8 0c fe ff ff       	callq  4004e0 <fgetc@plt>
  4006d4:	88 45 ea             	mov    %al,-0x16(%rbp)
  4006d7:	0f b6 45 ea          	movzbl -0x16(%rbp),%eax
  4006db:	88 45 eb             	mov    %al,-0x15(%rbp)
  4006de:	eb 1e                	jmp    4006fe <main+0x87>
  4006e0:	80 7d ea 0a          	cmpb   $0xa,-0x16(%rbp)
  4006e4:	75 09                	jne    4006ef <main+0x78>
  4006e6:	8b 45 ec             	mov    -0x14(%rbp),%eax
  4006e9:	83 c0 01             	add    $0x1,%eax
  4006ec:	89 45 ec             	mov    %eax,-0x14(%rbp)
  4006ef:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  4006f3:	48 89 c7             	mov    %rax,%rdi
  4006f6:	e8 e5 fd ff ff       	callq  4004e0 <fgetc@plt>
  4006fb:	88 45 ea             	mov    %al,-0x16(%rbp)
  4006fe:	80 7d ea ff          	cmpb   $0xff,-0x16(%rbp)
  400702:	75 dc                	jne    4006e0 <main+0x69>
  400704:	48 8b 45 f8          	mov    -0x8(%rbp),%rax
  400708:	48 89 c7             	mov    %rax,%rdi
  40070b:	e8 b0 fd ff ff       	callq  4004c0 <fclose@plt>
  400710:	8b 55 ec             	mov    -0x14(%rbp),%edx
  400713:	48 8d 45 ec          	lea    -0x14(%rbp),%rax
  400717:	89 d1                	mov    %edx,%ecx
  400719:	ba 16 08 40 00       	mov    $0x400816,%edx
  40071e:	be 04 00 00 00       	mov    $0x4,%esi
  400723:	48 89 c7             	mov    %rax,%rdi
  400726:	b8 00 00 00 00       	mov    $0x0,%eax
  40072b:	e8 ef fe ff ff       	callq  40061f <DYTAN_display>
  400730:	8b 55 ec             	mov    -0x14(%rbp),%edx
  400733:	8b 45 f0             	mov    -0x10(%rbp),%eax
  400736:	48 98                	cltq   
  400738:	48 8d 0c c5 00 00 00 	lea    0x0(,%rax,8),%rcx
  40073f:	00 
  400740:	48 8b 45 d0          	mov    -0x30(%rbp),%rax
  400744:	48 01 c8             	add    %rcx,%rax
  400747:	48 8b 00             	mov    (%rax),%rax
  40074a:	48 89 c6             	mov    %rax,%rsi
  40074d:	bf 1f 08 40 00       	mov    $0x40081f,%edi
  400752:	b8 00 00 00 00       	mov    $0x0,%eax
  400757:	e8 74 fd ff ff       	callq  4004d0 <printf@plt>
  40075c:	8b 45 ec             	mov    -0x14(%rbp),%eax
  40075f:	01 45 f4             	add    %eax,-0xc(%rbp)
  400762:	83 45 f0 01          	addl   $0x1,-0x10(%rbp)
  400766:	8b 45 f0             	mov    -0x10(%rbp),%eax
  400769:	3b 45 dc             	cmp    -0x24(%rbp),%eax
  40076c:	0f 8c 27 ff ff ff    	jl     400699 <main+0x22>
  400772:	8b 45 f4             	mov    -0xc(%rbp),%eax
  400775:	89 c6                	mov    %eax,%esi
  400777:	bf 27 08 40 00       	mov    $0x400827,%edi
  40077c:	b8 00 00 00 00       	mov    $0x0,%eax
  400781:	e8 4a fd ff ff       	callq  4004d0 <printf@plt>
  400786:	b8 00 00 00 00       	mov    $0x0,%eax
  40078b:	c9                   	leaveq 
  40078c:	c3                   	retq   
  40078d:	0f 1f 00             	nopl   (%rax)

0000000000400790 <__libc_csu_init>:
  400790:	41 57                	push   %r15
  400792:	41 89 ff             	mov    %edi,%r15d
  400795:	41 56                	push   %r14
  400797:	49 89 f6             	mov    %rsi,%r14
  40079a:	41 55                	push   %r13
  40079c:	49 89 d5             	mov    %rdx,%r13
  40079f:	41 54                	push   %r12
  4007a1:	4c 8d 25 68 06 20 00 	lea    0x200668(%rip),%r12        # 600e10 <__frame_dummy_init_array_entry>
  4007a8:	55                   	push   %rbp
  4007a9:	48 8d 2d 68 06 20 00 	lea    0x200668(%rip),%rbp        # 600e18 <__init_array_end>
  4007b0:	53                   	push   %rbx
  4007b1:	4c 29 e5             	sub    %r12,%rbp
  4007b4:	31 db                	xor    %ebx,%ebx
  4007b6:	48 c1 fd 03          	sar    $0x3,%rbp
  4007ba:	48 83 ec 08          	sub    $0x8,%rsp
  4007be:	e8 c5 fc ff ff       	callq  400488 <_init>
  4007c3:	48 85 ed             	test   %rbp,%rbp
  4007c6:	74 1e                	je     4007e6 <__libc_csu_init+0x56>
  4007c8:	0f 1f 84 00 00 00 00 	nopl   0x0(%rax,%rax,1)
  4007cf:	00 
  4007d0:	4c 89 ea             	mov    %r13,%rdx
  4007d3:	4c 89 f6             	mov    %r14,%rsi
  4007d6:	44 89 ff             	mov    %r15d,%edi
  4007d9:	41 ff 14 dc          	callq  *(%r12,%rbx,8)
  4007dd:	48 83 c3 01          	add    $0x1,%rbx
  4007e1:	48 39 eb             	cmp    %rbp,%rbx
  4007e4:	75 ea                	jne    4007d0 <__libc_csu_init+0x40>
  4007e6:	48 83 c4 08          	add    $0x8,%rsp
  4007ea:	5b                   	pop    %rbx
  4007eb:	5d                   	pop    %rbp
  4007ec:	41 5c                	pop    %r12
  4007ee:	41 5d                	pop    %r13
  4007f0:	41 5e                	pop    %r14
  4007f2:	41 5f                	pop    %r15
  4007f4:	c3                   	retq   
  4007f5:	66 66 2e 0f 1f 84 00 	data32 nopw %cs:0x0(%rax,%rax,1)
  4007fc:	00 00 00 00 

0000000000400800 <__libc_csu_fini>:
  400800:	f3 c3                	repz retq 

Disassembly of section .fini:

0000000000400804 <_fini>:
  400804:	48 83 ec 08          	sub    $0x8,%rsp
  400808:	48 83 c4 08          	add    $0x8,%rsp
  40080c:	c3                   	retq   
