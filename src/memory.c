#include <ultra64.h>
#include <PR/ultratypes.h>
#include <macros.h>
#include "types.h"

// Includes from SM64
//#include "buffers/buffers.h"
//#include "decompress.h"
//#include "game_init.h"
#include "main.h"
#include "memory.h"
#include "variables.h"
#include "common_structs.h"
#include "math_util.h"
#include <actor_types.h>

//#include "segment_symbols.h"
#include "segments.h"


s32 D_802BA270;
s32 D_802BA274;
u32 sPoolFreeSpace;
struct MainPoolBlock *sPoolListHeadL;
struct MainPoolBlock *sPoolListHeadR;

struct MainPoolState *gMainPoolState = NULL;

struct UnkStruct_802B8CD4 D_802B8CD4[] = {
    0
};
s32 D_802B8CE4 = 0; // pad

s32 func_802A7B70(s32 segment) {
    s32 old;
    old = gPrevLoadedAddress;
    segment = ALIGN16(segment);
    gPrevLoadedAddress += segment;
    return old;
}

uintptr_t set_segment_base_addr(s32 segment, void *addr) {
    gSegmentTable[segment] = (uintptr_t) addr & 0x1FFFFFFF;
    return gSegmentTable[segment];
}

void *get_segment_base_addr(s32 segment) {
    return (void *) (gSegmentTable[segment] | 0x80000000);
}

/**
 * @brief converts an RSP segment + offset address to a normal memory address
 */
void *segmented_to_virtual(const void *addr) {
    size_t segment = (uintptr_t) addr >> 24;
    size_t offset = (uintptr_t) addr & 0x00FFFFFF;

    return (void *) ((gSegmentTable[segment] + offset) | 0x80000000);
}

void move_segment_table_to_dmem(void) {
    s32 i;

    for (i = 0; i < 16; i++) {
        gSPSegment(gDisplayListHead++, i, gSegmentTable[i]);
    }
}

void func_802A7CF0(s32 start, s32 end) {

    start = ALIGN16(start);
    end &= ~0xF;

    D_8015F724 = (end - start) - 0x10;
    gPrevLoadedAddress = start;
}

s32 func_802A7D1C(s32 arg0) {
    s32 addr;

    arg0 = ALIGN16(arg0);
    D_8015F724 -= arg0;
    addr = gPrevLoadedAddress;
    gPrevLoadedAddress += arg0;

    return addr;
}

UNUSED void func_802A7D54(s32 arg0, s32 arg1) {
    gD_80150158[arg0].unk0 = arg0;
    gD_80150158[arg0].unk8 = arg1;
}

s32 func_802A7D70(s32 arg0, s32 arg1) {
    s32 temp_v0;
    s32 temp_a2 = arg1 - arg0;

    temp_v0 = func_802A7D1C(temp_a2);
    if (temp_v0 != 0) {
        dma_copy(temp_v0, arg0, temp_a2);
    }
    return temp_v0;
}

UNUSED void main_pool_init(u32 start, u32 end) {
    start = ALIGN16(start);
    end = ALIGN16(end - 15);

    sPoolFreeSpace = (end - start) - 16;

    sPoolListHeadL = (struct MainPoolBlock *) start;
    sPoolListHeadR = (struct MainPoolBlock *) end;
    sPoolListHeadL->prev = NULL;
    sPoolListHeadL->next = NULL;
    sPoolListHeadR->prev = NULL;
    sPoolListHeadR->next = NULL;
}

/**
 * Allocate a block of memory from the pool of given size, and from the
 * specified side of the pool (MEMORY_POOL_LEFT or MEMORY_POOL_RIGHT).
 * If there is not enough space, return NULL.
 */
void *main_pool_alloc(u32 size, u32 side) {
    struct MainPoolBlock *newListHead;
    void *addr = NULL;

    size = ALIGN16(size) + 8;
    if (sPoolFreeSpace >= size) {
        sPoolFreeSpace -= size;
        if (side == MEMORY_POOL_LEFT) {
            newListHead = (struct MainPoolBlock *) ((u8 *) sPoolListHeadL + size);
            sPoolListHeadL->next = newListHead;
            newListHead->prev = sPoolListHeadL;
            addr = (u8 *) sPoolListHeadL + 8;
            sPoolListHeadL = newListHead;
        } else {
            newListHead = (struct MainPoolBlock *) ((u8 *) sPoolListHeadR - size);
            sPoolListHeadR->prev = newListHead;
            newListHead->next = sPoolListHeadR;
            sPoolListHeadR = newListHead;
            addr = (u8 *) sPoolListHeadR + 8;
        }
    }
    return addr;
}
/**
 * Free a block of memory that was allocated from the pool. The block must be
 * the most recently allocated block from its end of the pool, otherwise all
 * newer blocks are freed as well.
 * Return the amount of free space left in the pool.
 */
u32 main_pool_free(void *addr) {
    struct MainPoolBlock *block = (struct MainPoolBlock *) ((u8 *) addr - 8);
    struct MainPoolBlock *oldListHead = (struct MainPoolBlock *) ((u8 *) addr - 8);

    if (oldListHead < sPoolListHeadL) {
        while (oldListHead->next != NULL) {
            oldListHead = oldListHead->next;
        }
        sPoolListHeadL = block;
        sPoolListHeadL->next = NULL;
        sPoolFreeSpace += (uintptr_t) oldListHead - (uintptr_t) sPoolListHeadL;
    } else {
        while (oldListHead->prev != NULL) {
            oldListHead = oldListHead->prev;
        }
        sPoolListHeadR = block->next;
        sPoolListHeadR->prev = NULL;
        sPoolFreeSpace += (uintptr_t) sPoolListHeadR - (uintptr_t) oldListHead;
    }
    return sPoolFreeSpace;
}
// main_pool_realloc
UNUSED void *main_pool_realloc(void *addr, u32 size) {
    void *newAddr = NULL;
    struct MainPoolBlock *block = (struct MainPoolBlock *) ((u8 *) addr - 8);

    if (block->next == sPoolListHeadL) {
        main_pool_free(addr);
        newAddr = main_pool_alloc(size, MEMORY_POOL_LEFT);
    }
    return newAddr;
}

UNUSED s32 main_pool_available(void) {
    return sPoolFreeSpace - 8;
}

UNUSED u32 main_pool_push_state(void) {
    struct MainPoolState *prevState = gMainPoolState;
    u32 freeSpace = sPoolFreeSpace;
    struct MainPoolBlock *lhead = sPoolListHeadL;
    struct MainPoolBlock *rhead = sPoolListHeadR;

    gMainPoolState = main_pool_alloc(sizeof(*gMainPoolState), MEMORY_POOL_LEFT);
    gMainPoolState->freeSpace = freeSpace;
    gMainPoolState->listHeadL = lhead;
    gMainPoolState->listHeadR = rhead;
    gMainPoolState->prev = prevState;
    return sPoolFreeSpace;
}

/**
 * Restore pool state from a previous call to main_pool_push_state. Return the
 * amount of free space left in the pool.
 */
UNUSED u32 main_pool_pop_state(void) {
    sPoolFreeSpace = gMainPoolState->freeSpace;
    sPoolListHeadL = gMainPoolState->listHeadL;
    sPoolListHeadR = gMainPoolState->listHeadR;
    gMainPoolState = gMainPoolState->prev;
    return sPoolFreeSpace;
}
// similar to sm64 dma_read
void *func_802A80B0(u8 *dest, u8 *srcStart, u8 *srcEnd) {
    u32 addr;
    u32 size = srcStart - dest;
    addr = main_pool_alloc(size, srcEnd);
    
    if (addr != 0) {

        osInvalDCache(addr, size);
        osPiStartDma(&gDmaIoMesg, OS_MESG_PRI_NORMAL, OS_READ, dest, addr, size,
                     &gDmaMesgQueue);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    }
    return addr;
}

// replaces call to dynamic_dma_read with dma_read.
UNUSED void *load_segment(s32 segment, u8 *srcStart, u8 *srcEnd, u32 side) {
    void *addr = func_802A80B0(srcStart, srcEnd, side);

    if (addr != NULL) {
        set_segment_base_addr(segment, addr);
    }
    return addr;
}

// Similar to sm64 load_to_fixed_pool_addr?
void *func_802A8190(s32 arg0, s32 arg1) {
    //u32 srcSize = ALIGN16(srcEnd - srcStart);
    //u32 destSize = ALIGN16((u8 *) sPoolListHeadR - destAddr);
    u32 addr;
    u32 temp_v0 = D_802B8CD4[arg0].unk4;
    u32 temp_v1 = D_802B8CD4[arg0].unk8;
    u32 temp_v2 = D_802B8CD4[arg0].unk2;
    addr = func_802A80B0(temp_v0, temp_v1, arg1);

        //dest = main_pool_alloc(destSize, MEMORY_POOL_RIGHT);
        if (addr != 0) {
            
            set_segment_base_addr(temp_v2, addr);
        }
    return addr;
}

UNUSED void func_802A81EC(void) {
    u32 addr;
    s32 temp_s0;
    s16 *phi_s1;
    s32 phi_s0;

    phi_s1 = &D_802B8CD4;
    phi_s0 = 0;
    do {
        if ((*phi_s1 & 1) != 0) {
            func_802A8190(phi_s0, 0);
        }
        temp_s0 = phi_s0 + 1;
        phi_s1 += 8;
        phi_s0 = temp_s0;
    } while (phi_s0 != 3);
}

UNUSED struct AllocOnlyPool *alloc_only_pool_init(u32 size, u32 side) {
    void *addr;
    struct AllocOnlyPool *subPool = NULL;

    size = ALIGN4(size);
    addr = main_pool_alloc(size + sizeof(struct AllocOnlyPool), side);
    if (addr != NULL) {
        subPool = (struct AllocOnlyPool *) addr;
        subPool->totalSpace = size;
        subPool->usedSpace = (u8 *) addr + sizeof(struct AllocOnlyPool);
        subPool->startPtr = 0;
        subPool->freePtr = (u8 *) addr + sizeof(struct AllocOnlyPool);
    }
    return subPool;
}

UNUSED u32 func_802A82AC(s32 arg0) {
    u32 temp_v0;
    u32 phi_v1;

    temp_v0 = D_801502A0 - arg0;
    phi_v1 = 0;
    if (temp_v0 >= (u32) gDisplayListHead) {
        D_801502A0 = temp_v0;
        phi_v1 = temp_v0;
    }
    return phi_v1;
}

s32 func_802A82E4(u8 *start, u8 *end) {
    s32 dest;
    u32 size;

    size = ALIGN16(end - start);
    dest = gPrevLoadedAddress;
    dma_copy(dest, start, size);
    gPrevLoadedAddress += size;
    return dest;
}

// unused mio0 decode func.
UNUSED s32 func_802A8348(s32 arg0, s32 arg1, s32 arg2) {
    s32 offset;
    UNUSED s32 *pad;
    s32 oldAddr;
    s32 newAddr;
    
    offset = ALIGN16(arg1 * arg2);
    oldAddr = gPrevLoadedAddress;
    newAddr = oldAddr + offset;
    pad = &newAddr;
    osInvalDCache(newAddr, offset);
    osPiStartDma(&gDmaIoMesg, 0, 0, &_other_texturesSegmentRomStart[arg0 & 0xFFFFFF], newAddr, offset, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
    
    func_80040030(newAddr, oldAddr);
    gPrevLoadedAddress += offset;
    return oldAddr;
}

UNUSED s32 func_802A841C(s32 arg0, s32 arg1, s32 arg2) {
    s32 temp_v0;
    s32 temp_a0;
    temp_v0 = gPrevLoadedAddress;
    temp_a0 = temp_v0 + arg2;
    arg1 = ALIGN16(arg1);
    arg2 = ALIGN16(arg2);
    
    osInvalDCache(temp_a0, arg1);
    osPiStartDma(&gDmaIoMesg, 0, 0, &_other_texturesSegmentRomStart[arg0 & 0xFFFFFF],temp_a0, arg1, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
    func_80040030(temp_a0, temp_v0);
    gPrevLoadedAddress += arg2;
    return temp_v0;
}

s32 func_802A84F4(s32 arg0, u32 arg1, u32 arg2) {
    u8 *temp_v0;
    u32 temp_a0;

    temp_v0 = gPrevLoadedAddress;
    temp_a0 = temp_v0 + arg2;
    arg1 = ALIGN16(arg1);
    arg2 = ALIGN16(arg2);
    osInvalDCache(temp_a0, arg1);
    osPiStartDma(&gDmaIoMesg, 0, 0, &_other_texturesSegmentRomStart[arg0 & 0xFFFFFF], temp_a0, arg1, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, (int) 1);
    mio0decode(temp_a0, temp_v0);
    gPrevLoadedAddress += arg2;
    return temp_v0;
}

u32 MIO0_0F(s32 arg0, u32 arg1, u32 arg2) {
    u32 oldHeapEndPtr;
    u8 *temp_v0;
    u32 temp_a0;

    arg1 = ALIGN16(arg1);
    arg2 = ALIGN16(arg2);
    oldHeapEndPtr = gHeapEndPtr;
    temp_v0 = gPrevLoadedAddress;

    osInvalDCache(temp_v0, arg1);
    osPiStartDma(&gDmaIoMesg, 0, 0, &_other_texturesSegmentRomStart[arg0 & 0xFFFFFF], temp_v0, arg1, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
    mio0decode(temp_v0, oldHeapEndPtr);
    gHeapEndPtr += arg2;
    return oldHeapEndPtr;
}

#ifdef MIPS_TO_C
//generated by m2c commit 8267401fa4ef7a38942dcca43353cc1bcc6efabc
extern f32 D_800DC608;
extern s16 *gHeapEndPtr;

void func_802A86A8(mk64_Vtx *arg0, u32 arg1) {
    f32 temp_f6;
    mk64_Vtx *var_v0;
    s16 *var_v1;
    s8 temp_a0;
    s8 temp_a3;
    u32 var_a2;

    var_v1 = gHeapEndPtr - (((arg1 * 0x10) + 0xF) & ~0xF);
    var_v0 = arg0;
    gHeapEndPtr = var_v1;
    var_a2 = 0;
    if (arg1 != 0) {
        do {
            if (gIsMirrorMode != 0) {
                *var_v1 = -var_v0->ob[0];
            } else {
                *var_v1 = var_v0->ob[0];
            }
            var_a2 += 1;
            var_v1 += 0x10;
            temp_f6 = (f32) var_v0->ob[1];
            var_v0 += 0xE;
            var_v1->unk-E = (s16) (s32) (temp_f6 * D_800DC608);
            temp_a0 = var_v0->unk-4;
            temp_a3 = var_v0->unk-3;
            var_v1->unk-C = (s16) var_v0->unk-A;
            var_v1->unk-8 = (s16) var_v0->unk-8;
            var_v1->unk-4 = (s8) (temp_a0 & 0xFC);
            var_v1->unk-3 = (s8) (temp_a3 & 0xFC);
            var_v1->unk-6 = (s16) var_v0->unk-6;
            var_v1->unk-2 = (s8) var_v0->unk-2;
            var_v1->unk-A = (s16) (s8) ((temp_a0 & 3) | ((temp_a3 * 4) & 0xC));
            var_v1->unk-1 = 0xFF;
        } while (var_a2 < arg1);
    }
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802A86A8.s")
#endif

void func_802A87A8(s32 arg0, u32 vertexCount) {
    s32 pad;
    u32 segment = SEGMENT_NUMBER2(arg0);
    u32 offset = SEGMENT_OFFSET(arg0);
    s32 old;
    s32 addr = VIRTUAL_TO_PHYSICAL2(gSegmentTable[segment] + offset);
    s32 temp_v0 = ALIGN16(vertexCount * 0x18);

    old = gPrevLoadedAddress;
    gPrevLoadedAddress += temp_v0;

    mio0decode(addr, old);
    func_802A86A8(old, vertexCount);
    set_segment_base_addr(4, gHeapEndPtr);
}

UNUSED void func_802A8844(void) {

}

void func_802A884C(Gfx *arg0, u8 arg1, s8 arg2) {
    s32 pad;
    s32 a = (arg2 * 0x18) + 0x9000008;
    s32 b = (arg2 * 0x18) + 0x9000000;
    Gfx macro[] = {gsSPNumLights(NUMLIGHTS_1)};

    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;

    D_802BA270++;
    arg0[D_802BA270].words.w0 = 0x3860010;

    arg0[D_802BA270].words.w1 = a;

    D_802BA270++;
    arg0[D_802BA270].words.w0 = 0x3880010;
    arg0[D_802BA270].words.w1 = b;
    D_802BA270++;
}

void func_802A8940(Gfx *arg0, u8 *arg1, u8 arg2) {

    u32 temp_v0 = arg1[D_802BA274++];
    u32 temp_t7 = ((arg1[D_802BA274++]) << 8 | temp_v0) * 8;
    arg0[D_802BA270].words.w0 = 0x6000000;
    arg0[D_802BA270].words.w1 = 0x7000000 + temp_t7;
    D_802BA270++;
}

// end display list
void func_802A89C0(Gfx *arg0, u8 arg1, u8 arg2) {
    arg0[D_802BA270].words.w0 = G_ENDDL << 24;
    arg0[D_802BA270].words.w1 = 0;
    D_802BA270++;
}

void func_802A8A04(Gfx *arg0, u8 arg1, u8 arg2) {
    Gfx macro[] = {gsSPSetGeometryMode(G_CULL_BACK)};
    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;
    D_802BA270++;
}

void func_802A8A70(Gfx *arg0, u8 arg1, u8 arg2) {
    Gfx macro[] = {gsSPClearGeometryMode(G_CULL_BACK)};
    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;
    D_802BA270++;
}

void func_802A8ADC(Gfx *arg0, u8 arg1, u8 arg2) {
    Gfx macro[] = {gsSPCullDisplayList(0, 160)};
    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;
    D_802BA270++;
}

void func_802A8B48(Gfx *arg0, u8 arg1, u8 arg2) {
    Gfx macro[] = {gsDPSetCombineMode(G_CC_MODULATERGBA, G_CC_MODULATERGBA)};
    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;
    D_802BA270++;
}

void func_802A8BB4(Gfx *arg0, u8 arg1, u8 arg2) {
    Gfx macro[] = {gsDPSetCombineMode(G_CC_MODULATERGBDECALA, G_CC_MODULATERGBDECALA)};
    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;
    D_802BA270++;
}

void func_802A8C20(Gfx *arg0, u8 arg1, u8 arg2) {
    Gfx macro[] = {gsDPSetCombineMode(G_CC_SHADE, G_CC_SHADE)};
    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;
    D_802BA270++;
}

void func_802A8C8C(Gfx *arg0, u8 arg1, u8 arg2) {
    Gfx macro[] = {gsDPSetCombineMode(G_CC_MODULATERGBDECALA, G_CC_MODULATERGBDECALA)};
    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;
    D_802BA270++;
}

void func_802A8CF8(Gfx *arg0, u8 arg1, u8 arg2) {
    Gfx macro[] = {gsDPSetCombineMode(G_CC_DECALRGBA, G_CC_DECALRGBA)};
    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;
    D_802BA270++;
}

void func_802A8D64(Gfx *arg0, u8 arg1, u8 arg2) {
    Gfx macro[] = {gsDPSetRenderMode(G_RM_AA_ZB_OPA_SURF, G_RM_AA_ZB_OPA_SURF2)};
    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;
    D_802BA270++;
}

void func_802A8DD0(Gfx *arg0, u8 arg1, u8 arg2) {
    Gfx macro[] = {gsDPSetRenderMode(G_RM_AA_ZB_TEX_EDGE, G_RM_AA_ZB_TEX_EDGE2)};
    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;
    D_802BA270++;
}

void func_802A8E3C(Gfx *arg0, u8 arg1, u8 arg2) {
    Gfx macro[] = {gsDPSetRenderMode(G_RM_AA_ZB_XLU_SURF, G_RM_AA_ZB_XLU_SURF2)};
    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;
    D_802BA270++;
}

void func_802A8EA8(Gfx *arg0, u8 arg1, u8 arg2) {
    Gfx macro[] = {gsDPSetRenderMode(G_RM_AA_ZB_OPA_DECAL, G_RM_AA_ZB_OPA_DECAL)};
    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;
    D_802BA270++;
}

void func_802A8F14(Gfx *arg0, u8 arg1, u8 arg2) {
    Gfx macro[] = {gsDPSetRenderMode(G_RM_AA_ZB_XLU_DECAL, G_RM_AA_ZB_XLU_DECAL)};
    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;
    D_802BA270++;
}

Gfx D_802B8D58[] = {gsDPTileSync()};

#ifdef MIPS_TO_C
//generated by m2c commit 9841ff34ca242f5f14b2eab2b54a7a65ac47d80f
void func_802A8F80(Gfx *displayList, s32 arg1, s8 arg2) {
    u32 sp58;
    s32 sp48;
    s32 sp44;
    s32 sp40;
    s32 temp_t5;
    s32 temp_t9;
    s32 var_v0;
    s32 var_v1;
    u8 temp_a0;
    u8 temp_a0_2;

    sp58.unk0 = D_802B8D58->words.w0;
    var_v0 = 0;
    sp58.unk4 = (u32) D_802B8D58->words.w1;
    switch (arg2) {
    case 26:
        var_v1 = 0x00000020;
        sp44 = 0x00000020;
        sp40 = 0;
block_9:
        sp48 = var_v1;
        break;
    case 44:
        var_v1 = 0x00000020;
        sp44 = 0x00000020;
        sp40 = 0;
        var_v0 = 0x100;
        goto block_9;
    case 27:
        var_v1 = 0x00000040;
        sp44 = 0x00000020;
        sp40 = 0;
        goto block_9;
    case 28:
        var_v1 = 0x00000020;
        sp44 = 0x00000040;
        sp40 = 0;
        goto block_9;
    case 29:
        var_v1 = 0x00000020;
        sp44 = 0x00000020;
        sp40 = 3;
        goto block_9;
    case 30:
        var_v1 = 0x00000040;
        sp44 = 0x00000020;
        sp40 = 3;
        goto block_9;
    case 31:
        var_v1 = 0x00000020;
        sp44 = 0x00000040;
        sp40 = 3;
        goto block_9;
    }
    temp_a0 = *(D_802BA274 + arg1);
    temp_t9 = D_802BA274 + 1;
    D_802BA274 = temp_t9;
    temp_a0_2 = *(temp_t9 + arg1);
    D_802BA274 = temp_t9 + 1;
    displayList[D_802BA270].words.w0 = sp58;
    displayList[D_802BA270].words.w1 = sp5C;
    D_802BA270 += 1;
    displayList[D_802BA270].words.w0 = (sp40 << 0x15) | 0xF5000000 | 0x100000 | (((s32) ((sp48 * 2) + 7) >> 3) << 9) | var_v0;
    displayList[D_802BA270].words.w1 = ((temp_a0_2 & 0xF) << 0x12) | (((u32) (temp_a0_2 & 0xF0) >> 4) << 0xE) | ((temp_a0 & 0xF) << 8) | (((u32) (temp_a0 & 0xF0) >> 4) * 0x10);
    temp_t5 = D_802BA270 + 1;
    D_802BA270 = temp_t5;
    displayList[temp_t5].words.w0 = 0xF2000000;
    displayList[D_802BA270].words.w1 = ((sp48 - 1) << 0xE) | ((sp44 - 1) * 4);
    D_802BA270 += 1;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802A8F80.s")
#endif

Gfx D_802B8D60[] = { gsDPTileSync() };
Gfx D_802B8D68[] = {  gsDPLoadSync() };

#ifdef MIPS_TO_C
//generated by m2c commit 9841ff34ca242f5f14b2eab2b54a7a65ac47d80f
void func_802A91E4(Gfx *displayList, s32 arg1, s8 arg2) {
    u32 sp48;
    u32 sp40;
    s32 sp2C;
    s32 sp28;
    s32 sp24;
    s32 temp_t6;
    s32 temp_t6_2;
    s32 temp_t8;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 var_v0;
    u32 temp_t1;
    u32 temp_t2;
    u32 temp_t6_3;
    u32 var_a2;
    u32 var_v0_2;
    u32 var_v1;
    u8 temp_a2;
    u8 temp_a3;

    sp48.unk0 = D_802B8D60->words.w0;
    sp48.unk4 = (u32) D_802B8D60->words.w1;
    sp40.unk0 = D_802B8D68->words.w0;
    sp40.unk4 = (u32) D_802B8D68->words.w1;
    switch (arg2) {
    case 32:
        var_v0 = 0x00000020;
        sp28 = 0x00000020;
        sp24 = 0;
block_8:
        sp2C = var_v0;
        break;
    case 33:
        var_v0 = 0x00000040;
        sp28 = 0x00000020;
        sp24 = 0;
        goto block_8;
    case 34:
        var_v0 = 0x00000020;
        sp28 = 0x00000040;
        sp24 = 0;
        goto block_8;
    case 35:
        var_v0 = 0x00000020;
        sp28 = 0x00000020;
        sp24 = 3;
        goto block_8;
    case 36:
        var_v0 = 0x00000040;
        sp28 = 0x00000020;
        sp24 = 3;
        goto block_8;
    case 37:
        var_v0 = 0x00000020;
        sp28 = 0x00000040;
        sp24 = 3;
        goto block_8;
    }
    temp_a2 = *(arg1 + D_802BA274);
    temp_t9 = D_802BA274 + 1;
    D_802BA274 = temp_t9;
    temp_t8 = temp_t9 + 1;
    D_802BA274 = temp_t8;
    temp_a3 = *(temp_t8 + arg1);
    D_802BA274 = temp_t8 + 1;
    temp_t6 = sp24 << 0x15;
    displayList[D_802BA270].words.w0 = temp_t6 | 0xFD000000 | 0x100000;
    displayList[D_802BA270].words.w1 = (temp_a2 << 0xB) + 0x05000000;
    temp_t9_2 = D_802BA270 + 1;
    D_802BA270 = temp_t9_2;
    displayList[temp_t9_2].words.w0 = sp48;
    var_v1 = 0x7FF;
    displayList[D_802BA270].words.w1 = sp4C;
    D_802BA270 += 1;
    displayList[D_802BA270].words.w0 = temp_t6 | 0xF5000000 | 0x100000 | (temp_a3 & 0xF);
    temp_t1 = ((u32) (temp_a3 & 0xF0) >> 4) << 0x18;
    displayList[D_802BA270].words.w1 = temp_t1;
    temp_t6_2 = D_802BA270 + 1;
    D_802BA270 = temp_t6_2;
    displayList[temp_t6_2].words.w0 = sp40;
    displayList[D_802BA270].words.w1 = sp44;
    temp_t6_3 = (u32) (sp2C * 2) >> 3;
    D_802BA270 += 1;
    temp_t2 = (sp2C * sp28) - 1;
    if (temp_t2 < 0x7FFU) {
        var_v1 = temp_t2;
    }
    if (temp_t6_3 == 0) {
        var_v0_2 = 1;
    } else {
        var_v0_2 = temp_t6_3;
    }
    var_a2 = temp_t6_3;
    if (temp_t6_3 == 0) {
        var_a2 = 1;
    }
    displayList[D_802BA270].words.w0 = 0xF3000000;
    displayList[D_802BA270].words.w1 = ((u32) (var_v0_2 + 0x7FF) / var_a2) | temp_t1 | (var_v1 << 0xC);
    D_802BA270 += 1;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802A91E4.s")
#endif

void func_802A94D8(Gfx *arg0, u8 arg1, u8 arg2) {
    Gfx macro[] = { gsSPTexture(0xFFFF, 0xFFFF, 0, G_TX_RENDERTILE, G_ON) };
    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;
    D_802BA270++;
}

void func_802A9544(Gfx *arg0, u8 arg1, u8 arg2) {
    Gfx macro[] = { gsSPTexture(0x1, 0x1, 0, G_TX_RENDERTILE, G_OFF) };

    arg0[D_802BA270].words.w0 = macro->words.w0;
    arg0[D_802BA270].words.w1 = macro->words.w1;
    D_802BA270++;
}

#ifdef MIPS_TO_C
//generated by m2c commit 9841ff34ca242f5f14b2eab2b54a7a65ac47d80f
void func_802A95B0(Gfx *displayList, u8 *arg1, s32 arg2) {
    s32 temp_t5;
    s32 temp_t7;
    s32 temp_t7_2;
    u8 temp_v0;

    temp_t7 = D_802BA274 + 1;
    temp_v0 = arg1[D_802BA274];
    D_802BA274 = temp_t7;
    temp_t5 = temp_t7 + 1;
    D_802BA274 = temp_t5;
    temp_t7_2 = temp_t5 + 1;
    D_802BA274 = temp_t7_2;
    D_802BA274 = temp_t7_2 + 1;
    displayList[D_802BA270].words.w0 = ((arg1[temp_t7_2] & 0x3F) << 0x11) | 0x04000000 | (((arg1[temp_t5] & 0x3F) * 0x410) - 1);
    displayList[D_802BA270].words.w1 = (((arg1[temp_t7] << 8) | temp_v0) * 0x10) + 0x04000000;
    D_802BA270 += 1;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802A95B0.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit 9841ff34ca242f5f14b2eab2b54a7a65ac47d80f
void func_802A9674(Gfx *displayList, u8 *arg1, s8 arg2) {
    s32 temp_t9;
    u8 temp_v0;

    temp_t9 = D_802BA274 + 1;
    temp_v0 = arg1[D_802BA274];
    D_802BA274 = temp_t9;
    D_802BA274 = temp_t9 + 1;
    displayList[D_802BA270].words.w0 = (((arg2 - 0x32) * 0x410) - 1) | 0x04000000;
    displayList[D_802BA270].words.w1 = (((arg1[temp_t9] << 8) | temp_v0) * 0x10) + 0x04000000;
    D_802BA270 += 1;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802A9674.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit 9841ff34ca242f5f14b2eab2b54a7a65ac47d80f
void func_802A9714(Gfx *displayList, u8 *arg1, s32 arg2) {
    s32 temp_t7;
    s32 var_a0;
    s32 var_a2;
    s32 var_a3;
    u8 temp_v0;
    u8 temp_v0_2;
    u8 temp_v0_3;

    temp_v0 = arg1[D_802BA274];
    temp_t7 = D_802BA274 + 1;
    D_802BA274 = temp_t7;
    var_a0 = temp_v0 & 0x1F;
    var_a3 = temp_v0 & 0x1F;
    if (gIsMirrorMode != 0) {
        temp_v0_2 = arg1[temp_t7];
        D_802BA274 = temp_t7 + 1;
        var_a2 = ((temp_v0 >> 5) & 7) | ((temp_v0_2 & 3) * 8);
        var_a3 = (temp_v0_2 >> 2) & 0x1F;
    } else {
        temp_v0_3 = arg1[D_802BA274];
        D_802BA274 += 1;
        var_a2 = ((temp_v0 >> 5) & 7) | ((temp_v0_3 & 3) * 8);
        var_a0 = (temp_v0_3 >> 2) & 0x1F;
    }
    displayList[D_802BA270].words.w0 = 0xBF000000;
    displayList[D_802BA270].words.w1 = (var_a3 << 0x11) | (var_a2 << 9) | (var_a0 * 2);
    D_802BA270 += 1;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802A9714.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit 9841ff34ca242f5f14b2eab2b54a7a65ac47d80f
void func_802A980C(Gfx *displayList, u8 *arg1, s32 arg2) {
    s32 temp_t7;
    s32 temp_t8;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 var_a0;
    s32 var_a2;
    s32 var_a3;
    s32 var_t0;
    s32 var_t1;
    s32 var_t2;
    u8 temp_v0;
    u8 temp_v0_2;
    u8 temp_v0_3;
    u8 temp_v0_4;
    u8 temp_v0_5;
    u8 temp_v0_6;

    temp_v0 = arg1[D_802BA274];
    temp_t7 = D_802BA274 + 1;
    D_802BA274 = temp_t7;
    var_a0 = temp_v0 & 0x1F;
    var_t0 = temp_v0 & 0x1F;
    if (gIsMirrorMode != 0) {
        temp_v0_2 = arg1[temp_t7];
        D_802BA274 = temp_t7 + 1;
        var_a3 = ((temp_v0 >> 5) & 7) | ((temp_v0_2 & 3) * 8);
        var_t0 = (temp_v0_2 >> 2) & 0x1F;
    } else {
        temp_v0_3 = arg1[D_802BA274];
        D_802BA274 += 1;
        var_a3 = ((temp_v0 >> 5) & 7) | ((temp_v0_3 & 3) * 8);
        var_a0 = (temp_v0_3 >> 2) & 0x1F;
    }
    temp_v0_4 = arg1[D_802BA274];
    temp_t9 = D_802BA274 + 1;
    D_802BA274 = temp_t9;
    if (gIsMirrorMode != 0) {
        var_a2 = temp_v0_4 & 0x1F;
        temp_v0_5 = arg1[temp_t9];
        D_802BA274 = temp_t9 + 1;
        var_t1 = ((temp_v0_4 >> 5) & 7) | ((temp_v0_5 & 3) * 8);
        var_t2 = (temp_v0_5 >> 2) & 0x1F;
    } else {
        var_t2 = temp_v0_4 & 0x1F;
        temp_v0_6 = arg1[D_802BA274];
        temp_t8 = (temp_v0_6 & 3) * 8;
        temp_t9_2 = (temp_v0_6 >> 2) & 0x1F;
        D_802BA274 += 1;
        var_t1 = ((temp_v0_4 >> 5) & 7) | temp_t8;
        var_a2 = temp_t9_2;
    }
    displayList[D_802BA270].words.w0 = (var_t0 << 0x11) | 0xB1000000 | (var_a3 << 9) | (var_a0 * 2);
    displayList[D_802BA270].words.w1 = (var_t2 << 0x11) | (var_t1 << 9) | (var_a2 * 2);
    D_802BA270 += 1;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802A980C.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit 9841ff34ca242f5f14b2eab2b54a7a65ac47d80f
void func_802A99A4(Gfx *displayList, u8 *arg1, s32 arg2) {
    s32 temp_t3;
    s32 temp_t6;
    s32 temp_t7;
    s32 var_a0;
    s32 var_a2;
    s32 var_a3;
    s32 var_t0;
    u8 temp_v0;
    u8 temp_v0_2;
    u8 temp_v0_3;

    temp_v0 = arg1[D_802BA274];
    temp_t7 = D_802BA274 + 1;
    D_802BA274 = temp_t7;
    var_a0 = temp_v0 & 0x1F;
    var_t0 = temp_v0 & 0x1F;
    if (gIsMirrorMode != 0) {
        temp_v0_2 = arg1[temp_t7];
        temp_t3 = temp_t7 + 1;
        D_802BA274 = temp_t3;
        var_a2 = ((temp_v0 >> 5) & 7) | ((temp_v0_2 & 3) * 8);
        var_a3 = (temp_v0_2 >> 2) & 0x1F;
        D_802BA274 = temp_t3 + 1;
        var_t0 = ((temp_v0_2 >> 7) & 1) | ((arg1[temp_t3] & 0xF) * 2);
    } else {
        temp_v0_3 = arg1[D_802BA274];
        temp_t6 = D_802BA274 + 1;
        D_802BA274 = temp_t6;
        var_a3 = ((temp_v0 >> 5) & 7) | ((temp_v0_3 & 3) * 8);
        var_a2 = (temp_v0_3 >> 2) & 0x1F;
        D_802BA274 = temp_t6 + 1;
        var_a0 = ((temp_v0_3 >> 7) & 1) | ((arg1[temp_t6] & 0xF) * 2);
    }
    displayList[D_802BA270].words.w0 = 0xB5000000;
    displayList[D_802BA270].words.w1 = (var_a0 << 0x19) | (var_t0 << 0x11) | (var_a3 << 9) | (var_a2 * 2);
    D_802BA270 += 1;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802A99A4.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit 8267401fa4ef7a38942dcca43353cc1bcc6efabc
void func_802A8F80(Gfx *, u8, s8, s32 *);              /* extern */
void func_802A91E4(Gfx *, u8, s8, s32 *);              /* extern */
void func_802A95B0(Gfx *, u8, s8, s32 *);              /* extern */
void func_802A9674(Gfx *, u8, s8, s32 *);              /* extern */
void func_802A9714(Gfx *, u8, s8, s32 *);              /* extern */
void func_802A980C(Gfx *, u8, s8, s32 *);              /* extern */
void func_802A99A4(Gfx *, u8, s8, s32 *);              /* extern */
extern s32 D_802BA270;
extern s32 D_802BA274;
extern Gfx *gHeapEndPtr;

void display_list_unpack(u32 arg0, s32 arg1, u8 arg2) {
    Gfx *sp2C;
    Gfx *temp_s1;
    s32 temp_v1;
    u8 temp_a0;
    u8 temp_s0;

    temp_s0 = gSegmentTable[arg0 >> 0x18] + (arg0 & 0xFFFFFF) + 0x80000000;
    temp_s1 = gHeapEndPtr - (((arg1 + 0xF) & ~0xF) + 8);
    gHeapEndPtr = temp_s1;
    D_802BA270 = 0;
    sp2C = temp_s1;
    D_802BA274 = 0;
case 0x31:
case 0x32:
loop_1:
    temp_v1 = D_802BA274;
    temp_a0 = *(temp_v1 + temp_s0);
    D_802BA274 = temp_v1 + 1;
    switch (temp_a0) {                              /* irregular */
    case 0x0:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0x1:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0x2:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0x3:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0x4:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0x5:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0x6:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0x7:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0x8:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0x9:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0xA:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0xB:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0xC:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0xD:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0xE:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0xF:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0x10:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0x11:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0x12:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0x13:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0x14:
        func_802A884C(temp_s1, temp_s0, (s8) temp_a0);
        goto loop_1;
    case 0x15:
        func_802A8B48(temp_s1, temp_s0, arg2);
        goto loop_1;
    case 0x16:
        func_802A8BB4(temp_s1, temp_s0, arg2);
        goto loop_1;
    case 0x17:
        func_802A8C20(temp_s1, temp_s0, arg2);
        goto loop_1;
    case 0x2E:
        func_802A8C8C(temp_s1, temp_s0, arg2);
        goto loop_1;
    case 0x53:
        func_802A8CF8(temp_s1, temp_s0, arg2);
        goto loop_1;
    case 0x18:
        func_802A8D64(temp_s1, temp_s0, arg2);
        goto loop_1;
    case 0x19:
        func_802A8DD0(temp_s1, temp_s0, arg2);
        goto loop_1;
    case 0x2F:
        func_802A8E3C(temp_s1, temp_s0, arg2);
        goto loop_1;
    case 0x54:
        func_802A8EA8(temp_s1, temp_s0, arg2);
        goto loop_1;
    case 0x55:
        func_802A8F14(temp_s1, temp_s0, arg2);
        goto loop_1;
    case 0x1A:
        func_802A8F80(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x2C:
        func_802A8F80(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x1B:
        func_802A8F80(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x1C:
        func_802A8F80(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x1D:
        func_802A8F80(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x1E:
        func_802A8F80(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x1F:
        func_802A8F80(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x20:
        func_802A91E4(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x21:
        func_802A91E4(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x22:
        func_802A91E4(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x23:
        func_802A91E4(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x24:
        func_802A91E4(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x25:
        func_802A91E4(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x26:
        func_802A94D8(temp_s1, temp_s0, (u8) (s8) temp_a0);
        goto loop_1;
    case 0x27:
        func_802A9544(temp_s1, temp_s0, (u8) (s8) temp_a0);
        goto loop_1;
    case 0x28:
        func_802A95B0(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x33:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x34:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x35:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x36:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x37:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x38:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x39:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x3A:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x3B:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x3C:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x3D:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x3E:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x3F:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x40:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x41:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x42:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x43:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x44:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x45:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x46:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x47:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x48:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x49:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x4A:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x4B:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x4C:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x4D:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x4E:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x4F:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x50:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x51:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x52:
        func_802A9674(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x29:
        func_802A9714(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x58:
        func_802A980C(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x30:
        func_802A99A4(temp_s1, temp_s0, (s8) temp_a0, &D_802BA274);
        goto loop_1;
    case 0x2D:
        func_802A8ADC(temp_s1, temp_s0, (u8) (s8) temp_a0);
        goto loop_1;
    case 0x2A:
        func_802A89C0(temp_s1, temp_s0, (u8) (s8) temp_a0);
        goto loop_1;
    case 0x56:
        func_802A8A04(temp_s1, temp_s0, (u8) (s8) temp_a0);
        goto loop_1;
    case 0x57:
        func_802A8A70(temp_s1, temp_s0, (u8) (s8) temp_a0);
        goto loop_1;
    case 0x2B:
        func_802A8940(temp_s1, (u8 *) temp_s0, (u8) (s8) temp_a0);
        goto loop_1;
    }
    set_segment_base_addr(7, sp2C);
}
#else
GLOBAL_ASM("asm/non_matchings/memory/display_list_unpack.s")
#endif

#ifdef MIPS_TO_C
//generated by mips_to_c commit 3c3b0cede1a99430bfd3edf8d385802b94f91307
u32 MIO0_0F(s32, s32, s32); // extern
extern uintptr_t gSegmentTable;

void func_802AA7C8(u32 arg0) {
    void *sp20;
    s32 temp_a0;
    void *temp_s0;
    void *temp_t2;
    void *phi_s0;
    s32 phi_v0;
    void *phi_s0_2;

    temp_s0 = *(&gSegmentTable + ((arg0 >> 0x18) * 4)) + (arg0 & 0xFFFFFF) + 0x80000000;
    phi_s0 = temp_s0;
    phi_v0 = 0;
loop_1:
    if (phi_s0->unk0 != 0) {
        phi_s0 += 0x10;
        phi_v0 += phi_s0->unk8;
        goto loop_1;
    }
    temp_t2 = gHeapEndPtr - phi_v0;
    gHeapEndPtr = temp_t2;
    sp20 = temp_t2;
    phi_s0_2 = temp_s0;
loop_4:
    temp_a0 = phi_s0_2->unk0;
    if (temp_a0 != 0) {
        MIO0_0F(temp_a0, phi_s0_2->unk4, phi_s0_2->unk8);
        phi_s0_2 += 0x10;
        goto loop_4;
    }
    gHeapEndPtr = sp20;
    set_segment_base_addr(5, sp20);
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AA7C8.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit d9d3d6575355663122de59f6b2882d8f174e2355 on Dec-09-2022
void *func_802AA88C(u8 *arg0, u8 *arg1) {
    s32 sp28;
    u8 *sp20;
    u32 sp1C;
    u32 temp_t6;
    u8 *temp_a0;

    temp_t6 = ((arg1 - arg0) + 0xF) & ~0xF;
    temp_a0 = gHeapEndPtr - temp_t6;
    sp20 = temp_a0;
    dma_copy(temp_a0, arg0, temp_t6);
    sp1C = gPrevLoadedAddress;
    sp28 = (temp_a0->unk4 + 0xF) & ~0xF;
    mio0decode(temp_a0, (u8 *) gPrevLoadedAddress);
    gPrevLoadedAddress += sp28;
    return (void *) sp1C;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AA88C.s")
#endif

s32 load_course(s32 courseId) {
    UNUSED s32 pad[4];
    s32 temp_v0;
    u32 *dlRomStart;
    u32 *dlRomEnd;
    u32 *vertexRomStart;
    u32 *vertexRomEnd;
    s32 sp4C;
    s32 gamestate;
    u32 *textures;
    mk64_Vtx *vertexStart;
    u32 *packedStart;
    u32 vertexCount;
    u32 *finalDL;
    s32 unknown1;
    s32 prevLoadedAddress_saved;
    s32 offsetRomStart;
    s32 offsetRomEnd;

    gamestate = gGamestate;
    dlRomStart = gCourseTable[courseId].dlRomStart;
    dlRomEnd = gCourseTable[courseId].dlRomEnd;
    offsetRomStart = gCourseTable[courseId].offsetRomStart;
    offsetRomEnd = gCourseTable[courseId].offsetRomEnd;
    vertexRomStart = gCourseTable[courseId].vertexRomStart;
    vertexRomEnd = gCourseTable[courseId].vertexRomEnd;
    textures = gCourseTable[courseId].textures;
    vertexStart = gCourseTable[courseId].vertexStart;
    packedStart = gCourseTable[courseId].packedStart;
    vertexCount = gCourseTable[courseId].vertexCount;
    finalDL = gCourseTable[courseId].finalDL;
    unknown1 = gCourseTable[courseId].unknown1;

    if ((gamestate == ENDING_SEQUENCE) || (gamestate == CREDITS_SEQUENCE)) {
        gHeapEndPtr = SEG_80280000;
    } else {
        gHeapEndPtr = SEG_8028DF00;
    }
    set_segment_base_addr(9, func_802A7D70(offsetRomStart, offsetRomEnd));
    if (gGamestate != ENDING_SEQUENCE) {
        set_segment_base_addr(6, func_802AA88C(dlRomStart, dlRomEnd));
    }
    prevLoadedAddress_saved = gPrevLoadedAddress;
    temp_v0 = func_802A82E4((u8 *)vertexRomStart, (u8 *)vertexRomEnd);
    sp4C = temp_v0;
    set_segment_base_addr(0xF, (void *)temp_v0);
    func_802A87A8(vertexStart, vertexCount);
    display_list_unpack(packedStart, finalDL, unknown1);
    func_802AA7C8(textures);
    gPrevLoadedAddress = prevLoadedAddress_saved;
    return temp_v0;
}

void func_802AAA70(uintptr_t addr) {
    s32 segment = SEGMENT_NUMBER2(addr);
    s32 offset = SEGMENT_OFFSET(addr);

    Gfx *macro;

    macro = VIRTUAL_TO_PHYSICAL2(gSegmentTable[segment] + offset);
    macro->words.w0 = G_ENDDL << 24;
    macro->words.w1 = G_SPNOOP;
}

void func_802AAAAC(UnkActorInner *arg0) {
    arg0->unk36 = 0x1388;
    arg0->unk38 = 0x1388;
    arg0->unk3A = 0x1388;
    arg0->unk30 = 0;
    arg0->unk32 = 0;
    arg0->unk34 = 0;
    arg0->unk3C[0] = 0;
    arg0->unk3C[1] = 0;
    arg0->unk3C[2] = 0;
    vec3f_set(arg0->unk48, 0.0f, 0.0f, 1.0f);
    vec3f_set(arg0->unk54, 1.0f, 0.0f, 0.0f);
    vec3f_set(arg0->unk60, 0.0f, 1.0f, 0.0f);
}

#ifdef MIPS_TO_C
//generated by m2c commit 6b6e1390bf30013247343623194964ff13d0c45b
extern f32 D_8015F8E4;
static f32 D_802B9E00 = 1859.0f;
static f32 D_802B9E04 = 1549.0f;
static f32 D_802B9E08 = -1102.0f;
static f32 D_802B9E0C = -1402.0f;
static f32 D_802B9E10 = 2405.0f;
static f32 D_802B9E14 = 2233.0f;
static f32 D_802B9E18 = 0.8f;
static f32 D_802B9E1C = -475.0f;
static f32 D_802B9E20 = -33.9f;
static f32 D_802B9E24 = -838.0f;
static f32 D_802B9E28 = -475.0f;
static f32 D_802B9E2C = -475.0f;
static f32 D_802B9E30 = -993.0f;
static f32 D_802B9E34 = -33.9f;
static f32 D_802B9E38 = -475.0f;
static f32 D_802B9E3C = -33.9f;
static f32 D_802B9E40 = -475.0f;
static f32 D_802B9E44 = -33.9f;
s16 gCurrentCourseId;                               /* unable to generate initializer */

f32 func_802AAB4C(Player *player) {
    f32 sp1C;
    f32 sp18;
    f32 temp_f0;
    f32 temp_f2;
    s16 temp_v0;
    s32 temp_v1;

    temp_v0 = gCurrentCourseId;
    temp_f0 = player->pos[0];
    temp_f2 = player->pos[2];
    switch (temp_v0) {                              /* irregular */
    case 2:
        if (D_802B9E00 < temp_f0) {
            goto block_43;
        }
        if (temp_f0 < D_802B9E04) {
            goto block_43;
        }
        if (D_802B9E08 < temp_f2) {
            goto block_43;
        }
        if (temp_f2 < D_802B9E0C) {
block_43:
        default:
            return D_8015F8E4;
        }
        return 20.0f;
    case 6:
        if (temp_f0 > 239.0f) {
            goto block_43;
        }
        if (temp_f0 < 67.0f) {
            goto block_43;
        }
        if (D_802B9E10 < temp_f2) {
            goto block_43;
        }
        if (temp_f2 < D_802B9E14) {
            goto block_43;
        }
        return D_802B9E18;
    case 12:
        if ((func_802ABD10(player->unk_110.unk3A) & 0xFF) == 5) {
            return (f32) (D_8015F6EE - 0xA);
        }
        goto block_43;
    case 18:
        sp18 = temp_f2;
        sp1C = temp_f0;
        temp_v1 = func_802ABD40(player->unk_110.unk3A) & 0xFF;
        if (temp_v1 == 0xFF) {
            sp18 = temp_f2;
            sp1C = temp_f0;
            if ((func_802ABD10(player->unk_110.unk3A) & 0xFF) == 0xF) {
                return D_802B9E1C;
            }
            if (temp_f0 > -478.0f) {
                return D_802B9E20;
            }
            if (temp_f0 < D_802B9E24) {
                return D_802B9E28;
            }
            if (temp_f2 > -436.0f) {
                return D_802B9E2C;
            }
            if (temp_f2 < D_802B9E30) {
                return D_802B9E34;
            }
            if (temp_f2 < temp_f0) {
                return D_802B9E38;
            }
            return D_802B9E3C;
        }
        if (temp_v1 >= 0x14) {
            return D_802B9E40;
        }
        return D_802B9E44;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AAB4C.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit 0927f17aac197848d4ebdf0c6bbad74b01f0851c
static f32 D_802B9E48 = -0.9f;

s32 func_802AAE4C(UnkActorInner *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, u16 arg5) {
    f32 sp6C;
    f32 sp64;
    f32 sp3C;
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    Vtx *temp_a0;
    Vtx *temp_a2;
    Vtx *temp_a3;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f10;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f16;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f4;
    f32 temp_f8;
    mk64_surface_map_ram *temp_v1;
    s32 var_v0;
    s32 var_v0_2;

    temp_v1 = &D_8015F580[arg5];
    temp_f0 = temp_v1->gravity;
    var_v0_2 = 1;
    if (temp_f0 < D_802B9E48) {
        return 0;
    }
    if (arg2 < (f32) temp_v1->vtx3[0]) {
        return 0;
    }
    if (arg4 < (f32) temp_v1->vtx3[2]) {
        return 0;
    }
    if ((f32) temp_v1->vtx2[0] < arg2) {
        return 0;
    }
    if ((f32) temp_v1->vtx2[2] < arg4) {
        return 0;
    }
    sp2C = temp_f0;
    if (arg3 < ((f32) temp_v1->vtx3[1] - (arg1 * 3.0f))) {
        return 0;
    }
    temp_a0 = temp_v1->vtxs[0];
    temp_a3 = temp_v1->vtxs[2];
    temp_a2 = temp_v1->vtxs[1];
    sp6C = (f32) temp_a3->v.ob[0];
    sp8 = arg4;
    sp10 = arg2;
    temp_f8 = (f32) temp_a0->v.ob[2] - arg4;
    sp64 = (f32) temp_a3->v.ob[2];
    temp_f4 = (f32) temp_a2->v.ob[0] - arg2;
    sp28 = temp_f8;
    sp24 = temp_f4;
    sp20 = (f32) temp_a0->v.ob[0] - arg2;
    temp_f0_2 = temp_f8 * temp_f4;
    sp8 = temp_f8;
    spC = temp_f4;
    sp1C = (f32) temp_a2->v.ob[2] - arg4;
    temp_f2 = sp20 * sp1C;
    temp_f10 = temp_f0_2 - temp_f2;
    sp18 = temp_f10;
    sp3C = temp_f10;
    if (temp_f2 == temp_f0_2) {
        sp14 = arg4;
        temp_f0_3 = sp6C - sp10;
        temp_f2_2 = sp64 - arg4;
        if ((((sp1C * temp_f0_3) - (spC * temp_f2_2)) * ((temp_f2_2 * sp20) - (temp_f0_3 * sp8))) < 0.0f) {
            var_v0_2 = 0;
        }
    } else {
        temp_f2_3 = sp64 - arg4;
        temp_f0_4 = sp6C - arg2;
        temp_f14 = sp24 * temp_f2_3;
        temp_f12 = sp1C * temp_f0_4;
        if (temp_f14 == temp_f12) {
            if ((sp3C * ((temp_f2_3 * sp20) - (temp_f0_4 * sp28))) < 0.0f) {
                var_v0_2 = 0;
            }
        } else {
            temp_f16 = temp_f12 - temp_f14;
            if ((sp18 * temp_f16) < 0.0f) {
                var_v0_2 = 0;
            } else {
                temp_f14_2 = temp_f0_4 * sp28;
                temp_f12_2 = temp_f2_3 * sp20;
                if ((temp_f14_2 != temp_f12_2) && ((temp_f16 * (temp_f12_2 - temp_f14_2)) < 0.0f)) {
                    var_v0_2 = 0;
                }
            }
        }
    }
    if (var_v0_2 == 0) {
        return 0;
    }
    temp_f0_5 = ((temp_v1->height * arg2) + (sp2C * arg3) + (temp_v1->rotation * arg4) + temp_v1->height2) - arg1;
    if (temp_f0_5 > 0.0f) {
        if (temp_f0_5 < arg0->unk3C[2]) {
            arg0->unk34 = 1;
            arg0->unk3A = arg5;
            arg0->unk3C[2] = temp_f0_5;
            arg0->unk60[0] = temp_v1->height;
            arg0->unk60[1] = temp_v1->gravity;
            arg0->unk60[2] = temp_v1->rotation;
        }
        return 0;
    }
    var_v0 = 0;
    if (temp_f0_5 > -16.0f) {
        arg0->unk34 = 1;
        arg0->unk3A = arg5;
        arg0->unk3C[2] = temp_f0_5;
        var_v0 = 1;
        arg0->unk60[0] = temp_v1->height;
        arg0->unk60[1] = temp_v1->gravity;
        arg0->unk60[2] = temp_v1->rotation;
    }
    return var_v0;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AAE4C.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit 0927f17aac197848d4ebdf0c6bbad74b01f0851c
s32 func_802AB288(UnkActorInner *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, u16 arg5) {
    f32 sp70;
    f32 sp6C;
    f32 sp3C;
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    Vtx *temp_a0;
    Vtx *temp_a2;
    Vtx *temp_a3;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f16;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f2_4;
    f32 temp_f4;
    f32 temp_f6;
    f32 temp_f8;
    mk64_surface_map_ram *temp_v1;
    s32 var_v0;
    s32 var_v0_2;

    temp_v1 = &D_8015F580[arg5];
    var_v0_2 = 1;
    if (arg2 < (f32) temp_v1->vtx3[0]) {
        return 0;
    }
    if ((f32) temp_v1->vtx2[0] < arg2) {
        return 0;
    }
    if ((f32) temp_v1->vtx2[1] < arg3) {
        return 0;
    }
    if (arg3 < (f32) temp_v1->vtx3[1]) {
        return 0;
    }
    temp_f2 = arg1 * 3.0f;
    if (arg4 < ((f32) temp_v1->vtx3[2] - temp_f2)) {
        return 0;
    }
    if (((f32) temp_v1->vtx2[2] + temp_f2) < arg4) {
        return 0;
    }
    temp_a0 = temp_v1->vtxs[0];
    temp_a3 = temp_v1->vtxs[2];
    temp_a2 = temp_v1->vtxs[1];
    sp70 = (f32) temp_a3->v.ob[0];
    sp8 = arg3;
    sp10 = arg2;
    temp_f4 = (f32) temp_a0->v.ob[1] - arg3;
    sp6C = (f32) temp_a3->v.ob[1];
    temp_f8 = (f32) temp_a2->v.ob[0] - arg2;
    sp2C = temp_f4;
    sp28 = temp_f8;
    sp24 = (f32) temp_a0->v.ob[0] - arg2;
    temp_f0 = temp_f4 * temp_f8;
    sp8 = temp_f4;
    spC = temp_f8;
    sp20 = (f32) temp_a2->v.ob[1] - arg3;
    temp_f2_2 = sp24 * sp20;
    temp_f6 = temp_f0 - temp_f2_2;
    sp1C = temp_f6;
    sp3C = temp_f6;
    if (temp_f2_2 == temp_f0) {
        sp14 = arg3;
        temp_f0_2 = sp70 - sp10;
        temp_f2_3 = sp6C - arg3;
        if ((((sp20 * temp_f0_2) - (spC * temp_f2_3)) * ((temp_f2_3 * sp24) - (temp_f0_2 * sp8))) < 0.0f) {
            var_v0_2 = 0;
        }
    } else {
        temp_f2_4 = sp6C - arg3;
        temp_f0_3 = sp70 - arg2;
        temp_f14 = sp28 * temp_f2_4;
        temp_f12 = sp20 * temp_f0_3;
        if (temp_f14 == temp_f12) {
            if ((sp3C * ((temp_f2_4 * sp24) - (temp_f0_3 * sp2C))) < 0.0f) {
                var_v0_2 = 0;
            }
        } else {
            temp_f16 = temp_f12 - temp_f14;
            if ((sp1C * temp_f16) < 0.0f) {
                var_v0_2 = 0;
            } else {
                temp_f14_2 = temp_f0_3 * sp2C;
                temp_f12_2 = temp_f2_4 * sp24;
                if ((temp_f14_2 != temp_f12_2) && ((temp_f16 * (temp_f12_2 - temp_f14_2)) < 0.0f)) {
                    var_v0_2 = 0;
                }
            }
        }
    }
    if (var_v0_2 == 0) {
        return 0;
    }
    temp_f0_4 = ((temp_v1->height * arg2) + (temp_v1->gravity * arg3) + (temp_v1->rotation * arg4) + temp_v1->height2) - arg1;
    if (temp_f0_4 > 0.0f) {
        if (temp_f0_4 < arg0->unk3C[0]) {
            arg0->unk30 = 1;
            arg0->unk36 = arg5;
            arg0->unk3C[0] = temp_f0_4;
            arg0->unk48[0] = temp_v1->height;
            arg0->unk48[1] = temp_v1->gravity;
            arg0->unk48[2] = temp_v1->rotation;
        }
        return 0;
    }
    var_v0 = 0;
    if (temp_f0_4 > -16.0f) {
        arg0->unk30 = 1;
        arg0->unk36 = arg5;
        arg0->unk3C[0] = temp_f0_4;
        var_v0 = 1;
        arg0->unk48[0] = temp_v1->height;
        arg0->unk48[1] = temp_v1->gravity;
        arg0->unk48[2] = temp_v1->rotation;
    }
    return var_v0;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AB288.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit 0927f17aac197848d4ebdf0c6bbad74b01f0851c
s32 func_802AB6C4(UnkActorInner *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, u16 arg5) {
    f32 sp68;
    f32 sp64;
    f32 sp38;
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    Vtx *temp_a0;
    Vtx *temp_a2;
    Vtx *temp_a3;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f10;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f16;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f4;
    f32 temp_f6;
    mk64_surface_map_ram *temp_v1;
    s32 var_v0;
    s32 var_v0_2;

    temp_v1 = &D_8015F580[arg5];
    var_v0_2 = 1;
    if (arg4 < (f32) temp_v1->vtx3[2]) {
        return 0;
    }
    if ((f32) temp_v1->vtx2[2] < arg4) {
        return 0;
    }
    if ((f32) temp_v1->vtx2[1] < arg3) {
        return 0;
    }
    if (arg3 < (f32) temp_v1->vtx3[1]) {
        return 0;
    }
    temp_f0 = arg1 * 3.0f;
    if (arg2 < ((f32) temp_v1->vtx3[0] - temp_f0)) {
        return 0;
    }
    if (((f32) temp_v1->vtx2[0] + temp_f0) < arg2) {
        return 0;
    }
    temp_a0 = temp_v1->vtxs[0];
    temp_a3 = temp_v1->vtxs[2];
    temp_a2 = temp_v1->vtxs[1];
    sp64 = (f32) temp_a3->v.ob[2];
    sp8 = arg3;
    sp10 = arg4;
    temp_f6 = (f32) temp_a0->v.ob[1] - arg3;
    sp68 = (f32) temp_a3->v.ob[1];
    temp_f10 = (f32) temp_a2->v.ob[2] - arg4;
    sp2C = temp_f6;
    sp28 = temp_f10;
    sp24 = (f32) temp_a0->v.ob[2] - arg4;
    temp_f0_2 = temp_f6 * temp_f10;
    sp8 = temp_f6;
    spC = temp_f10;
    sp20 = (f32) temp_a2->v.ob[1] - arg3;
    temp_f2 = sp24 * sp20;
    temp_f4 = temp_f0_2 - temp_f2;
    sp1C = temp_f4;
    sp38 = temp_f4;
    if (temp_f2 == temp_f0_2) {
        sp14 = arg3;
        temp_f0_3 = sp64 - sp10;
        temp_f2_2 = sp68 - arg3;
        if ((((sp20 * temp_f0_3) - (spC * temp_f2_2)) * ((temp_f2_2 * sp24) - (temp_f0_3 * sp8))) < 0.0f) {
            var_v0_2 = 0;
        }
    } else {
        temp_f2_3 = sp68 - arg3;
        temp_f0_4 = sp64 - arg4;
        temp_f14 = sp28 * temp_f2_3;
        temp_f12 = sp20 * temp_f0_4;
        if (temp_f14 == temp_f12) {
            if ((sp38 * ((temp_f2_3 * sp24) - (temp_f0_4 * sp2C))) < 0.0f) {
                var_v0_2 = 0;
            }
        } else {
            temp_f16 = temp_f12 - temp_f14;
            if ((sp1C * temp_f16) < 0.0f) {
                var_v0_2 = 0;
            } else {
                temp_f14_2 = temp_f0_4 * sp2C;
                temp_f12_2 = temp_f2_3 * sp24;
                if ((temp_f14_2 != temp_f12_2) && ((temp_f16 * (temp_f12_2 - temp_f14_2)) < 0.0f)) {
                    var_v0_2 = 0;
                }
            }
        }
    }
    if (var_v0_2 == 0) {
        return 0;
    }
    temp_f0_5 = ((temp_v1->height * arg2) + (temp_v1->gravity * arg3) + (temp_v1->rotation * arg4) + temp_v1->height2) - arg1;
    if (temp_f0_5 > 0.0f) {
        if (temp_f0_5 < arg0->unk3C[1]) {
            arg0->unk32 = 1;
            arg0->unk38 = arg5;
            arg0->unk3C[1] = temp_f0_5;
            arg0->unk54[0] = temp_v1->height;
            arg0->unk54[1] = temp_v1->gravity;
            arg0->unk54[2] = temp_v1->rotation;
        }
        return 0;
    }
    var_v0 = 0;
    if (temp_f0_5 > -16.0f) {
        arg0->unk32 = 1;
        arg0->unk38 = arg5;
        arg0->unk3C[1] = temp_f0_5;
        var_v0 = 1;
        arg0->unk54[0] = temp_v1->height;
        arg0->unk54[1] = temp_v1->gravity;
        arg0->unk54[2] = temp_v1->rotation;
    }
    return var_v0;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AB6C4.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit 0927f17aac197848d4ebdf0c6bbad74b01f0851c
s32 func_802ABB04(f32 arg0, f32 arg1, s32 arg2) {
    f32 sp6C;
    f32 sp64;
    f32 sp60;
    f32 sp58;
    f32 sp3C;
    f32 sp28;
    f32 sp20;
    f32 sp1C;
    Vtx *temp_a0;
    Vtx *temp_a1;
    Vtx *temp_a3;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f10;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f16_2;
    f32 temp_f18;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f6;
    mk64_surface_map_ram *temp_v0;
    s32 var_v1;

    temp_v0 = &D_8015F580[arg2 & 0xFFFF];
    temp_a1 = temp_v0->vtxs[1];
    temp_a0 = temp_v0->vtxs[0];
    var_v1 = 1;
    sp60 = (f32) temp_a1->v.ob[0];
    temp_f20 = (f32) temp_a0->v.ob[2] - arg1;
    sp58 = (f32) temp_a1->v.ob[2];
    temp_a3 = temp_v0->vtxs[2];
    temp_f22 = (f32) temp_a0->v.ob[0] - arg0;
    sp6C = (f32) temp_a3->v.ob[0];
    temp_f10 = sp60 - arg0;
    temp_f0 = temp_f20 * temp_f10;
    sp28 = temp_f10;
    sp64 = (f32) temp_a3->v.ob[2];
    temp_f6 = sp58 - arg1;
    temp_f2 = temp_f22 * temp_f6;
    sp20 = temp_f6;
    sp1C = temp_f0 - temp_f2;
    sp3C = sp1C;
    if (temp_f2 == temp_f0) {
        temp_f0_2 = sp6C - arg0;
        temp_f2_2 = sp64 - arg1;
        if ((((temp_f6 * temp_f0_2) - (temp_f10 * temp_f2_2)) * ((temp_f2_2 * temp_f22) - (temp_f0_2 * temp_f20))) < 0.0f) {
            var_v1 = 0;
        }
    } else {
        temp_f2_3 = sp64 - arg1;
        temp_f0_3 = sp6C - arg0;
        temp_f18 = sp28 * temp_f2_3;
        temp_f16 = sp20 * temp_f0_3;
        if (temp_f18 == temp_f16) {
            if ((sp3C * ((temp_f2_3 * temp_f22) - (temp_f0_3 * temp_f20))) < 0.0f) {
                var_v1 = 0;
            }
        } else {
            temp_f12 = temp_f16 - temp_f18;
            if ((sp1C * temp_f12) < 0.0f) {
                var_v1 = 0;
            } else {
                temp_f16_2 = temp_f0_3 * temp_f20;
                temp_f14 = temp_f2_3 * temp_f22;
                if ((temp_f16_2 != temp_f14) && ((temp_f12 * (temp_f14 - temp_f16_2)) < 0.0f)) {
                    var_v1 = 0;
                }
            }
        }
    }
    return var_v1;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802ABB04.s")
#endif

s8 func_802ABD10(u16 arg0) {
    mk64_surface_map_ram *tile = &D_8015F580[arg0];
    return tile->surfaceType;
}

s16 func_802ABD40(u16 arg0) {
    mk64_surface_map_ram *tile = &D_8015F580[arg0];
    return tile->flags & 0xFF;
}

s16 func_802ABD7C(u16 arg0) {
    mk64_surface_map_ram *tile = &D_8015F580[arg0];
    return tile->flags & 0x1000;
}

s16 func_802ABDB8(u16 arg0) {
    mk64_surface_map_ram *tile = &D_8015F580[arg0];
    return tile->flags & 0x400;
}

s16 func_802ABDF4(u16 arg0) {
    mk64_surface_map_ram *tile = &D_8015F580[arg0];
    return tile->flags & 0x800;
}

f32 func_802ABE30(f32 arg0, f32 arg1, f32 arg2, u16 arg3) {
    mk64_surface_map_ram *tile = &D_8015F580[arg3];
    if (tile->gravity == 0.0f) {
        return arg1;
    }
    return ((tile->height * arg0) + (tile->rotation * arg2) + tile->height2) / -tile->gravity;
}

f32 func_802ABEAC(UnkActorInner *arg0, Vec3f arg1) {
    if (arg0->unk34 == 1) {
        return func_802ABE30(arg1[0], arg1[1], arg1[2], arg0->unk3A);
    }
    if (arg0->unk30 == 1) {
        return func_802ABE30(arg1[0], arg1[1], arg1[2], arg0->unk36);
    }
    if (arg0->unk32 == 1) {
        return func_802ABE30(arg1[0], arg1[1], arg1[2], arg0->unk38);
    }
    return arg1[1];
}

#ifdef MIPS_TO_C
//generated by m2c commit 9841ff34ca242f5f14b2eab2b54a7a65ac47d80f
void process_shell_collision(f32 *arg0, f32 arg1, f32 *velocity, f32 arg3) {
    f32 sp40;
    f32 sp3C;
    f32 sp38;
    f32 sp24;
    f32 spC;
    f32 sp8;
    f32 sp4;
    f32 sp0;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f10;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f16_2;
    f32 temp_f16_3;
    f32 temp_f18;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f6;
    f64 temp_f16;

    temp_f2 = velocity->unk0;
    temp_f12 = velocity->unk4;
    temp_f14 = velocity->unk8;
    temp_f0 = sqrtf((temp_f2 * temp_f2) + (temp_f12 * temp_f12) + (temp_f14 * temp_f14));
    temp_f16 = (f64) temp_f0;
    sp24 = temp_f0;
    if ((temp_f16 > 4.5) || (temp_f16 < 3.5)) {
        sp24 = 4.0f;
    }
    temp_f10 = arg0->unk8;
    sp4 = temp_f10;
    temp_f16_2 = arg0->unk0;
    temp_f18 = arg0->unk4;
    temp_f0_2 = (temp_f10 * temp_f14) + ((temp_f16_2 * temp_f2) + (temp_f18 * temp_f12));
    sp0 = temp_f16_2 * temp_f0_2;
    spC = temp_f18 * temp_f0_2;
    sp40 = temp_f2 - sp0;
    sp3C = temp_f12 - spC;
    temp_f6 = temp_f10 * temp_f0_2;
    sp8 = temp_f6;
    sp38 = temp_f14 - temp_f6;
    temp_f2_2 = sp40 - sp0;
    temp_f12_2 = sp3C - spC;
    temp_f14_2 = sp38 - temp_f6;
    temp_f16_3 = (1.0f / sqrtf((temp_f2_2 * temp_f2_2) + (temp_f12_2 * temp_f12_2) + (temp_f14_2 * temp_f14_2))) * sp24;
    velocity->unk0 = temp_f2_2 * temp_f16_3;
    velocity->unk4 = (f32) (temp_f12_2 * temp_f16_3);
    velocity->unk8 = (f32) (temp_f14_2 * temp_f16_3);
}
#else
GLOBAL_ASM("asm/non_matchings/memory/process_shell_collision.s")
#endif

void func_802AC098(UnkActorInner *arg0, f32 *velocity) {
    if (arg0->unk3C[0] < 0.0f) {
        process_shell_collision(arg0->unk48, arg0->unk3C[0], velocity, 2.0f);
    }

    if (arg0->unk3C[1] < 0.0f) {
        process_shell_collision(arg0->unk54, arg0->unk3C[1], velocity, 2.0f);
    }
}

#ifdef MIPS_TO_C
//generated by m2c commit bece1d6db17040749f77dbbd090363cc6fb926f9
void func_802AC114(Vec3f arg0, f32 arg1, Vec3f arg2, f32 arg3) {
    f32 temp_f0;
    f32 temp_f12;
    f32 temp_f14;
    f32 temp_f16;
    f32 temp_f18;
    f32 temp_f20;
    f32 temp_f2;

    temp_f0 = arg2[0];
    temp_f2 = arg2[1];
    temp_f12 = arg2[2];
    temp_f14 = -arg0[0];
    temp_f16 = -arg0[1];
    temp_f18 = -arg0[2];
    temp_f20 = (temp_f14 * temp_f0) + (temp_f16 * temp_f2) + (temp_f18 * temp_f12);
    if (arg1 < -3.5) {
        arg2[0] = temp_f0  - (temp_f20 * temp_f14) - (0.5f * temp_f14 * temp_f20);
        arg2[1] = temp_f2  - (temp_f20 * temp_f16) - (0.5f * temp_f16 * temp_f20);
        arg2[2] = temp_f12 - (temp_f20 * temp_f18) - (0.5f * temp_f18 * temp_f20);
    } else {
        arg2[0] = temp_f0  - (temp_f20 * temp_f14);
        arg2[1] = temp_f2  - (temp_f20 * temp_f16);
        arg2[2] = temp_f12 - (temp_f20 * temp_f18);
    }
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AC114.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit bece1d6db17040749f77dbbd090363cc6fb926f9
mk64_surface_map_ram *func_802AAE4C(s16 *, ?, f32, f32, f32, s32); /* extern */
mk64_surface_map_ram *func_802AB288(s16 *, ?, f32, f32, f32, s32); /* extern */
mk64_surface_map_ram *func_802AB6C4(s16 *, ?, f32, f32, f32, s32); /* extern */
extern u8 D_8014F110;
extern s32 D_8015F584;
extern s16 D_8015F6E8;
extern s16 D_8015F6EA;
extern s16 D_8015F6F0;
extern s16 D_8015F6F2;

UNUSED mk64_surface_map_ram *func_802AC22C(void *arg0) {
    f32 spE4;
    f32 spE0;
    f32 spDC;
    s16 spD4;
    s16 spD2;
    s16 spD0;
    s32 sp70;
    f32 temp_f22;
    f32 temp_f24;
    f32 temp_f26;
    s16 temp_v0_2;
    s16 temp_v1;
    s32 temp_at;
    s32 temp_f4;
    s32 temp_f6;
    s32 temp_t3;
    s32 var_s2;
    u16 temp_s0;
    u16 temp_v1_3;
    u16 var_s1;
    u16 var_v0;
    u8 temp_v0;
    void *temp_v1_2;

    spD0 = 0;
    spD2 = 0;
    spD4 = 0;
    spDC = 1000.0f;
    spE0 = 1000.0f;
    spE4 = 1000.0f;
    temp_v0 = arg0->unkD;
    temp_f22 = arg0->unk0;
    temp_f24 = arg0->unk4;
    temp_f26 = arg0->unk8;
    switch (temp_v0) {                              /* irregular */
    case 0x80:
        if (func_802AB6C4(&spD0, 0x40A00000, temp_f22, temp_f24, temp_f26, (s32) arg0->unkE) == (mk64_surface_map_ram *)1) {
            arg0->unk10 = func_802ABE30(temp_f22, temp_f24, temp_f26, arg0->unkE);
            return (mk64_surface_map_ram *)1;
        }
    default:
block_10:
        temp_v0_2 = D_8015F6EA;
        temp_v1 = D_8015F6F2;
        temp_f4 = (s32) ((temp_f22 - (f32) temp_v0_2) / (f32) ((s32) (D_8015F6E8 - temp_v0_2) / 32));
        temp_f6 = (s32) ((temp_f26 - (f32) temp_v1) / (f32) ((s32) (D_8015F6F0 - temp_v1) / 32));
        if ((s16) temp_f4 < 0) {
            return NULL;
        }
        if ((s16) temp_f6 < 0) {
            return NULL;
        }
        if ((s16) temp_f4 >= 0x20) {
            return NULL;
        }
        if ((s16) temp_f6 >= 0x20) {
            return NULL;
        }
        temp_v1_2 = ((s16) ((s16) temp_f4 + ((s16) temp_f6 << 5)) * 4) + &D_8014F110;
        var_v0 = temp_v1_2->unk2;
        sp70 = (s32) var_v0;
        if (var_v0 == 0) {
            return NULL;
        }
        var_s1 = temp_v1_2->unk0;
        var_s2 = 0;
        if (sp70 > 0) {
loop_22:
            temp_s0 = *(D_8015F584 + (var_s1 * 2));
            var_v0 = (u16) &D_8015F580[temp_s0];
            temp_v1_3 = var_v0->unk0;
            if ((temp_v1_3 & 0x4000) != 0) {
                if (arg0->unkE != temp_s0) {
                    var_v0 = func_802AAE4C(&spD0, 0x40A00000, temp_f22, temp_f24, temp_f26, (s32) temp_s0);
                    if (var_v0 == (u16) (mk64_surface_map_ram *)1) {
                        arg0->unk10 = func_802ABE30(temp_f22, temp_f24, temp_f26, temp_s0 & 0xFFFF);
                        arg0->unkD = 0x40U;
                        arg0->unkE = temp_s0;
                        arg0->unkC = (s8) D_8015F580[temp_s0].surfaceType;
                        if ((D_8015F580[temp_s0].flags & 0x1000) != 0) {
                            arg0->unk14 = (mk64_surface_map_ram *)1;
                            return (mk64_surface_map_ram *)1;
                        }
                        arg0->unk14 = NULL;
                        return (mk64_surface_map_ram *)1;
                    }
                }
                goto block_37;
            }
            if ((temp_v1_3 & 0x8000) != 0) {
                if ((var_v0->unk1C != 1.0f) && (arg0->unkE != temp_s0)) {
                    var_v0 = func_802AB6C4(&spD0, 0x40A00000, temp_f22, temp_f24, temp_f26, (s32) temp_s0);
                    if (var_v0 == (u16) (mk64_surface_map_ram *)1) {
                        arg0->unk10 = func_802ABE30(temp_f22, temp_f24, temp_f26, temp_s0 & 0xFFFF);
                        arg0->unkD = 0x80U;
                        arg0->unkE = temp_s0;
                        arg0->unkC = (s8) D_8015F580[temp_s0].surfaceType;
                        return (mk64_surface_map_ram *)1;
                    }
                }
                goto block_37;
            }
            if ((var_v0->unk24 != 1.0f) && (arg0->unkE != temp_s0)) {
                var_v0 = func_802AB288(&spD0, 0x40A00000, temp_f22, temp_f24, temp_f26, (s32) temp_s0);
                if (var_v0 == (u16) (mk64_surface_map_ram *)1) {
                    arg0->unk10 = func_802ABE30(temp_f22, temp_f24, temp_f26, temp_s0 & 0xFFFF);
                    arg0->unkD = 0x20U;
                    arg0->unkE = temp_s0;
                    arg0->unkC = (s8) D_8015F580[temp_s0].surfaceType;
                    return (mk64_surface_map_ram *)1;
                }
            }
block_37:
            temp_t3 = (var_s2 + 1) & 0xFFFF;
            temp_at = temp_t3 < sp70;
            var_s2 = temp_t3;
            var_s1 = (var_s1 + 1) & 0xFFFF;
            if (temp_at == 0) {
                goto block_38;
            }
            goto loop_22;
        }
block_38:
        arg0->unk10 = temp_f24;
        arg0->unkC = 0;
        return (mk64_surface_map_ram *) var_v0;
    case 0x40:
        if (func_802AAE4C(&spD0, 0x40A00000, temp_f22, temp_f24, temp_f26, (s32) arg0->unkE) == (mk64_surface_map_ram *)1) {
            arg0->unk10 = func_802ABE30(temp_f22, temp_f24, temp_f26, arg0->unkE);
            return (mk64_surface_map_ram *)1;
        }
        goto block_10;
    case 0x20:
        if (func_802AB288(&spD0, 0x40A00000, temp_f22, temp_f24, temp_f26, (s32) arg0->unkE) == (mk64_surface_map_ram *)1) {
            arg0->unk10 = func_802ABE30(temp_f22, temp_f24, temp_f26, arg0->unkE);
            return (mk64_surface_map_ram *)1;
        }
        goto block_10;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AC22C.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit 0927f17aac197848d4ebdf0c6bbad74b01f0851c
s32 func_802AC760(UnkActorInner *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, u16 arg5, f32 arg6, f32 arg7, f32 arg8) {
    f32 sp6C;
    f32 sp64;
    f32 sp38;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    f32 sp18;
    f32 sp10;
    f32 spC;
    f32 sp8;
    Vtx *temp_a0;
    Vtx *temp_a2;
    Vtx *temp_a3;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f10;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f12_3;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f14_3;
    f32 temp_f16;
    f32 temp_f16_2;
    f32 temp_f18;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f2_4;
    f32 temp_f4;
    f32 temp_f6;
    mk64_surface_map_ram *temp_v1;
    s32 var_v0;

    temp_v1 = &D_8015F580[arg5];
    var_v0 = 1;
    if (arg2 < (f32) temp_v1->vtx3[0]) {
        return 0;
    }
    if (arg4 < (f32) temp_v1->vtx3[2]) {
        return 0;
    }
    if ((f32) temp_v1->vtx2[0] < arg2) {
        return 0;
    }
    if ((f32) temp_v1->vtx2[2] < arg4) {
        return 0;
    }
    if (arg3 < ((f32) temp_v1->vtx3[1] - (arg1 * 3.0f))) {
        return 0;
    }
    temp_a0 = temp_v1->vtxs[0];
    temp_a3 = temp_v1->vtxs[2];
    temp_a2 = temp_v1->vtxs[1];
    sp6C = (f32) temp_a3->v.ob[0];
    sp8 = arg4;
    sp10 = arg2;
    temp_f6 = (f32) temp_a0->v.ob[2] - arg4;
    sp64 = (f32) temp_a3->v.ob[2];
    temp_f4 = (f32) temp_a2->v.ob[0] - arg2;
    sp28 = temp_f6;
    sp24 = temp_f4;
    sp20 = (f32) temp_a0->v.ob[0] - arg2;
    temp_f0 = temp_f6 * temp_f4;
    sp8 = temp_f6;
    spC = temp_f4;
    temp_f18 = sp8;
    sp1C = (f32) temp_a2->v.ob[2] - arg4;
    temp_f2 = sp20 * sp1C;
    temp_f10 = temp_f0 - temp_f2;
    sp18 = temp_f10;
    sp38 = temp_f10;
    if (temp_f2 == temp_f0) {
        sp8 = arg4;
        temp_f0_2 = sp6C - sp10;
        temp_f2_2 = sp64 - arg4;
        if ((((sp1C * temp_f0_2) - (spC * temp_f2_2)) * ((temp_f2_2 * sp20) - (temp_f0_2 * temp_f18))) < 0.0f) {
            var_v0 = 0;
        }
    } else {
        temp_f2_3 = sp64 - arg4;
        temp_f0_3 = sp6C - arg2;
        temp_f14 = sp24 * temp_f2_3;
        temp_f12 = sp1C * temp_f0_3;
        if (temp_f14 == temp_f12) {
            if ((sp38 * ((temp_f2_3 * sp20) - (temp_f0_3 * sp28))) < 0.0f) {
                var_v0 = 0;
            }
        } else {
            temp_f16 = temp_f12 - temp_f14;
            if ((sp18 * temp_f16) < 0.0f) {
                var_v0 = 0;
            } else {
                temp_f14_2 = temp_f0_3 * sp28;
                temp_f12_2 = temp_f2_3 * sp20;
                if ((temp_f14_2 != temp_f12_2) && ((temp_f16 * (temp_f12_2 - temp_f14_2)) < 0.0f)) {
                    var_v0 = 0;
                }
            }
        }
    }
    if (var_v0 == 0) {
        return 0;
    }
    temp_f2_4 = temp_v1->height;
    temp_f12_3 = temp_v1->gravity;
    temp_f14_3 = temp_v1->rotation;
    temp_f16_2 = temp_v1->height2;
    temp_f0_4 = (temp_f2_4 * arg2) + (temp_f12_3 * arg3) + (temp_f14_3 * arg4) + temp_f16_2;
    if (arg1 < temp_f0_4) {
        if (temp_f0_4 < arg0->unk3C[2]) {
            arg0->unk34 = 1;
            arg0->unk3A = arg5;
            arg0->unk3C[2] = temp_f0_4 - arg1;
            arg0->unk60[0] = temp_v1->height;
            arg0->unk60[1] = temp_v1->gravity;
            arg0->unk60[2] = temp_v1->rotation;
        }
        return 0;
    }
    if (((temp_f2_4 * arg6) + (temp_f12_3 * arg7) + (temp_f14_3 * arg8) + temp_f16_2) < 0.0f) {
        return 0;
    }
    arg0->unk34 = 1;
    arg0->unk3A = arg5;
    arg0->unk3C[2] = temp_f0_4 - arg1;
    arg0->unk60[0] = temp_v1->height;
    arg0->unk60[1] = temp_v1->gravity;
    arg0->unk60[2] = temp_v1->rotation;
    return 1;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AC760.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit 0927f17aac197848d4ebdf0c6bbad74b01f0851c
s32 func_802ACBA4(UnkActorInner *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, u16 arg5, f32 arg6, f32 arg7, f32 arg8) {
    f32 sp70;
    f32 sp6C;
    f32 sp48;
    f32 sp3C;
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    Vtx *temp_a0;
    Vtx *temp_a2;
    Vtx *temp_a3;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f10;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f12_3;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f14_3;
    f32 temp_f16;
    f32 temp_f16_2;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f2_4;
    f32 temp_f2_5;
    f32 temp_f4;
    f32 temp_f4_2;
    f32 temp_f6;
    f32 temp_f8;
    f32 var_f10;
    mk64_surface_map_ram *temp_v1;
    s32 var_v0;

    temp_v1 = &D_8015F580[arg5];
    var_v0 = 1;
    if (arg2 < (f32) temp_v1->vtx3[0]) {
        return 0;
    }
    if ((f32) temp_v1->vtx2[0] < arg2) {
        return 0;
    }
    if ((f32) temp_v1->vtx2[1] < arg3) {
        return 0;
    }
    if (arg3 < (f32) temp_v1->vtx3[1]) {
        return 0;
    }
    temp_f2 = arg1 * 3.0f;
    if (arg4 < ((f32) temp_v1->vtx3[2] - temp_f2)) {
        return 0;
    }
    if (((f32) temp_v1->vtx2[2] + temp_f2) < arg4) {
        return 0;
    }
    temp_a0 = temp_v1->vtxs[0];
    temp_a3 = temp_v1->vtxs[2];
    temp_a2 = temp_v1->vtxs[1];
    sp70 = (f32) temp_a3->v.ob[0];
    sp8 = arg3;
    sp10 = arg2;
    temp_f10 = (f32) temp_a0->v.ob[1] - arg3;
    sp6C = (f32) temp_a3->v.ob[1];
    temp_f6 = (f32) temp_a2->v.ob[0] - arg2;
    sp2C = temp_f10;
    sp28 = temp_f6;
    sp24 = (f32) temp_a0->v.ob[0] - arg2;
    temp_f0 = temp_f10 * temp_f6;
    sp8 = temp_f10;
    spC = temp_f6;
    sp20 = (f32) temp_a2->v.ob[1] - arg3;
    temp_f2_2 = sp24 * sp20;
    temp_f4 = temp_f0 - temp_f2_2;
    sp1C = temp_f4;
    sp3C = temp_f4;
    if (temp_f2_2 == temp_f0) {
        sp14 = arg3;
        temp_f0_2 = sp70 - sp10;
        temp_f2_3 = sp6C - arg3;
        if ((((sp20 * temp_f0_2) - (spC * temp_f2_3)) * ((temp_f2_3 * sp24) - (temp_f0_2 * sp8))) < 0.0f) {
            var_v0 = 0;
        }
    } else {
        temp_f2_4 = sp6C - arg3;
        temp_f0_3 = sp70 - arg2;
        temp_f14 = sp28 * temp_f2_4;
        temp_f12 = sp20 * temp_f0_3;
        if (temp_f14 == temp_f12) {
            if ((sp3C * ((temp_f2_4 * sp24) - (temp_f0_3 * sp2C))) < 0.0f) {
                var_v0 = 0;
            }
        } else {
            temp_f16 = temp_f12 - temp_f14;
            if ((sp1C * temp_f16) < 0.0f) {
                var_v0 = 0;
            } else {
                temp_f14_2 = temp_f0_3 * sp2C;
                temp_f12_2 = temp_f2_4 * sp24;
                if ((temp_f14_2 != temp_f12_2) && ((temp_f16 * (temp_f12_2 - temp_f14_2)) < 0.0f)) {
                    var_v0 = 0;
                }
            }
        }
    }
    if (var_v0 == 0) {
        return 0;
    }
    temp_f2_5 = temp_v1->height;
    temp_f12_3 = temp_v1->gravity;
    temp_f14_3 = temp_v1->rotation;
    temp_f16_2 = temp_v1->height2;
    temp_f0_4 = (temp_f2_5 * arg2) + (temp_f12_3 * arg3) + (temp_f14_3 * arg4) + temp_f16_2;
    if (temp_v1->flags & 0x200) {
        temp_f4_2 = (temp_f2_5 * arg6) + (temp_f12_3 * arg7) + (temp_f14_3 * arg8) + temp_f16_2;
        sp48 = temp_f4_2;
        if ((temp_f0_4 > 0.0f) && (temp_f4_2 > 0.0f)) {
            if (temp_f0_4 < arg1) {
                arg0->unk30 = 1;
                arg0->unk36 = arg5;
                arg0->unk3C[0] = temp_f0_4 - arg1;
                arg0->unk48[0] = temp_v1->height;
                arg0->unk48[1] = temp_v1->gravity;
                arg0->unk48[2] = temp_v1->rotation;
                return 1;
            }
            return 0;
        }
        if ((temp_f0_4 < 0.0f) && (sp48 < 0.0f)) {
            temp_f0_5 = temp_f0_4 * -1.0f;
            if (temp_f0_5 < arg1) {
                arg0->unk30 = 1;
                arg0->unk36 = arg5;
                arg0->unk3C[0] = temp_f0_5 - arg1;
                arg0->unk48[0] = -temp_v1->height;
                arg0->unk48[1] = -temp_v1->gravity;
                arg0->unk48[2] = -temp_v1->rotation;
                return 1;
            }
            return 0;
        }
        if ((temp_f0_4 > 0.0f) && (sp48 < 0.0f)) {
            arg0->unk30 = 1;
            arg0->unk36 = arg5;
            arg0->unk3C[0] = -(temp_f0_4 + arg1);
            arg0->unk48[0] = -temp_v1->height;
            arg0->unk48[1] = -temp_v1->gravity;
            var_f10 = -temp_v1->rotation;
            arg0->unk48[2] = var_f10;
            return 1;
        }
        if ((temp_f0_4 < 0.0f) && (sp48 > 0.0f)) {
            arg0->unk30 = 1;
            arg0->unk36 = arg5;
            arg0->unk3C[0] = temp_f0_4 + arg1;
            arg0->unk48[0] = temp_v1->height;
            arg0->unk48[1] = temp_v1->gravity;
            arg0->unk48[2] = temp_v1->rotation;
            return 1;
        }
        if (temp_f0_4 == 0.0f) {
            if (sp48 >= 0.0f) {
                arg0->unk30 = 1;
                arg0->unk36 = arg5;
                arg0->unk3C[0] = sp48 + arg1;
                arg0->unk48[0] = temp_v1->height;
                arg0->unk48[1] = temp_v1->gravity;
                arg0->unk48[2] = temp_v1->rotation;
                return 1;
            }
            arg0->unk30 = 1;
            arg0->unk36 = arg5;
            arg0->unk3C[0] = -(sp48 + arg1);
            arg0->unk48[0] = temp_v1->height;
            arg0->unk48[1] = temp_v1->gravity;
            var_f10 = temp_v1->rotation;
            arg0->unk48[2] = var_f10;
            return 1;
        }
        return 0;
    }
    if (arg1 < temp_f0_4) {
        if (temp_f0_4 < arg0->unk3C[0]) {
            arg0->unk30 = 1;
            arg0->unk36 = arg5;
            arg0->unk3C[0] = temp_f0_4 - arg1;
            arg0->unk48[0] = temp_v1->height;
            arg0->unk48[1] = temp_v1->gravity;
            arg0->unk48[2] = temp_v1->rotation;
        }
        return 0;
    }
    temp_f8 = (temp_f2_5 * arg6) + (temp_f12_3 * arg7) + (temp_f14_3 * arg8) + temp_f16_2;
    sp48 = temp_f8;
    if (temp_f8 < 0.0f) {
        return 0;
    }
    arg0->unk30 = 1;
    arg0->unk36 = arg5;
    arg0->unk3C[0] = temp_f0_4 - arg1;
    arg0->unk48[0] = temp_v1->height;
    arg0->unk48[1] = temp_v1->gravity;
    var_f10 = temp_v1->rotation;
block_54:
    arg0->unk48[2] = var_f10;
    return 1;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802ACBA4.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit 0927f17aac197848d4ebdf0c6bbad74b01f0851c
s32 func_802AD278(UnkActorInner *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, u16 arg5, f32 arg6, f32 arg7, f32 arg8) {
    f32 sp68;
    f32 sp64;
    f32 sp44;
    f32 sp38;
    f32 sp2C;
    f32 sp28;
    f32 sp24;
    f32 sp20;
    f32 sp1C;
    f32 sp14;
    f32 sp10;
    f32 spC;
    f32 sp8;
    Vtx *temp_a0;
    Vtx *temp_a2;
    Vtx *temp_a3;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f0_6;
    f32 temp_f10;
    f32 temp_f10_2;
    f32 temp_f12;
    f32 temp_f12_2;
    f32 temp_f12_3;
    f32 temp_f14;
    f32 temp_f14_2;
    f32 temp_f14_3;
    f32 temp_f16;
    f32 temp_f16_2;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f2_4;
    f32 temp_f4;
    f32 temp_f6;
    f32 temp_f8;
    f32 var_f8;
    mk64_surface_map_ram *temp_v1;
    s32 var_v0;

    temp_v1 = &D_8015F580[arg5];
    var_v0 = 1;
    if (arg4 < (f32) temp_v1->vtx3[2]) {
        return 0;
    }
    if ((f32) temp_v1->vtx2[2] < arg4) {
        return 0;
    }
    if ((f32) temp_v1->vtx2[1] < arg3) {
        return 0;
    }
    if (arg3 < (f32) temp_v1->vtx3[1]) {
        return 0;
    }
    temp_f0 = arg1 * 3.0f;
    if (arg2 < ((f32) temp_v1->vtx3[0] - temp_f0)) {
        return 0;
    }
    if (((f32) temp_v1->vtx2[0] + temp_f0) < arg2) {
        return 0;
    }
    temp_a0 = temp_v1->vtxs[0];
    temp_a3 = temp_v1->vtxs[2];
    temp_a2 = temp_v1->vtxs[1];
    sp64 = (f32) temp_a3->v.ob[2];
    sp8 = arg3;
    sp10 = arg4;
    temp_f8 = (f32) temp_a0->v.ob[1] - arg3;
    sp68 = (f32) temp_a3->v.ob[1];
    temp_f6 = (f32) temp_a2->v.ob[2] - arg4;
    sp2C = temp_f8;
    sp28 = temp_f6;
    sp24 = (f32) temp_a0->v.ob[2] - arg4;
    temp_f0_2 = temp_f8 * temp_f6;
    sp8 = temp_f8;
    spC = temp_f6;
    sp20 = (f32) temp_a2->v.ob[1] - arg3;
    temp_f2 = sp24 * sp20;
    temp_f10 = temp_f0_2 - temp_f2;
    sp1C = temp_f10;
    sp38 = temp_f10;
    if (temp_f2 == temp_f0_2) {
        sp14 = arg3;
        temp_f0_3 = sp64 - sp10;
        temp_f2_2 = sp68 - arg3;
        if ((((sp20 * temp_f0_3) - (spC * temp_f2_2)) * ((temp_f2_2 * sp24) - (temp_f0_3 * sp8))) < 0.0f) {
            var_v0 = 0;
        }
    } else {
        temp_f2_3 = sp68 - arg3;
        temp_f0_4 = sp64 - arg4;
        temp_f14 = sp28 * temp_f2_3;
        temp_f12 = sp20 * temp_f0_4;
        if (temp_f14 == temp_f12) {
            if ((sp38 * ((temp_f2_3 * sp24) - (temp_f0_4 * sp2C))) < 0.0f) {
                var_v0 = 0;
            }
        } else {
            temp_f16 = temp_f12 - temp_f14;
            if ((sp1C * temp_f16) < 0.0f) {
                var_v0 = 0;
            } else {
                temp_f14_2 = temp_f0_4 * sp2C;
                temp_f12_2 = temp_f2_3 * sp24;
                if ((temp_f14_2 != temp_f12_2) && ((temp_f16 * (temp_f12_2 - temp_f14_2)) < 0.0f)) {
                    var_v0 = 0;
                }
            }
        }
    }
    if (var_v0 == 0) {
        return 0;
    }
    temp_f2_4 = temp_v1->height;
    temp_f12_3 = temp_v1->gravity;
    temp_f14_3 = temp_v1->rotation;
    temp_f16_2 = temp_v1->height2;
    temp_f0_5 = (temp_f2_4 * arg2) + (temp_f12_3 * arg3) + (temp_f14_3 * arg4) + temp_f16_2;
    if (temp_v1->flags & 0x200) {
        temp_f10_2 = (temp_f2_4 * arg6) + (temp_f12_3 * arg7) + (temp_f14_3 * arg8) + temp_f16_2;
        sp44 = temp_f10_2;
        if ((temp_f0_5 > 0.0f) && (temp_f10_2 > 0.0f)) {
            if (temp_f0_5 < arg1) {
                arg0->unk32 = 1;
                arg0->unk38 = arg5;
                arg0->unk3C[1] = temp_f0_5 - arg1;
                arg0->unk54[0] = temp_v1->height;
                arg0->unk54[1] = temp_v1->gravity;
                arg0->unk54[2] = temp_v1->rotation;
                return 1;
            }
            return 0;
        }
        if ((temp_f0_5 < 0.0f) && (sp44 < 0.0f)) {
            temp_f0_6 = temp_f0_5 * -1.0f;
            if (temp_f0_6 < arg1) {
                arg0->unk32 = 1;
                arg0->unk38 = arg5;
                arg0->unk3C[1] = temp_f0_6 - arg1;
                arg0->unk54[0] = -temp_v1->height;
                arg0->unk54[1] = -temp_v1->gravity;
                arg0->unk54[2] = -temp_v1->rotation;
                return 1;
            }
            return 0;
        }
        if ((temp_f0_5 > 0.0f) && (sp44 < 0.0f)) {
            arg0->unk32 = 1;
            arg0->unk38 = arg5;
            arg0->unk3C[1] = -(temp_f0_5 + arg1);
            arg0->unk54[0] = -temp_v1->height;
            arg0->unk54[1] = -temp_v1->gravity;
            var_f8 = -temp_v1->rotation;
            arg0->unk54[2] = var_f8;
            return 1;
        }
        if ((temp_f0_5 < 0.0f) && (sp44 > 0.0f)) {
            arg0->unk32 = 1;
            arg0->unk38 = arg5;
            arg0->unk3C[1] = temp_f0_5 + arg1;
            arg0->unk54[0] = temp_v1->height;
            arg0->unk54[1] = temp_v1->gravity;
            arg0->unk54[2] = temp_v1->rotation;
            return 1;
        }
        if (temp_f0_5 == 0.0f) {
            if (sp44 >= 0.0f) {
                arg0->unk32 = 1;
                arg0->unk38 = arg5;
                arg0->unk3C[1] = sp44 + arg1;
                arg0->unk54[0] = temp_v1->height;
                arg0->unk54[1] = temp_v1->gravity;
                arg0->unk54[2] = temp_v1->rotation;
                return 1;
            }
            arg0->unk32 = 1;
            arg0->unk38 = arg5;
            arg0->unk3C[1] = -(sp44 + arg1);
            arg0->unk54[0] = temp_v1->height;
            arg0->unk54[1] = temp_v1->gravity;
            var_f8 = temp_v1->rotation;
            arg0->unk54[2] = var_f8;
            return 1;
        }
        return 0;
    }
    if (arg1 < temp_f0_5) {
        if (temp_f0_5 < arg0->unk3C[1]) {
            arg0->unk32 = 1;
            arg0->unk38 = arg5;
            arg0->unk3C[1] = temp_f0_5 - arg1;
            arg0->unk54[0] = temp_v1->height;
            arg0->unk54[1] = temp_v1->gravity;
            arg0->unk54[2] = temp_v1->rotation;
        }
        return 0;
    }
    temp_f4 = (temp_f2_4 * arg6) + (temp_f12_3 * arg7) + (temp_f14_3 * arg8) + temp_f16_2;
    sp44 = temp_f4;
    if (temp_f4 < 0.0f) {
        return 0;
    }
    arg0->unk32 = 1;
    arg0->unk38 = arg5;
    arg0->unk3C[1] = temp_f0_5 - arg1;
    arg0->unk54[0] = temp_v1->height;
    arg0->unk54[1] = temp_v1->gravity;
    var_f8 = temp_v1->rotation;
    arg0->unk54[2] = var_f8;
    return 1;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AD278.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit a8f43e46d33dcb7e04d9cdc44a10b3daf4c92c3b
s32 func_802AC760(f32, f32, f32, f32, f32, f32, f32, f32, f32); /* extern */
s32 func_802ACBA4(UnkActorInner *, f32, f32, f32, f32, s32, f32, f32, f32); /* extern */
s32 func_802AD278(UnkActorInner *, f32, f32, f32, f32, s32, f32, f32, f32); /* extern */
extern u8 D_8014F110;
extern s32 D_8015F584;
extern s16 D_8015F6E8;
extern s16 D_8015F6EA;
extern s16 D_8015F6F0;
extern s16 D_8015F6F2;

s32 func_802AD950(UnkActorInner *arg0, f32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, f32 arg6, f32 arg7) {
    s16 temp_v0_4;
    s16 temp_v1;
    s32 temp_f10;
    s32 temp_f16;
    s32 temp_t7;
    u16 temp_s6;
    u16 temp_v0;
    u16 temp_v0_2;
    u16 temp_v0_3;
    u16 temp_v0_5;
    u16 temp_v1_3;
    void *temp_v1_2;
    s32 phi_s4;
    s32 phi_s4_2;
    u16 phi_s2;
    s32 phi_s3;
    s32 phi_s4_3;
    s32 phi_s4_4;
    s32 phi_s4_5;
    s32 phi_s4_6;

    arg0->unk30 = 0;
    arg0->unk32 = 0;
    arg0->unk34 = 0;
    arg0->unk3C = 1000.0f;
    arg0->unk40 = 1000.0f;
    arg0->unk44 = 1000.0f;
    temp_v0 = arg0->unk3A;
    phi_s4_6 = 0;
    if (((s32) temp_v0 < (s32) D_8015F588) && (func_802AC760(arg1, arg2, arg3, arg4, (bitwise f32) temp_v0, arg5, arg6, arg7) == 1)) {
        phi_s4_6 = 0x00004000;
    }
    temp_v0_2 = arg0->unk36;
    phi_s4_4 = phi_s4_6;
    if (((s32) temp_v0_2 < (s32) D_8015F588) && (func_802ACBA4(arg0, arg1, arg2, arg3, arg4, (s32) temp_v0_2, arg5, arg6, arg7) == 1)) {
        phi_s4_4 = (phi_s4_6 | 0x2000) & 0xFFFF;
    }
    temp_v0_3 = arg0->unk38;
    phi_s4 = phi_s4_4;
    if (((s32) temp_v0_3 < (s32) D_8015F588) && (func_802AD278(arg0, arg1, arg2, arg3, arg4, (s32) temp_v0_3, arg5, arg6, arg7) == 1)) {
        phi_s4 = (phi_s4_4 | 0x8000) & 0xFFFF;
    }
    phi_s4_2 = phi_s4;
    phi_s4_3 = phi_s4;
    if (phi_s4 == 0x0000E000) {
        goto block_39;
    }
    temp_v0_4 = D_8015F6EA;
    temp_v1 = D_8015F6F2;
    temp_f10 = (s32) ((arg2 - (f32) temp_v0_4) / (f32) ((s32) (D_8015F6E8 - temp_v0_4) / 32));
    temp_f16 = (s32) ((arg4 - (f32) temp_v1) / (f32) ((s32) (D_8015F6F0 - temp_v1) / 32));
    if ((s16) temp_f10 < 0) {
        return 0;
    }
    if ((s16) temp_f16 < 0) {
        return 0;
    }
    if ((s16) temp_f10 >= 0x20) {
        return 0;
    }
    if ((s16) temp_f16 >= 0x20) {
        return 0;
    }
    temp_v1_2 = ((s16) ((s16) temp_f10 + ((s16) temp_f16 << 5)) * 4) + &D_8014F110;
    temp_s6 = temp_v1_2->unk2;
    phi_s3 = 0;
    if (temp_s6 == 0) {

    } else {
        phi_s2 = temp_v1_2->unk0;
        if ((s32) temp_s6 > 0) {
loop_22:
            phi_s4_3 = phi_s4_2;
            phi_s4_5 = phi_s4_2;
            if (phi_s4_2 == 0x0000E000) {

            } else {
                temp_v0_5 = *(D_8015F584 + (phi_s2 * 2));
                temp_v1_3 = D_8015F580[temp_v0_5].flags;
                if ((temp_v1_3 & 0x4000) != 0) {
                    if (((phi_s4_2 & 0x4000) == 0) && (arg0->unk3A != temp_v0_5) && (func_802AC760((bitwise f32) arg0, arg1, arg2, arg3, arg4, (bitwise f32) temp_v0_5, arg5, arg6, arg7) == 1)) {
                        phi_s4_5 = (phi_s4_2 | 0x4000) & 0xFFFF;
                    }
                } else if ((temp_v1_3 & 0x8000) != 0) {
                    if (((phi_s4_2 & 0x8000) == 0) && (arg0->unk38 != temp_v0_5) && (func_802AD278(arg0, arg1, arg2, arg3, arg4, (s32) temp_v0_5, arg5, arg6, arg7) == 1)) {
                        phi_s4_5 = (phi_s4_2 | 0x8000) & 0xFFFF;
                    }
                } else if (((phi_s4_2 & 0x2000) == 0) && (arg0->unk36 != temp_v0_5) && (func_802ACBA4(arg0, arg1, arg2, arg3, arg4, (s32) temp_v0_5, arg5, arg6, arg7) == 1)) {
                    phi_s4_5 = (phi_s4_2 | 0x2000) & 0xFFFF;
                }
                temp_t7 = (phi_s3 + 1) & 0xFFFF;
                phi_s4_2 = phi_s4_5;
                phi_s2 = (phi_s2 + 1) & 0xFFFF;
                phi_s3 = temp_t7;
                phi_s4_3 = phi_s4_5;
                if (temp_t7 < (s32) temp_s6) {
                    goto loop_22;
                }
            }
        }
    }
block_39:
    return phi_s4_3;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AD950.s")
#endif

#ifdef MIPS_TO_C
//generated by mips_to_c commit 3c3b0cede1a99430bfd3edf8d385802b94f91307
s32 func_802AAE4C(void *, f32, f32, f32, f32, s32); // extern
s32 func_802AB288(void *, void *, f32, f32, f32, s32); // extern
s32 func_802AB6C4(void *, void *, f32, f32, f32, s32); // extern
extern u8 D_8014F110;
extern s32 D_8015F584;

s32 func_802ADDC8(void *arg0, void *arg1, f32 arg2, f32 arg3, f32 arg4) {
    s16 temp_v0_4;
    s16 temp_v1;
    s32 temp_f4;
    s32 temp_f6;
    s32 temp_t7;
    u16 temp_s6;
    u16 temp_v0;
    u16 temp_v0_2;
    u16 temp_v0_3;
    u16 temp_v0_5;
    u16 temp_v1_3;
    void *temp_v1_2;
    s32 phi_s4;
    s32 phi_s4_2;
    u16 phi_s2;
    s32 phi_s3;
    s32 phi_s4_3;
    s32 phi_s4_4;
    s32 phi_s4_5;
    s32 phi_s4_6;

    arg0->unk0 = 0;
    arg0->unk2 = 0;
    arg0->unk4 = 0;
    arg0->unkC = 1000.0f;
    arg0->unk10 = 1000.0f;
    arg0->unk14 = 1000.0f;
    temp_v0 = arg0->unkA;
    phi_s4_6 = 0;
    if ((temp_v0 < D_8015F588) && (func_802AAE4C(arg1, arg2, arg3, arg4, (bitwise f32) temp_v0) == 1)) {
        phi_s4_6 = 0x4000;
    }
    temp_v0_2 = arg0->unk6;
    phi_s4_4 = phi_s4_6;
    if ((temp_v0_2 < D_8015F588) && (func_802AB288(arg0, arg1, arg2, arg3, arg4, temp_v0_2) == 1)) {
        phi_s4_4 = (phi_s4_6 | 0x2000) & 0xFFFF;
    }
    temp_v0_3 = arg0->unk8;
    phi_s4 = phi_s4_4;
    if ((temp_v0_3 < D_8015F588) && (func_802AB6C4(arg0, arg1, arg2, arg3, arg4, temp_v0_3) == 1)) {
        phi_s4 = (phi_s4_4 | 0x8000) & 0xFFFF;
    }
    phi_s4_2 = phi_s4;
    phi_s4_3 = phi_s4;
    if (phi_s4 == 0xE000) {
        goto block_39;
    }
    temp_v0_4 = D_8015F6EA;
    temp_v1 = D_8015F6F2;
    temp_f4 = (arg2 - temp_v0_4) / ((D_8015F6E8 - temp_v0_4) / 0x20);
    temp_f6 = (arg4 - temp_v1) / ((D_8015F6F0 - temp_v1) / 0x20);
    if (temp_f4 < 0) {
        return 0;
    }
    if (temp_f6 < 0) {
        return 0;
    }
    if (temp_f4 >= 0x20) {
        return 0;
    }
    if (temp_f6 >= 0x20) {
        return 0;
    }
    temp_v1_2 = ((temp_f4 + (temp_f6 << 5)) * 4) + &D_8014F110;
    temp_s6 = temp_v1_2->unk2;
    phi_s3 = 0;
    if (temp_s6 == 0) {

    } else {
        phi_s2 = temp_v1_2->unk0;
        if (temp_s6 > 0) {
loop_22:
            phi_s4_3 = phi_s4_2;
            phi_s4_5 = phi_s4_2;
            if (phi_s4_2 == 0xE000) {

            } else {
                temp_v0_5 = *(D_8015F584 + (phi_s2 * 2));
                temp_v1_3 = *(D_8015F580 + (temp_v0_5 * 0x2C));
                if ((temp_v1_3 & 0x4000) != 0) {
                    if (((phi_s4_2 & 0x4000) == 0) && (arg0->unkA != temp_v0_5) && (func_802AAE4C(arg0, (bitwise f32) arg1, arg2, arg3, arg4, temp_v0_5) == 1)) {
                        phi_s4_5 = (phi_s4_2 | 0x4000) & 0xFFFF;
                    }
                } else if ((temp_v1_3 & 0x8000) != 0) {
                    if (((phi_s4_2 & 0x8000) == 0) && (arg0->unk8 != temp_v0_5) && (func_802AB6C4(arg0, arg1, arg2, arg3, arg4, temp_v0_5) == 1)) {
                        phi_s4_5 = (phi_s4_2 | 0x8000) & 0xFFFF;
                    }
                } else if (((phi_s4_2 & 0x2000) == 0) && (arg0->unk6 != temp_v0_5) && (func_802AB288(arg0, arg1, arg2, arg3, arg4, temp_v0_5) == 1)) {
                    phi_s4_5 = (phi_s4_2 | 0x2000) & 0xFFFF;
                }
                temp_t7 = (phi_s3 + 1) & 0xFFFF;
                phi_s4_2 = phi_s4_5;
                phi_s2 = (phi_s2 + 1) & 0xFFFF;
                phi_s3 = temp_t7;
                phi_s4_3 = phi_s4_5;
                if (temp_t7 < temp_s6) {
                    goto loop_22;
                }
            }
        }
    }
block_39:
    return phi_s4_3;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802ADDC8.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit d9d3d6575355663122de59f6b2882d8f174e2355 on Dec-09-2022
s32 func_802ABB04(f32, f32, s32);                   /* extern */
extern ? D_8014F110;
extern s32 D_8015F584;
static f32 D_802B9E4C = -3000.0f;
static f32 D_802B9E50 = 3000.0f;
static f32 D_802B9E54 = 3000.0f;
static f32 D_802B9E58 = 3000.0f;
static f32 D_802B9E5C = 3000.0f;
static f32 D_802B9E60 = 3000.0f;

f32 func_802AE1C0(f32 arg0, f32 arg1, f32 arg2) {
    f32 temp_f0;
    f32 var_f20;
    s32 temp_at;
    s32 temp_f4;
    s32 temp_f6;
    s32 temp_t0;
    s32 temp_t9;
    s32 var_s2;
    u16 temp_s0;
    u16 temp_t1;
    u16 var_s1;
    void *temp_t2;

    var_f20 = D_802B9E4C;
    temp_f4 = (s32) ((arg0 - (f32) D_8015F6EA) / (f32) ((s32) (D_8015F6E8 - D_8015F6EA) / 32));
    temp_f6 = (s32) ((arg2 - (f32) D_8015F6F2) / (f32) ((s32) (D_8015F6F0 - D_8015F6F2) / 32));
    temp_t0 = (s16) temp_f6 << 5;
    temp_t2 = ((s16) ((s16) temp_f4 + temp_t0) * 4) + &D_8014F110;
    temp_t1 = temp_t2->unk2;
    if ((s16) temp_f4 < 0) {
        return D_802B9E50;
    }
    if ((s16) temp_f6 & 0x04000000) {
        return D_802B9E54;
    }
    if ((s16) temp_f4 >= 0x20) {
        return D_802B9E58;
    }
    if (temp_t0 >= 0x400) {
        return D_802B9E5C;
    }
    if (temp_t1 == 0) {
        return D_802B9E60;
    }
    var_s1 = temp_t2->unk0;
    var_s2 = 0;
    if ((s32) temp_t1 > 0) {
        do {
            temp_s0 = *(D_8015F584 + (var_s1 * 2));
            if ((D_8015F580[temp_s0].flags & 0x4000) && (func_802ABB04(arg0, arg2, temp_s0 & 0xFFFF) == 1)) {
                temp_f0 = func_802ABE30(arg0, arg1, arg2, temp_s0 & 0xFFFF);
                if ((temp_f0 <= arg1) && (var_f20 < temp_f0)) {
                    var_f20 = temp_f0;
                }
            }
            temp_t9 = (var_s2 + 1) & 0xFFFF;
            temp_at = temp_t9 < (s32) temp_t1;
            var_s2 = temp_t9;
            var_s1 = (var_s1 + 1) & 0xFFFF;
        } while (temp_at != 0);
    }
    return var_f20;
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AE1C0.s")
#endif

#ifdef MIPS_TO_C
//generated by m2c commit 0927f17aac197848d4ebdf0c6bbad74b01f0851c
extern s32 D_8015F59C;
extern s32 D_8015F5A0;
extern s32 D_8015F5A4;
extern s16 D_8015F6E8;
extern s16 D_8015F6EA;
extern s16 D_8015F6F0;
extern s16 D_8015F6F2;
extern s16 D_8015F6FA;
extern s16 D_8015F6FC;
static f32 D_802B9E64 = -0.9f;
static f32 D_802B9E68 = 0.9f;
static f32 D_802B9E6C = 0.1f;
static f32 D_802B9E70 = -0.1f;

void func_802AE434(Vtx *arg0, Vtx *arg1, Vtx *arg2, s8 arg3, u16 arg4) {
    s16 spBA;
    s16 spB8;
    s16 spB6;
    s16 spB2;
    s16 spAC;
    s16 spAA;
    f32 sp58;
    f32 sp54;
    f32 sp50;
    f32 sp4C;
    s16 sp4A;
    s16 sp48;
    s16 sp44;
    s16 sp42;
    Vtx *sp1C;                                      /* compiler-managed */
    f64 sp10;
    f64 sp8;
    f64 sp0;
    Vtx *temp_t1;
    Vtx *temp_t2;
    Vtx *temp_t6;
    f32 temp_f0;
    f32 temp_f18_2;
    f32 temp_f4_2;
    f64 temp_f12;
    f64 temp_f14;
    f64 temp_f16;
    f64 temp_f18;
    f64 temp_f2;
    f64 temp_f4;
    f64 temp_f8;
    mk64_surface_map_ram *temp_t0;
    s16 var_a0;
    s16 var_a1;
    s16 var_a2;
    s16 var_a3;
    s16 var_t1;
    s16 var_t2;
    s16 var_t3;
    s16 var_t4;
    s16 var_t5;
    s16 var_v0;
    s16 var_v1;
    s32 temp_a0;
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_a3;
    s32 temp_v0;
    s32 temp_v1;
    u16 temp_a0_2;
    u16 temp_a3_2;
    u16 temp_v1_2;
    u16 var_a1_2;
    u16 var_t8;
    u16 var_t9;

    temp_t0 = &D_8015F580[D_8015F588];
    temp_t0->vtxs[0] = arg0;
    temp_t0->vtxs[1] = arg1;
    temp_t0->vtxs[2] = arg2;
    if ((arg0->v.flag != 4) || (arg1->v.flag != 4) || (temp_t0->vtxs[2]->v.flag != 4)) {
        temp_t6 = temp_t0->vtxs[0];
        sp1C = temp_t6;
        temp_t1 = temp_t0->vtxs[1];
        temp_t2 = temp_t0->vtxs[2];
        var_a0 = temp_t6->v.ob[0];
        var_t3 = temp_t1->v.ob[0];
        var_a2 = temp_t6->v.ob[1];
        var_a1 = temp_t6->v.ob[2];
        var_a3 = temp_t1->v.ob[1];
        var_t4 = temp_t1->v.ob[2];
        var_t5 = temp_t2->v.ob[0];
        var_v0 = temp_t2->v.ob[1];
        var_v1 = temp_t2->v.ob[2];
        if ((var_a0 == var_t3) && (var_a1 == var_t4)) {
            temp_t0->vtxs[2] = arg1;
            temp_t0->vtxs[1] = arg2;
            var_a0 = arg0->v.ob[0];
            var_a2 = arg0->v.ob[1];
            var_a1 = arg0->v.ob[2];
            var_t3 = arg1->v.ob[0];
            var_a3 = arg1->v.ob[1];
            var_t4 = arg1->v.ob[2];
            var_t5 = arg2->v.ob[0];
            var_v0 = arg2->v.ob[1];
            var_v1 = arg2->v.ob[2];
        }
        if (var_a0 >= var_t3) {
            if (var_a0 >= var_t5) {
                sp4A = var_a0;
            } else {
                goto block_12;
            }
        } else if (var_t3 >= var_t5) {
            sp4A = var_t3;
        } else {
block_12:
            sp4A = var_t5;
        }
        if (var_a1 >= var_t4) {
            if (var_a1 >= var_v1) {
                sp48 = var_a1;
            } else {
                goto block_19;
            }
        } else if (var_t4 >= var_v1) {
            sp48 = var_t4;
        } else {
block_19:
            sp48 = var_v1;
        }
        if (var_a2 >= var_a3) {
            if (var_a2 >= var_v0) {
                sp42 = var_a2;
            } else {
                goto block_26;
            }
        } else if (var_a3 >= var_v0) {
            sp42 = var_a3;
        } else {
block_26:
            sp42 = var_v0;
        }
        if (var_t3 >= var_a0) {
            if (var_t5 >= var_a0) {
                sp44 = var_a0;
            } else {
                goto block_33;
            }
        } else if (var_t5 >= var_t3) {
            sp44 = var_t3;
        } else {
block_33:
            sp44 = var_t5;
        }
        if (var_a3 >= var_a2) {
            if (var_v0 >= var_a2) {
                var_t2 = var_a2;
            } else {
                var_t2 = var_v0;
            }
        } else if (var_v0 >= var_a3) {
            var_t2 = var_a3;
        } else {
            var_t2 = var_v0;
        }
        if (var_t4 >= var_a1) {
            if (var_v1 >= var_a1) {
                var_t1 = var_a1;
            } else {
                var_t1 = var_v1;
            }
        } else if (var_v1 >= var_t4) {
            var_t1 = var_t4;
        } else {
            var_t1 = var_v1;
        }
        spAA = var_v1;
        spB8 = var_a2;
        spB2 = var_a3;
        spAC = var_v0;
        temp_v0 = spB2 - spB8;
        temp_v1 = spAA - var_t4;
        spB6 = var_a1;
        spBA = var_a0;
        temp_a0 = var_t4 - spB6;
        temp_a1 = spAC - spB2;
        temp_a2 = var_t5 - var_t3;
        temp_a3 = var_t3 - spBA;
        temp_f12 = (f64) ((temp_v0 * temp_v1) - (temp_a0 * temp_a1));
        temp_f18 = temp_f12 * temp_f12;
        sp10 = temp_f18;
        temp_f14 = (f64) ((temp_a0 * temp_a2) - (temp_a3 * temp_v1));
        temp_f8 = temp_f14 * temp_f14;
        sp8 = temp_f8;
        temp_f16 = (f64) ((temp_a3 * temp_a1) - (temp_v0 * temp_a2));
        temp_f4 = temp_f16 * temp_f16;
        sp0 = temp_f4;
        temp_f2 = (f64) sqrtf((f32) (temp_f18 + temp_f8 + temp_f4));
        if ((temp_f2 != 0.0) && ((temp_f0 = (f32) ((f64) (f32) temp_f12 / temp_f2), sp58 = temp_f0, temp_f4_2 = (f32) ((f64) (f32) temp_f14 / temp_f2), sp1C = temp_f4_2, sp54 = temp_f4_2, temp_f18_2 = (f32) ((f64) (f32) temp_f16 / temp_f2), sp50 = temp_f18_2, sp4C = -(((f32) spBA * temp_f0) + (temp_f4_2 * (f32) spB8) + (temp_f18_2 * (f32) spB6)), (D_8015F59C == 0)) || (!(temp_f4_2 < D_802B9E64) && !(D_802B9E68 < temp_f4_2))) && ((D_8015F5A0 == 0) || !(sp1C < D_802B9E6C) || !(D_802B9E70 < sp1C))) {
            temp_t0->vtx2[0] = sp4A;
            temp_t0->vtx3[2] = var_t1;
            temp_t0->vtx3[1] = var_t2;
            temp_t0->vtx3[0] = sp44;
            temp_t0->vtx2[1] = sp42;
            temp_t0->vtx2[2] = sp48;
            if (sp44 < D_8015F6EA) {
                D_8015F6EA = sp44;
            }
            if (var_t2 < D_8015F6EE) {
                D_8015F6EE = var_t2;
            }
            if (var_t1 < D_8015F6F2) {
                D_8015F6F2 = var_t1;
            }
            if (D_8015F6E8 < sp4A) {
                D_8015F6E8 = sp4A;
            }
            if (D_8015F6EC < sp42) {
                D_8015F6EC = sp42;
            }
            if (D_8015F6F0 < sp48) {
                D_8015F6F0 = sp48;
            }
            temp_t0->height = sp58;
            temp_t0->gravity = sp54;
            var_a1_2 = arg4;
            temp_t0->rotation = sp50;
            temp_t0->height2 = sp4C;
            temp_t0->surfaceType = (u16) arg3;
            D_8015F6FA = 0;
            D_8015F6FC = 0;
            temp_a3_2 = temp_t0->vtxs[0]->v.flag;
            temp_v1_2 = temp_t0->vtxs[1]->v.flag;
            temp_a0_2 = temp_t0->vtxs[2]->v.flag;
            if ((temp_a3_2 == 1) && (temp_v1_2 == 1) && (temp_a0_2 == 1)) {
                var_t8 = (arg4 | 0x400) & 0xFFFF;
                goto block_81;
            }
            if ((temp_a3_2 == 2) && (temp_v1_2 == 2) && (temp_a0_2 == 2)) {
                var_a1_2 = (arg4 | 0x800) & 0xFFFF;
            } else if ((temp_a3_2 == 3) && (temp_v1_2 == 3) && (temp_a0_2 == 3)) {
                var_a1_2 = (arg4 | 0x1000) & 0xFFFF;
            } else if (D_8015F5A4 != 0) {
                var_t8 = (arg4 | 0x200) & 0xFFFF;
block_81:
                var_a1_2 = var_t8;
            }
            temp_t0->flags = var_a1_2;
            if (sp10 <= sp8) {
                var_t9 = var_a1_2 | 0x4000;
                if (sp0 <= sp8) {
                    goto block_89;
                }
            }
            if ((sp8 < sp10) && (sp0 <= sp10)) {
                temp_t0->flags |= 0x8000;
            } else {
                var_t9 = temp_t0->flags | 0x2000;
block_89:
                temp_t0->flags = var_t9;
            }
            D_8015F588 += 1;
        }
    }
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AE434.s")
#endif

#ifdef MIPS_TO_C
//generated by mips_to_c commit 3c3b0cede1a99430bfd3edf8d385802b94f91307
void func_802AE434(s32, s32, s32, s8, s32); // extern
extern ? D_8015F668;

void func_802AEB14(s32 arg0, s8 arg1, u16 arg2) {
    func_802AE434(*(&D_8015F668 + ((((arg0 & 0xFF0000) >> 0x10) >> 1) * 4)), *(&D_8015F668 + ((((arg0 & 0xFF00) >> 8) >> 1) * 4)), *(&D_8015F668 + (((arg0 & 0xFF) >> 1) * 4)), arg1, arg2);
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AEB14.s")
#endif

#ifdef MIPS_TO_C
//generated by mips_to_c commit 3c3b0cede1a99430bfd3edf8d385802b94f91307
void func_802AE434(s32, s32, s32, s8, s32); // extern
extern ? D_8015F668;

void func_802AEBA4(s32 arg0, s32 arg1, s8 arg2, u16 arg3) {
    s32 sp28;
    s32 sp24;
    s32 sp20;

    sp28 = *(&D_8015F668 + ((((arg1 & 0xFF0000) >> 0x10) >> 1) * 4));
    sp24 = *(&D_8015F668 + ((((arg1 & 0xFF00) >> 8) >> 1) * 4));
    sp20 = *(&D_8015F668 + (((arg1 & 0xFF) >> 1) * 4));
    func_802AE434(*(&D_8015F668 + ((((arg0 & 0xFF0000) >> 0x10) >> 1) * 4)), *(&D_8015F668 + ((((arg0 & 0xFF00) >> 8) >> 1) * 4)), *(&D_8015F668 + (((arg0 & 0xFF) >> 1) * 4)), arg2, arg3);
    func_802AE434(sp28, sp24, sp20, arg2, arg3);
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AEBA4.s")
#endif

#ifdef MIPS_TO_C
//generated by mips_to_c commit 3c3b0cede1a99430bfd3edf8d385802b94f91307
void func_802AE434(s32, s32, s32, s8, s32); // extern
extern ? D_8015F668;

void func_802AECA0(s32 arg0, s8 arg1, u16 arg2) {
    s32 sp2C;
    s32 sp24;
    s32 sp20;
    s32 temp_a0;
    s32 temp_a2;

    temp_a0 = *(&D_8015F668 + ((((arg0 & 0xFF0000) >> 0x10) >> 1) * 4));
    temp_a2 = *(&D_8015F668 + (((arg0 & 0xFF) >> 1) * 4));
    sp2C = temp_a0;
    sp24 = temp_a2;
    sp20 = *(&D_8015F668 + ((((arg0 & 0xFF000000) >> 0x18) >> 1) * 4));
    func_802AE434(temp_a0, *(&D_8015F668 + ((((arg0 & 0xFF00) >> 8) >> 1) * 4)), temp_a2, arg1, arg2);
    func_802AE434(temp_a0, sp24, sp20, arg1, arg2);
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AECA0.s")
#endif

#ifdef MIPS_TO_C
//generated by mips_to_c commit 3c3b0cede1a99430bfd3edf8d385802b94f91307
extern ? D_8015F668;
extern uintptr_t gSegmentTable;

void func_802AED6C(u32 arg0, s32 arg1, s32 arg2) {
    s32 temp_a2;
    s32 temp_t0;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s32 temp_v0_5;
    s32 temp_v1;
    s32 temp_v1_2;
    s32 phi_v0;
    s32 *phi_a0;
    s32 phi_v1;
    s32 phi_v0_2;
    void *phi_a0_2;
    s32 phi_v1_2;
    s32 phi_a2;
    s32 phi_a2_2;

    temp_v0 = *(&gSegmentTable + ((arg0 >> 0x18) * 4)) + (arg0 & 0xFFFFFF) + 0x80000000;
    phi_v0 = temp_v0;
    phi_v1 = 0;
    phi_v0_2 = temp_v0;
    phi_a2 = arg2;
    phi_v1_2 = 0;
    phi_a2_2 = arg2;
    if (arg1 != 0) {
        temp_t0 = arg1 & 3;
        if (temp_t0 != 0) {
            phi_a0 = (arg2 * 4) + &D_8015F668;
            do {
                *phi_a0 = phi_v0;
                temp_v1 = phi_v1 + 1;
                temp_v0_2 = phi_v0 + 0x10;
                temp_a2 = phi_a2_2 + 1;
                phi_v0 = temp_v0_2;
                phi_a0 += 4;
                phi_v1 = temp_v1;
                phi_v0_2 = temp_v0_2;
                phi_a2 = temp_a2;
                phi_v1_2 = temp_v1;
                phi_a2_2 = temp_a2;
            } while (temp_t0 != temp_v1);
            if (temp_v1 != arg1) {
                goto block_5;
            }
        } else {
block_5:
            phi_a0_2 = (phi_a2 * 4) + &D_8015F668;
            do {
                phi_a0_2->unk0 = phi_v0_2;
                temp_v0_3 = phi_v0_2 + 0x10;
                phi_a0_2->unk4 = temp_v0_3;
                temp_v0_4 = temp_v0_3 + 0x10;
                phi_a0_2->unk8 = temp_v0_4;
                temp_v0_5 = temp_v0_4 + 0x10;
                phi_a0_2->unkC = temp_v0_5;
                temp_v1_2 = phi_v1_2 + 4;
                phi_v0_2 = temp_v0_5 + 0x10;
                phi_a0_2 += 0x10;
                phi_v1_2 = temp_v1_2;
            } while (temp_v1_2 != arg1);
        }
    }
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AED6C.s")
#endif

s32 func_802AEE1C(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s16 arg5, s16 arg6, s16 arg7) {
    f32 temp_f0;
    f32 temp_f18;
    f32 temp_f2;

    temp_f0 = arg6 - arg4;
    temp_f2 = arg7 - arg5;
    if (temp_f0 == 0.0f) {
        if (arg4 < arg0) {
            return 0;
        }
        if (arg1 < arg4) {
            return 0;
        }
        if (temp_f2 > 0.0f) {
            if ((arg5 < arg2) && (arg3 < arg7)) {
                return 1;
            }
        } else if ((arg7 < arg2) && (arg3 < arg5)) {
            return 1;
        }
    } else {
        if (temp_f2 == 0.0f) {
            if (arg5 < arg2) {
                return 0;
            }
            if (arg3 < arg5) {
                return 0;
            }
            if (temp_f0 > 0.0f) {
                if ((arg4 < arg0) && (arg1 < arg6)) {
                    return 1;
                }
            } else if ((arg6 < arg0) && (arg1 < arg4)) {
                return 1;
            }
        } else {
            temp_f18 = ((temp_f0 / temp_f2) * (arg2 - arg5)) + arg4;
            if ((arg0 <= temp_f18) && (temp_f18 <= arg1)) {
                return 1;
            }
            temp_f18 = ((temp_f0 / temp_f2) * (arg3 - arg5)) + arg4;
            if ((arg0 <= temp_f18) && (temp_f18 <= arg1)) {
                return 1;
            }
            temp_f18 = ((temp_f2 / temp_f0) * (arg0 - arg4)) + arg5;
            if ((arg2 <= temp_f18) && (temp_f18 <= arg3)) {
                return 1;
            }
            temp_f18 = ((temp_f2 / temp_f0) * (arg1 - arg4)) + arg5;
            if ((arg2 <= temp_f18) && (temp_f18 <= arg3)) {
                return 1;
            }
        }
    }
    return 0;
}

s32 func_802AF0FC(s16 arg0, s16 arg1, s16 arg2, s16 arg3, u16 surfaceMapIndex) {
    mk64_surface_map_ram *temp_v0;
    s16 vx1;
    s16 vz1;
    s16 vx2;
    s16 vz2;
    s16 vx3;
    s16 vz3;

    temp_v0 = &D_8015F580[surfaceMapIndex];
    vx1 = temp_v0->vtxs[0]->v.ob[0];
    vz1 = temp_v0->vtxs[0]->v.ob[2];
    vx2 = temp_v0->vtxs[1]->v.ob[0];
    vz2 = temp_v0->vtxs[1]->v.ob[2];
    vx3 = temp_v0->vtxs[2]->v.ob[0];
    vz3 = temp_v0->vtxs[2]->v.ob[2];
    if ((vx1 >= arg0) && (arg1 >= vx1) && (vz1 >= arg2) && (arg3 >= vz1)) {
        return 1;
    }
    if ((vx2 >= arg0) && (arg1 >= vx2) && (vz2 >= arg2) && (arg3 >= vz2)) {
        return 1;
    }
    if ((vx3 >= arg0) && (arg1 >= vx3) && (vz3 >= arg2) && (arg3 >= vz3)) {
        return 1;
    }
    if (func_802AEE1C(arg0, arg1, arg2, arg3, vx1, vz1, vx2, vz2) == 1) {
        return 1;
    }
    if (func_802AEE1C(arg0, arg1, arg2, arg3, vx2, vz2, vx3, vz3) == 1) {
        return 1;
    }
    if (func_802AEE1C(arg0, arg1, arg2, arg3, vx3, vz3, vx1, vz1) == 1) {
        return 1;
    }
    return 0;
}

#ifdef MIPS_TO_C
//generated by m2c commit 0927f17aac197848d4ebdf0c6bbad74b01f0851c
extern ? D_8014F110;
extern s32 D_8015F584;
extern u16 D_8015F58A;
extern s16 D_8015F6E8;
extern s16 D_8015F6EA;
extern s16 D_8015F6F0;
extern s16 D_8015F6F2;
extern s32 gPrevLoadedAddress;

void func_802AF314(void) {
    s32 sp4C;
    ? *var_v0;
    s16 temp_a1;
    s16 temp_a3;
    s16 temp_s1;
    s16 temp_s2;
    s32 temp_fp;
    s32 temp_s6;
    s32 var_s0;
    s32 var_s3;
    s32 var_s5;
    s32 var_s7;
    void *temp_v0;
    void *temp_v0_2;

    var_v0 = &D_8014F110;
    do {
        var_v0 += 0x10;
        var_v0->unk-A = 0;
        var_v0->unk-6 = 0;
        var_v0->unk-2 = 0;
        var_v0->unk-E = 0;
    } while (var_v0 != &gNumActors);
    D_8015F58A = 0;
    D_8015F584 = gPrevLoadedAddress;
    var_s7 = 0;
    temp_s6 = (s32) (D_8015F6E8 - D_8015F6EA) / 32;
    temp_fp = (s32) (D_8015F6F0 - D_8015F6F2) / 32;
    do {
        var_s5 = 0;
        sp4C = temp_fp * var_s7;
loop_4:
        var_s0 = 0;
        var_s3 = 0;
        temp_s1 = (D_8015F6EA + (temp_s6 * var_s5)) - 0x14;
        temp_s2 = (D_8015F6F2 + sp4C) - 0x14;
        if ((s32) D_8015F588 > 0) {
            do {
                temp_a3 = temp_s2 + temp_fp + 0x28;
                temp_v0 = var_s3 + D_8015F580;
                if ((temp_v0->unkE >= temp_s2) && (temp_a3 >= temp_v0->unk8)) {
                    temp_a1 = temp_s1 + temp_s6 + 0x28;
                    if ((temp_v0->unkA >= temp_s1) && (temp_a1 >= temp_v0->unk4)) {
                        if (func_802AF0FC(temp_s1, temp_a1, temp_s2, temp_a3, (u16) var_s0) == 1) {
                            temp_v0_2 = (((var_s7 << 5) + var_s5) * 4) + &D_8014F110;
                            if (temp_v0_2->unk2 == 0) {
                                temp_v0_2->unk0 = (u16) D_8015F58A;
                            }
                            temp_v0_2->unk2 = (u16) (temp_v0_2->unk2 + 1);
                            *(D_8015F584 + (D_8015F58A * 2)) = (s16) var_s0;
                            D_8015F58A += 1;
                        }
                    }
                }
                var_s0 += 1;
                var_s3 += 0x2C;
            } while (var_s0 < (s32) D_8015F588);
        }
        var_s5 += 1;
        if (var_s5 != 0x20) {
            goto loop_4;
        }
        var_s7 += 1;
    } while (var_s7 != 0x20);
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AF314.s")
#endif

void func_802AF588(s32 arg0) {
    func_802AF5D8(arg0, -1, 0xFF);
}

void func_802AF5AC(s32 arg0, s8 arg1) {
    func_802AF5D8(arg0, arg1, 0xFF);
}

#ifdef MIPS_TO_C
//generated by mips_to_c commit 3c3b0cede1a99430bfd3edf8d385802b94f91307
void func_802AEB14(u32, s8, s32); // extern
void func_802AEBA4(u32, u32, s8, s32); // extern
void func_802AECA0(u32, s8, s32); // extern
void func_802AED6C(u32, s32, u32); // extern
extern s16 D_8015F6FA;
extern s16 D_8015F6FC;
extern uintptr_t gSegmentTable;

void func_802AF5D8(u32 arg0, s8 arg1, s32 arg2) {
    s32 temp_s5;
    s32 temp_v0;
    u32 temp_s0;
    u32 temp_s1;
    void *phi_s3;
    s32 phi_s6;
    s32 phi_s6_2;

    D_8015F6FA = 0;
    temp_s5 = arg2 & 0xFFFF;
    D_8015F6FC = 0;
    phi_s3 = *(&gSegmentTable + ((arg0 >> 0x18) * 4)) + (arg0 & 0xFFFFFF) + 0x80000000;
    phi_s6_2 = 0;
loop_1:
    temp_s0 = phi_s3->unk0;
    temp_s1 = phi_s3->unk4;
    temp_v0 = temp_s0 & 0xFF000000;
    if (temp_v0 == 0x6000000) {
        func_802AF5D8(temp_s1, arg1, temp_s5 & 0xFFFF);
        phi_s6 = phi_s6_2 + 1;
        goto block_12;
    }
    if (temp_v0 == 0x4000000) {
        func_802AED6C(temp_s1, (temp_s0 >> 0xA) & 0x3F, ((temp_s0 >> 0x10) & 0xFF) >> 1);
        phi_s6 = phi_s6_2 + 1;
        goto block_12;
    }
    if (temp_v0 == 0xBF000000) {
        D_8015F58C += 1;
        func_802AEB14(temp_s1, arg1, temp_s5 & 0xFFFF);
        phi_s6 = phi_s6_2 + 1;
        goto block_12;
    }
    if (temp_v0 == 0xB1000000) {
        D_8015F58C += 2;
        func_802AEBA4(temp_s0, temp_s1, arg1, temp_s5 & 0xFFFF);
        phi_s6 = phi_s6_2 + 1;
        goto block_12;
    }
    if (temp_v0 == 0xB5000000) {
        D_8015F58C += 2;
        func_802AECA0(temp_s1, arg1, temp_s5 & 0xFFFF);
        phi_s6 = phi_s6_2 + 1;
        goto block_12;
    }
    phi_s6 = phi_s6_2 + 1;
    if (temp_v0 != 0xB8000000) {
block_12:
        phi_s3 += 8;
        phi_s6_2 = phi_s6;
        if (phi_s6 != 0x1FFF) {
            goto loop_1;
        }
    }
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AF5D8.s")
#endif

void func_802AF7B4(uintptr_t addr, s32 uls, s32 ult) {
    s32 segment = SEGMENT_NUMBER2(addr);
    s32 offset = SEGMENT_OFFSET(addr);
    Gfx *phi_v0 = VIRTUAL_TO_PHYSICAL2(gSegmentTable[segment] + offset);
    s32 gfxCommand;

    uls = (uls << 12) & 0xFFF000;
    ult &= 0xFFF;

    while(1) {

        gfxCommand = phi_v0->words.w0 & 0xFF000000;

        if (gfxCommand == G_ENDDL << 24) {
            break;
        } else if (gfxCommand == (G_SETTILESIZE << 24)) {
            phi_v0->words.w0 = 0xF2000000 | uls | ult;

            break;
        }
        phi_v0++;
    }
}

#ifdef MIPS_TO_C
//generated by mips_to_c commit 3c3b0cede1a99430bfd3edf8d385802b94f91307
extern uintptr_t gSegmentTable;

void func_802AF83C(u32 arg0, u32 arg1, ? arg2, s8 arg3, u8 arg4, u8 arg5, u8 arg6) {
    u32 temp_v1;
    void *phi_v0;
    u32 phi_v1;

    phi_v0 = *(&gSegmentTable + ((arg0 >> 0x18) * 4)) + (arg0 & 0xFFFFFF) + 0x80000000;
    phi_v1 = 0;
    if (arg1 != 0) {
        do {
            temp_v1 = phi_v1 + 1;
            phi_v1 = temp_v1;
            if (arg4 != 0) {
                phi_v0->unkC = arg4;
                phi_v0->unkD = arg5;
                phi_v0->unkE = arg6;
            }
            phi_v0->unkF = arg3;
            phi_v0 += 0x10;
        } while (temp_v1 < arg1);
    }
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AF83C.s")
#endif

#ifdef MIPS_TO_C
//generated by mips_to_c commit 3c3b0cede1a99430bfd3edf8d385802b94f91307
void func_802AF83C(u32, s32, u32, s8, s32, s32, s32); // extern
extern uintptr_t gSegmentTable;

void func_802AF8BC(u32 arg0, s8 arg1, s32 arg2, s32 arg3, u8 arg4) {
    s32 temp_s2;
    s32 temp_s3;
    s32 temp_v1;
    u32 temp_a0;
    u32 temp_v0;
    void *phi_s0;

    temp_s2 = arg2 & 0xFF;
    temp_s3 = arg3 & 0xFF;
    phi_s0 = *(&gSegmentTable + ((arg0 >> 0x18) * 4)) + (arg0 & 0xFFFFFF) + 0x80000000;
loop_1:
    temp_v0 = phi_s0->unk0;
    temp_a0 = phi_s0->unk4;
    temp_v1 = temp_v0 & 0xFF000000;
    if (temp_v1 != 0xB8000000) {
        if (temp_v1 == 0x6000000) {
            func_802AF8BC(temp_a0, arg1, temp_s2 & 0xFF, temp_s3 & 0xFF, arg4);
            phi_s0 += 8;
        } else {
            if (temp_v1 == 0x4000000) {
                func_802AF83C(temp_a0, (temp_v0 >> 0xA) & 0x3F, ((temp_v0 >> 0x10) & 0xFF) >> 1, arg1, temp_s2, temp_s3, arg4);
            }
            phi_s0 += 8;
        }
        goto loop_1;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AF8BC.s")
#endif

void func_802AF9F0(Vec3f arg0, f32 arg1, Vec3f arg2) {
    arg2[0] -= arg0[0] * arg1;
    arg2[1] -= arg0[1] * arg1;
    arg2[2] -= arg0[2] * arg1;
}

#ifdef MIPS_TO_C
//generated by m2c commit 6b6e1390bf30013247343623194964ff13d0c45b
s32 func_802AC760(s16 *, f32, f32, f32, f32, s32, f32, f32, f32); /* extern */
s32 func_802ACBA4(s16 *, f32, f32, f32, f32, s32, f32, f32, f32); /* extern */
s32 func_802AD278(s16 *, f32, f32, f32, f32, s32, f32, f32, f32); /* extern */
extern ? D_8014F110;
extern s32 D_8015F584;
extern s16 D_8015F6E8;
extern s16 D_8015F6EA;
extern s16 D_8015F6F0;
extern s16 D_8015F6F2;

s32 func_802AFA34(Player *player, KartBoundingBoxCorner *corner, f32 cornerPos[0], f32 cornerPos[1], f32 cornerPos[2]) {
    f32 sp128;
    f32 sp11C;
    f32 sp110;
    f32 sp10C;
    f32 sp108;
    f32 sp104;
    s16 sp102;
    s16 sp100;
    s16 spFE;
    s16 spFC;
    s16 spFA;
    s16 spF8;
    f32 spD4;
    s32 sp8C;
    f32 temp_f0;
    f32 temp_f0_2;
    f32 temp_f0_3;
    f32 temp_f0_4;
    f32 temp_f0_5;
    f32 temp_f0_6;
    f32 temp_f20;
    f32 temp_f22;
    f32 temp_f24;
    f32 temp_f26;
    f32 temp_f2;
    f32 temp_f2_2;
    f32 temp_f2_3;
    f32 temp_f2_4;
    f32 temp_f2_5;
    f32 temp_f2_6;
    mk64_surface_map_ram *temp_v0_4;
    s16 temp_v0_2;
    s16 temp_v1;
    s32 temp_at;
    s32 temp_f10;
    s32 temp_f16;
    s32 temp_t6;
    s32 var_s2;
    u16 temp_s0;
    u16 temp_v0_3;
    u16 temp_v1_3;
    u16 var_s1;
    u8 temp_v0;
    void *temp_v1_2;

    sp104 = 1000.0f;
    sp108 = 1000.0f;
    sp10C = 1000.0f;
    temp_f20 = player->boundingBoxSize;
    spFE = 0x1388;
    sp100 = 0x1388;
    sp102 = 0x1388;
    spF8 = 0;
    spFA = 0;
    spFC = 0;
    temp_v0 = corner->surfaceFlags;
    temp_f22 = corner->cornerPos[0];
    temp_f24 = corner->cornerPos[1];
    temp_f26 = corner->cornerPos[2];
    switch (temp_v0) {                              /* irregular */
    case 0x80:
        if (func_802AD278(&spF8, temp_f20, temp_f22, temp_f24, temp_f26, (s32) corner->surfaceMapIndex, cornerPos[0], cornerPos[1], cornerPos[2]) == 1) {
            temp_f0 = func_802ABE30(temp_f22, temp_f24, temp_f26, corner->surfaceMapIndex);
            temp_f2 = player->pos[1];
            if (!(temp_f2 < temp_f0) && !((2.0f * temp_f20) < (temp_f2 - temp_f0))) {
                corner->cornerGroundY = temp_f0;
                func_802AF9F0(&sp11C, sp108, &corner->cornerPos[0]);
                return 1;
            }
        }
    default:
block_22:
        temp_v0_2 = D_8015F6EA;
        temp_v1 = D_8015F6F2;
        temp_f10 = (s32) ((temp_f22 - (f32) temp_v0_2) / (f32) ((s32) (D_8015F6E8 - temp_v0_2) / 32));
        temp_f16 = (s32) ((temp_f26 - (f32) temp_v1) / (f32) ((s32) (D_8015F6F0 - temp_v1) / 32));
        if ((s16) temp_f10 < 0) {
            goto block_57;
        }
        if ((s16) temp_f16 < 0) {
            goto block_57;
        }
        if ((s16) temp_f10 >= 0x20) {
            goto block_57;
        }
        if ((s16) temp_f16 >= 0x20) {
            goto block_57;
        }
        temp_v1_2 = ((s16) ((s16) temp_f10 + ((s16) temp_f16 << 5)) * 4) + &D_8014F110;
        temp_v0_3 = temp_v1_2->unk2;
        sp8C = (s32) temp_v0_3;
        if (temp_v0_3 == 0) {
            goto block_57;
        }
        var_s1 = temp_v1_2->unk0;
        var_s2 = 0;
        if (sp8C > 0) {
loop_34:
            temp_s0 = *(D_8015F584 + (var_s1 * 2));
            temp_v0_4 = &D_8015F580[temp_s0];
            temp_v1_3 = temp_v0_4->flags;
            if (temp_v1_3 & 0x4000) {
                if ((corner->surfaceMapIndex != temp_s0) && (func_802AC760(&spF8, temp_f20, temp_f22, temp_f24, temp_f26, (s32) temp_s0, cornerPos[0], cornerPos[1], cornerPos[2]) == 1)) {
                    temp_f0_2 = func_802ABE30(temp_f22, temp_f24, temp_f26, temp_s0 & 0xFFFF);
                    temp_f2_2 = player->pos[1];
                    if (!(temp_f2_2 < temp_f0_2) && !((2.0f * temp_f20) < (temp_f2_2 - temp_f0_2))) {
                        spD4 = temp_f0_2;
                        func_802AF9F0(&sp128, sp10C, &corner->cornerPos[0]);
                        corner->cornerGroundY = spD4;
                        corner->surfaceFlags = 0x40;
                        corner->surfaceMapIndex = temp_s0;
                        corner->surfaceType = (u8) D_8015F580[temp_s0].surfaceType;
                        if (D_8015F580[temp_s0].flags & 0x1000) {
                            corner->unk_14 = 1;
                            return 1;
                        }
                        corner->unk_14 = 0;
                        return 1;
                    }
                }
                goto block_55;
            }
            if (temp_v1_3 & 0x8000) {
                if ((temp_v0_4->gravity != 0.0f) && (corner->surfaceMapIndex != temp_s0) && (func_802AD278(&spF8, temp_f20, temp_f22, temp_f24, temp_f26, (s32) temp_s0, cornerPos[0], cornerPos[1], cornerPos[2]) == 1)) {
                    temp_f0_3 = func_802ABE30(temp_f22, temp_f24, temp_f26, temp_s0 & 0xFFFF);
                    temp_f2_3 = player->pos[1];
                    if (!(temp_f2_3 < temp_f0_3) && !((2.0f * temp_f20) < (temp_f2_3 - temp_f0_3))) {
                        corner->cornerGroundY = temp_f0_3;
                        func_802AF9F0(&sp11C, sp108, &corner->cornerPos[0]);
                        corner->cornerGroundY = func_802ABE30(temp_f22, temp_f24, temp_f26, temp_s0 & 0xFFFF);
                        corner->surfaceFlags = 0x80;
                        corner->surfaceMapIndex = temp_s0;
                        corner->surfaceType = (u8) D_8015F580[temp_s0].surfaceType;
                        return 1;
                    }
                }
                goto block_55;
            }
            if ((temp_v0_4->gravity != 0.0f) && (corner->surfaceMapIndex != temp_s0) && (func_802ACBA4(&spF8, temp_f20, temp_f22, temp_f24, temp_f26, (s32) temp_s0, cornerPos[0], cornerPos[1], cornerPos[2]) == 1)) {
                temp_f0_4 = func_802ABE30(temp_f22, temp_f24, temp_f26, temp_s0 & 0xFFFF);
                temp_f2_4 = player->pos[1];
                if (!(temp_f2_4 < temp_f0_4) && !((2.0f * temp_f20) < (temp_f2_4 - temp_f0_4))) {
                    corner->cornerGroundY = temp_f0_4;
                    func_802AF9F0(&sp110, sp104, &corner->cornerPos[0]);
                    corner->surfaceFlags = 0x20;
                    corner->surfaceMapIndex = temp_s0;
                    corner->surfaceType = (u8) D_8015F580[temp_s0].surfaceType;
                    return 1;
                }
            }
block_55:
            temp_t6 = (var_s2 + 1) & 0xFFFF;
            temp_at = temp_t6 < sp8C;
            var_s2 = temp_t6;
            var_s1 = (var_s1 + 1) & 0xFFFF;
            if (temp_at == 0) {
                goto block_56;
            }
            goto loop_34;
        }
block_56:
        corner->cornerGroundY = temp_f24;
        corner->surfaceType = 0;
block_57:
        return 0;
    case 0x40:
        if (func_802AC760(&spF8, temp_f20, temp_f22, temp_f24, temp_f26, (s32) corner->surfaceMapIndex, cornerPos[0], cornerPos[1], cornerPos[2]) == 1) {
            temp_f0_5 = func_802ABE30(temp_f22, temp_f24, temp_f26, corner->surfaceMapIndex);
            temp_f2_5 = player->pos[1];
            if (!(temp_f2_5 < temp_f0_5) && !((2.0f * temp_f20) < (temp_f2_5 - temp_f0_5))) {
                corner->cornerGroundY = temp_f0_5;
                func_802AF9F0(&sp128, sp10C, &corner->cornerPos[0]);
                return 1;
            }
        }
        goto block_22;
    case 0x20:
        if (func_802ACBA4(&spF8, temp_f20, temp_f22, temp_f24, temp_f26, (s32) corner->surfaceMapIndex, cornerPos[0], cornerPos[1], cornerPos[2]) == 1) {
            temp_f0_6 = func_802ABE30(temp_f22, temp_f24, temp_f26, corner->surfaceMapIndex);
            temp_f2_6 = player->pos[1];
            if (!(temp_f2_6 < temp_f0_6) && !((2.0f * temp_f20) < (temp_f2_6 - temp_f0_6))) {
                corner->cornerGroundY = temp_f0_6;
                func_802AF9F0(&sp110, sp104, &corner->cornerPos[0]);
                return 1;
            }
        }
        goto block_22;
    case 0x0:
        goto block_22;
    }
}
#else
GLOBAL_ASM("asm/non_matchings/memory/func_802AFA34.s")
#endif
