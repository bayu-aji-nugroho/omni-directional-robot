#include <Arduino.h>
#include <../lib/movementLIB/movement.h>
#include <HardwareSerial.h>
#include <PS4Controller.h>
#include <nvs_flash.h>

Movement *Fr,*Fl,*Br, *Bl;
boolean show = true;

void setup() {
  nvs_flash_erase(); 
  nvs_flash_init();
  Serial.begin(115200);
  delay(2000);

  PS4.begin("40:1A:58:62:D6:A2");  
  Serial.print("program berjalan");
 
  Fr = new Movement(0.1,0.01,0.1,34,35,1,12,13);
  Fl = new Movement(0.1,0.01,0.1,1,1,1,1,1);
  Bl = new Movement(0.1,0.01,0.1,1,1,1,1,1);
  Br = new Movement(0.1,0.01,0.1,1,1,1,1,1);

  Fr->begin();
  Fl->begin();
  Br->begin();
  Bl->begin();
  
}

void loop() {
  if(PS4.isConnected()){
    control();
  }
  
}

void move(int forward, int strafe, int turn){
  int vfl = forward + strafe + turn; 
  int vfr = forward - strafe - turn; 
  int vbl = forward - strafe + turn;
  int vbr = forward + strafe - turn;
  
  float max_val = std::max({abs(vfl), abs(vfr), abs(vbl), abs(vbr)});

 if (max_val > 255) {
    vfl = (vfl / max_val) * 255;
    vfr = (vfr / max_val) * 255;
    vbl = (vbl / max_val) * 255;
    vbr = (vbr / max_val) * 255;
  }
  float Vfr = map(vfr, -255, 255, -100, 100);  
  float Vfl = map(vfl, -255, 255, -100, 100); 
  float Vbr = map(vbr, -255, 255, -100, 100);  
  float Vbl = map(vbl, -255, 255, -100, 100);  

  Fr->update(Vfr);
  Fl->update(Vfl);
  Br->update(Vbr);
  Bl->update(Vbl);                                    
}

void control(){
  
  if(PS4.Triangle()){
    Serial.println("segitiga di tekan");
  }
  if(PS4.LStickY() || PS4.LStickX() || PS4.RStickX() ){
    move(PS4.LStickY(), PS4.LStickX(),PS4.RStickX());
  }
  if(PS4.Square()){
    Serial.println("kotak di tekan");
  }
  if(PS4.Circle()){
    Serial.println("lingkaran di tekan");   
    delay(200);
  }
}