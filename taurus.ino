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

#define USE_SECUENTIAL 1

#define SIGN_COUNT 12
#define PLANET_COUNT 7
#define WIDTH 120
#define HEIGHT 96
#define IMG_SIZE 64

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
		TOTAL
};

// TODO:
// Better scene random order

Scene currentScene = EYES;
Scene lastScene = -1;
TVout TV;
unsigned long startTime = 0;

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

void setup() {
		TV.begin(PAL, WIDTH, HEIGHT);
		TV.clear_screen();
  		//TV.select_font(font4x6);
  		TV.select_font(font6x8);
  		//TV.select_font(font8x8);
		startTime = TV.millis();
		randomSeed(analogRead(0));

		pinMode(LED_PIN, OUTPUT);
		digitalWrite(LED_PIN, LOW);
}

bool checkSceneEnded(long seconds) {
		//return false;

		if (TV.millis() - startTime > seconds * 1000) {
				Scene nextScene = currentScene;
				
			#if USE_SECUENTIAL
				nextScene = nextScene + 1;
				if (nextScene == TOTAL) {
					nextScene = 0;
				}
			#else
				while (nextScene == currentScene) {
					nextScene = (Scene)random(TOTAL);
				}
			#endif

				currentScene = nextScene;
				return true;
		}

		return false;
}

void tauroSymbolScene() {
		const char* s = getSignSymbol(1);
		TV.bitmap(BMP_X, BMP_Y, s);
		TV.delay(1);
		checkSceneEnded(5);
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
				return;
		}

		for (int i = 0; i < 30; i++) {
				TV.set_pixel(random(WIDTH), random(HEIGHT), 0);
		}

		if (TV.millis() - startTime > 2600) {
				TV.shift(1, 3);
				TV.shift(1, 0);
		}

		TV.delay(200);

		checkSceneEnded(10);
}

int cmdIdx = 0;
void commandsScene() {
		TV.println(cmds_words[cmdIdx % CMDS_WORDS_COUNT]);
		cmdIdx++;
		TV.delay(1100);


		if (checkSceneEnded(10)) {
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
		TV.delay(150);

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
		TV.delay(250);
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
					TV.print(" OK.");
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

char *lastSnakeBmp = NULL;
Orientation orientation = Tau_NE;
int snakeX = WIDTH / 2;
int snakeY = HEIGHT / 2;
unsigned long lastReset = 0;
void symbolsSnakeScene(bool changed) {
		if (changed) {
				if (random(2) == 0) {
						lastSnakeBmp = getSignSymbol(random(SIGN_COUNT));
				} else {
						lastSnakeBmp = getPlanetSymbol(random(PLANET_COUNT));
				}

				TV.bitmap(BMP_X, BMP_Y, lastSnakeBmp);
				
				lastReset = TV.millis();
				snakeX = WIDTH / 2;
				snakeY = HEIGHT / 2;
		}

		if (TV.millis() - lastReset > 1500) {
				TV.clear_screen();
				TV.bitmap(BMP_X, BMP_Y, lastSnakeBmp);
				lastReset = TV.millis();
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
				default:
						TV.print("Invalid scene. ");
						TV.delay(1000);
						break;
		}
}
