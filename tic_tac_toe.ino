/**************************************************************************
 * @file tic_tac_toe.ino
 * @brief Self-contained Tic-Tac-Toe game engine for ESP32-S3 and SSD1306
 * * Hardware Configuration:
 * - MCU: ESP32-S3
 * - Display: SSD1306 128x32 OLED Panel
 * - SDA: GPIO 47
 * - SCL: GPIO 48
 * * Control Interface:
 * - Inputs are processed via numbers 1-9 over USB Serial at 115200 baud.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

#define I2C_SDA 47
#define I2C_SCL 48

#define OLED_RESET     -1 
#define SCREEN_ADDRESS 0x3C 
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// Game State Definition
char board[9]; 
char currentPlayer;
bool isGameOver;
String gameStatusMessage;

void resetGame();
bool checkWin();
bool checkDraw();
void drawInterface();

void setup() {
  Serial.begin(115200);

  Wire.begin(I2C_SDA, I2C_SCL);
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("ERROR: SSD1306 allocation failed."));
    for(;;); 
  }

  resetGame();
}

void loop() {
  if (Serial.available() > 0) {
    char input = Serial.read();
    
    // Clear any trailing newline or carriage return characters
    while(Serial.available() > 0) { Serial.read(); }

    if (isGameOver) {
      resetGame();
    } else {
      if (input >= '1' && input <= '9') {
        int index = input - '1';
        
        // Validate if cell is empty
        if (board[index] == ' ') {
          board[index] = currentPlayer;
          
          if (checkWin()) {
            gameStatusMessage = String(currentPlayer) + " WINS!";
            isGameOver = true;
          } else if (checkDraw()) {
            gameStatusMessage = "DRAW!";
            isGameOver = true;
          } else {
            // Switch turn
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
            gameStatusMessage = String(currentPlayer) + " TURN";
          }
        }
      }
    }
    drawInterface();
  }
}

void resetGame() {
  for (int i = 0; i < 9; i++) {
    board[i] = ' ';
  }
  currentPlayer = 'X';
  isGameOver = false;
  gameStatusMessage = "X TURN";
  drawInterface();
}

bool checkWin() {
  // Win condition row/column/diagonal mapping offsets
  const int winStates[8][3] = {
    {0, 1, 2}, {3, 4, 5}, {6, 7, 8}, // Rows
    {0, 3, 6}, {1, 4, 7}, {2, 5, 8}, // Columns
    {0, 4, 8}, {2, 4, 6}             // Diagonals
  };

  for (int i = 0; i < 8; i++) {
    if (board[winStates[i][0]] != ' ' &&
        board[winStates[i][0]] == board[winStates[i][1]] &&
        board[winStates[i][0]] == board[winStates[i][2]]) {
      return true;
    }
  }
  return false;
}

bool checkDraw() {
  for (int i = 0; i < 9; i++) {
    if (board[i] == ' ') return false;
  }
  return true;
}

void drawInterface() {
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);

  // Render 3x3 Grid Matrix Borders (Constrained to 32px height)
  display.drawFastVLine(74, 0, 32, SSD1306_WHITE);
  display.drawFastVLine(86, 0, 32, SSD1306_WHITE);
  display.drawFastHLine(64, 10, 32, SSD1306_WHITE);
  display.drawFastHLine(64, 21, 32, SSD1306_WHITE);

  // Map and Render Board Matrix Tokens
  int colWidth = 12;
  int rowHeight = 11;
  int startX = 66;

  for (int i = 0; i < 9; i++) {
    if (board[i] != ' ') {
      int row = i / 3;
      int col = i % 3;
      display.setCursor(startX + (col * colWidth), 2 + (row * rowHeight));
      display.print(board[i]);
    }
  }

  // Render Sidebar Text Status Metadata
  display.setCursor(0, 4);
  display.print(F("TIC-TAC-TOE"));
  display.setCursor(0, 18);
  display.print(gameStatusMessage);

  display.display();
}