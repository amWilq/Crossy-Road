#include <iostream>
#include <deque>
#include <vector>
#include <conio.h>
#include <time.h>
using namespace std;
class cPlayer
{
public:
    int x, y;
    cPlayer(int width) { x = width / 2; y = 0; } ///Pozycja startowa samochodu. 2= w połowie pola
};
class cLane
{
private:
    deque<bool> cars;
    bool right;
public:
    cLane(int width)
    {
        for (int i = 0; i < width; i++)
            cars.push_back(false); ///Dodaje elemend na koniec. Kierunek elementów, w tym przypadku od lewej strony
        right = rand() % 2; /// Generuje randomowo false albo true '01'

    }
    void Move()
    {
        if (right) /// Jeżeli prawda to cała linia przesuwa się od prawej do lewej
        {
            if (rand() % 2 == 1)
                cars.push_front(true);///Przesuwa element na początek
            else
                cars.push_front(false);///Przesuwa element na początek
            cars.pop_back();///Usuwa ostatnii element
        }
        else ///Jeżeli fałsz to cała linia przesuwa się od prawej do lewej
        {
            if (rand() % 2 == 1)
                cars.push_back(true);///Przesuwa element na koniec
            else
                cars.push_back(false);///Przesuwa element na koniec
            cars.pop_front();///Usuwa pierwszy element
        }

    }
    bool CheckPos(int pos) { return cars[pos]; }
    void ChangeDirection() { right = !right; }
};
class cGame
{
private:
    bool quit;
    int IloscLini;
    int Szerokosc;
    int Punkty;
    cPlayer * Gracz;
    vector<cLane*> KierunekPrzeszkod;
public:
    cGame(int w, int h) /// deklaracja wykosoci i szerowkosci mapy
    {
        IloscLini = h;
        Szerokosc = w;
        quit = false;
        for (int i = 0; i < IloscLini; i++)
            KierunekPrzeszkod.push_back(new cLane(Szerokosc));
        Gracz = new cPlayer(Szerokosc);
    }
    ~cGame()
    {
        delete Gracz;
        for (int i = 0; i < KierunekPrzeszkod.size(); i++)
        {
            cLane * current = KierunekPrzeszkod.back();
            KierunekPrzeszkod.pop_back();
            delete current;
        }
    }
    void Draw()
    {
        system("cls");
        for (int i = 0; i < IloscLini; i++)
        {
            for (int j = 0; j < Szerokosc; j++)
            {
                if (i == 0 && (j == 0 || j == Szerokosc - 1)) cout << "S"; ///Jezeli i nie jest pierwsza linia oraz Linie nie jes trowan ilosci lini(ostatnia Linia)
                if (i == IloscLini - 1 && (j == 0 || j == Szerokosc - 1)) cout << "F";
                if (KierunekPrzeszkod[i]->CheckPos(j) && i != 0 && i != IloscLini - 1)
                    cout << "-";
                else if (Gracz->x == j && Gracz->y == i)
                    cout << "V";
                else
                    cout << " ";
            }
            cout << endl;
        }

        cout << "Score: " << Punkty << endl;
        cout << "Sterowanie na WSAD" << endl;
    }
    void Input()
    {
        if (_kbhit())
        {
            char current = _getch();
            if (current == 'a' || current == 'A')
                Gracz->x--;
            if (current == 'd' || current == 'D')
                Gracz->x++;
            if (current == 'w' || current == 'W')
                Gracz->y--;
            if (current == 's' || current == 'S')
                Gracz->y++;
            if (current == 'q' || current == 'Q')
                quit = true;
        }
    }
    void Logic()
    {
        for (int i = 1; i < IloscLini - 1; i++)
        {
            if (rand() % 10 == 1)
                KierunekPrzeszkod[i]->Move();
            if (KierunekPrzeszkod[i]->CheckPos(Gracz->x) && Gracz->y == i)
                quit = true;
        }
        if (Gracz->y == IloscLini - 1)
        {
            Punkty++;
            Gracz->y = 0;
            cout << "\x07";
            KierunekPrzeszkod[rand() % IloscLini]->ChangeDirection();
        }
    }
    void Run()
    {
        while (!quit)
        {
            Input();
            Draw();
            Logic();
        }
    }
};
int main()
{
    srand(time(NULL));
    cGame game(30, 5);
    game.Run();
    cout << "Game over!" << endl;
    getchar();
    return 0;
}
