#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include <avr/pgmspace.h>

#define I2C_ADDRESS 0x3C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const bool OLED_DIAGNOSTIC_MODE = false;
const uint16_t OLED_BOOT_DELAY_MS = 900;
const uint8_t OLED_INIT_RETRY_COUNT = 3;

const uint8_t MAP_W = 10;
const uint8_t MAP_H = 10;
const char LEVEL_MAP[MAP_H][MAP_W + 1] PROGMEM = {
  "##########",
  "#....#...#",
  "#.##.#.#D#",
  "#.#..#...#",
  "#.#.###..#",
  "#...D....#",
  "###.#.##.#",
  "#...#....#",
  "#.B...#..#",
  "##########"
};

const int16_t DIR_X[64] PROGMEM = {
  128, 127, 126, 122, 118, 113, 106, 99, 91, 81, 71, 60, 49, 37, 25, 13,
  0, -13, -25, -37, -49, -60, -71, -81, -91, -99, -106, -113, -118, -122, -126, -127,
  -128, -127, -126, -122, -118, -113, -106, -99, -91, -81, -71, -60, -49, -37, -25, -13,
  0, 13, 25, 37, 49, 60, 71, 81, 91, 99, 106, 113, 118, 122, 126, 127
};

const int16_t DIR_Y[64] PROGMEM = {
  0, 13, 25, 37, 49, 60, 71, 81, 91, 99, 106, 113, 118, 122, 126, 127,
  128, 127, 126, 122, 118, 113, 106, 99, 91, 81, 71, 60, 49, 37, 25, 13,
  0, -13, -25, -37, -49, -60, -71, -81, -91, -99, -106, -113, -118, -122, -126, -127,
  -128, -127, -126, -122, -118, -113, -106, -99, -91, -81, -71, -60, -49, -37, -25, -13
};

const int8_t RAY_OFFSETS[32] PROGMEM = {
  -10, -10, -9, -8, -8, -7, -6, -6,
  -5, -4, -4, -3, -2, -2, -1, 0,
  0, 1, 2, 2, 3, 4, 4, 5,
  6, 6, 7, 8, 8, 9, 10, 10
};

const int16_t RAY_CORR[32] PROGMEM = {
  71, 71, 81, 91, 91, 99, 106, 106,
  113, 118, 118, 122, 126, 126, 127, 128,
  128, 127, 126, 126, 122, 118, 118, 113,
  106, 106, 99, 91, 91, 81, 71, 71
};

const uint8_t ENEMY_COUNT = 4;
const int16_t ENEMY_SPAWN_X[ENEMY_COUNT] PROGMEM = {
  8 * 128 + 64,
  7 * 128 + 64,
  7 * 128 + 64,
  3 * 128 + 64
};
const int16_t ENEMY_SPAWN_Y[ENEMY_COUNT] PROGMEM = {
  1 * 128 + 64,
  3 * 128 + 64,
  5 * 128 + 64,
  7 * 128 + 64
};

const uint8_t PICKUP_COUNT = 3;
const int16_t PICKUP_SPAWN_X[PICKUP_COUNT] PROGMEM = {
  3 * 128 + 64,
  2 * 128 + 64,
  8 * 128 + 64
};
const int16_t PICKUP_SPAWN_Y[PICKUP_COUNT] PROGMEM = {
  1 * 128 + 64,
  5 * 128 + 64,
  7 * 128 + 64
};
const uint8_t PICKUP_KIND[PICKUP_COUNT] PROGMEM = { 0, 1, 0 };

const uint8_t RAY_COUNT = 32;
const uint8_t COLUMN_W = 4;
const uint8_t WORLD_SHIFT = 7;
const int16_t WORLD_ONE = 1 << WORLD_SHIFT;
const int16_t MOVE_STEP = 10;
const int16_t RAY_STEP = 8;
const uint8_t MAX_RAY_STEPS = 96;
const uint8_t MAX_PLAYER_HP = 99;
const uint8_t MAX_PLAYER_AMMO = 24;

enum PickupType : uint8_t {
  PICKUP_AMMO = 0,
  PICKUP_MED = 1
};

enum StatusCode : uint8_t {
  STATUS_NONE = 0,
  STATUS_HIT,
  STATUS_DOWN,
  STATUS_AMMO,
  STATUS_HEAL,
  STATUS_DOOR,
  STATUS_NO_AMMO,
  STATUS_CLEAR,
  STATUS_DEAD
};

enum RenderKind : uint8_t {
  RENDER_ENEMY = 0,
  RENDER_PICKUP = 1
};

char levelMapRuntime[MAP_H][MAP_W + 1];
int16_t playerX = 1 * WORLD_ONE + WORLD_ONE / 2;
int16_t playerY = 1 * WORLD_ONE + WORLD_ONE / 2;
uint8_t playerAngle = 0;
uint8_t playerHealth = MAX_PLAYER_HP;
uint8_t playerAmmo = 10;

int16_t enemyX[ENEMY_COUNT];
int16_t enemyY[ENEMY_COUNT];
uint8_t enemyHealth[ENEMY_COUNT];
uint16_t enemyAttackCooldown[ENEMY_COUNT];

int16_t pickupX[PICKUP_COUNT];
int16_t pickupY[PICKUP_COUNT];
uint8_t pickupType[PICKUP_COUNT];
bool pickupActive[PICKUP_COUNT];

bool startHeld = false;
bool altHeld = false;
bool czekajNaPuszczenieStartu = true;
bool titleScreen = true;
bool gameWon = false;
bool gameOver = false;

uint16_t statusUntil = 0;
uint8_t statusCode = STATUS_NONE;
uint16_t weaponAnimUntil = 0;
uint16_t weaponFlashUntil = 0;
uint16_t fireCooldownUntil = 0;
uint16_t enemyThinkAt = 0;

uint16_t wallDepths[RAY_COUNT];
char wallKinds[RAY_COUNT];

uint16_t renderDepths[ENEMY_COUNT + PICKUP_COUNT];
int16_t renderScreenXs[ENEMY_COUNT + PICKUP_COUNT];
uint8_t renderSizes[ENEMY_COUNT + PICKUP_COUNT];
uint8_t renderKinds[ENEMY_COUNT + PICKUP_COUNT];
uint8_t renderIndices[ENEMY_COUNT + PICKUP_COUNT];
uint8_t renderCount = 0;

inline uint16_t max16(uint16_t a, uint16_t b) {
  return a > b ? a : b;
}

inline uint8_t max8(uint8_t a, uint8_t b) {
  return a > b ? a : b;
}

inline uint8_t min8(uint8_t a, uint8_t b) {
  return a < b ? a : b;
}

inline int16_t dirXAt(uint8_t idx) {
  return pgm_read_word(&DIR_X[idx]);
}

inline int16_t dirYAt(uint8_t idx) {
  return pgm_read_word(&DIR_Y[idx]);
}

inline int8_t rayOffsetAt(uint8_t idx) {
  return static_cast<int8_t>(pgm_read_byte(&RAY_OFFSETS[idx]));
}

inline int16_t rayCorrAt(uint8_t idx) {
  return pgm_read_word(&RAY_CORR[idx]);
}

inline uint16_t now16() {
  return static_cast<uint16_t>(millis());
}

inline bool timerActive(uint16_t now, uint16_t until) {
  return static_cast<int16_t>(until - now) > 0;
}

inline bool upPressed() {
  return !(PIND & B10000000);
}

inline bool downPressed() {
  return !(PINB & B00000010);
}

inline bool rightPressed() {
  return !(PINB & B00000001);
}

inline bool leftPressed() {
  return !(PINB & B00000100);
}

inline bool startPressed() {
  return !(PIND & B00100000);
}

inline bool altPressed() {
  return !(PIND & B00010000);
}

uint8_t wrapAngle(int16_t angle) {
  return static_cast<uint8_t>(angle) & 63;
}

void setStatus(StatusCode code, uint16_t durationMs) {
  statusCode = code;
  statusUntil = now16() + durationMs;
}

char tileAt(int8_t tx, int8_t ty) {
  if (tx < 0 || ty < 0 || tx >= MAP_W || ty >= MAP_H) return '#';
  return levelMapRuntime[ty][tx];
}

void setTile(int8_t tx, int8_t ty, char tile) {
  if (tx < 0 || ty < 0 || tx >= MAP_W || ty >= MAP_H) return;
  levelMapRuntime[ty][tx] = tile;
}

bool wallAtTile(int8_t tx, int8_t ty) {
  char tile = tileAt(tx, ty);
  return tile == '#' || tile == 'D' || tile == 'B';
}

bool canMoveTo(int16_t x, int16_t y) {
  const int16_t radius = 10;
  return !wallAtTile((x - radius) >> WORLD_SHIFT, (y - radius) >> WORLD_SHIFT) &&
         !wallAtTile((x + radius) >> WORLD_SHIFT, (y - radius) >> WORLD_SHIFT) &&
         !wallAtTile((x - radius) >> WORLD_SHIFT, (y + radius) >> WORLD_SHIFT) &&
         !wallAtTile((x + radius) >> WORLD_SHIFT, (y + radius) >> WORLD_SHIFT);
}

uint16_t approxDistance(int16_t dx, int16_t dy) {
  dx = abs(dx);
  dy = abs(dy);
  return dx > dy ? dx + dy / 2 : dy + dx / 2;
}

void copyBaseMap() {
  for (uint8_t y = 0; y < MAP_H; y++) {
    for (uint8_t x = 0; x < MAP_W; x++) {
      levelMapRuntime[y][x] = pgm_read_byte(&LEVEL_MAP[y][x]);
    }
    levelMapRuntime[y][MAP_W] = '\0';
  }
}

void resetDemoState() {
  copyBaseMap();
  playerX = 1 * WORLD_ONE + WORLD_ONE / 2;
  playerY = 1 * WORLD_ONE + WORLD_ONE / 2;
  playerAngle = 0;
  playerHealth = MAX_PLAYER_HP;
  playerAmmo = 10;
  gameWon = false;
  gameOver = false;
  statusCode = STATUS_NONE;
  statusUntil = 0;
  weaponAnimUntil = 0;
  weaponFlashUntil = 0;
  fireCooldownUntil = 0;
  enemyThinkAt = 0;
  startHeld = false;
  altHeld = false;

  for (uint8_t i = 0; i < ENEMY_COUNT; i++) {
    enemyX[i] = pgm_read_word(&ENEMY_SPAWN_X[i]);
    enemyY[i] = pgm_read_word(&ENEMY_SPAWN_Y[i]);
    enemyHealth[i] = i == 0 ? 3 : 2;
    enemyAttackCooldown[i] = 0;
  }

  for (uint8_t i = 0; i < PICKUP_COUNT; i++) {
    pickupX[i] = pgm_read_word(&PICKUP_SPAWN_X[i]);
    pickupY[i] = pgm_read_word(&PICKUP_SPAWN_Y[i]);
    pickupType[i] = pgm_read_byte(&PICKUP_KIND[i]);
    pickupActive[i] = true;
  }
}

bool lineOfSight(int16_t fromX, int16_t fromY, int16_t toX, int16_t toY) {
  int16_t dx = toX - fromX;
  int16_t dy = toY - fromY;
  uint8_t steps = max(abs(dx), abs(dy)) / 12;
  if (steps == 0) return true;

  int32_t x = fromX;
  int32_t y = fromY;
  int16_t stepX = dx / steps;
  int16_t stepY = dy / steps;

  for (uint8_t i = 0; i < steps; i++) {
    x += stepX;
    y += stepY;
    if (wallAtTile(x >> WORLD_SHIFT, y >> WORLD_SHIFT)) {
      return false;
    }
  }
  return true;
}

int8_t livingEnemies() {
  int8_t count = 0;
  for (uint8_t i = 0; i < ENEMY_COUNT; i++) {
    if (enemyHealth[i] > 0) count++;
  }
  return count;
}

uint16_t castRay(uint8_t rayIndex) {
  uint8_t rayAngle = wrapAngle(playerAngle + rayOffsetAt(rayIndex));
  int32_t rayX = playerX;
  int32_t rayY = playerY;

  for (uint8_t step = 0; step < MAX_RAY_STEPS; step++) {
    rayX += (dirXAt(rayAngle) * RAY_STEP) / 128;
    rayY += (dirYAt(rayAngle) * RAY_STEP) / 128;

    char tile = tileAt(rayX >> WORLD_SHIFT, rayY >> WORLD_SHIFT);
    if (tile == '#' || tile == 'D') {
      wallKinds[rayIndex] = tile;
      uint16_t rawDist = (step + 1) * RAY_STEP;
      return max16(16, (rawDist * rayCorrAt(rayIndex)) / 128);
    }
  }

  wallKinds[rayIndex] = '#';
  return MAX_RAY_STEPS * RAY_STEP;
}

void drawWallColumn(uint8_t rayIndex, uint16_t dist, char wallKind, uint16_t now) {
  uint8_t x = rayIndex * COLUMN_W;
  uint8_t wallHeight = min8(52, 4600U / max16(24, dist));
  int8_t top = (SCREEN_HEIGHT / 2) - (wallHeight / 2) - 2;
  if (top < 8) top = 8;
  uint8_t bottom = top + wallHeight;

  if (wallKind == 'D') {
    display.drawRect(x, top, COLUMN_W, wallHeight, SH110X_WHITE);
    if (wallHeight > 4) {
      display.drawFastVLine(x + 1, top + 1, wallHeight - 2, SH110X_WHITE);
      display.drawFastVLine(x + 2, top + 1, wallHeight - 2, SH110X_WHITE);
      display.drawFastVLine(x + 1 + ((now / 240U) & 1), top + 1, wallHeight - 2, SH110X_BLACK);
      display.drawFastHLine(x, top + wallHeight / 2, COLUMN_W, SH110X_WHITE);
      for (uint8_t knobY = top + 3; knobY + 3 < bottom; knobY += 6) {
        if (((knobY + rayIndex + (now / 100U)) & 1) == 0) {
          display.drawPixel(x + 2, knobY, SH110X_BLACK);
        }
      }
    }
    return;
  }

  if (wallKind == 'B') {
    display.fillRect(x, top, COLUMN_W, wallHeight, SH110X_WHITE);
    if (wallHeight > 5) {
      for (uint8_t y = top + 1; y + 1 < bottom; y += 4) {
        display.drawFastHLine(x, y, COLUMN_W, SH110X_BLACK);
      }
      if (((rayIndex + (now / 160U)) & 1) == 0) {
        display.drawFastVLine(x + 1, top + 1, wallHeight - 2, SH110X_BLACK);
      } else {
        display.drawFastVLine(x + 2, top + 1, wallHeight - 2, SH110X_BLACK);
      }
    }
    return;
  }

  if (dist < 120) {
    display.fillRect(x, top, COLUMN_W, wallHeight, SH110X_WHITE);
    if (wallHeight > 6) {
      for (uint8_t y = top + 2; y + 2 < bottom; y += 6) {
        display.drawFastHLine(x, y, COLUMN_W, SH110X_BLACK);
      }
    }
  } else if (dist < 220) {
    display.drawRect(x, top, COLUMN_W, wallHeight, SH110X_WHITE);
    display.drawFastVLine(x + 1, top, wallHeight, SH110X_WHITE);
    display.drawFastVLine(x + 2, top, wallHeight, SH110X_WHITE);
    if (((rayIndex + (now / 120U)) & 1) == 0 && wallHeight > 8) {
      for (uint8_t y = top + 3; y + 2 < bottom; y += 7) {
        display.drawPixel(x + 1, y, SH110X_BLACK);
      }
    }
  } else {
    display.drawFastVLine(x + 1, top, wallHeight, SH110X_WHITE);
    display.drawFastVLine(x + 2, top, wallHeight, SH110X_WHITE);
    if (((rayIndex + (now / 180U)) & 3) == 0) {
      display.drawPixel(x + 1, top + wallHeight / 2, SH110X_WHITE);
    }
  }
}

bool projectSprite(int16_t objX, int16_t objY, uint16_t& forward, int16_t& screenX, uint8_t& size, uint8_t& rayIndex) {
  int16_t dx = objX - playerX;
  int16_t dy = objY - playerY;
  int16_t side = (dx * (-dirYAt(playerAngle)) + dy * dirXAt(playerAngle)) / 128;
  forward = (dx * dirXAt(playerAngle) + dy * dirYAt(playerAngle)) / 128;

  if (forward <= 18) return false;
  if (abs(side) > forward + 16) return false;

  int16_t clampedForward = forward > 24 ? forward : 24;
  int16_t spriteForward = forward > 64 ? forward : 64;
  screenX = 64 + (side * 64) / clampedForward;
  size = constrain(3400 / spriteForward, 6, 30);
  rayIndex = constrain(screenX / COLUMN_W, 0, RAY_COUNT - 1);

  if (forward >= static_cast<int16_t>(wallDepths[rayIndex]) - 6) return false;
  return true;
}

void drawEnemySprite(int16_t centerX, uint8_t size, uint8_t hp, uint16_t now) {
  int8_t bob = ((now / 120U) + (centerX / 9)) & 1 ? 0 : 1;
  int16_t left = centerX - size / 2;
  int16_t top = (SCREEN_HEIGHT / 2) - size / 2 + 2 + bob;
  if (left > SCREEN_WIDTH || left + size < 0) return;

  int16_t headW = max8(5, size / 2);
  int16_t bodyW = max8(4, size / 3);
  int16_t headX = centerX - headW / 2;
  int16_t bodyX = centerX - bodyW / 2;
  uint8_t headH = max8(4, size / 2);
  uint8_t bodyH = max8(4, size / 2);
  uint8_t armLift = ((now / 100U) + hp) & 1 ? max8(1, size / 8) : 0;
  uint8_t mouthOffset = ((now / 90U) + hp) & 1 ? 1 : 0;

  display.drawLine(headX + 1, top, headX + 3, top - 2, SH110X_WHITE);
  display.drawLine(headX + headW - 2, top, headX + headW - 4, top - 2, SH110X_WHITE);
  display.drawRect(headX, top, headW, headH, SH110X_WHITE);
  display.fillRect(headX + 1, top + 1, 1, 1, SH110X_WHITE);
  display.fillRect(headX + headW - 2, top + 1, 1, 1, SH110X_WHITE);
  display.drawFastHLine(headX + 1, top + headH - 2 - mouthOffset, headW - 2, SH110X_WHITE);
  display.fillRect(bodyX, top + size / 2, bodyW, bodyH, SH110X_WHITE);
  display.drawLine(bodyX - 2, top + size / 2 + armLift, bodyX, top + size / 2 + 2, SH110X_WHITE);
  display.drawLine(bodyX + bodyW + 1, top + size / 2 + armLift, bodyX + bodyW - 1, top + size / 2 + 2, SH110X_WHITE);
  display.drawLine(bodyX + 1, top + size, bodyX, top + size + max8(2, size / 4), SH110X_WHITE);
  display.drawLine(bodyX + bodyW - 2, top + size, bodyX + bodyW - 1, top + size + max8(2, size / 4), SH110X_WHITE);

  if (hp == 1) {
    display.drawFastVLine(centerX, top + size / 2 + 1, bodyH - 2, SH110X_BLACK);
    display.drawPixel(headX + 1, top + 1, SH110X_BLACK);
  }
}

void drawPickupSprite(int16_t centerX, uint8_t size, uint8_t type, uint16_t now) {
  int8_t bob = ((now / 180U) + (centerX / 6)) & 1 ? 0 : 1;
  int16_t left = centerX - size / 2;
  int16_t top = (SCREEN_HEIGHT / 2) - size / 2 + 8 + bob;
  if (left > SCREEN_WIDTH || left + size < 0) return;

  uint8_t box = max8(5, size / 2);
  int16_t boxX = centerX - box / 2;
  display.drawRect(boxX, top, box, box, SH110X_WHITE);
  display.drawFastHLine(boxX, top + box, box, SH110X_WHITE);

  if (type == PICKUP_AMMO) {
    display.drawFastVLine(centerX - 2, top + 1, box - 2, SH110X_WHITE);
    display.drawFastVLine(centerX + 1, top + 1, box - 2, SH110X_WHITE);
  } else {
    uint8_t midY = top + box / 2;
    display.drawFastHLine(boxX + 1, midY, box - 2, SH110X_WHITE);
    display.drawFastVLine(centerX, top + 1, box - 2, SH110X_WHITE);
  }
}

void collectVisibleObjects() {
  renderCount = 0;

  for (uint8_t i = 0; i < ENEMY_COUNT; i++) {
    if (enemyHealth[i] == 0 || !lineOfSight(playerX, playerY, enemyX[i], enemyY[i])) continue;

    uint16_t forward = 0;
    int16_t screenX = 0;
    uint8_t size = 0;
    uint8_t rayIndex = 0;
    if (!projectSprite(enemyX[i], enemyY[i], forward, screenX, size, rayIndex)) continue;

    renderDepths[renderCount] = forward;
    renderScreenXs[renderCount] = screenX;
    renderSizes[renderCount] = size;
    renderKinds[renderCount] = RENDER_ENEMY;
    renderIndices[renderCount] = i;
    renderCount++;
  }

  for (uint8_t i = 0; i < PICKUP_COUNT; i++) {
    if (!pickupActive[i] || !lineOfSight(playerX, playerY, pickupX[i], pickupY[i])) continue;

    uint16_t forward = 0;
    int16_t screenX = 0;
    uint8_t size = 0;
    uint8_t rayIndex = 0;
    if (!projectSprite(pickupX[i], pickupY[i], forward, screenX, size, rayIndex)) continue;

    renderDepths[renderCount] = forward;
    renderScreenXs[renderCount] = screenX;
    renderSizes[renderCount] = size;
    renderKinds[renderCount] = RENDER_PICKUP;
    renderIndices[renderCount] = i;
    renderCount++;
  }

  for (uint8_t i = 0; i < renderCount; i++) {
    for (uint8_t j = i + 1; j < renderCount; j++) {
      if (renderDepths[j] > renderDepths[i]) {
        uint16_t depthTmp = renderDepths[i];
        renderDepths[i] = renderDepths[j];
        renderDepths[j] = depthTmp;

        int16_t xTmp = renderScreenXs[i];
        renderScreenXs[i] = renderScreenXs[j];
        renderScreenXs[j] = xTmp;

        uint8_t sizeTmp = renderSizes[i];
        renderSizes[i] = renderSizes[j];
        renderSizes[j] = sizeTmp;

        uint8_t kindTmp = renderKinds[i];
        renderKinds[i] = renderKinds[j];
        renderKinds[j] = kindTmp;

        uint8_t idxTmp = renderIndices[i];
        renderIndices[i] = renderIndices[j];
        renderIndices[j] = idxTmp;
      }
    }
  }
}

void drawMinimap() {
  const uint8_t cell = 3;
  const uint8_t mapX = SCREEN_WIDTH - MAP_W * cell - 2;
  const uint8_t mapY = 9;

  display.drawRect(mapX - 1, mapY - 1, MAP_W * cell + 2, MAP_H * cell + 2, SH110X_WHITE);

  for (uint8_t y = 0; y < MAP_H; y++) {
    for (uint8_t x = 0; x < MAP_W; x++) {
      char tile = tileAt(x, y);
      uint8_t px = mapX + x * cell;
      uint8_t py = mapY + y * cell;
      if (tile == '#') {
        display.fillRect(px, py, cell, cell, SH110X_WHITE);
      } else if (tile == 'D') {
        display.drawRect(px, py, cell, cell, SH110X_WHITE);
      }
    }
  }

  for (uint8_t i = 0; i < PICKUP_COUNT; i++) {
    if (!pickupActive[i]) continue;
    uint8_t px = mapX + (pickupX[i] >> WORLD_SHIFT) * cell + 1;
    uint8_t py = mapY + (pickupY[i] >> WORLD_SHIFT) * cell + 1;
    display.drawPixel(px, py, SH110X_WHITE);
  }

  for (uint8_t i = 0; i < ENEMY_COUNT; i++) {
    if (enemyHealth[i] == 0) continue;
    uint8_t px = mapX + (enemyX[i] >> WORLD_SHIFT) * cell;
    uint8_t py = mapY + (enemyY[i] >> WORLD_SHIFT) * cell;
    display.drawRect(px, py, 2, 2, SH110X_WHITE);
  }

  uint8_t playerTileX = playerX >> WORLD_SHIFT;
  uint8_t playerTileY = playerY >> WORLD_SHIFT;
  uint8_t px = mapX + playerTileX * cell + 1;
  uint8_t py = mapY + playerTileY * cell + 1;
  display.fillRect(px, py, 2, 2, SH110X_WHITE);
  display.drawPixel(px + constrain(dirXAt(playerAngle) / 96, -1, 1), py + constrain(dirYAt(playerAngle) / 96, -1, 1), SH110X_WHITE);
}

void drawCrosshair() {
  display.drawFastHLine(60, 30, 8, SH110X_WHITE);
  display.drawFastVLine(64, 26, 8, SH110X_WHITE);
  display.drawPixel(64, 30, SH110X_BLACK);
}

void drawWeapon(uint16_t now) {
  bool moving = upPressed() || downPressed();
  int8_t bobX = 0;
  int8_t bobY = 0;
  if (moving) {
    uint8_t walkPhase = (now / 70U) & 3;
    bobX = walkPhase < 2 ? walkPhase : 3 - walkPhase;
    bobY = walkPhase == 1 || walkPhase == 2 ? 1 : 0;
  }

  int8_t recoil = timerActive(now, weaponAnimUntil) ? 3 : 0;
  int16_t gunX = 50 + bobX;
  int16_t gunY = 52 + bobY + recoil;

  display.fillRect(gunX, gunY, 28, 9, SH110X_WHITE);
  display.drawFastHLine(gunX + 2, gunY + 2, 24, SH110X_BLACK);
  display.fillRect(gunX + 8, gunY - 7, 14, 8, SH110X_WHITE);
  display.fillRect(gunX + 14, gunY - 11, 3, 4, SH110X_WHITE);
  display.fillRect(gunX + 11, gunY + 9, 8, 4, SH110X_WHITE);
  display.drawFastVLine(gunX + 13, gunY + 9, 4, SH110X_BLACK);

  if (timerActive(now, weaponFlashUntil)) {
    int16_t flashX = gunX + 22;
    int16_t flashY = gunY - 8;
    display.drawFastHLine(flashX, flashY + 3, 10, SH110X_WHITE);
    display.drawFastVLine(flashX + 5, flashY, 8, SH110X_WHITE);
    display.drawLine(flashX + 2, flashY + 1, flashX + 8, flashY - 3, SH110X_WHITE);
    display.drawLine(flashX + 2, flashY + 5, flashX + 8, flashY + 9, SH110X_WHITE);
    display.drawLine(flashX + 7, flashY + 2, flashX + 12, flashY + 1, SH110X_WHITE);
    display.drawLine(flashX + 7, flashY + 4, flashX + 12, flashY + 5, SH110X_WHITE);
  }
}

void drawHud() {
  display.drawFastHLine(0, 7, SCREEN_WIDTH, SH110X_WHITE);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(2, 0);
  display.print(F("HP"));
  display.print(playerHealth);
  display.print(F(" A"));
  display.print(playerAmmo);
  display.print(F(" E"));
  display.print(livingEnemies());
}

void drawStatusLine(uint16_t now) {
  if (!timerActive(now, statusUntil)) return;

  display.setTextColor(SH110X_WHITE);
  display.setTextSize(1);
  display.setCursor(2, 54);

  switch (statusCode) {
    case STATUS_HIT:
      display.print(F("Hit!"));
      break;
    case STATUS_DOWN:
      display.print(F("Demon down"));
      break;
    case STATUS_AMMO:
      display.print(F("Ammo up"));
      break;
    case STATUS_HEAL:
      display.print(F("Medkit"));
      break;
    case STATUS_DOOR:
      display.print(F("Door open"));
      break;
    case STATUS_NO_AMMO:
      display.print(F("Out of ammo"));
      break;
    case STATUS_CLEAR:
      display.print(F("Floor clear - PD4"));
      break;
    case STATUS_DEAD:
      display.print(F("You died - PD4"));
      break;
    default:
      break;
  }
}

void drawFloorAndCeiling(uint16_t now) {
  uint8_t starShift = (now / 220U) & 3;
  for (uint8_t y = 10; y < 26; y += 5) {
    for (uint8_t x = (y + starShift) & 3; x < 96; x += 17) {
      display.drawPixel(x, y, SH110X_WHITE);
    }
  }

  for (uint8_t y = 37; y < 52; y += 5) {
    uint8_t offset = ((y + (now / 140U)) & 7);
    for (uint8_t x = offset; x < 92; x += 12) {
      display.drawFastHLine(x, y, 4, SH110X_WHITE);
    }
  }
}

void drawEndOverlay() {
  display.fillRect(18, 18, 92, 26, SH110X_BLACK);
  display.drawRect(18, 18, 92, 26, SH110X_WHITE);
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(2);
  display.setCursor(24, 22);
  if (gameWon) {
    display.print(F("CLEAR"));
  } else {
    display.print(F("DEAD"));
  }
  display.setTextSize(1);
  display.setCursor(28, 38);
  display.print(F("PD4 = restart"));
}

void drawScene() {
  uint16_t now = now16();
  display.clearDisplay();
  display.drawFastHLine(0, SCREEN_HEIGHT / 2, SCREEN_WIDTH, SH110X_WHITE);
  drawFloorAndCeiling(now);

  for (uint8_t ray = 0; ray < RAY_COUNT; ray++) {
    uint16_t dist = castRay(ray);
    wallDepths[ray] = dist;
    drawWallColumn(ray, dist, wallKinds[ray], now);
  }

  collectVisibleObjects();
  for (uint8_t i = 0; i < renderCount; i++) {
    if (renderKinds[i] == RENDER_ENEMY) {
      drawEnemySprite(renderScreenXs[i], renderSizes[i], enemyHealth[renderIndices[i]], now);
    } else {
      drawPickupSprite(renderScreenXs[i], renderSizes[i], pickupType[renderIndices[i]], now);
    }
  }

  drawCrosshair();
  drawWeapon(now);
  drawHud();
  drawMinimap();
  drawStatusLine(now);

  if (gameWon || gameOver) {
    drawEndOverlay();
  }

  display.display();
}

void collectPickups() {
  for (uint8_t i = 0; i < PICKUP_COUNT; i++) {
    if (!pickupActive[i]) continue;

    uint16_t dist = approxDistance(pickupX[i] - playerX, pickupY[i] - playerY);
    if (dist > 20) continue;

    pickupActive[i] = false;
    if (pickupType[i] == PICKUP_AMMO) {
      playerAmmo = min8(MAX_PLAYER_AMMO, static_cast<uint8_t>(playerAmmo + 6));
      setStatus(STATUS_AMMO, 1000);
    } else {
      playerHealth = min8(MAX_PLAYER_HP, static_cast<uint8_t>(playerHealth + 20));
      setStatus(STATUS_HEAL, 1000);
    }
  }
}

void moveEnemyTowardPlayer(uint8_t enemyIdx) {
  int16_t dx = playerX - enemyX[enemyIdx];
  int16_t dy = playerY - enemyY[enemyIdx];
  int16_t stepX = dx == 0 ? 0 : (dx > 0 ? 6 : -6);
  int16_t stepY = dy == 0 ? 0 : (dy > 0 ? 6 : -6);

  int16_t nextX = enemyX[enemyIdx] + stepX;
  int16_t nextY = enemyY[enemyIdx] + stepY;

  if (canMoveTo(nextX, enemyY[enemyIdx])) enemyX[enemyIdx] = nextX;
  if (canMoveTo(enemyX[enemyIdx], nextY)) enemyY[enemyIdx] = nextY;
}

void updateEnemies(uint16_t now) {
  if (gameWon || gameOver) return;
  if (timerActive(now, enemyThinkAt)) return;
  enemyThinkAt = now + 150;

  for (uint8_t i = 0; i < ENEMY_COUNT; i++) {
    if (enemyHealth[i] == 0) continue;

    uint16_t dist = approxDistance(enemyX[i] - playerX, enemyY[i] - playerY);
    if (dist > 46) {
      if (lineOfSight(enemyX[i], enemyY[i], playerX, playerY)) {
        moveEnemyTowardPlayer(i);
      }
      continue;
    }

    if (!timerActive(now, enemyAttackCooldown[i])) {
      enemyAttackCooldown[i] = now + 700;
      if (playerHealth <= 8) {
        playerHealth = 0;
      } else {
        playerHealth -= 8;
      }
      if (playerHealth == 0) {
        gameOver = true;
        setStatus(STATUS_DEAD, 4000);
      }
    }
  }
}

int8_t enemyInCrosshair() {
  int8_t bestEnemy = -1;
  uint16_t bestForward = 65000;

  for (uint8_t i = 0; i < ENEMY_COUNT; i++) {
    if (enemyHealth[i] == 0) continue;
    if (!lineOfSight(playerX, playerY, enemyX[i], enemyY[i])) continue;

    uint16_t forward = 0;
    int16_t screenX = 0;
    uint8_t size = 0;
    uint8_t rayIndex = 0;
    if (!projectSprite(enemyX[i], enemyY[i], forward, screenX, size, rayIndex)) continue;
    if (abs(screenX - 64) > 10) continue;
    if (forward >= bestForward) continue;

    bestForward = forward;
    bestEnemy = i;
  }

  return bestEnemy;
}

int8_t doorAhead() {
  int16_t lookX = playerX + (dirXAt(playerAngle) * 38) / 128;
  int16_t lookY = playerY + (dirYAt(playerAngle) * 38) / 128;
  int8_t tx = lookX >> WORLD_SHIFT;
  int8_t ty = lookY >> WORLD_SHIFT;
  return tileAt(tx, ty) == 'D' ? (ty << 4) | tx : -1;
}

void tryOpenDoor() {
  int8_t door = doorAhead();
  if (door < 0) return;

  int8_t tx = door & 0x0F;
  int8_t ty = (door >> 4) & 0x0F;
  setTile(tx, ty, '.');
  setStatus(STATUS_DOOR, 900);
}

void shootWeapon(uint16_t now) {
  if (timerActive(now, fireCooldownUntil)) return;
  fireCooldownUntil = now + 160;
  weaponAnimUntil = now + 120;
  weaponFlashUntil = now + 70;

  if (playerAmmo == 0) {
    setStatus(STATUS_NO_AMMO, 900);
    return;
  }

  playerAmmo--;
  int8_t hitEnemy = enemyInCrosshair();
  if (hitEnemy < 0) {
    return;
  }

  if (enemyHealth[hitEnemy] > 0) {
    enemyHealth[hitEnemy]--;
  }

  if (enemyHealth[hitEnemy] == 0) {
    setStatus(STATUS_DOWN, 1200);
    if (livingEnemies() == 0) {
      gameWon = true;
      setStatus(STATUS_CLEAR, 5000);
    }
  } else {
    setStatus(STATUS_HIT, 800);
  }
}

void handleMovement() {
  if (gameWon || gameOver) return;

  if (leftPressed()) {
    playerAngle = wrapAngle(playerAngle - 2);
  }
  if (rightPressed()) {
    playerAngle = wrapAngle(playerAngle + 2);
  }

  int16_t moveX = 0;
  int16_t moveY = 0;

  if (upPressed()) {
    moveX += (dirXAt(playerAngle) * MOVE_STEP) / 128;
    moveY += (dirYAt(playerAngle) * MOVE_STEP) / 128;
  }
  if (downPressed()) {
    moveX -= (dirXAt(playerAngle) * MOVE_STEP) / 128;
    moveY -= (dirYAt(playerAngle) * MOVE_STEP) / 128;
  }

  int16_t nextX = playerX + moveX;
  int16_t nextY = playerY + moveY;

  if (canMoveTo(nextX, playerY)) playerX = nextX;
  if (canMoveTo(playerX, nextY)) playerY = nextY;
}

void handleActions() {
  uint16_t now = now16();
  bool startNow = startPressed();
  bool altNow = altPressed();

  if (startNow && !startHeld && !titleScreen) {
    shootWeapon(now);
  }

  if (altNow && !altHeld) {
    if (gameWon || gameOver) {
      resetDemoState();
    } else {
      tryOpenDoor();
    }
  }

  startHeld = startNow;
  altHeld = altNow;
}

void drawTitle() {
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);
  display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SH110X_WHITE);
  display.setTextSize(2);
  display.setCursor(18, 5);
  display.print(F("DOOM"));
  display.setCursor(18, 23);
  display.print(F("DEMO"));
  display.setTextSize(1);
  display.setCursor(10, 42);
  display.print(F("4 demons, maze floor"));
  display.setCursor(8, 50);
  display.print(F("START = play/shoot"));
  display.setCursor(12, 58);
  display.print(F("PD4 = open/reset"));
  display.display();
}

void runDiagnosticFrame() {
  uint8_t phase = (millis() / 900U) % 4;
  display.clearDisplay();
  display.setTextColor(SH110X_WHITE);

  if (phase == 0) {
    display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SH110X_WHITE);
  } else if (phase == 1) {
    display.drawRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SH110X_WHITE);
    display.drawLine(0, 0, SCREEN_WIDTH - 1, SCREEN_HEIGHT - 1, SH110X_WHITE);
    display.drawLine(SCREEN_WIDTH - 1, 0, 0, SCREEN_HEIGHT - 1, SH110X_WHITE);
    display.setTextSize(2);
    display.setCursor(14, 24);
    display.print(F("TEST"));
  } else if (phase == 2) {
    display.setTextSize(2);
    display.setCursor(10, 8);
    display.print(F("DOOM"));
    display.setCursor(10, 28);
    display.print(F("OLED"));
    display.setTextSize(1);
    display.setCursor(22, 54);
    display.print(F("diag mode"));
  } else {
    for (uint8_t y = 0; y < SCREEN_HEIGHT; y += 4) {
      for (uint8_t x = (y / 4) % 2 ? 2 : 0; x < SCREEN_WIDTH; x += 4) {
        display.drawPixel(x, y, SH110X_WHITE);
      }
    }
    display.drawFastHLine(0, 10, SCREEN_WIDTH, SH110X_WHITE);
    display.drawFastHLine(0, 53, SCREEN_WIDTH, SH110X_WHITE);
  }

  display.display();
}

bool initDisplaySafely() {
  Wire.begin();
  delay(OLED_BOOT_DELAY_MS);

  for (uint8_t attempt = 0; attempt < OLED_INIT_RETRY_COUNT; attempt++) {
    if (display.begin(I2C_ADDRESS, true)) {
      display.setRotation(2);
      display.clearDisplay();
      display.display();
      return true;
    }
    delay(250);
  }

  return false;
}

void setup() {
  PORTD |= B10110000;
  PORTB |= B00000111;

  if (!initDisplaySafely()) {
    while (true) {
      delay(100);
    }
  }

  resetDemoState();

  display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, SH110X_WHITE);
  display.display();
  delay(250);
  display.clearDisplay();
  display.display();
  delay(120);

  drawTitle();
}

void loop() {
  if (OLED_DIAGNOSTIC_MODE) {
    runDiagnosticFrame();
    delay(30);
    return;
  }

  if (titleScreen) {
    drawTitle();
    bool startNow = startPressed();
    if (czekajNaPuszczenieStartu) {
      if (!startNow) {
        czekajNaPuszczenieStartu = false;
      }
      delay(16);
      return;
    }

    if (startNow && !startHeld) {
      titleScreen = false;
      resetDemoState();
      startHeld = true;
      delay(180);
    } else {
      startHeld = startNow;
      delay(16);
      return;
    }
  }

  collectPickups();
  handleMovement();
  handleActions();
  updateEnemies(now16());
  drawScene();
  delay(16);
}
