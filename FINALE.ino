#include <Wire.h>
#include <AFMotor.h>
#include <Servo.h>

AF_Stepper stepper1(200, 1);  
AF_Stepper stepper2(200, 2);  


Servo cameraServo;            
Servo additionalServo;       

void setup() {
  Wire.begin(4);              
  Wire.onReceive(receiveEvent);
  Wire.onRequest(requestEvent);
  Serial.begin(9600);
  
  stepper1.setSpeed(10);      
  stepper2.setSpeed(10);      
  cameraServo.attach(9);     
  additionalServo.attach(10); 
  
  Serial.println("Setup  is complete");
}

void loop() {
  //waiting for I2C commands
  delay(500);
}

void receiveEvent(int howMany) {
  while (Wire.available() > 0) {
    int command = Wire.read();
    Serial.print("Command received: ");
    Serial.println(command); 

    switch(command) {
      case I2C_MOVE_UP:
        stepper2.step(100, FORWARD, SINGLE); 
        break;
      case I2C_MOVE_DOWN:
        stepper2.step(100, BACKWARD, SINGLE); 
        break;
      case I2C_ROTATE_LEFT:
        stepper1.step(100, BACKWARD, SINGLE); 
        break;
      case I2C_ROTATE_RIGHT:
        stepper1.step(100, FORWARD, SINGLE);
        break;
      case I2C_ROTATE_UP:
        cameraServo.write(180);  
        break;
      case I2C_ROTATE_DOWN:
        cameraServo.write(0);    
        break;
      case I2C_TOGGLE_CAMERA:
        additionalServo.write(90); 
        delay(500);               
        additionalServo.write(0); 
        break;
      case I2C_STOP:
        stepper1.release(); 
        stepper2.release();  
        cameraServo.write(90);   
        additionalServo.write(90); 
        break;
      default:
        Serial.println("Unknown command");
        break;
    }
  }
}

void requestEvent() {
  static int y = 0;  
  Wire.write(y);  
  loop_y(); 

}
