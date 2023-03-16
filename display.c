#include <dc/pvr.h>
#include <kos.h>
#include "display.h"
int currentBuffer;
uint16_t* framebuffer_1;
uint16_t* framebuffer_2;
uint32_t framebufferSize;
uint16_t* backbuffer;

/* Initialize double buffer
 * parameter: size of framebuffer */
void dis_initializeDoublebuffer(){
    framebufferSize = SCREEN_WIDTH * SCREEN_HEIGHT * sizeof(uint16_t);

    // allocate memory with 32-byte alignment
    framebuffer_1 = (uint16_t*) memalign(32, framebufferSize);
    framebuffer_2 = (uint16_t*) memalign(32, framebufferSize);

    memset(framebuffer_1,'\0', framebufferSize);
    memset(framebuffer_2,'\0', framebufferSize);

    backbuffer = framebuffer_1;
    currentBuffer = 1;
}

void dis_initializeDisplay(){
    //set our video mode
    vid_set_mode(DM_320x240, PM_RGB565);

    //initialize software double buffer
    dis_initializeDoublebuffer();
}



//flip double buffer
void dis_flipBuffer(){
    // Store Queue Trasnfer
    if(currentBuffer == 1){
        currentBuffer = 2;
        backbuffer = framebuffer_2;
        vid_waitvbl();
}
    // DMA Trasnfer
     dcache_flush_range((uint32_t) backbuffer,framebufferSize);
     while (!pvr_dma_ready());
     pvr_dma_transfer(backbuffer,(uint32_t) vram_s, framebufferSize,PVR_DMA_VRAM32, -1, NULL, (ptr_t) NULL);
}

/* Initialize double buffer
 * parameter: Red, Green, Blue */
void dis_clearBackBuffer(int r, int g, int b){
    memset(backbuffer, PACK_RGB565(r, g, b), framebufferSize);
}

