#include <Arduino.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

#include <EEPROM.h>

/** standard U of A library settings, assuming Atmel Mega SPI pins */
#define TFT_CS   6  // Chip select line for TFT display
#define TFT_DC   7  // Data/command line for TFT
#define TFT_RST  8  // Reset line for TFT (or connect to +5V)



Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);

/** specify pins */
const int JOYSTICK_VERT = 0;    // Analog input A0 - vertical
const int JOYSTICK_HORIZ = 1;   // Analog input A1 - horizontald
const int JOYSTICK_BUTTON = 9;  // Digital input pin 9 for the button

int buttonValue;

typedef struct {
  char char1;
  char char2;
  char char3;
  uint8_t score;
} highscore;

typedef struct {
    int y;
    int x;
    int dir;
    /**direction:
     *  1 = UP
     *  2 = DOWN
     *  3 = LEFT
     *  4 = RIGHT
     */
} SnakeMove;

int score = 0;

typedef struct {
    uint16_t y;
    uint16_t x;
} coordinates;

coordinates random_apple() {
    coordinates apple;
    apple.x = ((rand() %128) *4) %128;
    apple.y = ((rand() %160) *4) % 160;
    return apple;
}

void storeHighscoreEEPROM(highscore hScore, int place){
  int actualPlace = place * 4;
  EEPROM.put(actualPlace, hScore.char1);
  EEPROM.put(actualPlace + 1, hScore.char2);
  EEPROM.put(actualPlace + 2, hScore.char3);
  EEPROM.put(actualPlace + 3, hScore.score);
}

highscore readHighscoreEEPROM(int place){
  int actualPlace = place * 4;
  highscore hScore;
  EEPROM.get(actualPlace, hScore.char1);
  EEPROM.get(actualPlace + 1, hScore.char2);
  EEPROM.get(actualPlace + 2, hScore.char3);
  EEPROM.get(actualPlace + 3, hScore.score);
  return hScore;
}

highscore enter_new_highscore(highscore hScore){
  tft.fillRect(0, 0, 128, 160, 0x8811);
  tft.setCursor(20, 15);
  tft.setTextColor(0x07E0);
  tft.setTextSize(2);
  tft.println("NEW HIGH");
  tft.setCursor(30,34);
  tft.println("SCORE");
  tft.println();

  tft.setCursor(30, 70);
  tft.setTextColor(0xFFFF);
  tft.setTextSize(2);
  tft.print("AAA   ");
  tft.print(score);

  hScore.char1 = 'A';
  hScore.char2 = 'A';
  hScore.char3 = 'A';

  int hasBeenpressed = 0;

  int init_vert = analogRead(JOYSTICK_VERT); // do auto calibration

  while(true){
    int vert = analogRead(JOYSTICK_VERT);       // will be 0-1023

    if (vert > (init_vert + 10)){
      hScore.char1 = hScore.char1 + 1;
    }
    else if (vert < (init_vert - 10)){
      hScore.char1 = hScore.char1 - 1;
    }
    tft.setCursor(30, 70);
    tft.setTextColor(0x0000, 0xFFFF);
    tft.setTextSize(2);
    tft.print(hScore.char1);

    // read the pushbutton state, if pressed exit loop and move onto next character
    buttonValue = digitalRead(JOYSTICK_BUTTON);

    if (buttonValue == LOW && hasBeenpressed != 1) {
        hasBeenpressed = 1;
        tft.setCursor(30, 70);
        tft.setTextColor(0xFFFF, 0x8811);
        tft.setTextSize(2);
        tft.print(hScore.char1);
        break;
    }
    else if (buttonValue == HIGH){
        hasBeenpressed = 0;
    }

    delay(100);
  }

  while(true){
    int vert = analogRead(JOYSTICK_VERT);       // will be 0-1023

    if (vert > (init_vert + 10)){
      hScore.char2 = hScore.char2 + 1;
    }
    else if (vert < (init_vert - 10)){
      hScore.char2 = hScore.char2 - 1;
    }
    tft.setCursor(42, 70);
    tft.setTextColor(0x0000, 0xFFFF);
    tft.setTextSize(2);
    tft.print(hScore.char2);

    // read the pushbutton state, if pressed exit loop and move onto next character
    buttonValue = digitalRead(JOYSTICK_BUTTON);

    if (buttonValue == LOW && hasBeenpressed != 1) {
        hasBeenpressed = 1;
        tft.setCursor(42, 70);
        tft.setTextColor(0xFFFF, 0x8811);
        tft.setTextSize(2);
        tft.print(hScore.char2);
        break;
    }
    else if (buttonValue == HIGH){
        hasBeenpressed = 0;
    }

    delay(100);
  }

  while(true){
    int vert = analogRead(JOYSTICK_VERT);       // will be 0-1023

    if (vert > (init_vert + 10)){
      hScore.char3 = hScore.char3 + 1;
    }
    else if (vert < (init_vert - 10)){
      hScore.char3 = hScore.char3 - 1;
    }
    tft.setCursor(54, 70);
    tft.setTextColor(0x0000, 0xFFFF);
    tft.setTextSize(2);
    tft.print(hScore.char3);

    // read the pushbutton state, if pressed exit loop and move onto next character
    buttonValue = digitalRead(JOYSTICK_BUTTON);

    if (buttonValue == LOW && hasBeenpressed != 1) {
        hasBeenpressed = 1;
        tft.setCursor(54, 70);
        tft.setTextColor(0xFFFF, 0x8811);
        tft.setTextSize(2);
        tft.print(hScore.char3);
        break;
    }
    else if (buttonValue == HIGH){
        hasBeenpressed = 0;
    }

    delay(100);
  }

  return hScore;
}

void highscores(){
  int j = 4;
  highscore hScore;
  while (j < 14){
    hScore = readHighscoreEEPROM(j);
    if(score <= hScore.score){
      j++;
    }
    else{
      highscore hScore2;

      hScore.score = score;
      hScore = enter_new_highscore(hScore);
      while (j < 14){
        hScore2 = readHighscoreEEPROM(j);
        storeHighscoreEEPROM(hScore, j);
        hScore = hScore2;
        j++;
      }
      break;
    }
  }

  tft.fillRect(0, 0, 128, 160, 0x8811);
  tft.setCursor(40, 15);
  tft.setTextColor(0x07E0);
  tft.setTextSize(2);
  tft.println("HIGH");
  tft.setCursor(28,34);
  tft.println("SCORES");
  tft.println();

  tft.setTextColor(0xFFFF);
  tft.setTextSize(1.5);
  for (int i = 4; i < 14; i++){
    hScore = readHighscoreEEPROM(i);
    if(hScore.score != 0){
      tft.print("     ");
      tft.print(i-3);
      tft.print(") ");
      tft.print(hScore.char1);
      tft.print(hScore.char2);
      tft.print(hScore.char3);
      tft.print("    ");
      tft.println(hScore.score);
    }
  }
  delay(10000);
}

void main_game() {
    // Initialize snake (so a new snake is made each time)
    SnakeMove head[160];
    int length = 8;

    for(int i; i < 160; i++){
      head[i].x = 0;
      head[i].y = 0;
      head[i].dir = 0;
    }
    SnakeMove fill;

    int gameO = 0;

    int level = 1;
    head[0].x = 64; // initial head position
    head[0].y = 80;
    head[0].dir = 1;
    head[1].x = 64;
    head[1].y = 76;
    head[1].dir = 1;
    score = 0 ;

    int init_vert, init_horiz;
    init_vert    = analogRead(JOYSTICK_VERT); // do auto calibration
    init_horiz = analogRead(JOYSTICK_HORIZ);

    tft.initR(INITR_BLACKTAB);
    tft.fillRect(0, 0, 128, 160, 0x0000);

    coordinates choose_apple = random_apple();

    while(gameO != 1) {
        // Prints apple in random spot
        tft.fillRect(choose_apple.x, choose_apple.y, 4, 4, 0x00FF);

        int vert, horiz, select;

        vert = analogRead(JOYSTICK_VERT);       // will be 0-1023
        horiz = analogRead(JOYSTICK_HORIZ);     // will be 0-1023
        select = digitalRead(JOYSTICK_BUTTON);  // HIGH if not pressed, LOW otherwise

        int vertical = vert;
        int horizontal = horiz;

        //
        if (vert != init_vert) {
            vertical = vert;
            horizontal = init_horiz;
        }

        //
        else if(horiz != init_horiz) {
            horizontal = horiz;
            vertical = init_vert;
        }

        int i;
        int j;
        int buffer_x[length/4];
        int buffer_y[length/4];

        /** going up **/
        if(head[0].dir == 1) {
            i = 0;
            while(i < (length/4)) {
                buffer_x[i] = head[i].x;
                buffer_y[i] = head[i].y;
                i++;
            }

            head[0].y = head[0].y - 4;
            tft.fillRect(head[0].x, head[0].y, 4, 4, 0xFFFF);

            j = 0;
            while(j<(length/4)-1) {
                head[j+1].x = buffer_x[j];
                head[j+1].y = buffer_y[j];
                tft.fillRect(head[j+1].x, head[j+1].y, 4, 4, 0xFFFF);
                j++;
            }
            tft.fillRect(buffer_x[(length/4)-1], buffer_y[(length/4)-1], 4, 4, 0x0000);

            // left
            if((horizontal+10 < init_horiz) && (horizontal-10 < init_horiz)) {
                j = 0;
                while(j<(length/4)-1) {
                    head[j+1].x = buffer_x[j];
                    head[j+1].y = buffer_y[j];
                    tft.fillRect(head[j+1].x, head[j+1].y, 4, 4, 0xFFFF);
                    j++;
                }
                head[0].dir = 3; //the direction will now go left
            }


            //right
            else if((horizontal+10 > init_horiz) && (horizontal-10 > init_horiz)) {
                j = 0;
                while(j<(length/4)-1) {
                    head[j+1].x = buffer_x[j];
                    head[j+1].y = buffer_y[j];
                    tft.fillRect(head[j+1].x, head[j+1].y, 4, 4, 0xFFFF);
                    j++;
                }
                head[0].dir = 4; //the direction will now go to the right
                tft.fillRect(buffer_x[(length/4)-1], buffer_y[(length/4)-1], 4, 4, 0x0000);
            }
            delay(50);
        }

        /** going down **/
        else if(head[0].dir == 2) {
            i = 0;
            while(i < (length/4)) {
                buffer_x[i] = head[i].x;
                buffer_y[i] = head[i].y;
                i++;
            }

            head[0].y = head[0].y + 4;
            tft.fillRect(head[0].x, head[0].y, 4, 4, 0xFFFF);

            j = 0;
            while(j<(length/4)-1) {
                head[j+1].x = buffer_x[j];
                head[j+1].y = buffer_y[j];
                tft.fillRect(head[j+1].x, head[j+1].y, 4, 4, 0xFFFF);
                j++;
            }
            tft.fillRect(buffer_x[(length/4)-1], buffer_y[(length/4)-1], 4, 4, 0x0000);

            //left
            if((horizontal+10 < init_horiz) && (horizontal-10 < init_horiz)) {
                j = 0;
                while(j<(length/4)-1) {
                    head[j+1].x = buffer_x[j];
                    head[j+1].y = buffer_y[j];
                    tft.fillRect(head[j+1].x, head[j+1].y, 4, 4, 0xFFFF);
                    j++;
                }
                head[0].dir = 3; //the direction will now go left
                tft.fillRect(buffer_x[(length/4)-1], buffer_y[(length/4)-1], 4, 4, 0x0000);
            }


            //right
            else if((horizontal+10 > init_horiz) && (horizontal-10 > init_horiz)) {
                j = 0;
                while(j<(length/4)-1) {
                    head[j+1].x = buffer_x[j];
                    head[j+1].y = buffer_y[j];
                    tft.fillRect(head[j+1].x, head[j+1].y, 4, 4, 0xFFFF);
                    j++;
                }
                head[0].dir = 4; //the direction will now go to the right
                tft.fillRect(buffer_x[(length/4)-1], buffer_y[(length/4)-1], 4, 4, 0x0000);
            }
            delay(50);
        }

        /** going left **/
        else if(head[0].dir == 3) {
            i = 0;
            while (i < (length/4)) {
                buffer_x[i] = head[i].x;
                buffer_y[i] = head[i].y;
                i++;
            }

            // move head[0] to the left
            head[0].x = head[0].x - 4;
            tft.fillRect(head[0].x, head[0].y, 4, 4, 0xFFFF);

            //the rest of the body follows
            j = 0;
            while(j<(length/4)-1) {
                head[j+1].x = buffer_x[j];
                head[j+1].y = buffer_y[j];
                tft.fillRect(head[j+1].x, head[j+1].y, 4, 4, 0xFFFF);
                j++;
            }
            tft.fillRect(buffer_x[(length/4)-1], buffer_y[(length/4)-1], 4, 4, 0x0000);

            //up
            if((vertical+10 < init_vert) && (vertical-10 < init_vert)) {
                j = 0;
                while(j<(length/4)-1) {
                    head[j+1].x = buffer_x[j];
                    head[j+1].y = buffer_y[j];
                    tft.fillRect(head[j+1].x, head[j+1].y, 4, 4, 0xFFFF);
                    j++;
                }
                head[0].dir = 1; //the direction will now go up
                tft.fillRect(buffer_x[(length/4)-1], buffer_y[(length/4)-1], 4, 4, 0x0000);
            }

            //down
            else if((vertical+10 > init_vert) && (vertical-10 > init_vert)) {
                j = 0;
                while(j<(length/4)-1) {
                    head[j+1].x = buffer_x[j];
                    head[j+1].y = buffer_y[j];
                    tft.fillRect(head[j+1].x, head[j+1].y, 4, 4, 0xFFFF);
                    j++;
                }
                head[0].dir = 2; //the direction will now go down
                tft.fillRect(buffer_x[(length/4)-1], buffer_y[(length/4)-1], 4, 4, 0x0000);
            }
        delay(50);
        }

        /** going right **/
        else if(head[0].dir == 4) {
            i = 0;
            while (i < (length/4)) {
                buffer_x[i] = head[i].x;
                buffer_y[i] = head[i].y;
                i++;
            }

            // move head[0] to the right
            head[0].x = head[0].x + 4;
            tft.fillRect(head[0].x, head[0].y, 4, 4, 0xFFFF);

            //the rest of the body follows
            j = 0;
            while(j<(length/4)-1) {
                head[j+1].x = buffer_x[j];
                head[j+1].y = buffer_y[j];
                tft.fillRect(head[j+1].x, head[j+1].y, 4, 4, 0xFFFF);
                j++;
            }
            tft.fillRect(buffer_x[(length/4)-1], buffer_y[(length/4)-1], 4, 4, 0x0000);

            //up
            if((vertical+10 < init_vert) && (vertical-10 < init_vert)) {
                j = 0;
                while(j<(length/4)-1) {
                    head[j+1].x = buffer_x[j];
                    head[j+1].y = buffer_y[j];
                    tft.fillRect(head[j+1].x, head[j+1].y, 4, 4, 0xFFFF);
                    j++;
                }
                head[0].dir = 1; //the direction will now go up
                tft.fillRect(buffer_x[(length/4)-1], buffer_y[(length/4)-1], 4, 4, 0x0000);
            }

            //down
            else if((vertical+10 > init_vert) && (vertical-10 > init_vert)) {
                j = 0;
                while(j<(length/4)-1) {
                    head[j+1].x = buffer_x[j];
                    head[j+1].y = buffer_y[j];
                    tft.fillRect(head[j+1].x, head[j+1].y, 4, 4, 0xFFFF);
                    j++;
                }
                head[0].dir = 2; //the direction will now go down
                tft.fillRect(buffer_x[(length/4)-1], buffer_y[(length/4)-1], 4, 4, 0x0000);
            }
            delay(50);
        }

        /**EATING THE APPLE**/
        if (choose_apple.x == head[0].x && choose_apple.y == head[0].y) {
            length = length + 4;
            choose_apple = random_apple();
            score++;
        }

        /**GAME OVER CONDITIONS**/
        if (head[0].x < 0 || head[0].x > 125) {
            gameO = 1;
        }

        if (head[0].y < 0 || head[0].y > 157) {
            gameO = 1;
        }

        for(int i = 1; i < 160; i++){
            if(head[0].x == head[i].x && head[0].y == head[i].y){
              gameO = 1;
            }
        }
        delay(100);
    }
}

void welcome() {

    pinMode(JOYSTICK_BUTTON, INPUT);
    digitalWrite(JOYSTICK_BUTTON, HIGH);

    tft.initR(INITR_BLACKTAB);

    //SNAKE
    tft.fillRect(0, 0, 128, 160, 0x8811);
    tft.fillRect(10, 10, 108, 34, 0xFFFF);
    tft.setCursor(20, 16);
    tft.setTextColor (0x8811);
    tft.setTextSize(3);
    tft.println("SNAKE");

    //PRESS BUTTON TO CONTINUE
    tft.setCursor(29, 51);
    tft.setTextSize(1);
    tft.setTextColor(0xFFFF);
    tft.println("PRESS BUTTON");
    tft.setCursor(32, 62);
    tft.print("TO CONTINUE");

    tft.fillRect(46, 90, 40, 13, 0xFFFF);
    tft.setTextSize(1);
    tft.setTextColor(0x8811);
    tft.setCursor(52, 93);
    tft.print("START");

    while(true) {
        // read the pushbutton state
        buttonValue = digitalRead(JOYSTICK_BUTTON);

        if (buttonValue == LOW) {
            break;
        }


    }
}

void game_over() {
    highscores();

    pinMode(JOYSTICK_BUTTON, INPUT);
    digitalWrite(JOYSTICK_BUTTON, HIGH);

    delay(500);
    tft.fillRect(0, 0, 128, 160, 0x0000);
    tft.setCursor(10, 15);
    tft.setTextColor(0x07E0);
    tft.setTextSize(2);
    tft.println("GAME OVER");

    tft.setCursor(41, 60);
    tft.setTextSize(1.8);
    tft.setTextColor(0xF800);
    tft.print("SCORE: ");
    tft.println(score);

    tft.fillRect(35, 85, 58, 13, 0xFFFF);
    tft.setCursor(38, 88);
    tft.setTextColor(0x0000);
    tft.setTextSize(1.5);
    tft.println("New Game?");

    tft.setCursor(20, 102);
    tft.setTextColor(0xF800);
    tft.setTextSize(1);
    tft.println("Press button to");
    tft.setCursor(40, 111);
    tft.setTextSize(1);
    tft.println("Continue");


    while(true) {
        // read the pushbutton state
        buttonValue = digitalRead(JOYSTICK_BUTTON);

        if (buttonValue == LOW) {
            break;

        }
    }
}

int main() {
    init();
    Serial.begin(9600);
    int eepromInit = 1; // Assume Initialized
    highscore init;

    // Check if Initialized
    for (int i = 0; i < 4; i++){
      init = readHighscoreEEPROM(i);
      if (init.char1 != 'I' || init.char2 != 'N' || init.char3 != 'I'){
        init.char1 = 'I';
        init.char2 = 'N';
        init.char3 = 'I';
        storeHighscoreEEPROM(init, i);
        eepromInit = 0;
      }
    }
    // If not inizalized 'reset' scores
    if (eepromInit == 0){
      for (int i = 4; i < 14; i++){
        // Dosent matter what the stored chars are
        init.char1 = 0;
        init.char2 = 0;
        init.char3 = 0;   // they are reset when a score is set
        init.score = 0;   // Score must be zero though
        storeHighscoreEEPROM(init, i);
      }
    }

    welcome();
    while(true){
      main_game();
      game_over();
    }

    return 0;
}
