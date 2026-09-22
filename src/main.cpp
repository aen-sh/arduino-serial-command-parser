#include <Arduino.h>

const int BUFFER_SIZE = 64;
char inputBuffer[BUFFER_SIZE];
int bufferIndex = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("Ready. Type something and press Enter");
}

void loop() {
  while (Serial.available() > 0) {
    char c = Serial.read();

    if (c == '\n') {
      inputBuffer[bufferIndex] = '\0';

      Serial.print("You typed: ");
      Serial.println(inputBuffer);

      bufferIndex = 0;
    } else if (c != '\r') {
      if (bufferIndex < BUFFER_SIZE - 1) {
        inputBuffer[bufferIndex] = c;
        bufferIndex++;
        Serial.print(c);
      } else {
        Serial.println("ERROR: Input too long, buffer reset");
        bufferIndex = 0;
      }
    }
  }
}