#include <Arduino.h>
#include <rmt_led_strip.hpp>
#include <gfx.hpp>
#include <Bm437_Acer_VGA_8x8.h>
#include <neopixel_panel.hpp>
using rgbw32 = gfx::rgbw_pixel<32>;
using hsl24 = gfx::hsl_pixel<24>;
using colorw32 = gfx::color<rgbw32>;

//#define COLOR_TEXT_ONLY
#define LED_PIN 18
#define LED_HRES 8
#define LED_VRES 32
#define LED_SWAP_XY 1
#define LED_SNAKE_LAYOUT 1
#if LED_SWAP_XY
#define LED_WIDTH LED_VRES
#define LED_HEIGHT LED_HRES
#else
#define LED_WIDTH LED_HRES
#define LED_HEIGHT LED_VRES
#endif
#define LED_COUNT (LED_WIDTH*LED_HEIGHT)
#define V_WIDTH LED_WIDTH
#define V_HEIGHT LED_HEIGHT
#define BUF_WIDTH V_WIDTH
#define BUF_HEIGHT (V_HEIGHT+4)
#define PALETTE_SIZE (256 * 3)
#define INT_SIZE 2
// VGA color palette for flames
unsigned char pal[PALETTE_SIZE] = {0, 0, 0, 0, 0, 24, 0, 0, 24, 0, 0, 28,
                                   0, 0, 32, 0, 0, 32, 0, 0, 36, 0, 0, 40,
                                   8, 0, 40, 16, 0, 36, 24, 0, 36, 32, 0, 32,
                                   40, 0, 28, 48, 0, 28, 56, 0, 24, 64, 0, 20,
                                   72, 0, 20, 80, 0, 16, 88, 0, 16, 96, 0, 12,
                                   104, 0, 8, 112, 0, 8, 120, 0, 4, 128, 0, 0,
                                   128, 0, 0, 132, 0, 0, 136, 0, 0, 140, 0, 0,
                                   144, 0, 0, 144, 0, 0, 148, 0, 0, 152, 0, 0,
                                   156, 0, 0, 160, 0, 0, 160, 0, 0, 164, 0, 0,
                                   168, 0, 0, 172, 0, 0, 176, 0, 0, 180, 0, 0,
                                   184, 4, 0, 188, 4, 0, 192, 8, 0, 196, 8, 0,
                                   200, 12, 0, 204, 12, 0, 208, 16, 0, 212, 16, 0,
                                   216, 20, 0, 220, 20, 0, 224, 24, 0, 228, 24, 0,
                                   232, 28, 0, 236, 28, 0, 240, 32, 0, 244, 32, 0,
                                   252, 36, 0, 252, 36, 0, 252, 40, 0, 252, 40, 0,
                                   252, 44, 0, 252, 44, 0, 252, 48, 0, 252, 48, 0,
                                   252, 52, 0, 252, 52, 0, 252, 56, 0, 252, 56, 0,
                                   252, 60, 0, 252, 60, 0, 252, 64, 0, 252, 64, 0,
                                   252, 68, 0, 252, 68, 0, 252, 72, 0, 252, 72, 0,
                                   252, 76, 0, 252, 76, 0, 252, 80, 0, 252, 80, 0,
                                   252, 84, 0, 252, 84, 0, 252, 88, 0, 252, 88, 0,
                                   252, 92, 0, 252, 96, 0, 252, 96, 0, 252, 100, 0,
                                   252, 100, 0, 252, 104, 0, 252, 104, 0, 252, 108, 0,
                                   252, 108, 0, 252, 112, 0, 252, 112, 0, 252, 116, 0,
                                   252, 116, 0, 252, 120, 0, 252, 120, 0, 252, 124, 0,
                                   252, 124, 0, 252, 128, 0, 252, 128, 0, 252, 132, 0,
                                   252, 132, 0, 252, 136, 0, 252, 136, 0, 252, 140, 0,
                                   252, 140, 0, 252, 144, 0, 252, 144, 0, 252, 148, 0,
                                   252, 152, 0, 252, 152, 0, 252, 156, 0, 252, 156, 0,
                                   252, 160, 0, 252, 160, 0, 252, 164, 0, 252, 164, 0,
                                   252, 168, 0, 252, 168, 0, 252, 172, 0, 252, 172, 0,
                                   252, 176, 0, 252, 176, 0, 252, 180, 0, 252, 180, 0,
                                   252, 184, 0, 252, 184, 0, 252, 188, 0, 252, 188, 0,
                                   252, 192, 0, 252, 192, 0, 252, 196, 0, 252, 196, 0,
                                   252, 200, 0, 252, 200, 0, 252, 204, 0, 252, 208, 0,
                                   252, 208, 0, 252, 208, 0, 252, 208, 0, 252, 208, 0,
                                   252, 212, 0, 252, 212, 0, 252, 212, 0, 252, 212, 0,
                                   252, 216, 0, 252, 216, 0, 252, 216, 0, 252, 216, 0,
                                   252, 216, 0, 252, 220, 0, 252, 220, 0, 252, 220, 0,
                                   252, 220, 0, 252, 224, 0, 252, 224, 0, 252, 224, 0,
                                   252, 224, 0, 252, 228, 0, 252, 228, 0, 252, 228, 0,
                                   252, 228, 0, 252, 228, 0, 252, 232, 0, 252, 232, 0,
                                   252, 232, 0, 252, 232, 0, 252, 236, 0, 252, 236, 0,
                                   252, 236, 0, 252, 236, 0, 252, 240, 0, 252, 240, 0,
                                   252, 240, 0, 252, 240, 0, 252, 240, 0, 252, 244, 0,
                                   252, 244, 0, 252, 244, 0, 252, 244, 0, 252, 248, 0,
                                   252, 248, 0, 252, 248, 0, 252, 248, 0, 252, 252, 0,
                                   252, 252, 4, 252, 252, 8, 252, 252, 12, 252, 252, 16,
                                   252, 252, 20, 252, 252, 24, 252, 252, 28, 252, 252, 32,
                                   252, 252, 36, 252, 252, 40, 252, 252, 40, 252, 252, 44,
                                   252, 252, 48, 252, 252, 52, 252, 252, 56, 252, 252, 60,
                                   252, 252, 64, 252, 252, 68, 252, 252, 72, 252, 252, 76,
                                   252, 252, 80, 252, 252, 84, 252, 252, 84, 252, 252, 88,
                                   252, 252, 92, 252, 252, 96, 252, 252, 100, 252, 252, 104,
                                   252, 252, 108, 252, 252, 112, 252, 252, 116, 252, 252, 120,
                                   252, 252, 124, 252, 252, 124, 252, 252, 128, 252, 252, 132,
                                   252, 252, 136, 252, 252, 140, 252, 252, 144, 252, 252, 148,
                                   252, 252, 152, 252, 252, 156, 252, 252, 160, 252, 252, 164,
                                   252, 252, 168, 252, 252, 168, 252, 252, 172, 252, 252, 176,
                                   252, 252, 180, 252, 252, 184, 252, 252, 188, 252, 252, 192,
                                   252, 252, 196, 252, 252, 200, 252, 252, 204, 252, 252, 208,
                                   252, 252, 208, 252, 252, 212, 252, 252, 216, 252, 252, 220,
                                   252, 252, 224, 252, 252, 228, 252, 252, 232, 252, 252, 236,
                                   252, 252, 240, 252, 252, 244, 252, 252, 248, 252, 252, 252};
uint8_t p1[BUF_HEIGHT][BUF_WIDTH];   // VGA buffer, quarter resolution w/extra lines
using namespace gfx;
using namespace arduino;
ws2812 leds(LED_PIN,LED_COUNT);


neopixel_panel panel(leds,LED_HRES,LED_SWAP_XY,LED_SNAKE_LAYOUT);
using fb_type = bitmap<rgbw32>;
// repeat the text so it scrolls
text_info text_data("honey the codewitch honey the codewitch",Bm437_Acer_VGA_8x8_FON,false);

void setup() {
    Serial.begin(115200);
    if(!panel.initialize()) {
        Serial.println("Error initializing panel");
    } else {
        Serial.println("Panel initialized");
    }
     // Initialize the buffer to 0s
    for (int i = 0; i < BUF_HEIGHT; i++) {
        for (int j = 0; j < BUF_WIDTH; j++) {
            p1[i][j] = 0;
        }
    }
}
void do_text() {
    text_data.transparent_background=false;
    static int i = 0;
    static float h = 0;
    static float hd = .01;
    srect16 sr = (srect16)panel.bounds();
    sr.x2=32767;
    hsl24 hsl(true,h,.66,.125);
    rgbw32 rgbw,rgbw2;
    convert(hsl,&rgbw);
    hsl.channelr<0>(fmodf(h+.5,1));
    hsl.channelr<1>(.33f);
    hsl.channelr<2>(.06f);
    convert(hsl,&rgbw2);
    draw::text(panel,sr.offset(i,0),text_data,rgbw,rgbw2);
    --i;
    if(i<strlen(text_data.text)*-4) {
        i=0;
    }
    if(hd>0 && h+hd>1) {
        hd=-hd;
    } else if(hd<0 && h+hd<0) {
        hd = -hd;
    }
    h+=hd;
}
void do_black_text() {
    text_data.transparent_background=true;
    static int i = 0;
    srect16 sr = (srect16)panel.bounds();
    sr.x2=32767;
    draw::text(panel,sr.offset(i,0),text_data,decltype(panel)::pixel_type());
    --i;
    if(i<strlen(text_data.text)*-4) {
        i=0;
    }
    
}

void do_fire() {
    static unsigned int i, j, k, l, delta;  // looping variables, counters, and data
    // Transform current buffer
    for (i = 1; i < BUF_HEIGHT; ++i) {
        for (j = 0; j < BUF_WIDTH; ++j) {
            if (j == 0)
                p1[i - 1][j] = (p1[i][j] +
                                p1[i - 1][79] +
                                p1[i][j + 1] +
                                p1[i + 1][j]) >>
                                2;
            else if (j == 79)
                p1[i - 1][j] = (p1[i][j] +
                                p1[i][j - 1] +
                                p1[i + 1][0] +
                                p1[i + 1][j]) >> 2
                                ;
            else
                p1[i - 1][j] = (p1[i][j] +
                                p1[i][j - 1] +
                                p1[i][j + 1] +
                                p1[i + 1][j]) >>
                                2;

            if (p1[i][j] > 11)
                p1[i][j] = p1[i][j] - 12;
            else if (p1[i][j] > 3)
                p1[i][j] = p1[i][j] - 4;
            else {
                if (p1[i][j] > 0) p1[i][j]--;
                if (p1[i][j] > 0) p1[i][j]--;
                if (p1[i][j] > 0) p1[i][j]--;
            }
        }
    }
    delta = 0;
    for (j = 0; j < BUF_WIDTH; j++) {
        if (rand()%10 < 5) {
            delta = (rand()&1) * 255;
        }
        p1[BUF_HEIGHT - 2][j] = delta;
        p1[BUF_HEIGHT - 1][j] = delta;
    }
    rgbw_pixel<32> col;
    
    for(int y = 0;y<LED_HEIGHT;++y) {
        for(int x = 0;x<LED_WIDTH;++x) {
            int i = x/4;
            int j = y/4;
            uint8_t* pc = &pal[(p1[y][x]*3)];
            uint8_t r = *pc;
            ++pc;
            uint8_t g = *pc;
            ++pc;
            uint8_t b = *pc;
            col = rgbw_pixel<32>(r,g,b,0);
            point16 pt(x,y);
            panel.point(pt,col);
        }
    }
}

void loop() {

    panel.suspend();
#ifdef COLOR_TEXT_ONLY
    do_text();
#else
    do_fire();
    do_black_text();
    // dim it
    draw::filled_rectangle(panel,panel.bounds(),rgba_pixel<32>(true,0,0,0,.50));
#endif
    panel.resume();
    delay(50);
}
