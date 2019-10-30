int latchPin = 11;
int clockPin = 10;
int dataPin = 12;

int numOfRegisters = 8;
byte* registerState;

int hourHands[] = {
  5,  // 1 hand
  11, // 2 hand
  17, // 3 hand
  24, // 4
  29, // 5 hand
  35, // 6 hand
  41, // 7 hand
  47, // 8 hand
  53, // 9 hand
  59, // 10 hand
  66,    // 11
  71,    // 12
};

int shiftPins[] = {
  56, 57, 58, 59, 60, 61, 62, 63, //1th: 56=1
  48, 49, 50, 51, 52, 53, 54, 55, //2th: 48=9
  40, 41, 42, 43, 44, 45, 46, 47, //3th: 41=17
  32, 33, 34, 35, 36, 37, 38, 39, //4th: 33=25
  16, 17, 18, 19, 20, 21, 22, 23, //5th: 33=16
  24, 25, 26, 27, 28, 29, 30, 31, //6th: 41=24
  0, 1, 2, 3, 4, 5, 6, 7,  //7th: 49=1
  8, 9, 10, 11, 12, 13, 14, 15, //8th: 57=8
};
int shiftPinsCount = 64;

// minuteShiftPins The position in this array maps to the minute hand
// position in the shiftPins array
int minuteShiftPins[] = {
  7, 6, 5, 4, 2, 1, 0,
  15, 14, 12, 11, 10, 9, 8,
  22, 21, 20, 19, 18, 16,
  31, 30,  29, 28, 26, 25, 24,
  47, 46, 44, 43, 42, 41,
  39, 38, 37, 36, 35, 34, 32,
  63, 62, 61, 60, 58, 57, 56,
  55, 54, 52, 51, 50, 49, 48,
};
int maxShiftMinuteHands = 48;

// hourShiftPins maps to the position of the hour hand in the shiftPins array
int hourShiftPins[] = {
  3, 13, 23, 17, 27, 45, 40, 33, 59, 53,
};
int hourShiftPinsCount = 10;

int arduinoPins[] = {
  9, 8, 6, 7, 5, 4, 3, 2,
};
int arduinoPinsCount = 8;

int arduinoPinsMinuteHands[] = {
  6, 5, 4, 3, 1, 0,
};
int arduinoPinsMinuteHandsCount = 6;

int arduinoPinsHourHands[] = {
  2, 7,
};
int arduinoPinsHourHandsCount = 2;

bool isArduinoPinsHourPin(int pin) {
  for (int k = 0; k < hourShiftPinsCount; k++) {
    if (hourShiftPins[k] == pin) {
      return true;
    }
  }
  return false;
}

bool isShiftPinsHourPin(int pin) {
  for (int k = 0; k < hourShiftPinsCount; k++) {
    if (hourShiftPins[k] == pin) {
      return true;
    }
  }
  return false;
}

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);

  //Initialize array
  registerState = new byte[numOfRegisters];
  for (size_t i = 0; i < numOfRegisters; i++) {
    registerState[i] = 0;
  }

  //set pins to output to control the shift register
  pinMode(latchPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  pinMode(dataPin, OUTPUT);


  for (int k = arduinoPinsCount - 1; k >= 0; k--) {
    pinMode(arduinoPins[k], OUTPUT);
  }

}

int previousMinute = 0;
void writeMinute(int clockHandLocation) {
  Serial.print("actual Min:      ");
  Serial.println(clockHandLocation);
  
  // Decrement because clocks start with 1 and array's start at 0
  clockHandLocation--;

  Serial.print("minuteShiftPins: ");
  Serial.println(minuteShiftPins[clockHandLocation]);

  if (clockHandLocation <= 6) {
    digitalWrite(arduinoPins[arduinoPinsMinuteHands[previousMinute]], LOW);
  }
  if (clockHandLocation < 6) {
    digitalWrite( arduinoPins[arduinoPinsMinuteHands[previousMinute]], LOW);
    digitalWrite( arduinoPins[arduinoPinsMinuteHands[clockHandLocation]], HIGH);

    Serial.print("ArduinoPins:       ");
    Serial.println(shiftPins[minuteShiftPins[clockHandLocation]]);
  }

  else {
    // Decrement clock hand location
    clockHandLocation = clockHandLocation - 6;

    // Turn off previous led
    if (shiftPins[minuteShiftPins[previousMinute]] == 34) {
      digitalWrite(13, LOW);
    } else {
      regWrite(shiftPins[minuteShiftPins[previousMinute]], LOW);
    }

    // Turn on new led
    if (shiftPins[minuteShiftPins[clockHandLocation]] == 34) {
      digitalWrite(13, HIGH);
    } else {
      regWrite(shiftPins[minuteShiftPins[clockHandLocation]], HIGH);
      Serial.print("shiftPins:       ");
      Serial.println(shiftPins[minuteShiftPins[clockHandLocation]]);
    }


  }
  Serial.println("----------------");
  // Save previous led so it can be shut off on the next move
  previousMinute = clockHandLocation;
}
//
//void writeHour() {
//
//}

//

void loop() {
  //effectA(300);
  regWrite(59, LOW);
  //digitalWrite(3, HIGH);

  //Serial.println("test");
  //  regWrite(hourHands[2], HIGH);
  //  regWrite(43, HIGH);
  //delay(10000);
  //  shiftPins(k, LOW);

  //  for (int k = 0; k < arduinoPinsMinuteHandsCount; k++) {
  //    Serial.println(k);
  //    digitalWrite(arduinoPins[arduinoPinsMinuteHands[k]], HIGH);
  //    //writeMinute(k);
  //    delay(1000);
  //    digitalWrite(arduinoPins[arduinoPinsMinuteHands[k]], LOW);
  //  }

    // Loop through minute hands
    for (int k = 1; k <= 60; k++) {
      writeMinute(k);
      delay(500);
    }

  // Loop through hour hands
//  for (int k = 0; k <= hourShiftPinsCount; k++) {
//    Serial.println(hourShiftPins[k]);
//
//    regWrite(shiftPins[hourShiftPins[k]], HIGH);
//    delay(3000);
//    regWrite(shiftPins[hourShiftPins[k]], LOW);
//  }

  //    writeMinute(33);
  //    delay(1000);
}

int totalNumberLEDs() {
  return sizeof(shiftPins) + sizeof(arduinoPins);
}

void effectA(int speed) {
  int numPins = numOfRegisters * 8;
  for (int k = numPins - 1; k >= 0; k--) {
    Serial.println(k);
    if (k == 34) {
      digitalWrite(13, HIGH);
      delay(speed);
      digitalWrite(13, LOW);
      continue;
    }

    regWrite(k, HIGH);
    delay(speed);
    regWrite(k, LOW);
  }

  for (int k = arduinoPinsCount - 1; k >= 0; k--) {
    Serial.println(arduinoPins[k]);
    digitalWrite(arduinoPins[k], HIGH);
    delay(speed);
    digitalWrite(arduinoPins[k], LOW);
  }
}



void regWrite(int pin, bool state) {
  int ledLocation = shiftPins[pin];

  //Determines register
  int reg = ledLocation / 8;
  //Determines pin for actual register
  int actualPin = ledLocation - (8 * reg);

  //Begin session
  digitalWrite(latchPin, LOW);

  for (int i = 0; i < numOfRegisters; i++) {
    //Get actual states for register
    byte* states = &registerState[i];

    //Update state
    if (i == reg) {
      bitWrite(*states, actualPin, state);
    }

    //Write
    shiftOut(dataPin, clockPin, MSBFIRST, *states);
  }

  //End session
  digitalWrite(latchPin, HIGH);
}
