#include <simple2d.h>
#include <wiringPi.h>

#define MAX_STAGES 10
#define MAX_COLOR 5
#define CLEAR_SCREEN_DELAY 100
#define DISPLAY_TIME 40
#define RED    0
#define BLUE   1
#define YELLOW 2
#define GREEN  3
#define WHITE  4
#define MAX_ELAPSED_TIME 700

bool missionCompleted = false;
int i = 0;
int k = 0;
int j = -640;
int angle;
int MaxStage = 1;
int seed=0;
int time = 0;
int	clearScreenDelay = 100;
int lastColorPressed = -1;
int pressed = 0;
int currentStage = 0;
int sequenceElapsedTime = 0;
char *message;
char *letter;
int c=0;

void (*render_fct[2])();
S2D_Image *img;
S2D_Image *scan;
S2D_Image *MissLance;
S2D_Image *MissLanched;
S2D_Text *txtTop;
S2D_Text *txtTop2;
S2D_Text *txtBot;
S2D_Text *txtBot2;
S2D_Sound *snd;
S2D_Text *txt;

static const int color[5][4] = {
 /* R    G    B    Button */
    1,   0,   0,   0,      // RED
    0,   1,   0,   3,      // GREEN
    0,   0,   1,   1,      // BLUE
    1,   1,   0,   2,      // YELLOW
    1,   1,   1,   4,      // WHITE
};


static int randColor[MAX_STAGES];


S2D_Window *window;

void renderVerticalMessage(int x, int y, char *message)
{
    int i=0;
    
    txt->x = x;
    txt->y = y;
        
    while (message[i] != '\0')
    {
        letter[0] = message[i++];
        S2D_SetText(txt, letter);
        S2D_DrawText(txt);
        txt->y = txt->y + 35;

    }
    
}

void calcRandColor()
{
	randColor[0] = rand() % MAX_COLOR;
	while ((randColor[1] = (rand() % MAX_COLOR)) == randColor[0]);
    while ((randColor[2] = (rand() % MAX_COLOR)) == randColor[1]);
    while ((randColor[3] = (rand() % MAX_COLOR)) == randColor[2]);
	while ((randColor[4] = (rand() % MAX_COLOR)) == randColor[3]);
	while ((randColor[5] = (rand() % MAX_COLOR)) == randColor[4]);
    while ((randColor[6] = (rand() % MAX_COLOR)) == randColor[5]);
	while ((randColor[7] = (rand() % MAX_COLOR)) == randColor[6]);
	while ((randColor[8] = (rand() % MAX_COLOR)) == randColor[7]);
	while ((randColor[9] = (rand() % MAX_COLOR)) == randColor[8]);
}

void resetStage()
{
    sequenceElapsedTime = 0;
    pressed = 0;
    lastColorPressed = -1;
    MaxStage = 1;
    sprintf(message, "Stage : %d", MaxStage);
    S2D_SetText(txtTop, message);
    S2D_SetText(txtTop2, message);
    S2D_SetText(txtBot, message);
    S2D_SetText(txtBot2, message);
    S2D_PlaySound(snd);
    i = 0;
    angle = 0;
    j = -640;  
    txtTop->x = i++;
    txtTop2->x = j++;

    txtBot->x = i++;
    txtBot2->x = j++;
}

void compareSequence(int aColor)
{
    lastColorPressed = aColor;
    if (color[randColor[pressed]][3] == aColor)
    {
        if (pressed == 0)
        {
            sequenceElapsedTime = 0;
        }
        pressed++;
            
        if (pressed == MaxStage)
        {
            srand(rand());
            calcRandColor();
            
            clearScreenDelay = 0;
            MaxStage++;
            pressed = 0;
            time = 0;
            lastColorPressed = -1;
            sprintf(message, "Stage : %d", MaxStage);

            S2D_SetText(txtTop, message);
            S2D_SetText(txtTop2, message);
            S2D_SetText(txtBot, message);
            S2D_SetText(txtBot2, message);
            S2D_PlaySound(snd);
            i = 0;
            angle = 0;
            j = -640;  
            txtTop->x = i++;
            txtTop2->x = j++;

            txtBot->x = i++;
            txtBot2->x = j++;
        
            if (MaxStage == MAX_STAGES)
            {
                missionCompleted = true;
                S2D_SetText(txtTop, "Mission Complete!");
                S2D_SetText(txtTop2, "Mission Complete!");
                S2D_SetText(txtBot, "Mission Completed!");
                S2D_SetText(txtBot2, "Mission Completed!");
                S2D_PlaySound(snd);
            }            
        }
    }
    else
    {
        resetStage();
    }
}

bool inputButtonPressed(int button)
{
    bool press = false;
    while (digitalRead(button) == 0)
    {
        press = true;
    }
    
    return press;
}

void checkColorButtons()
 {
    if (pressed != 0)
        sequenceElapsedTime++;
        
    if (sequenceElapsedTime > MAX_ELAPSED_TIME)
        resetStage();
        
    //printf("sequenceElapsedTime = %d \n", sequenceElapsedTime);    
    if (lastColorPressed != RED)
    {
        if (inputButtonPressed(RED) == true)
        {
            compareSequence(RED);
        }
    }
    if (lastColorPressed != BLUE)
    {
        if (inputButtonPressed(BLUE) == true)
        {
            compareSequence(BLUE);
        }
    }
    if (lastColorPressed != YELLOW)
    {
        if (inputButtonPressed(YELLOW) == true)
        {
            compareSequence(YELLOW);
        }
    }
    if (lastColorPressed != GREEN)
    {
        if (inputButtonPressed(GREEN) == true)
        {
            compareSequence(GREEN);
        }
    }
    if (lastColorPressed != WHITE)
    {
        if (inputButtonPressed(WHITE) == true)
        {
            compareSequence(WHITE);
        }
    }  
}

void clearScreen()
{
    S2D_DrawTriangle(
        320,  90, 0, 0, 0, 1,
        150, 165, 0, 0, 0, 1,
        150, 315, 0, 0, 0, 1);

    S2D_DrawTriangle(
        320,  90, 0, 0, 0, 1,
        150, 315, 0, 0, 0, 1,
        320, 390, 0, 0, 0, 1);

    S2D_DrawTriangle(
        320,  90, 0, 0, 0, 1,
        320, 390, 0, 0, 0, 1,
        490, 315, 0, 0, 0, 1);

    S2D_DrawTriangle(
        320,  90, 0, 0, 0, 1,
        490, 315, 0, 0, 0, 1,
        490, 165, 0, 0, 0, 1);
}

void render() {
	
    if (currentStage != MaxStage)
    {
        currentStage = MaxStage;
        for (int wait = 0; wait < 10000000; wait++)
            digitalRead(0);
    }
    checkColorButtons();

//    S2D_DrawImage(img);
	   
    if (clearScreenDelay++ < CLEAR_SCREEN_DELAY)
    {
        clearScreen();

        k = 0;
    }
	else
	{
        if (missionCompleted == false)
		{
			//printf("i = %d \n", i);
            if ((++k % DISPLAY_TIME) == 0)
            {
                time++;
            };
				
            if (time == MaxStage)
            {
                clearScreenDelay = 0;
                time = time % MaxStage;
            }
            else
            {
                time = time % MaxStage;

                S2D_DrawTriangle(
                    320,  90, color[randColor[time]][0], color[randColor[time]][1], color[randColor[time]][2], 1,
                    150, 165, color[randColor[time]][0], color[randColor[time]][1], color[randColor[time]][2], 1,
                    150, 315, color[randColor[time]][0], color[randColor[time]][1], color[randColor[time]][2], 1);

                S2D_DrawTriangle(
                    320,  90, color[randColor[time]][0], color[randColor[time]][1], color[randColor[time]][2], 1,
                    150, 315, color[randColor[time]][0], color[randColor[time]][1], color[randColor[time]][2], 1,
                    320, 390, color[randColor[time]][0], color[randColor[time]][1], color[randColor[time]][2], 1);
	  
                S2D_DrawTriangle(
                    320,  90, color[randColor[time]][0], color[randColor[time]][1], color[randColor[time]][2], 1,
                    320, 390, color[randColor[time]][0], color[randColor[time]][1], color[randColor[time]][2], 1,
                    490, 315, color[randColor[time]][0], color[randColor[time]][1], color[randColor[time]][2], 1);

                S2D_DrawTriangle(
                    320,  90, color[randColor[time]][0], color[randColor[time]][1], color[randColor[time]][2], 1,
                    490, 315, color[randColor[time]][0], color[randColor[time]][1], color[randColor[time]][2], 1,
                    490, 165, color[randColor[time]][0], color[randColor[time]][1], color[randColor[time]][2], 1);
            }
        }
    }
               //   S2D_DrawTriangle(
               //     320,  240, 0, 0.7, 0, 1,
               //     320 + (220 * cos((i%360) * 3.14159 / 180)), 240 + (220 * sin((i%360) * 3.14159 / 180)), 0, 0.7, 0, 1,
               //     320 + (220 * cos((i%360) * 3.14159 / 180)), 245 + (220 * sin((i%360) * 3.14159 / 180)), 0, 0.7, 0, 1);
 
                  S2D_DrawTriangle(
                    220, 239, 0, 0.7, 0, 1,
                    420, 239, 0, 0.7, 0, 1,
                    420, 241, 0, 0.7, 0, 1);
                  S2D_DrawTriangle(
                    220, 239, 0, 0.7, 0, 1,
                    420, 241, 0, 0.7, 0, 1,
                    220, 241, 0, 0.7, 0, 1);

                  S2D_DrawTriangle(
                    319, 140, 0, 0.7, 0, 1,
                    319, 340, 0, 0.7, 0, 1,
                    321, 340, 0, 0.7, 0, 1);
                  S2D_DrawTriangle(
                    319, 140, 0, 0.7, 0, 1,
                    321, 340, 0, 0.7, 0, 1,
                    321, 140, 0, 0.7, 0, 1);
                    
  S2D_RotateImage(scan, (angle++%360), S2D_TOP_RIGHT);
  S2D_DrawImage(scan);

  S2D_DrawImage(img);

  MissLance->y = i;
  
  S2D_DrawImage(MissLance);
  
  MissLance->y = j;
  
  S2D_DrawImage(MissLance);

  MissLanched->y = i;
  
  S2D_DrawImage(MissLanched);
 
  MissLanched->y = j;
  
  S2D_DrawImage(MissLanched);
 
  if (missionCompleted == false)
    {
     	if (i==480)
        {
            i = 0;
            j = -480;
        }
	  
        txtTop->x = i++;
        txtTop2->x = j++;

        txtBot->x = i++;
        txtBot2->x = j++;
        
        //renderVerticalMessage(20, i, (char *)txtTop->msg);
        //renderVerticalMessage(20, j, (char *)txtTop2->msg);
        //renderVerticalMessage(620, i, (char *)txtBot->msg);
        //renderVerticalMessage(620, j, (char *)txtBot2->msg);

        //S2D_DrawText(txtTop);
        //S2D_DrawText(txtTop2);

        //S2D_DrawText(txtBot);
        //S2D_DrawText(txtBot2);
    }
    else
    {
           if ((k++ % DISPLAY_TIME) == 0)
            {
                time++;
            };
				
            if (time % 2)
            {
                txtTop->x = 130;
                txtTop->y = 220;

                S2D_DrawText(txtTop);
            }
    }
}

void on_key(S2D_Event e) 
{
    switch (e.type) 
    {
    case S2D_KEY_DOWN:
        printf("Key down: %s\n", e.key);
        if (strcmp(e.key, "Escape") == 0) S2D_Close(window);
        if (strcmp(e.key, "Return") == 0) 
        {
            c++;
                sprintf(message, "toto", MaxStage);
            message[0] = c;
    S2D_SetText(txtTop, message);
    S2D_SetText(txtTop2, message);
    S2D_SetText(txtBot, message);
    S2D_SetText(txtBot2, message);
            //clearScreenDelay = 50;
            //printf("clearScreen = %d \n", clearScreen);
            //MaxStage++;
            //if (MaxStage = MAX_STAGES)
           // {
            //    missionCompleted = true;
            //    S2D_SetText(txtTop, "Mission Complete!");
            //    S2D_SetText(txtTop2, "Mission Complete!");
            //    S2D_SetText(txtBot, "Mission Completed!");
            //    S2D_SetText(txtBot2, "Mission Completed!");
            //    S2D_PlaySound(snd);
           // }
           // else
           // {
           //     clearScreenDelay=CLEAR_SCREEN_DELAY;
           //     k=0;
           //     calcRandColor();
       //  
        //    }
        }
        break;

        case S2D_KEY_HELD:
            printf("Key held: %s\n", e.key);
            break;

        case S2D_KEY_UP:
            printf("Key up: %s\n", e.key);
            break;
    }
}

int main() 
{    
    wiringPiSetup() ;
  
    pinMode (0, INPUT) ;	// Buttons
    pinMode (1, INPUT) ;	// Buttons
    pinMode (2, INPUT) ;	// Buttons
    pinMode (3, INPUT) ;	// Buttons
    pinMode (4, INPUT) ;	// Buttons

    pullUpDnControl (0, PUD_UP) ;
    pullUpDnControl (1, PUD_UP) ;
    pullUpDnControl (2, PUD_UP) ;
    pullUpDnControl (3, PUD_UP) ;
    pullUpDnControl (4, PUD_UP) ;
  
    srand(rand());
    calcRandColor();

    message = (char*) malloc(100 * sizeof(char));

    render_fct[0] = render;


    letter = (char*) malloc(2 * sizeof(char));
    sprintf(letter, " ");
    
    txt = S2D_CreateText("Alien-Encounters-Regular.ttf", "", 40);

    window = S2D_CreateWindow("Hello Triangle", 640, 480, NULL, render_fct[0], 0);

    //img = S2D_CreateImage("old-metal-green-military-background-rivets-armor-84791421.jpg");
    //img = S2D_CreateImage("Radar.png");
    img = S2D_CreateImage("Radar2Trans.png");
    //scan = S2D_CreateImage("aiguillePPReverse.png");    
    scan = S2D_CreateImage("aiguillePPSuperSmall.png");    
    MissLance = S2D_CreateImage("MissileLance.png");    
    MissLanched = S2D_CreateImage("MissileLaunched.png");    

    img->x = 70;
    img->y = 0;

    img->width  = 500;
    img->height = 480;

    MissLance->x = 0;
    MissLance->y = 0;

    MissLance->width  = 80;
    MissLance->height = 380;
    
    MissLanched->x = 550;
    MissLanched->y = 0;

    MissLanched->width  = 90;
    MissLanched->height = 450;    
    
    scan->x = 220;
    scan->y = 240;

    scan->width  = 100;
    scan->height = 150;

    txtTop = S2D_CreateText("Alien-Encounters-Regular.ttf", "Missile Lance", 40);
    txtTop2 = S2D_CreateText("Alien-Encounters-Regular.ttf", "Missile Lance", 40);
    txtBot = S2D_CreateText("Alien-Encounters-Regular.ttf", "Missile Launched", 40);
    txtBot2 = S2D_CreateText("Alien-Encounters-Regular.ttf", "Missile Launched", 40);
//S2D_RotateText(txtTop, 90, 0);
//1txtTop->rotate = 90;

    txtTop->y = 0;
    txtTop2->y = 0;
    txtBot->y = 430;
    txtBot2->y = 430;
	  
    txtTop->color.r = 0.0;
    txtTop->color.g = 0.8;
    txtTop->color.b = 0.0;
    txtTop->color.a = 1.0;
	
    txtTop2->color.r = 0.0;
    txtTop2->color.g = 0.8;
    txtTop2->color.b = 0.0;
    txtTop2->color.a = 1.0;    

    txtBot->color.r = 0.0;
    txtBot->color.g = 0.8;
    txtBot->color.b = 0.0;
    txtBot->color.a = 1.0;
		
    txtBot2->color.r = 0.0;
    txtBot2->color.g = 0.8;
    txtBot2->color.b = 0.0;
    txtBot2->color.a = 1.0;    

    txt->color.r = 0.0;
    txt->color.g = 0.8;
    txt->color.b = 0.0;
    txt->color.a = 1.0;

    snd = S2D_CreateSound("NFF-success.wav");

    window->on_key = on_key;

    S2D_Show(window);
    S2D_FreeImage(scan);  
    S2D_FreeImage(img);  
    S2D_FreeImage(MissLance);
    
    free(message);
      
    S2D_FreeText(txtTop);
    S2D_FreeText(txtTop2);
    S2D_FreeText(txtBot);
    S2D_FreeText(txtBot2);  

    free(letter);
    S2D_FreeText(txt);

    S2D_FreeSound(snd);  
  
    S2D_FreeWindow(window);

    S2D_Close(window);

    return 0;
}
