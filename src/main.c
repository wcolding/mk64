#include <ultra64.h>
#include <PR/os.h>
#include <macros.h>
#include "types.h"
#include "config.h"
#include "profiler.h"
#include "main.h"
#include "memory.h"
#include "menus.h"
#include "segments.h"
#include "segment_symbols.h"
#include "common_structs.h"
#include <defines.h>
#include "framebuffers.h"
#include "camera.h"
#include "profiler.h"
#include "race_logic.h"
#include "skybox_and_splitscreen.h"
#include "code_800431B0.h"
#include "code_8008C1D0.h"
#include "code_80281780.h"
#include "limited_items.h"

// Declarations (not in this file)
void func_80091B78(void);

void audio_init();

void create_debug_thread(void);
void start_debug_thread(void);


struct SPTask *create_next_audio_frame_task(void);

struct VblankHandler *gVblankHandler1 = NULL;
struct VblankHandler *gVblankHandler2 = NULL;

struct SPTask *gActiveSPTask = NULL;
struct SPTask *sCurrentAudioSPTask = NULL;
struct SPTask* sCurrentDisplaySPTask = NULL;
struct SPTask* sNextAudioSPTask = NULL;
struct SPTask* sNextDisplaySPTask = NULL;


struct Controller gControllers[8];
struct Controller *gControllerOne = &gControllers[0];
struct Controller *gControllerTwo = &gControllers[1];
struct Controller *gControllerThree = &gControllers[2];
struct Controller *gControllerFour = &gControllers[3];
struct Controller *gControllerFive = &gControllers[4];
struct Controller *gControllerSix = &gControllers[5];
struct Controller *gControllerSeven = &gControllers[6];
struct Controller *gControllerEight = &gControllers[7];

Player gPlayers[8];
Player *gPlayerOne = &gPlayers[0];
Player *gPlayerTwo = &gPlayers[1];
Player *gPlayerThree = &gPlayers[2];
Player *gPlayerFour = &gPlayers[3];
Player *gPlayerFive = &gPlayers[4];
Player *gPlayerSix = &gPlayers[5];
Player *gPlayerSeven = &gPlayers[6];
Player *gPlayerEight = &gPlayers[7];

Player *gPlayerOneCopy = &gPlayers[0];
Player *gPlayerTwoCopy = &gPlayers[1];
UNUSED Player *gPlayerThreeCopy = &gPlayers[2];
UNUSED Player *gPlayerFourCopy = &gPlayers[3];

s32 D_800FD850[3];
struct GfxPool gGfxPools[2];
struct GfxPool *gGfxPool;

s32 gfxPool_padding; // is this necessary?
struct VblankHandler gGameVblankHandler;
struct VblankHandler sSoundVblankHandler;
OSMesgQueue gDmaMesgQueue, gGameVblankQueue, gGfxVblankQueue, unused_gMsgQueue, gIntrMesgQueue, gSPTaskMesgQueue;
OSMesgQueue sSoundMesgQueue;
OSMesg sSoundMesgBuf[1];
OSMesg gDmaMesgBuf[1], gGameMesgBuf;
OSMesg gGfxMesgBuf[1];
UNUSED OSMesg D_8014F010, D_8014F014;
OSMesg gIntrMesgBuf[16], gSPTaskMesgBuf[16];
OSMesg gMainReceivedMesg;
OSIoMesg gDmaIoMesg;
OSMesgQueue gSIEventMesgQueue;
OSMesg gSIEventMesgBuf[3];

OSContStatus gControllerStatuses[4];

OSContPad gControllerPads[4];
u8 gControllerBits;

u8 D_8014F110[4096];
u16 gNumActors;
u16 D_80150112;
s32 D_80150114;
f32 D_80150118;
u16 wasSoftReset;
u16 D_8015011E;

s32 D_80150120;
s32 gMenuSelectionFromQuit;
UNUSED s32 D_80150128;
UNUSED s32 D_8015012C;
f32 D_80150130[4];
//f32 D_80150134;
//f32 D_80150138;
//f32 D_8015013C;
UNUSED s32 D_80150140;
UNUSED s32 D_80150144;
f32 D_80150148;
f32 D_8015014C;
f32 D_80150150;
UNUSED f32 D_80150154;

struct D_80150158 gD_80150158[16];
uintptr_t gSegmentTable[16];
Gfx *gDisplayListHead;
struct SPTask *gGfxSPTask;
s32 D_801502A0;
s32 D_801502A4;
uintptr_t gPhysicalFramebuffers[3];
u32 D_801502B4;
UNUSED u32 D_801502B8;
UNUSED u32 D_801502BC;
Mat4 D_801502C0;

s32 padding[2048];

u16 D_80152300[4];
u16 D_80152308;

UNUSED OSThread paddingThread;
OSThread gIdleThread;
ALIGNED8 u8 gIdleThreadStack[STACKSIZE]; // Based on sm64 and padding between bss symbols.
OSThread gVideoThread;
ALIGNED8 u8 gVideoThreadStack[STACKSIZE];
UNUSED OSThread D_80156820;
UNUSED ALIGNED8 u8 D_8015680_Stack[STACKSIZE];
OSThread gGameLoopThread;
ALIGNED8 u8 gGameLoopThreadStack[STACKSIZE];
OSThread gAudioThread;
ALIGNED8 u8 gAudioThreadStack[STACKSIZE];
UNUSED OSThread D_8015CD30;
UNUSED ALIGNED8 u8 D_8015CD30_Stack[STACKSIZE / 2];

u8 gGfxSPTaskYieldBuffer[4352];
u32 gGfxSPTaskStack[256];
OSMesg gPIMesgBuf[32];
OSMesgQueue gPIMesgQueue;

s32 gGamestate = 0xFFFF;
u16 D_800DC510 = 0;
u16 D_800DC514 = 0;
u16 D_800DC518 = 0;
u16 D_800DC51C = 0;
u16 gEnableDebugMode = 0;
s32 gGamestateNext = 7; // = COURSE_DATA_MENU?;
UNUSED s32 D_800DC528 = 1;
s32 gActiveScreenMode = SCREEN_MODE_1P;
s32 gScreenModeSelection = SCREEN_MODE_1P;
UNUSED s32 D_800DC534  = 0;
s32 gPlayerCountSelection1 = 2;

s32 gModeSelection = GRAND_PRIX;
s32 D_800DC540 = 0;
s32 D_800DC544 = 0;
s32 gCCSelection = CC_50;
s32 gGlobalTimer = 0;
UNUSED s32 D_800DC550 = 0;
UNUSED s32 D_800DC554 = 0;
UNUSED s32 D_800DC558 = 0;
// Framebuffer rendering values (max 3)
u16 sRenderedFramebuffer = 0;
u16 sRenderingFramebuffer = 0;
UNUSED u16 D_800DC564 = 0;
s32 D_800DC568 = 0;
s32 D_800DC56C[8] = {0};
s16 sNumVBlanks = 0;
UNUSED s16 D_800DC590 = 0;
f32 gVBlankTimer = 0.0f;
f32 gCourseTimer = 0.0f;

void create_thread(OSThread *thread, OSId id, void (*entry)(void *), void *arg, void *sp, OSPri pri) {
    thread->next = NULL;
    thread->queue = NULL;
    osCreateThread(thread, id, entry, arg, sp, pri);
}

void main_func(void) {
    osInitialize();
    create_thread(&gIdleThread, 1, &thread1_idle, NULL, gIdleThreadStack + ARRAY_COUNT(gIdleThreadStack), 100);
    osStartThread(&gIdleThread);
}

/**
 * Initialize hardware, start main thread, then idle.
 */
void thread1_idle(void *arg) {
    osCreateViManager(OS_PRIORITY_VIMGR);
    if (osTvType == TV_TYPE_NTSC) {
        osViSetMode(&osViModeTable[OS_VI_NTSC_LAN1]);
    } else {
        osViSetMode(&osViModeTable[OS_VI_MPAL_LAN1]);
    }
    osViBlack(TRUE);
    osViSetSpecialFeatures(OS_VI_GAMMA_OFF);
    osCreatePiManager(OS_PRIORITY_PIMGR, &gPIMesgQueue, gPIMesgBuf, ARRAY_COUNT(gPIMesgBuf));
    wasSoftReset = (s16) osResetType;
    create_debug_thread();
    start_debug_thread();
    create_thread(&gVideoThread, 3, &thread3_video, arg, gVideoThreadStack + ARRAY_COUNT(gVideoThreadStack), 100);
    osStartThread(&gVideoThread);
    osSetThreadPri(NULL, 0);

    // halt
    while (TRUE);
}

void setup_mesg_queues(void) {
    osCreateMesgQueue(&gDmaMesgQueue, gDmaMesgBuf, ARRAY_COUNT(gDmaMesgBuf));
    osCreateMesgQueue(&gSPTaskMesgQueue, gSPTaskMesgBuf, ARRAY_COUNT(gSPTaskMesgBuf));
    osCreateMesgQueue(&gIntrMesgQueue, gIntrMesgBuf, ARRAY_COUNT(gIntrMesgBuf));
    osViSetEvent(&gIntrMesgQueue, (OSMesg) MESG_VI_VBLANK, 1);
    osSetEventMesg(OS_EVENT_SP, &gIntrMesgQueue, (OSMesg) MESG_SP_COMPLETE);
    osSetEventMesg(OS_EVENT_DP, &gIntrMesgQueue, (OSMesg) MESG_DP_COMPLETE);
}

void start_sptask(s32 taskType) {
    if (taskType == M_AUDTASK) {
        gActiveSPTask = sCurrentAudioSPTask;
    } else {
        gActiveSPTask = sCurrentDisplaySPTask;
    }
    osSpTaskLoad(&gActiveSPTask->task);
    osSpTaskStartGo(&gActiveSPTask->task);
    gActiveSPTask->state = SPTASK_STATE_RUNNING;
}

/**
 * Initializes the Fast3D OSTask structure.
 * Loads F3DEX or F3DLX based on the number of players
 **/
void create_gfx_task_structure(void) {
    gGfxSPTask->msgqueue = &gGfxVblankQueue;
    gGfxSPTask->msg = (OSMesg) 2;
    gGfxSPTask->task.t.type = M_GFXTASK;
    gGfxSPTask->task.t.flags = (1 << 1);
    gGfxSPTask->task.t.ucode_boot = rspbootTextStart;
    gGfxSPTask->task.t.ucode_boot_size = ((u8 *) rspbootTextEnd - (u8 *) rspbootTextStart);
    if (gGamestate != RACING || gPlayerCountSelection1 - 1 == 0) {
        gGfxSPTask->task.t.ucode = gspF3DEXTextStart;
        gGfxSPTask->task.t.ucode_data = gspF3DEXDataStart;
    } else {
        gGfxSPTask->task.t.ucode = gspF3DLXTextStart;
        gGfxSPTask->task.t.ucode_data = gspF3DLXDataStart;
    }
    gGfxSPTask->task.t.flags = 0;
    gGfxSPTask->task.t.flags = (1 << 1);
    gGfxSPTask->task.t.ucode_size = SP_UCODE_SIZE;
    gGfxSPTask->task.t.ucode_data_size = SP_UCODE_DATA_SIZE;
    gGfxSPTask->task.t.dram_stack = (u64 *) &gGfxSPTaskStack;
    gGfxSPTask->task.t.dram_stack_size = SP_DRAM_STACK_SIZE8;
    gGfxSPTask->task.t.output_buff = (u64 *) &gGfxSPTaskOutputBuffer;
    gGfxSPTask->task.t.output_buff_size = (u64 *) &gGfxSPTaskOutputBufferSize;
    gGfxSPTask->task.t.data_ptr = (u64 *) gGfxPool->gfxPool;
    gGfxSPTask->task.t.data_size = (gDisplayListHead - gGfxPool->gfxPool) * sizeof(Gfx);
    func_8008C214();
    gGfxSPTask->task.t.yield_data_ptr = (u64 *) &gGfxSPTaskYieldBuffer;
    gGfxSPTask->task.t.yield_data_size = 0xD00; /* Not equal to OS_YIELD_DATA_SIZE */
}


void init_controllers(void) {
    osCreateMesgQueue(&gSIEventMesgQueue, &gSIEventMesgBuf[0], ARRAY_COUNT(gSIEventMesgBuf));
    osSetEventMesg(OS_EVENT_SI, &gSIEventMesgQueue, (OSMesg) 0x33333333);
    osContInit(&gSIEventMesgQueue, &gControllerBits, &gControllerStatuses);
    if ((gControllerBits & 1) == 0) {
        sIsController1Unplugged = TRUE;
    } else {
        sIsController1Unplugged = FALSE;
    }
}

void update_controller(s32 index) { 
    struct Controller *controller = &gControllers[index];
    u16 stick;

    if (sIsController1Unplugged) {
        return;
    }

    controller->rawStickX = gControllerPads[index].stick_x;
    controller->rawStickY = gControllerPads[index].stick_y;

    if ((gControllerPads[index].button & 4) != 0) {
        gControllerPads[index].button |= Z_TRIG;
    }
    controller->buttonPressed = gControllerPads[index].button & (gControllerPads[index].button ^ controller->button);
    controller->buttonDepressed = controller->button & (gControllerPads[index].button ^ controller->button);
    controller->button = gControllerPads[index].button;

    stick = 0;
    if (controller->rawStickX < -50) {
        stick |= L_JPAD;
    }
    if (controller->rawStickX > 50) {
        stick |= R_JPAD;
    }
    if (controller->rawStickY < -50) {
        stick |= D_JPAD;
    }
    if (controller->rawStickY > 50) {
        stick |= U_JPAD;
    }
    controller->stickPressed = stick & (stick ^ controller->stickDirection);
    controller->stickDepressed = controller->stickDirection & (stick ^ controller->stickDirection);
    controller->stickDirection = stick;
}

void read_controllers(void) {
    OSMesg msg;

    osContStartReadData(&gSIEventMesgQueue);
    osRecvMesg(&gSIEventMesgQueue, &msg, OS_MESG_BLOCK);
    osContGetReadData(&gControllerPads);
    update_controller(0);
    update_controller(1);
    update_controller(2);
    update_controller(3);
    gControllerFive->button = (s16) (((gControllerOne->button | gControllerTwo->button) | gControllerThree->button) | gControllerFour->button);
    gControllerFive->buttonPressed = (s16) (((gControllerOne->buttonPressed | gControllerTwo->buttonPressed) | gControllerThree->buttonPressed) | gControllerFour->buttonPressed);
    gControllerFive->buttonDepressed = (s16) (((gControllerOne->buttonDepressed | gControllerTwo->buttonDepressed) | gControllerThree->buttonDepressed) | gControllerFour->buttonDepressed);
    gControllerFive->stickDirection = (s16) (((gControllerOne->stickDirection | gControllerTwo->stickDirection) | gControllerThree->stickDirection) | gControllerFour->stickDirection);
    gControllerFive->stickPressed = (s16) (((gControllerOne->stickPressed | gControllerTwo->stickPressed) | gControllerThree->stickPressed) | gControllerFour->stickPressed);
    gControllerFive->stickDepressed = (s16) (((gControllerOne->stickDepressed | gControllerTwo->stickDepressed) | gControllerThree->stickDepressed) | gControllerFour->stickDepressed);
}

void func_80000BEC(void) {
    D_801502B4 = VIRTUAL_TO_PHYSICAL(&gZBuffer);
}

void dispatch_audio_sptask(struct SPTask *spTask) {
    osWritebackDCacheAll();
    osSendMesg(&gSPTaskMesgQueue, spTask, OS_MESG_NOBLOCK);
}

void exec_display_list(struct SPTask *spTask) {
    osWritebackDCacheAll();
    spTask->state = SPTASK_STATE_NOT_STARTED;
    if (sCurrentDisplaySPTask == NULL) {
        sCurrentDisplaySPTask = spTask;
        sNextDisplaySPTask = NULL;
        osSendMesg(&gIntrMesgQueue, (OSMesg) MESG_START_GFX_SPTASK, OS_MESG_NOBLOCK);
    }
    else{
        sNextDisplaySPTask = spTask;
    }
}

/**
 * Set default RCP (Reality Co-Processor) settings.
 */
void init_rcp(void) {
    move_segment_table_to_dmem();
    init_rdp();
    set_viewport();
    select_framebuffer();
    init_z_buffer();
}

/**
 * End the master display list and initialize the graphics task structure for the next frame to be rendered.
 */
void end_master_display_list(void) {
    gDPFullSync(gDisplayListHead++);
    gSPEndDisplayList(gDisplayListHead++);
    create_gfx_task_structure();
}

// clear_frame_buffer from SM64, with a few edits
//! TODO: Why did void* work for matching
void *clear_framebuffer(s32 color) {
    gDPPipeSync(gDisplayListHead++);

    gDPSetRenderMode(gDisplayListHead++, G_RM_OPA_SURF, G_RM_OPA_SURF2);
    gDPSetCycleType(gDisplayListHead++, G_CYC_FILL);

    gDPSetFillColor(gDisplayListHead++, color);
    gDPFillRectangle(gDisplayListHead++, 0, 0, SCREEN_WIDTH - 1,
                     SCREEN_HEIGHT - 1);

    gDPPipeSync(gDisplayListHead++);

    gDPSetCycleType(gDisplayListHead++, G_CYC_1CYCLE);
}

void rendering_init(void) {
    gGfxPool = &gGfxPools[0];
    set_segment_base_addr(1, gGfxPool);
    gGfxSPTask = &gGfxPool->spTask;
    gDisplayListHead = gGfxPool->gfxPool;
    init_rcp();
    clear_framebuffer(0);
    end_master_display_list();
    exec_display_list(&gGfxPool->spTask);
    sRenderingFramebuffer++;
    gGlobalTimer++;
}

void config_gfx_pool(void) {
    gGfxPool = &gGfxPools[gGlobalTimer & 1];
    set_segment_base_addr(1, gGfxPool);
    gDisplayListHead = gGfxPool->gfxPool;
    gGfxSPTask = &gGfxPool->spTask;
}

/**
 * Send current master display list for rendering.
 * Tell the VI which colour framebuffer to display.
 * Yields to the VI framerate twice, locking the game at 30 FPS.
 * Selects the next framebuffer to be rendered and displayed.
 */
void display_and_vsync(void) {
    profiler_log_thread5_time(BEFORE_DISPLAY_LISTS);
    osRecvMesg(&gGfxVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    exec_display_list(&gGfxPool->spTask);
    profiler_log_thread5_time(AFTER_DISPLAY_LISTS);
    osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    osViSwapBuffer((void *) PHYSICAL_TO_VIRTUAL(gPhysicalFramebuffers[sRenderedFramebuffer]));
    profiler_log_thread5_time(THREAD5_END);
    osRecvMesg(&gGameVblankQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    crash_screen_set_framebuffer(gPhysicalFramebuffers[sRenderedFramebuffer]);
    if (++sRenderedFramebuffer == 3) {
        sRenderedFramebuffer = 0;
    }
    if (++sRenderingFramebuffer == 3) {
        sRenderingFramebuffer = 0;
    }
    gGlobalTimer++;
}

void init_seg_80280000(void) {
    bzero(SEG_80280000, 0xDF00);
    osWritebackDCacheAll();
    dma_copy(SEG_80280000, &_code_80280000SegmentRomStart, ALIGN16((u32)&_code_80280000SegmentRomEnd - (u32)&_code_80280000SegmentRomStart));
    osInvalICache(SEG_80280000, 0xDF00);
    osInvalDCache(SEG_80280000, 0xDF00);
}

void init_seg_8028DF00(void) {
    bzero(SEG_8028DF00, 0x2C470);
    osWritebackDCacheAll();
    dma_copy(SEG_8028DF00, &_code_8028DF00SegmentRomStart, ALIGN16((u32)&_code_8028DF00SegmentRomEnd - (u32)&_code_8028DF00SegmentRomStart));
    osInvalICache(SEG_8028DF00, 0x2C470);
    osInvalDCache(SEG_8028DF00, 0x2C470);
}

void dma_copy(u8 *dest, u8 *arg1, u32 size) {

    osInvalDCache(dest, size);
    while(size > 0x100) {
        osPiStartDma(&gDmaIoMesg, 0, 0, arg1, dest, 0x100, &gDmaMesgQueue);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
        size -= 0x100;
        arg1 += 0x100;
        dest += 0x100;
    }
    if (size != 0) {
        osPiStartDma(&gDmaIoMesg, 0, 0, arg1, dest, size, &gDmaMesgQueue);
        osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, 1);
    }
}

/**
 * Setup main segments and framebuffers.
 */
void setup_game_memory(void) {
    UNUSED u32 pad[2];
    u32 sp2C;
    u32 sp40;
    s32 texture_seg;
    s32 sp38;
    UNUSED s32 unknown_padding;

    init_seg_8028DF00();
    gHeapEndPtr = SEG_8028DF00;
    set_segment_base_addr(0, 0x80000000);
    func_802A7CF0(&D_801978D0, 0x80242F00);
    func_80000BEC();
    osInvalDCache(SEG_802BA370, 0x5810);
    osPiStartDma(&gDmaIoMesg, 0, 0, &_data_802BA370SegmentRomStart, SEG_802BA370, 0x5810, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);
    set_segment_base_addr(2, func_802A7D70(&_data_segment2SegmentRomStart, &_data_segment2SegmentRomEnd));
    sp2C = (u32)&_common_texturesSegmentRomEnd - (u32)&_common_texturesSegmentRomStart;
    sp2C = ALIGN16(sp2C);
    texture_seg = SEG_8028DF00-sp2C;
    osPiStartDma(&gDmaIoMesg, 0, 0, &_common_texturesSegmentRomStart, texture_seg, sp2C, &gDmaMesgQueue);
    osRecvMesg(&gDmaMesgQueue, &gMainReceivedMesg, OS_MESG_BLOCK);

    sp40 = *(u32 *)(texture_seg + 4);
    sp40 = ALIGN16(sp40);
    sp38 = gPrevLoadedAddress;
    mio0decode(texture_seg, sp38);
    set_segment_base_addr(0xD, sp38);

    gPrevLoadedAddress += sp40;
    D_8015F734 = gPrevLoadedAddress;
}

/**
 * @brief 
 * 
 */
void game_init_clear_framebuffer(void) {
    gGamestateNext = 0; // = START_MENU_FROM_QUIT?
    clear_framebuffer(0);
}

void race_logic_loop(void) {
    s16 i;
    s32 pad;
    u16 rotY;
    f32 pad2 = 0;

    D_80150112 = 0;
    D_80164AF0 = 0;
    if (D_800DC5FC != 0) {
        func_80290B14();
    }
    if (gIsInQuitToMenuTransition != 0) {
        func_802A38B4(); return;
    }

    if (sNumVBlanks >= 6) {
        sNumVBlanks = 5;
    }
    if (sNumVBlanks < 0) {
        sNumVBlanks = 1;
    }
    func_802A4EF4();

    switch(gActiveScreenMode) {
        case SCREEN_MODE_1P:
            D_80150114 = 2;
            staff_ghosts_loop();
            if (D_800DC5FC == 0) {

                for (i = 0; i < D_80150114; i++) {
                    if (D_8015011E) {
                        gCourseTimer += 0.01666666;
                    }
                    func_802909F0();
                    evaluate_player_collision();
                    func_800382DC();
                    func_8001EE98(gPlayerOneCopy, camera1, 0);
                    func_80028F70();
                    func_8028F474();
                    func_80059AC8();
                    update_simple_objects();
                    func_802966A0();
                    func_8028FCBC();

                }
                func_80022744();
            }
            func_8005A070();
            sNumVBlanks = 0;
            profiler_log_thread5_time(LEVEL_SCRIPT_EXECUTE);
            D_8015F788 = 0;
            func_802A59A4();
            if (gEnableDebugMode == 0) {
                D_800DC514 = 0;
            } else {
                if (D_800DC514 != 0) {

                    if ((gControllerOne->buttonPressed & R_TRIG) &&
                        (gControllerOne->button & A_BUTTON) &&
                        (gControllerOne->button & B_BUTTON)) {
                            D_800DC514 = 0;
                    }

                    rotY = camera1->rot[1];
                    D_801625E8 = D_800DC5EC->pathCounter;
                    if (rotY < 0x2000) {
                        func_80057A50(40, 100, "SOUTH  ", D_801625E8);
                    } else if (rotY < 0x6000) {
                        func_80057A50(40, 100, "EAST   ", D_801625E8);
                    } else if (rotY < 0xA000) {
                        func_80057A50(40, 100, "NORTH  ", D_801625E8);
                    } else if (rotY < 0xE000) {
                        func_80057A50(40, 100, "WEST   ", D_801625E8);
                    } else {
                        func_80057A50(40, 100, "SOUTH  ", D_801625E8);
                    }

                } else {
                    if ((gControllerOne->buttonPressed & L_TRIG) &&
                        (gControllerOne->button & A_BUTTON) &&
                        (gControllerOne->button & B_BUTTON)) {
                            D_800DC514 = 1;
                    }
                }
            }
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_VERTICAL:
            if (gCurrentCourseId == COURSE_DK_JUNGLE) {
                D_80150114 = 3;
            } else {
                D_80150114 = 2;
            }
            if (D_800DC5FC == 0) {
                    for (i = 0; i < D_80150114; i++) {
                        if (D_8015011E != 0) {
                            gCourseTimer += 0.01666666;
                        }
                        func_802909F0();
                        evaluate_player_collision();
                        func_800382DC();
                        func_8001EE98(gPlayerOneCopy, camera1, 0);
                        func_80029060();
                        func_8001EE98(gPlayerTwoCopy, camera2, 1);
                        func_80029150();
                        func_8028F474();
                        func_80059AC8();
                        update_simple_objects();
                        func_802966A0();
                        func_8028FCBC();
                    }
                    func_80022744();
                }
                func_8005A070();
                profiler_log_thread5_time(LEVEL_SCRIPT_EXECUTE);
                sNumVBlanks = 0;
                move_segment_table_to_dmem();
                init_rdp();
                if (D_800DC5B0 != 0) {
                    select_framebuffer();
                }
                D_8015F788 = 0;
                if (gPlayerWinningIndex == 0) {
                    func_802A5FAC();
                    func_802A5CB4();
                } else {
                    func_802A5CB4();
                    func_802A5FAC();
                }
            break;

        case SCREEN_MODE_2P_SPLITSCREEN_HORIZONTAL:

            if (gCurrentCourseId == COURSE_DK_JUNGLE) {
                D_80150114 = 3;
            } else {
                D_80150114 = 2;
            }

            if (D_800DC5FC == 0) {
                    for (i = 0; i < D_80150114; i++) {
                        if (D_8015011E != 0) {
                            gCourseTimer += 0.01666666;
                        }
                        func_802909F0();
                        evaluate_player_collision();
                        func_800382DC();
                        func_8001EE98(gPlayerOneCopy, camera1, 0);
                        func_80029060();
                        func_8001EE98(gPlayerTwoCopy, camera2, 1);
                        func_80029150();
                        func_8028F474();
                        func_80059AC8();
                        update_simple_objects();
                        func_802966A0();
                        func_8028FCBC();
                    }
                func_80022744();
            }
            profiler_log_thread5_time(LEVEL_SCRIPT_EXECUTE);
            sNumVBlanks = (u16)0;
            func_8005A070();
            move_segment_table_to_dmem();
            init_rdp();
            if (D_800DC5B0 != 0) {
                select_framebuffer();
            }
            D_8015F788 = 0;
            if (gPlayerWinningIndex == 0) {
                func_802A65B8();
                func_802A62A4();
            } else {
                func_802A62A4();
                func_802A65B8();
            }

            break;

        case SCREEN_MODE_3P_4P_SPLITSCREEN:
            if (gPlayerCountSelection1 == 3) {
                switch(gCurrentCourseId) {
                    case COURSE_BOWSER_CASTLE:
                    case COURSE_MOO_MOO_FARM:
                    case COURSE_SKYSCRAPER:
                    case COURSE_DK_JUNGLE:
                        D_80150114 = 3;
                        break;
                    default:
                        D_80150114 = 2;
                        break;
                }
            } else {
                // Four players
                switch(gCurrentCourseId) {
                    case COURSE_BLOCK_FORT:
                    case COURSE_DOUBLE_DECK:
                    case COURSE_BIG_DONUT:
                        D_80150114 = 2;
                        break;
                    case COURSE_DK_JUNGLE:
                        D_80150114 = 4;
                        break;
                    default:
                        D_80150114 = 3;
                        break;
                }
            }
            if (D_800DC5FC == 0) {
                for (i = 0; i < D_80150114; i++) {
                    if (D_8015011E != 0) {
                        gCourseTimer += 0.01666666;
                    }
                    func_802909F0();
                    evaluate_player_collision();
                    func_800382DC();
                    func_8001EE98(gPlayerOneCopy, camera1, 0);
                    func_80029158();
                    func_8001EE98(gPlayerTwo, camera2, 1);
                    func_800291E8();
                    func_8001EE98(gPlayerThree, camera3, 2);
                    func_800291F0();
                    func_8001EE98(gPlayerFour, camera4, 3);
                    func_800291F8();
                    func_8028F474();
                    func_80059AC8();
                    update_simple_objects();
                    func_802966A0();
                    func_8028FCBC();
                }
                func_80022744();
            }
        func_8005A070();
        sNumVBlanks = 0;
        profiler_log_thread5_time(LEVEL_SCRIPT_EXECUTE);
        move_segment_table_to_dmem();
        init_rdp();
        if (D_800DC5B0 != 0) {
            select_framebuffer();
        }
        D_8015F788 = 0;
        if (gPlayerWinningIndex == 0) {
            func_802A6BB0();
            func_802A6E94();
            func_802A7178();
            func_802A68CC();
        } else if (gPlayerWinningIndex == 1) {
            func_802A68CC();
            func_802A6E94();
            func_802A7178();
            func_802A6BB0();
        } else if (gPlayerWinningIndex == 2) {
            func_802A68CC();
            func_802A6BB0();
            func_802A7178();
            func_802A6E94();
        } else {
            func_802A68CC();
            func_802A6BB0();
            func_802A6E94();
            func_802A7178();
        }
        break;
    }

    if (gEnableDebugMode == 0) {
        gEnableResourceMeters = 0;
    } else {
        if (gEnableResourceMeters != 0) {
            resource_display();
            if ((!(gControllerOne->button & L_TRIG)) &&
                (gControllerOne->button & R_TRIG) &&
                (gControllerOne->buttonPressed & B_BUTTON)) {
                    gEnableResourceMeters = 0;
            }
        } else {
            if ((!(gControllerOne->button & L_TRIG)) &&
                (gControllerOne->button & R_TRIG) &&
                (gControllerOne->buttonPressed & B_BUTTON)) {
                        gEnableResourceMeters = 1;
            }
        }
    }
    func_802A4300();
    func_800591B4();
    func_80093E20();
    gDPFullSync(gDisplayListHead++);
    gSPEndDisplayList(gDisplayListHead++);
}

/**
 * mk64's game loop depends on a series of states.
 * It runs a wide branching series of code based on these states.
 * State 1) Clear framebuffer
 * State 2) Run menus
 * State 3) Process race related logic
 * State 4) Ending sequence
 * State 5) Credits
 * 
 * Note that the state doesn't flip-flop at random but is permanent
 * until the state changes (ie. Exit menus and start a race).
 */

void game_state_handler(void) {
    switch (gGamestate) {
        case 7:
            game_init_clear_framebuffer();
            break;
        case START_MENU_FROM_QUIT:
        case MAIN_MENU_FROM_QUIT:
        case PLAYER_SELECT_MENU_FROM_QUIT:
        case COURSE_SELECT_MENU_FROM_QUIT:
            // Display black
            osViBlack(0);
            update_menus();
            init_rcp();
            // gGfxPool->mtxPool->m or gGfxPool?
            func_80094A64(gGfxPool->mtxPool->m);
            break;
        case RACING:
            race_logic_loop();
            break;
        case ENDING_SEQUENCE:
            ending_sequence_loop();
            break;
        case CREDITS_SEQUENCE:
            credits_loop();
            break;
    }
}

void interrupt_gfx_sptask(void) {
    if (gActiveSPTask->task.t.type == M_GFXTASK) {
        gActiveSPTask->state = SPTASK_STATE_INTERRUPTED;
        osSpTaskYield();
    }
}

void receive_new_tasks(void) {
    s32 pad;
    struct SPTask *spTask;

    while(osRecvMesg(&gSPTaskMesgQueue, (OSMesg *) &spTask, OS_MESG_NOBLOCK) != -1) {
        spTask->state = SPTASK_STATE_NOT_STARTED;
        switch(spTask->task.t.type) {
            case 2:
                sNextAudioSPTask = spTask;
                break;
            case 1:
                sNextDisplaySPTask = spTask;
                break;
        }
    }

    if (sCurrentAudioSPTask == NULL && sNextAudioSPTask != NULL) {
        sCurrentAudioSPTask = sNextAudioSPTask;
        sNextAudioSPTask = NULL;
    }
    if (sCurrentDisplaySPTask == NULL && sNextDisplaySPTask != NULL) {
        sCurrentDisplaySPTask = sNextDisplaySPTask;
        sNextDisplaySPTask = NULL;
    }
}

void set_vblank_handler(s32 index, struct VblankHandler *handler, OSMesgQueue *queue, OSMesg *msg) {
    handler->queue = queue;
    handler->msg = msg;
    switch (index) {
        case 1:
            gVblankHandler1 = handler;
            break;
        case 2:
            gVblankHandler2 = handler;
            break;
    }
}

void start_gfx_sptask(void) {
    if (gActiveSPTask == NULL && sCurrentDisplaySPTask != NULL
        && sCurrentDisplaySPTask->state == SPTASK_STATE_NOT_STARTED) {
        profiler_log_gfx_time(TASKS_QUEUED);
        start_sptask(M_GFXTASK);
    }
}

void handle_vblank(void) {
    gVBlankTimer += 0.01666666;
    sNumVBlanks++;

    receive_new_tasks();

    // First try to kick off an audio task. If the gfx task is currently
    // running, we need to asynchronously interrupt it -- handle_sp_complete
    // will pick up on what we're doing and start the audio task for us.
    // If there is already an audio task running, there is nothing to do.
    // If there is no audio task available, try a gfx task instead.
    if (sCurrentAudioSPTask != NULL) {
        if (gActiveSPTask != NULL) {
            interrupt_gfx_sptask();
        } else {
            profiler_log_vblank_time();
            start_sptask(M_AUDTASK);
        }
    } else {
        if (gActiveSPTask == NULL && sCurrentDisplaySPTask != NULL
                && sCurrentDisplaySPTask->state != SPTASK_STATE_FINISHED) {
            profiler_log_gfx_time(TASKS_QUEUED);
            start_sptask(M_GFXTASK);
        }
    }

    /* This is where I would put my rumble code... If I had any. */
    #if ENABLE_RUMBLE
    rumble_thread_update_vi();
    #endif

    if (gVblankHandler1 != NULL) {
        osSendMesg(gVblankHandler1->queue, gVblankHandler1->msg, OS_MESG_NOBLOCK);
    }
    if (gVblankHandler2 != NULL) {
        osSendMesg(gVblankHandler2->queue, gVblankHandler2->msg, OS_MESG_NOBLOCK);
    }
}

void handle_dp_complete(void) {
    // Gfx SP task is completely done.
    if (sCurrentDisplaySPTask->msgqueue != NULL) {
        osSendMesg(sCurrentDisplaySPTask->msgqueue, sCurrentDisplaySPTask->msg, OS_MESG_NOBLOCK);
    }
    profiler_log_gfx_time(RDP_COMPLETE);
    sCurrentDisplaySPTask->state = SPTASK_STATE_FINISHED_DP;
    sCurrentDisplaySPTask = NULL;
}

void handle_sp_complete(void) {
    struct SPTask *curSPTask = gActiveSPTask;

    gActiveSPTask = NULL;
    
    if (curSPTask->state == SPTASK_STATE_INTERRUPTED) {
        // handle_vblank tried to start an audio task while there was already a
        // gfx task running, so it had to interrupt the gfx task. That interruption
        // just finished.
        if (osSpTaskYielded(curSPTask) == 0) {
            // The gfx task completed before we had time to interrupt it.
            // Mark it finished, just like below.
            curSPTask->state = SPTASK_STATE_FINISHED;
            profiler_log_gfx_time(RSP_COMPLETE);
        }
        // Start the audio task, as expected by handle_vblank.
        profiler_log_vblank_time();
        start_sptask(M_AUDTASK);
    } else {
        curSPTask->state = SPTASK_STATE_FINISHED;
        if (curSPTask->task.t.type == M_AUDTASK) {
            // After audio tasks come gfx tasks.
            profiler_log_vblank_time();
            if (sCurrentDisplaySPTask != NULL) {
                if (sCurrentDisplaySPTask->state != SPTASK_STATE_FINISHED) {
                    if (sCurrentDisplaySPTask->state != SPTASK_STATE_INTERRUPTED) {
                        profiler_log_gfx_time(TASKS_QUEUED);
                    }
                    start_sptask(M_GFXTASK);
                }
            }
            sCurrentAudioSPTask = NULL;
            if (curSPTask->msgqueue != NULL) {
                osSendMesg(curSPTask->msgqueue, curSPTask->msg, OS_MESG_NOBLOCK);
            }
        } else {
            // The SP process is done, but there is still a Display Processor notification
            // that needs to arrive before we can consider the task completely finished and
            // null out sCurrentDisplaySPTask. That happens in handle_dp_complete.
            profiler_log_gfx_time(RSP_COMPLETE);
        }
    };
}

void thread3_video(UNUSED void *arg0) {
    s32 i;
    u64 *framebuffer1;
    OSMesg msg;
    s32 pad[4];

    gPhysicalFramebuffers[0] = (uintptr_t *) &gFramebuffer0;
    gPhysicalFramebuffers[1] = (uintptr_t *) &gFramebuffer1;
    gPhysicalFramebuffers[2] = (uintptr_t *) &gFramebuffer2;

    framebuffer1 = &gFramebuffer1;
    for (i = 0; i < 19200; i++) {
        framebuffer1[i] = 0;
    }

    setup_mesg_queues();
    setup_game_memory();

    create_thread(&gAudioThread, 4, &thread4_audio, 0, gAudioThreadStack + ARRAY_COUNT(gAudioThreadStack), 20);
    osStartThread(&gAudioThread);

    create_thread(&gGameLoopThread, 5, &thread5_game_loop, 0, gGameLoopThreadStack + ARRAY_COUNT(gGameLoopThreadStack), 10);
    osStartThread(&gGameLoopThread);

    while (TRUE) {
        osRecvMesg(&gIntrMesgQueue, &msg, OS_MESG_BLOCK);
        switch ((u32) msg) {
            case MESG_VI_VBLANK:
                handle_vblank();
                break;
            case MESG_SP_COMPLETE:
                handle_sp_complete();
                break;
            case MESG_DP_COMPLETE:
                handle_dp_complete();
                break;
            case MESG_START_GFX_SPTASK:
                start_gfx_sptask();
                break;
        }
    }
}

void func_800025D4(void) {
    func_80091B78();
    gActiveScreenMode = SCREEN_MODE_1P;
    func_802A4D18();
}

void func_80002600(void) {
    func_80091B78();
    gActiveScreenMode = SCREEN_MODE_1P;
    func_802A4D18();
}

void func_8000262C(void) {
    func_80091B78();
    gActiveScreenMode = SCREEN_MODE_1P;
    func_802A4D18();
}

void func_80002658(void) {
    func_80091B78();
    gActiveScreenMode = SCREEN_MODE_1P;
    func_802A4D18();
}

/**
 * Sets courseId to NULL if 
 * 
 * 
 */
void update_gamestate(void) {
    switch (gGamestate) {
        case START_MENU_FROM_QUIT:
            func_80002658();
            gCurrentlyLoadedCourseId = COURSE_NULL;
            break;
        case MAIN_MENU_FROM_QUIT:
            func_800025D4();
            gCurrentlyLoadedCourseId = COURSE_NULL;
            break;
        case PLAYER_SELECT_MENU_FROM_QUIT:
            func_80002600();
            gCurrentlyLoadedCourseId = COURSE_NULL;
            break;
        case COURSE_SELECT_MENU_FROM_QUIT:
            func_8000262C();
            gCurrentlyLoadedCourseId = COURSE_NULL;
            break;
        case RACING:
            init_seg_8028DF00();
            setup_race();
            break;
        case ENDING_SEQUENCE:
            gCurrentlyLoadedCourseId = COURSE_NULL;
            init_seg_80280000();
            load_ending_sequence_royalraceway();
            break;
        case CREDITS_SEQUENCE:
            gCurrentlyLoadedCourseId = COURSE_NULL;
            init_seg_8028DF00();
            init_seg_80280000();
            load_credits();
            break;
        }
}

void thread5_game_loop(UNUSED void *arg) { 
    osCreateMesgQueue(&gGfxVblankQueue, &gGfxMesgBuf, 1);
    osCreateMesgQueue(&gGameVblankQueue, &gGameMesgBuf, 1);
    init_controllers();
    if (!wasSoftReset) {
        clear_nmi_buffer();
    }
    set_vblank_handler(2, &gGameVblankHandler, &gGameVblankQueue, (OSMesg) OS_EVENT_SW2);
    // These variables track stats such as player wins.
    // In the event of a console reset, it remembers them.
    gNmiUnknown1 = &pAppNmiBuffer[0];  // 2  u8's, tracks number of times player 1/2 won a VS race
    gNmiUnknown2 = &pAppNmiBuffer[2];  // 9  u8's, 3x3, tracks number of times player 1/2/3   has placed in 1st/2nd/3rd in a VS race
    gNmiUnknown3 = &pAppNmiBuffer[11]; // 12 u8's, 4x3, tracks number of times player 1/2/3/4 has placed in 1st/2nd/3rd in a VS race
    gNmiUnknown4 = &pAppNmiBuffer[23]; // 2  u8's, tracking number of Battle mode wins by player 1/2
    gNmiUnknown5 = &pAppNmiBuffer[25]; // 3  u8's, tracking number of Battle mode wins by player 1/2/3
    gNmiUnknown6 = &pAppNmiBuffer[28]; // 4  u8's, tracking number of Battle mode wins by player 1/2/3/4
    rendering_init();
    read_controllers();
    func_800C5CB8();

    while(TRUE) {
        func_800CB2C4();

        // Update the gamestate if it has changed.
        if (gGamestateNext != gGamestate) {
            gGamestate = gGamestateNext;
            update_gamestate();
        }

        UpdateRandoCourses();
        profiler_log_thread5_time(THREAD5_START);
        config_gfx_pool();
        read_controllers();
        game_state_handler();
        end_master_display_list();
        display_and_vsync();
    }
}

/**
 * Sound processing thread. Runs at 50 or 60 FPS according to osTvType.
 */
void thread4_audio(UNUSED void *arg) {
    UNUSED u32 unused[3];
    audio_init();
    osCreateMesgQueue(&sSoundMesgQueue, &sSoundMesgBuf, ARRAY_COUNT(sSoundMesgBuf));
    set_vblank_handler(1, &sSoundVblankHandler, &sSoundMesgQueue, (OSMesg) 512);

    while (TRUE) {
        OSMesg msg;
        struct SPTask *spTask;

        osRecvMesg(&sSoundMesgQueue, &msg, OS_MESG_BLOCK);

        profiler_log_thread4_time();

        spTask = create_next_audio_frame_task();
        if (spTask != NULL) {
            dispatch_audio_sptask(spTask);
        }
        profiler_log_thread4_time();
    }
}
