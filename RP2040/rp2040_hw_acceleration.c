#include "arm_2d.h"
#include "hardware/interp.h"

#define PICO_BLENDING_INIT \
    interp_config cfg = interp_default_config();\
    interp_config_set_blend(&cfg, true);\
    interp_set_config(interp0, 0, &cfg);\
    \
    cfg = interp_default_config();\
    interp_set_config(interp0, 1, &cfg);

__STATIC_INLINE void __unpack(uint16_t hwColor,
                            __arm_2d_color_fast_rgb_t * ptRGB)
{
    /* uses explicit extraction, leading to a more efficient autovectorized code */
    ptRGB->B = (uint16_t) (hwColor & 0x1F);
    ptRGB->G = (uint16_t) (hwColor & 0x7E0) ;
    ptRGB->R = (uint16_t) (hwColor & 0xF800);
}

__STATIC_INLINE uint16_t __pack(__arm_2d_color_fast_rgb_t * ptRGB)
{
    uint16_t result = 
    (ptRGB->R & 0xF800) | (ptRGB->G & 0x7E0) | (ptRGB->B & 0x1F);
    
    return result;
}

#define PICO_BLENDING(SRC,TARGET,ALPHA) \
  { \
    __arm_2d_color_fast_rgb_t tSrcPix, tTargetPix; \
    __unpack(*(SRC), &tSrcPix); \
    __unpack(*(TARGET), &tTargetPix); \
    interp0->accum[1] = (ALPHA); \
     \
    for (int i = 0; i < 3; i++)  \
    {     \
        interp0->base[1] = tTargetPix.BGRA[i]; \
        interp0->base[0] = tSrcPix.BGRA[i]; \
        tTargetPix.BGRA[i] = interp0->peek[1] & 0x0FFFF;  \
    } \
 \
    *(TARGET) = __pack(&tTargetPix); \
  }

__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_colour_filling_with_opacity(
                                        uint16_t *__RESTRICT pTargetBase,
                                        int16_t iTargetStride,
                                        arm_2d_size_t *__RESTRICT ptCopySize,
                                        uint16_t Colour,
                                        uint_fast16_t hwRatio)
{
    int_fast16_t iWidth = ptCopySize->iWidth;
    int_fast16_t iHeight = ptCopySize->iHeight;

#if !defined(__ARM_2D_CFG_UNSAFE_IGNORE_ALPHA_255_COMPENSATION__)
    hwRatio += (hwRatio == 255);
#endif

    PICO_BLENDING_INIT;

    uint16_t hwRatioCompl = 255 - hwRatio;



    for (int_fast16_t y = 0; y < iHeight; y++) {

        for (int_fast16_t x = 0; x < iWidth; x++){
            PICO_BLENDING(&Colour,pTargetBase,hwRatioCompl);
            pTargetBase++;
        }

        pTargetBase += iTargetStride - iWidth;
    }
}


__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_colour_filling_mask(
                            uint16_t *__RESTRICT pTarget,
                            int16_t iTargetStride,
                            uint8_t *__RESTRICT pchAlpha,
                            int16_t iAlphaStride,
                            arm_2d_size_t *__RESTRICT ptCopySize,
                            uint16_t Colour)
{
    int_fast16_t iHeight = ptCopySize->iHeight;
    int_fast16_t iWidth  = ptCopySize->iWidth;

    PICO_BLENDING_INIT;



    for (int_fast16_t y = 0; y < iHeight; y++) {

        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwAlpha = 255 - (*pchAlpha++);
#if !defined(__ARM_2D_CFG_UNSAFE_IGNORE_ALPHA_255_COMPENSATION__)
            hwAlpha -= (hwAlpha == 1);
#endif

            PICO_BLENDING(&Colour,pTarget,hwAlpha);
            pTarget++;
        }

        pchAlpha += (iAlphaStride - iWidth);
        pTarget += (iTargetStride - iWidth);
    }
}


__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_tile_copy_colour_keying_opacity(
                                                uint16_t * __RESTRICT pSourceBase,
                                                int16_t         iSourceStride,
                                                uint16_t * __RESTRICT pTargetBase,
                                                int16_t         iTargetStride,
                                                arm_2d_size_t * __RESTRICT ptCopySize,
                                                uint_fast16_t hwRatio,
                                                uint16_t   Colour)
{
    int_fast16_t    iHeight = ptCopySize->iHeight;
    int_fast16_t    iWidth = ptCopySize->iWidth;

    PICO_BLENDING_INIT;


#if !defined(__ARM_2D_CFG_UNSAFE_IGNORE_ALPHA_255_COMPENSATION__)
    hwRatio += (hwRatio == 255);
#endif

    uint16_t        hwRatioCompl = 255 - hwRatio;

    for (int_fast16_t y = 0; y < iHeight; y++) {

        for (int_fast16_t x = 0; x < iWidth; x++) {

            if (*pSourceBase != Colour) {
                PICO_BLENDING(pSourceBase,pTargetBase,hwRatioCompl);
            }
            pSourceBase++;
            pTargetBase++;
        }
        pSourceBase += (iSourceStride - iWidth);
        pTargetBase += (iTargetStride - iWidth);
    }
}

__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_colour_filling_mask_opacity(
                            uint16_t *__RESTRICT pTarget,
                            int16_t iTargetStride,
                            uint8_t *__RESTRICT pchAlpha,
                            int16_t iAlphaStride,
                            arm_2d_size_t *__RESTRICT ptCopySize,
                            uint16_t Colour,
                            uint_fast16_t hwOpacity)
{
    int_fast16_t iHeight = ptCopySize->iHeight;
    int_fast16_t iWidth  = ptCopySize->iWidth;

    PICO_BLENDING_INIT;

    for (int_fast16_t y = 0; y < iHeight; y++) {

        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwAlpha = 255 - ((*pchAlpha++) * hwOpacity >> 8);

#if !defined(__ARM_2D_CFG_UNSAFE_IGNORE_ALPHA_255_COMPENSATION__)
            hwAlpha -= (hwAlpha == 2) * 2;
#endif
            PICO_BLENDING(&Colour,pTarget,hwAlpha);
            pTarget++;

        }

        pchAlpha += (iAlphaStride - iWidth);
        pTarget += (iTargetStride - iWidth);
    }
}

__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_colour_filling_channel_mask_opacity(
                            uint16_t *__RESTRICT pTarget,
                            int16_t iTargetStride,
                            uint32_t *__RESTRICT pwAlpha,
                            int16_t iAlphaStride,
                            arm_2d_size_t *__RESTRICT ptCopySize,
                            uint16_t Colour,
                            uint_fast16_t hwOpacity)
{
    int_fast16_t iHeight = ptCopySize->iHeight;
    int_fast16_t iWidth  = ptCopySize->iWidth;

    PICO_BLENDING_INIT;

    for (int_fast16_t y = 0; y < iHeight; y++) {

        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwAlpha = 255 - (*(uint8_t *)(pwAlpha++) * hwOpacity >> 8);

#if !defined(__ARM_2D_CFG_UNSAFE_IGNORE_ALPHA_255_COMPENSATION__)
            hwAlpha -= (hwAlpha == 2) * 2;
#endif

            PICO_BLENDING(&Colour,pTarget,hwAlpha);

            pTarget++;
        }

        pwAlpha += (iAlphaStride - iWidth);
        pTarget += (iTargetStride - iWidth);
    }
}

__OVERRIDE_WEAK
void __arm_2d_impl_rgb565_colour_filling_channel_mask(
                            uint16_t *__RESTRICT pTarget,
                            int16_t iTargetStride,
                            uint32_t *__RESTRICT pwAlpha,
                            int16_t iAlphaStride,
                            arm_2d_size_t *__RESTRICT ptCopySize,
                            uint16_t Colour)
{
    int_fast16_t iHeight = ptCopySize->iHeight;
    int_fast16_t iWidth  = ptCopySize->iWidth;

    PICO_BLENDING_INIT;

    for (int_fast16_t y = 0; y < iHeight; y++) {

        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwAlpha = 256 - *(uint8_t *)(pwAlpha++);

#if !defined(__ARM_2D_CFG_UNSAFE_IGNORE_ALPHA_255_COMPENSATION__)
            hwAlpha -= (hwAlpha == 1);
#endif

            PICO_BLENDING(&Colour,pTarget,hwAlpha);
            pTarget++;
        }

        pwAlpha += (iAlphaStride - iWidth);
        pTarget += (iTargetStride - iWidth);
    }
}