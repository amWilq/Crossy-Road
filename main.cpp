#include <iostream>
#include <fstream>
#include <deque>
#include <vector>
#include <conio.h>
#include <string>
#include <stdlib.h>
#include "time.h"
#include "windows.h"
using namespace std;


void SetConsoleSize(int width, int height)
{
    HWND console = GetConsoleWindow();
    RECT r;
    GetWindowRect(console, &r);
    MoveWindow(console, r.left, r.top, width, height, TRUE);
}
void removeCursor()
{
    CONSOLE_CURSOR_INFO curInfo;
    GetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
    curInfo.bVisible = 0;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &curInfo);
}
void gotoxy(int x, int y)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

    _COORD pos;
    pos.X = x;
    pos.Y = y;

    SetConsoleCursorPosition(hConsole, pos);
}
void TextColor(int x)
{
    HANDLE mau;
    mau = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(mau, x);
}
void WindowInit()
{
    SetConsoleSize(480, 640);
    removeCursor();
}

class TablicaRekordow
{
public:
    string imie[5];
    int wynik[5];
    TablicaRekordow()
    {
        for (int i = 0; i < 5; i++)
        {
            imie[i] = " ";
            wynik[i] = 0;
        }
        string line;
        int j = 0;
        fstream Rekordy;
        Rekordy.open("rekordy.txt", ios::in | ios::out | ios::app);
        Rekordy.seekg(0, ios_base::beg);
        //Rekordy.open("/Users/grzeg/source/repos/ConsoleApplication2/Debug/rekordy.txt", ios::in);
        if (Rekordy.is_open())
        {
            while (getline(Rekordy, line))
            {
                j++;
                if ((j % 2) == 1) imie[(j - 1)/2] = line;
                if ((j % 2) == 0) wynik[(j - 2)/2] = stoi(line);
            }
            Rekordy.close();
        }
        else
        {
            cout << "Blad otwarcia pliku do odczytu";
            Sleep(2000);
        }


    }

    void dodajRekord(string s, int w)
    {
        int temp1, temp2;
        string s1, s2;
        for (int i = 0; i < 5; i++)
        {
            if (w > wynik[i])
            {
                temp1 = wynik[i];
                s1 = imie[i];
                wynik[i] = w;
                imie[i] = s;
                while (i < 4)
                {
                    i++;
                    temp2=wynik[i];
                    s2 = imie[i];
                    wynik[i] = temp1;
                    imie[i] = s1;
                    temp1 = temp2;
                    s1 = s2;
                }
                break;
            }
        }
        string line;
        int j = 0;
        fstream Rekordy;
        Rekordy.open("rekordy.txt", ios::in | ios::out | ios::trunc);
        //Rekordy.open("/Users/grzeg/source/repos/ConsoleApplication2/Debug/rekordy.txt", ios::out | ios::trunc);
        if (Rekordy.is_open())
        {
            while (j < 10)
            {
                j++;
                if ((j % 2) == 1) Rekordy << imie[(j - 1) / 2] << endl;
                if ((j % 2) == 0) Rekordy << wynik[(j - 2) / 2] << endl;
            }
            Rekordy.close();
        }
        else
        {
            cout << "Blad otwarcia pliku do zapisu";
            Sleep(2000);
        }
    }
    friend class cGame;
};

class cPlayer
{
public:
    int x, y;
    cPlayer(int width) { x = width / 2; y = 0; } ///Pozycja startowa samochodu. 2= w połowie pola
};

class cLane
{
private:
    int poziom;
    deque<bool> cars;
    bool right;
public:
    cLane(int width, int p)
    {
        poziom = p;
        for (int i = 0; i < width; i++)
            cars.push_front((rand() % poziom )== 2); ///Dodaje elemend na koniec. Kierunek elementów, w tym przypadku od lewej strony
        right = rand() % 2; /// Generuje randomowo false albo true '01'

    }
    void Move()
    {
        if (right) /// Jeżeli prawda to cała linia przesuwa się od prawej do lewej
        {
            if (rand() % 10 == 1)
                cars.push_front(true);///Przesuwa element na początek
            else
                cars.push_front(false);///Przesuwa element na początek
            cars.pop_back();///Usuwa ostatnii element
        }
        else ///Jeżeli fałsz to cała linia przesuwa się od prawej do lewej
        {
            if (rand() % 10 == 1)
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
    int diff;
    TablicaRekordow* Tab;
    bool quit;
    int IloscLini;
    int Szerokosc;
    int Punkty=0;
    cPlayer* Gracz;
    vector<cLane*> KierunekPrzeszkod;

public:
    cGame(int w, int h, int p)

    /// deklaracja wykosoci i szerowkosci mapy
    {
        diff = p;
        IloscLini = h;
        Szerokosc = w;
        quit = false;
        for (int i = 0; i < IloscLini; i++)
            KierunekPrzeszkod.push_back(new cLane(Szerokosc,p));
        Gracz = new cPlayer(Szerokosc);
        Tab = new TablicaRekordow();
    }
    ~cGame()
    {
        delete Tab;
        delete Gracz;
        for (int i = 0; i < KierunekPrzeszkod.size(); i++)
        {
            cLane* current = KierunekPrzeszkod.back();
            KierunekPrzeszkod.pop_back();
            delete current;
        }
    }


    void ZdobytePunkty()
    {
        TextColor(14);
        gotoxy(Szerokosc / 2 - 8, IloscLini / 2);
        switch (diff)
        {
            case 100:
                Punkty = Punkty + 100;
                cout << "Zdobyles 100 punktow \a "; /// /a wydaje dzwięk po zdobyciu punktów
                break;
            case 20:
                Punkty = Punkty + 200;
                cout << "Zdobyles 200 punktow \a "; /// /a wydaje dzwięk po zdobyciu punktów
                break;
            case 5:
                Punkty = Punkty + 300;
                cout << "Zdobyles 300 punktow \a "; /// /a wydaje dzwięk po zdobyciu punktów
                break;
            case 2:
                Punkty = Punkty + 400;
                cout << "Zdobyles 400 punktow \a "; /// /a wydaje dzwięk po zdobyciu punktów
                break;
        }
        Sleep(1000);
        TextColor(7);
    }


    void Draw()
    {

        system("cls");
        for (int i = 0; i < IloscLini; i++)
        {
            for (int j = 0; j < Szerokosc; j++)
            {
                if (i == 0 && (j == 0 || j == Szerokosc - 1)) cout << "P"; ///Jezeli i nie jest pierwsza linia oraz Linie nie jes trowan ilosci lini(ostatnia Linia)
                if (i == IloscLini - 1 && (j == 0 || j == Szerokosc - 1)) cout << "K";
                if (KierunekPrzeszkod[i]->CheckPos(j) && i != 0 && i != IloscLini - 1)

                    cout << "~";



                else if (Gracz->x == j && Gracz->y == i)

                    cout << "|";
                else
                    cout << " ";
            }

            cout << endl;
        }

        TextColor(15);
        gotoxy(0, IloscLini + 3);
        cout << "autor: Lukasz Pospiech, Grzegorz Suchon" << endl;
        if (Tab->wynik[0] > 0)
        {
            cout << "------------------------------------" << endl;
            cout << "   NAJLEPSZE WYNIKI" << endl;
            cout << "------------------------------------" << endl;
        }
        for (int i = 0; i < 5; i++)
        {
            if (Tab->wynik[i] > 0)
            {
                cout << (i + 1) << ". " << Tab->imie[i] << "   " << Tab->wynik[i] << " pkt" << endl;
            }
        }
        TextColor(0);
        TextColor(10);
        gotoxy((Szerokosc + 5), IloscLini / 2);
        cout << "Twoje punkty: " << Punkty;

    }


    void Input()
    /* _kbhit robi pętlę dopóki użytkownik
    * nie wciśnie odpowieniego klawisza.
    */
    {
        if (_kbhit())
        {
            char current = _getch();
            if (current == 'a' || current == 'A')
            {
                if ((Gracz->x)!=0) Gracz->x--;
            }
            if (current == 'd' || current == 'D')
            {
                if ((Gracz->x) != (Szerokosc - 1)) Gracz->x++;
            }
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
            ZdobytePunkty();
            Gracz->y = 0;
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

        if (Punkty > Tab->wynik[4])
        {
            gotoxy(Szerokosc / 2 - 8, IloscLini / 2);
            TextColor(12);
            cout << "Nowy rekord! Podaj imie: " << endl;
            gotoxy(Szerokosc / 2 - 4, IloscLini / 2 + 1);
            string im;
            cin >> im;

            Tab->dodajRekord(im, Punkty);
            gotoxy(Szerokosc / 2 - 8, IloscLini / 2);
            cout << "                         " << endl;
            gotoxy(Szerokosc / 2 - 4, IloscLini / 2 + 1);
            cout << "                       ";
        }

        gotoxy(Szerokosc / 2 - 4, IloscLini / 2);
        TextColor(12);
        cout << "GAME OVER" << endl;
        Beep(300, 500);

        gotoxy(Szerokosc / 2 - 8, IloscLini / 2 + 1);
        cout << "Jeszcze raz (y/n)? ";
        while (true)
        {
            char current = _getch();
            if (current == 'y')
            {
                cGame game(Szerokosc, IloscLini, diff);
                game.Run();
                break;
            }
            else if (current == 'n')
            {
                gotoxy(1, IloscLini+15);
                TextColor(0);
                exit(0);
            }
        }
    }
};

int main()
{
    int a, b, c;
    cout << "Wybierz wielkosc planszy:" << endl;
    cout << "1. Mala plansza" << endl;
    cout << "2. Srednia plansza" << endl;
    cout << "3. Duza plansza" << endl;
    cout << "4. Bardzo duza plansza" << endl;
    cin >> a;
    switch (a)
    {
        case 1:
            a = 30;
            b = 4;
            break;
        case 2:
            a = 50;
            b = 5;
            break;
        case 3:
            a = 70;
            b = 7;
            break;
        case 4:
            a = 90;
            b = 11;
            break;
        default:
            cout << "Zle wybrales, sprobuj jeszcze raz" << endl;
            Sleep(2000);
            exit(0);
    }

    cout << "Wybierz poziom trudnosci:" << endl;
    cout << "1. Latwy" << endl;
    cout << "2. Sredni" << endl;
    cout << "3. Trudny" << endl;
    cout << "4. Bardzo trudny" << endl;
    cin >> c;
    switch (c)
    {
        case 1:
            c = 100;
            break;
        case 2:
            c = 20;
            break;
        case 3:
            c=5;
            break;
        case 4:
            c = 3;
            break;
        default:
            cout << "Zle wybrales, sprobuj jeszcze raz" << endl;
            Sleep(2000);
            exit(0);
    }


    srand(time(NULL));
    cGame game(a, b, c);
    game.Run();
    //getchar();
    //return 0;
}