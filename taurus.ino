#include <TVout.h>
#include <fontALL.h>

// SIGNS
#include "include/sign-aries.h"
#include "include/sign-taurus.h"
#include "include/sign-gemini.h"
#include "include/sign-cancer.h"
#include "include/sign-leo.h"
#include "include/sign-virgo.h"
#include "include/sign-libra.h"
#include "include/sign-scorpio.h"
#include "include/sign-sagittarius.h"
#include "include/sign-capricorn.h"
#include "include/sign-aquarius.h"
#include "include/sign-pisces.h"

#include "include/words.h"

// PLANETS
#include "include/planet-mars.h"
#include "include/planet-jupiter.h"
#include "include/planet-mercury.h"
#include "include/planet-neptune.h"
#include "include/planet-saturn.h"
#include "include/planet-uranus.h"
#include "include/planet-venus.h"

// EYES
#include "include/eye-closed.h"
#include "include/eye-open.h"

#include "include/white-bear.h"

#define SIGN_COUNT 12
#define PLANET_COUNT 7
#define WIDTH 128
#define HEIGHT 96
#define HF_WIDTH (WIDTH / 2)
#define HF_HEIGHT (HEIGHT / 2)
#define IMG_SIZE 64
#define PI 3.14159265

// Coordinates to place bitmaps on the center of the screen
#define BMP_X ((WIDTH / 2) - (IMG_SIZE / 2))
#define BMP_Y ((HEIGHT / 2) - (IMG_SIZE / 2))

#define FONT_W 6
#define FONT_H 8

#define LED_PIN 13

enum Scene {
    SYMBOL_TAURUS = 0,
    SYMBOLS_FAST,
    PLANETS_SYMBOLS_FAST,
    WORDS_FAST,
    WORDS_FAST_INLINE,
    STARS,
    NOISE,
    SYMBOL_FADE,
    SYMBOL_CHECKERBOARD,
    SYMBOL_GLITCH_SIZE,
    SYMBOL_GLITCH_LOAD,
    CONSTELLATIONS,
    COMMANDS,
    WORD_CURSOR,
    WORMHOLE,
    SYMBOLS_SNAKE,
    SCROLL_STARS,
    EYES,
    CHANNEL,
    VHS,
    SYMBOL_SHIFT,
    BLINDS,
    FADE_IN,
    PARTY_WORDS,
    FILL_LINES,
    CIRCLES,
    L_SYSTEM,
    CLOCK,
    CHART,
    HEX_SCENE,
    PYRAMID,
    TOTAL
};

Scene currentScene = PARTY_WORDS; // Initial scene
Scene lastScene = -1;
TVout TV;
unsigned long startTime = 0;
bool sequentialMode = true;
unsigned int sceneCounter = 0;

unsigned char backBuffer[HEIGHT][WIDTH / 8] = { 0 };

unsigned char inline backBufferGet(int x, int y) {
    return (backBuffer[y][x / 8] >> (7 - (x % 8))) & 0x01;
}

void backBufferSet(int x, int y, unsigned char c) {
    if (c == 1) {
        backBuffer[y][x / 8] |= (0x01 << (7 - (x % 8)));
    } else {
        backBuffer[y][x / 8] &= ~(0x01 << (7 - (x % 8)));
    }
}

void screenToBackBuffer() {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            backBufferSet(i, j, TV.get_pixel(i, j));
        }
    }
}

void backBufferToScreen() {
    for (int i = 0; i < WIDTH; i++) {
        for (int j = 0; j < HEIGHT; j++) {
            TV.set_pixel(i, j, backBufferGet(i, j));
        }
    }
}

const unsigned char* getPlanetSymbol(int i) {
    switch (i) {
        case 0:
            return venus_planet;;
        case 1:
            return jupiter_planet;
        case 2:
            return mars_planet;
        case 3:
            return mercury_planet;
        case 4:
            return neptune_planet;
        case 5:
            return saturn_planet;
        case 6:
            return uranus_planet;
        default:
            return venus_planet;
    }
}

const unsigned char* getSignSymbol(int i) {
    switch (i) {
        case 0:
            return aries_sign;
        case 1:
            return taurus_sign;
        case 2:
            return gemini_sign;
        case 3:
            return cancer_sign;
        case 4:
            return leo_sign;
        case 5:
            return virgo_sign;
        case 6:
            return libra_sign;
        case 7:
            return scorpio_sign;
        case 8:
            return sagittarius_sign;
        case 9:
            return capricorn_sign;
        case 10:
            return aquarius_sign;
        case 11:
            return pisces_sign;
        default:
            return taurus_sign;
    }
}

const unsigned char* getRandomSymbol() {
    if (random(2) == 0) {
        return getSignSymbol(random(SIGN_COUNT));
    } else {
        return getPlanetSymbol(random(PLANET_COUNT));
    }
} 

void blinkForever() {
    while (true) {
        digitalWrite(LED_PIN, LOW);
        delay(500);
        digitalWrite(LED_PIN, HIGH);
        delay(500);
    }
}

void toCartesian(int x, int y, double degrees, int len, int *endX, int *endY) {
    int o = (int)(sin((degrees * PI) / 180.0) * len);
    int a = (int)(cos((degrees * PI) / 180.0) * len);

    *endX = x + a;
    *endY = y - o;  
}

void drawLineAngle(int x, int y, double degrees, int len, int color) {
    int endX, endY;
    toCartesian(x, y, degrees, len, &endX, &endY);
    TV.draw_line(x, y, endX, endY, color);
}

void setup() {
    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);

    char err = TV.begin(PAL, WIDTH, HEIGHT);
    if (err) {
        blinkForever();
    }
    
    TV.clear_screen();
    TV.select_font(font6x8);
    startTime = TV.millis();
    randomSeed(analogRead(0) | analogRead(1));
}

bool checkSceneEnded(long seconds) {
    if (TV.millis() - startTime > seconds * 1000) {
        sceneCounter++;
        if (sceneCounter == TOTAL) {
            sceneCounter = 0;
            sequentialMode = !sequentialMode;
        }

        Scene nextScene = currentScene;
        
        if (sequentialMode) {
            nextScene = nextScene + 1;
            if (nextScene == TOTAL) {
                nextScene = 0;
            }
        }
        else {
            while (nextScene == currentScene) {
                nextScene = (Scene)random(TOTAL);
            }
        }

        currentScene = nextScene;
        return true;
    }

    return false;
}

void testScene() {
    TV.draw_line(0, 0, WIDTH - 1, HEIGHT - 1, 1);
    TV.draw_line(0, HEIGHT - 1, WIDTH - 1, 0, 1);
    TV.draw_line(0, HEIGHT / 2, WIDTH - 1, HEIGHT / 2, 1);
    TV.draw_line(WIDTH / 2, 0, WIDTH / 2, HEIGHT - 1, 1);
    
    TV.draw_circle(WIDTH / 2, HEIGHT / 2, 5, 1, 1);
}

void tauroSymbolScene() {
    const char* s = getSignSymbol(1);
    TV.bitmap(BMP_X, BMP_Y, s);
    TV.delay(5000);
    checkSceneEnded(5);
}

int pyramidBase = HF_WIDTH;
int pyramidHeight = HEIGHT - 10;
void pyramidScene(bool changed) {
    TV.clear_screen();
    pyramidBase = (sin(TV.millis() / 100.0f) + 1) / 2.0f * HF_WIDTH;
    
    //base line
    TV.draw_line(HF_WIDTH + pyramidBase, pyramidHeight, HF_WIDTH - pyramidBase, pyramidHeight, 1);

    //top right line
    TV.draw_line(HF_WIDTH + pyramidBase, pyramidHeight, HF_WIDTH, 0, 1);

    //top left line
    TV.draw_line(HF_WIDTH - pyramidBase, pyramidHeight, HF_WIDTH, 0, 1);

    TV.delay(1);

    checkSceneEnded(15);
}

const char* hexDigits = "ABCDEF0123456789 ";
void hexScene(bool changed) {
    if (changed) {
        TV.clear_screen();
        TV.set_cursor(0, 0);
    }

    int digit = random(16 + 1);
    TV.print(hexDigits[digit]);
    TV.delay(1);
    
    if (checkSceneEnded(10)) {
        TV.print("\n\nMEMORY DUMP DONE");
        TV.delay(5000);
    }
}

void starsScene() { 
    int sign = random(SIGN_COUNT);

    TV.clear_screen();
    TV.set_cursor(0, HEIGHT / 2);
    TV.print(signs_words[sign + 12]);

    TV.delay(500);
    TV.clear_screen();
    const char* s = getSignSymbol(sign);
    TV.bitmap(BMP_X, BMP_Y, s);
    TV.delay(500);

    TV.clear_screen();
    int starCount = random(7, 10);
    for (int i = 0; i < starCount; i++) {
        int x = random(WIDTH);
        int y = random(HEIGHT);

        TV.set_pixel(x, y, 1);
    }
    TV.delay(500);

    checkSceneEnded(15);
}

int constellationI = 0;
void constellationsScene() {
    TV.clear_screen();
    int rayCount = random(5, 9);

    int x = random(WIDTH);
    int y = random(HEIGHT);
    int x2 = random(WIDTH);
    int y2 = random(HEIGHT);
    
    for (int i = 0; i < rayCount; i++) {
        // Main star
        TV.draw_line(x, y, x2, y2, 1);
        x2 = random(WIDTH);
        y2 = random(HEIGHT);
    }
        
    for (int i = 0; i < random(6, 14); i++) {
        // Background stars
        TV.set_pixel(random(WIDTH), random(HEIGHT), 1);
    }

    TV.set_cursor(0, HEIGHT - 8);
    TV.print(stars_words[constellationI % STARS_WORDS_COUNT]);
    constellationI++;
    
    TV.delay(700);

    checkSceneEnded(20);
}

int signLoad = -1;
void symbolGlitchLoadScene(bool changed) {
    if (changed) {
        signLoad = random(SIGN_COUNT);
        const char* s = getSignSymbol(signLoad);
        TV.bitmap(BMP_X, BMP_Y, s);
        TV.delay(3000);
        return;
    }

    const char* s = getSignSymbol(signLoad);
    TV.bitmap(BMP_X, BMP_Y, s, 1, IMG_SIZE - random(1, 10), IMG_SIZE - random(1, 10));
    TV.set_cursor(0, HEIGHT - 8);
    TV.print(signs_words[12 + signLoad]);
    TV.print("?");
    TV.delay(random(100, 250));
    
    TV.clear_screen();
    TV.bitmap(BMP_X, BMP_Y, s);
    TV.delay(1000);

    checkSceneEnded(20);
}

void symbolFadeScene(bool changed) {
    if (changed) {
        int sign = random(SIGN_COUNT);
        const char* s = getSignSymbol(sign);
        TV.bitmap(BMP_X, BMP_Y, s);
        screenToBackBuffer();
        TV.clear_screen();
    }

    int x = random(WIDTH);
    int y = random(HEIGHT);
    unsigned char c = backBufferGet(x, y);
    TV.set_pixel(x, y, !c);

    checkSceneEnded(20);
}

int cmdIdx = 0;
void commandsScene() {
    if (cmdIdx < CMDS_WORDS_COUNT) {
        TV.println(cmds_words[cmdIdx % CMDS_WORDS_COUNT]);
        cmdIdx++;
        TV.delay(900);
    }

    if (checkSceneEnded(15)) {
        cmdIdx = 0;
    }
}

int glitchSign = -1;
void symbolCheckerboardScene(bool changed) {
    if (changed) {
        glitchSign = random(SIGN_COUNT);
        const char* s = getSignSymbol(glitchSign);
        TV.bitmap(BMP_X, BMP_Y, s);

        for (int x = 0; x < WIDTH; x++) {
            for (int y = 0; y < HEIGHT; y++) {
                if ((x < WIDTH / 2 && y < HEIGHT / 2) || (x >= WIDTH / 2 && y >= HEIGHT / 2)) {
                    unsigned char c = TV.get_pixel(x, y);
                    TV.set_pixel(x, y, !c);
                }
            }
        }
    }

    for (int x = 0; x < WIDTH; x++) {
        for (int y = 0; y < HEIGHT; y++) {
            unsigned char c = TV.get_pixel(x, y);
            TV.set_pixel(x, y, !c);
        }
    }

    TV.delay(50);

    checkSceneEnded(10);
}

void symbolGlitchSizeScene() {
    TV.clear_screen();
    const char* s = getSignSymbol(random(SIGN_COUNT));
    TV.bitmap(BMP_X, BMP_Y, s, 0, IMG_SIZE - random(0, 10), IMG_SIZE - random(0, 10));
    TV.delay(random(20, 250));

    if (checkSceneEnded(20)) {
        int mid = HEIGHT / 2;
        const char* word = errors_words[random(ERRORS_WORDS_COUNT)];
        int xOffset = (strlen(word) * FONT_W) / 2;
        int yOffset = FONT_H / 2;

        for (int i = 0; i < 50; i++) { //flashes
            TV.draw_rect(0, mid - FONT_H, WIDTH, FONT_H * 2, 0, 0);
            TV.set_cursor((WIDTH / 2) - xOffset, (HEIGHT / 2) - yOffset);
            if (i % 2 == 0) {
                TV.print(word);
            }

            TV.delay(200);
        }
    }
}

void noiseScene() {
    for (int i = 0; i < 30; i++) {
        TV.set_pixel(random(WIDTH), random(HEIGHT), random(3));
    }

    if (checkSceneEnded(8)) {
        TV.delay(1000);
    }
}

void fastWordsScene() {
    if (random(4) == 0) {
        TV.print("_");
    }
    TV.println(signs_words[random(SIGNS_WORDS_COUNT)]);
    TV.delay(150);
    checkSceneEnded(10);
}

void wordsFastInlineScene() {
    TV.clear_screen();
    TV.set_cursor(0, (HEIGHT / 2) - (FONT_H / 2));
    TV.print(signs_words[SIGN_COUNT + random(SIGN_COUNT)]);
    TV.delay(120);

    checkSceneEnded(10);
}

int fssI = 0;
void fastSymbolsScene() {
    const char* s = getSignSymbol(fssI % SIGN_COUNT);
    TV.bitmap(BMP_X, BMP_Y, s);
    TV.delay(150);
    fssI++;

    checkSceneEnded(20);
}

void planetSymbolsFastScene() {
    const char* s = getPlanetSymbol(fssI % PLANET_COUNT);
    TV.bitmap(BMP_X, BMP_Y, s);
    TV.delay(150);
    fssI++;

    checkSceneEnded(20);
}

char *cursorWord = NULL;
int cursorIdx = 0;
void wordCursorScene(bool changed) {
    if (changed) {
        cursorWord = signs_words[SIGN_COUNT + random(SIGN_COUNT)];
        cursorIdx = 0;
    }

    if (cursorWord[cursorIdx]) {
        char randChar = random(65, 91);
        TV.set_cursor(cursorIdx * FONT_W, (HEIGHT / 2) - (FONT_H / 2));

        TV.print(randChar);
        
        if (cursorWord[cursorIdx] == randChar) {
            cursorIdx++;
        }

        TV.delay(50);

        if (!cursorWord[cursorIdx]) {
            TV.print(" LOADED.");
        }
    }

    checkSceneEnded(15);
}

void wormholeScene(bool changed) {
    TV.clear_screen();
    int circleCount = random(3, 7);
    for (int i = 0; i < circleCount; i++) {
        TV.draw_circle(WIDTH / 2, HEIGHT / 2, random(HEIGHT / 2), WHITE, BLACK);
    }

    TV.delay(120);

    checkSceneEnded(8);
}

enum Orientation {
    Tau_NE, Tau_SE, Tau_SW, Tau_NW, TOTAL_ORIENTATION
};

unsigned char *lastSnakeBmp = NULL;
Orientation orientation = Tau_NE;
int snakeX = WIDTH / 2;
int snakeY = HEIGHT / 2;
unsigned long lastReset = 0;
void symbolsSnakeScene(bool changed) {
    if (changed) {
        lastSnakeBmp = getRandomSymbol();

        TV.bitmap(BMP_X, BMP_Y, lastSnakeBmp);
        
        lastReset = TV.millis();
        snakeX = WIDTH / 2;
        snakeY = HEIGHT / 2;
    }

    if (TV.millis() - lastReset > 2000) {
        TV.clear_screen();
        TV.bitmap(BMP_X, BMP_Y, lastSnakeBmp);
        lastReset = TV.millis();
        snakeX = WIDTH / 2;
        snakeY = HEIGHT / 2;
        orientation = random((int)TOTAL_ORIENTATION);
    }

    unsigned char currentColor = TV.get_pixel(snakeX, snakeY);
    TV.set_pixel(snakeX, snakeY, !currentColor);

    switch (orientation) {
        case Tau_NE:
            snakeY--;
            snakeX++;
        break;
        case Tau_SE:
            snakeY++;
            snakeX++;
        break;
        case Tau_SW:
            snakeX--;
            snakeY++;
        break;
        case Tau_NW:
            snakeX--;
            snakeY--;
        break;
    }

    if (snakeX < 0) {
        snakeX = WIDTH;
    } else if (snakeX > WIDTH) {
        snakeX = 0;
    }

    if (snakeY < 0) {
        snakeY = HEIGHT;
    } else if (snakeY > HEIGHT) {
        snakeY = 0;
    }

    unsigned char newColor = TV.get_pixel(snakeX, snakeY);

    if (newColor != currentColor) {
        orientation = orientation + 1;
        orientation = orientation % TOTAL_ORIENTATION;
    }

    checkSceneEnded(20);
}

void scrollStarsScene(bool changed) {
    if (changed) {
        // Draw the earth
        TV.draw_circle(WIDTH / 2, HEIGHT / 2, HEIGHT / 2, WHITE, WHITE);
    }

    TV.shift(1, 1);

    // Place a new star on top line
    if (random(7) == 0) {
        TV.set_pixel(random(WIDTH), 0, 1);
    }

    TV.delay(100);

    if (checkSceneEnded(12)) {
        TV.delay(1000);
    }
}

void eyesScene(bool changed) {
    TV.bitmap(BMP_X, BMP_Y, open_eye);

    int irisRadius = 6;
    int irisOffset = -irisRadius + random(irisRadius * 2);
    TV.draw_circle((WIDTH / 2) + irisOffset, HEIGHT / 2, irisRadius, 1, 1);

    TV.delay(1200);

    TV.bitmap(BMP_X, BMP_Y, closed_eye);

    TV.delay(random(200, 600));

    checkSceneEnded(20);
}

void channelScene(bool changed) {
    if (changed) {
        char *s = "VIDEO 1";
        TV.set_cursor(WIDTH - (strlen(s) * FONT_W) - 2, FONT_H + 2);
        TV.print(s);
    }

    TV.delay(1);

    checkSceneEnded(8);
}

int vhsTime = 0;
void vhsScene(bool changed) {
    char *stop = "STOP";
    char *play = "PLAY";
    const int timerWidth = 5;
    char *timer = "--:--";

    if (changed) {
        vhsTime = 0;
        TV.set_cursor(2, FONT_H + 2);
        TV.print(stop);

        TV.set_cursor(WIDTH - (timerWidth * FONT_W) - 2, FONT_H + 2);
        TV.print(timer);

        TV.delay(3500);
        TV.clear_screen();

        TV.set_cursor(2, FONT_H + 2);
        TV.print(play);

        return;
    }

    TV.set_cursor(WIDTH - (timerWidth * FONT_W) - 2, FONT_H + 2);
    TV.print("00:0");
    TV.print(vhsTime % 10);
    vhsTime++;

    TV.delay(1000);

    checkSceneEnded(9);
}

void shiftLine(int orientation, int direction, int index, int step) {
    if (orientation == 0) {
        // Shift column
        if (direction == 1) {
            for (int i = HEIGHT - 2; i >= 0; i--) {
                unsigned char pix = TV.get_pixel(index, i);
                TV.set_pixel(index, i + step, pix);
            }
        } else {
            for (int i = 1; i <= HEIGHT - 1; i++) {
                unsigned char pix = TV.get_pixel(index, i);
                TV.set_pixel(index, i - step, pix);
            }   
        }
    } else {
        // Shift row
        if (direction == 1) {
            for (int i = WIDTH - 2; i >= 0; i--) {
                unsigned char pix = TV.get_pixel(i, index);
                TV.set_pixel(i + step, index, pix);
            }
        } else {
            for (int i = 1; i <= WIDTH - 1; i++) {
                unsigned char pix = TV.get_pixel(i, index);
                TV.set_pixel(i - step, index, pix);
            }   
        }       
    }
}

#define STEP_COUNT 300
unsigned char steps[STEP_COUNT][4] = { 0 };
bool rewinding = false;
int stepIdx = 0;
unsigned char *shiftSymbol = NULL;
void symbolShiftScene(bool changed) {
    if (changed) {
        rewinding = false;
        stepIdx = 0;

        shiftSymbol = getRandomSymbol();

        TV.bitmap(BMP_X, BMP_Y, shiftSymbol);
        TV.delay(1000);
    }

    int direction, step, col, row, orientation;

    if (!rewinding) {
        direction = random(2);
        step = random(1, 6);
        col = random(WIDTH);
        row = random(HEIGHT);
        orientation = random(2);

        steps[stepIdx][0] = orientation;
        steps[stepIdx][1] = direction;
        steps[stepIdx][2] = (orientation == 0) ? col : row;
        steps[stepIdx][3] = step;

        stepIdx++;
        if (stepIdx == STEP_COUNT) {
            rewinding = true;
        }
    } else if (stepIdx >= 0) {
        orientation = steps[stepIdx][0];
        direction = (steps[stepIdx][1] == 0) ? 1 : 0;
        col = row = steps[stepIdx][2];
        step = steps[stepIdx][3];       

        stepIdx--;
    } else {
        TV.bitmap(BMP_X, BMP_Y, shiftSymbol);
        rewinding = false;
        stepIdx = 0;
        TV.delay(200);
    }

    shiftLine(orientation, direction, (orientation == 0) ? col : row, step);

    if (checkSceneEnded(30)) {
        TV.delay(1500);
    }
}


void blindsScene(bool changed) {
    if (changed) {
        TV.bitmap(BMP_X, BMP_Y, getRandomSymbol());
        screenToBackBuffer();
        TV.clear_screen();
    }

    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH / 8; j++) {
            backBuffer[i][j] += 1;
        }
    }

    backBufferToScreen();

    checkSceneEnded(20);
}

void fadeInScene(bool changed) {
    if (changed) {
        TV.bitmap(BMP_X, BMP_Y, getRandomSymbol());
        screenToBackBuffer();
        TV.clear_screen();
    }

    int x = random(WIDTH);
    int y = random(HEIGHT);

    if (backBufferGet(x, y)) {
        TV.set_pixel(x, y, 1);
    }

    if (checkSceneEnded(20)) {
        TV.delay(1000);
    }
}

int partyIdx = 0;
void partyWordsScene() {
    TV.clear_screen();
    TV.set_cursor(0, (HEIGHT / 2) - (FONT_H / 2));
    TV.print(event_words[partyIdx % EVENT_WORDS_COUNT]);
    TV.delay(200);

    partyIdx++;

    if (checkSceneEnded(8)) {
        TV.set_cursor(0, (HEIGHT / 2) - (FONT_H / 2));
        TV.print("ORGULLO TAURINO III");
        TV.delay(4000);
    }
}

int fillLinesDirection = 0;
void fillLinesScene(bool changed) {
    if (changed) {
        TV.bitmap(BMP_X, BMP_Y, getRandomSymbol());
        screenToBackBuffer();
        TV.clear_screen();
        fillLinesDirection = random(4); 
    }

    int x = random(WIDTH);
    int y = random(HEIGHT);

    while (!backBufferGet(x, y)) {
        TV.set_pixel(x, y, 1);

        switch (fillLinesDirection) {
            case UP:
                y--;
                break;
            case DOWN:
                y++;
                break;
            case RIGHT:
                x++;
                break;
            case LEFT:
                x--;
                break;
        }

        if (x < 0 || x > WIDTH - 1 || y < 0 || y > HEIGHT - 1) {
            break;
        }
    }

    TV.delay_frame(1);
    
    checkSceneEnded(25);
}

unsigned int circleCounter = 0;
void circlesScene(bool changed) {
    if (changed) {

    }

    for (int i = 0; i < HEIGHT / 2; i++) {
        TV.draw_circle(WIDTH / 2, HEIGHT / 2, i, (i + circleCounter) % 2);
        TV.delay_frame(1);
    }

    circleCounter++;
    
    checkSceneEnded(15);
}

int startLen = 40;
int branches = 3;
double branchAngle = 30.0;
double angleOffset = 0.0;
int rouletteDenominator = 1;
int treeXOffset = 0;

void lSystemRecursive(int x, int y, double angle, int len) {
    if (len < 1) {
        return;
    }

    drawLineAngle(x, y, angle, len, 1);
    int endX, endY;
    toCartesian(x, y, angle, len, &endX, &endY);

    double startAngle = angle + ((branchAngle * (branches - 1)) / 2.0) + angleOffset;
    for (int i = 0; i < branches; i++) {
        double a = startAngle - (i * branchAngle);
        if (random(rouletteDenominator) != 0) {
            lSystemRecursive(endX, endY, a, len / 2);
        }
    }
}

void drawLSystem() {
    startLen = random(30, 50);
    branches = random(3, 5);
    branchAngle = random(25, 40);
    angleOffset = -10 + random(20);
    rouletteDenominator = random(3, 5);
    treeXOffset = -20 + random(40);
    lSystemRecursive(treeXOffset + (WIDTH / 2), HEIGHT - 1, 90.0 + angleOffset, startLen);
} 

void lSystemScene() {
    drawLSystem();
    TV.delay(200);
    TV.clear_screen();

    checkSceneEnded(20);
}

double currentAngle, hourAngle, minuteAngle;
double angleDiff = 360.0 / 60.0;
const int maxRadius = (HEIGHT / 2) - 8;
const int secondsLen = maxRadius - 4;
const int minutesLen = maxRadius - 13;
const int hoursLen = maxRadius - 22;
void clockScene(bool changed) {
    if (changed) {
        TV.draw_circle(WIDTH / 2, HEIGHT / 2, maxRadius, 1, 0);

        currentAngle = random(60) * angleDiff;
        hourAngle = random(60) * angleDiff;
        minuteAngle = random(60) * angleDiff;
    }

    drawLineAngle(WIDTH / 2, HEIGHT / 2, currentAngle, secondsLen, 0);
    drawLineAngle(WIDTH / 2, HEIGHT / 2, minuteAngle, minutesLen, 0);

    currentAngle -= angleDiff;
    minuteAngle -= angleDiff / 60.0;
    // Dont bother updating hours

    drawLineAngle(WIDTH / 2, HEIGHT / 2, currentAngle, secondsLen, 1);  
    drawLineAngle(WIDTH / 2, HEIGHT / 2, hourAngle, hoursLen, 1);
    drawLineAngle(WIDTH / 2, HEIGHT / 2, minuteAngle, minutesLen, 1);

    checkSceneEnded(15);
}

double chartRayLen = 0;
void chartScene(bool changed) {
    const int chartRadius = (HEIGHT / 2) - 6;
    if (changed) {
        chartRayLen = 0;
        TV.draw_circle(WIDTH / 2, HEIGHT / 2, chartRadius, 1, 0);
        TV.draw_circle(WIDTH / 2, HEIGHT / 2, chartRadius - 7, 1, 0);
        TV.draw_circle(WIDTH / 2, HEIGHT / 2, 12, 1, 0);

        for (int i = 0; i < 12; i++) {
            drawLineAngle(WIDTH / 2, HEIGHT / 2, i * (360.0 / 12), chartRadius, 1);
        }

        TV.delay(2000);
    }

    int rays = random(2, 7);
    double offset = random(180);
    for (int i = 0; i < rays; i++) {
        drawLineAngle(WIDTH / 2, HEIGHT / 2, (i * (360.0 / rays)) + offset, chartRayLen + (chartRadius / 2), 1);
    }

    TV.delay(50);
    chartRayLen += 0.2;

    checkSceneEnded(20);
}

void loop() {
    bool changed = false;
    if (currentScene != lastScene) {
        TV.clear_screen();
        TV.set_cursor(0, 0);

        digitalWrite(LED_PIN, HIGH);
        TV.delay(100);
        digitalWrite(LED_PIN, LOW);

        startTime = TV.millis();
        changed = true;
    }

    lastScene = currentScene;

    switch (currentScene) {
        case SYMBOL_TAURUS:
            tauroSymbolScene();
            break;
        case SYMBOLS_FAST:
            fastSymbolsScene();
            break;
        case PLANETS_SYMBOLS_FAST:
            planetSymbolsFastScene();
            break;
        case WORDS_FAST:
            fastWordsScene();
            break;
        case WORDS_FAST_INLINE:
            wordsFastInlineScene();
            break;
        case STARS:
            starsScene();
            break;
        case NOISE:
            noiseScene();
            break;
        case SYMBOL_FADE:
            symbolFadeScene(changed);
            break;
        case SYMBOL_CHECKERBOARD:
            symbolCheckerboardScene(changed);
            break;
        case SYMBOL_GLITCH_SIZE:
            symbolGlitchSizeScene();
            break;
        case CONSTELLATIONS:
            constellationsScene();
            break;
        case COMMANDS:
            commandsScene();
            break;
        case SYMBOL_GLITCH_LOAD:
            symbolGlitchLoadScene(changed);
            break;
        case WORD_CURSOR:
            wordCursorScene(changed);
            break;
        case WORMHOLE:
            wormholeScene(changed);
            break;
        case SYMBOLS_SNAKE:
            symbolsSnakeScene(changed);
            break;
        case SCROLL_STARS:
            scrollStarsScene(changed);
            break;
        case EYES:
            eyesScene(changed);
            break;
        case CHANNEL:
            channelScene(changed);
            break;
        case VHS:
            vhsScene(changed);
            break;
        case SYMBOL_SHIFT:
            symbolShiftScene(changed);
            break;
        case BLINDS:
            blindsScene(changed);
            break;
        case FADE_IN:
            fadeInScene(changed);
            break;
        case PARTY_WORDS:
            partyWordsScene();
            break;
        case FILL_LINES:
            fillLinesScene(changed);
            break;
        case CIRCLES:
            circlesScene(changed);
            break;
        case L_SYSTEM:
            lSystemScene();
            break;
        case CLOCK:
            clockScene(changed);
            break;
        case CHART:
            chartScene(changed);
            break;
        case HEX_SCENE:
            hexScene(changed);
            break;
        case PYRAMID:
            pyramidScene(changed);
            break;
        default:
            blinkForever();
            break;
    }
}
