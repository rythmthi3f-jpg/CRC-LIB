 
 
 #include <CrcLib.h> //initialize Crc library
 
 
 
 //Program execution variable
 bool motorRotating = LOW; // will allow Motor_1 rotation

void setup() {
  // put your setup code here, to run once:
  CrcLib::Initialize();
  Serial.begin(9600);
  CrcLib::InitializePwmOutput(CRC_PWM_9); //Top left motor
  CrcLib::InitializePwmOutput(CRC_PWM_10); //Bottom left motor
  CrcLib::InitializePwmOutput(CRC_PWM_11); //Top right motor 
  CrcLib::InitializePwmOutput(CRC_PWM_12); //Bottom right motor
  CrcLib::InitializePwmOutput(CRC_PWM_1, 500, 2500); //For claw?
  CrcLib::InitializePwmOutput(CRC_PWM_6); //Lift motor



  //Check whether a remote is successfully connected
  if (CrcLib::IsCommValid()){
    printf("Started Robot");
  } else{
    printf("Controller is not connected");
    CrcLib::SetPwmOutput(CRC_PWM_9, 0);
    CrcLib::SetPwmOutput(CRC_PWM_10, 0);
    CrcLib::SetPwmOutput(CRC_PWM_11, 0);
    CrcLib::SetPwmOutput(CRC_PWM_12, 0);
    CrcLib::SetPwmOutput(CRC_PWM_1, 0);
    CrcLib::SetPwmOutput(CRC_PWM_6, 0);
  } 

}

void loop() {
  // put your main code here, to run repeatedly:
 CrcLib::Update();

 //raising the claw
 if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT)==HIGH){
  CrcLib::SetPwmOutput(CRC_PWM_6, 128);  
  printf("Lowering claw");
 } 

 //lowering the claw
  if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT)==HIGH){
    CrcLib::SetPwmOutput(CRC_PWM_6, -127);
    printf("Raising claw");
  }  
  


  
 //Closing claw
 if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP)==HIGH){
  CrcLib::SetPwmOutput(CRC_PWM_1, 64);
  printf("Closing claw");
 }

 //Opening claw
 if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_UP)==HIGH){
  CrcLib::SetPwmOutput(CRC_PWM_1, -64);
  printf("Opening claw");
 }



  //Movement system
  CrcLib::MoveHolonomic(ANALOG::JOYSTICK1_X, ANALOG::JOYSTICK1_Y, ANALOG::JOYSTICK2_Y, CRC_PWM_11, CRC_PWM_12, CRC_PWM_9, CRC_PWM_10);
  // CrcLib::MoveArcade(ANALOG::JOYSTICK1_X, ANALOG::JOYSTICK1_Y, CRC_PWM_11, CRC_PWM_9);


  //Resetting the motors if buttons aren't being pressed 
  if (((CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT)) == LOW) && (CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT) == LOW)) {
    CrcLib::SetPwmOutput(CRC_PWM_6, 0);
  }
    

}
