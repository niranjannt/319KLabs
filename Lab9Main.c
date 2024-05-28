// Lab9Main.c
// Runs on MSPM0G3507
// Lab 9 ECE319K
// Your name
// Last Modified: 12/31/2023

#include <stdio.h>
#include <stdint.h>
#include <ti/devices/msp/msp.h>
#include "../inc/ST7735.h"
#include "../inc/Clock.h"
#include "../inc/LaunchPad.h"
#include "../inc/TExaS.h"
#include "../inc/Timer.h"
#include "../inc/ADC1.h"
#include "../inc/DAC5.h"
#include "SmallFont.h"
#include "LED.h"
#include "Switch.h"
#include "Sound.h"
#include "images/images.h"
// ****note to ECE319K students****
// the data sheet says the ADC does not work when clock is 80 MHz
// however, the ADC seems to work on my boards at 80 MHz
// I suggest you try 80MHz, but if it doesn't work, switch to 40MHz
uint32_t Data, Flag, Position, index2;
int32_t Timer=40;
uint32_t Sub=0;
# define GREENN 12
# define YELLOWW 17
# define REDD 13
int32_t timecounter;
uint32_t Zero;
uint32_t FakeTime;
uint32_t GameOver;
uint32_t HoopFlag;


uint32_t i=0;
uint32_t Count;

uint32_t xarray[4];
uint32_t z=0;
uint32_t PlayerScore;


const uint16_t *imagearray[4];
uint16_t sizearray [4];

struct sprite{
int32_t x, y;
int32_t w, h;
int32_t vx,vy;
int32_t oldx, oldy;
const uint16_t *blank;
const uint16_t *image;
const uint16_t *ball;
const uint16_t *white;
int life;
int32_t size;
};

typedef struct sprite sprite_t;
sprite_t ball;

struct hoop{
int32_t x, y;
int32_t w, h;
int32_t vx,vy;
int32_t oldx, oldy;
const uint16_t *blank;
const uint16_t *image;
int32_t size;
};


typedef struct hoop hoop_t;
hoop_t hoop;
void HoopInit1(void){
    hoop.oldx=30;
    hoop.oldy=60;

}
void HoopInit(void){
    hoop.y=60;
    hoop.w=60;
    hoop.h=60;
    hoop.vy=0;
    hoop.vx=0;
    hoop.blank=blackhoop;
    hoop.size=60;
    xarray[0]=15;
    xarray[1]=30;
    xarray[2]=50;


}


void BallInit(void){
    ball.x=0;
    ball.y=0;
    ball.w=32;
    ball.h=32;
    ball.vy=-3;
    ball.vx=0;
    ball.oldx=50;
    ball.oldy=159;
    ball.image=hoop40;
    ball.blank=black;
    ball.life=1;
    ball.size=20;
    ball.ball=basketblack20;
   // ball.white=basketwhite;

    }

void BallInit2(void){
    ball.x=ball.x;
    ball.y=0;
    ball.w=32;
    ball.h=32;
    ball.vy=-3;
    ball.vx=0;
    ball.oldx=50;
    ball.oldy=159;
    ball.image=hoop40;
    ball.blank=black;
    ball.life=1;
    ball.size=20;
    ball.ball=basketblack20;
   // ball.white=basketwhite;

    }


void ArrayInit(void){
    imagearray[0]=basketblack30;
    imagearray[1]=basketblack28;
    imagearray[2]=basketblack25;
    imagearray[3]=basketblack20;
    xarray[0]=50;
 ///   xarray[1]=70;
  ///  xarray[2]=90;
  ///  xarray[3]=40;
  ///  xarray[4]=20;
    sizearray[0]=30;
    sizearray[1]=28;
    sizearray[2]=25;
    sizearray[3]=20;


}
void BallMove(void){
    ball.y=ball.oldy;
   // ball.x=ball.oldx+ball.vx;
    ball.image=basketblack20;
    ball.size=20;
      i++;
    if(i>=4){
    i=3;
    }
}
    void BallMove2(void){
       ball.y=ball.oldy+ball.vy;
       ball.x=ball.oldx+ball.vx;
        ball.image=basketblack20;
        ball.size=20;
        i++;
          if(i>=4){
          i=3;
          }
    }


void ChangeHoopCoord(void){
    hoop.oldx=hoop.x;
}

    void ChangeCoord(void){
        ball.oldy=ball.y;
        ball.oldx=ball.x;
        ball.size=ball.size;
        //ball.ball=ball.image;
        hoop.x=xarray[z];
         z++;
         if(z>3){
         z=0;
                 }

}
uint32_t ScoreCounter=0; //score for Player 1
uint32_t ScoreCounter2=0; //score for Player 2
char Player1[] = "Score:";
char Time[]="Clock:";
char TimeSpan[]="Jam:";
char Player2[]="Score:";
char Jugador1[] = "Skor:";
char Jugador2[] = "Skor:";
char *playerarrayspan[2];
char *playerarray[2];


// do not need to use IIDX when there is a single interrupt source

//}
//get data from adc and pixels to move
//make old pixels black
//put ball on new pixels
//}
uint32_t noslide;

void PLL_Init(void){ // set phase lock loop (PLL)

  // Clock_Init40MHz(); // run this line for 40MHz
  Clock_Init80MHz(0);   // run this line for 80MHz
}

uint32_t M=1;
uint32_t Random32(void){
  M = 1664525*M+1013904223;
  return M;
}
uint32_t Random(uint32_t n){
  return (Random32()>>16)%n;
}
uint32_t shootball, stopshoot, shootballleft, shootballright;
// games  engine runs at 30Hz
uint32_t ConvertXPos(uint32_t Data){
    if (noslide==1){
        return ball.x;
    }
    return (128*Data)/4096;
}

uint32_t beforetime;
void Sound_Buzzer2(void);

void TIMG12_IRQHandler(void){uint32_t pos,msg,Ps;
static uint32_t last0=0;
uint32_t now0 = Switch_In();
  if((TIMG12->CPU_INT.IIDX) == 1){ // this will acknowledge
 // game engine goes here
      timecounter++;

        ball.x=ConvertXPos(ADCin());
     Flag=1;
     if((now0 == 0x80000)&&(last0 == 0)){
        // just touched, runs once per touch
            shootball=1;
        }
        if(((GPIOB->DIN31_0 & (1<<7)) == 0x80)&&(last0 == 0)){
            shootballleft=1;

        // just released, runs once per release

       }
        if(((GPIOA->DIN31_0 & (1<<8)) == 0x100)&&(last0 == 0)){
           shootballright=1;

             // just released, runs once per release

            }

        last0 = now0;
     }

    // 2) read input switches
    // 3) move sprites
    // 4) start sounds
    // 5) set semaphore
  int Lang_Flag;
    // NO LCD OUTPUT IN INTERRUPT SERVICE ROUTINES
    if(timecounter>=1212){
        if(Lang_Flag==1){

        if(playerarray[index2]==Player2){
//index2--;
GameOver=1;
noslide=0;
ArrayInit();
BallInit2();
  shootball=0;
  shootballleft=0;
 shootballright=0;
  timecounter=0;
Zero=1;

Sound_Buzzer2();


          }
        else if(playerarray[index2]!=Player2){
index2++;
noslide=0;
ArrayInit();
BallInit2();
  shootball=0;
  shootballleft=0;
   shootballright=0;
  timecounter=0;
  beforetime=(timecounter*33)/1000;
Zero=1;
PlayerScore=1;

Sound_Buzzer2();


          }

        }
        if(Lang_Flag==2){


        if(playerarrayspan[index2]==Jugador2){
           //   index2--;
            GameOver=1;
              noslide=0;
             ArrayInit();
              BallInit2();
                shootball=0;
                shootballleft=0;
                shootballright=0;
                timecounter=0;
Zero=1;

Sound_Buzzer2();



          }
        else if(playerarrayspan[index2]!=Jugador2){
           index2++;
           noslide=0;
          ArrayInit();
           BallInit2();
             shootball=0;
             shootballleft=0;
            shootballright=0;
             timecounter=0;
             beforetime=(timecounter*33)/1000;
              Zero=1;
              PlayerScore=1;

Sound_Buzzer2();


          }
    }


}

}







void ChangeHoopCoord(void);


uint8_t TExaS_LaunchPadLogicPB27PB26(void){
  return (0x80|((GPIOB->DOUT31_0>>26)&0x03));
}


int Lang_Flag=0;

int main2(void){ // main2
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_InvertDisplay(1);

  ST7735_DrawBitmap(110, 110, players2, 16, 50); // player ship bottom

  for(uint32_t t=500;t>0;t=t-5){
    SmallFont_OutVertical(t,104,6); // top left
    Clock_Delay1ms(50);              // delay 50 msec
  }
  ST7735_FillScreen(0x0000);   // set screen to black
  ST7735_SetCursor(1, 1);
  ST7735_OutString("GAME OVER");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Nice try,");
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Earthling!");
  ST7735_SetCursor(2, 4);
  ST7735_OutUDec(1234);
  while(1){
  }
}

// ALL ST7735 OUTPUT MUST OCCUR IN MAIN
int main(void){ // final main
    uint32_t timethen,timenow;
    uint32_t scoredone, scoredone2;
  __disable_irq();
  PLL_Init(); // set bus speed
  LaunchPad_Init();
  ST7735_InitPrintf();
    //note: if you colors are weird, see different options for
    // ST7735_InitR(INITR_REDTAB); inside ST7735_InitPrintf()
  ST7735_FillScreen(ST7735_BLACK);
  ST7735_InvertDisplay(1);

  ADCinit();     //PB18 = ADC1 channel 5, slidepot
  Switch_Init(); // initialize switches
  LED_Init();    // initialize LED
  Sound_Init();  // initialize sound
  //positioninit();

  TExaS_Init(0,0,&TExaS_LaunchPadLogicPB27PB26); // PB27 and PB26
    // initialize interrupts on TimerG12 at 30 Hz
  TimerG12_IntArm(80000000/30,2);
 // TimerG0_IntArm(period2, 1,3);
  ST7735_InvertDisplay(1);
  // initialize all data structures
  ArrayInit();
  BallInit2();
  HoopInit1();
  HoopInit();
  playerarray[0]= Player1;
  playerarray[1]= Player2;
  playerarrayspan[0]=Jugador1;
  playerarrayspan[1]=Jugador2;

  uint32_t Switch_InA(void);

  ST7735_SetTextColor(62266);
  ST7735_SetCursor(1, 1);
  ST7735_OutString("Welcome to Arcade");
  ST7735_SetCursor(1, 2);
  ST7735_OutString("Hoops!");
  ST7735_SetTextColor(64640);
  ST7735_SetCursor(1, 3);
  ST7735_OutString("Selamat datang di");
  ST7735_SetCursor(1, 4);
  ST7735_OutString("Arcade Hoops!");
  ST7735_SetCursor(1, 6);
  ST7735_SetTextColor(65480);
  ST7735_OutString("Push top button");
  ST7735_SetCursor(1, 7);
  ST7735_OutString("for English");
  ST7735_SetCursor(1, 8);
  ST7735_OutString("and bottom button");
  ST7735_SetCursor(1, 9);
  ST7735_OutString("for Indonesian.");
  ST7735_SetTextColor(9920);
  ST7735_SetCursor(1, 11);
  ST7735_OutString("Tekan tombol");
  ST7735_SetCursor(1, 12);
  ST7735_OutString("atas untuk");
  ST7735_SetCursor(1, 13);
  ST7735_OutString("Bahasa Inggris dan");
  ST7735_SetCursor(1, 14);
  ST7735_OutString("tombol bawah untuk");
  ST7735_SetCursor(1, 15);
  ST7735_OutString("Bahasa Indonesia.");
  uint32_t lang_select;
  while(Lang_Flag==0){
      lang_select = Switch_InA();
      if(lang_select == 0x8000){
          Lang_Flag=2;
      }
      if(lang_select == 0x20000){ //PA17
          Lang_Flag=1;
      }
  }
  ST7735_FillScreen(0x0000);
  void Ding(void);


__enable_irq();

beforetime=(timecounter*33)/1000;

while(1){

moveagain:

    if(GameOver==1){
        ST7735_FillScreen(0x0000);
        ST7735_SetCursor(1, 1);

    if(Lang_Flag==1){
        ST7735_OutString("Game Over");


    if(ScoreCounter>ScoreCounter2){
         ST7735_SetCursor(1, 2);
         ST7735_SetTextColor(64640);
         ST7735_OutString("Player 1 Wins!");


            }
    else if(ScoreCounter<ScoreCounter2){
          ST7735_SetCursor(1, 2);
          ST7735_SetTextColor(64640);
          ST7735_OutString("Player 2 Wins");


            }
            else if(ScoreCounter==ScoreCounter2){
                       ST7735_SetCursor(1, 2);
                       ST7735_SetTextColor(64640);
                       ST7735_OutString("Tie");


                   }


         }
        if(Lang_Flag==2){
            ST7735_OutString("Permainan berakhir");

            if(ScoreCounter>ScoreCounter2){
                ST7735_SetCursor(1, 2);
                ST7735_SetTextColor(64640);
                ST7735_OutString("Pemain 1 Menang");
            }


            else if(ScoreCounter<ScoreCounter2){
                ST7735_SetCursor(1, 2);
                ST7735_SetTextColor(64640);
                ST7735_OutString("Pemain 2 Menang");


            }
            else if(ScoreCounter==ScoreCounter2){
                       ST7735_SetCursor(1, 2);
                       ST7735_SetTextColor(64640);
                       ST7735_OutString("Dasi");


                   }
        }
        while(1){}

        }

if(Zero==0){

    if(Lang_Flag==1){
        ST7735_SetCursor(2, 12);
if(((timecounter*33)/1000)<=9){
    printf("%s", Time);
                       printf("%s", "00");
                       printf("%d", (timecounter*33)/1000);
}
else if (((timecounter*33)/1000)>9){
    if((timecounter*33)/1000==37){
           LED_On(GREENN);
       }
       if((timecounter*33)/1000==38){
             LED_Off(GREENN);
             LED_On(YELLOWW);
        }
       if((timecounter*33)/1000==39){
              LED_Off(YELLOWW);
              LED_On(REDD);
            ST7735_DrawBitmap(ball.x, ball.y, ball.blank, ball.size, ball.size);
       }

           printf("%s", Time);
                   printf("%s", "0");
                   printf("%d", (timecounter*33)/1000);
}

    }
    if(Lang_Flag==2){
        ST7735_SetCursor(2, 12);
        if(((timecounter*33)/1000)<=9){
            printf("%s", TimeSpan);
                               printf("%s", "00");
                               printf("%d", (timecounter*33)/1000);
        }
        else if (((timecounter*33)/1000)>9){
            if((timecounter*33)/1000==37){
                      LED_On(GREENN);
                  }
                  if((timecounter*33)/1000==38){
                        LED_Off(GREENN);
                        LED_On(YELLOWW);
                   }
                  if((timecounter*33)/1000==39){
                         LED_Off(YELLOWW);
                         LED_On(REDD);
                        ST7735_DrawBitmap(ball.x, ball.y, ball.blank, ball.size, ball.size);
                  }
                   printf("%s", TimeSpan);
                           printf("%s", "0");
                           printf("%d", (timecounter*33)/1000);
        }
               }


    }



else if(Zero==1){
    ST7735_DrawBitmap(ball.x, ball.y, ball.blank, ball.size, ball.size);
    LED_Off(REDD);
    if(Lang_Flag==1){
        Zero=0;
        ST7735_SetCursor(2, 12);

        printf("%s", Time);
        printf("%s", "000");
    }
    if(Lang_Flag==2){
        Zero=0;
        ST7735_SetCursor(2, 12);


        printf("%s", TimeSpan);
        printf("%s", "000");

    }
}
ST7735_SetCursor(2, 10);

if(Lang_Flag==1){
printf("%s", playerarray[index2] );

if(playerarray[index2]==Player2){
      printf("%2d", ScoreCounter2);

  }
else if(playerarray[index2]!=Player2){
    printf("%2d", ScoreCounter);


  }

}
if(Lang_Flag==2){
printf("%s", playerarrayspan[index2] );

if(playerarrayspan[index2]==Jugador2){
      printf("%2d", ScoreCounter2);


  }
else if(playerarrayspan[index2]!=Jugador2){
    printf("%2d", ScoreCounter);


  }


    }


     while(Flag==0){}
     Flag=0;
       if((((timecounter*33)/1000) - beforetime) == 2){
        beforetime=((timecounter*33)/1000);
        ST7735_DrawBitmap(hoop.oldx, hoop.oldy, blackhoop, hoop.size, hoop.size); // player ship bottom
        ST7735_DrawBitmap(hoop.x, hoop.y, hoop40, hoop.size, hoop.size); // player ship bottom
        ChangeHoopCoord();
    }
      // ChangeCoord();
     BallMove();

    /// while(PS){   }

    // ST7735_DrawBitmap(hoop.oldx, hoop.oldy, hoop40, hoop.size, hoop.size); // player ship bottom

     //ST7735_DrawBitmap(hoop.x, hoop.y, hoop40, hoop.size, hoop.size); // player ship bottom
     ST7735_DrawBitmap(ball.oldx, ball.oldy, ball.blank, ball.size, ball.size);
     ST7735_DrawBitmap(ball.x, ball.y, ball.image, ball.size, ball.size);
     //HoopInit();
     ChangeCoord();
     ST7735_DrawBitmap(110, 110, players1, 16, 50); // player ship bottom

if(PlayerScore){
    ST7735_DrawBitmap(110, 110, BLACK, 16, 50); // player ship bottom

    ST7735_DrawBitmap(110, 110, players2, 16, 50); // player ship bottom

}

 while(shootball==0 & shootballleft==0 & shootballright==0){goto moveagain;}
     if(shootball==1){
     shootball=1;
     noslide=1;
     ball.vy=-3;
     }
     else if(shootballleft==1){
         shootballleft=1;
          ball.vx=-2;
          ball.vy=-3;

     }
     else if(shootballright==1){
         shootballright=1;
         ball.vx=+2;
         ball.vy=-3;

     }


if(ball.y<=34){
    ST7735_DrawBitmap(hoop.oldx, hoop.oldy, hoop40, 60, 60); // player ship bottom

    if((ball.y==33) && (ball.x>=(hoop.oldx+5)) && (ball.x<=(hoop.oldx+25))){
        if(Lang_Flag == 1){
        if(playerarray[index2]==Player2){
            ScoreCounter2++;
            Ding();

        }
        ScoreCounter++;
        Ding();

        }
        if(Lang_Flag == 2){
        if(playerarrayspan[index2]==Jugador2){
            ScoreCounter2++;
            Ding();

        }
        ScoreCounter++;
        Ding();


        }
    }
    uint32_t wait=10000;
    ST7735_DrawBitmap(ball.oldx, ball.oldy, ball.blank, ball.size, ball.size );
    /// ST7735_DrawBitmap(0, 159, brickwall, 128, 160);

    ball.y=79;
    ST7735_DrawBitmap(ball.x, ball.y, ball.image, ball.size, ball.size );
    ball.vy=-ball.vy;
    ChangeCoord();
    BallMove();
    while(wait!=0){
        wait--;

    }











}



//  ST7735_DrawBitmap(ball.oldx, ball.oldy, ball.ball, ball.size, ball.size);
noslide=1;
ST7735_DrawBitmap(hoop.oldx, hoop.oldy, hoop40, 60, 60); // player ship bottom

ST7735_DrawBitmap(ball.oldx, ball.oldy, ball.blank, ball.size, ball.size);
BallMove2();

ST7735_DrawBitmap(ball.x, ball.y, ball.image, ball.size, ball.size);

ChangeCoord();
if(ball.vy==+3){
    ST7735_DrawBitmap(hoop.oldx, hoop.oldy, blackhoop, 60, 60); // player ship bottom

    ST7735_DrawBitmap(hoop.oldx, hoop.oldy, hoop40, 60, 60); // player ship bottom     !!
    ///ST7735_DrawBitmap(0, 159, brickwall, 128, 160);

    ST7735_DrawBitmap(ball.oldx, ball.oldy, ball.blank, ball.size, ball.size );
   ArrayInit();
    BallInit2();
    shootball=0;
    shootballleft=0;
    shootballright=0;
    noslide=0;



}

}


     }




     // delay 50 msec

    // wait for semaphore

       // clear semaphore
       // update ST7735R
// check for end game or level switch
