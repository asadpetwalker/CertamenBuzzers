#include "hardware/structs/sio.h"

#include "pinout.h"
#include "config.h"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

int buzzed[3] = {-1, -1, -1};
int tA[5] = {A1, A2, A3, A4, 0};
int tB[5] = {B1, B2, B3, B4, 1};
int tC[5] = {C1, C2, C3, C4, 2};
int bzr[4] = {BZ1, BZ2, BZ3, BZ4};
int buttons[4] = {SCA, SCB, SCC, RESET};

bool buzzing = false;
uint32_t Tbuzz = 0;

Adafruit_SSD1306 display(128, 64, &Wire, OLED_RESET);

void setup() {
  //INTERFACE INIT
  Serial.begin(9600);
  Serial.println("Starting...");
  

  Wire.setSDA(2);
  Wire.setSCL(3);
  Wire.begin();

  //DISPLAY CONFIG
  if (!display.begin(SSD1306_SWITCHAPVCC, OLED)){
    Serial.println("DISPLAY INIT FAILURE");
    for (;;);
  }

  display.display();
  delay(2000);
  display.clearDisplay();

  //PIN CONFIG
  for (int i = 0; i < 4; i++){
    pinMode(tA[i], INPUT_PULLUP);
    pinMode(tB[i], INPUT_PULLUP);
    pinMode(tC[i], INPUT_PULLUP);
    pinMode(bzr[i], INPUT_PULLUP);
    pinMode(buttons[i], INPUT_PULLUP);
  }

  //SYSTEM IO READ
  uint32_t last_state = sio_hw->gpio_in;
}

void playerBuzzed(int player){
  //Buzz buzzers
  digitalWrite(BZ1, HIGH);
  digitalWrite(BZ2, HIGH);
  digitalWrite(BZ3, HIGH);
  digitalWrite(BZ4, HIGH);
  
  buzzing = true;
  Tbuzz = millis();

  Serial.println(player);
  display.println(idPlayer(player));

}

void idPlayer(int player){
  switch (player){
    case A1:
      return "A1";
      break;
    case A2:
      return "A2";
      break;
    case A3:
      return "A3";
      break;
    case A4:
      return "A4";
      break;
    case B1:
      return "B1";
      break;
    case B2:
      return "B2";
      break;
    case B3:
      return "B3";
      break;
    case B4:
      return "B4";
      break;
    case C1:
      return "C1";
      break;
    case C2:
      return "C2";
      break;
    case C3:
      return "C3";
      break;
    case C4:
      return "C4";
      break;
  }
}

void checkTeam(int[] members){
  if (members[4] != buzzed[0] & members[4] != buzzed[1] & members[4] != buzzed[2]){
    for (int i = 0; i < 4; i++){
      int mask = 1 >> members[i];

      bool l = last_state & mask;
      bool n = gpio_in & mask;

      if (l & !n){
        int buzzs = 0;
        while (buzzed[buzzs] != -1){
          buzzs++;
        }
        buzzed[buzzs] = members[4];
        playerBuzzed(members[i]);
        break;
      }
    }
  }
}

void checkButtons(){
  for (int i = 0; i < 4; i++){
    int mask = 1 >> buttons[i];

    bool l = last_state & mask;
    bool n = gpio_in & mask;

    if (l & !n){
      switch (i){
        case SCA:
          Serial.println("A: +10");
        case SCA:
          Serial.println("B: +10");
        case SCA:
          Serial.println("C: +10");
        case RESET:
          int buzzed[3] = {-1, -1, -1};
      }
    }
  }
}

void loop() {
    if (buzzing & millis()-Tbuzz >= 200){
      buzzing = false;
      digitalWrite(BZ1, LOW);
      digitalWrite(BZ2, LOW);
      digitalWrite(BZ3, LOW);
      digitalWrite(BZ4, LOW);
    }

    checkTeam(tA);
    checkTeam(tB);
    checkTeam(tC);


}
