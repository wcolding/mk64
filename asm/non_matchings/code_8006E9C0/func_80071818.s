glabel func_80071818
/* 072418 80071818 27BDFFE8 */  addiu $sp, $sp, -0x18
/* 07241C 8007181C AFBF0014 */  sw    $ra, 0x14($sp)
/* 072420 80071820 3C048018 */  lui   $a0, %hi(D_80183DA0) # $a0, 0x8018
/* 072424 80071824 0C01C7C0 */  jal   find_unused_obj_index
/* 072428 80071828 24843DA0 */   addiu $a0, %lo(D_80183DA0) # addiu $a0, $a0, 0x3da0
/* 07242C 8007182C 3C048018 */  lui   $a0, %hi(D_80183DB8) # $a0, 0x8018
/* 072430 80071830 0C01C7C0 */  jal   find_unused_obj_index
/* 072434 80071834 24843DB8 */   addiu $a0, %lo(D_80183DB8) # addiu $a0, $a0, 0x3db8
/* 072438 80071838 3C048018 */  lui   $a0, %hi(D_80183DBC) # $a0, 0x8018
/* 07243C 8007183C 0C01C7C0 */  jal   find_unused_obj_index
/* 072440 80071840 24843DBC */   addiu $a0, %lo(D_80183DBC) # addiu $a0, $a0, 0x3dbc
/* 072444 80071844 3C048018 */  lui   $a0, %hi(gItemWindowObjectByPlayerId) # $a0, 0x8018
/* 072448 80071848 0C01C7C0 */  jal   find_unused_obj_index
/* 07244C 8007184C 24843E88 */   addiu $a0, %lo(gItemWindowObjectByPlayerId) # addiu $a0, $a0, 0x3e88
/* 072450 80071850 3C048018 */  lui   $a0, %hi(gItemWindowObjectByPlayerId + 4) # $a0, 0x8018
/* 072454 80071854 0C01C7C0 */  jal   find_unused_obj_index
/* 072458 80071858 24843E8C */   addiu $a0, %lo(gItemWindowObjectByPlayerId + 4) # addiu $a0, $a0, 0x3e8c
/* 07245C 8007185C 0C01C064 */  jal   func_80070190
/* 072460 80071860 00000000 */   nop
/* 072464 80071864 0C01C157 */  jal   func_8007055C
/* 072468 80071868 00000000 */   nop
/* 07246C 8007186C 0C01C157 */  jal   func_8007055C
/* 072470 80071870 00000000 */   nop
/* 072474 80071874 0C01C1E0 */  jal   func_80070780
/* 072478 80071878 00000000 */   nop
/* 07247C 8007187C 3C028019 */  lui   $v0, %hi(D_8018CA70) # $v0, 0x8019
/* 072480 80071880 2442CA70 */  addiu $v0, %lo(D_8018CA70) # addiu $v0, $v0, -0x3590
/* 072484 80071884 24030032 */  li    $v1, 50
/* 072488 80071888 240EFFAE */  li    $t6, -82
/* 07248C 8007188C 240F0050 */  li    $t7, 80
/* 072490 80071890 24180078 */  li    $t8, 120
/* 072494 80071894 241900D2 */  li    $t9, 210
/* 072498 80071898 2408004B */  li    $t0, 75
/* 07249C 8007189C 24090010 */  li    $t1, 16
/* 0724A0 800718A0 240A0067 */  li    $t2, 103
/* 0724A4 800718A4 240B0028 */  li    $t3, 40
/* 0724A8 800718A8 A44E0042 */  sh    $t6, 0x42($v0)
/* 0724AC 800718AC A4430044 */  sh    $v1, 0x44($v0)
/* 0724B0 800718B0 A4400046 */  sh    $zero, 0x46($v0)
/* 0724B4 800718B4 A4400048 */  sh    $zero, 0x48($v0)
/* 0724B8 800718B8 A44F004A */  sh    $t7, 0x4a($v0)
/* 0724BC 800718BC A458004C */  sh    $t8, 0x4c($v0)
/* 0724C0 800718C0 A4430062 */  sh    $v1, 0x62($v0)
/* 0724C4 800718C4 A4590064 */  sh    $t9, 0x64($v0)
/* 0724C8 800718C8 A4400066 */  sh    $zero, 0x66($v0)
/* 0724CC 800718CC A4400068 */  sh    $zero, 0x68($v0)
/* 0724D0 800718D0 A448004E */  sh    $t0, 0x4e($v0)
/* 0724D4 800718D4 A4490058 */  sh    $t1, 0x58($v0)
/* 0724D8 800718D8 A44A005A */  sh    $t2, 0x5a($v0)
/* 0724DC 800718DC A44B0060 */  sh    $t3, 0x60($v0)
/* 0724E0 800718E0 3C048018 */  lui   $a0, %hi(gItemWindowObjectByPlayerId) # $a0, 0x8018
/* 0724E4 800718E4 0C01BB9F */  jal   func_8006EE7C
/* 0724E8 800718E8 8C843E88 */   lw    $a0, %lo(gItemWindowObjectByPlayerId)($a0)
/* 0724EC 800718EC 3C028019 */  lui   $v0, %hi(D_8018CA70) # $v0, 0x8019
/* 0724F0 800718F0 2442CA70 */  addiu $v0, %lo(D_8018CA70) # addiu $v0, $v0, -0x3590
/* 0724F4 800718F4 240C0043 */  li    $t4, 67
/* 0724F8 800718F8 240D0032 */  li    $t5, 50
/* 0724FC 800718FC 240E00F0 */  li    $t6, 240
/* 072500 80071900 240F0078 */  li    $t7, 120
/* 072504 80071904 241800C8 */  li    $t8, 200
/* 072508 80071908 241900D2 */  li    $t9, 210
/* 07250C 8007190C 240800DC */  li    $t0, 220
/* 072510 80071910 24090010 */  li    $t1, 16
/* 072514 80071914 240A00F7 */  li    $t2, 247
/* 072518 80071918 240B0028 */  li    $t3, 40
/* 07251C 8007191C A44C00C6 */  sh    $t4, 0xc6($v0)
/* 072520 80071920 A44D00C8 */  sh    $t5, 0xc8($v0)
/* 072524 80071924 A44000CA */  sh    $zero, 0xca($v0)
/* 072528 80071928 A44000CC */  sh    $zero, 0xcc($v0)
/* 07252C 8007192C A44E00CE */  sh    $t6, 0xce($v0)
/* 072530 80071930 A44F00D0 */  sh    $t7, 0xd0($v0)
/* 072534 80071934 A45800E6 */  sh    $t8, 0xe6($v0)
/* 072538 80071938 A45900E8 */  sh    $t9, 0xe8($v0)
/* 07253C 8007193C A44000EA */  sh    $zero, 0xea($v0)
/* 072540 80071940 A44000EC */  sh    $zero, 0xec($v0)
/* 072544 80071944 A44800D2 */  sh    $t0, 0xd2($v0)
/* 072548 80071948 A44900DC */  sh    $t1, 0xdc($v0)
/* 07254C 8007194C A44A00DE */  sh    $t2, 0xde($v0)
/* 072550 80071950 A44B00E4 */  sh    $t3, 0xe4($v0)
/* 072554 80071954 3C048018 */  lui   $a0, %hi(gItemWindowObjectByPlayerId + 4) # $a0, 0x8018
/* 072558 80071958 0C01BB9F */  jal   func_8006EE7C
/* 07255C 8007195C 8C843E8C */   lw    $a0, %lo(gItemWindowObjectByPlayerId + 4)($a0)
/* 072560 80071960 3C013F00 */  li    $at, 0x3F000000 # 0.500000
/* 072564 80071964 44812000 */  mtc1  $at, $f4
/* 072568 80071968 3C028019 */  lui   $v0, %hi(D_8018CA70) # $v0, 0x8019
/* 07256C 8007196C 3C048016 */  lui   $a0, %hi(gGPCurrentRaceRankByPlayerId) # $a0, 0x8016
/* 072570 80071970 248443B8 */  addiu $a0, %lo(gGPCurrentRaceRankByPlayerId) # addiu $a0, $a0, 0x43b8
/* 072574 80071974 2442CA70 */  addiu $v0, %lo(D_8018CA70) # addiu $v0, $v0, -0x3590
/* 072578 80071978 E4440088 */  swc1  $f4, 0x88($v0)
/* 07257C 8007197C 8C8C0000 */  lw    $t4, ($a0)
/* 072580 80071980 8C8D0004 */  lw    $t5, 4($a0)
/* 072584 80071984 C4460088 */  lwc1  $f6, 0x88($v0)
/* 072588 80071988 3C018019 */  lui   $at, %hi(D_8018D3C4) # $at, 0x8019
/* 07258C 8007198C 240E001E */  li    $t6, 30
/* 072590 80071990 A44C006A */  sh    $t4, 0x6a($v0)
/* 072594 80071994 A44D00EE */  sh    $t5, 0xee($v0)
/* 072598 80071998 E4460004 */  swc1  $f6, 4($v0)
/* 07259C 8007199C AC2ED3C4 */  sw    $t6, %lo(D_8018D3C4)($at)
/* 0725A0 800719A0 3C018019 */  lui   $at, %hi(D_8018D3BC) # $at, 0x8019
/* 0725A4 800719A4 240F0018 */  li    $t7, 24
/* 0725A8 800719A8 AC2FD3BC */  sw    $t7, %lo(D_8018D3BC)($at)
/* 0725AC 800719AC 3C03800E */  lui   $v1, %hi(gModeSelection) # $v1, 0x800e
/* 0725B0 800719B0 8C63C53C */  lw    $v1, %lo(gModeSelection)($v1)
/* 0725B4 800719B4 3C018019 */  lui   $at, %hi(D_8018D3C0) # $at, 0x8019
/* 0725B8 800719B8 24180028 */  li    $t8, 40
/* 0725BC 800719BC AC38D3C0 */  sw    $t8, %lo(D_8018D3C0)($at)
/* 0725C0 800719C0 3C018016 */  lui   $at, %hi(D_801657A2) # $at, 0x8016
/* 0725C4 800719C4 24190666 */  li    $t9, 1638
/* 0725C8 800719C8 10600009 */  beqz  $v1, .L800719F0
/* 0725CC 800719CC A43957A2 */   sh    $t9, %lo(D_801657A2)($at)
/* 0725D0 800719D0 24020002 */  li    $v0, 2
/* 0725D4 800719D4 1062000A */  beq   $v1, $v0, .L80071A00
/* 0725D8 800719D8 3C018019 */   lui   $at, %hi(D_8018D158) # 0x8019
/* 0725DC 800719DC 24010003 */  li    $at, 3
/* 0725E0 800719E0 10610009 */  beq   $v1, $at, .L80071A08
/* 0725E4 800719E4 00000000 */   nop
/* 0725E8 800719E8 1000000A */  b     .L80071A14
/* 0725EC 800719EC 8FBF0014 */   lw    $ra, 0x14($sp)
.L800719F0:
/* 0725F0 800719F0 24080008 */  li    $t0, 8
/* 0725F4 800719F4 3C018019 */  lui   $at, %hi(D_8018D158) # $at, 0x8019
/* 0725F8 800719F8 10000005 */  b     .L80071A10
/* 0725FC 800719FC AC28D158 */   sw    $t0, %lo(D_8018D158)($at)
.L80071A00:
/* 072600 80071A00 10000003 */  b     .L80071A10
/* 072604 80071A04 AC22D158 */   sw    $v0, %lo(D_8018D158)($at)
.L80071A08:
/* 072608 80071A08 3C018019 */  lui   $at, %hi(D_8018D158) # $at, 0x8019
/* 07260C 80071A0C AC22D158 */  sw    $v0, %lo(D_8018D158)($at)
.L80071A10:
/* 072610 80071A10 8FBF0014 */  lw    $ra, 0x14($sp)
.L80071A14:
/* 072614 80071A14 27BD0018 */  addiu $sp, $sp, 0x18
/* 072618 80071A18 03E00008 */  jr    $ra
/* 07261C 80071A1C 00000000 */   nop
