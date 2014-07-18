#include "includes.h"

SDL_Window * mainWindow = NULL;
SDL_Renderer * mainRenderer = NULL;



bool init();
bool loadMedia();
void close();
bool checkCollision(vector<SDL_Rect>& a, vector<SDL_Rect>& b);

const int gameWidth = 1280;
const int gameHeight = 720;


#include <iostream>
#include <cstring>
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include <vector>
using namespace std;


class monster
{

public:
    monster(int, int, int);

    void renderMonster();
    void shoot();
    void moveMonster();
    void kill();
    vector<SDL_Rect> &getCollider();
    void setCollider();
    void setSize();

private:

    int spawnX, spawnY;
    int monsterType;
    int offsetX, offsetY;
    vector<SDL_Rect> mCollider;
    int monsterWidth;
    int monsterHeight;
    bool changeDir;

};

class bullet
{
public:

    bullet(int, int);

    static const int bulletvelocity = 12;
    void travel();
    void renderbullet();
    void destroy();
    void setCollider();
    vector<SDL_Rect>& getCollider();
private:

    int bPosX, bPosY;
    int bVelY;
    vector<SDL_Rect> bCollider;

};

struct node
{
    bullet * currentBullet;

    node * next;
};

void insertLLLnode(node * &head, int, int);
void renderLLLbullet(node * head);
void findCollision(node * head, shipProtector, shipProtector, shipProtector, shipProtector, shipProtector);
void setColliders(node * head);

class mainShip
{

 public:

    mainShip();

    static const int shipWidth = 64;
    static const int shipHeight = 64;
    static const int shipVelocity = 10;

    void handleEvent(SDL_Event &event);
    void moveShip();
    void render();

 private:
        int sPosX, sPosY;
        int sVelX, sVelY;


};

class GameTexture
{
    public:

    GameTexture();
    ~GameTexture();

    bool loadFromFile(string);
    void deallocate();
    void render(int, int);
    void free();
    int getWidth();
    int getHeight();

    private:

        SDL_Texture * mTexture;
        int tWidth;
        int tHeight;
};

class shipProtector
{
    public:

        static const int protectorWidth = 128;
        static const int protectorHeight  = 110;

    shipProtector(int, int);
    void renderProtector();
    vector<SDL_Rect>& getColliders(int);
    int getX();
    int getY();
    void coutCollision();
    void setColliders();

private:

    int pPosX, pPosY;
    vector<SDL_Rect> pColliders;

};




node * head = NULL;

GameTexture bgTexture;
GameTexture shipTexture;

GameTexture pTexLBB;
GameTexture pTexRBB;
GameTexture pTexLBT;
GameTexture pTexLCEB;
GameTexture pTexLCET;
GameTexture pTexLCOB;
GameTexture pTexLCOT;
GameTexture pTexRBT;
GameTexture pTexRCEB;
GameTexture pTexRCET;
GameTexture pTexRCOB;
GameTexture pTexRCOT;
GameTexture bulletTexture;
GameTexture monsterTypeA;
GameTexture monsterTypeB;
GameTexture monsterTypeC;


int main(int argc, char * argv[])
{




    int adder = 150;
    if(!init())
        cout << "\nSDL Failed to Initialize. ERROR: " << SDL_GetError();

    else
    {
        if(!loadMedia())
            cout << "\nLoading media failed. ERROR: " << SDL_GetError();
        else
        {

            bool quit = false;
            SDL_Event event;
            mainShip ship;
            monster testMonster(1140, 250, 1);
            monster testMonsterb(1040, 250, 2);
            monster testMonsterc(940, 250, 3);


            //shipProtector protectora(150, 440);
            shipProtector protectorb(375, 440);
            shipProtector protectorc(600, 440);
            shipProtector protectord(825, 440);
            shipProtector protectore(1025, 440);

            //protectora.setColliders();
            protectorb.setColliders();
            protectorc.setColliders();
            protectord.setColliders();
            protectore.setColliders();

            while (!quit)
            {
                while(SDL_PollEvent(&event) != 0)
                {
                    if(event.type == SDL_QUIT)
                        quit = true;

                    ship.handleEvent(event);
                }


             ship.moveShip();
             testMonster.setSize();
             testMonsterb.setSize();
             testMonsterc.setSize();
             testMonster.moveMonster();
             testMonsterb.moveMonster();
             testMonsterc.moveMonster();


             SDL_SetRenderDrawColor(mainRenderer, 0xFF, 0xFF, 0xFF,0xFF);
             bgTexture.render(0,0);

             shipProtector protectora(adder, 440);
             protectora.setColliders();

            setColliders(head);
            findCollision(head, protectora, protectorb, protectorc, protectord, protectore);



             ship.render();
             protectora.renderProtector();
             protectorb.renderProtector();
             protectorc.renderProtector();
             protectord.renderProtector();
             protectore.renderProtector();

             renderLLLbullet(head);
             testMonster.renderMonster();
             testMonsterb.renderMonster();
             testMonsterc.renderMonster();



             SDL_RenderPresent(mainRenderer); // RENDER ALL TO FRONT BUFFER


           /*  if(checkCollision(protectora.getColliders(),protectorb.getColliders()))
                cout << "\nCOLLISION DETECTED!";
             else if(!(checkCollision(protectora.getColliders(),protectorb.getColliders())))
                cout << "\nNO COLLISION";
             adder += 1;*/

            }

        }
    }

    close();
    return 0;
}


bool checkCollision(vector<SDL_Rect>& a, vector<SDL_Rect>& b)
{
    int leftA, leftB;
    int rightA, rightB;
    int topA, topB;
    int bottomA, bottomB;

    for( int Abox = 0; Abox < a.size(); Abox++ )
    {
        leftA = a[ Abox ].x;
        rightA = a[ Abox ].x + a[ Abox ].w;
        topA = a[ Abox ].y;
        bottomA = a[ Abox ].y + a[ Abox ].h;

        for( int Bbox = 0; Bbox < b.size(); Bbox++ )
        {

            leftB = b[ Bbox ].x;
            rightB = b[ Bbox ].x + b[ Bbox ].w;
            topB = b[ Bbox ].y;
            bottomB = b[ Bbox ].y + b[ Bbox ].h;


            if( ( ( bottomA <= topB ) || ( topA >= bottomB ) || ( rightA <= leftB ) || ( leftA >= rightB ) ) == false )
                return true;

        }
    }


    return false;

}

void insertLLLnode(node *&head, int PosX, int PosY)
{
    if(!head)
        {
            head = new node;
            head -> currentBullet = new bullet(PosX +31, PosY);
            head -> next = NULL;
            return;
        }
    insertLLLnode(head -> next, PosX, PosY);
}

void renderLLLbullet(node * head)
{
    if(!head)
        return;
    head -> currentBullet->renderbullet();
    head -> currentBullet->travel();
    renderLLLbullet(head -> next);
}

void findCollision(node * head, shipProtector a, shipProtector b, shipProtector c, shipProtector d, shipProtector e)
{
        if(!head)
            return;
        for (int i = 0; i < 12; i++)
        {
            if(
        checkCollision(a.getColliders(i), head -> currentBullet -> getCollider()) == true ||
        checkCollision(b.getColliders(i), head -> currentBullet -> getCollider()) == true ||
        checkCollision(c.getColliders(i), head -> currentBullet -> getCollider()) == true ||
        checkCollision(d.getColliders(i), head -> currentBullet -> getCollider()) == true ||
        checkCollision(e.getColliders(i), head -> currentBullet -> getCollider()) == true)
            cout << "\nCollision DETECTED!";

        }


    findCollision(head -> next, a,b,c,d,e);

}

void setColliders(node * head)
{
    if(!head)
        return;
    head -> currentBullet -> setCollider();

    setColliders(head -> next);
}


bool init()
{
    bool working = true;

    if(SDL_Init(SDL_INIT_VIDEO))
    {
        cout << "\nSDL Video Subsystem failed. ERROR: " << SDL_GetError();
        working = false;
    }
    else
    {
        mainWindow = SDL_CreateWindow("Space Invaders v 1.0.5", ((1920 - gameWidth) / 2), ((1080 - gameHeight) / 2), gameWidth, gameHeight, SDL_WINDOW_SHOWN);
        if(!mainWindow)
        {
            cout << "\nFailed to create main game window. ERROR: " << SDL_GetError();
            working = false;
        }
        else
        {
        mainRenderer = SDL_CreateRenderer(mainWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
        if(!mainRenderer)
        {
            cout << "\nFailed to initialize renderer. ERROR: " << SDL_GetError();
            working = false;
        }
        else
        {
            SDL_SetRenderDrawColor(mainRenderer, 0xFF, 0xFF, 0xFF, 0xFF);


            int imgFlags = IMG_INIT_PNG;
            if(!(IMG_Init(imgFlags) & imgFlags))
            {
                cout << "\nSDL_image PNG module failed to initialize.Error: " << SDL_GetError();
                working = false;
            }
        }
      }
    }
    return working;
}

bool loadMedia()
{
    bool loaded = true;

        if(!bgTexture.loadFromFile("_img/mainBG.png"))
        {
            cout << "\nFailed to load BG texture. ERROR: " << SDL_GetError();
            loaded = false;
        }

        if(!shipTexture.loadFromFile("_img/ship.png"))
        {
            cout << "\nFailed to load ship texture. ERROR: " << SDL_GetError();
            loaded = false;
        }


        if(!pTexLBB.loadFromFile("_img/protectorLBB.png"))
        {
            cout << "\nFailed to load ship protector texture ERROR: " << SDL_GetError();
            loaded = false;
        }
         if(!pTexLBT.loadFromFile("_img/protectorLBT.png"))
        {
            cout << "\nFailed to load ship protector texture ERROR: " << SDL_GetError();
            loaded = false;
        }
         if(!pTexLCEB.loadFromFile("_img/protectorLCEB.png"))
        {
            cout << "\nFailed to load ship protector texture ERROR: " << SDL_GetError();
            loaded = false;
        }
         if(!pTexLCET.loadFromFile("_img/protectorLCET.png"))
        {
            cout << "\nFailed to load ship protector texture ERROR: " << SDL_GetError();
            loaded = false;
        }
         if(!pTexLCOB.loadFromFile("_img/protectorLCOB.png"))
        {
            cout << "\nFailed to load ship protector texture ERROR: " << SDL_GetError();
            loaded = false;
        }
         if(!pTexLCOT.loadFromFile("_img/protectorLCOT.png"))
        {
            cout << "\nFailed to load ship protector texture ERROR: " << SDL_GetError();
            loaded = false;
        }
         if(!pTexRBB.loadFromFile("_img/protectorRBB.png"))
        {
            cout << "\nFailed to load ship protector texture ERROR: " << SDL_GetError();
            loaded = false;
        }

        if(!pTexRBT.loadFromFile("_img/protectorRBT.png"))
        {
            cout << "\nFailed to load ship protector texture ERROR: " << SDL_GetError();
            loaded = false;
        }
        if(!pTexRCEB.loadFromFile("_img/protectorRCEB.png"))
        {
            cout << "\nFailed to load ship protector texture ERROR: " << SDL_GetError();
            loaded = false;
        }
         if(!pTexRCET.loadFromFile("_img/protectorRCET.png"))
        {
            cout << "\nFailed to load ship protector texture ERROR: " << SDL_GetError();
            loaded = false;
        }
         if(!pTexRCOB.loadFromFile("_img/protectorRCOB.png"))
        {
            cout << "\nFailed to load ship protector texture ERROR: " << SDL_GetError();
            loaded = false;
        }
         if(!pTexRCOT.loadFromFile("_img/protectorRCOT.png"))
        {
            cout << "\nFailed to load ship protector texture ERROR: " << SDL_GetError();
            loaded = false;
        }
         if(!bulletTexture.loadFromFile("_img/bullet.png"))
        {
            cout << "\nFailed to load bullet texture ERROR: " << SDL_GetError();
            loaded = false;
        }

        if(!monsterTypeA.loadFromFile("_img/monsterTypeA.png"))
        {
            cout << "\nFailed to load monsterA texture ERROR: " << SDL_GetError();
            loaded = false;
        }
        if(!monsterTypeB.loadFromFile("_img/monsterTypeB.png"))
        {
            cout << "\nFailed to load monsterB texture ERROR: " << SDL_GetError();
            loaded = false;
        }
        if(!monsterTypeC.loadFromFile("_img/monsterTypeC.png"))
        {
            cout << "\nFailed to load monsterC texture ERROR: " << SDL_GetError();
            loaded = false;
        }

    return loaded;
}



void close()
{
    bgTexture.free();
    shipTexture.free();

    SDL_DestroyRenderer(mainRenderer);
    SDL_DestroyWindow(mainWindow);
    mainRenderer = NULL;
    mainWindow = NULL;

    IMG_Quit();
    SDL_Quit();
}

bullet::bullet(int posX, int posY)
{
    bPosX = posX;
    bPosY = posY;
    bVelY = 0;
    bCollider.resize(1);
    bCollider[0].w = 4;
    bCollider[0].h = 16;
}

void bullet::travel()
{
    bPosY -= bulletvelocity;
}

void bullet::renderbullet()
{
    bulletTexture.render(bPosX, bPosY);
}

void bullet::setCollider()
{
    bCollider[0].x = bPosX;
    bCollider[0].y = bPosY;
}

vector<SDL_Rect> &bullet::getCollider()
{
    return bCollider;
}

shipProtector::shipProtector(int x, int y)
{
    pPosX = x;
    pPosY = y;

    pColliders.resize(12); // creates 12 colliders
    pColliders[0].w = 32;
    pColliders[0].h = 28;

    pColliders[1].w = 32;
    pColliders[1].h = 28;

    pColliders[2].w = 32;
    pColliders[2].h = 25;

    pColliders[3].w = 32;
    pColliders[3].h = 25;

    pColliders[4].w = 46;
    pColliders[4].h = 32;

    pColliders[5].w = 46;
    pColliders[5].h = 32;

    pColliders[6].w = 43;
    pColliders[6].h = 25;

    pColliders[7].w = 43;
    pColliders[7].h = 25;

    pColliders[8].w = 19;
    pColliders[8].h = 25;

    pColliders[9].w = 19;
    pColliders[9].h = 25;

    pColliders[10].w = 19;
    pColliders[10].h = 17;

    pColliders[11].w = 19;
    pColliders[11].h = 17;
}


void shipProtector::setColliders()
{

        pColliders[0].x = pPosX;
        pColliders[0].y = pPosY + protectorHeight - (pColliders[0].h / 2);

        pColliders[1].x = pPosX +  protectorWidth  - (pColliders[1].w);
        pColliders[1].y = pPosY +  protectorHeight - (pColliders[1].h);

        pColliders[2].x = pPosX;
        pColliders[2].y = pPosY + protectorHeight - (pColliders[1].h) - (pColliders[2].h);

        pColliders[3].x = pPosX +  protectorWidth  - (pColliders[3].w);
        pColliders[3].y = pPosY +  protectorHeight - pColliders[1].h - (pColliders[3].h);

        pColliders[4].x = pPosX;
        pColliders[4].y = pPosY + (pColliders[6].h);

        pColliders[5].x = pPosX +  protectorWidth  - (pColliders[5].w);
        pColliders[5].y = pPosY + (pColliders[7].h);

        pColliders[6].x = pPosX;
        pColliders[6].y = pPosY;

        pColliders[7].x = pPosX + protectorWidth - (pColliders[7].w) - 3;
        pColliders[7].y = pPosY;

        pColliders[8].x = pPosX + (pColliders[6].w) + 3;
        pColliders[8].y = pPosY;

        pColliders[9].x = pPosX + protectorWidth / 2;
        pColliders[9].y = pPosY;

        pColliders[10].x = pPosX + (pColliders[4].w);
        pColliders[10].y = pPosY + (pColliders[8].h);

        pColliders[11].x = pPosX + protectorWidth / 2;
        pColliders[11].y = pPosY + (pColliders[8].h);

}

void shipProtector::renderProtector()
{
    pTexLBB.render(pPosX, pPosY);
    pTexRBB.render(pPosX, pPosY);
    pTexLBT.render(pPosX, pPosY);
    pTexLCEB.render(pPosX, pPosY);
    pTexLCET.render(pPosX, pPosY);
    pTexLCOB.render(pPosX, pPosY);
    pTexLCOT.render(pPosX, pPosY);
    pTexRBT.render(pPosX, pPosY);
    pTexRCEB.render(pPosX, pPosY);
    pTexRCET.render(pPosX, pPosY);
    pTexRCOB.render(pPosX, pPosY);
    pTexRCOT.render(pPosX, pPosY);
}

int shipProtector::getX()
{
    return pPosX;
}

int shipProtector::getY()
{
    return pPosY;
}


vector<SDL_Rect> &shipProtector::getColliders(int current)
{
    return pColliders;
}

void shipProtector::coutCollision()
{
    cout << pColliders[5].x << " ";
    cout << pColliders[5].y;
 }

GameTexture::GameTexture()
{
    mTexture = NULL;
    tWidth = 0;
    tHeight = 0;
}

GameTexture::~GameTexture()
{
    free();
}

bool GameTexture::loadFromFile(string path)
{
    free(); // Clear previous texture.
    SDL_Texture * newTexture = NULL;
    SDL_Surface * loadedSurface = IMG_Load(path.c_str());
        if (!loadedSurface)
            cout << "\nFailed to load image at " << path << " ERROR: " << SDL_GetError();
        else
        {
            newTexture = SDL_CreateTextureFromSurface(mainRenderer, loadedSurface);
                if(!newTexture)
                    cout << "\nUnable to create game texture form file. EROR: " << SDL_GetError();
            else
            {
                tWidth = loadedSurface -> w;
                tHeight = loadedSurface -> h;
            }

            SDL_FreeSurface(loadedSurface);
        }
mTexture = newTexture;
return mTexture != NULL;

}

void GameTexture::free()
{
    if(mTexture != NULL)
    {
        SDL_DestroyTexture(mTexture);
        mTexture = NULL;
        tWidth = 0;
        tHeight = 0;
    }
}

void GameTexture::render(int x, int y)
{
    SDL_Rect renderquad = {x, y, tWidth, tHeight};
    SDL_RenderCopy(mainRenderer, mTexture, NULL, &renderquad);
}

int GameTexture::getWidth()
{
    return tWidth;
}

int GameTexture::getHeight()
{
    return tHeight;
}

monster::monster(int x, int y, int type)
{
    spawnX = x;
    spawnY = y;
    offsetX = 15;
    offsetY = 0;
    mCollider.resize(1);
    mCollider[0].w = 0;
    mCollider[0].h = 32;
    monsterWidth = 0;
    monsterHeight = 32;
    monsterType = type;
    changeDir = false;

}


void monster::renderMonster()
{
    switch(monsterType)
    {
    case 1:
        {
            monsterTypeA.render(spawnX, spawnY);
            break;
        }
    case 2:
        {
            monsterTypeB.render(spawnX, spawnY);
            break;
        }
    case 3:
        {
            monsterTypeC.render(spawnX, spawnY);
            break;
        }

    default:
        {
            cout << "\nENTERED MONSTER SWITCH DEFAULT!";
            break;
        }
    }
}

void monster::shoot()
{
    insertLLLnode(head, spawnX, spawnY);
}

void monster::moveMonster()
{

    if(spawnX + monsterWidth > gameWidth)
        changeDir = true;
    else if(spawnX < 0)
        changeDir = false;

    if(!changeDir)
    {
       spawnX += offsetX;
       cout << "\nSPAWN X : " << spawnX;
    }


    else if(changeDir)
    {
        spawnX -= offsetX;
        cout << "\nHELLO";
    }



}

void monster::setCollider()
{
    mCollider.resize(1);
    mCollider[0].x = spawnX;
    mCollider[0].y = spawnY;

    switch(monsterType)
     {
    case 1:
        {
            mCollider[0].w = 32;
            break;
        }
    case 2:
        {
            mCollider[0].w = 44;
            break;
        }
    case 3:
        {
            mCollider[0].w = 48;
            break;
        }
    }
}

vector<SDL_Rect> &monster::getCollider()
{
    return mCollider;
}

void monster::setSize()
{
    switch(monsterType)
     {
    case 1:
        {
            monsterWidth = 32;
            break;
        }
    case 2:
        {
             monsterWidth = 44;
            break;
        }
    case 3:
        {
             monsterWidth = 48;
            break;
        }
    }
}


mainShip::mainShip()
{
    sPosX = 640;
    sPosY = 656;
    sVelX = 0;
    sVelY = 0;
}

void mainShip::handleEvent(SDL_Event &event)
{

    if(event.type == SDL_KEYDOWN && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_UP:
                {
                    cout << "\nPressed KEY UP!";
                    sVelY -= shipVelocity;
                    cout << "\nVELOCITY ON Y IS: " << sVelY;
                    break;
                }
            case SDLK_DOWN:
                {
                    cout << "\nPressed KEY DOWN!";
                    sVelY += shipVelocity;
                    cout << "\nVELOCITY ON Y IS: " << sVelY;
                    break;
                }
            case SDLK_LEFT:
                {
                    cout << "\n\nPressed KEY LEFT!";
                    sVelX -= shipVelocity;
                    cout << "\nVELOCITY ON X IS: " << sVelX;
                    cout << "\nPOSITION ON X IS: " << sPosX;
                    break;
                }
            case SDLK_RIGHT:
                {
                    cout << "\nPressed KEY RIGHT!";
                    sVelX += shipVelocity;
                    cout << "\nVELOCITY ON X IS: " << sVelX;
                    cout << "\nPOSITION ON X IS: " << sPosX;
                    break;
                }
            case SDLK_SPACE:
                {
                    cout << "\nSPACE IS PRESSED";
                    insertLLLnode(head, sPosX, sPosY);
                    break;
                }

        }
    }

    else if(event.type == SDL_KEYUP && event.key.repeat == 0)
    {
        switch (event.key.keysym.sym)
        {
            case SDLK_UP:
                {
                    cout << "\nUP KEY RELEASED";
                    sVelY += shipVelocity;
                    break;
                }
            case SDLK_DOWN:
                {
                    cout << "\nDOWN KEY RELEASED";
                    sVelY -= shipVelocity;
                    break;
                }
            case SDLK_LEFT:
                {
                    cout << "\n\nLEFT KEY RELEASED";
                    sVelX += shipVelocity;
                    break;
                }
            case SDLK_RIGHT:
                {
                    cout << "\nRIGHT KEY RELEASED";
                    sVelX -= shipVelocity;
                    break;
                }
            case SDLK_SPACE:
            {
                cout << "\nSPACE RELEASED!";
               /** delete aBullet;
                aBullet = NULL; **/
                break;
            }
        }
    }

}


void mainShip::moveShip()
{
    sPosX += sVelX;
        if ((sPosX < 0) || (sPosX + shipWidth > gameWidth))
        {
             sPosX -= sVelX;
             cout << "\nDAFUQ";

        }

    sPosY += sVelY;
        if ((sPosY < 0) || (sPosY + shipHeight > gameHeight))
            sPosY -= sVelY;

}

void mainShip::render()
{
    shipTexture.render(sPosX,sPosY);
}
