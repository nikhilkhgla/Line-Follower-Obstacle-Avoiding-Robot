#define enA 10   // Enable1 L298 Pin enA 
#define in1 9    // Motor1 L298 Pin in1 
#define in2 8    // Motor1 L298 Pin in2 
#define in3 7    // Motor2 L298 Pin in3 
#define in4 6    // Motor2 L298 Pin in4 
#define enB 5    // Enable2 L298 Pin enB 
#define L_S A0   // IR sensor Left
#define R_S A1   // IR sensor Right
#define echo A2  // Echo pin for Ultrasonic Sensor
#define trigger A3 // Trigger pin for Ultrasonic Sensor
#define servo A5 // Servo control pin

int Set = 15;
int distance_L, distance_F, distance_R; 

void setup(){ 
    Serial.begin(9600); // start serial communication at 9600bps

    // Pin setup
    pinMode(R_S, INPUT); 
    pinMode(L_S, INPUT); 
    pinMode(echo, INPUT);
    pinMode(trigger, OUTPUT);
    pinMode(enA, OUTPUT);
    pinMode(in1, OUTPUT);
    pinMode(in2, OUTPUT);
    pinMode(in3, OUTPUT);
    pinMode(in4, OUTPUT);
    pinMode(enB, OUTPUT);
    pinMode(servo, OUTPUT);

    // Set motor speed to 50%
    analogWrite(enA, 128); // Motor1 speed
    analogWrite(enB, 128); // Motor2 speed

    // Servo initialization
    for (int angle = 70; angle <= 140; angle += 5) {
        servoPulse(servo, angle);
    }
    for (int angle = 140; angle >= 0; angle -= 5) {
        servoPulse(servo, angle);
    }
    for (int angle = 0; angle <= 70; angle += 5) {
        servoPulse(servo, angle);
    }

    distance_F = Ultrasonic_read();
    delay(500);
}

void loop(){  
    // Line Follower and Obstacle Avoiding
    distance_F = Ultrasonic_read();
    Serial.print("D F="); Serial.println(distance_F);

    if ((digitalRead(R_S) == 0) && (digitalRead(L_S) == 0)) {
        if (distance_F > Set) {
            forward();
        } else {
            Check_side();
        }  
    } else if ((digitalRead(R_S) == 1) && (digitalRead(L_S) == 0)) {
        turnRight();  
    } else if ((digitalRead(R_S) == 0) && (digitalRead(L_S) == 1)) {
        turnLeft();  
    }

    delay(10);
}

void servoPulse(int pin, int angle) {
    int pwm = (angle * 11) + 500; // Convert angle to microseconds
    digitalWrite(pin, HIGH);
    delayMicroseconds(pwm);
    digitalWrite(pin, LOW);
    delay(50); // Refresh cycle of servo
}

long Ultrasonic_read() {
    digitalWrite(trigger, LOW);
    delayMicroseconds(2);
    digitalWrite(trigger, HIGH);
    delayMicroseconds(10);
    long time = pulseIn(echo, HIGH);
    return time / 29 / 2;
}

void compareDistance() {
    if (distance_L > distance_R) {
        turnLeft();
        delay(500);
        forward();
        delay(600);
        turnRight();
        delay(500);
        forward();
        delay(600);
        turnRight();
        delay(400);
    } else {
        turnRight();
        delay(500);
        forward();
        delay(600);
        turnLeft();
        delay(500);
        forward();
        delay(600);  
        turnLeft();
        delay(400);
    }
}

void Check_side() {
    Stop();
    delay(100);

    for (int angle = 70; angle <= 140; angle += 5) {
        servoPulse(servo, angle);
    }
    delay(300);
    distance_R = Ultrasonic_read();
    Serial.print("D R="); Serial.println(distance_R);
    delay(100);

    for (int angle = 140; angle >= 0; angle -= 5) {
        servoPulse(servo, angle);
    }
    delay(500);
    distance_L = Ultrasonic_read();
    Serial.print("D L="); Serial.println(distance_L);
    delay(100);

    for (int angle = 0; angle <= 70; angle += 5) {
        servoPulse(servo, angle);
    }
    delay(300);
    compareDistance();
}

void forward() {  // Forward
    digitalWrite(in1, LOW); 
    digitalWrite(in2, HIGH); 
    digitalWrite(in3, HIGH); 
    digitalWrite(in4, LOW); 
}

void backward() { // Backward
    digitalWrite(in1, HIGH); 
    digitalWrite(in2, LOW); 
    digitalWrite(in3, LOW); 
    digitalWrite(in4, HIGH); 
}

void turnRight() { // Turn Right
    digitalWrite(in1, LOW); 
    digitalWrite(in2, HIGH); 
    digitalWrite(in3, LOW); 
    digitalWrite(in4, HIGH); 
}

void turnLeft() { // Turn Left
    digitalWrite(in1, HIGH); 
    digitalWrite(in2, LOW); 
    digitalWrite(in3, HIGH); 
    digitalWrite(in4, LOW); 
}

void Stop() { // Stop
    digitalWrite(in1, LOW); 
    digitalWrite(in2, LOW); 
    digitalWrite(in3, LOW); 
    digitalWrite(in4, LOW); 
}
