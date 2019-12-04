// ----------------------------------------------------------
// *** UPDATE THE START TIME HERE ***
int currentHour = 6;
int currentMinute = 21;
// ----------------------------------------------------------

int latchPin = 11;
int clockPin = 10;
int dataPin = 12;

int numOfRegisters = 8;
byte* registerState;

int hourHands[] = {
  5,  // 1
  11, // 2
  17, // 3
  24, // 4
  29, // 5
  35, // 6
  41, // 7
  47, // 8
  53, // 9
  59, // 10
  66, // 11
  71, // 12
};

int shiftPins[] = {
  56, 57, 58, 59, 60, 61, 62, 63, //1th: 56=1
  48, 49, 50, 51, 52, 53, 54, 55, //2th: 48=9
  40, 41, 42, 43, 44, 45, 46, 47, //3th: 41=17
  32, 33, 34, 35, 36, 37, 38, 39, //4th: 33=25
  16, 17, 18, 19, 20, 21, 22, 23, //5th: 33=16
  24, 25, 26, 27, 28, 29, 30, 31, //6th: 41=24
  0, 1, 2, 3, 4, 5, 6, 7,         //7th: 49=1
  8, 9, 10, 11, 12, 13, 14, 15,   //8th: 57=8
};

// minuteShiftPins The position in this array maps to the minute hand
// position in the shiftPins array
int minuteShiftPins[] = {
  7, 6, 5, 4, 2, 1, 0,          // 7
  15, 14, 12, 11, 10, 9, 8,     // 7
  22, 21, 20, 19, 18, 16,       // 6
  31, 30,  29, 28, 26, 25, 24,  // 7
  47, 46, 44, 43, 42, 41,       // 6
  39, 38, 37, 36, 35, 34, 32,   // 7
  63, 62, 61, 60, 58, 57, 56,   // 7
  55, 54, 52, 51, 50, 49, 48,   // 7
};

// hourShiftPins maps to the position of the hour hand in the shiftPins array
int hourShiftPins[] = {
  3, 13, 23, 17, 27, 45, 40, 33, 59, 53,
};

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


void setup() {
  // NOTE: ONLY ENABLE THIS FOR DEBUGGING
  // Open serial communications and wait for port to open:
  //Serial.begin(9600);

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

  // Startup
  startupSequence();
  startupSequence();
}

int previousMinute = 0;
void writeMinute(int clockHandLocation) {
  Serial.print("Min:      ");
  Serial.println(clockHandLocation);

  // Decrement because clocks start with 1 and array's start at 0
  clockHandLocation--;

  // Turn off previous pin
  if (previousMinute < 6) {
    digitalWrite(arduinoPins[arduinoPinsMinuteHands[previousMinute]], LOW);
  } else {
    // Decrement the number of pins used on the board
    int realLocation = previousMinute - arduinoPinsMinuteHandsCount;

    // Turn off previous led
    if (shiftPins[minuteShiftPins[realLocation]] == 34) {
      digitalWrite(13, LOW);
    } else {
      regWrite(shiftPins[minuteShiftPins[realLocation]], LOW);
    }
  }

  // Turn on Next LED
  if (clockHandLocation < 6) {
    digitalWrite( arduinoPins[arduinoPinsMinuteHands[clockHandLocation]], HIGH);
  } else {
    // Decrement the number of pins used on the board
    int realLocation = clockHandLocation - arduinoPinsMinuteHandsCount;

    // Turn on new led
    if (shiftPins[minuteShiftPins[realLocation]] == 34) {
      digitalWrite(13, HIGH);
    } else {
      regWrite(shiftPins[minuteShiftPins[realLocation]], HIGH);
    }
  }

  // Save previous led so it can be shut off on the next move
  previousMinute = clockHandLocation;
}

int previousHour = 0;
void writeHour(int clockHandLocation) {
  Serial.print("Hour:      ");
  Serial.println(clockHandLocation);

  // Decrement because clocks start with 1 and array's start at 0
  clockHandLocation--;

  // Turn off previous pin
  if (previousHour == 0 ) {
    digitalWrite(arduinoPins[arduinoPinsHourHands[previousHour]], LOW);
  }
  if (previousHour == 11) {
    digitalWrite(arduinoPins[arduinoPinsHourHands[1]], LOW);
  }
  if (previousHour >= 1 && previousHour <= 10) {
    int real = previousHour - 1;
    // Turn off previous led
    regWrite(shiftPins[hourShiftPins[real]], LOW);
  }

  if (clockHandLocation < 1) {
    digitalWrite(arduinoPins[arduinoPinsHourHands[clockHandLocation]], HIGH);
  }

  if (clockHandLocation >= 1 && clockHandLocation <= 10) {
    // Decrement by 1 since there the 1 hour hand is hooked up the arduino
    int realLocation = clockHandLocation - 1;

    regWrite(shiftPins[hourShiftPins[realLocation]], HIGH);

  }

  if (clockHandLocation == 11) {
    digitalWrite(arduinoPins[arduinoPinsHourHands[1]], HIGH);
  }

  previousHour = clockHandLocation;
}

void writeTime(int hour, int minute) {
  writeMinute(minute);
  writeHour(hour);
}

void startupSequence() {
  for (int minute = 1; minute <= 60; minute++) {
    if (minute % 5 == 0) {
      writeHour(minute / 5);
    }
    writeMinute(minute);
    delay(20);
  }
  for (int minute = 60; minute >= 1; minute--) {
    if (minute % 5 == 0) {
      writeHour(minute / 5);
    }
    writeMinute(minute);
    delay(20);
  }
}

long second = (long)1000;
long minute = (long)60 * second;

void loop() {

  // Write a random pin low to turn off all LEDs managed by the shift register
  //  regWrite(59, LOW);
  for (; currentHour <= 12; currentHour++) {
    for (; currentMinute <= 60; currentMinute++) {
      writeTime(currentHour, currentMinute);
      delay(minute);
    }
    writeTime(currentHour, 1);
  }

  // Reset the time
  currentHour = 1;
  currentMinute = 1;
}

// Generic function to write to the shift registers
void regWrite(int pin, bool state) {
  int ledLocation = shiftPins[pin];

  // Determines register
  int reg = ledLocation / 8;
  //Determines pin for actual register
  int actualPin = ledLocation - (8 * reg);

  // Begin session
  digitalWrite(latchPin, LOW);

  for (int i = 0; i < numOfRegisters; i++) {
    // Get actual states for register
    byte* states = &registerState[i];

    // Update state
    if (i == reg) {
      bitWrite(*states, actualPin, state);
    }

    // Write
    shiftOut(dataPin, clockPin, MSBFIRST, *states);
  }

  // End session
  digitalWrite(latchPin, HIGH);
}
