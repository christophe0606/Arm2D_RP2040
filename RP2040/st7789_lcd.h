#ifndef _ST7789_LCD_H_
#define _ST7789_LCD_H_


extern void refresh_lcd(int16_t x, 
                        int16_t y, 
                        int16_t width, 
                        int16_t height, 
                        const uint8_t *bitmap);
extern void start_lcd();

#endif