#include <Uefi.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/UefiLib.h>

#include "video.h"
#include "keyboard.h"
//#include "sound.h"
//#include "delay.h"

struct shot_t {
  INT8 x,y; // x==-1 -> inactive shot
};
#define NUMSHOTS 5
static struct shot_t shots[NUMSHOTS];
static INT8 ship,shipmove;
static INT8 shotcolor=4,shipcolor=5,aliencolor=3;
static BOOLEAN psychedelic=FALSE;

struct alien_t {
  INT8 x,y; // x==-1 -> dead alien
  INT8 xadd;
};
#define NUMALIENS 40
static struct alien_t aliens[NUMALIENS];
static UINT8 livealiens = NUMALIENS;

static BOOLEAN gameover, winner;

void changecolors(INT8 c)
{
        shotcolor+=c;
        if (shotcolor>15) shotcolor=1;
        else if (shotcolor<1) shotcolor=15;
        shipcolor+=c;
        if (shipcolor>15) shipcolor=1;
        else if (shipcolor<1) shipcolor=15;
        aliencolor+=c;
        if (aliencolor>15) aliencolor=1;
        else if (aliencolor<1) aliencolor=15;
}

void displaypause()
{
  CHAR16 key;
  BOOLEAN pressed;

  video_usecolor(6,1);
  video_putstring(30,10,L"    PAUSE    ");
  video_putstring(30,11,L"  PRESS \'P\'  ");
  video_update();
  do{
    key_decode(&key,&pressed);
  } while (!(pressed&&(key==L'p')));
};

void resetgame()
{
  gameover=FALSE;
  winner=FALSE;
  {
    UINT8 i;
    for (i=0;i<NUMSHOTS;++i) shots[i].x=-1;
  };
  ship=40;
  livealiens = NUMALIENS;
  shipmove=0;
  {
    UINT8 x,y;
    for (x=0;x<8;++x) {
      for (y=0;y<5;++y) {
	struct alien_t *a = &aliens[y*8+x];
	a->x=x*7+12;
	a->y=y*2;
	a->xadd = ( (x%2) ^ (y%2) ) ? -1 : 1;
      };
    };
  };
};

void display()
{
  // clear screen
  video_usecolor(0,0);
  video_fill(0,0,80,25,0);

  // display shots
  {
    UINT8 i;
    video_usecolor(shotcolor,0);
    for (i=0;i<NUMSHOTS;++i) {
      if (shots[i].x!=-1) {
	video_putchar(shots[i].x,shots[i].y,L'|');
      };
    };
  };

  // display ship
  {
    video_usecolor(shipcolor,0);
    video_putchar(ship-1,24,L'/');
    video_putchar(ship,23,L'_');
    video_putchar(ship+1,24,L'\\');
  };

  // display aliens
  {
    UINT8 i;
    video_usecolor(aliencolor,0);
    for (i=0;i<NUMALIENS;++i) {
      if (aliens[i].x!=-1) {
	video_putchar(aliens[i].x-1,aliens[i].y,L'-');
	video_putchar(aliens[i].x,aliens[i].y,L'*');
	video_putchar(aliens[i].x+1,aliens[i].y,L'-');
      };
    };
  };

  // show the picture
  video_update();
};

void keywork()
{
  CHAR16 key;
  BOOLEAN pressed;
  for (;;) {
    key_decode(&key,&pressed);
    switch (key) {
    case L'<':
      if (pressed) {
	shipmove=-1;
      }else{
	if (shipmove==-1) shipmove=0;
      };
      break;
    case L'>':
      if (pressed) {
	shipmove=1;
      }else{
	if (shipmove==1) shipmove=0;
      };
      break;
    case L' ':
      {
	UINT8 i;
	if (!pressed) break;
	for (i=0;i<NUMSHOTS;++i) {
	  if (shots[i].x==-1) {
	    shots[i].x=ship;
	    shots[i].y=24;
	    break;
	  };
	};
      };
      break;
    case L'x':
      return;
    case L'@':
      if (pressed) {
        //reboot();
	//TODO quit
      }
      break;
    case L'u':
      if (pressed) {
        changecolors(+1);
      }
      break;
    case L'd':
      if (pressed) {
        changecolors(-1);
      }
      break;
    case L'0':
      if (pressed) {
        if (psychedelic==FALSE) psychedelic=TRUE;
        else {
          psychedelic=FALSE;
          shotcolor=4,shipcolor=5,aliencolor=3;
        }
      }
      break;
    case L'p':
      if (pressed) {
        displaypause();
      }
      break;
    };
  };
};

void calculate()
{
  UINT8 i,k;
  BOOLEAN foundalien=FALSE;

  ship+=shipmove;
  if (ship<1) ship=1;
  if (ship>78) ship=78;

  for (i=0;i<NUMSHOTS;++i) {
    if (shots[i].x!=-1) {
      shots[i].y--;
	if (shots[i].y==-1) {
	  shots[i].x=-1;
	};
    };
  };
  for (i=0;i<NUMALIENS;++i) {
    if (aliens[i].x!=-1) {
      foundalien=TRUE;
      for (k=0;k<NUMSHOTS;++k) {
	if (shots[k].x!=-1) {
	  if (shots[k].y==aliens[i].y) {
	    if ( (shots[k].x<=aliens[i].x+1) &&
		 (shots[k].x>=aliens[i].x-1) ) {
	      shots[k].x=-1;
	      aliens[i].x=-1;
          livealiens--;
	      goto nextalien;
	    };
	  };
	};
      };
      
      aliens[i].x += aliens[i].xadd;
      if (aliens[i].x<1) {
	aliens[i].x=1;
	aliens[i].xadd=1;
	aliens[i].y++;
      }else if (aliens[i].x>78) {
	aliens[i].x=78;
	aliens[i].xadd=-1;
	aliens[i].y++;
      };
      if (aliens[i].y>=20) gameover=TRUE;
    };
  nextalien:;
  };

  if (!foundalien) gameover = winner = TRUE;
};

/*void sounder()
{
  {
    UINT8 i;
    for (i=0;i<NUMSHOTS;++i) {
      if (shots[i].x!=-1) {
	sound_freq(shots[i].y*100+100);
      }else{
	sound_nosound();
      };
      delay_wait(1193180/1000+livealiens*500);
    };
  };
  sound_nosound();
};*/

void displaygameover()
{
  CHAR16 key;
  BOOLEAN pressed;

  video_usecolor(6,1);
  video_putstring(30,10,L"  GAME OVER  ");
  video_putstring(30,11,winner ? L"   YOU WIN   " : L"  YOU LOSE   ");
  video_putstring(30,12,L"  PRESS ESC  ");
  video_update();
  do{
    key_decode(&key,&pressed);
  } while (!(pressed&&(key==L'@')));
};

void game()
{
  //   video_blinkchars(FALSE);
  video_hidecursor();

  //for (;;) {
    resetgame();

    while (!gameover) {
      display();
      //sounder();
      keywork();
      calculate();
      if (psychedelic==TRUE) changecolors(+1);
      
      gBS->Stall(1000);  // 1 millisecond
    };

    displaygameover();
  //};
};
