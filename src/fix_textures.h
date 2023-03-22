#ifndef FIX_TEXTURES_H
#define FIX_TEXTURES_H

#include "textures.h"
#include "math_util.h"

extern s16 gCupCourseOrder[5][4];
extern MkTexture *D_800E82C8[0x16]; // Holds some textures for the course select screen

u64* mappedTitleTextures[20] = {
    gTextureTitleMarioRaceway,      // 0x00
    gTextureTitleChocoMountain,     // 0x01
    gTextureTitleBowsersCastle,     // 0x02
    gTextureTitleBansheeBoardwalk,  // 0x03

    gTextureTitleYoshiValley,       // 0x04
    gTextureTitleFrappeSnowland,    // 0x05
    gTextureTitleKoopaTroopaBeach,  // 0x06
    gTextureTitleRoyalRaceway,      // 0x07

    gTextureTitleLuigiRaceway,      // 0x08
    gTextureTitleMooMooFarm,        // 0x09
    gTextureTitleToadsTurnpike,     // 0x0A
    gTextureTitleKalimariDesert,    // 0x0B

    gTextureTitleSherbetLand,       // 0x0C
    gTextureTitleRainbowRoad,       // 0x0D
    gTextureTitleWarioStadium,      // 0x0E
    0,                              // 0x0F

    0,                              // 0x10
    0,                              // 0x11
    gTextureTitleDKsJungleParkway,  // 0x12
    0                               // 0x13
};

MkTexture RandomizedRaceNameTextures[5] = {
    {1, 0, gTextureTitleWarioStadium,     140, 18, 157, 112, 0x0, 0},
    {1, 0, gTextureTitleMooMooFarm,       140, 18, 157, 136, 0x0, 0},
    {1, 0, gTextureTitleKoopaTroopaBeach, 140, 18, 157, 160, 0x0, 0},
    {1, 0, gTextureTitleKalimariDesert,   140, 18, 157, 184, 0x0, 0},
    {0, 0, NULL, 0, 0, 0, 0, 0, 0},
};

void updateCourseNameTextures(void)
{
    D_800E82C8[0x05] = RandomizedRaceNameTextures;
    D_800E82C8[0x10] = RandomizedRaceNameTextures;
}

#endif