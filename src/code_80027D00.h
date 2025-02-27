#ifndef CODE_80027D00_H
#define CODE_80027D00_H

#include "common_structs.h"
#include "camera.h"

/* Function Prototypes */

s32 get_player_index_for_player(Player*);
void func_80028F70();
void func_80029060();
void func_80029150();
void func_80029158();
void func_800291E8();
void func_800291F0();
void func_800291F8();
void func_80029200(Player*, s8);
void func_8002934C(Player*, Camera*, s8, s8);
void func_8002A194(Player*, f32, f32, f32);
void func_8002A5F4(Vec3f, f32, Vec3f, f32, f32);
void func_8002A704(Player*, s8);
void func_8002A79C(Player*, s8);
void func_8002A8A4(Player*, s8);
void func_8002AA50(Player*);
void func_8002AAC0(Player*);
void func_8002AB70(Player*);
void func_8002AE20();
void func_8002AE28();
void func_8002AE30();
void func_8002AE38(Player*, s8, f32, f32, f32, f32);
void func_8002B218(Player*);
void func_8002B308(Player*, s8, s8);
void func_8002B5C0(Player*, s8, s8);
void func_8002B830(Player*, s8, s8);
void func_8002B8A4(Player*, Player*);
void func_8002BD58(Player*);
void func_8002C11C(Player*);
void func_8002C17C(Player*, s8);
void func_8002D268(Player*, Camera*, s8, s8);
void func_8002E4C4(Player*);
void func_8002FCA8(Player*, s8);
void func_8002FE84(Player*, f32);
f32  func_80030150(Player*, s8);
void func_80030A34(Player*);
void func_80030C34(Player*);
void func_80030E00(Player*);
void func_80030FC8(Player*);
void func_80031F48(Player*, f32);
void func_80032050(Player*);
void func_8003221C(Player*);
void func_800323E4(Player*);
void func_80032700(Player*);
void func_80032CB0(Player*, f32);
void func_80032D94(Player*);
void func_80033280(Player*, f32);
void func_800332E8(Player*, s32);
void func_800337CC(Player*, f32, s32);
void func_80033850(Player*, f32);
void func_8003680C(Player*, s16);
void func_80036C5C(Player*);
void func_80036CB4(Player*);
void func_80036DB4(Player*, Vec3f, Vec3f);
void func_800371F4(Player*, Vec3f, Vec3f);
void func_80037614(Player*, Vec3f, Vec3f);
void func_8003777C(Player*, Vec3f, Vec3f);
void func_800378E8(Player*, Vec3f, Vec3f);
void func_80037A4C(Player*, Vec3f, Vec3f);
void func_80037BB4(Player*player, Vec3f);
void func_800382DC();
s16  func_80038534(struct Controller*);
s16  func_800388B0(struct Controller*);
void func_80038BE4(Player*, s16);

/* This is where I'd put my static data, if I had any */

extern s16 D_80164AF0;
extern Player *D_801653C0[8];

extern f32 D_800ED7B8;
extern f32 D_800ED7BC;
extern f32 D_800ED7C0;
extern f32 D_800ED7C4;
extern f32 D_800ED7C8;

extern f64 D_800ED9D8;
extern f64 D_800ED9E0;
extern f64 D_800ED9E8;
extern f64 D_800ED9F0;
extern f64 D_800ED9F8;
extern f64 D_800EDA00;
extern f64 D_800EDA08;
extern f64 D_800EDA10;
extern f64 D_800EDA18;
extern f64 D_800EDA20;
extern f64 D_800EDA28;
extern f64 D_800EDA30;
extern f64 D_800EDA38;
extern f64 D_800EDA40;
extern f64 D_800EDA48;
extern f64 D_800EDA50;
extern f64 D_800EDA58;
extern f64 D_800EDA60;

extern f64 D_800EDA68;
extern f64 D_800EDA70;
extern f64 D_800EDA78;
extern f64 D_800EDA80;
extern f64 D_800EDA88;
extern f64 D_800EDA90;
extern f64 D_800EDA98;
extern f64 D_800EDAA0;
extern f64 D_800EDAA8;
extern f64 D_800EDAB0;
extern f64 D_800EDAB8;
extern f64 D_800EDAC0;
extern f64 D_800EDAC8;
extern f64 D_800EDAD0;
extern f64 D_800EDAD8;
extern f64 D_800EDAE0;
extern f64 D_800EDAE8;
extern f64 D_800EDAF0;

extern f64 D_800EDAF8;

extern f64 D_800EDB98;
extern f64 D_800EDBA0;

extern f64 D_800EDBA8;
extern f64 D_800EDBB0;
extern f64 D_800EDBB8;
extern f64 D_800EDBC0;
extern f64 D_800EDBC8;
extern f64 D_800EDBD0;
extern f64 D_800EDBD8;
extern f64 D_800EDBE0;
extern f64 D_800EDBE8;
extern f64 D_800EDBF0;
extern f64 D_800EDBF8;
extern f64 D_800EDC00;
extern f64 D_800EDC08;
extern f64 D_800EDC10;
extern f64 D_800EDC18;
extern f64 D_800EDC20;
extern f64 D_800EDC28;
extern f64 D_800EDC30;
extern f64 D_800EDC38;
extern f64 D_800EDC40;

extern struct Controller *gControllerEight;
extern struct Controller *gControllerFour;
extern struct Controller *gControllerOne;
extern struct Controller *gControllerSeven;
extern struct Controller *gControllerSix;
extern struct Controller *gControllerThree;
extern struct Controller *gControllerTwo;

#endif
