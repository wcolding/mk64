glabel func_80080078
/* 080C78 80080078 27BDFFD8 */  addiu $sp, $sp, -0x28
/* 080C7C 8008007C AFBF0024 */  sw    $ra, 0x24($sp)
/* 080C80 80080080 AFB10020 */  sw    $s1, 0x20($sp)
/* 080C84 80080084 3C050600 */  lui   $a1, %hi(gTLUTWhomp) # $a1, 0x600
/* 080C88 80080088 3C060600 */  lui   $a2, %hi(D_06007138) # $a2, 0x600
/* 080C8C 8008008C 240E0040 */  li    $t6, 64
/* 080C90 80080090 00808825 */  move  $s1, $a0
/* 080C94 80080094 AFB0001C */  sw    $s0, 0x1c($sp)
/* 080C98 80080098 AFAE0010 */  sw    $t6, 0x10($sp)
/* 080C9C 8008009C 24C67138 */  addiu $a2, %lo(D_06007138) # addiu $a2, $a2, 0x7138
/* 080CA0 800800A0 24A56F38 */  addiu $a1, %lo(gTLUTWhomp) # addiu $a1, $a1, 0x6f38
/* 080CA4 800800A4 0C01CD11 */  jal   func_80073444
/* 080CA8 800800A8 24070010 */   li    $a3, 16
/* 080CAC 800800AC 001178C0 */  sll   $t7, $s1, 3
/* 080CB0 800800B0 01F17823 */  subu  $t7, $t7, $s1
/* 080CB4 800800B4 3C188016 */  lui   $t8, %hi(D_80165C18) # $t8, 0x8016
/* 080CB8 800800B8 27185C18 */  addiu $t8, %lo(D_80165C18) # addiu $t8, $t8, 0x5c18
/* 080CBC 800800BC 000F7940 */  sll   $t7, $t7, 5
/* 080CC0 800800C0 3C013F80 */  li    $at, 0x3F800000 # 1.000000
/* 080CC4 800800C4 44812000 */  mtc1  $at, $f4
/* 080CC8 800800C8 01F88021 */  addu  $s0, $t7, $t8
/* 080CCC 800800CC 3C190601 */  lui   $t9, %hi(D_06009088) # $t9, 0x601
/* 080CD0 800800D0 27399088 */  addiu $t9, %lo(D_06009088) # addiu $t9, $t9, -0x6f78
/* 080CD4 800800D4 2408000C */  li    $t0, 12
/* 080CD8 800800D8 3C050400 */  lui   $a1, (0x04000220 >> 16) # lui $a1, 0x400
/* 080CDC 800800DC AE190070 */  sw    $t9, 0x70($s0)
/* 080CE0 800800E0 A60800C8 */  sh    $t0, 0xc8($s0)
/* 080CE4 800800E4 34A50220 */  ori   $a1, (0x04000220 & 0xFFFF) # ori $a1, $a1, 0x220
/* 080CE8 800800E8 02202025 */  move  $a0, $s1
/* 080CEC 800800EC 0C01C870 */  jal   func_800721C0
/* 080CF0 800800F0 E6040000 */   swc1  $f4, ($s0)
/* 080CF4 800800F4 24090002 */  li    $t1, 2
/* 080CF8 800800F8 240A0008 */  li    $t2, 8
/* 080CFC 800800FC A60900A4 */  sh    $t1, 0xa4($s0)
/* 080D00 80080100 A20A00DF */  sb    $t2, 0xdf($s0)
/* 080D04 80080104 02202025 */  move  $a0, $s1
/* 080D08 80080108 00002825 */  move  $a1, $zero
/* 080D0C 8008010C 00003025 */  move  $a2, $zero
/* 080D10 80080110 0C022E22 */  jal   func_8008B888
/* 080D14 80080114 00003825 */   move  $a3, $zero
/* 080D18 80080118 44800000 */  mtc1  $zero, $f0
/* 080D1C 8008011C 02202025 */  move  $a0, $s1
/* 080D20 80080120 44050000 */  mfc1  $a1, $f0
/* 080D24 80080124 44060000 */  mfc1  $a2, $f0
/* 080D28 80080128 44070000 */  mfc1  $a3, $f0
/* 080D2C 8008012C E6000044 */  swc1  $f0, 0x44($s0)
/* 080D30 80080130 0C022E03 */  jal   func_8008B80C
/* 080D34 80080134 E6000014 */   swc1  $f0, 0x14($s0)
/* 080D38 80080138 3C0141F0 */  li    $at, 0x41F00000 # 30.000000
/* 080D3C 8008013C 44813000 */  mtc1  $at, $f6
/* 080D40 80080140 3C0B800E */  lui   $t3, %hi(gIsMirrorMode) # $t3, 0x800e
/* 080D44 80080144 00003825 */  move  $a3, $zero
/* 080D48 80080148 E6060020 */  swc1  $f6, 0x20($s0)
/* 080D4C 8008014C 8D6BC604 */  lw    $t3, %lo(gIsMirrorMode)($t3)
/* 080D50 80080150 02202025 */  move  $a0, $s1
/* 080D54 80080154 00002825 */  move  $a1, $zero
/* 080D58 80080158 11600007 */  beqz  $t3, .L80080178
/* 080D5C 8008015C 3406C000 */   li    $a2, 49152
/* 080D60 80080160 02202025 */  move  $a0, $s1
/* 080D64 80080164 00002825 */  move  $a1, $zero
/* 080D68 80080168 0C022E2F */  jal   func_8008B8BC
/* 080D6C 8008016C 24064000 */   li    $a2, 16384
/* 080D70 80080170 10000004 */  b     .L80080184
/* 080D74 80080174 860200A0 */   lh    $v0, 0xa0($s0)
.L80080178:
/* 080D78 80080178 0C022E2F */  jal   func_8008B8BC
/* 080D7C 8008017C 00003825 */   move  $a3, $zero
/* 080D80 80080180 860200A0 */  lh    $v0, 0xa0($s0)
.L80080184:
/* 080D84 80080184 24010001 */  li    $at, 1
/* 080D88 80080188 240C0002 */  li    $t4, 2
/* 080D8C 8008018C 1040000B */  beqz  $v0, .L800801BC
/* 080D90 80080190 00000000 */   nop   
/* 080D94 80080194 1041000B */  beq   $v0, $at, .L800801C4
/* 080D98 80080198 240D003C */   li    $t5, 60
/* 080D9C 8008019C 24010002 */  li    $at, 2
/* 080DA0 800801A0 1041000A */  beq   $v0, $at, .L800801CC
/* 080DA4 800801A4 240E0078 */   li    $t6, 120
/* 080DA8 800801A8 24010003 */  li    $at, 3
/* 080DAC 800801AC 10410009 */  beq   $v0, $at, .L800801D4
/* 080DB0 800801B0 240F00B4 */   li    $t7, 180
/* 080DB4 800801B4 10000008 */  b     .L800801D8
/* 080DB8 800801B8 00000000 */   nop   
.L800801BC:
/* 080DBC 800801BC 10000006 */  b     .L800801D8
/* 080DC0 800801C0 AE0C0050 */   sw    $t4, 0x50($s0)
.L800801C4:
/* 080DC4 800801C4 10000004 */  b     .L800801D8
/* 080DC8 800801C8 AE0D0050 */   sw    $t5, 0x50($s0)
.L800801CC:
/* 080DCC 800801CC 10000002 */  b     .L800801D8
/* 080DD0 800801D0 AE0E0050 */   sw    $t6, 0x50($s0)
.L800801D4:
/* 080DD4 800801D4 AE0F0050 */  sw    $t7, 0x50($s0)
.L800801D8:
/* 080DD8 800801D8 0C01C937 */  jal   func_800724DC
/* 080DDC 800801DC 02202025 */   move  $a0, $s1
/* 080DE0 800801E0 0C01C922 */  jal   func_80072488
/* 080DE4 800801E4 02202025 */   move  $a0, $s1
/* 080DE8 800801E8 8FBF0024 */  lw    $ra, 0x24($sp)
/* 080DEC 800801EC 8FB0001C */  lw    $s0, 0x1c($sp)
/* 080DF0 800801F0 8FB10020 */  lw    $s1, 0x20($sp)
/* 080DF4 800801F4 03E00008 */  jr    $ra
/* 080DF8 800801F8 27BD0028 */   addiu $sp, $sp, 0x28
