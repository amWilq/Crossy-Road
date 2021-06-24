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

class a
{
public:
    int s;
    void SetS() {
        cin >> s;
        cout << s << endl;}
    int GetS() const { return s; }


};
class A {
public:
    int num1,num2;
    void start()
    {
        system("cls");
        gotoxy(5,15);
        TextColor(12);

        cout << "Podaj rozmiar planszy, najpierw szerokosci potem wysokośc";
        cin >> num1 >> num2;


    }
};

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
    deque<bool> cars;
    bool right;
public:
    cLane(int width)
    {

        a a1;
        a1.SetS();
        int s1=a1.GetS();

        for (int i = 0; i < width; i++)
            cars.push_front(rand() % s1 == 2); ///Dodaje elemend na koniec. Kierunek elementów, w tym przypadku od lewej strony
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
    TablicaRekordow* Tab;
    bool quit;
    int IloscLini;
    int Szerokosc;
    int Punkty=0;
    cPlayer* Gracz;
    vector<cLane*> KierunekPrzeszkod;

public:
    cGame(int w, int h)

        /// deklaracja wykosoci i szerowkosci mapy
    {

        IloscLini = h;
        Szerokosc = w;
        quit = false;
        for (int i = 0; i < IloscLini; i++)
            KierunekPrzeszkod.push_back(new cLane(Szerokosc));
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

    int num1;
    void start()
    {
        TextColor(14);

        cout << " \n";
        cout << "Plansza nr 1: Mała\n";
        cout << "Plansza nr 2: Średnia\n";
        cout << "Plansza nr 3: Duża\n";
        cout << "Plansza nr 4: Długa\n";

        cin >> num1;


        switch (num1) {

            case 1:
            {
                cGame game (50,4);
                game.Run();
                break;
            }
            case 2:
            {
                cGame game (80,5);
                game.Run();
                break;
            }
            case 3:
            {
                cGame game (80,10);
                game.Run();
                break;
            }
            case 4:
            {
                cGame game (80,15);
                game.Run();
                break;
            }
            default:
                system("cls");
                gotoxy(5,15);
                TextColor(12);
                cout << "Zle wybrales, sporbuj jeszcze raz";
                Sleep(5000);
                exit(0);
        }


        system("cls");
        gotoxy(5,15);
        TextColor(12);




    }



    void ZdobytePunkty()
    {
        Punkty += 100;
        TextColor(14);
        gotoxy(Szerokosc / 2 - 8, IloscLini / 2);
        cout << "Zdobyles 100 punktow \a "; /// /a wydaje dzwięk po zdobyciu punktów
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
        cout << "autor: Lukasz, Grzegorz" << endl;
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
                cGame game(30, 4);
                game.Run();
                break;
            }
            else if (current == 'n')
            {
                exit(0);
            }
        }
    }
};

int main()
{
    srand(time(NULL));
    ///A obj;
    ///obj.start();
    cGame game(0,0);
    game.start();
    getchar();
    return 0;
}