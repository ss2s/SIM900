#define GSM Serial
#define LEO Serial1
#define LED 13
#define KNOPKA 7 // ПИН ПОДКЛЮЧЕНИЯ КНОПКИ
#define MODEMBOOT 9

void ring(){
  GSM.println("ATD+380965116553;"); //// НОМЕР ТЕЛЕФОНА НА КОТОРЫЙ БУДЕТ ЗВОНИТЬ ////
   
  delay(2000);
}

void modemcheck(){

  GSM.println("AT");

  delay(100);

  if (!GSM.find("OK")){
    digitalWrite(MODEMBOOT, HIGH);            
    delay(1000);
    digitalWrite(MODEMBOOT, LOW);
  }
  delay(2000);    
  
  GSM.println("ATE0"); 
   
  while(1){
      GSM.println("AT+COPS?");
      if (GSM.find("+COPS: 0")) break;
 
  }
}
 
void setup() {

  GSM.begin(9600);


  delay(2000);

  pinMode(KNOPKA, INPUT_PULLUP);
  pinMode(LED, OUTPUT);
  pinMode(MODEMBOOT, OUTPUT);

  modemcheck();
}
 
void loop() {
    
   bool chk = digitalRead(KNOPKA);

  if(chk == LOW)
  {
    modemcheck();
    digitalWrite(LED, HIGH);
    ring();
  }
  else
  {
    digitalWrite(LED, LOW);
    delay(50);             // задержка. в это время кнопка не распознается
  }

  if (LEO.available()>0){
    int inByte = LEO.read();
    GSM.write(inByte);
  }

  if (GSM.available()>0){
    int inByte = GSM.read();
    LEO.write(inByte);
  } 

  if (digitalRead(LED)>0){
    GSM.println("AT+CPAS");

    delay(100);

    if (!GSM.find("4")){
      delay(100);
      digitalWrite(LED, LOW);
    }
  }
}
 

