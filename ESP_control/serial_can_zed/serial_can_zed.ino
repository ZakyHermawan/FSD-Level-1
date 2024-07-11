#define MAX_BUFF_LEN 255

char c;
char s[MAX_BUFF_LEN];
uint8_t idx = 0;

int value1 = 0;
int value2 = 0;

void setup() {
  Serial.begin(115200);
  pinMode(25, OUTPUT);
  pinMode(26, OUTPUT);
}

void loop() {
  if (Serial.available() > 0) {
    c = Serial.read();
    if (c != '\n' && idx < MAX_BUFF_LEN - 1) {  // Ensure buffer does not overflow
      s[idx++] = c;
    } else {
      s[idx] = '\0';  // Null-terminate the string
      idx = 0;
      // Process the string to extract two integers
      char *token = strtok(s, ",");
      if (token != NULL) {
        value1 = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) {
          value2 = atoi(token);
        }
      }

      // Use the values
      Serial.print("ESP: ");
      Serial.print(value1);
      Serial.print(", ");
      Serial.println(value2);

      // Write to DAC pins
      dacWrite(25, value1);
      dacWrite(26, value2);
    }
  }
}
