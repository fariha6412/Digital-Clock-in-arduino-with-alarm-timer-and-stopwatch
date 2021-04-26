#include <LiquidCrystal.h>;
const int rs = 12, en = 11, d5 = 6, d6 = 5, d7 = 4, d8 = 3;
LiquidCrystal lcd(rs, en, d5, d6, d7, d8);

bool isAlarm = false, isStopwatch = false, isTimer = false;
const int buzzer = 2, led = 13;
const int pushbtn1 = 10; 
const int pushbtn2 = 9;
const int pushbtn3 = 8;
const int pushbtn4 = 7;

int hour=10, minute=15, second=5, pis=0, mls=0, lastTime=0, now;
int hourAlarm=0, minuteAlarm=0, secondAlarm=0;
int mode=0;
int flag = 0, flagAlarm = 0;  //indicates 0 = AM, 1 = PM


int btnstate1; //indicates pushbtn's state
int btnstate2;
int btnstate3;
int btnstate4;


void setup() {
  lcd.begin(16, 2);
  pinMode(pushbtn2, INPUT);
  pinMode(pushbtn3, INPUT);
  pinMode(pushbtn4, INPUT);
  pinMode(pushbtn1, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(led, OUTPUT);
  Serial.begin(9600);
}

void loop(){
  if(mode==0)clock();
  else if(mode==1)setTime();
  else if(mode==2)setAlarm();
  else if(mode==3)setTimer();
  else if(mode==4)stopwatch();
}


//////////****************CHECKING FOR NEW SCREEN***************//////////
void checkState(){
  btnstate1=digitalRead(pushbtn1);
  btnstate2=digitalRead(pushbtn2);
  btnstate3=digitalRead(pushbtn3);
  btnstate4=digitalRead(pushbtn4);
  if(btnstate1|btnstate2|btnstate3|btnstate4) {
     if(btnstate1==1){
       mode=1;
     }
     if(btnstate2==1){
       mode=2;
     }
     if(btnstate3==1){
       mode=3;
     }
     if(btnstate4==1){
       mode=4;
     }
    }
    else {
       mode=0;
    }
}


//////////****************CLOCK***************//////////
void clock(){
  lcd.setCursor(0,0);
  lcd.print("What time is it?");
  lcd.setCursor(0,1);
  lcd.print("It's ");
  if(hour<10)lcd.print("0");
  lcd.print(hour);
  lcd.print(":");
  if(minute<10)lcd.print("0");
  lcd.print(minute);
  lcd.print(":");
  if(second<10)lcd.print("0");
  lcd.print(second);

 if(flag==0) lcd.print(" AM");
 if(flag==1) lcd.print(" PM");
 
 checkState();
 delay(200);
 runningClock(&hour, &minute, &second, &now, &lastTime, &mls, &flag);
}

void runningClock(int* _hour,int* _minute, int* _second, int* _now, int* _lastTime, int* _mls, int* _flag){
 int increHour=0; 
 *_now=(millis());
 *_mls+=(*_now-*_lastTime);
 *_lastTime=*_now;

 if(*_mls >= 1000){
  *_second+=(*_mls/1000); 
  *_mls%=1000;
  }
 if(*_second >= 60){
   *_minute+=*_second/60;
   *_second%=60;
  }
 if(*_minute >= 60){
   increHour=*_minute/60;
   	*_minute%=60;
  }
  if(increHour%24==0); //hour as before
  else {
    increHour%=24;
    if(increHour<12){
      if(*_hour==12){
        *_hour=increHour;
      }
      else if(*_hour+increHour>=12){
        *_hour-=12;
        *_flag=*_flag==0?1:0;
      }
    }
    else{
      *_hour=((*_hour+increHour)%12);
      *_flag=*_flag==0?1:0;
      
    }
 	if(*_flag==0 & *_hour==12)*_hour=0;
 	else if(*_flag==1 & *_hour==0)*_hour=12;
 }
  if(isAlarm)checkAlarm();
}



//////////****************TIMESETTING***************//////////
void setTime() {
  int *main_var;
  int X, Y;
  lcd.clear();
  
  int currentChangeIn = 1; //1 is hour 2 is minute and so on
  while(true) 
  {
    lcd.setCursor(0,0);
  	lcd.print("Set Time");
  	lcd.setCursor(0,1);
  	if(hour<10)lcd.print("0");
  	lcd.print(hour);
  	lcd.print(":");
  	if(minute<10)lcd.print("0");
  	lcd.print(minute);
  	lcd.print(":");
  	if(second<10)lcd.print("0");
  	lcd.print(second);

  	if(flag==0) lcd.print(" AM");
  	if(flag==1) lcd.print(" PM");
    
   	btnstate1=digitalRead(pushbtn1);
  	btnstate2=digitalRead(pushbtn2);
  	btnstate3=digitalRead(pushbtn3);
  	btnstate4=digitalRead(pushbtn4);
    if(btnstate1==1){
      mode=0;
      delay(200);
      break;
    }
    if(btnstate3==1){
      if(currentChangeIn==3)currentChangeIn=0;
      currentChangeIn+=1;
      delay(200);
    }
    if(currentChangeIn==1){
      main_var = &hour;
      X = 0;
      Y = 1;
     
      if(digitalRead(pushbtn2)==1){
        hour+=1;
        if(hour==13)hour=1;
        else if(hour==12 & flag==0)hour=0;
        delay(200);
      }
    }
    if(currentChangeIn==2){
      main_var = &minute;
      X = 3;
      Y = 1;
     
      if(digitalRead(pushbtn2)==1){
        minute+=1;
        if(minute==60)minute=0;
        delay(200);
      }
    }
    if(currentChangeIn==3){
      main_var = &second;
      X = 6;
      Y = 1;
     
      if(digitalRead(pushbtn2)==1){
        second+=1;
        if(second==60)second=0;
        delay(200);
      }
    }
    if(btnstate4==1){
      flag=flag==0?1:0;
      if(flag==0 & hour==12)hour=0;
      else if(flag==1 & hour==0)hour=12;
      delay(200);
    }
    blink(*main_var,X,Y);
	runningClock(&hour, &minute, &second, &now, &lastTime, &mls, &flag);
  }
}



//////////****************ALARM***************//////////
void setAlarm() {
  int *main_var;
  int X, Y;
  lcd.clear();
  
  int currentChangeIn = 1; //1 is hour 2 is minute and so on
  while(true) 
  {
    lcd.setCursor(0,0);
  	lcd.print("Set Alarm");
  	lcd.setCursor(0,1);
  	if(hourAlarm<10)lcd.print("0");
  	lcd.print(hourAlarm);
  	lcd.print(":");
  	if(minuteAlarm<10)lcd.print("0");
  	lcd.print(minuteAlarm);
  	lcd.print(":");
  	if(secondAlarm<10)lcd.print("0");
  	lcd.print(secondAlarm);

  	if(flagAlarm==0) lcd.print(" AM");
  	if(flagAlarm==1) lcd.print(" PM");
    
   	btnstate1=digitalRead(pushbtn1);
  	btnstate2=digitalRead(pushbtn2);
  	btnstate3=digitalRead(pushbtn3);
  	btnstate4=digitalRead(pushbtn4);
    if(btnstate1==1){
      mode=0;
      delay(200);
      break;
    }
    if(btnstate3==1){
      if(currentChangeIn==3)currentChangeIn=0;
      currentChangeIn+=1;
      delay(200);
    }
    if(currentChangeIn==1){
      main_var=&hourAlarm;
      X = 0;
      Y = 1;
     
      if(digitalRead(pushbtn2)==1){
        hourAlarm+=1;
        if(hourAlarm==13)hourAlarm=1;
        else if(hourAlarm==12 & flagAlarm==0)hourAlarm=0;
        delay(200);
      }
    }
    if(currentChangeIn==2){
      main_var=&minuteAlarm;
      X = 3;
      Y = 1;
     
      if(digitalRead(pushbtn2)==1){
        minuteAlarm+=1;
        if(minuteAlarm==60)minuteAlarm=0;
        delay(200);
      }
    }
    if(currentChangeIn==3){
      main_var=&secondAlarm;
      X = 6;
      Y = 1;
     
      if(digitalRead(pushbtn2)==1){
        secondAlarm+=1;
        if(secondAlarm==60)secondAlarm=0;
        delay(200);
      }
    }
    if(btnstate4==1){
      flagAlarm=flagAlarm==0?1:0;
      if(flagAlarm==0 & hourAlarm==12)hourAlarm=0;
      else if(flagAlarm==1 & hourAlarm==0)hourAlarm=12;
      delay(200);
    }
    blink(*main_var,X,Y);
  }
  isAlarm=true;
  runningClock(&hour, &minute, &second, &now, &lastTime, &mls, &flag);
}

void checkAlarm(){
  if(hour==hourAlarm & minute==minuteAlarm & second==secondAlarm & flag==flagAlarm){
    tone(buzzer, 1000);
    delay(3000);
    noTone(buzzer);
    isAlarm=false;
  }
}


//////////****************TIMER***************//////////
void setTimer() {
  int minuteTimer=0, secondTimer=0, mlsTimer=0, lastTimeTimer;
  int *main_var;
  int X, Y;
  lcd.clear();
  int brFlag=0; //blinking stopper
  int currentChangeIn = 2; //1 is hour 2 is minute and so on
  while(true) 
  {
    lcd.setCursor(0,0);
  	lcd.print("Set Timer");
  	lcd.setCursor(0,1);
  	if(minuteTimer<10)lcd.print("0");
  	lcd.print(minuteTimer);
  	lcd.print(":");
  	if(secondTimer<10)lcd.print("0");
  	lcd.print(secondTimer);
    
   	btnstate1=digitalRead(pushbtn1);
  	btnstate2=digitalRead(pushbtn2);
  	btnstate3=digitalRead(pushbtn3);
  	btnstate4=digitalRead(pushbtn4);
    if(btnstate1==1){
      mode=0;
      delay(200);
      break;
    }
    if(btnstate3==1){
      currentChangeIn+=1;
      if(currentChangeIn==4)currentChangeIn=2;
      delay(200);
    }
    if(currentChangeIn==2){
      main_var=&minuteTimer;
      X = 0;
      Y = 1;
     
      if(digitalRead(pushbtn2)==1){
        minuteTimer+=1;
        delay(200);
      }
    }
    if(currentChangeIn==3){
      main_var=&secondTimer;
      X = 3;
      Y = 1;
     
      if(digitalRead(pushbtn2)==1){
        secondTimer+=1;
        if(secondTimer==60)secondTimer=0;
        delay(200);
      }
    }
    if(btnstate4==1){
      if(!isTimer)isTimer=true;
      else{
        isTimer=false;
      }
      brFlag=1;
      lastTimeTimer=millis();
      delay(200);
    }
    if(isTimer)runningTimer(&minuteTimer, &secondTimer, &mlsTimer, &lastTimeTimer);
    if(minuteTimer==0&secondTimer==0)brFlag=0;
    if(brFlag==0)blink(*main_var,X,Y);
    delay(200);
    runningClock(&hour, &minute, &second, &now, &lastTime, &mls, &flag);
  }
}

void runningTimer(int* _minute, int* _second, int* _mls, int* _lastTime){
 int _now=millis();
 *_mls+=(_now-*_lastTime);
 *_lastTime=_now;
 if(*_mls >= 1000){
  *_second-=(*_mls/1000); 
  *_mls%=1000;
  }
 if(*_second <= 0){
   if(*_minute == 0 & *_second < 0){
   	isTimer=false;
   	digitalWrite(led, HIGH);
   	delay(3000);
   	digitalWrite(led, LOW);
    *_second=0;
  }
   else if(*_minute>0){
    *_minute-=1;
   	*_second=59;
   }
 }
}



//////////****************STOPWATCH***************//////////
void stopwatch(){
  int hourStopwatch=0, minuteStopwatch=0, secondStopwatch=0, mlsStopwatch=0, lastTimeStopwatch=0;
  lcd.clear();
  
  while(true) 
  {
    lcd.setCursor(0,0);
  	lcd.print("Stopwatch");
  	lcd.setCursor(0,1);
  	if(hourStopwatch<10)lcd.print("0");
  	lcd.print(hourStopwatch);
  	lcd.print(":");
  	if(minuteStopwatch<10)lcd.print("0");
  	lcd.print(minuteStopwatch);
  	lcd.print(":");
  	if(secondStopwatch<10)lcd.print("0");
  	lcd.print(secondStopwatch);
    
   	btnstate1=digitalRead(pushbtn1);
  	btnstate2=digitalRead(pushbtn2);
  	btnstate3=digitalRead(pushbtn3);
  	btnstate4=digitalRead(pushbtn4);
    if(btnstate1==1){
      mode=0;
      delay(200);
      break;
    }
    if(btnstate2==1){
      isStopwatch=true;
      lastTimeStopwatch=millis();
      delay(200);
    }
    if(btnstate3==1){
      isStopwatch=false;
      delay(200);
    }
    if(btnstate4==1){
      isStopwatch=false;
   	  hourStopwatch=0;
      minuteStopwatch=0;
      secondStopwatch=0;
      delay(200);
    }
    if(isStopwatch)runningStopwatch(&hourStopwatch, &minuteStopwatch, &secondStopwatch, &lastTimeStopwatch, &mlsStopwatch);
 	runningClock(&hour, &minute, &second, &now, &lastTime, &mls, &flag);
  }
}

void runningStopwatch(int* _hour,int* _minute, int* _second, int* _lastTime, int* _mls){
 int increHour=0, _now; 
 _now=(millis());
 *_mls+=(_now-*_lastTime);
 *_lastTime=_now;

 if(*_mls >= 1000){
  *_second+=(*_mls/1000); 
  *_mls%=1000;
  }
 if(*_second >= 60){
   *_minute+=*_second/60;
   *_second%=60;
  }
 if(*_minute >= 60){
   *_hour=*_minute/60;
   	*_minute%=60;
  }
  if(*_hour>=12){
    isStopwatch=false;
  }
}


//////////****************COMMON FUNCTION***************//////////
void blink(int main_var, int X, int Y){
  lcd.setCursor(X, Y);
  	if(pis == 0){ 
  	  if(main_var<10)lcd.print("0");
  	  lcd.print(main_var);
  	  delay(200);
  	}
  	if(pis == 1){
 	  lcd.print("  ");
 	  delay(200);
    }
  	pis=(pis+1)%2;
}

//////////****************THANK YOU***************//////////