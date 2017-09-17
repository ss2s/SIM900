#define GSM Serial
#define LEO Serial1
#define LED 13
#define KNOPKA 7 // ПИН ПОДКЛЮЧЕНИЯ КНОПКИ
#define MODEMBOOT 9

unsigned long pulsVal = 5000; // ДЛИТЕЛЬНОСТЬ ИМПУЛЬСА В мкс. ДЛИТЕЛЬНОСТЬ НАЖАТИЯ ДЛЯ СРАБОТКИ КНОПКИ
unsigned long duration;
unsigned long durationTimeout = 50000000; // ДЛИТЕЛЬНОСТЬ ТАЙМАУТА (ДОЛЖНА БЫТЬ В 50 И БОЛЕЕ РАЗ БОЛЬШЕ ЧЕМ ДЛИТЕЛЬНОСТЬ ИМПУЛЬСА)
boolean lastLed;

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
  digitalWrite(LED, LOW);

  modemcheck();
}
 
void loop() {

  duration = pulseIn(KNOPKA, LOW, durationTimeout);

  if (pulsVal < duration){

    modemcheck();
    digitalWrite(LED, HIGH);
    ring();
  }
    
  // bool chk = digitalRead(KNOPKA);

  // if(chk == LOW)
  // {
  //   modemcheck();
  //   digitalWrite(LED, HIGH);
  //   ring();
  // }

  if (LEO.available()>0){
    int inByte = LEO.read();
    GSM.write(inByte);
  }

  if (GSM.available()>0){
    int inByte = GSM.read();
    LEO.write(inByte);
  } 

  if (digitalRead(LED)>0){

    delay(200);   // ЕСЛИ БУДЕТ ВЫКЛЮЧАТЬ РЕЛЕ ДО ТОГО МОМЕНТА
                  // КАК ВЗЯЛИ ТРУБКУ. УВЕЛИЧИТЬ ЗАДЕРЖКУ НА ВРЕМЯ
                  // КОТОРОЕ ИДУТ ГУДКИ (ЕСЛИ РАБОТАЕТ ЛУЧШЕ НЕ ТРОГАТЬ)

    GSM.println("AT+CPAS");

    delay(200);

    if (!GSM.find("4")){
      delay(1000);
      digitalWrite(LED, LOW);
    }
  }
}
 

