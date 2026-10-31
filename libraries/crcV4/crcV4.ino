#include <CrcLib.h> //initialize Crc library
  
bool motorRotating = LOW; // will allow Motor_1 rotation
bool LowPower = false;
bool IsPressing = false;
int Low_Bound;
int High_Bound;
int clawOpenCloseTarget = 0;
int clawRotateTarget = 0;
const int AXIS_DEADZONE = 8;
const int SERVO_STEP_MAX = 3;

void setup() {
  
  // put your setup code here, to run once:
  CrcLib::Initialize();
  Serial.begin(9600);
  CrcLib::InitializePwmOutput(CRC_PWM_1);  //For front left motor
  CrcLib::InitializePwmOutput(CRC_PWM_2); //For back left motor
  CrcLib::InitializePwmOutput(CRC_PWM_3); //For front right motor
  CrcLib::InitializePwmOutput(CRC_PWM_4); //For back right motor
  CrcLib::SetPwmOutput(CRC_PWM_1, 0);
  CrcLib::SetPwmOutput(CRC_PWM_2, 0);
  CrcLib::SetPwmOutput(CRC_PWM_3, 0);
  CrcLib::SetPwmOutput(CRC_PWM_4, 0);
  
  CrcLib::InitializePwmOutput(CRC_PWM_9, 500, 2500); // Claw rotation servo
  CrcLib::InitializePwmOutput(CRC_PWM_11, 500, 2000); // Claw open/close servo
  CrcLib::InitializePwmOutput(CRC_PWM_5, 0); // Claw up /down
  
  if (CrcLib::IsCommValid()){
    Serial.println("Started Robot");
  } else{
    Serial.println("Controller is not connected");
  } 

}

void loop() {
  
 CrcLib::Update();
 
 if (CrcLib::IsCommValid()){
    Serial.println("Started Robot");
     int clawUpDown = 0;
  int triggerL = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_L);
  int triggerR = CrcLib::ReadAnalogChannel(ANALOG::GACHETTE_R);

   if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT)==HIGH){
    clawUpDown = -128;
    Serial.println("claw up");
   } 

  if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT)==HIGH){
    clawUpDown = 127;
    Serial.println("claw down");
  } 

  if (CrcLib::ReadDigitalChannel(BUTTON::L1) == HIGH) {
    if (triggerL > AXIS_DEADZONE) {
      int step = map(triggerL, AXIS_DEADZONE, 127, 1, SERVO_STEP_MAX);
      clawOpenCloseTarget += step;
    }
    if (triggerR > AXIS_DEADZONE) {
      int step = map(triggerR, AXIS_DEADZONE, 127, 1, SERVO_STEP_MAX);
      clawOpenCloseTarget -= step;
    }
  }

  if (CrcLib::ReadDigitalChannel(BUTTON::R1) == HIGH) {
    if (triggerL > AXIS_DEADZONE) {
      int step = map(triggerL, AXIS_DEADZONE, 127, 1, SERVO_STEP_MAX);
      clawRotateTarget += step;
    }
    if (triggerR > AXIS_DEADZONE) {
      int step = map(triggerR, AXIS_DEADZONE, 127, 1, SERVO_STEP_MAX);
      clawRotateTarget -= step;
    }
  }

  clawOpenCloseTarget = constrain(clawOpenCloseTarget, -128, 127);
  clawRotateTarget = constrain(clawRotateTarget, -128, 127);

  Serial.print("OpenClose=");
  Serial.print(clawOpenCloseTarget);
  Serial.print(" Rotate=");
  Serial.println(clawRotateTarget);

  CrcLib::SetPwmOutput(CRC_PWM_9, clawOpenCloseTarget);
  CrcLib::SetPwmOutput(CRC_PWM_5, clawUpDown);
  CrcLib::SetPwmOutput(CRC_PWM_11, clawRotateTarget);
  
  if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP) == HIGH) { //If Pressing Y Button and not in low power mode
    if (IsPressing == false){
      IsPressing = true;
      if (LowPower == false){
      LowPower = true; 
      Low_Bound = -64;
      High_Bound= 64;
       }
    }
  }
  if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP) == HIGH) { //If Pressing Y Button and in low power mode
     if (IsPressing == false){
       if (LowPower == true) {
          LowPower = false;
          Low_Bound = -128;
          High_Bound = 127;
       }
     
     }
    }





  int forward  = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_X); // forward/backward
  int rotation = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK1_Y); // yaw rotation
  int strafe   = CrcLib::ReadAnalogChannel(ANALOG::JOYSTICK2_X); // strafe left/right

  int frontLeft  = forward - strafe - rotation;
  int frontRight = forward - strafe + rotation;
  int backLeft   = forward + strafe - rotation;
  int backRight  = forward + strafe + rotation;

  frontLeft = constrain(frontLeft,  Low_Bound, High_Bound);
  frontRight = constrain(frontRight, Low_Bound, High_Bound);
  backLeft = constrain(backLeft,   Low_Bound, High_Bound);
  backRight = constrain(backRight,  Low_Bound, High_Bound);


  CrcLib::SetPwmOutput(CRC_PWM_1,  frontLeft); 
  CrcLib::SetPwmOutput(CRC_PWM_2, backLeft); 
  CrcLib::SetPwmOutput(CRC_PWM_3, frontRight); 
  CrcLib::SetPwmOutput(CRC_PWM_4, backRight); 

  } else{
    Serial.println("Controller is not connected");
    CrcLib::SetPwmOutput(CRC_PWM_1, 0);
    CrcLib::SetPwmOutput(CRC_PWM_2, 0);
    CrcLib::SetPwmOutput(CRC_PWM_3, 0);
    CrcLib::SetPwmOutput(CRC_PWM_4, 0);
    CrcLib::SetPwmOutput(CRC_PWM_5, 0);
    CrcLib::SetPwmOutput(CRC_PWM_9, 0);
    CrcLib::SetPwmOutput(CRC_PWM_11, 0);
  } 

 

}
