glabel func_800792D8
/* 079ED8 800792D8 27BDFFD8 */  addiu $sp, $sp, -0x28
/* 079EDC 800792DC AFBF001C */  sw    $ra, 0x1c($sp)
/* 079EE0 800792E0 14A00005 */  bnez  $a1, .L800792F8
/* 079EE4 800792E4 AFA40028 */   sw    $a0, 0x28($sp)
/* 079EE8 800792E8 3C018016 */  lui   $at, %hi(D_801656F0) # $at, 0x8016
/* 079EEC 800792EC A42056F0 */  sh    $zero, %lo(D_801656F0)($at)
/* 079EF0 800792F0 3C018019 */  lui   $at, %hi(D_8018D168) # $at, 0x8019
/* 079EF4 800792F4 AC20D168 */  sw    $zero, %lo(D_8018D168)($at)
.L800792F8:
/* 079EF8 800792F8 3C050D02 */  lui   $a1, %hi(gTLUTLakituCountdown) # $a1, 0xd02
/* 079EFC 800792FC 3C060F06 */  lui   $a2, %hi(D_0F05EB50) # $a2, 0xf06
/* 079F00 80079300 240E0048 */  li    $t6, 72
/* 079F04 80079304 AFAE0010 */  sw    $t6, 0x10($sp)
/* 079F08 80079308 24C6EB50 */  addiu $a2, %lo(D_0F05EB50) # addiu $a2, $a2, -0x14b0
/* 079F0C 8007930C 24A54ED8 */  addiu $a1, %lo(gTLUTLakituCountdown) # addiu $a1, $a1, 0x4ed8
/* 079F10 80079310 8FA40028 */  lw    $a0, 0x28($sp)
/* 079F14 80079314 0C01CD11 */  jal   func_80073444
/* 079F18 80079318 24070038 */   li    $a3, 56
/* 079F1C 8007931C 8FA40028 */  lw    $a0, 0x28($sp)
/* 079F20 80079320 3C188016 */  lui   $t8, %hi(D_80165C18) # $t8, 0x8016
/* 079F24 80079324 27185C18 */  addiu $t8, %lo(D_80165C18) # addiu $t8, $t8, 0x5c18
/* 079F28 80079328 000478C0 */  sll   $t7, $a0, 3
/* 079F2C 8007932C 01E47823 */  subu  $t7, $t7, $a0
/* 079F30 80079330 000F7940 */  sll   $t7, $t7, 5
/* 079F34 80079334 3C190D00 */  lui   $t9, %hi(D_0D005EB0) # $t9, 0xd00
/* 079F38 80079338 27395EB0 */  addiu $t9, %lo(D_0D005EB0) # addiu $t9, $t9, 0x5eb0
/* 079F3C 8007933C 01F81021 */  addu  $v0, $t7, $t8
/* 079F40 80079340 AC590074 */  sw    $t9, 0x74($v0)
/* 079F44 80079344 3C01800F */  lui   $at, %hi(D_800EEBF4) # $at, 0x800f
/* 079F48 80079348 C424EBF4 */  lwc1  $f4, %lo(D_800EEBF4)($at)
/* 079F4C 8007934C AFA20024 */  sw    $v0, 0x24($sp)
/* 079F50 80079350 24050010 */  li    $a1, 16
/* 079F54 80079354 0C01C87A */  jal   func_800721E8
/* 079F58 80079358 E4440000 */   swc1  $f4, ($v0)
/* 079F5C 8007935C 0C01C922 */  jal   func_80072488
/* 079F60 80079360 8FA40028 */   lw    $a0, 0x28($sp)
/* 079F64 80079364 8FBF001C */  lw    $ra, 0x1c($sp)
/* 079F68 80079368 3C088019 */  lui   $t0, %hi(D_8018D180) # $t0, 0x8019
/* 079F6C 8007936C 8FA20024 */  lw    $v0, 0x24($sp)
/* 079F70 80079370 8D08D180 */  lw    $t0, %lo(D_8018D180)($t0)
/* 079F74 80079374 27BD0028 */  addiu $sp, $sp, 0x28
/* 079F78 80079378 03E00008 */  jr    $ra
/* 079F7C 8007937C AC480048 */   sw    $t0, 0x48($v0)
