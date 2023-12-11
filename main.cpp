#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <string>
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <vector>
#include <stdio.h>
#include <string.h>
using namespace std;

//Create a window
SDL_Window *gameWindow;

//Textures
//Create a texture for the tile sheet
SDL_Texture* textureMap = NULL;
SDL_Texture* textureCavemanEastSheet = NULL;     // Sheet of east Caveman's texture
SDL_Texture* textureCavemanWestSheet = NULL;     // Sheet of west Caveman's texture
SDL_Texture* textureCavemanNorthSheet = NULL;    // Sheet of north Caveman's texture
SDL_Texture* textureCavemanSouthSheet = NULL;    // Sheet of south Caveman's texture
SDL_Texture* textureCavemanSheet = NULL;         // Generic sheet of Caveman's texture

SDL_Texture* textureVladEastSheet = NULL;     //sheet of east Vlad texture
SDL_Texture* textureVladWestSheet = NULL;     //sheet of west Vlad texture
SDL_Texture* textureVladSheetEW = NULL;         //generic sheet of Vlad texture
SDL_Texture* textureVladNorthSheet = NULL;    //sheet of north Vlad texture
SDL_Texture* textureVladSouthSheet = NULL;    //sheet of south Vlad texture
SDL_Texture* textureVladSheetNS = NULL;         //generic sheet of Vlad texture

//Create a temporary surface because we always need one at hand
SDL_Surface* tempSurface = NULL;
//Create a Renderer 'cause we always need one of these as well
SDL_Renderer* renderer;

//Rectangles
//Source and destination rectangles for tile sheet and placement of one tile
SDL_Rect dstRect;
SDL_Rect srcRect;
//SDL_Rect rectBackground;
SDL_Rect solidTileRect;

//All the Caveman rectangles
SDL_Rect rectCavemanEastSheet;   // Rectangle for sheet of east walking Caveman
SDL_Rect rectCavemanEast;        // One east walking caveman rectangle
SDL_Rect rectCavemanWestSheet;   // Rectangle for sheet of west walking Caveman
SDL_Rect rectCavemanWest;        // One west walking caveman rectangle
SDL_Rect rectCavemanNorthSheet;  // Rectangle for sheet of north walking Caveman
SDL_Rect rectCavemanNorth;       // One north walking caveman rectangle
SDL_Rect rectCavemanSouthSheet;  // Rectangle for sheet of south walking Caveman
SDL_Rect rectCavemanSouth   ;       // One south walking caveman rectangle

SDL_Rect rectCavemanPlacement;   // Where to place the caveman on the screen
SDL_Rect rectCaveman;            // Rectangle containing caveman placed on screen
SDL_Rect rectCavemanSheet;       // Sheet containing Caveman for current direction

//All the Vlad rectangles
SDL_Rect rectVladEastSheet;   //Rectangle for sheet of east walking Vlad
SDL_Rect rectVladEast;        //One east walking Vlad rectangle
SDL_Rect rectVladWestSheet;   //Rectangle for sheet of west walking Vlad
SDL_Rect rectVladWest;        //One west walking Vlad rectangle
SDL_Rect rectVladNorthSheet;  //Rectangle for sheet of north walking Vlad
SDL_Rect rectVladNorth;       //One north walking Vlad rectangle
SDL_Rect rectVladSouthSheet;  //Rectangle for sheet of south walking Vlad
SDL_Rect rectVladSouth;       //One south walking Vlad rectangle

SDL_Rect rectVladPlacementEW;   //Where to place the e/w Vlad on the screen
SDL_Rect rectVladSheetEW;       //Sheet containing e/w Vlad for currect direction
SDL_Rect rectVladEW;            //Rectangle containing e/w Vlad placed on screen

//explosion
SDL_Rect rectExplosion;

//Game constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 640;

const int Caveman_WIDTH = 32;
const int Caveman_HEIGHT = 32;

const int Vlad_WIDTH = 32;
const int Vlad_HEIGHT = 32;

const int FPS = 30;
const int FRAME_DELAY = 100/FPS;

const int Caveman_SPEED = 1;
const int MAX_Caveman_FRAME = 8;  //number of Cavemans per sheet

const int Vlad_SPEED = 1;     //the Vlad moves faster than the Caveman
const int MAX_Vlad_FRAME = 7; //number of Vlads per sheet

const int MAX_EXPLOSION_FRAME = 10;

//Global Variables so everyone can share
//Background variables
int displayRow = 0;             //number of rows you want displayed
int displayCol = 0;             //number of columns you want displayed
int mapRow = 0;                 //number of rows in map
int mapCol = 0;                 //number of columns in map
int tileHeight = 0;             //height of tiles in tile sheet
int tileWidth = 0;              //width of tiles in tile sheet

//Caveman variables
int CavemanWidth;                // Width of one caveman frame
int CavemanHeight;               // Height of one caveman frame
int SpriteFrame = 0;
int FrameCounter = 0;
int xCavemanPos = 20;             // X position of caveman on screen
int yCavemanPos = 620;            // Y position of caveman on screen
bool cavemanDestroyed = false;

//Vlad variables
int VladWidth;                //width of one Vlad frame
int VladHeight;               //height of one Vlad frame
int SpriteFrameVladEW = 0;
int FrameCounterVladEW = 0;
int xVladEWpos = 150;          //x position of e/w Vlad on screen
int yVladEWpos = 390;         //y position of e/w Vlad on screen
bool VladDestroyed = false;
bool VladExploded = false;

//explosion!
SDL_Rect rectExplosionPlacement;
int explosionWidth = 64;
int explosionHeight = 64;
SDL_Texture* textureExplosionSheet = NULL;

//Note:  west = 0, east = 1, south = 2, north = 3
int CavemanDirection;
int VladDirectionEW = 1;
int VladDirectionNS = 2;

//place holders for array of frame ID numbers
int* layer1, *layer2;

int sheetWidth = 0;             //width of destination game screen
int sheetHeight = 0;            //height of destination game screen

vector<int> solidTiles;         //array of solid tile frameIDs
struct SolidTileStruct          //structure to hold position of solid tile data
{
    int x;          //x position of solid tile
    int y;          //y position of solid tile
    bool solid;     //we know the tile is solid but put a 1 here anyway
    int frameID;    //frame ID of solid tile
};
SolidTileStruct tiles[1000];
int numSolidTiles = 0;

//Generic Function prototypes
bool ProgramIsRunning();
void FillRect(SDL_Rect &rect, int x, int y, int width, int height);
SDL_Surface* loadImage(const char* path);
SDL_Texture* loadTexture(SDL_Surface* surface);
SDL_Surface* setTransparentColor(SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b);
void CloseShop();
void DrawImage(SDL_Surface* image, SDL_Surface* destSurface, int x, int y);
bool ProgramIsRunning();
bool InitGame();
bool InitSDL();
void CreateGameImages();
void RunGame();
void DrawGame();

//void setUpEachSheet(const char* path , int r, int g, int b, int maxFrame, SDL_Texture textureSheet, SDL_Rect rectSheet, SDL_rectDirSheet)

//Tile function prototypes
void getMapData();
void generateSolidLayer();
void drawLayer(int layer[]);
bool isTileSolid(int tile);

//Caveman function prototypes
void UpdateCaveman();
void moveUp();
void moveDown();
void moveLeft();
void moveRight();

//Vlad function prototypes
void updateEastWestVlad();
void updateNorthSouthVlad();

//Collision function prototypes
void Collide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &xPos, int &yPos );
void enemyCollide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &direction);
void PlayerTileCollision(SDL_Rect movingRect, int &xPos, int &yPos);
bool EnemyTileCollision(SDL_Rect movingRect, int &direction);
void playerEnemyCollide(SDL_Rect playerRect, SDL_Rect enemyRect);
bool CheckCollision(SDL_Rect r1,SDL_Rect r2);

//Destruction function prototypes
void destroyVlad();
void destroyCaveman();
void explosion(int xEnemyPos, int yEnemyPos);

int main(int argc, char *argv[])
{
    const Uint8 *keys = SDL_GetKeyboardState(NULL);

    if(!InitGame())
    {
        printf("Unable to initialize from main.\n");
        CloseShop();   //If InitGame failed, stop the presses
    }

    //Create all the game images
    CreateGameImages();

    while(ProgramIsRunning())
    {
        //CHECK INPUT
        //Move the Caveman when the up/down/left right key is pressed
        if(keys[SDL_SCANCODE_UP])
        {
            //move the shape north
            moveUp();
            yCavemanPos = yCavemanPos - Caveman_SPEED; //decrement y
        }

        if(keys[SDL_SCANCODE_DOWN])
        {
            //move shape south
            moveDown();
            yCavemanPos = yCavemanPos + Caveman_SPEED; //increment y
        }

        if(keys[SDL_SCANCODE_LEFT])
        {
            //move shape east
            moveLeft();
            xCavemanPos = xCavemanPos - Caveman_SPEED; //decrement x
        }

        if(keys[SDL_SCANCODE_RIGHT])
        {
            //move shape west
            moveRight();
            xCavemanPos = xCavemanPos + Caveman_SPEED;  //increment x
        }

        if(VladDestroyed == true && VladExploded == false)

        {

        explosion(rectVladPlacementEW.x, rectVladPlacementEW.y);

        VladExploded = true;

        }

        RunGame();          //Update & Draw

    }//end while not quit

    printf("It is safer to search in the maze then to be in a cheeseless situation.  Spencer Johnson");
    CloseShop();
    return 0;
}

//Functions

void getMapData()
{
    //Get the tile data
    //Initialize the layers
    layer1 = layer2 = NULL;

    //Load map file
     std::ifstream in("graphics/map.map");

    if(!in.good())
        printf("The map did not load.");

    std::string buffer;

    //1. Get the line that says 'destWidth'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    sheetWidth = atoi(buffer.c_str());

    //2. Get the line that says 'destHeight'
    getline(in, buffer, ' ');
    //Get the height value
    getline(in, buffer, '\n');
    sheetHeight = atoi(buffer.c_str());

    //3. Get the line that says 'mapWidth'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    mapCol = atoi(buffer.c_str());

    //4. Get the line that says 'mapHeight'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    mapRow = atoi(buffer.c_str());

    //5. Get the line that says 'tile_width'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    tileWidth = atoi(buffer.c_str());

    //6. Get the line that says 'tile_height'
    getline(in, buffer, ' ');
    //Get the width value
    getline(in, buffer, '\n');
    tileHeight = atoi(buffer.c_str());

    //7. Get the line that says 'solid_tiles'
    getline(in, buffer, '\n');

    //8. Get the row containing solid tiles
    getline(in, buffer, '\n');

    char* string = new char[buffer.size() + 1];
    strcpy(string, buffer.c_str());

    char* token = strtok(string, " ,");

    while(token != NULL)
    {
        solidTiles.push_back(atoi(token));
        token = strtok(NULL, " ,");
    }

    delete[] string;

    //Set up an array for each layer before getting data for each layer
    layer1 = new int[mapCol * mapRow];
    layer2 = new int[mapCol * mapRow];

    //9. Get the row containing 'layer1' of background (not solid tiles)
    getline(in, buffer, '\n');

    int i = 0;  //array index
    for(int row = 0; row < mapRow; row++)
    {
        for(int col = 0; col < mapCol; col++)
        {
            char delim = ','; //comma delimiter between frame numbers
            if(col == mapCol - 1)
            {
                delim = '\n';   //EOL delimiter for next line in map
            }
            getline(in, buffer, delim);         //get the frame ID without delimeter
            int data = atoi(buffer.c_str());    //convert to integer
            layer1[i] = data;                    //store it in data array
            i++;                                //move to next cell in array
        }//end for col
    }//end for row

    //10. Get the row containing 'layer2' of solid tile placement
    getline(in, buffer, '\n');

    i = 0;  //array index
    for(int row = 0; row < mapRow; row++)
    {
        for(int col = 0; col < mapCol; col++)
        {
            char delim = ','; //comma delimiter between frame numbers
            if(col == mapCol - 1)
            {
                delim = '\n';   //EOL delimiter for next line in map
            }
            getline(in, buffer, delim);         //get the frame ID without delimeter
            int data = atoi(buffer.c_str());    //convert to integer
            layer2[i] = data;                   //store it in data array
            i++;                                //move to next cell in array
        }//end for col
    }//end for row

    in.close();
}//end load map

SDL_Surface* loadImage(const char* path)
{
    //Load image at specified path
	tempSurface = SDL_LoadBMP(path);

	if( tempSurface == NULL )
	{
		printf("Unable to load image at path: %s\n", path);
	}
    return tempSurface;
}//end loadImage

SDL_Texture* loadTexture(SDL_Surface* tempSurface)
{
	//texture
	SDL_Texture* newTexture = NULL;
    //Create texture from surface pixels
    newTexture = SDL_CreateTextureFromSurface(renderer, tempSurface);
    if( newTexture == NULL )
    {
        printf("Unable to create texture\n");
    }
    //Get rid of the surface
    SDL_FreeSurface( tempSurface );
	return newTexture;
}//end create a texture

void FillRect(SDL_Rect &rect, int x, int y, int width, int height)
{
    //Initialize the rectangle
    rect.x = x;         //initial x position of upper left corner
    rect.y = y;         //initial y position of upper left corner
    rect.w = width;     //width of rectangle
    rect.h = height;    //height of rectangle
}//end rectangle initializing

void CloseShop()
{
    //Destroy all objects
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(gameWindow);

    //SDL_DestroyTexture(textureBackground);
    SDL_DestroyTexture(textureMap);

    if (cavemanDestroyed == false)
    {
        destroyCaveman();
    }
    if (VladDestroyed == false)
    {
        destroyVlad();
    }

    //destroy pointers to arrays
    if (layer1 != NULL)
    {
        delete[] layer1;
        layer1 = NULL;
    }
    if (layer2 != NULL)
    {
        delete[] layer2;
        layer2 = NULL;
    }
    //release vector memory
    solidTiles.clear();

    SDL_Quit(); //Quit the program
}//End Close Shop

void destroyCaveman()
{
    SDL_DestroyTexture(textureCavemanSouthSheet);
    SDL_DestroyTexture(textureCavemanNorthSheet);
    SDL_DestroyTexture(textureCavemanEastSheet);
    SDL_DestroyTexture(textureCavemanWestSheet);
    SDL_DestroyTexture(textureCavemanSheet);
}
void destroyVlad()
{
    SDL_DestroyTexture(textureVladEastSheet);
    SDL_DestroyTexture(textureVladWestSheet);
    SDL_DestroyTexture(textureVladSheetEW);
}

SDL_Surface* setTransparentColor (SDL_Surface* surface, Uint8 r, Uint8 g, Uint8 b)
{
    //create the transparent color (pink in this case) and put in colorKey
    Uint32 colorKey;
    colorKey = SDL_MapRGB(surface->format, r, g, b);
    //set the colorKey color as the transparent one in the image
    SDL_SetColorKey(surface, SDL_TRUE, colorKey);
    return surface;
}//End setTransparent Color

bool ProgramIsRunning()
{
    SDL_Event event;
    bool running = true;

    while(SDL_PollEvent(&event))
    {
        if(event.type == SDL_QUIT)
            running = false;
    }
    return running;
}

bool InitGame()
{
    //Init SDL
    if(!InitSDL())
        return false;

    //Initiatialize game variables

    return true;
}

bool InitSDL()
{
     //Initialize SDL
    if(SDL_Init(SDL_INIT_EVERYTHING) < 0)
    {
        printf("SDL failed to initialize!\n");
        SDL_Quit();
    }
    //Init audio subsystem
    if(Mix_OpenAudio( 44100, MIX_DEFAULT_FORMAT, 2, 2048 ) == -1)
    {
        printf("Open Audio failed to initialize!\n");
        return false;
    }
    return true;
}

void CreateGameImages()
{
    //SDL_Init( SDL_INIT_EVERYTHING );
    //Create a window
    gameWindow = SDL_CreateWindow(
    "Can you escape the maze?", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);

    //Create a renderer for the window
    renderer = SDL_CreateRenderer(gameWindow, -1, 0);

    //load the tile sheet
    tempSurface = loadImage("graphics/threeTiles.bmp");
    //no need to set transparent color for the tiles
    textureMap = loadTexture(tempSurface);
    //get the map data and create the solid layer
    getMapData();
    generateSolidLayer();

    //setUpEachSheet("graphics/WalkEast/walkEast.bmp", 106, 76, 48, MAX_Caveman_FRAME, textureCavemanEastSheet, SDL_Rect rectCavemanEast);

    //load all Caveman stuff
    //Set up East walking Cavemen
    //create the surface - load the east sheet
    tempSurface = loadImage("graphics/WalkEast/walkEast.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureCavemanEastSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one Caveman walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of Cavemen
    SDL_QueryTexture(textureCavemanEastSheet, NULL, NULL, &rectCavemanEastSheet.w, &rectCavemanEastSheet.h);
    //get the width of one frame (one Caveman) by dividing the width of the sheet by the number of frames
    CavemanWidth = rectCavemanEastSheet.w/MAX_Caveman_FRAME;
    //get the height of one frame
    CavemanHeight = rectCavemanEastSheet.h;  //height on one Caveman is same as height of sheet
    //Initialize the rectangle Caveman east
    FillRect(rectCavemanEast, 0, 0, CavemanWidth, CavemanHeight);

    //Set up West walking Cavemen
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/WalkWest/walkWest.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureCavemanWestSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one Caveman walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of Cavemen
    SDL_QueryTexture(textureCavemanWestSheet, NULL, NULL, &rectCavemanWestSheet.w, &rectCavemanWestSheet.h);
    //get the width of one frame (one Caveman) by dividing the width of the sheet by the number of frames
    CavemanWidth = rectCavemanWestSheet.w/MAX_Caveman_FRAME;
    //get the height of one frame
    CavemanHeight = rectCavemanWestSheet.h;  //height on one Caveman is same as height of sheet
    //Initialize the rectangle for Caveman west
    FillRect(rectCavemanWest, 0, 0, CavemanWidth, CavemanHeight);

    //Set up North walking Cavemen
    //create the surface - load the north sheet
    tempSurface = loadImage("graphics/WalkNorth/walkNorth.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureCavemanNorthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one Caveman walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of Cavemen
    SDL_QueryTexture(textureCavemanNorthSheet, NULL, NULL, &rectCavemanNorthSheet.w, &rectCavemanNorthSheet.h);
    //get the width of one frame (one Caveman) by dividing the width of the sheet by the number of frames
    CavemanWidth = rectCavemanNorthSheet.w/MAX_Caveman_FRAME;
    //get the height of one frame
    CavemanHeight = rectCavemanNorthSheet.h;  //height on one Caveman is same as height of sheet
    //Initialize the rectangle for Caveman north
    FillRect(rectCavemanNorth, 0, 0, CavemanWidth, CavemanHeight);

    //Set up South walking Cavemen
    //create the surface - load the south sheet
    tempSurface = loadImage("graphics/WalkSouth/walkSouth.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureCavemanSouthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one Caveman walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of Cavemen
    SDL_QueryTexture(textureCavemanSouthSheet, NULL, NULL, &rectCavemanSouthSheet.w, &rectCavemanSouthSheet.h);
    //get the width of one frame (one Caveman) by dividing the width of the sheet by the number of frames
    CavemanWidth = rectCavemanSouthSheet.w/MAX_Caveman_FRAME;
    //get the height of one frame
    CavemanHeight = rectCavemanSouthSheet.h;  //height on one Caveman is same as height of sheet
    //Initialize the rectangle for Caveman south
    FillRect(rectCavemanSouth, 0, 0, CavemanWidth, CavemanHeight);

    //load all Vlad stuff
    //Set up East walking Vlad
    //create the surface - load the east sheet
    tempSurface = loadImage("graphics/WalkEast/eastVlad.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureVladEastSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one Vlad walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of Vlad
    SDL_QueryTexture(textureVladEastSheet, NULL, NULL, &rectVladEastSheet.w, &rectVladEastSheet.h);
    //get the width of one frame (one Vlad) by dividing the width of the sheet by the number of frames
    VladWidth = rectVladEastSheet.w/MAX_Vlad_FRAME;
    //get the height of one frame
    VladHeight = rectVladEastSheet.h;  //height on one Vlad is same as height of sheet
    //Initialize the rectangle Vlad east
    FillRect(rectVladEast, 0, 0, VladWidth, VladHeight);

    //Set up West walking Vlad
    //create the surface - load the west sheet
    tempSurface = loadImage("graphics/WalkWest/westVlad.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureVladWestSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one Vlad walking west
    //SDL_QueryTexture() method gets the width and height of the sheet of Vlad
    SDL_QueryTexture(textureVladWestSheet, NULL, NULL, &rectVladWestSheet.w, &rectVladWestSheet.h);
    //get the width of one frame (one Vlad) by dividing the width of the sheet by the number of frames
    VladWidth = rectVladWestSheet.w/MAX_Vlad_FRAME;
    //get the height of one frame
    VladHeight = rectVladWestSheet.h;  //height on one Vlad is same as height of sheet
    //Initialize the rectangle Vlad west
    FillRect(rectVladWest, 0, 0, VladWidth, VladHeight);

    //Set up North walking Vlad
    //create the surface - load the north sheet
    tempSurface = loadImage("graphics/WalkNorth/northVlad.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureVladNorthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one Vlad walking east
    //SDL_QueryTexture() method gets the width and height of the sheet of Vlad
    SDL_QueryTexture(textureVladNorthSheet, NULL, NULL, &rectVladNorthSheet.w, &rectVladNorthSheet.h);
    //get the width of one frame (one Vlad) by dividing the width of the sheet by the number of frames
    VladWidth = rectVladNorthSheet.w/MAX_Vlad_FRAME;
    //get the height of one frame
    VladHeight = rectVladNorthSheet.h;  //height on one Vlad is same as height of sheet
    //Initialize the rectangle Vlad east
    FillRect(rectVladNorth, 0, 0, VladWidth, VladHeight);

    //Set up South walking Vlad
    //create the surface - load the south sheet
    tempSurface = loadImage("graphics/WalkSouth/southVlad.bmp");
    //Set magenta as transparency color
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    //create the texture for the whole sheet
    textureVladSouthSheet = loadTexture(tempSurface);
    //Create the rectangle to hold one Vlad walking west
    //SDL_QueryTexture() method gets the width and height of the sheet of Vlad
    SDL_QueryTexture(textureVladSouthSheet, NULL, NULL, &rectVladSouthSheet.w, &rectVladSouthSheet.h);
    //get the width of one frame (one Vlad) by dividing the width of the sheet by the number of frames
    VladWidth = rectVladSouthSheet.w/MAX_Vlad_FRAME;
    //get the height of one frame
    VladHeight = rectVladSouthSheet.h;  //height on one Vlad is same as height of sheet
    //Initialize the rectangle Vlad west
    FillRect(rectVladSouth, 0, 0, VladWidth, VladHeight);

    //set up explosion
    tempSurface = loadImage("graphics/explosion.bmp");
    tempSurface = setTransparentColor(tempSurface, 106, 76, 48);
    textureExplosionSheet = loadTexture(tempSurface);
    rectExplosion = {0, 0, explosionWidth, explosionHeight};

}

void generateSolidLayer()   //create a structure to hold solid tile data
{
    int begIndex = 0;   //position of solid tile in tiles structure
    for (int row = 0; row < mapRow; ++row)
    {
            for (int col = 0; col < mapCol; ++col)
            {
                int frame = layer2[row*mapCol+col];//get frame ID of solid tile from layer 2
                if (frame > 0)  //if the frame is not zero
                {
                    if (isTileSolid(layer2[row*mapCol+col])) //if tile is solid
                    {
                        tiles[begIndex].solid = 1;              //then solid field is 1
                        tiles[begIndex].frameID = frame;        //save the frame ID
                        tiles[begIndex].x = col*tileWidth;      //save THIS x position of the solid tile
                        tiles[begIndex].y = row*tileHeight;     //save THIS y position of the solid tile
                        begIndex = begIndex + 1;                //move to the next tile
                    }//end if tile is solid
                }
            }
    }
    numSolidTiles = begIndex - 1;
}

bool isTileSolid(int tile)
{
    for(unsigned int i = 0; i < solidTiles.size(); i++)
    {
        if(solidTiles[i] == tile)
        {
            return true;
        }
    }
    return false;
}

void drawLayer(int layer[])
{
    //Draw a Layer
        for (int row = 0; row < mapRow; ++row)
        {
            for (int col = 0; col < mapCol; ++col)
            {
                int frame = layer[row*mapCol+col];//get frame ID number to draw
                //if frameID is not a zero then grab it from sheet
                if(frame > 0)
                {
                    //Compute the x and y destination tile position
                    int xRawBytes = ((frame - 1) * tileWidth);
                    int yRawBytes = ((frame - 1) * tileHeight);
                    int xSource = xRawBytes % sheetWidth;
                    int ySource = (yRawBytes / sheetWidth) * tileHeight;
                    //Initialize the source rectangle with position of tile you want to draw
                    FillRect(srcRect, xSource, ySource, tileWidth, tileHeight);

                    // Compute the x and y location on game screen
                    int xDest = col*tileWidth;
                    int yDest = row*tileHeight;

                    //Initialize the destination rectangle with x and y position on game screen
                    FillRect(dstRect, xDest, yDest, tileWidth, tileHeight);

                    //Grab tile from sheet and place on game screen
                    SDL_RenderCopy(renderer, textureMap, &srcRect, &dstRect);

                }//end if frame > 0
            }//end for col
        }//end for row
}
void UpdateCaveman()
{
    //Increment time to delay each frame
    FrameCounter++;
    //If delay time is greater than max amount
    if(FrameCounter > FRAME_DELAY)
    {
        FrameCounter = 0;   //start new delay time
        SpriteFrame++;      //advance to next frame
    }
    if(SpriteFrame > MAX_Caveman_FRAME - 1) //if last frame is reached
        SpriteFrame = 0;                   //start at frame zero again

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (CavemanDirection == 1)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectCavemanEast.x = SpriteFrame * rectCavemanEast.w;
        if ( xCavemanPos > SCREEN_WIDTH - Caveman_WIDTH)       //check right
        //if edge of bitmap collides with screen
        {
            xCavemanPos = SCREEN_WIDTH - Caveman_WIDTH;        //stop and kiss the side
        }
        //create the rectangle on screen for Caveman placement
        FillRect(rectCaveman, rectCavemanEast.x, rectCavemanEast.y, rectCavemanEast.w, rectCavemanEast.h);
        textureCavemanSheet = textureCavemanEastSheet;
    }// end walking east

    if (CavemanDirection == 0)                       //walking to the west (left)
    {
        //Advance to next frame on sheet
        rectCavemanWest.x = SpriteFrame * rectCavemanWest.w;
       // xCavemanPos = xCavemanPos - Caveman_SPEED;              //decrement x
        if ( xCavemanPos < 0 )                                //check left
        //if edge of bitmap collides with screen
        {
            xCavemanPos = 0;                                  //stop and kiss the side
        }
        //create the rectangle on screen for Caveman placement
        FillRect(rectCaveman, rectCavemanWest.x, rectCavemanWest.y, rectCavemanWest.w, rectCavemanWest.h);
        textureCavemanSheet = textureCavemanWestSheet;
    }// end walking West

    if (CavemanDirection == 2)                       //if we are going south
    {
        //Advance to next frame on sheet
        rectCavemanSouth.x = SpriteFrame * rectCavemanSouth.w;
        //yCavemanPos = yCavemanPos + Caveman_SPEED;                       //increment y
        if ( yCavemanPos > SCREEN_HEIGHT - Caveman_HEIGHT )    //check bottom
        {
            yCavemanPos = SCREEN_HEIGHT - Caveman_HEIGHT;      //stop and kiss the side
        }
        //create the rectangle on screen for Caveman placement
        FillRect(rectCaveman, rectCavemanSouth.x, rectCavemanSouth.y, rectCavemanSouth.w, rectCavemanSouth.h);
        //copy the south facing Cavemen to the generic sheet
        textureCavemanSheet = textureCavemanSouthSheet;
    }// end walking south

    if (CavemanDirection == 3)                       //if we are going north
    {
        //Advance to next frame on sheet
        rectCavemanNorth.x = SpriteFrame * rectCavemanNorth.w;
        //yCavemanPos = yCavemanPos - Caveman_SPEED;                       //decrement y
        if ( yCavemanPos < 0 )                                //check top
        //if edge of bitmap collides with screen
        {
            yCavemanPos = 0;                                  //stop and kiss the side
        }
        //create the rectangle on screen for Caveman placement
        FillRect(rectCaveman, rectCavemanNorth.x, rectCavemanNorth.y, rectCavemanNorth.w, rectCavemanNorth.h);
        //copy the north facing Caveman to the generic Caveman sheet
        textureCavemanSheet = textureCavemanNorthSheet;
    }// end walking north

    //Initialize the placement rectangle to the position on the screen the Caveman will be drawn.
    FillRect(rectCavemanPlacement, xCavemanPos, yCavemanPos, CavemanWidth/2, CavemanHeight/2);

    //Now check to see if the Caveman has collided with a solid tile
    PlayerTileCollision(rectCavemanPlacement, xCavemanPos, yCavemanPos);

    //Now check to see if the Caveman has collided with an enemy
    playerEnemyCollide(rectCavemanPlacement, rectVladPlacementEW);
}

void updateEastWestVlad()
{
    bool collision;
    //Increment time to delay each frame
    FrameCounterVladEW++;
    //If delay time is greater than max amount
    if(FrameCounterVladEW > FRAME_DELAY)
    {
        FrameCounterVladEW = 0;   //start new delay time
        SpriteFrameVladEW++;      //advance to next frame
    }
    if(SpriteFrameVladEW > MAX_Vlad_FRAME - 1) //if last frame is reached
        SpriteFrameVladEW = 0;                   //start at frame zero again

    //Note:  west = 0, east = 1, south = 2, north = 3
    // west = 0
    if (VladDirectionEW == 1)                       //if we are going east (right)
    {
        //Advance to next frame on sheet
        rectVladEast.x = SpriteFrameVladEW * rectVladEast.w;
        xVladEWpos = xVladEWpos + Vlad_SPEED;              //increment x
        //create the rectangle on screen for Vlad placement
        FillRect(rectVladEW, rectVladEast.x, rectVladEast.y, rectVladEast.w, rectVladEast.h);
        textureVladSheetEW = textureVladEastSheet;
    }// end walking east

    if (VladDirectionEW == 0)                       //walking to the west (left)
    {
        //Advance to next frame on sheet
        rectVladWest.x = SpriteFrameVladEW * rectVladWest.w;
        xVladEWpos = xVladEWpos - Vlad_SPEED;              //decrement x
        //create the rectangle on screen for Vlad placement
        FillRect(rectVladEW, rectVladWest.x, rectVladWest.y, rectVladWest.w, rectVladWest.h);
        textureVladSheetEW = textureVladWestSheet;
    }// end walking West

    //Now check to see if the Vlad has collided with a solid tile
    collision = EnemyTileCollision(rectVladPlacementEW, VladDirectionEW);

    if (collision == true)
    {
        FrameCounterVladEW = 0;       //start new position
    }//end if

    //Initialize the placement rectangle to the position on the screen the Vlad will be drawn.
    FillRect(rectVladPlacementEW, xVladEWpos, yVladEWpos, VladWidth/2.5, VladHeight/2.5);
}

void RunGame()
{
    UpdateCaveman();              //Update position of Caveman
    updateEastWestVlad();     //Update position of east/west Vlad
    DrawGame();
}

void DrawGame()
{
    SDL_RenderPresent(renderer);

    //Clear the window
    SDL_RenderClear(renderer);

    //Copy the background)
    //SDL_RenderCopy(renderer, textureBackground, NULL, &rectBackground);

    //draw each layer
    drawLayer (layer1);
    drawLayer (layer2);

    //Render the Caveman
    SDL_RenderCopy(renderer, textureCavemanSheet, &rectCaveman, &rectCavemanPlacement);
    //Render the Vlad
    SDL_RenderCopy(renderer, textureVladSheetEW, &rectVladEW, &rectVladPlacementEW);

    //display the game screen with updated position of Caveman
    SDL_RenderPresent(renderer);
    SDL_Delay(5);  //delay a bit
}

    //Note:  west = 0, east = 1, south = 2, north = 3
    void moveUp() {
    CavemanDirection = 3;
    xCavemanPos -= Caveman_SPEED; // Move the caveman left
}

    void moveDown() {
    CavemanDirection = 2;
    xCavemanPos += Caveman_SPEED; // Move the caveman right
}


    void moveLeft() {
    CavemanDirection = 0;
    xCavemanPos -= Caveman_SPEED; // Move the caveman left
}

    void moveRight() {
    CavemanDirection = 1;
    xCavemanPos += Caveman_SPEED; // Move the caveman right
}


void Collide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &xPos, int &yPos )
{
        //we're here because we've had a collision with a solid tile
        //now determine what side of the solid tile did the movingRect collide with
        bool intersectsTop = false;     //Collision with top of obstacle
        bool intersectsRight = false;   //Collision with right side of obstacle
        bool moveInXDirection = false;  //Did collision occur while moving right(east) or left(west)?

        if(movingRect.x > obstacleRect.x)   //if the movingRect x is greater than the obstacle x
            intersectsRight = true;         //then the collision is on the right side of the obstacle
        // y up is neg
        if(movingRect.y < obstacleRect.y)   //if the movingRect y is less than the obstacle y
            intersectsTop = true;           //then the collision is on the top of the obstacle

        //compute the height & width of the intersection rectangle (where the two rects collide)
        //to determine which side of the obstacle the movingRect hit
        int cHeight, cWidth;

        if(intersectsTop)  //if the collision is on the top of the obstacle
        {
           cHeight = abs(obstacleRect.y - (movingRect.y + movingRect.h));
        }
        else    //else collision was on bottom, left, or right
        {
            cHeight = abs(obstacleRect.y + obstacleRect.h - movingRect.y);
        }
        if(intersectsRight) //if the collision is on the right of the obstacle
        {
             cWidth = abs(obstacleRect.x + obstacleRect.w - movingRect.x);
        }
        else    //else collision was on left, top, or bottom
        {
            cWidth = abs(obstacleRect.x - (movingRect.x + movingRect.w));
        }

        if (cHeight > cWidth) //was the collision from the left or the right?
            moveInXDirection = true;    //if so then moving in the x direction is true
        else
            moveInXDirection = false;   //if not then moving in the x direction is false

        //now determine which direction the sprite needs to back off from
        if(moveInXDirection)            //if moving left or right
        {
            if(intersectsRight)         //if colliding on the right
            {
                xPos = xPos + 1;    //stop by increasing x (move back right)
            }
            else                        //else colliding on the left
            {
                xPos = xPos - 1;    //stop by decreasing x (move back left)
            }
        }
        else                            //else moving up or down
        {
            if(intersectsTop)           //if colliding with top
            {
                yPos = yPos - 1;    //stop by decreasing y (move back up)
            }
            else                        //else colliding with bottom
            {
                yPos = yPos + 1;    //stop by increasing y (move back down)
            }
        }//end: back off, bub.
}//end Collide

void PlayerTileCollision(SDL_Rect movingRect, int &xPos, int &yPos)
{
    //   indexes to cycle through solid tile vector array
    int begIndex = 0;
    //int endIndex = solidTiles.size();
    int endIndex = numSolidTiles;

    //make the moving rectangle a constant
    const SDL_Rect& moving = movingRect;

    for (int i = begIndex; i <= endIndex; i++)
    {
        //Get the x and y location of solid tile
        int xDest = tiles[i].x;
        int yDest = tiles[i].y;

        //make the solid tile a rectangle
        FillRect(solidTileRect, xDest, yDest, tileWidth, tileHeight);

        //make the solid rectangle a constant
        const SDL_Rect& solid = solidTileRect;

        //now check to see if the moving rectangle (sprite) has intersected a solid tile
        if( SDL_HasIntersection(&moving, &solid))
        {
            //the moving sprite has collided with the solid tile so stop that sprite in its tracks
            Collide(movingRect, solidTileRect, xPos, yPos);
        }//end if intersected
    }//end for
}//end tile collision

bool EnemyTileCollision(SDL_Rect movingRect, int &direction)
{
    //loop indexes to cycle through solid tile vector array
    int i = 0;
    //int endIndex = solidTiles.size();

    //make the moving rectangle a constant
    const SDL_Rect& moving = movingRect;
    bool done = false;

        while (i <= numSolidTiles && done == false)
        {
            //Get the x and y location of solid tile
            int xDest = tiles[i].x;
            int yDest = tiles[i].y;

            //make the solid tile a rectangle
            FillRect(solidTileRect, xDest, yDest, tileWidth, tileHeight);

            //make the solid rectangle a constant
            const SDL_Rect& solid = solidTileRect;

            //now check to see if the moving rectangle (sprite) has intersected a solid tile
            if( SDL_HasIntersection(&moving, &solid))
            {
                //the moving sprite has collided with the solid tile so stop that sprite in its tracks
                enemyCollide(movingRect, solidTileRect, direction);
                done = true;       //done checking solid tiles as soon as 1 collision occurs
            }//end if intersected
            i++;
        }//end while
        if (done == true)
            return true;
        else
            return false;
}//end tile collision

void enemyCollide(SDL_Rect movingRect, SDL_Rect obstacleRect, int &direction)
{
    {
        //now determine what side of the obstacle did the movingRect collide with?
        bool intersectsTop = false;     //Collision with top of obstacle
        bool intersectsRight = false;   //Collision with right side of obstacle
        bool moveInXDirection = false;  //Did collision occur while moving right(east) or left(west)?

        if(movingRect.x > obstacleRect.x)   //if the movingRect x is greater than the obstacle x
            intersectsRight = true;         //then the collision is on the right side of the obstacle
        // y up is neg
        if(movingRect.y < obstacleRect.y)   //if the movingRect y is less than the obstacle y
            intersectsTop = true;           //then the collision is on the top of the obstacle

        //compute the height & width of the intersection rectangle (where the two rects collide)
        //to determine which side of the obstacle the movingRect hit
        int cHeight, cWidth;

        if(intersectsTop)  //if the collision is on the top of the obstacle
        {
           cHeight = abs(obstacleRect.y - (movingRect.y + movingRect.h));
        }
        else    //else collision was on bottom, left, or right
        {
            cHeight = abs(obstacleRect.y + obstacleRect.h - movingRect.y);
        }
        if(intersectsRight) //if the collision is on the right of the obstacle
        {
             cWidth = abs(obstacleRect.x + obstacleRect.w - movingRect.x);
        }
        else    //else collision was on left, top, or bottom
        {
            cWidth = abs(obstacleRect.x - (movingRect.x + movingRect.w));
        }

        if (cHeight > cWidth) //was the collision from the left or the right?
            moveInXDirection = true;    //if so then moving in the x direction is true
        else
            moveInXDirection = false;   //if not then moving in the x direction is false

        // adjust moving object's position accordingly
        if(moveInXDirection == true)            //if moving left or right
        {
            if(intersectsRight == true)         //if colliding on the right
            {
                direction = 1;    //change direction to left
            }
            //else
            if(intersectsRight == false)
                //else colliding on the left
            {
                direction = 0;    //change direction to right
            }
        }
        //else                            //else moving up or down
        if (moveInXDirection == false)
        {
            if(intersectsTop == true)           //if colliding with top
            {
                direction = 3;    //then change direction to up
            }
            //else                        //else colliding with bottom
            if(intersectsTop == false)
            {
                direction = 2;    //then change direction to down
            }
        }//end adjustment of direction
     }//end check if there is a collision
}//end Collide

void playerEnemyCollide(SDL_Rect playerRect, SDL_Rect enemyRect)
{
    //make the moving rectangle a constant
    const SDL_Rect& PLAYER_RECT = playerRect;
    //make the solid rectangle a constant
    const SDL_Rect& ENEMY_RECT = enemyRect;
    //create a constant rectangle with current Vlad position for comparing
    const SDL_Rect& CONST_Vlad_RECT = rectVladPlacementEW;

   // if( SDL_HasIntersection(&PLAYER_RECT, &ENEMY_RECT))
   if (CheckCollision(playerRect, enemyRect))
    {
        if (SDL_RectEquals(&ENEMY_RECT, &CONST_Vlad_RECT))
        {
            VladDestroyed = true;
            destroyVlad();
        }
    }
}

void explosion(int xEnemyPos, int yEnemyPos)

{

    int frameCounterExplosion = 0;

    int SpriteFrameExplosion = 0;

    FillRect(rectExplosionPlacement, (xEnemyPos - 25), (yEnemyPos-25), explosionWidth, explosionHeight);



    for (int pos = 0; pos < MAX_EXPLOSION_FRAME; pos++)

    {

        if(frameCounterExplosion > FRAME_DELAY)

        {

            frameCounterExplosion = 0;

            SpriteFrameExplosion++;

        }

        if(SpriteFrameExplosion > MAX_EXPLOSION_FRAME - 1)

        SpriteFrameExplosion = 0;

        rectExplosion.x = SpriteFrameExplosion * explosionWidth;



        FillRect(rectExplosion, rectExplosion.x, rectExplosion.y, rectExplosion.w, rectExplosion.h);

        SDL_RenderCopy(renderer, textureExplosionSheet, &rectExplosion, &rectExplosionPlacement);

        SDL_RenderPresent(renderer);

        SDL_Delay(100);

        SpriteFrameExplosion++;

    }//end FOR

}//end explosion

bool CheckCollision(SDL_Rect r1,SDL_Rect r2)
{
    //check to see if rectangles overlap
    //if not, return false
    if(r1.x > r2.x+r2.w)
    {
      return false;
    }
    else if(r1.y > r2.y+r2.h)
    {
      return false;
    }
    else if(r1.x+r1.w < r2.x)
    {
      return false;
    }
    else if(r1.y+r1.h < r2.y)
    {
      return false;
    }
    else
    {
      return true;
    }
}


