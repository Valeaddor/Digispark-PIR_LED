#define PIN_PHOTO_SENSOR 1
#define PIN_PIR_SENSOR 0
#define BUILDIN_LED 1

#define delayTimeMIN 30000
#define delayTimeMAX 150000
#define lightSensorMIN 100


boolean LightSensorON = false;
boolean isDark = true;
boolean LEDisON = false;
unsigned int val = 0;
unsigned int PIRstate = LOW;
unsigned int delayTime = 30000;
unsigned long movetime = 0;
unsigned long lastmove = 0;

void setup() {

  pinMode(PIN_PIR_SENSOR, INPUT); // объявляем пин 0 как вход
  pinMode(BUILDIN_LED, OUTPUT); // объявляем пин 1 как выход
//  pinMode(PIN_PHOTO_SENSOR, INPUT);
 
  val = analogRead(PIN_PHOTO_SENSOR);

  if (val == 0) FlashError();                             // мигаем ошибкой и отключаем работу с фоторезистором
  else if (val>=100) FlashLightSensorVal(val/100);        // мигаем значением с фоторезистора деленным на 100
  else FlashLightOneSec();                                // мигаем долго один раз
  
  delay(1000);

}

void loop() {

  if(LightSensorON)
  { 
    val = analogRead(PIN_PHOTO_SENSOR);
    if(val<lightSensorMIN) isDark = true;
    else isDark = false;
  };

  PIRstate = digitalRead(PIN_PIR_SENSOR);
    
  if (PIRstate == HIGH)                             // если зарегистрировано движение
    movetime = millis();                            // сохраняем текущее время движения
  
  if (isDark)                                       // ТЕМНО
  {
    if (PIRstate == HIGH)                           // и есть движение
    {
      if(!LEDisON)                                  // а лампочка не горит
      {
        digitalWrite(BUILDIN_LED,HIGH);             // включаем лампочку
        LEDisON = true;                             // запоминаем состоянии лампочки
        lastmove = movetime;                        // сохраняем время последнего движения
      } else                                        
      {                                             // иначе если лампочка уже горит
        FreshTimer();                               // Обновляем значение таймера горения лампочки
      };
      
    } else                                          // если движения нет
    if (LEDisON)                                    // а лампочка горит
    {
      TurnLEDbyTimer();                             // выключаем лампочку если закончилось время
    };
  } 
  
  else                                              // СВЕТЛО
    if (LEDisON)                                    // и лампочка горит
    {
      if (PIRstate == HIGH)                         // и есть движение
      {
        FreshTimer();                               // Обновляем значение таймера горения лампочки
        
      } else                                        // и нет движения
      TurnLEDbyTimer();                             // выключаем лампочку если закончилось время
    };
  
  if(millis() < lastmove) lastmove = 0;             // проверяем не сбросился ли счетчик millis()

  delay(10);

}

void FlashError()
{
  for (byte i=1; i<=10; i++) {
    digitalWrite(BUILDIN_LED,HIGH);
    delay(100);
    digitalWrite(BUILDIN_LED,LOW);
    delay(100);
  };
};

void FlashLightSensorVal(byte count)
{
  for (byte i=1; i<=count; i++)
  {
    digitalWrite(BUILDIN_LED,HIGH);
    delay(250);
    digitalWrite(BUILDIN_LED,LOW);
    delay(250);
  };

  LightSensorON = true;
  
};

void FlashLightOneSec()
{
  digitalWrite(BUILDIN_LED,HIGH);
  delay(1000);
  digitalWrite(BUILDIN_LED,LOW);
  delay(1000);

  LightSensorON = true;
  
};

void TurnLEDbyTimer()
{
    if (millis()>lastmove+delayTime)                // проверяем закончилось ли время отведенное на горение
    {
      digitalWrite(BUILDIN_LED,LOW);                // гасим лампочку
      delayTime = delayTimeMIN;                     // время горения возвращаем к изначальному значению
      LEDisON = false;                              // запоминаем состояние лампочки      
    };
};

void FreshTimer()
{
  if((movetime > (lastmove+10000))&&(delayTime < delayTimeMAX))
      { 
        delayTime = delayTime + delayTimeMIN;
      };
  lastmove = movetime;                              // сохраняем время последнего движения
};
