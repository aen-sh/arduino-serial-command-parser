#include <Arduino.h>

enum State {
  IDLE,
  BLINKING
};

State currentState = IDLE;

int totalBlinks = 0;
int countBlinks = 0;
unsigned long lastBlinkTime = 0;
bool ledState = false;


const int BUFFER_SIZE = 64;
char inputBuffer[BUFFER_SIZE];
int bufferIndex = 0;

const int LED_PIN = LED_BUILTIN;

void processCommand(char* input);

void setup() {
  Serial.begin(9600);
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Ready. Type something and press Enter");
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n') {
      inputBuffer[bufferIndex] = '\0';
      processCommand(inputBuffer);
      bufferIndex = 0;
    } else if (c != '\r') {
      if (bufferIndex < BUFFER_SIZE - 1) {
        inputBuffer[bufferIndex] = c;
        bufferIndex++;
      } else {
        Serial.println("ERROR: input too long, buffer reset");
        bufferIndex = 0;
      }
    }
  }

  if (currentState == BLINKING) {
    if (millis() - lastBlinkTime >= 200) {
      ledState = !ledState;
      digitalWrite(LED_PIN, ledState);
      lastBlinkTime = millis();
      countBlinks++;
      if (countBlinks >= totalBlinks * 2) {
        currentState = IDLE;
        Serial.println("OK: blink done");
      }
    }
  }
}

void processCommand(char* input) {
  char* command = input;
  char* argument = NULL;

  char* spacePos = strchr(input, ' ');
  if (spacePos != NULL) {
    *spacePos = '\0';
    argument = spacePos + 1;
  }

  if (strcmp(command, "LED") == 0) {
    if (argument == NULL) {
      Serial.println("ERROR: missing argument (ON/OFF)");
    } else if (strcmp(argument, "ON") == 0) {
      digitalWrite(LED_PIN, HIGH);
      currentState = IDLE;
      ledState = true;
      Serial.println("OK: LED is ON");
    } else if (strcmp(argument, "OFF") == 0) {
      digitalWrite(LED_PIN, LOW);
      currentState = IDLE;
      ledState = false;
      Serial.println("OK: LED is OFF");
    } else {
      Serial.println("ERROR: invalid argument, expected ON/OFF");
    }
  } else if (strcmp(command, "BLINK") == 0) {
    if (argument == NULL) {
      Serial.println("ERROR: missing argument (number)");
    } else {
      totalBlinks = atoi(argument);
      if (totalBlinks <= 0) {
        Serial.println("ERROR: invalid argument, expected positive number");
      } else {
        countBlinks = 0;
        lastBlinkTime = millis();
        currentState = BLINKING;

        Serial.println("OK: blink started");
      }
    }

  } else if (strcmp(command, "STATUS") == 0) {
    Serial.print("OK: LED is ");
    Serial.println(digitalRead(LED_PIN) == HIGH ? "ON" : "OFF");

  } else if (strcmp(command, "HELP") == 0) {
    Serial.println("Commands: LED ON, LED OFF, BLINK <n>, STATUS, HELP");

  } else {
    Serial.println("ERROR: unknown command");
  }
}
