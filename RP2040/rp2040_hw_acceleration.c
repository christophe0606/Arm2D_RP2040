#include "arm_2d.h"
#include "hardware/interp.h"

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

    interp_config cfg = interp_default_config();
    interp_config_set_blend(&cfg, true);
    interp_set_config(interp0, 0, &cfg);
    
    cfg = interp_default_config();
    interp_set_config(interp0, 1, &cfg);

    uint16_t hwRatioCompl = 255 - hwRatio;

    interp0->accum[1] = hwRatioCompl;


    for (int_fast16_t y = 0; y < iHeight; y++) {

        for (int_fast16_t x = 0; x < iWidth; x++){
            __arm_2d_color_fast_rgb_t tSrcPix, tTargetPix;
            __arm_2d_rgb565_unpack(Colour, &tSrcPix);
            __arm_2d_rgb565_unpack(*pTargetBase, &tTargetPix);
                
            for (int i = 0; i < 3; i++) 
            {    
                interp0->base[1] = tTargetPix.BGRA[i];
                interp0->base[0] = tSrcPix.BGRA[i];
                tTargetPix.BGRA[i] = interp0->peek[1] & 0x0FFFF;                               \
                   
            }                         
            //__API_PIXEL_BLENDING( &Colour,  pTargetBase++, hwRatioCompl);
            
            *pTargetBase++ = __arm_2d_rgb565_pack(&tTargetPix);
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

    interp_config cfg = interp_default_config();
    interp_config_set_blend(&cfg, true);
    interp_set_config(interp0, 0, &cfg);
    
    cfg = interp_default_config();
    interp_set_config(interp0, 1, &cfg);



    for (int_fast16_t y = 0; y < iHeight; y++) {

        for (int_fast16_t x = 0; x < iWidth; x++) {
            uint16_t hwAlpha = 255 - (*pchAlpha++);
#if !defined(__ARM_2D_CFG_UNSAFE_IGNORE_ALPHA_255_COMPENSATION__)
            hwAlpha -= (hwAlpha == 1);
#endif
            //__API_PIXEL_BLENDING(&Colour, pTarget++, hwAlpha);
            interp0->accum[1] = hwAlpha;

            __arm_2d_color_fast_rgb_t tSrcPix, tTargetPix;
            __arm_2d_rgb565_unpack(Colour, &tSrcPix);
            __arm_2d_rgb565_unpack(*pTarget, &tTargetPix);
                
            for (int i = 0; i < 3; i++) 
            {    
                interp0->base[1] = tTargetPix.BGRA[i];
                interp0->base[0] = tSrcPix.BGRA[i];
                tTargetPix.BGRA[i] = interp0->peek[1] & 0x0FFFF;                               \
                   
            }                         
            
            *pTarget++ = __arm_2d_rgb565_pack(&tTargetPix);

        }

        pchAlpha += (iAlphaStride - iWidth);
        pTarget += (iTargetStride - iWidth);
    }
}
