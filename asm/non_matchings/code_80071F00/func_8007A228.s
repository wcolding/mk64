glabel func_8007A228
/* 07AE28 8007A228 27BDFFD8 */  addiu $sp, $sp, -0x28
/* 07AE2C 8007A22C AFBF0024 */  sw    $ra, 0x24($sp)
/* 07AE30 8007A230 AFB00020 */  sw    $s0, 0x20($sp)
/* 07AE34 8007A234 0C01E47C */  jal   func_800791F0
/* 07AE38 8007A238 00808025 */   move  $s0, $a0
/* 07AE3C 8007A23C 3C050D02 */  lui   $a1, %hi(gTLUTLakituFinalLap) # $a1, 0xd02
/* 07AE40 8007A240 3C060F0B */  lui   $a2, %hi(D_0F0AD750) # $a2, 0xf0b
/* 07AE44 8007A244 240E0038 */  li    $t6, 56
/* 07AE48 8007A248 AFAE0010 */  sw    $t6, 0x10($sp)
/* 07AE4C 8007A24C 24C6D750 */  addiu $a2, %lo(D_0F0AD750) # addiu $a2, $a2, -0x28b0
/* 07AE50 8007A250 24A558D8 */  addiu $a1, %lo(gTLUTLakituFinalLap) # addiu $a1, $a1, 0x58d8
/* 07AE54 8007A254 02002025 */  move  $a0, $s0
/* 07AE58 8007A258 0C01CD11 */  jal   func_80073444
/* 07AE5C 8007A25C 24070048 */   li    $a3, 72
/* 07AE60 8007A260 001078C0 */  sll   $t7, $s0, 3
/* 07AE64 8007A264 3C01800F */  lui   $at, %hi(D_800EECB8) # $at, 0x800f
/* 07AE68 8007A268 01F07823 */  subu  $t7, $t7, $s0
/* 07AE6C 8007A26C 3C188016 */  lui   $t8, %hi(D_80165C18) # $t8, 0x8016
/* 07AE70 8007A270 C420ECB8 */  lwc1  $f0, %lo(D_800EECB8)($at)
/* 07AE74 8007A274 27185C18 */  addiu $t8, %lo(D_80165C18) # addiu $t8, $t8, 0x5c18
/* 07AE78 8007A278 000F7940 */  sll   $t7, $t7, 5
/* 07AE7C 8007A27C 3C198019 */  lui   $t9, %hi(D_8018C028) # $t9, 0x8019
/* 07AE80 8007A280 8F39C028 */  lw    $t9, %lo(D_8018C028)($t9)
/* 07AE84 8007A284 01F81021 */  addu  $v0, $t7, $t8
/* 07AE88 8007A288 3C080D00 */  lui   $t0, %hi(D_0D006730) # $t0, 0xd00
/* 07AE8C 8007A28C 25086730 */  addiu $t0, %lo(D_0D006730) # addiu $t0, $t0, 0x6730
/* 07AE90 8007A290 AC480074 */  sw    $t0, 0x74($v0)
/* 07AE94 8007A294 3C01800F */  lui   $at, %hi(D_800EECBC) # $at, 0x800f
/* 07AE98 8007A298 E440000C */  swc1  $f0, 0xc($v0)
/* 07AE9C 8007A29C E4400008 */  swc1  $f0, 8($v0)
/* 07AEA0 8007A2A0 E4400004 */  swc1  $f0, 4($v0)
/* 07AEA4 8007A2A4 AC590064 */  sw    $t9, 0x64($v0)
/* 07AEA8 8007A2A8 C424ECBC */  lwc1  $f4, %lo(D_800EECBC)($at)
/* 07AEAC 8007A2AC 02002025 */  move  $a0, $s0
/* 07AEB0 8007A2B0 24050010 */  li    $a1, 16
/* 07AEB4 8007A2B4 0C01C87A */  jal   func_800721E8
/* 07AEB8 8007A2B8 E4440000 */   swc1  $f4, ($v0)
/* 07AEBC 8007A2BC 3C06800E */  lui   $a2, %hi(D_800E694C) # $a2, 0x800e
/* 07AEC0 8007A2C0 24C6694C */  addiu $a2, %lo(D_800E694C) # addiu $a2, $a2, 0x694c
/* 07AEC4 8007A2C4 02002025 */  move  $a0, $s0
/* 07AEC8 8007A2C8 0C021BC4 */  jal   func_80086F10
/* 07AECC 8007A2CC 24050005 */   li    $a1, 5
/* 07AED0 8007A2D0 0C01C922 */  jal   func_80072488
/* 07AED4 8007A2D4 02002025 */   move  $a0, $s0
/* 07AED8 8007A2D8 8FBF0024 */  lw    $ra, 0x24($sp)
/* 07AEDC 8007A2DC 8FB00020 */  lw    $s0, 0x20($sp)
/* 07AEE0 8007A2E0 27BD0028 */  addiu $sp, $sp, 0x28
/* 07AEE4 8007A2E4 03E00008 */  jr    $ra
/* 07AEE8 8007A2E8 00000000 */   nop   
