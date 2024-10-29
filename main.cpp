#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <windows.h>

using namespace std;

struct Adresat
{
    int id = 0;
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

int wczytajIloscOsob()
{
    fstream plik;
    plik.open("KsiazkaAdresowa.txt", ios::in);

    if(plik.good() == false)
    {
        cout << "Plik nie istnieje!" << endl;
        system("pause");
    }

    string linia;
    int licznik = 0;

    while(getline(plik, linia))
    {
        licznik++;
    }

    plik.close();

    int iloscOsob = licznik;

    return iloscOsob;
}

void odczytajDaneAdresatow(vector <Adresat> &adresaci)
{
    Adresat danaOsoba;
    string daneOsob;

    fstream plik;
    plik.open("KsiazkaAdresowa.txt", ios::in);

    adresaci.clear();

    if(plik.good())
    {
        while(getline(plik, daneOsob))
        {
            string daneKonkretnejOsoby;
            int idOsoby = 1;

            for(size_t i{}; i < daneOsob.length(); ++i)
            {
                if(daneOsob[i] != '|')
                {
                    daneKonkretnejOsoby += daneOsob[i];
                }
                else
                {
                    switch(idOsoby)
                    {
                    case 1:
                        danaOsoba.id = stoi(daneKonkretnejOsoby);
                        break;
                    case 2:
                        danaOsoba.imie = daneKonkretnejOsoby;
                        break;
                    case 3:
                        danaOsoba.nazwisko = daneKonkretnejOsoby;
                        break;
                    case 4:
                        danaOsoba.numerTelefonu = daneKonkretnejOsoby;
                        break;
                    case 5:
                        danaOsoba.mail = daneKonkretnejOsoby;
                        break;
                    case 6:
                        danaOsoba.adres = daneKonkretnejOsoby;
                        break;
                    }
                    daneKonkretnejOsoby.clear();
                    idOsoby++;
                }

            }
            adresaci.push_back(danaOsoba);
            daneOsob.clear();
        }
    }
    plik.close();
}

int wczytajIdOstatniegoAdresata(vector <Adresat> adresaci)
{
    int idOstatniegoAdresata, rozmiarWektora;

    odczytajDaneAdresatow(adresaci);

    if(adresaci.size() == 0)
    {
        idOstatniegoAdresata = 0;
    }
    else
    {
        rozmiarWektora = adresaci.size()-1;

        idOstatniegoAdresata = adresaci[rozmiarWektora].id;
    }


    return idOstatniegoAdresata;
}

void zapiszAdresataDoPliku(vector <Adresat> &chwilowyAdresatDoZapisu)
{
    string imie, nazwisko, numerTelefonu, mail, adres;

    fstream plik;
    plik.open("KsiazkaAdresowa.txt", ios::out | ios::app);

    if(plik.good())
    {
        int iloscOsob = chwilowyAdresatDoZapisu.size();
        for(int i=0; i < iloscOsob; ++i)
        {
            plik << chwilowyAdresatDoZapisu[i].id << "|";
            plik << chwilowyAdresatDoZapisu[i].imie << "|";
            plik << chwilowyAdresatDoZapisu[i].nazwisko << "|";
            plik << chwilowyAdresatDoZapisu[i].numerTelefonu << "|";
            plik << chwilowyAdresatDoZapisu[i].mail << "|";
            plik << chwilowyAdresatDoZapisu[i].adres << "|" << endl;
        }
    }
    else
    {
        cout << "Plik nie istnieje!" << endl;
        system("pause");
    }
    plik.close();
}

int pobierzDaneOdAdresata(int iloscOsob, vector <Adresat> adresaci)
{
    Adresat nowyAdresat;
    string imie, nazwisko, numerTelefonu, mail, adres;
    vector <Adresat> chwilowyAdresatDoZapisu;

    cout << "Podaj imie: ";
    nowyAdresat.imie = wczytajLinie();
    cout << "Podaj nazwisko: ";
    nowyAdresat.nazwisko = wczytajLinie();
    cout << "Podaj numer telefonu: ";
    nowyAdresat.numerTelefonu = wczytajLinie();
    cout << "Podaj email: ";
    nowyAdresat.mail = wczytajLinie();
    cout << "Podaj adres: ";
    nowyAdresat.adres = wczytajLinie();

    nowyAdresat.id = wczytajIdOstatniegoAdresata(adresaci) + 1;
    chwilowyAdresatDoZapisu.push_back(nowyAdresat);

    zapiszAdresataDoPliku(chwilowyAdresatDoZapisu);

    chwilowyAdresatDoZapisu.clear();

    cout << endl << "Dodano osobe do ksiazki adresowej." << endl;

    return iloscOsob + 1;
}

bool czyPlikIstnieje(const string &nazwaPliku)
{
    fstream plik;
    plik.open(nazwaPliku.c_str(), ios::in);

    if(plik.is_open())
    {
        plik.close();
        return true;
    }
    plik.close();
    return false;
}

void wyswietlKsiazkeAdresowa(vector <Adresat> adresaci)
{
    odczytajDaneAdresatow(adresaci);

    if(adresaci.size() == 0)
    {
        cout << "Nie dodano jeszcze zadnego adresata!" << endl;
        Sleep(2000);
    }
    else
    {
        for(size_t i{}; i < adresaci.size(); i++)
        {
            cout << endl;
            cout << adresaci[i].id << endl;
            cout << adresaci[i].imie << endl;
            cout << adresaci[i].nazwisko << endl;
            cout << adresaci[i].numerTelefonu << endl;
            cout << adresaci[i].mail << endl;
            cout << adresaci[i].adres << endl;
        }
    }
}

void wyszukajAdresatowPoImieniu(vector <Adresat> adresaci)
{
    string imie;
    cout << "Podaj imie osoby, jaka mamy wyszukac: ";
    cin >> imie;

    odczytajDaneAdresatow(adresaci);

    size_t i{};
    int iloscPowtorzenImienia = 0;

    while(i < adresaci.size())
    {
        if(adresaci[i].imie == imie)
        {
            cout << endl << adresaci[i].id << endl;
            cout << "Imie: " << adresaci[i].imie << endl;
            cout << "Nazwisko: " << adresaci[i].nazwisko << endl;
            cout << "Numer telefonu: " << adresaci[i].numerTelefonu << endl;
            cout << "Email: " << adresaci[i].mail << endl;
            cout << "Adres: " << adresaci[i].adres << endl << endl;
            iloscPowtorzenImienia++;
        }
        i++;
    }
    if(iloscPowtorzenImienia == 0)
    {
        cout << "W ksiazce adresowej nie ma takich osob." << endl;
    }
}

void wyszukajAdresatowPoNazwisku(vector <Adresat> adresaci)
{
    string nazwisko;
    cout << "Podaj nazwisko osoby, jaka mamy wyszukac: ";
    cin >> nazwisko;

    odczytajDaneAdresatow(adresaci);

    size_t i{};
    int iloscPowtorzenNazwiska = 0;

    while(i < adresaci.size())
    {
        if(adresaci[i].nazwisko == nazwisko)
        {
            cout << endl << adresaci[i].id << endl;
            cout << "Imie: " << adresaci[i].imie << endl;
            cout << "Nazwisko: " << adresaci[i].nazwisko << endl;
            cout << "Numer telefonu: " << adresaci[i].numerTelefonu << endl;
            cout << "Email: " << adresaci[i].mail << endl;
            cout << "Adres: " << adresaci[i].adres << endl << endl;
            iloscPowtorzenNazwiska++;
        }
        i++;
    }
    if(iloscPowtorzenNazwiska == 0)
    {
        cout << "W ksiazce adresowej nie ma takich osob." << endl;
    }
}

void wyczyscPlik()
{
    ofstream plik;
    plik.open("KsiazkaAdresowa.txt", ofstream::out | ofstream::trunc);
    plik.close();
}

bool czyAdresatOPodanymIdIstnieje(vector <Adresat> adresaci, int idAdresata)
{
    for(size_t i{}; i < adresaci.size(); i++)
    {
        if(adresaci[i].id == idAdresata)
        {
            return true;
        }
    }
    return false;
}

int naJakiejPozycjiWWektorzeZnajdujeSieAdresat(vector <Adresat> adresaci, int numerId)
{
    int szukanaPozycja;

    for(size_t i{}; i < adresaci.size(); i++)
    {
        if(adresaci[i].id == numerId)
        {
            szukanaPozycja = i;
        }
    }
    return szukanaPozycja;
}

void usunAdresata(vector <Adresat> &adresaci)
{
    int idAdresataDoUsuniecia, szukanaPozycja;
    char znakPotwierdzenia;

    cout << "Podaj numer id adresata, ktorego dane chcesz usunac:" << endl;
    idAdresataDoUsuniecia = wczytajLiczbeCalkowita();

    odczytajDaneAdresatow(adresaci);

    if(czyAdresatOPodanymIdIstnieje(adresaci, idAdresataDoUsuniecia) == false)
    {
        cout << "Adresat o podanym numerze id nie istnieje!" << endl;
    }
    else
    {
        for(size_t i{}; i < adresaci.size(); i++)
        {
            if(adresaci[i].id == idAdresataDoUsuniecia)
            {
                szukanaPozycja = naJakiejPozycjiWWektorzeZnajdujeSieAdresat(adresaci, idAdresataDoUsuniecia);
//            cout << adresaci[i].id << ". " << adresaci[i].imie << ' ' << adresaci[i].nazwisko << endl;
//            cout << "Szukana pozycja: " << szukanaPozycja << endl;

                cout << "Adresat do usuniecia: " << adresaci[i].imie << ' ' << adresaci[i].nazwisko << endl;
                cout << "Wcisnij 't' na klawiaturze, aby potwierdzic." << endl;
                znakPotwierdzenia = wczytajZnak();

                if(znakPotwierdzenia == 't')
                {
                    adresaci.erase(adresaci.begin() + szukanaPozycja);
                    wyczyscPlik();
                    zapiszAdresataDoPliku(adresaci);
                    cout << "Usunieto adresata." << endl;
                    break;
                }
            }
        }
    }
}

void modyfikujImieAdresata(vector <Adresat> &adresaci, int idAdresata)
{
    string imie;
    odczytajDaneAdresatow(adresaci);

    cout << "Dotychczasowe imie: " << adresaci[idAdresata-1].imie << endl;
    cout << "Podaj nowe imie:" << endl;
    imie = wczytajLinie();

    for(size_t i{}; i < adresaci.size(); i++)
    {
        if(adresaci[i].id == idAdresata)
        {
            adresaci[i].imie = imie;
            wyczyscPlik();
            zapiszAdresataDoPliku(adresaci);
            cout << "Zapisano nowe imie." << endl;
        }
    }
}

void modyfikujNazwiskoAdresata(vector <Adresat> &adresaci, int idAdresata)
{
    string nazwisko;
    odczytajDaneAdresatow(adresaci);

    cout << "Dotychczasowe nazwisko: " << adresaci[idAdresata-1].nazwisko << endl;
    cout << "Podaj nowe nazwisko:" << endl;
    nazwisko = wczytajLinie();

    for(size_t i{}; i < adresaci.size(); i++)
    {
        if(adresaci[i].id == idAdresata)
        {
            adresaci[i].nazwisko = nazwisko;
            wyczyscPlik();
            zapiszAdresataDoPliku(adresaci);
            cout << "Zapisano nowe nazwisko." << endl;
        }
    }
}

void modyfikujNumerTelefonuAdresata(vector <Adresat> &adresaci, int idAdresata)
{
    string numerTelefonu;
    odczytajDaneAdresatow(adresaci);

    cout << "Dotychczasowy numer telefonu: " << adresaci[idAdresata-1].numerTelefonu << endl;
    cout << "Podaj nowy numer telefonu:" << endl;
    numerTelefonu = wczytajLinie();

    for(size_t i{}; i < adresaci.size(); i++)
    {
        if(adresaci[i].id == idAdresata)
        {
            adresaci[i].numerTelefonu = numerTelefonu;
            wyczyscPlik();
            zapiszAdresataDoPliku(adresaci);
            cout << "Zapisano nowy numer telefonu." << endl;
        }
    }
}

void modyfikujMailAdresata(vector <Adresat> &adresaci, int idAdresata)
{
    string mail;
    odczytajDaneAdresatow(adresaci);

    cout << "Dotychczasowy mail: " << adresaci[idAdresata-1].mail << endl;
    cout << "Podaj nowy mail:" << endl;
    mail = wczytajLinie();

    for(size_t i{}; i < adresaci.size(); i++)
    {
        if(adresaci[i].id == idAdresata)
        {
            adresaci[i].mail = mail;
            wyczyscPlik();
            zapiszAdresataDoPliku(adresaci);
            cout << "Zapisano nowy mail." << endl;
        }
    }
}

void modyfikujAdresAdresata(vector <Adresat> &adresaci, int idAdresata)
{
    string adres;
    odczytajDaneAdresatow(adresaci);

    cout << "Dotychczasowy adres: " << adresaci[idAdresata-1].adres << endl;
    cout << "Podaj nowy adres:" << endl;
    adres = wczytajLinie();

    for(size_t i{}; i < adresaci.size(); i++)
    {
        if(adresaci[i].id == idAdresata)
        {
            adresaci[i].adres = adres;
            wyczyscPlik();
            zapiszAdresataDoPliku(adresaci);
            cout << "Zapisano nowy adres." << endl;
        }
    }
}

int modyfikujDaneAdresata(vector <Adresat> &adresaci)
{
    Adresat danaOsoba;
    char wybor;
    int idAdresata;

    odczytajDaneAdresatow(adresaci);

    cout << "Podaj numer id adresata, ktorego dane chcesz zmodyfikowac:" << endl;
    idAdresata = wczytajLiczbeCalkowita();

    if(czyAdresatOPodanymIdIstnieje(adresaci, idAdresata) == false)
    {
        cout << "Adresat o podanym numerze id nie istnieje!" << endl;
    }
    else
    {
        while(true)
        {
            cout << "Podaj, ktore dane chcesz zmodyfikowac:" << endl;
            cout << "1. Imie" << endl;
            cout << "2. Nazwisko" << endl;
            cout << "3. Numer telefonu" << endl;
            cout << "4. Email" << endl;
            cout << "5. Adres" << endl;
            cout << "6. Powrot do menu glownego" << endl;
            wybor = wczytajZnak();

            switch(wybor)
            {
            case '1':
                modyfikujImieAdresata(adresaci, idAdresata);
                break;
            case '2':
                modyfikujNazwiskoAdresata(adresaci, idAdresata);
                break;
            case '3':
                modyfikujNumerTelefonuAdresata(adresaci, idAdresata);
                break;
            case '4':
                modyfikujMailAdresata(adresaci, idAdresata);
                break;
            case '5':
                modyfikujAdresAdresata(adresaci, idAdresata);
                break;
            case '6':
                return 0;
            }
        }
    }
}

int main()
{
    vector <Adresat> adresaci;
    char wybor;
    int iloscOsob = 0;
    iloscOsob = wczytajIloscOsob();

    while(true)
    {
        system("cls");
        cout << "KSIAZKA ADRESOWA" << endl;
        cout << "1. Dodaj dane adresata" << endl;
        cout << "2. Wyszukaj po imieniu" << endl;
        cout << "3. Wyszukaj po nazwisku" << endl;
        cout << "4. Wyswietl cala ksiazke" << endl;
        cout << "5. Usun adresata" << endl;
        cout << "6. Edytuj adresata" << endl;
        cout << "9. Zakoncz prace" << endl << endl;
        cout << "Twoj wybor: ";
        wybor = wczytajZnak();

        switch(wybor)
        {
        case '1':
            iloscOsob = pobierzDaneOdAdresata(iloscOsob, adresaci);
            system("pause");
            break;
        case '2':
            wyszukajAdresatowPoImieniu(adresaci);
            system("pause");
            break;
        case '3':
            wyszukajAdresatowPoNazwisku(adresaci);
            system("pause");
            break;
        case '4' :
            wyswietlKsiazkeAdresowa(adresaci);
            system("pause");
            break;
        case '5' :
            usunAdresata(adresaci);
            system("pause");
            break;
        case '6' :
            modyfikujDaneAdresata(adresaci);
            system("pause");
            break;
        case '9':
            exit(0);
        }
    }

    return 0;
}
