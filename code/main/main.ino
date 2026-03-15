#include "hardware/structs/sio.h"

#include "pinout.h"
#include "config.h"

volatile int An = -1;
volatile int Bn = -1;
volatile int Cn = -1;

int buzzed[3] = {-1, -1, -1};
int tA[5] = {A1, A2, A3, A4, 0};
int tB[5] = {B1, B2, B3, B4, 1};
int tC[5] = {C1, C2, C3, C4, 2};

bool buzzing = false;
unsigned long Tbuzz = 0;

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < 4; i++){
    pinMode(tA[i], INPUT_PULLUP);
    pinMode(tB[i], INPUT_PULLUP);
    pinMode(tC[i], INPUT_PULLUP);
  }

  pinMode(RESET, INPUT_PULLUP);
  
  pinMode(BZ1, OUTPUT);
  pinMode(BZ2, OUTPUT);
  pinMode(BZ3, OUTPUT);
  pinMode(BZ4, OUTPUT);

  uint32_t last_state = sio_hw->gpio_in;
}

void buzzed(int player){
  //Buzz buzzers
  digitalWrite(BZ1, HIGH);
  digitalWrite(BZ2, HIGH);
  digitalWrite(BZ3, HIGH);
  digitalWrite(BZ4, HIGH);
  
  bool buzzing = true;
  Tbuzz = millis();

  Serial.println(player);

}

void checkTeam(int[] members){
  if (members[-1] != buzzed[0] & members[-1] != buzzed[1] & members[-1] != buzzed[2]){
    for (int i = 0; i < members.length-1; i++){
      int mask = 1 >> members[i];

      bool prev = last_state & mask;
      bool curr = gpio_in & mask;

      if (prev & !curr){
        int buzzs = 0;
        while (buzzed[buzzs] != -1){
          buzzs++;
        }
        buzzed[buzzs] = members[-1];
        buzzed(members[i]);
        break;
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
