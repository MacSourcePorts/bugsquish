/*
  bugsquish.c
  
  Bug Squish

  by Bill Kendrick
  bill@newbreedsoftware.com
  http://www.newbreedsoftware.com/
  
  March 19, 2000 - April 20, 2002
*/


#ifndef EMBEDDED
#define VERSION "0.0.6"
#else
#define VERSION "0.0.6-embedded"
#endif


/* #ncludes: */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <SDL.h>
#include <SDL_image.h>

#ifndef NOSOUND
#include <SDL_mixer.h>
#endif

#ifndef EMBEDDED
#include "data/images/flyswatter.xbm"
#include "data/images/flyswatter-mask.xbm"
#endif


#ifdef LINUX
#define LINUX_DAT
#endif
#ifdef EMBEDDED
#define LINUX_DAT
#endif


#define POINTS_FOR_BONUS_BLOOD 10000 /* How many points til bonus blood */
#define MAX_BUGS_TO_START 8          /* When beginning level, limit to this */
#define CHANCE_OF_EXTRA 100          /* How often to add extras */
#define CHANCE_OF_OUCH 20            /* How often, when being sucked, to say */
#define CHANCE_OF_FAST_BUG 10        /* How often, when adding bugs, fast */
#define CHANCE_OF_SPORATICNESS 20    /* How often to change direction */


#ifndef EMBEDDED
#define WIDTH 640
#define HEIGHT 480
#else
#define WIDTH 240
#define HEIGHT 320
#endif


/* Image enumerations: */

enum {
  IMG_PRESENTS,
  IMG_TITLE,
  IMG_ARM1,
  IMG_ARM2,
  IMG_ARM3,
  IMG_ARM4,
  IMG_ARM5,
  IMG_GAUGE_FULL,
  IMG_GAUGE_EMPTY,
  IMG_SCOREBOX,
  IMG_HIGHSCORE,
  IMG_WAVEBOX,
  IMG_BONUS,
  IMG_PAUSED,
  IMG_NUMBERS,
  IMG_LARGE0,
  IMG_LARGE1,
  IMG_LARGE2,
  IMG_LARGE3,
  IMG_LARGE4,
  IMG_LARGE5,
  IMG_LARGE6,
  IMG_LARGE7,
  IMG_LARGE8,
  IMG_LARGE9,
  IMG_BUG0A,
  IMG_BUG0B,
  IMG_BUG0C,
  IMG_BUG1A,
  IMG_BUG1B,
  IMG_BUG1C,
  IMG_BUG2A,
  IMG_BUG2B,
  IMG_BUG2C,
  IMG_BUG3A,
  IMG_BUG3B,
  IMG_BUG3C,
  IMG_BUG4A,
  IMG_BUG4B,
  IMG_BUG4C,
  IMG_BLOOD,
  IMG_BLOOD_SQUISHED,
  IMG_MULTIPLIER,
  IMG_MULTIPLIER_SQUISHED,
  IMG_TIMES2,
#ifdef EMBEDDED
  IMG_START_CONT,
#endif
  NUM_IMAGES
};


/* Image filenames: */

const char * image_names [NUM_IMAGES] = {
  DATA_PREFIX "images/presents.png",
  DATA_PREFIX "images/title.png",
  DATA_PREFIX "images/arm1.png",
  DATA_PREFIX "images/arm2.png",
  DATA_PREFIX "images/arm3.png",
  DATA_PREFIX "images/arm4.png",
  DATA_PREFIX "images/arm5.png",
  DATA_PREFIX "images/gauge-full.png",
  DATA_PREFIX "images/gauge-empty.png",
  DATA_PREFIX "images/scorebox.png",
  DATA_PREFIX "images/highscore.png",
  DATA_PREFIX "images/wavebox.png",
  DATA_PREFIX "images/bonus.png",
  DATA_PREFIX "images/paused.png",
  DATA_PREFIX "images/numbers.png",
  DATA_PREFIX "images/large0.png",
  DATA_PREFIX "images/large1.png",
  DATA_PREFIX "images/large2.png",
  DATA_PREFIX "images/large3.png",
  DATA_PREFIX "images/large4.png",
  DATA_PREFIX "images/large5.png",
  DATA_PREFIX "images/large6.png",
  DATA_PREFIX "images/large7.png",
  DATA_PREFIX "images/large8.png",
  DATA_PREFIX "images/large9.png",
  DATA_PREFIX "images/bug0a.png",
  DATA_PREFIX "images/bug0b.png",
  DATA_PREFIX "images/bug0c.png",
  DATA_PREFIX "images/bug1a.png",
  DATA_PREFIX "images/bug1b.png",
  DATA_PREFIX "images/bug1c.png",
  DATA_PREFIX "images/bug2a.png",
  DATA_PREFIX "images/bug2b.png",
  DATA_PREFIX "images/bug2c.png",
  DATA_PREFIX "images/bug3a.png",
  DATA_PREFIX "images/bug3b.png",
  DATA_PREFIX "images/bug3c.png",
  DATA_PREFIX "images/bug4a.png",
  DATA_PREFIX "images/bug4b.png",
  DATA_PREFIX "images/bug4c.png",
  DATA_PREFIX "images/blood.png",
  DATA_PREFIX "images/blood-squished.png",
  DATA_PREFIX "images/mult.png",
  DATA_PREFIX "images/mult-squished.png",
  DATA_PREFIX "images/times2.png"
#ifdef EMBEDDED
  ,
  DATA_PREFIX "images/start_cont.png"
#endif
};


/* Bug enumerations: */

enum {
  BUG_MOSQUITO,
  BUG_TICK,
  BUG_ROACH,
  BUG_STICK,
  BUG_PILL,
  NUM_BUGS
};


/* Sound enumerations: */

enum {
  SND_SQUISH1,
  SND_SQUISH2,
  SND_OUCH,
  SND_UGH,
  SND_GLUG,
  SND_BONUS,
  SND_AH,
  SND_HIGHSCORE,
  NUM_SOUNDS
};


/* Sound filenames: */

const char * sound_names[NUM_SOUNDS] = {
  DATA_PREFIX "sounds/squish2.wav",
  DATA_PREFIX "sounds/squish1.wav",
  DATA_PREFIX "sounds/ouch.wav",
  DATA_PREFIX "sounds/ugh.wav",
  DATA_PREFIX "sounds/glug.wav",
  DATA_PREFIX "sounds/bonus.wav",
  DATA_PREFIX "sounds/ah.wav",
  DATA_PREFIX "sounds/highscore.wav"
};


/* Music filenames: */

#define MUS_TITLE DATA_PREFIX "music/corpses.mod"
#define MUS_GAME DATA_PREFIX "music/adventures.mod"


/* Bug type: */

typedef struct bug_type {
  int alive, kind, x, y, xm, ym, xmm, target_y;
} bug_type;

typedef struct splat_type {
  int active, img, x, y;
#ifdef EMBEDDED
  int timer;
#endif
} splat_type;

typedef struct extra_type {
  int active, kind, x, y, xm, ym;
} extra_type;


#define MAX_BUGS 64
#define MAX_SPLATS 256


/* Local function prototypes: */

void setup(void);
int title(void);
int game(void);
void seticon(void);
int pause_screen(void);
void playsound(int snd, int chan);
void resetlevel(void);
void addbug(void);
void addsplat(int x, int y, int kind);
void intro(void);
void drawbonusnumbers(int num);
void incrementscore(int amt);
void addblood(void);
void drawdigits(int v, int x, int y);
void loaddata(void);
void savedata(void);
FILE * opendata(char * mode);
void getargs(int argc, char * argv[]);
void usage(int ret);
#ifdef EMBEDDED
FILE * openstate(char * mode);
void loadstate(void);
void savestate(void);
#endif

/* Global variables: */

SDL_Surface * screen;
SDL_Surface * images[NUM_IMAGES];
int use_fullscreen, use_sound;
int level, score, highscore, highlevel, blood, bugs_added, multiplier,
  has_highscore;
int num_bugs, next_level, old_blood, bonus;
#ifdef EMBEDDED
int can_continue;
#endif
bug_type bugs[MAX_BUGS];
splat_type splats[MAX_SPLATS];
extra_type extra;

#ifndef NOSOUND
Mix_Chunk * sounds[NUM_SOUNDS];
Mix_Music * mus_title, * mus_game;
#endif


/* --- MAIN --- */

int main(int argc, char * argv[])
{
  int done;
  
  
  /* Check for arguments: */
  
  getargs(argc, argv);


  /* Setup: */
    
  setup();
  
    
  /* Load data: */
  
  loaddata();


#ifdef EMBEDDED

  /* Load game state: */

  loadstate();
#endif
  
  
  /* Stress warning: */
  
  printf("\n"
	 "IMPORTANT NOTE!!!\n"
	 "-----------------\n"
	 "This game requires a lot of mousing.  Please play responsibly\n"
	 "to ensure you do not damage your wrist!  Use the game's Pause\n"
	 "function to temporarily stop your game so you may take breaks.\n\n");
  
  
  /* Intro screen: */
  
  intro();
  
  
  /* MAIN FUNCTION LOOP: */
  
  done = 0;
  
  do
    {
      done = title();
      
      if (!done)
	done = game();
    }
  while (!done);
  
  
  /* Save data: */
  
  savedata();
  

#ifdef EMBEDDED

  /* Save game state: */

  savestate();
#endif
 

  /* Quit and exit: */
  
  SDL_Quit();
  
  return(0);
}


/* Game function! */

int game(void)
{
  SDL_Rect src, dest;
  SDL_Event event;
  int done, quit, gameover, i, frame, mouse_x, mouse_y, j;
  
  
  /* Start the game! */
 
#ifndef EMBEDDED
  level = 1;
  score = 0;
  has_highscore = 0;
  multiplier = 1;
  blood = (images[IMG_GAUGE_FULL] -> h);
  old_blood = 0;
  bonus = 0;
  next_level = 0;

  resetlevel();
#else
  if (can_continue == 0)
  {
    level = 1;
    score = 0;
    has_highscore = 0;
    multiplier = 1;
    blood = (images[IMG_GAUGE_FULL] -> h);
    old_blood = 0;
    bonus = 0;
    next_level = 0;
    
    resetlevel();
  }

  can_continue = 1;
#endif
  
  mouse_x = 0;
  mouse_y = 0;
  
  
  /* MAIN GAME LOOP! */
  
  done = 0;
  gameover = 0;
  quit = 0;
  frame = 0;
    
  do
    {
      frame++;
      
      /* Handle events: */
      
      while (SDL_PollEvent(&event) > 0)
	{
	  if (event.type == SDL_QUIT)
	    {
	      /* WM Quit - Quit! */
	      
	      quit = 1;
	    }
	  else if (event.type == SDL_KEYDOWN)
	    {
	      /* Keypress! ESCAPE - Quit, Space - Pause!*/
	      
	      if (event.key.keysym.sym == SDLK_ESCAPE)
		done = 1;
	      else if (event.key.keysym.sym == SDLK_SPACE)
		{
		  /* Pause! */
		  
		  quit = pause_screen();
		}
	    }
#ifndef EMBEDDED
          else if (event.type == SDL_MOUSEMOTION)
            {
              mouse_x = event.motion.x;
              mouse_y = event.motion.y;
            }
	  else if (event.type == SDL_MOUSEBUTTONDOWN)
	    {
#else
          else if (event.type == SDL_MOUSEMOTION)
            {
              mouse_x = event.motion.x;
              mouse_y = event.motion.y;

	      event.button.x = mouse_x;
	      event.button.y = mouse_y;
#endif
	      if (!gameover)
		{
		  /* Mouse click! - Squish! */
		  
		  for (i = 0; i < MAX_BUGS; i++)
		    {
		      if (bugs[i].alive &&
			  event.button.x >= bugs[i].x - 16 &&
			  event.button.x <= bugs[i].x + 32 &&
			  event.button.y >= bugs[i].y - 16 &&
			  event.button.y <= bugs[i].y + 32)
			{
			  bugs[i].alive = 0;
			  playsound(SND_SQUISH1 + (rand() % 2), 0);
			  addsplat(bugs[i].x, bugs[i].y,
				   (IMG_BUG0C + bugs[i].kind * 3));
			  incrementscore((HEIGHT - bugs[i].y) / 4);
			}
		    }
		  
		  
		  /* Grab an extra? */
		  
		  if (extra.active &&
		      event.button.x >= extra.x - 16 &&
		      event.button.x <= extra.x + 32 &&
		      event.button.y >= extra.y - 16 &&
		      event.button.y <= extra.y + 32)
		    {
		      if (extra.kind == 0)
			{
			  /* Extra blood! */
			  
			  addblood();
			}
		      else
			{
			  /* Multiplier! */
			  
			  if (multiplier == 1)
			    {
			      multiplier = 2;
			      playsound(SND_BONUS, 1);
			    }
			  else
			    playsound(SND_GLUG, 1);
			}
		      
		      addsplat(extra.x, extra.y,
			       IMG_BLOOD_SQUISHED + extra.kind * 2);
		      extra.active = 0;
		    }
		}
	    }
	}
      
      
      /* Move all bugs: */
      
      num_bugs = 0;
      
      for (i = 0; i < MAX_BUGS; i++)
	{
	  if (bugs[i].alive)
	    {
	      num_bugs++;
	      
	      /* Move: */
	      
	      bugs[i].x = bugs[i].x + bugs[i].xm;

#ifndef EMBEDDED
	      bugs[i].y = bugs[i].y + bugs[i].ym;
#else
	      if (bugs[i].ym == 1 && (frame % 2) == 0)
		bugs[i].y++;
	      else
		bugs[i].y = bugs[i].y + (bugs[i].ym / 2);
#endif
	      
	      
	      if (bugs[i].kind == BUG_MOSQUITO)
		{
		  /* Wave back and forth? */
		  
		  bugs[i].xm = bugs[i].xm + bugs[i].xmm;
		  
		  if (bugs[i].xm < -8)
		    bugs[i].xmm = 1;
		  else if (bugs[i].xm > 8)
		    bugs[i].xmm = -1;
		}
	      else if (bugs[i].kind == BUG_STICK)
		{
		  /* Move horizontally sporatically? */
		  
		  if (rand() % CHANCE_OF_SPORATICNESS)
		    bugs[i].xm = (rand() % (level * 2)) - level;
		}
	      else if (bugs[i].kind == BUG_ROACH)
		{
		  /* Move vertically sporatically? */
		  
		  if (rand() % CHANCE_OF_SPORATICNESS)
		    bugs[i].ym = (rand() % (level * 2)) - (level / 2);
		}
              else if (bugs[i].kind == BUG_PILL)
                {
                  if (bugs[i].x > mouse_x && bugs[i].xm < 8)
                    bugs[i].xm++;
                  else if (bugs[i].x <= mouse_x && bugs[i].xm > -8)
                    bugs[i].xm--;
                }
	      
	      
	      /* Land? */
	      
	      if (bugs[i].y >= bugs[i].target_y && bugs[i].ym > 0)
		{
		  bugs[i].alive = bugs[i].target_y;
		  bugs[i].xm = 0;
		  bugs[i].xmm = 0;
		  bugs[i].ym = 0;
		  
		  playsound(SND_OUCH, 2);
		}
	      
	      
	      /* Fly away? */
	      
	      if (gameover)
		{
		  if (bugs[i].ym >= 0)
		    bugs[i].ym = -2;
		  
		  if (bugs[i].y <= -32)
		    bugs[i].alive = 0;
		}
	      
	      
	      /* Keep in horizontal bounds: */
	      
	      if (bugs[i].x < images[IMG_GAUGE_FULL] -> w)
		{
		  bugs[i].x = images[IMG_GAUGE_FULL] -> w;
		  bugs[i].xm = 1;
		}
	      else if (bugs[i].x > WIDTH - 33)
		{
		  bugs[i].x = WIDTH - 33;
		  bugs[i].xm = -1;
		}
	      
	      
	      /* Suck blood! */
	      
	      if (!gameover && bugs[i].y >= bugs[i].target_y)
		{
		  if ((frame % 4) == 0)
		    {
		      blood--;
		      
#ifndef NOSOUND
		      if (use_sound)
			{
			  if ((rand() % CHANCE_OF_OUCH) == 0 && !gameover &&
			      !Mix_Playing(2))
			    {
			      playsound(SND_OUCH, 2);
			    }
			}
#endif
		      
		      if (blood <= 0)
			{
			  blood = 0;
			  
			  if (!gameover)
			    {
			      gameover = 1;
#ifdef EMBEDDED
			      can_continue = 0;
#endif
			      playsound(SND_UGH, 2);
			      
			      
			      /* Make bugs fly away (all done! yum!) */
			      
			      for (i = 0; i < MAX_BUGS; i++)
				bugs[i].ym = -(rand() % 5) - 3;
			    }
			}
		    }
		}
	    }
	}


      /* Handle extra: */
      
      if (extra.active)
	{
	  /* Move it: */
	  
	  extra.x = extra.x + extra.xm;
	  extra.y = extra.y + extra.ym;
	  
	  
	  /* Out of bounds? */
	  
	  if (extra.x < -32 || extra.x > WIDTH ||
	      extra.y < -32 || extra.y > HEIGHT)
	    {
	      extra.active = 0;
	    }
	}
      else if ((rand() % CHANCE_OF_EXTRA) == 0 && !next_level && !gameover)
	{
	  /* Add an extra! */
	  
	  extra.active = 1;
	  extra.kind = (rand() % 2);
	  
	  if ((rand() % 2) == 0)
	    {
	      /* From the top! */
	      
	      extra.y = -32;
	      extra.x = (rand() % 608);
	      
	      extra.xm = (rand() % 32) - 16;
	      extra.ym = (rand() % 16);
	    }
	  else
	    {
	      /* From one of the sides! */
	      
	      if ((rand() % 2) == 0)
		{
		  /* Left: */
		  
		  extra.x = -32;
		  extra.xm = (rand() % 16);
		}
	      else
		{
		  /* Left: */
		  
		  extra.x = WIDTH;
		  extra.xm = -(rand() % 16);
		}
	      
	      extra.ym = (rand() % 32) - 16;
	    }
	}
      
      
      /* Quit if game's over! */
      
      if (num_bugs == 0 && gameover)
	done = 1;
      
      
      /* Erase screen: */
      
      SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 255, 255, 255));
      
      
      /* Draw arm: */
      
      dest.x = 0;
#ifndef EMBEDDED
      dest.y = HEIGHT - images[IMG_ARM1] -> h;
#else
      dest.y = HEIGHT - images[IMG_ARM1] -> h - 40;
#endif
      dest.w = WIDTH;
      dest.h = images[IMG_ARM1] -> h;
      
      if (!next_level)
	{
	  /* Faded, based on blood level: */
	  
	  SDL_BlitSurface(images[(IMG_ARM5 -
				  ((blood * 5) /
				   ((images[IMG_GAUGE_FULL] -> h) + 1)))],
			  NULL, screen, &dest);
	}
      else
	{
	  /* Blood bonus happening - show based on recorded blood level: */
	  
	  SDL_BlitSurface(images[(IMG_ARM5 -
				  ((old_blood * 5) /
				   ((images[IMG_GAUGE_FULL] -> h) + 1)))],
			  NULL, screen, &dest);
	}
      

      /* Draw all splats: */

      for (i = 0; i < MAX_SPLATS; i++)
	{
	  if (splats[i].active)
	    {
	      dest.x = splats[i].x;
	      dest.y = splats[i].y;
	      dest.w = 32;
	      dest.h = 32;
	    
#ifndef EMBEDDED
	      SDL_BlitSurface(images[splats[i].img], NULL,
		              screen, &dest);
#else
	      if (splats[i].timer > 10)
	      {
	        SDL_BlitSurface(images[splats[i].img], NULL,
			        screen, &dest);
	      }
	      else
	      {
		for (j = (splats[i].timer % 2); j < 32; j = j + 2)
		{
	          dest.x = splats[i].x;
		  dest.y = splats[i].y + j;
		  dest.w = 32;
	          dest.h = 1;

		  src.x = 0;
		  src.y = j;
		  src.w = 32;
		  src.h = 1;

		  SDL_BlitSurface(images[splats[i].img], &src,
				  screen, &dest);
		}
	      }

              splats[i].timer--;
		
	      if (splats[i].timer == 0)
	        splats[i].active = 0;
#endif
	    }
	}
      
      
      /* Draw all bugs: */

      for (i = 0; i < MAX_BUGS; i++)
	{
	  if (bugs[i].alive)
	    {
	      dest.x = bugs[i].x;
	      dest.y = bugs[i].y;
	      dest.w = 32;
	      dest.h = 32;
	      
	      if (bugs[i].ym != 0)
		SDL_BlitSurface(images[IMG_BUG0A + bugs[i].kind * 3 +
				      ((frame + i) % 2)], NULL,
				screen, &dest);
	      else
		SDL_BlitSurface(images[IMG_BUG0A + bugs[i].kind * 3 +
				      ((frame / 8) % 2)], NULL,
				screen, &dest);
	    }
	}

      
      /* Draw extra: */
      
      if (extra.active)
	{
	  dest.x = extra.x;
	  dest.y = extra.y;
	  dest.w = 32;
	  dest.h = 32;
	  
	  SDL_BlitSurface(images[IMG_BLOOD + extra.kind * 2], NULL,
			  screen, &dest);
	}
      
      
      /* Draw blood gauge: */
      
      if (blood > 0)
	{
	  /* (Full part) */
	  
	  src.x = 0;
	  src.y = (images[IMG_GAUGE_FULL] -> h) - blood;
	  src.w = (images[IMG_GAUGE_FULL] -> w);
	  src.h = blood;
	  
	  dest.x = src.x;
	  dest.y = src.y;
	  dest.w = src.w;
	  dest.h = src.h;
	  
	  SDL_BlitSurface(images[IMG_GAUGE_FULL], &src, screen, &dest);
	}
      
      
      if (blood < (images[IMG_GAUGE_EMPTY] -> h) && blood >= 0)
	{
	  /* (Empty part) */
	  
	  src.x = 0;
	  src.y = 0;
	  src.w = (images[IMG_GAUGE_EMPTY] -> w);
	  src.h = (images[IMG_GAUGE_EMPTY] -> h) - blood;
	  
	  dest.x = src.x;
	  dest.y = src.y;
	  dest.w = src.w;
	  dest.h = src.h;
	  
	  SDL_BlitSurface(images[IMG_GAUGE_EMPTY], &src, screen, &dest);
	}
      
      
      /* Multiplier value: */

      if (multiplier == 2)
	{
	  dest.x = ((images[IMG_GAUGE_FULL] -> w) / 2) - 8;
	  dest.y = (images[IMG_GAUGE_FULL] -> h) + 2;
	  dest.w = 16;
	  dest.h = 16;
	  
	  SDL_BlitSurface(images[IMG_TIMES2], NULL, screen, &dest);
	}
      
      
      /* Score: */
      
      dest.x = 0;
      dest.y = HEIGHT - 40;
      dest.w = 128;
      dest.h = 40;
      
      SDL_BlitSurface(images[IMG_SCOREBOX], NULL, screen, &dest);
      
      drawdigits(score, 2, HEIGHT - 18);
      
      
      /* High score bubble? */
      
      if (has_highscore)
	{
	  dest.x = 110;
	  dest.y = HEIGHT - 18;
	  dest.w = 16;
	  dest.h = 16;
	  
	  SDL_BlitSurface(images[IMG_HIGHSCORE], NULL, screen, &dest);
	}


      /* Wave: */
      
      dest.x = WIDTH - 64;
      dest.y = HEIGHT - 40;
      dest.w = 64;
      dest.h = 40;
      
      SDL_BlitSurface(images[IMG_WAVEBOX], NULL, screen, &dest);
      
      drawdigits(level, WIDTH - 62, HEIGHT - 18);
      
      
      /* Go to next level? */
      
      if (num_bugs == 0 && bugs_added >= level * 4 && !gameover)
	{
	  level++;
	  
	  if (level > 32)
	    level = 32;
	  
	  next_level = 1;
	  old_blood = blood;
	  bonus = 0;
	}
      
      
      /* Add more bugs: */
      
      if (((rand() % (35 - level)) == 0 || num_bugs == 0)
	  && bugs_added < level * 4 && !next_level && !gameover)
	addbug();
      
      
      if (next_level == 1)
	{
	  /* Grab bonus for blood: */
	  
	  if (blood > 5)
	    {
	      bonus = bonus + 5 * multiplier;
	      blood = blood - 5;
	    }
	  else
	    {
	      blood = 0;
	      bonus = bonus + blood * multiplier;
	    }

	  
#ifndef NOSOUND
	  if (use_sound)
	    {
	      if (!Mix_Playing(1))
		{
		  playsound(SND_GLUG, 1);
		}
	    }
#endif
	  
	  
	  /* Draw bonus box: */
	  
	  dest.x = (WIDTH - images[IMG_BONUS]->w) / 2;
	  dest.y = (HEIGHT - images[IMG_BONUS]->h) / 2;
	  
	  SDL_BlitSurface(images[IMG_BONUS], NULL,
			  screen, &dest);
	  
	  drawbonusnumbers(bonus);
	  
	  
	  /* No more blood - go to next level! */
	  
	  if (blood <= 0)
	    {
	      blood = old_blood;
	      next_level = 0;
	      
	      resetlevel();
	      playsound(SND_BONUS, 1);
	      
	      incrementscore(bonus);
	    }
	}
      
      
      /* Update screen, play music, keep framerate stable: */
      
      SDL_Flip(screen);
      SDL_Delay(30);
      
#ifndef NOSOUND
      if (use_sound)
	{
	  if (!Mix_PlayingMusic())
	    {
	      Mix_PlayMusic(mus_game, 0);
	    }
	}
#endif
    }
  while (!done && !quit);
  

#ifndef NOSOUND
  if (use_sound)
    Mix_HaltMusic();
#endif

  return(quit);
}


/* Setup function: */

void setup(void)
{
  SDL_Surface * image;
  int i;
  SDL_Rect dest;
  Uint8 temp_bitmap[32], temp_bitmask[32];
  Uint8 b;

  
  /* Init SDL Video: */
  
  if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
      fprintf(stderr,
              "\nError: I could not initialize video!\n"
              "The Simple DirectMedia error that occured was:\n"
              "%s\n\n", SDL_GetError());
      exit(1);
    }

  
  /* Init SDL Audio: */
  
  if (use_sound == 1)
    {
      if (SDL_Init(SDL_INIT_AUDIO) < 0)
        {
          fprintf(stderr,
                  "\nWarning: I could not initialize audio!\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", SDL_GetError());
          use_sound = 0;
        }
    }
  
  
  /* Open sound: */
  
#ifndef NOSOUND
  if (use_sound == 1)
    {
      if (Mix_OpenAudio(22050, AUDIO_S16, 2, 256) < 0)
        {
          fprintf(stderr,
                  "\nWarning: I could not set up audio for 22050 Hz "
                  "16-bit stereo.\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", SDL_GetError());
          use_sound = 0;
        }
    }
#endif


  /* Open display: */
  
  if (use_fullscreen == 1)
    {
      screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16,
		                SDL_FULLSCREEN | SDL_HWSURFACE);
      if (screen == NULL)
        {
          fprintf(stderr,
                  "\nWarning: I could not set up fullscreen video for "
                  "%d x %d mode.\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", WIDTH, HEIGHT, SDL_GetError());
          use_fullscreen = 0;
        }
    }
  
  if (use_fullscreen == 0)
    {
      screen = SDL_SetVideoMode(WIDTH, HEIGHT, 16, 0);
      
      if (screen == NULL)
        {
          fprintf(stderr,
                  "\nError: I could not set up video for %d x %d mode.\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", WIDTH, HEIGHT, SDL_GetError());
          exit(1);
        }
    }
  
  
  /* Set icon image: */
  
  seticon();
  
  
  /* Set window manager stuff: */
  
  SDL_WM_SetCaption("Bug Squish", "Bug Squish");


#ifndef EMBEDDED
  /* Make cursor: */
  
  for (i = 0; i < 32; i++)
    {
      b = flyswatter_bits[i];
      
      temp_bitmap[i] = (((b & 0x01) << 7) |
			((b & 0x02) << 5) |
			((b & 0x04) << 3) |
			((b & 0x08) << 1) |
			((b & 0x10) >> 1) |
			((b & 0x20) >> 3) |
			((b & 0x40) >> 5) |
			((b & 0x80) >> 7));
      
      b = flyswatter_mask_bits[i];
      
      temp_bitmask[i] = (((b & 0x01) << 7) |
			 ((b & 0x02) << 5) |
			 ((b & 0x04) << 3) |
			 ((b & 0x08) << 1) |
			 ((b & 0x10) >> 1) |
			 ((b & 0x20) >> 3) |
			 ((b & 0x40) >> 5) |
			 ((b & 0x80) >> 7));
    }
  
  SDL_SetCursor(SDL_CreateCursor(temp_bitmap, temp_bitmask, 16, 16, 0, 0));
#endif
  
  
  /* Load graphics: */
  
  for (i = 0; i < NUM_IMAGES; i++)
    {
      /* Load image file: */
     
#ifndef EMBEDDED
      image = IMG_Load(image_names[i]);
#else
      image = SDL_LoadBMP(image_names[i]);
#endif

      if (image == NULL)
        {
          fprintf(stderr,
                  "\nError: I couldn't load a graphics file:\n"
                  "%s\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", image_names[i], SDL_GetError());
          exit(1);
        }
      
      
      /* Set transparency: */
     
#ifdef EMBEDDED
      images[i] = SDL_DisplayFormat(image);
      if (images[i] == NULL)
        {
          fprintf(stderr,
                  "\nError: I couldn't convert a file to the display format:\n"
                  "%s\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", image_names[i], SDL_GetError());
          exit(1);
        }
      
      if (SDL_SetColorKey(images[i], (SDL_SRCCOLORKEY | SDL_RLEACCEL),
                          SDL_MapRGB(images[i] -> format,
                                     0xFF, 0xFF, 0xFF)) == -1)
        {
          fprintf(stderr,
                  "\nError: I could not set the color key for the file:\n"
                  "%s\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", image_names[i], SDL_GetError());
          exit(1);
        }

      SDL_FreeSurface(image);

#else
      images[i] = image;
#endif
      
      
      /* Draw percentage bar: */
      
      dest.x = 0;
      dest.y = HEIGHT - 10;
      dest.w = (WIDTH * i) / NUM_IMAGES;
      dest.h = 10;
      
      SDL_FillRect(screen, &dest, SDL_MapRGB(screen->format, 255, 255, 255));
      SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);
      SDL_Delay(1);
    }


#ifndef NOSOUND
#ifdef EMBEDDED
  if (use_sound == 1)
  {
    Mix_Volume(-1, MIX_MAX_VOLUME);
  }
#endif
#endif
  
  
#ifndef NOSOUND
  if (use_sound == 1)
    {
      /* Load sounds: */
      
      for (i = 0; i < NUM_SOUNDS; i++)
        {
          sounds[i] = Mix_LoadWAV(sound_names[i]);
          if (sounds[i] == NULL)
            {
              fprintf(stderr,
                      "\nError: I could not load the sound file:\n"
                      "%s\n"
                      "The Simple DirectMedia error that occured was:\n"
                      "%s\n\n", sound_names[i], SDL_GetError());
              exit(1);
            }
        }
      
      
      /* Load musics: */
      
      /* (title) */
      
      mus_title = Mix_LoadMUS(MUS_TITLE);
      if (mus_title == NULL)
        {
          fprintf(stderr,
                  "\nError: I could not load the music file:\n"
                  "%s\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", MUS_TITLE, SDL_GetError());
          exit(1);
        }

      /* (game) */
      
      mus_game = Mix_LoadMUS(MUS_GAME);
      if (mus_game == NULL)
        {
          fprintf(stderr,
                  "\nError: I could not load the music file:\n"
                  "%s\n"
                  "The Simple DirectMedia error that occured was:\n"
                  "%s\n\n", MUS_GAME, SDL_GetError());
          exit(1);
        }
      /* (gameover) */
    }
#endif

  
  /* Seed random generator: */
  
  srand(SDL_GetTicks());
}


/* Title screen: */

int title(void)
{
  int done, quit, i, x, y;
  SDL_Event event;
  SDL_Rect dest, src;

  
  /* Start music, if it's not playing: */
  
#ifndef NOSOUND
  if (use_sound)
    {
      if (!Mix_PlayingMusic())
	{
	  Mix_PlayMusic(mus_title, 0);
	}
    }
#endif
  
 
  /* Animate introduction of title screen: */
 
#ifndef EMBEDDED
  for (i = 0; i <= WIDTH / 2; i = i + 32)
#else
  for (i = 0; i <= HEIGHT / 2; i = i + 32)
#endif
    {
      for (y = HEIGHT / 2 - i; y < HEIGHT / 2 + i; y = y + 16)
	{
	  for (x = WIDTH / 2 - i; x < WIDTH / 2 + i; x = x + 16)
	    {
	      dest.x = x;
	      dest.y = y;
	      dest.w = 16;
	      dest.h = 16;
	      
	      SDL_BlitSurface(images[IMG_TITLE], &dest,
			      screen, &dest);
	    }
	}
      
      SDL_Flip(screen);
      SDL_Delay(30);
    }
  
  
  /* Draw score: */
  
  drawdigits(score, WIDTH - 174, HEIGHT - 58);
  drawdigits(level, WIDTH - 40, HEIGHT - 58);
  
  
  /* Draw high score: */
  
  drawdigits(highscore, WIDTH - 174, HEIGHT - 21);
  drawdigits(highlevel, WIDTH - 40, HEIGHT - 21);


#ifdef EMBEDDED
  /* Draw start & continue buttons: */

  dest.x = 54;
  dest.y = 158;

  if (can_continue)
  {
    SDL_BlitSurface(images[IMG_START_CONT], NULL,
		    screen, &dest);
  }
  else
  {
    src.x = 0;
    src.y = 0;
    src.w = images[IMG_START_CONT]->w;
    src.h = 40;

    SDL_BlitSurface(images[IMG_START_CONT], &src,
		    screen, &dest);
  }
#endif
  

  SDL_Flip(screen);
  
  
  /* MAIN TITLE LOOP: */
  
  done = 0;
  quit = 0;
  
  do
    {
      while (SDL_PollEvent(&event) > 0)
	{
	  if (event.type == SDL_QUIT)
	    {
	      /* WM Quit - Quit! */
	      
	      quit = 1;
	    }
	  else if (event.type == SDL_KEYDOWN)
	    {
	      /* Keypress! ESCAPE - Quit, Space/Return - Start!*/
	      
	      if (event.key.keysym.sym == SDLK_ESCAPE)
		quit = 1;
	      else if (event.key.keysym.sym == SDLK_SPACE ||
		       event.key.keysym.sym == SDLK_RETURN)
		done = 1;
	    }
	  else if (event.type == SDL_MOUSEBUTTONDOWN)
	    {
	      /* Mouse click! - Start! */

#ifndef EMBEDDED
	      done = 1;
#else
	      if (event.button.x >= 54 &&
	          event.button.x < 54 + images[IMG_START_CONT]->w)
	      {
		if (event.button.y >= 158 &&
		    event.button.y < 158 + images[IMG_START_CONT]->h / 2)
		{
	          can_continue = 0;
		  done = 1;
		}
		else if (event.button.y >= 158 + images[IMG_START_CONT]->h / 2
			 && event.button.y < 158 + images[IMG_START_CONT]->h &&
			 can_continue)
		{
		  done = 1;
		}
	      }
#endif
	    }
	}
      
      
      /* Keep playing music: */
      
#ifndef NOSOUND
      if (use_sound)
	{
	  if (!Mix_PlayingMusic())
	    {
	      Mix_PlayMusic(mus_title, 0);
	    }
	}
#endif
      
      
      /* Pause for a moment: */
      
      SDL_Delay(100);
    }
  while (!done && !quit);

  
  /* Silence music: */
  
#ifndef NOSOUND
  if (use_sound)
    Mix_HaltMusic();
#endif

  
  return(quit);
}


/* Set icon: */

void seticon(void)
{
#ifndef EMBEDDED
  int masklen;
  Uint8 * mask;
  SDL_Surface * icon;
  
  
  /* Load icon into a surface: */
  
  icon = IMG_Load(DATA_PREFIX "images/icon.png");
  if (icon == NULL)
    {
      fprintf(stderr,
              "\nError: I could not load the icon image: %s\n"
              "The Simple DirectMedia error that occured was:\n"
              "%s\n\n", DATA_PREFIX "images/icon.png", SDL_GetError());
      exit(1);
    }
  
  
  /* Create mask: */
  
  masklen = (((icon -> w) + 7) / 8) * (icon -> h);
  mask = malloc(masklen * sizeof(Uint8));
  memset(mask, 0xFF, masklen);
  
  
  /* Set icon: */
  
  SDL_WM_SetIcon(icon, mask);
  
  
  /* Free icon surface & mask: */
  
  free(mask);
  SDL_FreeSurface(icon);
#endif
}


/* Pause screen: */

int pause_screen(void)
{
  SDL_Rect dest;
  SDL_Event event;
  int done, quit;
  
  
  /* Draw "Paused" notice: */
  
  dest.x = (WIDTH - images[IMG_PAUSED]->w) / 2;
  dest.y = (HEIGHT - images[IMG_PAUSED]->h) / 2;
  
  SDL_BlitSurface(images[IMG_PAUSED], NULL, screen, &dest);
  SDL_UpdateRect(screen, dest.x, dest.y, dest.w, dest.h);
  
  
  /* Wait for space to be pressed again: */
  
  done = 0;
  quit = 0;
  
  do
    {
      while (SDL_PollEvent(&event) > 0)
	{
	  if (event.type == SDL_QUIT)
	    {
	      /* WM Quit - Quit! */
	      
	      quit = 1;
	    }
	  else if (event.type == SDL_KEYDOWN)
	    {
	      /* Keypress! ESCAPE - Quit, Space/Return - Continue!*/
	      
	      if (event.key.keysym.sym == SDLK_ESCAPE)
		quit = 1;
	      else if (event.key.keysym.sym == SDLK_SPACE ||
		       event.key.keysym.sym == SDLK_RETURN)
		done = 1;
	    }
	}
      
      
      /* Keep playing music: */
      
#ifndef NOSOUND
      if (use_sound)
	{
	  if (!Mix_PlayingMusic())
	    {
	      Mix_PlayMusic(mus_game, 0);
	    }
	}
#endif
      
      
      /* Pause for a moment: */
      
      SDL_Delay(100);
    }
  while (!done && !quit);
  
  return(quit);
}


/* Play a sound: */

void playsound(int snd, int chan)
{
#ifndef NOSOUND
#ifndef EMBEDDED
  chan = -1;
#endif
  if (use_sound)
    Mix_PlayChannel(chan, sounds[snd], 0);
#endif
}


/* Start a new level: */

void resetlevel(void)
{
  int i;
  
  
  /* Reset all bugs: */
  
  for (i = 0; i < MAX_BUGS; i++)
    {
      bugs[i].alive = 0;
    }  
  
  
  /* Reset all splats: */
  
  for (i = 0; i < MAX_SPLATS; i++)
    {
      splats[i].active = 0;
    }  
  
  
  /* Add some: */
  
  bugs_added = 0;
  
  for (i = 0; i < level / 2 && i < MAX_BUGS_TO_START; i++)
    addbug();
  
  
  multiplier = 1;
}


/* Add a bug: */

void addbug(void)
{
  int i, found;
  
  
  /* Find a free slot: */
  
  found = -1;
  
  for (i = 0; i < MAX_BUGS && found == -1; i++)
    {
      if (bugs[i].alive == 0)
	found = i;
    }
  
  
  /* Add the bug: */
  
  if (found != -1)
    {
      bugs[found].alive = 1;
      
      bugs[found].kind = ((rand() % ((level / 3) + 1)) % NUM_BUGS);
      bugs[found].x = (rand() % WIDTH - 32);
      bugs[found].y = -30;
      bugs[found].xm = (rand() % (level * 2)) - level;
      bugs[found].xmm = (rand() % 5) - 2;
      bugs[found].ym = (rand() % level) + 2 + level;
      
      if ((rand() % CHANCE_OF_FAST_BUG) == 0)
	bugs[found].ym = bugs[found].ym + 16;
      
      if (bugs[found].ym > 24)
	bugs[found].ym = 24;
      
      bugs[found].target_y = ((HEIGHT - images[IMG_ARM1] -> h) +
			      (rand() % ((images[IMG_ARM1] -> h) / 2)));
#ifdef EMBEDDED
      bugs[found].target_y -= 40;
#endif
      
      bugs_added++;
    }
}


/* Add a splat: */

void addsplat(int x, int y, int img)
{
  int i, found;
  
  
  /* Find a free slot: */
  
  found = -1;
  
  for (i = 0; i < MAX_SPLATS && found == -1; i++)
    {
      if (splats[i].active == 0)
	found = i;
    }
  
  
  /* Add the splat: */
  
  if (found != -1)
    {
      splats[found].active = 1;
      splats[found].img = img;
      splats[found].x = x;
      splats[found].y = y;
#ifdef EMBEDDED
      splats[found].timer = 50;
#endif
    }
}


/* Intro screen: */

void intro(void)
{
  float scale, scalespeed;
  int x, y, done, count;
  SDL_Rect src, dest;
  SDL_Event event;

  
  /* Start music! */

#ifndef NOSOUND
  if (use_sound)
    Mix_PlayMusic(mus_title, 0);
#endif

  
  /* Blank screen: */
  
  SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
  
  
  /* Zoom it! */
  
  scalespeed = 0.1;
  done = 0;
 
#ifndef EMBEDDED
  for (scale = 1; scale < 5 && done == 0; scale = scale + scalespeed)
    {
      scalespeed = scalespeed + 0.1;
      
      /* Draw each row and column... */
      
      for (y = 0; y < 96; y++)
	{
	  for (x = 0; x < 128; x++)
	    {
	      /* Blit one pixel: */
	      
	      src.x = x;
	      src.y = y;
	      src.w = 1;
	      src.h = 1;
	      
	      dest.x = 0;
	      dest.y = 0;
	      dest.w = 1;
	      dest.h = 1;
	      
	      SDL_BlitSurface(images[IMG_PRESENTS], &src,
			      screen, &dest);
	      
	      
	      /* Duplicate it as a rectangle: */
	      
	      dest.x = WIDTH / 2 - (64 * scale) + x * scale;
	      dest.y = HEIGHT / 2 - (48 * scale) + y * scale;
	      dest.w = scale + 1;
	      dest.h = scale + 1;
	      
	      SDL_FillRect(screen, &dest,
			   * (Uint32 *) (screen->pixels));
	    }
	}
      
      
      /* Update screen and keep animation slow enough to see! */
      
      SDL_Flip(screen);
      SDL_Delay(100);

      while (SDL_PollEvent(&event))
	{
	  if (event.type == SDL_MOUSEBUTTONDOWN ||
	      event.type == SDL_KEYDOWN)
	    done = 1;
	}
    }
#else
  dest.x = (WIDTH - images[IMG_PRESENTS]->w) / 2;
  dest.y = (HEIGHT - images[IMG_PRESENTS]->h) / 2;
  SDL_BlitSurface(images[IMG_PRESENTS], NULL, screen, &dest);
  SDL_Flip(screen);
  SDL_Delay(2000);
#endif
  
  
  /* Pause for a moment before erasing intro. screen... */
  
  count = 0;
  
  do
    {
      /* A keypress or mouse click will quit the intro. early: */
      
      while (SDL_PollEvent(&event))
	{
	  if (event.type == SDL_MOUSEBUTTONDOWN ||
	      event.type == SDL_KEYDOWN)
	    done = 1;
	}
      
      
      /* Wait for 1 second: */
      
      SDL_Delay(50);
      count++;
    }
  while (!done && count < 20);
}


/* Draw bonus numbers: */

void drawbonusnumbers(int num)
{
  int place, i, digit, nonzero, x;
  SDL_Rect dest;
  
  place = 10000;
  nonzero = 0;
#ifndef EMBEDDED
  x = 286;
#else
  x = (WIDTH / 2) - images[IMG_LARGE0]->w;
#endif
  
  for (i = 0; i < 5; i++)
    {
      digit = num / place;
      
      if (digit != 0)
	nonzero = 1;
      
      if (digit != 0 || nonzero)
	{
	  dest.x = x;
	  dest.y = HEIGHT / 2;
	  
	  SDL_BlitSurface(images[IMG_LARGE0 + digit], NULL,
			  screen, &dest);
	  
	  x = x + images[IMG_LARGE0 + digit] -> w;
	}
      
      num = num - (digit * place);
      
      place = place / 10;
    }
}


/* Add score (handle bonus blood, too!) */

void incrementscore(int amt)
{
  score = score + amt;
  
  
  /* Did we pass a point spot? */
  
  if (((score - amt) / POINTS_FOR_BONUS_BLOOD) <
      (score / POINTS_FOR_BONUS_BLOOD))
    addblood();
  
  if (score > highscore)
    {
      highscore = score;
      highlevel = level;
      
      if (has_highscore == 0)
	{
	  has_highscore = 1;
	  playsound(SND_HIGHSCORE, 1);
	}
    }
}


/* Add blood */

void addblood(void)
{
  if (blood <= (images[IMG_GAUGE_FULL] -> h))
    {
      blood = blood + 20;
      
      if (blood > (images[IMG_GAUGE_FULL] -> h))
	blood = (images[IMG_GAUGE_FULL] -> h);
      
      playsound(SND_AH, 1);
    }
  else
    playsound(SND_GLUG, 1);
}


/* Draw 16x16 font digits on the screen: */

void drawdigits(int v, int x, int y)
{
  SDL_Rect src, dest;
  char str[10];
  int i;
  
  sprintf(str, "%d", v);
  for (i = 0; i < strlen(str); i++)
    {
      dest.x = i * 16 + x;
      dest.y = y;
      dest.w = 16;
      dest.h = 16;
      
      src.x = (str[i] - '0') * 16;
      src.y = 0;
      src.w = 16;
      src.h = 16;
      
      SDL_BlitSurface(images[IMG_NUMBERS], &src, screen, &dest);
    }
}


/* Load data from data file: */

void loaddata(void)
{
  FILE * fi;
  char temp[128];
  
  
  /* Set defaults in case we can't load: */
  
  score = 0;
  level = 0;
  highscore = 1234;
  highlevel = 5;
  
  
  /* Try to open file: */
  
  fi = opendata("r");
  if (fi != NULL)
    {
      do
	{
	  fgets(temp, sizeof(temp), fi);
	  
	  if (!feof(fi))
	    {
	      temp[strlen(temp) - 1] = '\0';
	      
	      
              /* Parse each line: */
              
              if (strstr(temp, "highscore=") == temp)
                {
                  highscore = atoi(temp + 10);
                  
                  if (highscore == 0)
                    highscore = 1234;
                }
	      else if (strstr(temp, "highwave=") == temp)
                {
                  highlevel = atoi(temp + 9);
                  
                  if (highlevel == 0)
                    highlevel = 5;
                }
              else if (strstr(temp, "finalscore=") == temp)
                {
                  score = atoi(temp + 11);
                }
	      else if (strstr(temp, "finalwave=") == temp)
                {
                  level = atoi(temp + 10);
                }
	    }
	}
      while (!feof(fi));
      
      fclose(fi);
    }


#ifdef EMBEDDED
  can_continue = 0;
#endif
}


/* Save data to data file: */

void savedata(void)
{
  FILE * fi;
  
  
  /* Try to open file: */
  
  fi = opendata("w");
  if (fi != NULL)
    {
      fprintf(fi, "# Bug Squish data file\n\n");
      
      fprintf(fi, "highscore=%d\n", highscore);
      fprintf(fi, "highwave=%d\n\n", highlevel);

      fprintf(fi, "finalscore=%d\n", score);
      fprintf(fi, "finalwave=%d\n\n", level);
      
      fprintf(fi, "# (File automatically created.)\n");
      
      fclose(fi);
    }
}


#ifdef EMBEDDED

/* Open the state file: */

FILE * openstate(char * mode)
{
  char * filename, * home;
  FILE * fi;
  
  
  /* Get home directory (from $HOME variable)... if we can't determine it,
     use the current directory ("."): */
  
  if (getenv("HOME") != NULL)
    home = getenv("HOME");
  else
    home = ".";
  
  
  /* Create the buffer for the filename: */
  
  filename = (char *) malloc(sizeof(char) * (strlen(home) +
                                             strlen("/.bugsquish-state") + 1));
  
  strcpy(filename, home);
  strcat(filename, "/.bugsquish-state");
  
  
  /* Try opening the file: */
  
  fi = fopen(filename, mode);
  
  if (fi == NULL)
    {
      fprintf(stderr, "\nWarning: I could not open the state file "); 
      
      if (strcmp(mode, "r") == 0)
        fprintf(stderr, "for read:");
      else if (strcmp(mode, "w") == 0)
        fprintf(stderr, "for write:");
      
     fprintf(stderr, "\n%s\n"
              "The error that occured was:\n"
              "%s\n\n", filename, strerror(errno));
    }
  
  
  return(fi);
}

#endif


/* Open the data file: */

FILE * opendata(char * mode)
{
  char * filename, * home;
  FILE * fi;
  
  
#ifdef LINUX_DAT
  /* Get home directory (from $HOME variable)... if we can't determine it,
     use the current directory ("."): */
  
  if (getenv("HOME") != NULL)
    home = getenv("HOME");
  else
    home = ".";
  
  
  /* Create the buffer for the filename: */
  
  filename = (char *) malloc(sizeof(char) * (strlen(home) +
                                             strlen("/.bugsquish") + 1));
  
  strcpy(filename, home);
  strcat(filename, "/.bugsquish");
#else
  filename = "bugsquish.dat";
#endif
  
  
  /* Try opening the file: */
  
  fi = fopen(filename, mode);
  
  if (fi == NULL)
    {
      fprintf(stderr, "\nWarning: I could not open the options file "); 
      
      if (strcmp(mode, "r") == 0)
        fprintf(stderr, "for read:");
      else if (strcmp(mode, "w") == 0)
        fprintf(stderr, "for write:");
      
     fprintf(stderr, "\n%s\n"
              "The error that occured was:\n"
              "%s\n\n", filename, strerror(errno));
    }
  
  
  return(fi);
}



/* Parse command line arguments: */

void getargs(int argc, char * argv[])
{
  int i;
  

  /* Set defaults: */
  
  use_sound = 1;
  use_fullscreen = 0;


  /* Grab args: */
  
  for (i = 1; i < argc; i++)
    {
      if (strcmp(argv[i], "--fullscreen") == 0 ||
          strcmp(argv[i], "-f") == 0)
        use_fullscreen = 1;
      else if (strcmp(argv[i], "--disable-sound") == 0 ||
               strcmp(argv[i], "--nosound") == 0 ||
               strcmp(argv[i], "-q") == 0)
        use_sound = 0;
      else if (strcmp(argv[i], "--help") == 0 ||
               strcmp(argv[i], "-h") == 0)
        {
          printf("Bug Squish\n"
                 "\n"
                 "Version " VERSION "\n"
                 "New Breed Software, 2001\n"
                 "\n"
                 "Programming: Bill Kendrick\n"
                 "Graphics: Bill Kendrick\n"
                 "Music: Sami I Saarnio and 'Kure4Kancer'\n"
                 "Concept: \"Blood Suckers\" by Brian Greenstone, Pangea Software\n"
                 "\n"
                 "Game controls:\n"
                 "  Mouse Movement   - Move fly swatter\n"
                 "  Any Mouse Button - Squish /\n"
                 "\n"
                 "Run with \"--usage\" for command-line options...\n"
                 "\n");
          
          exit(0);
        }
      else if (strcmp(argv[i], "--usage") == 0 ||
               strcmp(argv[i], "-u") == 0)
        usage(0);
      else
        usage(1);
    }
}


/* Display usage screen: */

void usage(int ret)
{
  FILE * fs;
  
  if (ret == 1)
    fs = stderr;
  else
    fs = stdout;
  
  fprintf(fs,
	  "\nUsage: bugsquish [--disable-sound] [--fullscreen] |\n"
	  "                   [--help (-h) | --usage (-u)]\n\n");
  fprintf(fs,
          "  --disable-sound - "
          "Disable sound and music. (Also \"--nosound\" or \"-q\")\n");
  fprintf(fs,
          "  --fullscreen    - "
          "Display in full screen instead of a window, if possible.\n"
          "                    (Also \"-f\")\n");
  fprintf(fs, "\n");
  
  exit(ret);
}


#ifdef EMBEDDED

void loadstate(void)
{
  FILE * fi;

  fi = openstate("r");
  if (fi != NULL)
  {
    can_continue = fgetc(fi);

    if (can_continue)
    {
      level = fgetc(fi);
      fread(&score, sizeof(int), 1, fi);
      fread(&blood, sizeof(int), 1, fi);
      fread(&bugs_added, sizeof(int), 1, fi);
      multiplier = fgetc(fi);
      has_highscore = fgetc(fi);
      fread(&num_bugs, sizeof(int), 1, fi);
      next_level = fgetc(fi);
      old_blood = fgetc(fi);
      fread(&bonus, sizeof(int), 1, fi);
      fread(bugs, sizeof(bug_type), MAX_BUGS, fi);
      fread(splats, sizeof(splat_type), MAX_SPLATS, fi);
      fread(&extra, sizeof(extra_type), 1, fi);
    }

    fclose(fi);
  }
  else
  {
    can_continue = 0;
  }
}


void savestate(void)
{
  FILE * fi;

  fi = openstate("w");
  if (fi != NULL)
  {
    fputc(can_continue, fi);

    if (can_continue)
    {
      fputc(level, fi);
      fwrite(&score, sizeof(int), 1, fi);
      fwrite(&blood, sizeof(int), 1, fi);
      fwrite(&bugs_added, sizeof(int), 1, fi);
      fputc(multiplier, fi);
      fputc(has_highscore, fi);
      fwrite(&num_bugs, sizeof(int), 1, fi);
      fputc(next_level, fi);
      fputc(old_blood, fi);
      fwrite(&bonus, sizeof(int), 1, fi);
      fwrite(bugs, sizeof(bug_type), MAX_BUGS, fi);
      fwrite(splats, sizeof(splat_type), MAX_SPLATS, fi);
      fwrite(&extra, sizeof(extra_type), 1, fi);
    }

    fclose(fi);
  }
}

#endif
