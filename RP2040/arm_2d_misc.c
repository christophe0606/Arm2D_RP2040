#include "arm_2d.h"
#include "pico/time.h"
#include "st7789_lcd.h"

__OVERRIDE_WEAK 
int64_t arm_2d_helper_get_system_timestamp(void)
{
    absolute_time_t t = get_absolute_time();
    uint32_t ms = to_ms_since_boot(t);
    return(ms);
}

__WEAK 
uint32_t arm_2d_helper_get_reference_clock_frequency(void)
{
    return 1000.0;
}

__OVERRIDE_WEAK 
void Disp0_DrawBitmap(  int16_t x, 
                        int16_t y, 
                        int16_t width, 
                        int16_t height, 
                        const uint8_t *bitmap)
{

    refresh_lcd(x, y, width, height, bitmap);

}