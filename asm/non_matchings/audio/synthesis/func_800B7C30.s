glabel func_800B7C30
/* 0B8830 800B7C30 27BDFEC0 */  addiu $sp, $sp, -0x140
/* 0B8834 800B7C34 3C18803B */  lui   $t8, %hi(gNotes) # $t8, 0x803b
/* 0B8838 800B7C38 8F181508 */  lw    $t8, %lo(gNotes)($t8)
/* 0B883C 800B7C3C 00047880 */  sll   $t7, $a0, 2
/* 0B8840 800B7C40 01E47823 */  subu  $t7, $t7, $a0
/* 0B8844 800B7C44 000F7980 */  sll   $t7, $t7, 6
/* 0B8848 800B7C48 01F8C821 */  addu  $t9, $t7, $t8
/* 0B884C 800B7C4C AFBF004C */  sw    $ra, 0x4c($sp)
/* 0B8850 800B7C50 AFBE0048 */  sw    $fp, 0x48($sp)
/* 0B8854 800B7C54 AFB70044 */  sw    $s7, 0x44($sp)
/* 0B8858 800B7C58 AFB60040 */  sw    $s6, 0x40($sp)
/* 0B885C 800B7C5C AFB5003C */  sw    $s5, 0x3c($sp)
/* 0B8860 800B7C60 AFB40038 */  sw    $s4, 0x38($sp)
/* 0B8864 800B7C64 AFB30034 */  sw    $s3, 0x34($sp)
/* 0B8868 800B7C68 AFB20030 */  sw    $s2, 0x30($sp)
/* 0B886C 800B7C6C AFB1002C */  sw    $s1, 0x2c($sp)
/* 0B8870 800B7C70 AFB00028 */  sw    $s0, 0x28($sp)
/* 0B8874 800B7C74 AFA40140 */  sw    $a0, 0x140($sp)
/* 0B8878 800B7C78 AFA50144 */  sw    $a1, 0x144($sp)
/* 0B887C 800B7C7C AFA7014C */  sw    $a3, 0x14c($sp)
/* 0B8880 800B7C80 AFA00128 */  sw    $zero, 0x128($sp)
/* 0B8884 800B7C84 AFB9008C */  sw    $t9, 0x8c($sp)
/* 0B8888 800B7C88 8CAF0000 */  lw    $t7, ($a1)
/* 0B888C 800B7C8C 24010001 */  li    $at, 1
/* 0B8890 800B7C90 00C0B825 */  move  $s7, $a2
/* 0B8894 800B7C94 000FC040 */  sll   $t8, $t7, 1
/* 0B8898 800B7C98 0018CFC2 */  srl   $t9, $t8, 0x1f
/* 0B889C 800B7C9C 17210009 */  bne   $t9, $at, .L800B7CC4
/* 0B88A0 800B7CA0 00001825 */   move  $v1, $zero
/* 0B88A4 800B7CA4 24030001 */  li    $v1, 1
/* 0B88A8 800B7CA8 A0C00000 */  sb    $zero, ($a2)
/* 0B88AC 800B7CAC ACC00008 */  sw    $zero, 8($a2)
/* 0B88B0 800B7CB0 A4C00004 */  sh    $zero, 4($a2)
/* 0B88B4 800B7CB4 A4C00010 */  sh    $zero, 0x10($a2)
/* 0B88B8 800B7CB8 A4C00012 */  sh    $zero, 0x12($a2)
/* 0B88BC 800B7CBC A0C00002 */  sb    $zero, 2($a2)
/* 0B88C0 800B7CC0 A0C00003 */  sb    $zero, 3($a2)
.L800B7CC4:
/* 0B88C4 800B7CC4 8FAE0144 */  lw    $t6, 0x144($sp)
/* 0B88C8 800B7CC8 02E03025 */  move  $a2, $s7
/* 0B88CC 800B7CCC 95CF000A */  lhu   $t7, 0xa($t6)
/* 0B88D0 800B7CD0 A7AF0112 */  sh    $t7, 0x112($sp)
/* 0B88D4 800B7CD4 95D80000 */  lhu   $t8, ($t6)
/* 0B88D8 800B7CD8 33190001 */  andi  $t9, $t8, 1
/* 0B88DC 800B7CDC 8FB80150 */  lw    $t8, 0x150($sp)
/* 0B88E0 800B7CE0 272E0001 */  addiu $t6, $t9, 1
/* 0B88E4 800B7CE4 AFAE00B0 */  sw    $t6, 0xb0($sp)
/* 0B88E8 800B7CE8 01F80019 */  multu $t7, $t8
/* 0B88EC 800B7CEC 96EF0004 */  lhu   $t7, 4($s7)
/* 0B88F0 800B7CF0 0000C812 */  mflo  $t9
/* 0B88F4 800B7CF4 00197040 */  sll   $t6, $t9, 1
/* 0B88F8 800B7CF8 01CF1021 */  addu  $v0, $t6, $t7
/* 0B88FC 800B7CFC A6E20004 */  sh    $v0, 4($s7)
/* 0B8900 800B7D00 8FB90144 */  lw    $t9, 0x144($sp)
/* 0B8904 800B7D04 00028402 */  srl   $s0, $v0, 0x10
/* 0B8908 800B7D08 02003825 */  move  $a3, $s0
/* 0B890C 800B7D0C 8F240000 */  lw    $a0, ($t9)
/* 0B8910 800B7D10 8FB80144 */  lw    $t8, 0x144($sp)
/* 0B8914 800B7D14 03202825 */  move  $a1, $t9
/* 0B8918 800B7D18 00047B80 */  sll   $t7, $a0, 0xe
/* 0B891C 800B7D1C 05E3000E */  bgezl $t7, .L800B7D58
/* 0B8920 800B7D20 8F0E000C */   lw    $t6, 0xc($t8)
/* 0B8924 800B7D24 0C02E16D */  jal   load_wave_samples
/* 0B8928 800B7D28 8FA40154 */   lw    $a0, 0x154($sp)
/* 0B892C 800B7D2C 8EE60008 */  lw    $a2, 8($s7)
/* 0B8930 800B7D30 0006C040 */  sll   $t8, $a2, 1
/* 0B8934 800B7D34 270E01A0 */  addiu $t6, $t8, 0x1a0
/* 0B8938 800B7D38 A7AE009E */  sh    $t6, 0x9e($sp)
/* 0B893C 800B7D3C 00D07821 */  addu  $t7, $a2, $s0
/* 0B8940 800B7D40 AEEF0008 */  sw    $t7, 8($s7)
/* 0B8944 800B7D44 8FB90144 */  lw    $t9, 0x144($sp)
/* 0B8948 800B7D48 8F240000 */  lw    $a0, ($t9)
/* 0B894C 800B7D4C 100001CF */  b     .L800B848C
/* 0B8950 800B7D50 AFA20154 */   sw    $v0, 0x154($sp)
/* 0B8954 800B7D54 8F0E000C */  lw    $t6, 0xc($t8)
.L800B7D58:
/* 0B8958 800B7D58 8DCF0000 */  lw    $t7, ($t6)
/* 0B895C 800B7D5C AFAF0130 */  sw    $t7, 0x130($sp)
/* 0B8960 800B7D60 8DF90008 */  lw    $t9, 8($t7)
/* 0B8964 800B7D64 AFB9012C */  sw    $t9, 0x12c($sp)
/* 0B8968 800B7D68 8F380004 */  lw    $t8, 4($t9)
/* 0B896C 800B7D6C 8FB900B0 */  lw    $t9, 0xb0($sp)
/* 0B8970 800B7D70 AFB800DC */  sw    $t8, 0xdc($sp)
/* 0B8974 800B7D74 8DEE0004 */  lw    $t6, 4($t7)
/* 0B8978 800B7D78 AFA000A0 */  sw    $zero, 0xa0($sp)
/* 0B897C 800B7D7C AFA000AC */  sw    $zero, 0xac($sp)
/* 0B8980 800B7D80 1B2001C2 */  blez  $t9, .L800B848C
/* 0B8984 800B7D84 AFAE00EC */   sw    $t6, 0xec($sp)
/* 0B8988 800B7D88 0002C402 */  srl   $t8, $v0, 0x10
/* 0B898C 800B7D8C AFB80050 */  sw    $t8, 0x50($sp)
/* 0B8990 800B7D90 AFA30114 */  sw    $v1, 0x114($sp)
/* 0B8994 800B7D94 8FB20154 */  lw    $s2, 0x154($sp)
/* 0B8998 800B7D98 8FAF0130 */  lw    $t7, 0x130($sp)
.L800B7D9C:
/* 0B899C 800B7D9C 8FAE00B0 */  lw    $t6, 0xb0($sp)
/* 0B89A0 800B7DA0 24010001 */  li    $at, 1
/* 0B89A4 800B7DA4 8DE5000C */  lw    $a1, 0xc($t7)
/* 0B89A8 800B7DA8 0000F025 */  move  $fp, $zero
/* 0B89AC 800B7DAC 0000A025 */  move  $s4, $zero
/* 0B89B0 800B7DB0 15C10003 */  bne   $t6, $at, .L800B7DC0
/* 0B89B4 800B7DB4 24A60008 */   addiu $a2, $a1, 8
/* 0B89B8 800B7DB8 1000000B */  b     .L800B7DE8
/* 0B89BC 800B7DBC 8FAD0050 */   lw    $t5, 0x50($sp)
.L800B7DC0:
/* 0B89C0 800B7DC0 8FB90050 */  lw    $t9, 0x50($sp)
/* 0B89C4 800B7DC4 8FAE00AC */  lw    $t6, 0xac($sp)
/* 0B89C8 800B7DC8 8FAD0050 */  lw    $t5, 0x50($sp)
/* 0B89CC 800B7DCC 33380001 */  andi  $t8, $t9, 1
/* 0B89D0 800B7DD0 13000005 */  beqz  $t8, .L800B7DE8
/* 0B89D4 800B7DD4 2401FFFE */   li    $at, -2
/* 0B89D8 800B7DD8 03217824 */  and   $t7, $t9, $at
/* 0B89DC 800B7DDC 000EC040 */  sll   $t8, $t6, 1
/* 0B89E0 800B7DE0 10000001 */  b     .L800B7DE8
/* 0B89E4 800B7DE4 01F86821 */   addu  $t5, $t7, $t8
.L800B7DE8:
/* 0B89E8 800B7DE8 8FB90128 */  lw    $t9, 0x128($sp)
/* 0B89EC 800B7DEC 5326001B */  beql  $t9, $a2, .L800B7E5C
/* 0B89F0 800B7DF0 00047AC0 */   sll   $t7, $a0, 0xb
/* 0B89F4 800B7DF4 8CAE0000 */  lw    $t6, ($a1)
/* 0B89F8 800B7DF8 8CB80004 */  lw    $t8, 4($a1)
/* 0B89FC 800B7DFC 3C0100FF */  lui   $at, (0x00FFFFFF >> 16) # lui $at, 0xff
/* 0B8A00 800B7E00 000E7900 */  sll   $t7, $t6, 4
/* 0B8A04 800B7E04 01F80019 */  multu $t7, $t8
/* 0B8A08 800B7E08 3421FFFF */  ori   $at, (0x00FFFFFF & 0xFFFF) # ori $at, $at, 0xffff
/* 0B8A0C 800B7E0C 02401025 */  move  $v0, $s2
/* 0B8A10 800B7E10 26520008 */  addiu $s2, $s2, 8
/* 0B8A14 800B7E14 00001812 */  mflo  $v1
/* 0B8A18 800B7E18 0061C824 */  and   $t9, $v1, $at
/* 0B8A1C 800B7E1C 3C010B00 */  lui   $at, 0xb00
/* 0B8A20 800B7E20 03217025 */  or    $t6, $t9, $at
/* 0B8A24 800B7E24 AC4E0000 */  sw    $t6, ($v0)
/* 0B8A28 800B7E28 8FAF0144 */  lw    $t7, 0x144($sp)
/* 0B8A2C 800B7E2C 3C018000 */  lui   $at, 0x8000
/* 0B8A30 800B7E30 8DF80000 */  lw    $t8, ($t7)
/* 0B8A34 800B7E34 0018CAC0 */  sll   $t9, $t8, 0xb
/* 0B8A38 800B7E38 00197742 */  srl   $t6, $t9, 0x1d
/* 0B8A3C 800B7E3C 000E7840 */  sll   $t7, $t6, 1
/* 0B8A40 800B7E40 01E6C021 */  addu  $t8, $t7, $a2
/* 0B8A44 800B7E44 0301C821 */  addu  $t9, $t8, $at
/* 0B8A48 800B7E48 AC590004 */  sw    $t9, 4($v0)
/* 0B8A4C 800B7E4C 8FAE0144 */  lw    $t6, 0x144($sp)
/* 0B8A50 800B7E50 8DC40000 */  lw    $a0, ($t6)
/* 0B8A54 800B7E54 AFA60128 */  sw    $a2, 0x128($sp)
/* 0B8A58 800B7E58 00047AC0 */  sll   $t7, $a0, 0xb
.L800B7E5C:
/* 0B8A5C 800B7E5C 000FC742 */  srl   $t8, $t7, 0x1d
/* 0B8A60 800B7E60 13000003 */  beqz  $t8, .L800B7E70
/* 0B8A64 800B7E64 3C19800F */   lui   $t9, %hi(gUnknownData_800F6290) # $t9, 0x800f
/* 0B8A68 800B7E68 27396290 */  addiu $t9, %lo(gUnknownData_800F6290) # addiu $t9, $t9, 0x6290
/* 0B8A6C 800B7E6C AFB90128 */  sw    $t9, 0x128($sp)
.L800B7E70:
/* 0B8A70 800B7E70 51A0011F */  beql  $t5, $zero, .L800B82F0
/* 0B8A74 800B7E74 8FB900B0 */   lw    $t9, 0xb0($sp)
/* 0B8A78 800B7E78 8EE60008 */  lw    $a2, 8($s7)
.L800B7E7C:
/* 0B8A7C 800B7E7C 8FAE00DC */  lw    $t6, 0xdc($sp)
/* 0B8A80 800B7E80 01BE2023 */  subu  $a0, $t5, $fp
/* 0B8A84 800B7E84 30D3000F */  andi  $s3, $a2, 0xf
/* 0B8A88 800B7E88 00005825 */  move  $t3, $zero
/* 0B8A8C 800B7E8C 00006025 */  move  $t4, $zero
/* 0B8A90 800B7E90 0080F825 */  move  $ra, $a0
/* 0B8A94 800B7E94 16600005 */  bnez  $s3, .L800B7EAC
/* 0B8A98 800B7E98 01C61823 */   subu  $v1, $t6, $a2
/* 0B8A9C 800B7E9C 92EF0000 */  lbu   $t7, ($s7)
/* 0B8AA0 800B7EA0 55E00003 */  bnel  $t7, $zero, .L800B7EB0
/* 0B8AA4 800B7EA4 24180010 */   li    $t8, 16
/* 0B8AA8 800B7EA8 24130010 */  li    $s3, 16
.L800B7EAC:
/* 0B8AAC 800B7EAC 24180010 */  li    $t8, 16
.L800B7EB0:
/* 0B8AB0 800B7EB0 03132823 */  subu  $a1, $t8, $s3
/* 0B8AB4 800B7EB4 0083082A */  slt   $at, $a0, $v1
/* 0B8AB8 800B7EB8 1020000C */  beqz  $at, .L800B7EEC
/* 0B8ABC 800B7EBC 00A0A825 */   move  $s5, $a1
/* 0B8AC0 800B7EC0 00851023 */  subu  $v0, $a0, $a1
/* 0B8AC4 800B7EC4 2442000F */  addiu $v0, $v0, 0xf
/* 0B8AC8 800B7EC8 04410003 */  bgez  $v0, .L800B7ED8
/* 0B8ACC 800B7ECC 0002C903 */   sra   $t9, $v0, 4
/* 0B8AD0 800B7ED0 2441000F */  addiu $at, $v0, 0xf
/* 0B8AD4 800B7ED4 0001C903 */  sra   $t9, $at, 4
.L800B7ED8:
/* 0B8AD8 800B7ED8 00198900 */  sll   $s1, $t9, 4
/* 0B8ADC 800B7EDC 00B17021 */  addu  $t6, $a1, $s1
/* 0B8AE0 800B7EE0 03204025 */  move  $t0, $t9
/* 0B8AE4 800B7EE4 10000013 */  b     .L800B7F34
/* 0B8AE8 800B7EE8 01C4B023 */   subu  $s6, $t6, $a0
.L800B7EEC:
/* 0B8AEC 800B7EEC 00658823 */  subu  $s1, $v1, $a1
/* 0B8AF0 800B7EF0 1E200003 */  bgtz  $s1, .L800B7F00
/* 0B8AF4 800B7EF4 0000B025 */   move  $s6, $zero
/* 0B8AF8 800B7EF8 00008825 */  move  $s1, $zero
/* 0B8AFC 800B7EFC 0060A825 */  move  $s5, $v1
.L800B7F00:
/* 0B8B00 800B7F00 8FB8012C */  lw    $t8, 0x12c($sp)
/* 0B8B04 800B7F04 2628000F */  addiu $t0, $s1, 0xf
/* 0B8B08 800B7F08 05010003 */  bgez  $t0, .L800B7F18
/* 0B8B0C 800B7F0C 00087903 */   sra   $t7, $t0, 4
/* 0B8B10 800B7F10 2501000F */  addiu $at, $t0, 0xf
/* 0B8B14 800B7F14 00017903 */  sra   $t7, $at, 4
.L800B7F18:
/* 0B8B18 800B7F18 8F190008 */  lw    $t9, 8($t8)
/* 0B8B1C 800B7F1C 01E04025 */  move  $t0, $t7
/* 0B8B20 800B7F20 53200004 */  beql  $t9, $zero, .L800B7F34
/* 0B8B24 800B7F24 240B0001 */   li    $t3, 1
/* 0B8B28 800B7F28 10000002 */  b     .L800B7F34
/* 0B8B2C 800B7F2C 240C0001 */   li    $t4, 1
/* 0B8B30 800B7F30 240B0001 */  li    $t3, 1
.L800B7F34:
/* 0B8B34 800B7F34 11000044 */  beqz  $t0, .L800B8048
/* 0B8B38 800B7F38 00005025 */   move  $t2, $zero
/* 0B8B3C 800B7F3C 8FAF0130 */  lw    $t7, 0x130($sp)
/* 0B8B40 800B7F40 00D31023 */  subu  $v0, $a2, $s3
/* 0B8B44 800B7F44 24420010 */  addiu $v0, $v0, 0x10
/* 0B8B48 800B7F48 91F80001 */  lbu   $t8, 1($t7)
/* 0B8B4C 800B7F4C 04410003 */  bgez  $v0, .L800B7F5C
/* 0B8B50 800B7F50 00027103 */   sra   $t6, $v0, 4
/* 0B8B54 800B7F54 2441000F */  addiu $at, $v0, 0xf
/* 0B8B58 800B7F58 00017103 */  sra   $t6, $at, 4
.L800B7F5C:
/* 0B8B5C 800B7F5C 24010081 */  li    $at, 129
/* 0B8B60 800B7F60 1701000C */  bne   $t8, $at, .L800B7F94
/* 0B8B64 800B7F64 01C01025 */   move  $v0, $t6
/* 0B8B68 800B7F68 000EC8C0 */  sll   $t9, $t6, 3
/* 0B8B6C 800B7F6C 000880C0 */  sll   $s0, $t0, 3
/* 0B8B70 800B7F70 02088021 */  addu  $s0, $s0, $t0
/* 0B8B74 800B7F74 032EC821 */  addu  $t9, $t9, $t6
/* 0B8B78 800B7F78 8FAE00EC */  lw    $t6, 0xec($sp)
/* 0B8B7C 800B7F7C 2610001F */  addiu $s0, $s0, 0x1f
/* 0B8B80 800B7F80 2401FFF0 */  li    $at, -16
/* 0B8B84 800B7F84 02017824 */  and   $t7, $s0, $at
/* 0B8B88 800B7F88 01E08025 */  move  $s0, $t7
/* 0B8B8C 800B7F8C 10000019 */  b     .L800B7FF4
/* 0B8B90 800B7F90 032E2021 */   addu  $a0, $t9, $t6
.L800B7F94:
/* 0B8B94 800B7F94 24030009 */  li    $v1, 9
/* 0B8B98 800B7F98 00430019 */  multu $v0, $v1
/* 0B8B9C 800B7F9C 8FB900EC */  lw    $t9, 0xec($sp)
/* 0B8BA0 800B7FA0 2401FFF0 */  li    $at, -16
/* 0B8BA4 800B7FA4 AFBF00D8 */  sw    $ra, 0xd8($sp)
/* 0B8BA8 800B7FA8 8FA60114 */  lw    $a2, 0x114($sp)
/* 0B8BAC 800B7FAC 26E70001 */  addiu $a3, $s7, 1
/* 0B8BB0 800B7FB0 AFAB011C */  sw    $t3, 0x11c($sp)
/* 0B8BB4 800B7FB4 AFAC0118 */  sw    $t4, 0x118($sp)
/* 0B8BB8 800B7FB8 AFAD00E4 */  sw    $t5, 0xe4($sp)
/* 0B8BBC 800B7FBC 0000C012 */  mflo  $t8
/* 0B8BC0 800B7FC0 03192021 */  addu  $a0, $t8, $t9
/* 0B8BC4 800B7FC4 00000000 */  nop   
/* 0B8BC8 800B7FC8 01030019 */  multu $t0, $v1
/* 0B8BCC 800B7FCC 00008012 */  mflo  $s0
/* 0B8BD0 800B7FD0 2610001F */  addiu $s0, $s0, 0x1f
/* 0B8BD4 800B7FD4 02012824 */  and   $a1, $s0, $at
/* 0B8BD8 800B7FD8 0C02EB43 */  jal   func_800BAD0C
/* 0B8BDC 800B7FDC 00A08025 */   move  $s0, $a1
/* 0B8BE0 800B7FE0 8FAB011C */  lw    $t3, 0x11c($sp)
/* 0B8BE4 800B7FE4 8FAC0118 */  lw    $t4, 0x118($sp)
/* 0B8BE8 800B7FE8 8FAD00E4 */  lw    $t5, 0xe4($sp)
/* 0B8BEC 800B7FEC 8FBF00D8 */  lw    $ra, 0xd8($sp)
/* 0B8BF0 800B7FF0 00402025 */  move  $a0, $v0
.L800B7FF4:
/* 0B8BF4 800B7FF4 240F0540 */  li    $t7, 1344
/* 0B8BF8 800B7FF8 01F04823 */  subu  $t1, $t7, $s0
/* 0B8BFC 800B7FFC 0009C400 */  sll   $t8, $t1, 0x10
/* 0B8C00 800B8000 00107103 */  sra   $t6, $s0, 4
/* 0B8C04 800B8004 31CF00FF */  andi  $t7, $t6, 0xff
/* 0B8C08 800B8008 00184C03 */  sra   $t1, $t8, 0x10
/* 0B8C0C 800B800C 000FC400 */  sll   $t8, $t7, 0x10
/* 0B8C10 800B8010 3C011400 */  lui   $at, 0x1400
/* 0B8C14 800B8014 0301C825 */  or    $t9, $t8, $at
/* 0B8C18 800B8018 308A000F */  andi  $t2, $a0, 0xf
/* 0B8C1C 800B801C 312EFFFF */  andi  $t6, $t1, 0xffff
/* 0B8C20 800B8020 032E7825 */  or    $t7, $t9, $t6
/* 0B8C24 800B8024 008AC023 */  subu  $t8, $a0, $t2
/* 0B8C28 800B8028 3C018000 */  lui   $at, 0x8000
/* 0B8C2C 800B802C 02401825 */  move  $v1, $s2
/* 0B8C30 800B8030 0301C821 */  addu  $t9, $t8, $at
/* 0B8C34 800B8034 AC790004 */  sw    $t9, 4($v1)
/* 0B8C38 800B8038 AC6F0000 */  sw    $t7, ($v1)
/* 0B8C3C 800B803C 92E50000 */  lbu   $a1, ($s7)
/* 0B8C40 800B8040 1000000B */  b     .L800B8070
/* 0B8C44 800B8044 26520008 */   addiu $s2, $s2, 8
.L800B8048:
/* 0B8C48 800B8048 000870C0 */  sll   $t6, $t0, 3
/* 0B8C4C 800B804C 01C87021 */  addu  $t6, $t6, $t0
/* 0B8C50 800B8050 25CF001F */  addiu $t7, $t6, 0x1f
/* 0B8C54 800B8054 31F8FFF0 */  andi  $t8, $t7, 0xfff0
/* 0B8C58 800B8058 24190540 */  li    $t9, 1344
/* 0B8C5C 800B805C 03384823 */  subu  $t1, $t9, $t8
/* 0B8C60 800B8060 00097400 */  sll   $t6, $t1, 0x10
/* 0B8C64 800B8064 00008825 */  move  $s1, $zero
/* 0B8C68 800B8068 000E4C03 */  sra   $t1, $t6, 0x10
/* 0B8C6C 800B806C 92E50000 */  lbu   $a1, ($s7)
.L800B8070:
/* 0B8C70 800B8070 10A0000D */  beqz  $a1, .L800B80A8
/* 0B8C74 800B8074 02401025 */   move  $v0, $s2
/* 0B8C78 800B8078 3C190F00 */  lui   $t9, 0xf00
/* 0B8C7C 800B807C AC590000 */  sw    $t9, ($v0)
/* 0B8C80 800B8080 8FB80130 */  lw    $t8, 0x130($sp)
/* 0B8C84 800B8084 3C018000 */  lui   $at, (0x80000010 >> 16) # lui $at, 0x8000
/* 0B8C88 800B8088 34210010 */  ori   $at, (0x80000010 & 0xFFFF) # ori $at, $at, 0x10
/* 0B8C8C 800B808C 8F0E0008 */  lw    $t6, 8($t8)
/* 0B8C90 800B8090 24190002 */  li    $t9, 2
/* 0B8C94 800B8094 26520008 */  addiu $s2, $s2, 8
/* 0B8C98 800B8098 01C17821 */  addu  $t7, $t6, $at
/* 0B8C9C 800B809C AC4F0004 */  sw    $t7, 4($v0)
/* 0B8CA0 800B80A0 AFB90114 */  sw    $t9, 0x114($sp)
/* 0B8CA4 800B80A4 A2E00000 */  sb    $zero, ($s7)
.L800B80A8:
/* 0B8CA8 800B80A8 17C0001E */  bnez  $fp, .L800B8124
/* 0B8CAC 800B80AC 02401825 */   move  $v1, $s2
/* 0B8CB0 800B80B0 012AC021 */  addu  $t8, $t1, $t2
/* 0B8CB4 800B80B4 330EFFFF */  andi  $t6, $t8, 0xffff
/* 0B8CB8 800B80B8 3C010800 */  lui   $at, 0x800
/* 0B8CBC 800B80BC 01C17825 */  or    $t7, $t6, $at
/* 0B8CC0 800B80C0 00113040 */  sll   $a2, $s1, 1
/* 0B8CC4 800B80C4 30D9FFFF */  andi  $t9, $a2, 0xffff
/* 0B8CC8 800B80C8 3C0101A0 */  lui   $at, 0x1a0
/* 0B8CCC 800B80CC 02401025 */  move  $v0, $s2
/* 0B8CD0 800B80D0 0321C025 */  or    $t8, $t9, $at
/* 0B8CD4 800B80D4 AC580004 */  sw    $t8, 4($v0)
/* 0B8CD8 800B80D8 AC4F0000 */  sw    $t7, ($v0)
/* 0B8CDC 800B80DC 8FAE0114 */  lw    $t6, 0x114($sp)
/* 0B8CE0 800B80E0 26520008 */  addiu $s2, $s2, 8
/* 0B8CE4 800B80E4 3C010100 */  lui   $at, 0x100
/* 0B8CE8 800B80E8 31CF00FF */  andi  $t7, $t6, 0xff
/* 0B8CEC 800B80EC 000FCC00 */  sll   $t9, $t7, 0x10
/* 0B8CF0 800B80F0 0321C025 */  or    $t8, $t9, $at
/* 0B8CF4 800B80F4 02401825 */  move  $v1, $s2
/* 0B8CF8 800B80F8 AC780000 */  sw    $t8, ($v1)
/* 0B8CFC 800B80FC 8EEE000C */  lw    $t6, 0xc($s7)
/* 0B8D00 800B8100 3C018000 */  lui   $at, 0x8000
/* 0B8D04 800B8104 0235C021 */  addu  $t8, $s1, $s5
/* 0B8D08 800B8108 01C17821 */  addu  $t7, $t6, $at
/* 0B8D0C 800B810C AC6F0004 */  sw    $t7, 4($v1)
/* 0B8D10 800B8110 0013C840 */  sll   $t9, $s3, 1
/* 0B8D14 800B8114 26520008 */  addiu $s2, $s2, 8
/* 0B8D18 800B8118 AFB900FC */  sw    $t9, 0xfc($sp)
/* 0B8D1C 800B811C 10000030 */  b     .L800B81E0
/* 0B8D20 800B8120 03163823 */   subu  $a3, $t8, $s6
.L800B8124:
/* 0B8D24 800B8124 012A7021 */  addu  $t6, $t1, $t2
/* 0B8D28 800B8128 31CFFFFF */  andi  $t7, $t6, 0xffff
/* 0B8D2C 800B812C 3C010800 */  lui   $at, 0x800
/* 0B8D30 800B8130 01E1C825 */  or    $t9, $t7, $at
/* 0B8D34 800B8134 2401FFF0 */  li    $at, -16
/* 0B8D38 800B8138 2682001F */  addiu $v0, $s4, 0x1f
/* 0B8D3C 800B813C 0041C024 */  and   $t8, $v0, $at
/* 0B8D40 800B8140 270F01A0 */  addiu $t7, $t8, 0x1a0
/* 0B8D44 800B8144 03001025 */  move  $v0, $t8
/* 0B8D48 800B8148 AC790000 */  sw    $t9, ($v1)
/* 0B8D4C 800B814C 00113040 */  sll   $a2, $s1, 1
/* 0B8D50 800B8150 30D8FFFF */  andi  $t8, $a2, 0xffff
/* 0B8D54 800B8154 000FCC00 */  sll   $t9, $t7, 0x10
/* 0B8D58 800B8158 03387025 */  or    $t6, $t9, $t8
/* 0B8D5C 800B815C AC6E0004 */  sw    $t6, 4($v1)
/* 0B8D60 800B8160 8FAF0114 */  lw    $t7, 0x114($sp)
/* 0B8D64 800B8164 26520008 */  addiu $s2, $s2, 8
/* 0B8D68 800B8168 3C010100 */  lui   $at, 0x100
/* 0B8D6C 800B816C 31F900FF */  andi  $t9, $t7, 0xff
/* 0B8D70 800B8170 0019C400 */  sll   $t8, $t9, 0x10
/* 0B8D74 800B8174 03017025 */  or    $t6, $t8, $at
/* 0B8D78 800B8178 02402025 */  move  $a0, $s2
/* 0B8D7C 800B817C AC8E0000 */  sw    $t6, ($a0)
/* 0B8D80 800B8180 8EEF000C */  lw    $t7, 0xc($s7)
/* 0B8D84 800B8184 3C018000 */  lui   $at, 0x8000
/* 0B8D88 800B8188 0013C040 */  sll   $t8, $s3, 1
/* 0B8D8C 800B818C 01E1C821 */  addu  $t9, $t7, $at
/* 0B8D90 800B8190 3C0100FF */  lui   $at, (0x00FFFFFF >> 16) # lui $at, 0xff
/* 0B8D94 800B8194 00587021 */  addu  $t6, $v0, $t8
/* 0B8D98 800B8198 AC990004 */  sw    $t9, 4($a0)
/* 0B8D9C 800B819C 25CF01A0 */  addiu $t7, $t6, 0x1a0
/* 0B8DA0 800B81A0 3421FFFF */  ori   $at, (0x00FFFFFF & 0xFFFF) # ori $at, $at, 0xffff
/* 0B8DA4 800B81A4 01E1C824 */  and   $t9, $t7, $at
/* 0B8DA8 800B81A8 3C010A00 */  lui   $at, 0xa00
/* 0B8DAC 800B81AC 26520008 */  addiu $s2, $s2, 8
/* 0B8DB0 800B81B0 0321C025 */  or    $t8, $t9, $at
/* 0B8DB4 800B81B4 02357021 */  addu  $t6, $s1, $s5
/* 0B8DB8 800B81B8 01D63823 */  subu  $a3, $t6, $s6
/* 0B8DBC 800B81BC 02402825 */  move  $a1, $s2
/* 0B8DC0 800B81C0 ACB80000 */  sw    $t8, ($a1)
/* 0B8DC4 800B81C4 00077040 */  sll   $t6, $a3, 1
/* 0B8DC8 800B81C8 269901A0 */  addiu $t9, $s4, 0x1a0
/* 0B8DCC 800B81CC 0019C400 */  sll   $t8, $t9, 0x10
/* 0B8DD0 800B81D0 31CFFFFF */  andi  $t7, $t6, 0xffff
/* 0B8DD4 800B81D4 030FC825 */  or    $t9, $t8, $t7
/* 0B8DD8 800B81D8 ACB90004 */  sw    $t9, 4($a1)
/* 0B8DDC 800B81DC 26520008 */  addiu $s2, $s2, 8
.L800B81E0:
/* 0B8DE0 800B81E0 8FAE0114 */  lw    $t6, 0x114($sp)
/* 0B8DE4 800B81E4 24010001 */  li    $at, 1
/* 0B8DE8 800B81E8 03C7F021 */  addu  $fp, $fp, $a3
/* 0B8DEC 800B81EC 11C10006 */  beq   $t6, $at, .L800B8208
/* 0B8DF0 800B81F0 24180020 */   li    $t8, 32
/* 0B8DF4 800B81F4 39C20002 */  xori  $v0, $t6, 2
/* 0B8DF8 800B81F8 10400007 */  beqz  $v0, .L800B8218
/* 0B8DFC 800B81FC AFA00114 */   sw    $zero, 0x114($sp)
/* 0B8E00 800B8200 10000008 */  b     .L800B8224
/* 0B8E04 800B8204 0007C840 */   sll   $t9, $a3, 1
.L800B8208:
/* 0B8E08 800B8208 AFB800FC */  sw    $t8, 0xfc($sp)
/* 0B8E0C 800B820C 24D40020 */  addiu $s4, $a2, 0x20
/* 0B8E10 800B8210 1000000B */  b     .L800B8240
/* 0B8E14 800B8214 AFA00114 */   sw    $zero, 0x114($sp)
.L800B8218:
/* 0B8E18 800B8218 00077840 */  sll   $t7, $a3, 1
/* 0B8E1C 800B821C 10000008 */  b     .L800B8240
/* 0B8E20 800B8220 01F4A021 */   addu  $s4, $t7, $s4
.L800B8224:
/* 0B8E24 800B8224 52800004 */  beql  $s4, $zero, .L800B8238
/* 0B8E28 800B8228 0267A021 */   addu  $s4, $s3, $a3
/* 0B8E2C 800B822C 10000004 */  b     .L800B8240
/* 0B8E30 800B8230 0334A021 */   addu  $s4, $t9, $s4
/* 0B8E34 800B8234 0267A021 */  addu  $s4, $s3, $a3
.L800B8238:
/* 0B8E38 800B8238 00147040 */  sll   $t6, $s4, 1
/* 0B8E3C 800B823C 01C0A025 */  move  $s4, $t6
.L800B8240:
/* 0B8E40 800B8240 1160001E */  beqz  $t3, .L800B82BC
/* 0B8E44 800B8244 240F0001 */   li    $t7, 1
/* 0B8E48 800B8248 3C0100FF */  lui   $at, (0x00FFFFFF >> 16) # lui $at, 0xff
/* 0B8E4C 800B824C 3421FFFF */  ori   $at, (0x00FFFFFF & 0xFFFF) # ori $at, $at, 0xffff
/* 0B8E50 800B8250 269801A0 */  addiu $t8, $s4, 0x1a0
/* 0B8E54 800B8254 03017824 */  and   $t7, $t8, $at
/* 0B8E58 800B8258 8FB00144 */  lw    $s0, 0x144($sp)
/* 0B8E5C 800B825C 8FB1008C */  lw    $s1, 0x8c($sp)
/* 0B8E60 800B8260 3C010200 */  lui   $at, 0x200
/* 0B8E64 800B8264 02401025 */  move  $v0, $s2
/* 0B8E68 800B8268 01BE7023 */  subu  $t6, $t5, $fp
/* 0B8E6C 800B826C 000EC040 */  sll   $t8, $t6, 1
/* 0B8E70 800B8270 01E1C825 */  or    $t9, $t7, $at
/* 0B8E74 800B8274 AC590000 */  sw    $t9, ($v0)
/* 0B8E78 800B8278 AC580004 */  sw    $t8, 4($v0)
/* 0B8E7C 800B827C 92190000 */  lbu   $t9, ($s0)
/* 0B8E80 800B8280 26520008 */  addiu $s2, $s2, 8
/* 0B8E84 800B8284 372E0020 */  ori   $t6, $t9, 0x20
/* 0B8E88 800B8288 A20E0000 */  sb    $t6, ($s0)
/* 0B8E8C 800B828C 922F00B0 */  lbu   $t7, 0xb0($s1)
/* 0B8E90 800B8290 35F90020 */  ori   $t9, $t7, 0x20
/* 0B8E94 800B8294 A23900B0 */  sb    $t9, 0xb0($s1)
/* 0B8E98 800B8298 922E00B0 */  lbu   $t6, 0xb0($s1)
/* 0B8E9C 800B829C 31D8FF7F */  andi  $t8, $t6, 0xff7f
/* 0B8EA0 800B82A0 A23800B0 */  sb    $t8, 0xb0($s1)
/* 0B8EA4 800B82A4 AFAD00E4 */  sw    $t5, 0xe4($sp)
/* 0B8EA8 800B82A8 8FA50140 */  lw    $a1, 0x140($sp)
/* 0B8EAC 800B82AC 0C02DBED */  jal   func_800B6FB4
/* 0B8EB0 800B82B0 8FA40158 */   lw    $a0, 0x158($sp)
/* 0B8EB4 800B82B4 1000000D */  b     .L800B82EC
/* 0B8EB8 800B82B8 8FAD00E4 */   lw    $t5, 0xe4($sp)
.L800B82BC:
/* 0B8EBC 800B82BC 51800007 */  beql  $t4, $zero, .L800B82DC
/* 0B8EC0 800B82C0 8EF80008 */   lw    $t8, 8($s7)
/* 0B8EC4 800B82C4 A2EF0000 */  sb    $t7, ($s7)
/* 0B8EC8 800B82C8 8FB9012C */  lw    $t9, 0x12c($sp)
/* 0B8ECC 800B82CC 8F2E0000 */  lw    $t6, ($t9)
/* 0B8ED0 800B82D0 10000004 */  b     .L800B82E4
/* 0B8ED4 800B82D4 AEEE0008 */   sw    $t6, 8($s7)
/* 0B8ED8 800B82D8 8EF80008 */  lw    $t8, 8($s7)
.L800B82DC:
/* 0B8EDC 800B82DC 031F7821 */  addu  $t7, $t8, $ra
/* 0B8EE0 800B82E0 AEEF0008 */  sw    $t7, 8($s7)
.L800B82E4:
/* 0B8EE4 800B82E4 57CDFEE5 */  bnel  $fp, $t5, .L800B7E7C
/* 0B8EE8 800B82E8 8EE60008 */   lw    $a2, 8($s7)
.L800B82EC:
/* 0B8EEC 800B82EC 8FB900B0 */  lw    $t9, 0xb0($sp)
.L800B82F0:
/* 0B8EF0 800B82F0 24010001 */  li    $at, 1
/* 0B8EF4 800B82F4 8FB00144 */  lw    $s0, 0x144($sp)
/* 0B8EF8 800B82F8 13210009 */  beq   $t9, $at, .L800B8320
/* 0B8EFC 800B82FC 8FB800FC */   lw    $t8, 0xfc($sp)
/* 0B8F00 800B8300 24010002 */  li    $at, 2
/* 0B8F04 800B8304 1321000E */  beq   $t9, $at, .L800B8340
/* 0B8F08 800B8308 8FB800AC */   lw    $t8, 0xac($sp)
/* 0B8F0C 800B830C 8E040000 */  lw    $a0, ($s0)
/* 0B8F10 800B8310 00041880 */  sll   $v1, $a0, 2
/* 0B8F14 800B8314 000377C2 */  srl   $t6, $v1, 0x1f
/* 0B8F18 800B8318 10000052 */  b     .L800B8464
/* 0B8F1C 800B831C 01C01825 */   move  $v1, $t6
.L800B8320:
/* 0B8F20 800B8320 8FB90144 */  lw    $t9, 0x144($sp)
/* 0B8F24 800B8324 270F01A0 */  addiu $t7, $t8, 0x1a0
/* 0B8F28 800B8328 A7AF009E */  sh    $t7, 0x9e($sp)
/* 0B8F2C 800B832C 8F240000 */  lw    $a0, ($t9)
/* 0B8F30 800B8330 00041880 */  sll   $v1, $a0, 2
/* 0B8F34 800B8334 000377C2 */  srl   $t6, $v1, 0x1f
/* 0B8F38 800B8338 1000004A */  b     .L800B8464
/* 0B8F3C 800B833C 01C01825 */   move  $v1, $t6
.L800B8340:
/* 0B8F40 800B8340 13000009 */  beqz  $t8, .L800B8368
/* 0B8F44 800B8344 8FA50144 */   lw    $a1, 0x144($sp)
/* 0B8F48 800B8348 24010001 */  li    $at, 1
/* 0B8F4C 800B834C 1301002E */  beq   $t8, $at, .L800B8408
/* 0B8F50 800B8350 02401025 */   move  $v0, $s2
/* 0B8F54 800B8354 8E040000 */  lw    $a0, ($s0)
/* 0B8F58 800B8358 00041880 */  sll   $v1, $a0, 2
/* 0B8F5C 800B835C 00037FC2 */  srl   $t7, $v1, 0x1f
/* 0B8F60 800B8360 10000040 */  b     .L800B8464
/* 0B8F64 800B8364 01E01825 */   move  $v1, $t7
.L800B8368:
/* 0B8F68 800B8368 02401025 */  move  $v0, $s2
/* 0B8F6C 800B836C 26520008 */  addiu $s2, $s2, 8
/* 0B8F70 800B8370 05A10003 */  bgez  $t5, .L800B8380
/* 0B8F74 800B8374 000DC843 */   sra   $t9, $t5, 1
/* 0B8F78 800B8378 25A10001 */  addiu $at, $t5, 1
/* 0B8F7C 800B837C 0001C843 */  sra   $t9, $at, 1
.L800B8380:
/* 0B8F80 800B8380 27380007 */  addiu $t8, $t9, 7
/* 0B8F84 800B8384 330FFFF8 */  andi  $t7, $t8, 0xfff8
/* 0B8F88 800B8388 3C011100 */  lui   $at, 0x1100
/* 0B8F8C 800B838C 01E1C825 */  or    $t9, $t7, $at
/* 0B8F90 800B8390 AC590000 */  sw    $t9, ($v0)
/* 0B8F94 800B8394 8FAE00FC */  lw    $t6, 0xfc($sp)
/* 0B8F98 800B8398 24180020 */  li    $t8, 32
/* 0B8F9C 800B839C 3C0100FF */  lui   $at, (0x00FFFFFF >> 16) # lui $at, 0xff
/* 0B8FA0 800B83A0 25CF01A0 */  addiu $t7, $t6, 0x1a0
/* 0B8FA4 800B83A4 000FCC00 */  sll   $t9, $t7, 0x10
/* 0B8FA8 800B83A8 372E0020 */  ori   $t6, $t9, 0x20
/* 0B8FAC 800B83AC AC4E0004 */  sw    $t6, 4($v0)
/* 0B8FB0 800B83B0 AFAD00A0 */  sw    $t5, 0xa0($sp)
/* 0B8FB4 800B83B4 A7B8009E */  sh    $t8, 0x9e($sp)
/* 0B8FB8 800B83B8 8CA40000 */  lw    $a0, ($a1)
/* 0B8FBC 800B83BC 02401025 */  move  $v0, $s2
/* 0B8FC0 800B83C0 25B90020 */  addiu $t9, $t5, 0x20
/* 0B8FC4 800B83C4 00041880 */  sll   $v1, $a0, 2
/* 0B8FC8 800B83C8 00037FC2 */  srl   $t7, $v1, 0x1f
/* 0B8FCC 800B83CC 11E00025 */  beqz  $t7, .L800B8464
/* 0B8FD0 800B83D0 01E01825 */   move  $v1, $t7
/* 0B8FD4 800B83D4 3421FFFF */  ori   $at, (0x00FFFFFF & 0xFFFF) # ori $at, $at, 0xffff
/* 0B8FD8 800B83D8 03217024 */  and   $t6, $t9, $at
/* 0B8FDC 800B83DC 3C010200 */  lui   $at, 0x200
/* 0B8FE0 800B83E0 01C1C025 */  or    $t8, $t6, $at
/* 0B8FE4 800B83E4 25AF0010 */  addiu $t7, $t5, 0x10
/* 0B8FE8 800B83E8 AC4F0004 */  sw    $t7, 4($v0)
/* 0B8FEC 800B83EC AC580000 */  sw    $t8, ($v0)
/* 0B8FF0 800B83F0 8CA40000 */  lw    $a0, ($a1)
/* 0B8FF4 800B83F4 26520008 */  addiu $s2, $s2, 8
/* 0B8FF8 800B83F8 00041880 */  sll   $v1, $a0, 2
/* 0B8FFC 800B83FC 0003CFC2 */  srl   $t9, $v1, 0x1f
/* 0B9000 800B8400 10000018 */  b     .L800B8464
/* 0B9004 800B8404 03201825 */   move  $v1, $t9
.L800B8408:
/* 0B9008 800B8408 26520008 */  addiu $s2, $s2, 8
/* 0B900C 800B840C 05A10003 */  bgez  $t5, .L800B841C
/* 0B9010 800B8410 000D7043 */   sra   $t6, $t5, 1
/* 0B9014 800B8414 25A10001 */  addiu $at, $t5, 1
/* 0B9018 800B8418 00017043 */  sra   $t6, $at, 1
.L800B841C:
/* 0B901C 800B841C 25CF0007 */  addiu $t7, $t6, 7
/* 0B9020 800B8420 31F9FFF8 */  andi  $t9, $t7, 0xfff8
/* 0B9024 800B8424 3C011100 */  lui   $at, 0x1100
/* 0B9028 800B8428 03217025 */  or    $t6, $t9, $at
/* 0B902C 800B842C AC4E0000 */  sw    $t6, ($v0)
/* 0B9030 800B8430 8FB800FC */  lw    $t8, 0xfc($sp)
/* 0B9034 800B8434 271901A0 */  addiu $t9, $t8, 0x1a0
/* 0B9038 800B8438 8FB800A0 */  lw    $t8, 0xa0($sp)
/* 0B903C 800B843C 00197400 */  sll   $t6, $t9, 0x10
/* 0B9040 800B8440 270F0020 */  addiu $t7, $t8, 0x20
/* 0B9044 800B8444 31F9FFFF */  andi  $t9, $t7, 0xffff
/* 0B9048 800B8448 01D9C025 */  or    $t8, $t6, $t9
/* 0B904C 800B844C AC580004 */  sw    $t8, 4($v0)
/* 0B9050 800B8450 8FAF0144 */  lw    $t7, 0x144($sp)
/* 0B9054 800B8454 8DE40000 */  lw    $a0, ($t7)
/* 0B9058 800B8458 00041880 */  sll   $v1, $a0, 2
/* 0B905C 800B845C 000377C2 */  srl   $t6, $v1, 0x1f
/* 0B9060 800B8460 01C01825 */  move  $v1, $t6
.L800B8464:
/* 0B9064 800B8464 10600003 */  beqz  $v1, .L800B8474
/* 0B9068 800B8468 8FA200AC */   lw    $v0, 0xac($sp)
/* 0B906C 800B846C 10000007 */  b     .L800B848C
/* 0B9070 800B8470 AFB20154 */   sw    $s2, 0x154($sp)
.L800B8474:
/* 0B9074 800B8474 8FB900B0 */  lw    $t9, 0xb0($sp)
/* 0B9078 800B8478 24420001 */  addiu $v0, $v0, 1
/* 0B907C 800B847C AFA200AC */  sw    $v0, 0xac($sp)
/* 0B9080 800B8480 5459FE46 */  bnel  $v0, $t9, .L800B7D9C
/* 0B9084 800B8484 8FAF0130 */   lw    $t7, 0x130($sp)
/* 0B9088 800B8488 AFB20154 */  sw    $s2, 0x154($sp)
.L800B848C:
/* 0B908C 800B848C 0004C040 */  sll   $t8, $a0, 1
/* 0B9090 800B8490 00187FC2 */  srl   $t7, $t8, 0x1f
/* 0B9094 800B8494 24010001 */  li    $at, 1
/* 0B9098 800B8498 8FB20154 */  lw    $s2, 0x154($sp)
/* 0B909C 800B849C 15E10006 */  bne   $t7, $at, .L800B84B8
/* 0B90A0 800B84A0 00001825 */   move  $v1, $zero
/* 0B90A4 800B84A4 8FAE0144 */  lw    $t6, 0x144($sp)
/* 0B90A8 800B84A8 24030001 */  li    $v1, 1
/* 0B90AC 800B84AC 91D90000 */  lbu   $t9, ($t6)
/* 0B90B0 800B84B0 3338FFBF */  andi  $t8, $t9, 0xffbf
/* 0B90B4 800B84B4 A1D80000 */  sb    $t8, ($t6)
.L800B84B8:
/* 0B90B8 800B84B8 8FB30150 */  lw    $s3, 0x150($sp)
/* 0B90BC 800B84BC 97B9009E */  lhu   $t9, 0x9e($sp)
/* 0B90C0 800B84C0 02402025 */  move  $a0, $s2
/* 0B90C4 800B84C4 00133040 */  sll   $a2, $s3, 1
/* 0B90C8 800B84C8 00C09825 */  move  $s3, $a2
/* 0B90CC 800B84CC 02E02825 */  move  $a1, $s7
/* 0B90D0 800B84D0 97A70112 */  lhu   $a3, 0x112($sp)
/* 0B90D4 800B84D4 AFA30014 */  sw    $v1, 0x14($sp)
/* 0B90D8 800B84D8 AFA30114 */  sw    $v1, 0x114($sp)
/* 0B90DC 800B84DC 0C02E191 */  jal   final_resample
/* 0B90E0 800B84E0 AFB90010 */   sw    $t9, 0x10($sp)
/* 0B90E4 800B84E4 8FB10144 */  lw    $s1, 0x144($sp)
/* 0B90E8 800B84E8 00409025 */  move  $s2, $v0
/* 0B90EC 800B84EC 8FB40114 */  lw    $s4, 0x114($sp)
/* 0B90F0 800B84F0 92380003 */  lbu   $t8, 3($s1)
/* 0B90F4 800B84F4 02402025 */  move  $a0, $s2
/* 0B90F8 800B84F8 02E03025 */  move  $a2, $s7
/* 0B90FC 800B84FC 17000004 */  bnez  $t8, .L800B8510
/* 0B9100 800B8500 02202825 */   move  $a1, $s1
/* 0B9104 800B8504 92EE0002 */  lbu   $t6, 2($s7)
/* 0B9108 800B8508 51C00004 */  beql  $t6, $zero, .L800B851C
/* 0B910C 800B850C 922F0004 */   lbu   $t7, 4($s1)
.L800B8510:
/* 0B9110 800B8510 1000000A */  b     .L800B853C
/* 0B9114 800B8514 24100001 */   li    $s0, 1
/* 0B9118 800B8518 922F0004 */  lbu   $t7, 4($s1)
.L800B851C:
/* 0B911C 800B851C 15E00005 */  bnez  $t7, .L800B8534
/* 0B9120 800B8520 00000000 */   nop   
/* 0B9124 800B8524 92F90003 */  lbu   $t9, 3($s7)
/* 0B9128 800B8528 00008025 */  move  $s0, $zero
/* 0B912C 800B852C 13200003 */  beqz  $t9, .L800B853C
/* 0B9130 800B8530 00000000 */   nop   
.L800B8534:
/* 0B9134 800B8534 10000001 */  b     .L800B853C
/* 0B9138 800B8538 24100002 */   li    $s0, 2
.L800B853C:
/* 0B913C 800B853C 8FA70150 */  lw    $a3, 0x150($sp)
/* 0B9140 800B8540 AFA00010 */  sw    $zero, 0x10($sp)
/* 0B9144 800B8544 AFB00014 */  sw    $s0, 0x14($sp)
/* 0B9148 800B8548 0C02E1A8 */  jal   func_800B86A0
/* 0B914C 800B854C AFB40018 */   sw    $s4, 0x18($sp)
/* 0B9150 800B8550 92380000 */  lbu   $t8, ($s1)
/* 0B9154 800B8554 00409025 */  move  $s2, $v0
/* 0B9158 800B8558 00402025 */  move  $a0, $v0
/* 0B915C 800B855C 330E0001 */  andi  $t6, $t8, 1
/* 0B9160 800B8560 11C00007 */  beqz  $t6, .L800B8580
/* 0B9164 800B8564 02202825 */   move  $a1, $s1
/* 0B9168 800B8568 02E03025 */  move  $a2, $s7
/* 0B916C 800B856C 02603825 */  move  $a3, $s3
/* 0B9170 800B8570 AFB40010 */  sw    $s4, 0x10($sp)
/* 0B9174 800B8574 0C02E28B */  jal   note_apply_headset_pan_effects
/* 0B9178 800B8578 AFB00014 */   sw    $s0, 0x14($sp)
/* 0B917C 800B857C 00409025 */  move  $s2, $v0
.L800B8580:
/* 0B9180 800B8580 8FBF004C */  lw    $ra, 0x4c($sp)
/* 0B9184 800B8584 02401025 */  move  $v0, $s2
/* 0B9188 800B8588 8FB20030 */  lw    $s2, 0x30($sp)
/* 0B918C 800B858C 8FB00028 */  lw    $s0, 0x28($sp)
/* 0B9190 800B8590 8FB1002C */  lw    $s1, 0x2c($sp)
/* 0B9194 800B8594 8FB30034 */  lw    $s3, 0x34($sp)
/* 0B9198 800B8598 8FB40038 */  lw    $s4, 0x38($sp)
/* 0B919C 800B859C 8FB5003C */  lw    $s5, 0x3c($sp)
/* 0B91A0 800B85A0 8FB60040 */  lw    $s6, 0x40($sp)
/* 0B91A4 800B85A4 8FB70044 */  lw    $s7, 0x44($sp)
/* 0B91A8 800B85A8 8FBE0048 */  lw    $fp, 0x48($sp)
/* 0B91AC 800B85AC 03E00008 */  jr    $ra
/* 0B91B0 800B85B0 27BD0140 */   addiu $sp, $sp, 0x140
