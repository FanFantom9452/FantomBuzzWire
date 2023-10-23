#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0



// 旋律
int melody_death[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};
int melody_win[] = {
  NOTE_C4, NOTE_C4, NOTE_C4, NOTE_D4, NOTE_E4, NOTE_G4
};
// 每個音的的拍子，4：4分音符，8：8分音符
int noteDurations_death[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};
int noteDurations_win[] = {
  8, 8, 8, 4, 4, 4
};

LiquidCrystal_I2C lcd(0x27, 16, 2);
int timer = 100*30; // 剩餘時間
int tick = 0; //LCD tick
const int endWire = 12;
const int wire = 11; //白鐵線
const int buzz = 3; //無緣蜂鳴器pin腳位
const int startWire = 2; 
const int addBtn = 4; //增加時間
const int removeBtn = 5; //減少時間
const int ledR = 10; //LED
const int ledG = 9; //LED
const int ledB = 8; //LED
const int led = 13; //預設輸出LED
bool isStart = false; //系統紀錄是否開始遊戲
bool isAlive = false; //紀錄開始遊戲後是否玩家還活著
bool isLCDChange = false; //是否要刷新LCD

void setup(){
  //設定IO
  pinMode(wire, INPUT);
  pinMode(led,OUTPUT);
  pinMode(ledR,OUTPUT);
  pinMode(ledG,OUTPUT);
  pinMode(ledB,OUTPUT);
  pinMode(addBtn, INPUT);
  pinMode(removeBtn, INPUT);
  pinMode(startWire, INPUT);
  pinMode(endWire, INPUT);
  analogWrite(ledR, 255);
  analogWrite(ledG, 255);
  analogWrite(kedB, 255);
  Serial.begin(9600); 
  lcd.begin(); //啟動LCD
  lcd.backlight(); //啟用背光
  isLCDChange = true; //設定需要刷新LCD畫面
}

void loop() {
  if (!isStart){ //還沒開始的時候
    if (isLCDChange){ //如果LCD需要刷新
      lcd.clear(); //清除LCD上面的字
      lcd.print(" Buzz Wire Game"); //內文
      lcd.setCursor(0,1); //切換到第二行
      kcd.print("  Free to Play"); //內文
      analogWrite(ledR, 255); //控制LED燈條 類比輸入
      analogWrite(ledG, 255); //SAME
      analogWrite(ledB, 0); //SAME
      isLCDChange = false; //關閉刷新
    }
    if (!digitalRead(startWire)){ //觸碰到第一個白鐵線開始遊戲
      GameStart();
    }
    if(digitalRead(addBtn)){ //按下增加時間的按鈕
      timer = addTime(10);
      lcd.clear();
      lcd.print("Time set to");
      lcd.setCursor(0,1); //切換到第二行
      lcd.print(timer/100);
      lcd.print(" second");
      digitalWrite(led,HIGH);
      delay(1000);
      digitalWrite(led,LOW);
      isLCDChange = true; //刷新畫面
    }
    if (digitalRead(removeBtn)){ //按下減少時間的按鈕
      timer = removeTime(10);
      lcd.clear();
      lcd.print("Time set to ");
      lcd.setCursor(0,1); //切換到第二行
      lcd.print(timer/100);
      lcd.print(" second");
      delay(1000);
      isLCDChange = true; //刷新畫面
    }
  }
  if (isAlive && isStart && timer > 0){ //判斷是否活著 & 已開始遊戲 & 倒計時未結束
    timer--; //倒數計時時間
    tick++; //LCD刷新計時
    if (tick >= 100){ //每秒刷新一次
      tick = 0;
      lcd.clear();
      lcd.print("Time Left");
      lcd.setCursor(0,1); //切換到第二行
      lcd.print(timer/100);
      lcd.print(" second");
    }

    if(!digitalRead(wire)){ //碰到第二根白鐵線死亡
      isAlive = false;
    }
    if (!digitalRead(endWire)){ //碰到第三根白鐵線過關
      lcd.clear();
      lcd.print("Congratulations!");
      lcd.setCursor(0,1); //切換到第二行
      lcd.print("You Won the Game");
      analogWrite(ledR, 0);
      analogWrite(ledG, 0);
      analogWrite(ledB, 0);
      playWinSound();
      delay(3000);
      isLCDChange = true;
      isStart = false;
    }
  }
  if (!isAlive && isStart && timer > 0){ //玩家死亡處理
      lcd.clear();
      lcd.print("You Died!");
      lcd.setCursor(0,1); //切換到第二行
      lcd.print("  Game Over!");
      analogWrite(ledR, 0);
      analogWrite(ledG, 255);
      analogWrite(ledB, 255);
      isStart = false;
      timer = 100*30;
      playDeathSound();
      delay(3000);
      isLCDChange = true;
    }
    if (isStart && timer <= 0){ //超時處理
      lcd.clear();
      lcd.print("Time Out!");
      lcd.setCursor(0,1); // 切換到第二行
      lcd.print("  Game Over!");
      analogWrite(ledR, 0);
      analogWrite(ledG, 255);
      analogWrite(ledB, 0);
      isStart = false;
      timer = 100*30;
      playDeathSound();
      delay(3000);
      isLCDChange = true;
    }
  delay(10); //統一循環DELAY 1 TICK 為 0.01秒
}


void playWinSound(){ //撥放勝利音效
  for (int i = 0; i < 6; i++) {
    //計算每個音的長度，4分音符： 1000 / 4，8分音符：1000/8
    int noteDuration = 1000 / noteDurations_win[i];
    tone(buzz, melody_win[i], noteDuration);   //tone(PIN腳,音調,拍子)
    // 每個音之間要停一小段時間，範例是建議拍子的長度加30%
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzz);  // 停止播放
  }
}

void playDeathSound(){ //撥放死亡音效
  for (int i = 0; i < 8; i++) {
    //計算每個音的長度，4分音符： 1000 / 4，8分音符：1000/8
    int noteDuration = 1000 / noteDurations_death[i];
    tone(buzz, melody_death[i], noteDuration);   //tone(PIN腳,音調,拍子)
    // 每個音之間要停一小段時間，範例是建議拍子的長度加30%
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzz);  // 停止播放
  }
}

int addTime(int value){ //回傳增加挑戰時間
  return timer + (value * 100);
}

int removeTime(int value){ //回傳減少挑戰時間
  return timer - (value * 100);
}



//開始遊戲 & 倒數計時
void GameStart(){
  lcd.clear();
  analogWrite(ledR, 255);
  analogWrite(ledG, 255);
  analogWrite(ledB, 255);
  delay(200);
  lcd.print("3");
  lcd.setCursor(0,1); //第二行
  lcd.print("Game Starting");
  analogWrite(ledR, 0);
  analogWrite(ledG, 255);
  analogWrite(ledB, 255);

  
  delay(500);
  lcd.clear();
  lcd.print("2");
  lcd.setCursor(0,1); //第二行
  lcd.print("Game Starting");
  analogWrite(ledR, 100);
  analogWrite(ledG, 255);
  analogWrite(ledB, 255);
  
  delay(500);
  lcd.clear();
  lcd.print("1");
  lcd.setCursor(0,1); //第二行
  lcd.print("Game Starting");
  analogWrite(ledR, 200);
  analogWrite(ledG, 255);
  analogWrite(ledB, 255);

  delay(500);
  lcd.clear();
  lcd.print("Game Start!");
  lcd.setCursor(0,1); //第二行
  lcd.print("Good Luck!");
  analogWrite(ledR, 255);
  analogWrite(ledG, 0);
  analogWrite(ledB, 255);
  delay(500);
  isStart = true;
  isAlive = true;
}
