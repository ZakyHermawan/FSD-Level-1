#define MAX_BUFF_LEN 255

char c;
char s[MAX_BUFF_LEN];
uint8_t idx = 0;

int desired_velocity = 0;
int desired_brake = 0;
int actual_velocity = 0;

int prevT = 0;
float eprev = 0;

float kp = 1.0;
float ki = 0.0;
float kd = 0.0;
float u;
float eintegral = 0;

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

      // Process the string to extract three integers
      char *token = strtok(s, ",");
      if (token != NULL) {
        desired_velocity = atoi(token);
        token = strtok(NULL, ",");
        if (token != NULL) {
          desired_brake = atoi(token);
          token = strtok(NULL, ",");
          if (token != NULL) {
            actual_velocity = atoi(token);
          }
        }
      }

      // Use the values
      Serial.print("ESP: ");
      Serial.print("Desired Velocity: ");
      Serial.print(desired_velocity);
      Serial.print(", Desired Brake: ");
      Serial.print(desired_brake);
      Serial.print(", Actual Velocity: ");
      Serial.println(actual_velocity);

      // PID control
      int currentTime = millis();
      int deltaTime = currentTime - prevT;
      prevT = currentTime;

      float error = desired_velocity - actual_velocity;
      eintegral += error * deltaTime;
      float derivative = (error - eprev) / deltaTime;

      u = kp * error + ki * eintegral + kd * derivative;

      eprev = error;

      // Ensure the control signal is within the DAC range
      if (u > 154) u = 154;
      if (u < 131) u = 131;

      // Write the control signal to the DAC
      dacWrite(25, (int)u);
      dacWrite(26, (int)u+62);
      
      // // Write to DAC pins
      // dacWrite(25, value1);
      // dacWrite(26, value2);


    }
  }
}
