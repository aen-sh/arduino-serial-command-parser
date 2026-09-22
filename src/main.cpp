#include <Arduino.h>

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
      Serial.println("ERROR: missing argument (ON\OFF)");
    } else if (strcmp(argument, "ON") == 0) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("OK: LED is ON");
    } else if (strcmp(argument, "OFF") == 0) {
      digitalWrite(LED_PIN, LOW);
      Serial.println("OK: LED is OFF");
    } else {
      Serial.println("ERROR: invalid argument, expected ON/OFF");
    }
  } else if (strcmp(command, "BLINK") == 0) {
    if (argument == NULL) {
      Serial.println("ERROR: missing argument (number)");
    } else {
      int  times = atoi(argument);
      if (times <= 0) {
        Serial.println("ERROR: invalid argument, expected positive number");
      } else {
        for (int i = 0; i < times; i++) {
          digitalWrite(LED_PIN, HIGH);
          delay(200);
          digitalWrite(LED_PIN, LOW);
          delay(200);
        }
        Serial.println("OK: blink done");
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