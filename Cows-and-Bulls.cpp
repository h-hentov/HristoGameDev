/*************************************************************
* Created by: Hristo Hentov                                  *
* Date: 05/07/2014                                           *
* Project: Cows and Bulls                                    *
**************************************************************/

#include <iostream>
#include <time.h>
#include <stdlib.h>

using namespace std;

void gameStart();
void gameEngine();
bool checknum(short[], short);
short * setArray(short);
short generateNum();

int main()
{
    gameStart();
    gameEngine();

    return 0;
}
void gameStart()
{
    cout << "ЙННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННН»";
    cout << "\nє                                                                             є";
    cout << "\nє                                 Bulls and Cows                              є";
    cout << "\nє                                                                             є";
    cout << "\nИНННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННННј";
    cout << "\n\t\t\t\t  --==RULES==-- ";
    cout << "\n\n\nThe game of Bulls and Cows is simple. \nYou and your opponent each choose a non-repetitive 4 digit number.";
    cout << "\nYou then take turns guessing each other's number. To guess the number you are \nrequired to enter a 4 digit number, that has no 2 digits that are the same.";
    cout << "\n\t-If you guess a number, it counts as a COW. \n\t-If you guess a number AND it's possition it's a BULL.";
    cout << "\n\nThe first one to guess his opponent's number wins.";
    cout << "\n\nHave fun and ";
    system("pause");
    system("CLS");
    return;
}

void gameEngine()
{
    short usernum, uGuess;
    bool valid = false;

    do
    {
        cout << "Please choose your secret number: ";
        cin >> usernum;

        if(checknum(setArray(usernum), 4))
                {
                    cout << "\nInvalid number! Please try again.\n";
                    valid = false;
                }

                else if(!checknum(setArray(usernum), 4))
                    valid = true;

    }
    while(!valid);

    short * usPtr = setArray(usernum);

    srand(time(NULL));
    short AINum = generateNum();
    short * aiPtr = setArray(AINum);

    valid = false;

do
    {
        short bulls = 0,bbulls = 0,cows = 0 ,bcows = 0;

         cout << "\n\nYour turn. Enter your guess: ";
         cin >> uGuess;

         short * aPtr = setArray(uGuess);

         for(int i = 0; i < 5; i++)
         {
            if(aPtr[i] == aiPtr[i])
                bulls++;

            for (int c = 0; c < 4; c++)
            {
                if(aPtr[i] == aiPtr[c])
                    cows++;
            }

         }

    cout << "\nYou have: " << bulls << " bulls and " << cows << " cows.";

    short AIGuess = generateNum();
    short * bPtr = setArray(AIGuess);

            for(int i = 0; i < 5; i++)
         {
            if(bPtr[i] == usPtr[i])
                bbulls++;

            for (int c = 0; c < 4; c++)
            {
                if(bPtr[i] == usPtr[c])
                    bcows++;
            }

         }

    cout << "\nComputer has " << bbulls << " bulls and " << bcows << " cows.";

    if(bulls == 4)
    {
        cout << "YOU WIN!";
        valid = true;
    }

    else if(bbulls == 4)
    {
        cout << "COMPUTER WINS!";
        valid = true;
    }

    }while(!valid);

}

bool checknum(short cNum[], short len)
{
    short c, temp;
    for(short i = 1; i < len; i++)
    {
        c = i;
        while(c > 0 && cNum[c] < cNum[c-1])
        {
            temp = cNum[c];
            cNum[c] = cNum[c-1];
            cNum[c-1] = temp;
            c--;
        }
    }


    for(short i = 0; i < len-1; i++)
    {
        if( cNum[i] == cNum[i+1] )
            return true;
    }
    return false;


}

short * setArray(short num)
{
        short unumArray[4];
        unumArray[0] = num / 1000;
        unumArray[1] = (num % 1000) / 100;
        unumArray[2] = ((num % 1000) % 100) / 10;
        unumArray[3] = ((num % 1000) % 100) % 10;
        short * arrPtr;
        arrPtr = new short[4];
        for (int i = 0; i < 4; i++)
            arrPtr[i] = unumArray[i];
        return arrPtr;
}

short generateNum()
{
    short CurNum = rand() % 9876 + 1023;
    while(checknum(setArray(CurNum), 4))
        CurNum = rand() % 9876 + 1023;
    return CurNum;
}
