 
 
 #include <CrcLib.h> //initialize Crc library
 
 #define MC_1 CRC_PWM_1  //Motor control pin name 
 
 
 
 //Program execution variable
 bool motorRotating = LOW; // will allow Motor_1 rotation

void setup() {
  // put your setup code here, to run once:
  CrcLib::Initialize();
  Serial.begin(9600);
  CrcLib::InitializePwmOutput(CRC_PWM_9);
  CrcLib::InitializePwmOutput(CRC_PWM_10);
  CrcLib::InitializePwmOutput(CRC_PWM_11);
  CrcLib::InitializePwmOutput(CRC_PWM_12);
  CrcLib::InitializePwmOutput(CRC_PWM_1, 500, 2500);
  CrcLib::InitializePWmOutput(CRC_PWM_6);

  if (CrcLib::IsCommValid()){
    printf("Started Robot");
  } else{
    printf("Controller is not connected");
  } 

}

void loop() {
  // put your main code here, to run repeatedly:
 CrcLib::Update();
 //Closing claw
 if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_RIGHT)==HIGH){
  CrcLib::SetPwmOutput(CRC_PWM_6, 64);
  printf("Closing claw");
 } 

 //Opening claw
  if (CrcLib::ReadDigitalChannel(BUTTON::COLORS_LEFT)==HIGH){
    CrcLib::SetPwMOutput(CRC_PWM_6, -64);
    printf("Opening claw");
  }  
  



  CrcLib::MoveHolonomic(ANALOG::JOYSTICK1_Y, ANALOG::JOYSTICK1_X, ANALOG::JOYSTICK2_X, CRC_PWM_1, CRC_PWM_2, CRC_PWM_3, CRC_PWM_4);

  //Reset motors
  CrcLib::SetPwmOutput(CRC_PWM_6,0);
  CrcLib::SetPwmOutput(CRC_PWM_1,0);
}
