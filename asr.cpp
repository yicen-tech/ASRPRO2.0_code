#include "asr.h"
extern "C"{ void * __dso_handle = 0 ;}
#include "setup.h"
#include "myLib/asr_servo.h"
#include "HardwareSerial.h"
#include "myLib/asr_event.h"

uint32_t snid;
Servo servo_0;
Servo servo_4;
Servo servo_5;
Servo servo_6;
uint8_t duoji = 0;
int trunk_mode = 0;
bool time_flag = 0;
int hour;
int minute;
bool date_flag = 0;
int month;
int day;
bool week_flag = 0;
int week;
bool weather_flag = 0;
int weather;
int weather_temp;

void stimer_1(TimerHandle_t xTimer);
TimerHandle_t softtimer_1=NULL;
void stimer_2(TimerHandle_t xTimer);
TimerHandle_t softtimer_2=NULL;
void stimer_3(TimerHandle_t xTimer);
TimerHandle_t softtimer_3=NULL;
void stimer_4(TimerHandle_t xTimer);
TimerHandle_t softtimer_4=NULL;

String Rec;
int number;
void UART_RX();

void ASR_CODE();
void app();

//{ID:250,keyword:"命令词",ASR:"最大音量",ASRTO:"音量调整到最大"}
//{ID:251,keyword:"命令词",ASR:"中等音量",ASRTO:"音量调整到中等"}
//{ID:252,keyword:"命令词",ASR:"最小音量",ASRTO:"音量调整到最小"}
//{speak:小爱-活泼女声,vol:4,speed:10,platform:haohaodada,version:V3}
//{playid:10001,voice:你好，我是您的智能语音助手，你可以叫我小逸}
//{playid:10002,voice:我休息了，你可以用小逸唤醒我}

void stimer_1(TimerHandle_t xTimer){
  duoji ^=1;
  if(duoji == 0) {
    servo_4.write(100);
  }
  else {
    servo_4.write(0);
  } 
}

void stimer_3(TimerHandle_t xTimer){
  duoji ^=1;
  if(duoji == 0) {
    servo_4.write(100);
  }
  else {
    servo_4.write(0);
  } 
}

void stimer_4(TimerHandle_t xTimer){
  duoji ^=1;
  if(duoji == 0) {
    servo_4.write(100);
  }
  else {
    servo_4.write(0);
  } 
}


void stimer_2(TimerHandle_t xTimer){
  digitalWrite(1,!(digitalRead(1)));
  digitalWrite(20,!(digitalRead(20)));
}

void wiper_proc(){
  if(trunk_mode == 0){
    xTimerStart(softtimer_1,0);
    xTimerStop(softtimer_3,0);
    xTimerStop(softtimer_4,0);
  }
  else if(trunk_mode == 1){
    xTimerStop(softtimer_1,0);
    xTimerStart(softtimer_3,0);
    xTimerStop(softtimer_4,0);
  }
  else if(trunk_mode == 2){
    xTimerStop(softtimer_1,0);
    xTimerStop(softtimer_3,0);
    xTimerStart(softtimer_4,0);
  }
}

void play_two_digit_number(int number) {
  if (number >= 10) {
      int tens = number / 10; // 十位
      int ones = number % 10; // 个位

      if (tens > 1) {
          play_audio(10084 + tens); // 播报十位（如 "二"）
      }
      play_audio(10094); // 播报 "十"

      if (ones > 0) {
          play_audio(10084 + ones); // 播报个位（如 "九"）
      }
  } else {
      play_audio(10084 + number); // 播报个位（如 "五"）
  }
}

void my_time_play(void){
  if (time_flag == 1) {
    //{playid:10512,voice:现在是}
    play_audio(10512);  

    play_two_digit_number(hour); // 播报小时
    play_audio(10100); // 播报“点”
    play_two_digit_number(minute);
    play_audio(10101); // 播报“分”
    time_flag = 0;
  }
}

void my_date_play(void){
  if (date_flag == 1) {
    //{playid:10511,voice:今天是}
    play_audio(10511);

    play_two_digit_number(month);
    play_audio(10102); // 播报“月”
    play_two_digit_number(day);
    play_audio(10103); // 播报“日”
    date_flag = 0;
  }
}

void my_week_play(){
  if (week_flag == 1) {
    //{playid:10513,voice:今天是}
    play_audio(10513);

    play_audio(10104 + week); // 播报星期
    week_flag = 0;
  }
}

void my_weather_play(){
  if (weather_flag == 1) {
    //{playid:10514,voice:今天天气}
    play_audio(10514);

    play_audio(10112 + weather); // 播报天气
    play_two_digit_number(weather_temp); // 播报天气温度
    play_audio(10122); // 播报“度”
    weather_flag = 0;
  }
}

void UART_RX(){
  while (1) {
    if(Serial.available() > 0){
      Rec = Serial.readString();
      if(Rec == "1"){
        digitalWrite(1,1);
        digitalWrite(20,1);
        // //{playid:10500,voice:已打开车灯}
        // play_audio(10500);   
      }
      else if(Rec == "2"){
        digitalWrite(1,0);
        digitalWrite(20,0);
        // //{playid:10501,voice:已关闭车灯}
        // play_audio(10501);          
      }
      else if(Rec == "3"){
        digitalWrite(1,0);
        digitalWrite(20,0);
        xTimerStart(softtimer_2,0);
        // //{playid:10502,voice:已打开双闪}
        // play_audio(10502);          
      }
      else if(Rec == "4"){
        digitalWrite(1,0);
        digitalWrite(20,0);
        xTimerStop(softtimer_2,0);
        // //{playid:10503,voice:已关闭双闪}
        // play_audio(10503);          
      }
      else if(Rec == "5"){
        servo_0.write(100);
        // //{playid:10504,voice:已打开左车门}
        // play_audio(10504);          
      }
      else if(Rec == "6"){
        servo_0.write(0);
        // //{playid:10505,voice:已关闭左车门}
        // play_audio(10505);          
      }
      else if(Rec == "7"){
        servo_5.write(100);
        // //{playid:10506,voice:已打开右车门}
        // play_audio(10506);          
      }
      else if(Rec == "8"){
        servo_5.write(180);
        // //{playid:10507,voice:已关闭右车门}
        // play_audio(10507);          
      }
      else if(Rec == "9"){
        servo_6.write(100);
        // //{playid:10508,voice:已打开后备箱}
        // play_audio(10508);          
      }
      else if(Rec == "a"){
        servo_6.write(0);
        // //{playid:10509,voice:已关闭后备箱}
        // play_audio(10509);          
      }      
      else if(Rec == "b"){
        xTimerStart(softtimer_1,0);
        xTimerStop(softtimer_3,0);
        xTimerStop(softtimer_4,0);
      }   
      else if(Rec == "c"){
        xTimerStop(softtimer_1,0);
        xTimerStart(softtimer_3,0);
        xTimerStop(softtimer_4,0);
      }
      else if(Rec == "d"){
        xTimerStop(softtimer_1,0);
        xTimerStop(softtimer_3,0);
        xTimerStart(softtimer_4,0);
      }   
      else if(Rec == "e"){
        xTimerStop(softtimer_1,0);
        xTimerStop(softtimer_3,0);
        xTimerStop(softtimer_4,0);
        servo_4.write(0);
      }      
      if (Rec.length() == 5) { // 判断是否为5位数字
        String data = Rec.substring(1); // 提取后4位数字
        int number = data.toInt(); // 转换后4位为整数

        if (Rec[0] == '1') { // 以1开头表示时间
          hour = number / 100; // 提取小时
          minute = number % 100; // 提取分钟   

          time_flag = 1;
        } else if (Rec[0] == '0') { // 以0开头表示日期
          month = number / 100; // 提取月份
          day = number % 100; // 提取日期

          date_flag = 1;
        }    
      }
      if (Rec.length() == 2) {
        if (Rec[0] == '2') {        
          String data = Rec.substring(1); 
          int number = data.toInt(); 

          week = number; // 提取星期
          week_flag = 1; // 设置星期标志
        }
      }
      if (Rec.length() == 4) {
        if (Rec[0] == '3') {        
          String data = Rec.substring(1); 
          int number = data.toInt(); 

          weather = number / 100; // 提取天气
          weather_temp = number % 100; // 提取天气温度
          weather_flag = 1; // 设置天气标志
        }
      }

    }
    delay(2);
  }
  vTaskDelete(NULL);
}

/*描述该功能...
*/
void ASR_CODE(){
  set_state_enter_wakeup(15000);
  switch (snid) {
    case 0:
      // Serial.print("hello");
     break;
    case 1:
      // Serial.print("hello");
     break;
    case 2:
      Serial.print("b3");
     break;
    case 3:
    //  digitalWrite(28,1);
     break;
    case 4:
    //  digitalWrite(28,0);
     break;
    case 5:
    //  digitalWrite(21,1);
     break;
    case 6:
      digitalWrite(20,1);
     break;
    case 7:
      digitalWrite(20,1);
     break;
    case 8:
      digitalWrite(20,0);
     break;
    case 9:
      digitalWrite(1,1);
      digitalWrite(20,1);
     break;
    case 10:
      digitalWrite(1,0);
      digitalWrite(20,0);
     break;
    case 11:
      digitalWrite(1,0);
      digitalWrite(20,0);
      xTimerStart(softtimer_2,0);
     break;
    case 12:
      digitalWrite(1,0);
      digitalWrite(20,0);
      xTimerStop(softtimer_2,0);
     break;
    case 13:
      servo_0.write(100);
      servo_5.write(100);
     break;
    case 14:
      servo_0.write(0);
      servo_5.write(180);
     break;
    case 15:
      servo_0.write(100);
     break;
    case 16:
      servo_0.write(0);
     break;
    case 17:
      servo_5.write(100);
     break;
    case 18:
      servo_5.write(180);
     break;
    case 19:
      xTimerStart(softtimer_1,0);
      xTimerStop(softtimer_3,0);
      xTimerStop(softtimer_4,0);
     break;
    case 20:
      xTimerStop(softtimer_1,0);
      xTimerStop(softtimer_3,0);
      xTimerStop(softtimer_4,0);
      servo_4.write(0);
     break;
    case 21:
      trunk_mode ++;
      if(trunk_mode >= 2) trunk_mode = 2;
      wiper_proc();
     break;
    case 22:
      trunk_mode --;
      if(trunk_mode <= 0) trunk_mode = 0;
      wiper_proc();
     break;
    case 23:
      servo_6.write(100);
     break;
    case 24:
      servo_6.write(0);
     break;
    case 25:
      Serial.print("b0");
     break;
    case 26:
      Serial.print("b1");
     break;
    case 27:
      Serial.print("b2");
     break;
    case 28:
      Serial.print("b3");
     break;
    case 29:
      Serial.print("b4");
     break;
    case 30:  
      Serial.print("b5");
      break;
    case 31:      
      Serial.print("b6");
     break;     
    case 32:
      Serial.print("b7");
      break;  
    case 33:
      Serial.print("b8");
      break;
    case 34:
      Serial.print("b9");
      break;  
    case 35:
      Serial.print("ba");
      break;  
    case 36:
      Serial.print("bb");
      break;
    case 37:
      Serial.print("bc");
      break;
    case 38:
      Serial.print("bd");
      break;
    case 39:
      Serial.print("be");
      break;
    case 40:
      Serial.print("bf");
      break;
    case 41:
      Serial.print("c0");
      break;
    case 42:
      Serial.print("c1");
      break;
    case 43:
      Serial.print("c2");
      break;
    case 44:
      Serial.print("c3");
      break;
    case 48:
      digitalWrite(1,1);
      break;
    case 49:
      digitalWrite(1,0);
      break;      
    case 50:
      digitalWrite(20,1);
      break;
    case 51:
      digitalWrite(20,0);
      break;   
   }
}

void app(){
  //操作系统的一个线程，独立主循环任务,可支持多个类似线程任务。
  //当存在多个线程任务时，注意优先级与占用内存设置。
  while (1) {
    my_time_play();
    my_date_play();
    my_week_play();
    my_weather_play();
    delay(100);
  }
  vTaskDelete(NULL);
}

void hardware_init(){
  softtimer_1=xTimerCreate("stimer_1",800,1,0,stimer_1);
  softtimer_2=xTimerCreate("stimer_2",200,1,0,stimer_2);
  softtimer_3=xTimerCreate("stimer_3",500,1,0,stimer_3);
  softtimer_4=xTimerCreate("stimer_4",250,1,0,stimer_4);
  //需要操作系统启动后初始化的内容
  vol_set(2);
  setPinFun(13,SECOND_FUNCTION);
  setPinFun(14,SECOND_FUNCTION);
  Serial.begin(9600);
  Rec = "";
  xTaskCreate(UART_RX,"UART_RX",128,NULL,4,NULL);
  xTaskCreate(app,"app",128,NULL,4,NULL);
  vTaskDelete(NULL);
}

void setup()
{
  //需要操作系统启动前初始化的内容
  //{ID:0,keyword:"唤醒词",ASR:"小逸小逸",ASRTO:"我在"}
  //{ID:1,keyword:"唤醒词",ASR:"小逸同学",ASRTO:"我在呢"}
  //{ID:2,keyword:"命令词",ASR:"今天天气",ASRTO:""}
  //{ID:3,keyword:"命令词",ASR:"你几岁了",ASRTO:"我今年已经五岁了"}
  //{ID:4,keyword:"命令词",ASR:"你会做什么",ASRTO:"我可以陪你聊天哦"}
  //{ID:5,keyword:"命令词",ASR:"给我讲个笑话",ASRTO:"以前有一个人在睡觉，然后一只蚊子过来咬他了。他被叮醒正准备一只手啪下去的时候，蚊子对他说，求求你别杀我，今天是我的生日。那个人听说后，小心翼翼把蚊子放在手心，一边拍手一边唱生日快乐歌！"}
  //{ID:6,keyword:"命令词",ASR:"帮我开灯",ASRTO:"好的，已为你开灯"}
  //{ID:7,keyword:"命令词",ASR:"开灯",ASRTO:"好的，已为你开灯"}
  //{ID:8,keyword:"命令词",ASR:"关灯",ASRTO:"好的，已为你关灯"}
  //{ID:9,keyword:"命令词",ASR:"打开车灯",ASRTO:"好的，已为你打开车灯"}
  //{ID:10,keyword:"命令词",ASR:"关闭车灯",ASRTO:"好的，已为你关闭车灯"}
  //{ID:11,keyword:"命令词",ASR:"打开双闪",ASRTO:"好的，已为你打开双闪"}
  //{ID:12,keyword:"命令词",ASR:"关闭双闪",ASRTO:"好的，已为你关闭双闪"}
  //{ID:13,keyword:"命令词",ASR:"打开车门",ASRTO:"好的，已为你打开车门"}
  //{ID:14,keyword:"命令词",ASR:"关闭车门",ASRTO:"好的，已为你关闭车门"}
  //{ID:15,keyword:"命令词",ASR:"打开左车门",ASRTO:"好的，已为你打开左车门"}
  //{ID:16,keyword:"命令词",ASR:"关闭左车门",ASRTO:"好的，已为你关闭左车门"}
  //{ID:17,keyword:"命令词",ASR:"打开右车门",ASRTO:"好的，已为你打开右车门"}
  //{ID:18,keyword:"命令词",ASR:"关闭右车门",ASRTO:"好的，已为你关闭右车门"}
  //{ID:19,keyword:"命令词",ASR:"打开雨刷",ASRTO:"好的，已为你打开雨刷"}
  //{ID:20,keyword:"命令词",ASR:"关闭雨刷",ASRTO:"好的，已为你关闭雨刷"}  
  //{ID:21,keyword:"命令词",ASR:"大一点",ASRTO:"好的，已为你加大雨刷器"}
  //{ID:22,keyword:"命令词",ASR:"小一点",ASRTO:"好的，已为你减小雨刷器"}  
  //{ID:23,keyword:"命令词",ASR:"打开后备箱",ASRTO:"好的，已为你打开后备箱"}
  //{ID:24,keyword:"命令词",ASR:"关闭后备箱",ASRTO:"好的，已为你关闭后备箱"}  
  //{ID:25,keyword:"命令词",ASR:"现在几点了",ASRTO:""}    /****************b0****************/
  //{ID:26,keyword:"命令词",ASR:"今天周几",ASRTO:""}      /****************b1****************/
  //{ID:27,keyword:"命令词",ASR:"今天几号了",ASRTO:""}    /****************b2****************/
  //{ID:28,keyword:"命令词",ASR:"今天天气怎么样",ASRTO:""}                /****b3****************/
  //{ID:29,keyword:"命令词",ASR:"播放音乐",ASRTO:"好的，已为你播放音乐"}    /***b4****************/
  //{ID:30,keyword:"命令词",ASR:"我要听晴天",ASRTO:"好的，已为你播放晴天"}  /***b5****************/
  //{ID:31,keyword:"命令词",ASR:"打开音乐",ASRTO:"好的，已为你打开音乐"}  /***b6****************/
  //{ID:32,keyword:"命令词",ASR:"关闭音乐",ASRTO:"好的，已为你关闭音乐"}  /***b7****************/
  //{ID:33,keyword:"命令词",ASR:"暂停",ASRTO:"好的，已为你暂停音乐"}    /***b8****************/
  //{ID:34,keyword:"命令词",ASR:"播放",ASRTO:"好的，已为你播放音乐"}    /***b9****************/
  //{ID:35,keyword:"命令词",ASR:"继续",ASRTO:"好的，已为你继续播放音乐"}  /***ba****************/
  //{ID:36,keyword:"命令词",ASR:"下一首",ASRTO:"好的，已为你播放下一首"}  /***bb****************/
  //{ID:37,keyword:"命令词",ASR:"上一首",ASRTO:"好的，已为你播放上一首"}  /***bc****************/
  //{ID:38,keyword:"命令词",ASR:"播放视频",ASRTO:"好的，已为你播放视频"}  /***bd****************/
  //{ID:39,keyword:"命令词",ASR:"打开视频",ASRTO:"好的，已为你打开视频"}  /***be****************/
  //{ID:40,keyword:"命令词",ASR:"关闭视频",ASRTO:"好的，已为你关闭视频"}  /***bf****************/
  //{ID:41,keyword:"命令词",ASR:"打开倒车影像",ASRTO:"好的，已为你打开倒车影像"}  /***c0****************/
  //{ID:42,keyword:"命令词",ASR:"返回主界面",ASRTO:"好的，已为你返回主界面"}  /***c1****************/
  //{ID:43,keyword:"命令词",ASR:"打开设置",ASRTO:"好的，已为你打开设置"}  /***c2****************/
  //{ID:44,keyword:"命令词",ASR:"打开游戏",ASRTO:"好的，已为你打开游戏"}  /***c3****************/
  //{ID:45,keyword:"命令词",ASR:"你是谁",ASRTO:"我是车载终端系统的语音助手，你可以叫我小逸"}  
  //{ID:46,keyword:"命令词",ASR:"你叫什么名字",ASRTO:"我是车载终端系统的语音助手，你可以叫我小逸"}
  //{ID:47,keyword:"命令词",ASR:"你能做什么",ASRTO:"我是车载终端系统的语音助手，你可以叫我小逸，我可以为你提供语音识别、语音控制等功能"}
  //{ID:48,keyword:"命令词",ASR:"打开左车灯",ASRTO:"好的，已为你打开左车灯"}
  //{ID:49,keyword:"命令词",ASR:"关闭左车灯",ASRTO:"好的，已为你关闭左车灯"}
  //{ID:50,keyword:"命令词",ASR:"打开右车灯",ASRTO:"好的，已为你打开右车灯"}
  //{ID:51,keyword:"命令词",ASR:"关闭右车灯",ASRTO:"好的，已为你关闭右车灯"}

 
  setPinFun(1,FIRST_FUNCTION);
  pinMode(1,output);
  dpmu_set_adio_reuse(PA1,DIGITAL_MODE);
  digitalWrite(1,0);
  setPinFun(20,SECOND_FUNCTION);
  pinMode(20,output);
  dpmu_set_adio_reuse(PC4,DIGITAL_MODE);
  digitalWrite(20,0);

  servo_0.attach(0);
  servo_4.attach(4);
  servo_5.attach(5);
  servo_6.attach(6);
  
  servo_0.write(0);
  servo_4.write(0);
  servo_5.write(180);
  servo_6.write(0);

  //{playid:10084,voice:零}
  //{playid:10085,voice:一}
  //{playid:10086,voice:二}
  //{playid:10087,voice:三}
  //{playid:10088,voice:四}
  //{playid:10089,voice:五}
  //{playid:10090,voice:六}
  //{playid:10091,voice:七}
  //{playid:10092,voice:八}
  //{playid:10093,voice:九}
  //{playid:10094,voice:十}
  //{playid:10095,voice:百}
  //{playid:10096,voice:千}
  //{playid:10097,voice:万}
  //{playid:10098,voice:亿}
  //{playid:10099,voice:负}
  //{playid:10100,voice:点}
  //{playid:10101,voice:分}
  //{playid:10102,voice:月}
  //{playid:10103,voice:日}
  //{playid:10104,voice:时}
  //{playid:10105,voice:星期一}
  //{playid:10106,voice:星期二}
  //{playid:10107,voice:星期三}
  //{playid:10108,voice:星期四}
  //{playid:10109,voice:星期五}
  //{playid:10110,voice:星期六}
  //{playid:10111,voice:星期日}
  //{playid:10112,voice:多云}
  //{playid:10113,voice:晴}
  //{playid:10114,voice:小雨}
  //{playid:10115,voice:中雨}
  //{playid:10116,voice:大雨}
  //{playid:10117,voice:雾}
  //{playid:10118,voice:霾}
  //{playid:10119,voice:雪}
  //{playid:10120,voice:阴}
  //{playid:10121,voice:阵雨}
  //{playid:10122,voice:度}

}



