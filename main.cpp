#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <fstream>
#include <vector>
#include <sstream>
#include <windows.h>
#include <string>
#include <algorithm>

using namespace std;
//using namespace Windows;

struct Uzytkownik
{
    int idUzytkownika = 0;
    string login = "", haslo = "";
};

struct Adresat
{
    int id = 0, idZalogowanegoUzytkownika = 0;
    string imie="", nazwisko="", numerTelefonu="", mail="", adres="";
};


char wczytajZnak()
{
    string wejscie = "";
    char znak = {0};

    while(true)
    {
        getline(cin, wejscie);

        if(wejscie.length() == 1)
        {
            znak = wejscie[0];
            break;
        }
//        else
//        {
//            cout << "To nie jest pojedynczy znak. Wpisz ponownie: " << endl;
//        }
    }
    return znak;
}

string wczytajLinie()
{
    string wejscie = "";
    cin.sync();
    getline(cin, wejscie);
    return wejscie;
}

int wczytajLiczbeCalkowita()
{
    string wejscie = "";
    int liczba = 0;

    while(true)
    {
        getline(cin, wejscie);

        stringstream myStream(wejscie);
        if(myStream >> liczba)
        {
            break;
        }
        cout << "To nie jest liczba. Wpisz ponownie: " << endl;
    }
    return liczba;
}

string konwersjaIntNaString(int liczba)
{
    ostringstream ss;
    ss << liczba;
    string str = ss.str();

    return str;
}

Uzytkownik odczytajDaneUzytkownikow(string daneUzytkownika)
{
    Uzytkownik danyUzytkownik;
    string pojedynczaDanaUzytkownika = "";
    int numerDanejUzytkownika = 1;

    for(int pozycjaZnaku = 0; pozycjaZnaku < daneUzytkownika.length(); pozycjaZnaku++)
    {
        if(daneUzytkownika[pozycjaZnaku] != '|')
        {
            pojedynczaDanaUzytkownika += daneUzytkownika[pozycjaZnaku];
        }
        else
        {
            switch(numerDanejUzytkownika)
                    {
                    case 1:
                        danyUzytkownik.idUzytkownika = stoi(pojedynczaDanaUzytkownika);
                        break;
                    case 2:
                        danyUzytkownik.login = pojedynczaDanaUzytkownika;
                        break;
                    case 3:
                        danyUzytkownik.haslo = pojedynczaDanaUzytkownika;
                        break;
                    }
                    pojedynczaDanaUzytkownika = "";
                    numerDanejUzytkownika++;
        }
    }

    return danyUzytkownik;
}

void wczytajUzytkownikowZPliku(vector<Uzytkownik> &uzytkownicy)
{
    Uzytkownik danyUzytkownik;
    string daneJednegoUzytkownika = "";

    fstream plik;
    plik.open("Uzytkownicy.txt", ios::in);

    if(plik.good())
    {
        while(getline(plik, daneJednegoUzytkownika))
        {
            danyUzytkownik = odczytajDaneUzytkownikow(daneJednegoUzytkownika);
            uzytkownicy.push_back(danyUzytkownik);
        }
        plik.close();
    }
}

void dopiszUzytkownikaDoPliku(Uzytkownik uzytkownik)
{
    fstream plik;
    plik.open("Uzytkownicy.txt", ios::out | ios::app);

    if(plik.good())
    {
        plik << uzytkownik.idUzytkownika << "|";
        plik << uzytkownik.login << "|";
        plik << uzytkownik.haslo << "|" << endl;
        plik.close();

        cout << "Konto zostalo utworzone.";
    }
    else
    {
        cout << "Plik nie istnieje!" << endl;
        system("pause");
    }
}

void zapiszUzytkownikowDoPliku(vector<Uzytkownik> &uzytkownicy)
{
    fstream plik;
    plik.open("Uzytkownicy.txt", ios::out);

    if(plik.good())
    {
        for(size_t i=0; i < uzytkownicy.size(); i++)
        {
            plik << uzytkownicy[i].idUzytkownika << "|";
            plik << uzytkownicy[i].login << "|";
            plik << uzytkownicy[i].haslo << "|" << endl;
        }
        plik.close();
    }
    else
    {
        cout << "Nie mozna otworzyc pliku Uzytkownicy.txt" << endl;
    }
}

void rejestracja(vector<Uzytkownik> &uzytkownicy)
{
    Uzytkownik uzytkownik;

    system("cls");

    if(uzytkownicy.empty())
    {
        uzytkownik.idUzytkownika = 1;
    }
    else
    {
        uzytkownik.idUzytkownika = uzytkownicy.back().idUzytkownika + 1;
    }

    cout << "Podaj nazwe uzytkownika: ";
    uzytkownik.login = wczytajLinie();

    size_t i=0;
    while(i < uzytkownicy.size())
    {
        if(uzytkownik.login == uzytkownicy[i].login)
        {
            cout << "Taki uzytkownik istnieje. Wpisz inna nazwe uzytkownika." << endl;
            uzytkownik.login = wczytajLinie();
            i = 0;
        }
        else
        {
            i++;
        }
    }

    cout << "Podaj haslo: ";
    uzytkownik.haslo = wczytajLinie();

    uzytkownicy.push_back(uzytkownik);

    dopiszUzytkownikaDoPliku(uzytkownik);
}

int logowanie(vector<Uzytkownik> &uzytkownicy)
{
    string nazwa, haslo;
    cout << "Podaj login: ";
    nazwa = wczytajLinie();

    size_t i=0;
    while(i < uzytkownicy.size())
    {
        if(uzytkownicy[i].login == nazwa)
        {
            for(int proby=0; proby <3; proby++)
            {
                cout << "Podaj haslo. Pozostalo prob " << 3-proby << ": ";
                haslo = wczytajLinie();

                if(uzytkownicy[i].haslo == haslo)
                {
                    cout << endl << "Zalogowales sie." << endl;
                    return uzytkownicy[i].idUzytkownika;
                }
            }
            cout << "Podales 3 razy bledne haslo. Poczekaj 3 sekundy przed kolejna proba." << endl;
            Sleep(3000);
        }
        i++;
    }
    cout << "Nie ma uzytkownika z takim loginem!" << endl;

    return 0;
}

void zmianaHasla(vector<Uzytkownik> &uzytkownicy, int idZalogowanegoUzytkownika)
{
    string haslo;
    cout << "Podaj nowe haslo: ";
    haslo = wczytajLinie();

    for(size_t i=0; i < uzytkownicy.size(); i++)
    {
        if(uzytkownicy[i].idUzytkownika == idZalogowanegoUzytkownika)
        {
            uzytkownicy[i].haslo = haslo;
            cout << "Haslo zostalo zmienione." << endl;
            zapiszUzytkownikowDoPliku(uzytkownicy);
        }
    }
}

Adresat odczytajDaneAdresata(string daneAdresata)
{
    Adresat adresat;
    string pojedynczaDanaAdresata = "";
    int numerDanejAdresata = 1;

    for(int pozycjaZnaku = 0; pozycjaZnaku < daneAdresata.length(); pozycjaZnaku++)
    {
        if(daneAdresata[pozycjaZnaku] != '|')
        {
            pojedynczaDanaAdresata += daneAdresata[pozycjaZnaku];
        }
        else
        {
            switch(numerDanejAdresata)
            {
                case 1:
                        adresat.id = stoi(pojedynczaDanaAdresata);
                        break;
                        case 2:
                        adresat.idZalogowanegoUzytkownika = stoi(pojedynczaDanaAdresata);
                        break;
                        case 3:
                        adresat.imie = pojedynczaDanaAdresata;
                        break;
                        case 4:
                        adresat.nazwisko = pojedynczaDanaAdresata;
                        break;
                        case 5:
                        adresat.numerTelefonu = pojedynczaDanaAdresata;
                        break;
                        case 6:
                        adresat.mail = pojedynczaDanaAdresata;
                        break;
                        case 7:
                        adresat.adres = pojedynczaDanaAdresata;
                        break;
            }
            pojedynczaDanaAdresata = "";
            numerDanejAdresata++;
        }
    }
    return adresat;
}

void wczytajAdresatowZPliku(vector<Adresat> &adresaci)
{
    Adresat adresat;
    string daneJednegoAdresata = "";

    fstream plik;
    plik.open("KsiazkaAdresowa.txt", ios::in);

    if(plik.good())
    {
        while(getline(plik, daneJednegoAdresata))
        {
            adresat = odczytajDaneAdresata(daneJednegoAdresata);
            adresaci.push_back(adresat);
        }
        plik.close();
    }
}

void dopiszAdresataDoPliku(Adresat adresat, int idZalogowanegoUzytkownika)
{
    fstream plik;
    plik.open("KsiazkaAdresowa.txt", ios::out | ios::app);

    if(plik.good())
    {
        plik << adresat.id << "|";
        plik << adresat.idZalogowanegoUzytkownika << "|";
        plik << adresat.imie << "|";
        plik << adresat.nazwisko << "|";
        plik << adresat.numerTelefonu << "|";
        plik << adresat.mail << "|";
        plik << adresat.adres << "|" << endl;
        plik.close();

        cout << endl << "Adresat zostal dodany." << endl;
//        system("pause");
    }
    else
    {
        cout << "Nie udalo sie otworzyc pliku i zapisac w nim danych." << endl;
//        system("pause");
    }
}

void zapiszAdresatowDoPliku(vector<Adresat> &adresaci)
{
    fstream plik;
    plik.open("KsiazkaAdresowa.txt", ios::out);

    if(plik.good())
    {
        for(size_t i=0; i < adresaci.size(); i++)
        {
            plik << konwersjaIntNaString(adresaci[i].id) << "|";
            plik << adresaci[i].idZalogowanegoUzytkownika << "|";
            plik << adresaci[i].imie << "|";
            plik << adresaci[i].nazwisko << "|";
            plik << adresaci[i].numerTelefonu << "|";
            plik << adresaci[i].mail << "|";
            plik << adresaci[i].adres << "|" << endl;
        }
        plik.close();
    }
    else
    {
        cout << "Nie mo¿na otworzyc pliku KsiazkaAdresowa.txt" << endl;
    }
}

void dodajAdresata(vector<Adresat> &adresaci, int idZalogowanegoUzytkownika)
{
    Adresat adresat;
    system("cls");

    if(adresaci.empty())
    {
        adresat.id = 1;
    }
    else
    {
        adresat.id = adresaci.back().id + 1;
    }

    adresat.idZalogowanegoUzytkownika = idZalogowanegoUzytkownika;

    cout << "Podaj imie: ";
    adresat.imie = wczytajLinie();

    cout << "Podaj nazwisko: ";
    adresat.nazwisko = wczytajLinie();

    cout << "Podaj numer telefonu: ";
    adresat.numerTelefonu = wczytajLinie();

    cout << "Podaj mail: ";
    adresat.mail = wczytajLinie();

    cout << "Podaj adres: ";
    adresat.adres = wczytajLinie();

    adresaci.push_back(adresat);

    dopiszAdresataDoPliku(adresat, idZalogowanegoUzytkownika);
}

void wczytajAdresatowDanegoUzytkownika(vector<Adresat> &adresaci, vector<Adresat> &adresaciDanegoUzytkownika, int idZalogowanegoUzytkownika)
{
    system("cls");

    for(size_t i=0; i < adresaci.size(); i++)
    {
        if(adresaci[i].idZalogowanegoUzytkownika == idZalogowanegoUzytkownika)
        {
            adresaciDanegoUzytkownika.push_back(adresaci[i]);
        }
    }
}

void wyswietlKsiazkeAdresowa(vector<Adresat> &adresaciDanegoUzytkownika)
{
    system("cls");

    if(!adresaciDanegoUzytkownika.empty())
    {
        for(size_t i=0; i < adresaciDanegoUzytkownika.size(); i++)
        {
            cout << "Id:                " << adresaciDanegoUzytkownika[i].id << endl;
            cout << "Imie:              " << adresaciDanegoUzytkownika[i].imie << endl;
            cout << "Nazwisko:          " << adresaciDanegoUzytkownika[i].nazwisko << endl;
            cout << "Numer telefonu:    " << adresaciDanegoUzytkownika[i].numerTelefonu << endl;
            cout << "Email:             " << adresaciDanegoUzytkownika[i].mail << endl;
            cout << "Adres:             " << adresaciDanegoUzytkownika[i].adres << endl << endl;
        }
        cout << endl;
    }
    else
    {
        cout << "Ksiazka adresowa uzytkownika jest pusta." << endl;
    }
}

void wyszukajAdresatowPoImieniu(vector<Adresat> &adresaciDanegoUzytkownika)
{
    string imie;
    int iloscPowtorzenImienia = 0;

    system("cls");

    if(!adresaciDanegoUzytkownika.empty())
    {
        cout << "Wyszukaj adresatow o imieniu: ";
        imie = wczytajLinie();

        for(size_t i=0; i < adresaciDanegoUzytkownika.size(); i++)
        {
            if(adresaciDanegoUzytkownika[i].imie == imie)
            {
                cout << "Id:                " << adresaciDanegoUzytkownika[i].id << endl;
                cout << "Imie:              " << adresaciDanegoUzytkownika[i].imie << endl;
                cout << "Nazwisko:          " << adresaciDanegoUzytkownika[i].nazwisko << endl;
                cout << "Numer telefonu:    " << adresaciDanegoUzytkownika[i].numerTelefonu << endl;
                cout << "Email:             " << adresaciDanegoUzytkownika[i].mail << endl;
                cout << "Adres:             " << adresaciDanegoUzytkownika[i].adres << endl << endl;
                iloscPowtorzenImienia++;
            }
        }
        if(iloscPowtorzenImienia == 0)
        {
            cout << "W ksiazce adresowej nie ma adresatow o tym imieniu." << endl;
        }
    }
    else
    {
        cout << "Ksiazka adresowa uzytkownika jest pusta." << endl;
    }
}

void wyszukajAdresatowPoNazwisku(vector<Adresat> &adresaciDanegoUzytkownika)
{
    string nazwisko;
    int iloscPowtorzenNazwiska = 0;

    system("cls");

    if(!adresaciDanegoUzytkownika.empty())
    {
        cout << "Wyszukaj adresatow o nazwisku: ";
        nazwisko = wczytajLinie();

        for(size_t i=0; i < adresaciDanegoUzytkownika.size(); i++)
        {
            if(adresaciDanegoUzytkownika[i].nazwisko == nazwisko)
            {
                cout << "Id:                " << adresaciDanegoUzytkownika[i].id << endl;
                cout << "Imie:              " << adresaciDanegoUzytkownika[i].imie << endl;
                cout << "Nazwisko:          " << adresaciDanegoUzytkownika[i].nazwisko << endl;
                cout << "Numer telefonu:    " << adresaciDanegoUzytkownika[i].numerTelefonu << endl;
                cout << "Email:             " << adresaciDanegoUzytkownika[i].mail << endl;
                cout << "Adres:             " << adresaciDanegoUzytkownika[i].adres << endl << endl;
                iloscPowtorzenNazwiska++;
            }
        }
        if(iloscPowtorzenNazwiska == 0)
        {
            cout << "W ksiazce adresowej nie ma adresatow o tym nazwisku." << endl;
        }
    }
    else
    {
        cout << "Ksiazka adresowa uzytkownika jest pusta." << endl;
    }
}

void przepiszDaneAdresataPoEdycjiLubUsunieciuDoPliku(vector<Adresat> &adresaci)
{
    fstream plikOryginalny, plikTymczasowy;
    plikTymczasowy.open("Adresaci_tymczasowy.txt", ios::out | ios::app);

    if(plikTymczasowy.good())
    {
        for(size_t i=0; i < adresaci.size(); i++)
        {
            plikTymczasowy << konwersjaIntNaString(adresaci[i].id) << "|";
            plikTymczasowy << adresaci[i].idZalogowanegoUzytkownika << "|";
            plikTymczasowy << adresaci[i].imie << "|";
            plikTymczasowy << adresaci[i].nazwisko << "|";
            plikTymczasowy << adresaci[i].numerTelefonu << "|";
            plikTymczasowy << adresaci[i].mail << "|";
            plikTymczasowy << adresaci[i].adres << "|" << endl;
        }

        plikTymczasowy.close();

        remove("KsiazkaAdresowa.txt");
        rename("Adresaci_tymczasowy.txt", "KsiazkaAdresowa.txt");
    }
}

void usunAdresata(vector<Adresat> &adresaciDanegoUzytkownika, vector<Adresat> &adresaci, int idZalogowanegoUzytkownika)
{
    int idAdresataDoUsuniecia = 0;
    char znakPotwierdzenia;
    bool czyAdresatIstnieje = false;

    system("cls");
    if(!adresaciDanegoUzytkownika.empty())
    {
        cout << "Podaj numer id adresata, ktorego dane chcesz usunac:" << endl;
        idAdresataDoUsuniecia = wczytajLiczbeCalkowita();

        for(size_t i=0; i < adresaci.size(); i++)
        {
            if(adresaci[i].idZalogowanegoUzytkownika == idZalogowanegoUzytkownika && adresaci[i].id == idAdresataDoUsuniecia)
            {
                czyAdresatIstnieje = true;
                cout << "Wcisnij 't' na klawiaturze, aby potwierdzic." << endl;
                znakPotwierdzenia = wczytajZnak();

                if(znakPotwierdzenia == 't')
                {
                    adresaci.erase(adresaci.begin() + i);
                    cout << "Usunieto adresata." << endl;
                    przepiszDaneAdresataPoEdycjiLubUsunieciuDoPliku(adresaci);
                    break;
                }
            }
        }
        if(!czyAdresatIstnieje)
        {
            cout << "W ksiazce adresowej nie ma takiego adresata." << endl;
        }
    }
    else
    {
        cout << "Ksiazka adresowa jest pusta." << endl;
    }
}

int modyfikujDaneAdresata(vector<Adresat> &adresaciDanegoUzytkownika, vector<Adresat> &adresaci, int idZalogowanegoUzytkownika)
{
    int idAdresataDoModyfikacji = 0;
    char znakPotwierdzenia;
    bool czyAdresatIstnieje = false;
    char wybor;

    system("cls");
    if(!adresaciDanegoUzytkownika.empty())
    {
        cout << "Podaj numer id adresata, ktorego dane chcesz zmodyfikowac:" << endl;
        idAdresataDoModyfikacji = wczytajLiczbeCalkowita();

        for(size_t i=0; i < adresaci.size(); i++)
        {
            if(adresaci[i].idZalogowanegoUzytkownika == idZalogowanegoUzytkownika && adresaci[i].id == idAdresataDoModyfikacji)
            {
                czyAdresatIstnieje = true;

                cout << "Podaj, ktore dane chcesz zmodyfikowac:" << endl;
                cout << "1. Imie" << endl;
                cout << "2. Nazwisko" << endl;
                cout << "3. Numer telefonu" << endl;
                cout << "4. Email" << endl;
                cout << "5. Adres" << endl;
                cout << "6. Powrot do menu glownego" << endl << endl;
                cout << "Twoj wybor: " << endl;
                wybor = wczytajZnak();

                switch(wybor)
                {
                case '1':
                    cout << "Dotychczasowe imie: " << adresaci[i].imie << endl;
                    cout << "Podaj nowe imie: ";
                    adresaci[i].imie = wczytajLinie();
                    cout << endl << "Imie zostalo zmienione." << endl;
                    break;
                case '2':
                    cout << "Dotychczasowe nazwisko: " << adresaci[i].nazwisko << endl;
                    cout << "Podaj nowe nazwisko: ";
                    adresaci[i].nazwisko = wczytajLinie();
                    cout << endl << "Nazwisko zostalo zmienione." << endl;
                    break;
                case '3':
                    cout << "Dotychczasowy numer telefonu: " << adresaci[i].numerTelefonu << endl;
                    cout << "Podaj nowy numer telefonu: ";
                    adresaci[i].numerTelefonu = wczytajLinie();
                    cout << endl << "Numer telefonu zostal zmieniony." << endl;
                    break;
                case '4':
                    cout << "Dotychczasowy email: " << adresaci[i].mail << endl;
                    cout << "Podaj nowy email: ";
                    adresaci[i].mail = wczytajLinie();
                    cout << endl << "Email zostal zmieniony." << endl;
                    break;
                case '5':
                    cout << "Dotychczasowy adres: " << adresaci[i].adres << endl;
                    cout << "Podaj nowy adres: ";
                    adresaci[i].adres = wczytajLinie();
                    cout << endl << "Adres zostal zmieniony." << endl;
                    break;
                case '6':
                    return 0;
                }
                przepiszDaneAdresataPoEdycjiLubUsunieciuDoPliku(adresaci);
            }
        }
        if(!czyAdresatIstnieje)
        {
            cout << "W ksiazce adresowej nie ma takiego adresata." << endl;
        }
    }
    else
    {
        cout << "Ksiazka adresowa jest pusta." << endl;
    }
}


int main()
{
    vector <Uzytkownik> uzytkownicy;
    vector <Adresat> adresaci;
    vector<Adresat> adresaciDanegoUzytkownika;
    int idZalogowanegoUzytkownika = 0;
    int iloscUzytkownikow = 0;

    wczytajUzytkownikowZPliku(uzytkownicy);
    wczytajAdresatowZPliku(adresaci);

    char wybor;

    while(true)
    {
        if(idZalogowanegoUzytkownika == 0)
        {
            system("cls");
            cout << "MENU GLOWNE" << endl;
            cout << "------------------------------" << endl;
            cout << "1. Rejestracja" << endl;
            cout << "2. Logowanie" << endl;
            cout << "9. Zakoncz program" << endl;
            cout << "------------------------------" << endl;
            cout << "Twoj wybor: " << endl;
            wybor = wczytajZnak();

            switch(wybor)
            {
            case '1':
                rejestracja(uzytkownicy);
                system("pause");
                break;
            case '2':
                idZalogowanegoUzytkownika = logowanie(uzytkownicy);
                system("pause");
                break;
            case '9':
                exit(0);
            }
        }
        else
        {
            wczytajAdresatowDanegoUzytkownika(adresaci, adresaciDanegoUzytkownika, idZalogowanegoUzytkownika);

            system("cls");
            cout << "MENU UZYTKOWNIKA" << endl;
            cout << "------------------------------" << endl;
            cout << "1. Dodaj adresata" << endl;
            cout << "2. Wyszukaj po imieniu" << endl;
            cout << "3. Wyszukaj po nazwisku" << endl;
            cout << "4. Wyswietl cala ksiazke" << endl;
            cout << "5. Usun adresata" << endl;
            cout << "6. Edytuj adresata" << endl;
            cout << "------------------------------" << endl;
            cout << "7. Zmien haslo" << endl;
            cout << "8. Wyloguj sie" << endl;
            cout << "------------------------------" << endl;
            cout << "9. Zakoncz prace" << endl << endl;
            cout << "Twoj wybor: ";
            wybor = wczytajZnak();

            switch(wybor)
            {
            case '1':
                dodajAdresata(adresaci, idZalogowanegoUzytkownika);
                system("pause");
                break;
            case '2':
                wyszukajAdresatowPoImieniu(adresaciDanegoUzytkownika);
                system("pause");
                break;
            case '3':
                wyszukajAdresatowPoNazwisku(adresaciDanegoUzytkownika);
                system("pause");
                break;
            case '4' :
                wyswietlKsiazkeAdresowa(adresaciDanegoUzytkownika);
                system("pause");
                break;
            case '5' :
                usunAdresata(adresaciDanegoUzytkownika, adresaci, idZalogowanegoUzytkownika);
                system("pause");
                break;
            case '6' :
                modyfikujDaneAdresata(adresaciDanegoUzytkownika, adresaci, idZalogowanegoUzytkownika);
                system("pause");
                break;
            case '7' :
                zmianaHasla(uzytkownicy, idZalogowanegoUzytkownika);
                system("pause");
                break;
            case '8' :
                idZalogowanegoUzytkownika = 0;
                adresaciDanegoUzytkownika.clear();
                system("pause");
                break;
            case '9':
                exit(0);
            }
        }
    }

    return 0;
}
