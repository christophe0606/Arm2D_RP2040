#ifndef _ST7789_LCD_H_
#define _ST7789_LCD_H_

#ifndef PIN_DIN
#define PIN_DIN 11
#endif 

#ifndef PIN_CLK
#define PIN_CLK 10
#endif 

#ifndef PIN_CS
#define PIN_CS 9
#endif 

#ifndef PIN_DC
#define PIN_DC 8
#endif 

#ifndef PIN_RESET
#define PIN_RESET 12
#endif 

#ifndef PIN_BL
#define PIN_BL 13
#endif 

extern void refresh_lcd(int16_t x, 
                        int16_t y, 
                        int16_t width, 
                        int16_t height, 
                        const uint8_t *bitmap);
extern void start_lcd();

#endif