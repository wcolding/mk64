glabel func_8006B9CC
/* 06C5CC 8006B9CC 27BDFFD8 */  addiu $sp, $sp, -0x28
/* 06C5D0 8006B9D0 AFA5002C */  sw    $a1, 0x2c($sp)
/* 06C5D4 8006B9D4 83AE002F */  lb    $t6, 0x2f($sp)
/* 06C5D8 8006B9D8 3C188019 */  lui   $t8, %hi(gPlayerBalloonStatus) # $t8, 0x8019
/* 06C5DC 8006B9DC 2718D5F0 */  addiu $t8, %lo(gPlayerBalloonStatus) # addiu $t8, $t8, -0x2a10
/* 06C5E0 8006B9E0 000E7880 */  sll   $t7, $t6, 2
/* 06C5E4 8006B9E4 01EE7823 */  subu  $t7, $t7, $t6
/* 06C5E8 8006B9E8 000F7840 */  sll   $t7, $t7, 1
/* 06C5EC 8006B9EC 01F81021 */  addu  $v0, $t7, $t8
/* 06C5F0 8006B9F0 94590000 */  lhu   $t9, ($v0)
/* 06C5F4 8006B9F4 AFBF001C */  sw    $ra, 0x1c($sp)
/* 06C5F8 8006B9F8 AFA40028 */  sw    $a0, 0x28($sp)
/* 06C5FC 8006B9FC 5320000A */  beql  $t9, $zero, .L8006BA28
/* 06C600 8006BA00 94480002 */   lhu   $t0, 2($v0)
/* 06C604 8006BA04 44800000 */  mtc1  $zero, $f0
/* 06C608 8006BA08 01C03825 */  move  $a3, $t6
/* 06C60C 8006BA0C AFA00010 */  sw    $zero, 0x10($sp)
/* 06C610 8006BA10 44050000 */  mfc1  $a1, $f0
/* 06C614 8006BA14 44060000 */  mfc1  $a2, $f0
/* 06C618 8006BA18 0C01A9F0 */  jal   func_8006A7C0
/* 06C61C 8006BA1C AFA20024 */   sw    $v0, 0x24($sp)
/* 06C620 8006BA20 8FA20024 */  lw    $v0, 0x24($sp)
/* 06C624 8006BA24 94480002 */  lhu   $t0, 2($v0)
.L8006BA28:
/* 06C628 8006BA28 3C053FE6 */  lui   $a1, (0x3FE66666 >> 16) # lui $a1, 0x3fe6
/* 06C62C 8006BA2C 34A56666 */  ori   $a1, (0x3FE66666 & 0xFFFF) # ori $a1, $a1, 0x6666
/* 06C630 8006BA30 11000009 */  beqz  $t0, .L8006BA58
/* 06C634 8006BA34 8FA40028 */   lw    $a0, 0x28($sp)
/* 06C638 8006BA38 3C064026 */  lui   $a2, (0x40266666 >> 16) # lui $a2, 0x4026
/* 06C63C 8006BA3C 24090001 */  li    $t1, 1
/* 06C640 8006BA40 AFA90010 */  sw    $t1, 0x10($sp)
/* 06C644 8006BA44 34C66666 */  ori   $a2, (0x40266666 & 0xFFFF) # ori $a2, $a2, 0x6666
/* 06C648 8006BA48 83A7002F */  lb    $a3, 0x2f($sp)
/* 06C64C 8006BA4C 0C01A9F0 */  jal   func_8006A7C0
/* 06C650 8006BA50 AFA20024 */   sw    $v0, 0x24($sp)
/* 06C654 8006BA54 8FA20024 */  lw    $v0, 0x24($sp)
.L8006BA58:
/* 06C658 8006BA58 944A0004 */  lhu   $t2, 4($v0)
/* 06C65C 8006BA5C 3C05BFE6 */  lui   $a1, (0xBFE66666 >> 16) # lui $a1, 0xbfe6
/* 06C660 8006BA60 34A56666 */  ori   $a1, (0xBFE66666 & 0xFFFF) # ori $a1, $a1, 0x6666
/* 06C664 8006BA64 11400007 */  beqz  $t2, .L8006BA84
/* 06C668 8006BA68 8FA40028 */   lw    $a0, 0x28($sp)
/* 06C66C 8006BA6C 3C064026 */  lui   $a2, (0x40266666 >> 16) # lui $a2, 0x4026
/* 06C670 8006BA70 240B0002 */  li    $t3, 2
/* 06C674 8006BA74 AFAB0010 */  sw    $t3, 0x10($sp)
/* 06C678 8006BA78 34C66666 */  ori   $a2, (0x40266666 & 0xFFFF) # ori $a2, $a2, 0x6666
/* 06C67C 8006BA7C 0C01A9F0 */  jal   func_8006A7C0
/* 06C680 8006BA80 83A7002F */   lb    $a3, 0x2f($sp)
.L8006BA84:
/* 06C684 8006BA84 8FBF001C */  lw    $ra, 0x1c($sp)
/* 06C688 8006BA88 27BD0028 */  addiu $sp, $sp, 0x28
/* 06C68C 8006BA8C 03E00008 */  jr    $ra
/* 06C690 8006BA90 00000000 */   nop
