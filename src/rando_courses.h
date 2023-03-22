#ifndef RANDO_COURSES_H
#define RANDO_COURSES_H

#include "math_util.h"
#include "fix_textures.h"

extern s32 gIsMirrorMode;
extern s16 gCupCourseOrder[5][4];
extern s8 gCharacterSelections[4];
extern s8 gCupSelection;
extern s8 gCupCourseSelection;
extern char *D_800E7500[9]; // Cup name strings

typedef struct {
    u8 mirrored;
    u8 id;
} MirrorableCourse;

MirrorableCourse gRandomizedCups[8][4] = {
    // Mario Cup
    {{0, COURSE_MARIO_RACEWAY}, {1, COURSE_MARIO_RACEWAY}, {0, COURSE_MARIO_RACEWAY}, {1, COURSE_MARIO_RACEWAY}},
    // Luigi Cup
    {{0, COURSE_LUIGI_RACEWAY}, {1, COURSE_LUIGI_RACEWAY}, {0, COURSE_LUIGI_RACEWAY}, {1, COURSE_LUIGI_RACEWAY}},
    // Yoshi Cup
    {{0, COURSE_YOSHI_VALLEY}, {1, COURSE_YOSHI_VALLEY}, {0, COURSE_YOSHI_VALLEY}, {1, COURSE_YOSHI_VALLEY}},
    // Toad Cup
    {{0, COURSE_TOADS_TURNPIKE}, {1, COURSE_TOADS_TURNPIKE}, {0, COURSE_TOADS_TURNPIKE}, {1, COURSE_TOADS_TURNPIKE}},
    
    // DK Cup
    {{0, COURSE_DK_JUNGLE}, {1, COURSE_DK_JUNGLE}, {0, COURSE_DK_JUNGLE}, {1, COURSE_DK_JUNGLE}},
    // Wario Cup
    {{0, COURSE_WARIO_STADIUM}, {1, COURSE_WARIO_STADIUM}, {0, COURSE_WARIO_STADIUM}, {1, COURSE_WARIO_STADIUM}},
    // Peach Cup
    {{0, COURSE_ROYAL_RACEWAY}, {1, COURSE_ROYAL_RACEWAY}, {0, COURSE_ROYAL_RACEWAY}, {1, COURSE_ROYAL_RACEWAY}},
    // Bowser Cup
    {{0, COURSE_BOWSER_CASTLE}, {1, COURSE_BOWSER_CASTLE}, {0, COURSE_BOWSER_CASTLE}, {1, COURSE_BOWSER_CASTLE}}
};

char *newCupNames[8] = {
    "mario cup",
    "luigi cup",
    "yoshi cup",
    "toad cup",
    "dk cup",
    "wario cup",
    "peach cup",
    "bowser cup"
};

void UpdateRandoCourses(void) 
{
    int i;
    for (i = 0; i < 4; i++)
    {
        gCupCourseOrder[0][i] = gRandomizedCups[gCharacterSelections[0]][i].id;
        RandomizedRaceNameTextures[i].textureData = mappedTitleTextures[gCupCourseOrder[0][i]];
    }

    gCupSelection = 0;
    gIsMirrorMode = gRandomizedCups[gCharacterSelections[0]][gCupCourseSelection].mirrored;
    D_800E7500[0] = newCupNames[gCharacterSelections[0]];
}

#endif