#define GSM Serial
#define LED 13
#define KNOPKA 7 // ПИН ПОДКЛЮЧЕНИЯ КНОПКИ
#define MODEMBOOT 9

// защита от наводок переменного тока 5 мс.
unsigned long pulsVal = 5000; // ДЛИТЕЛЬНОСТЬ ИМПУЛЬСА В мкс. ДЛИТЕЛЬНОСТЬ НАЖАТИЯ ДЛЯ СРАБОТКИ КНОПКИ
unsigned long duration;
unsigned long durationTimeout = 50000000; // ДЛИТЕЛЬНОСТЬ ТАЙМАУТА (ДОЛЖНА БЫТЬ В 50 И БОЛЕЕ РАЗ БОЛЬШЕ ЧЕМ ДЛИТЕЛЬНОСТЬ ИМПУЛЬСА)

void ring(){
  GSM.println("ATD+380XXXXXXXXX;"); //// НОМЕР ТЕЛЕФОНА НА КОТОРЫЙ БУДЕТ ЗВОНИТЬ ////
   
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

  GSM.println("AT+CLVL=100");
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

    GSM.println("AT");

    delay(100);

    if (!GSM.find("OK")){

      modemcheck();
    }

    digitalWrite(LED, HIGH);

    ring();
  }

  if (digitalRead(LED)>0){

    delay(200);   // ЕСЛИ БУДЕТ ВЫКЛЮЧАТЬ РЕЛЕ ДО ТОГО МОМЕНТА
                  // КАК ВЗЯЛИ ТРУБКУ. УВЕЛИЧИТЬ ЗАДЕРЖКУ НА ВРЕМЯ
                  // КОТОРОЕ ИДУТ ГУДКИ (ЕСЛИ РАБОТАЕТ ЛУЧШЕ НЕ ТРОГАТЬ)

    GSM.println("AT+CPAS");

    delay(2000);  // задержка

    if (!GSM.find("4")){
      //delay(2000);  // задержка выключения усилителя. в это время кнопка не работает. по умолчанию откл.
      digitalWrite(LED, LOW);
    }
  }
}
 

