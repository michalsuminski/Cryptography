#include <string>
#include <boost/multiprecision/miller_rabin.hpp>
#include <boost/random/mersenne_twister.hpp>
#include <vector>
#include <fstream>
#include <stdio.h>
#include <boost/container_hash/hash.hpp>

using namespace std;
using namespace boost::multiprecision;
using namespace boost::random;

cpp_int GetPrime() {
    mt11213b base_gen(clock());
//    adaptuje generator liczb pseudo-losowych jakim jest mt11213b i produkuje losową liczbe 512 bitową, cpp_int jest dowolnego rozmiaru bitowego(moze byc nawet 1024)
    independent_bits_engine<mt11213b, 512, cpp_int> gen(base_gen);

    cpp_int n;
    do {
        n = gen();
    } while (!miller_rabin_test(n,
                                25)); // jesli true to jest probablePrime, "...recommends 25 trials for a pretty strong likelihood that n is prime" - wziete z dokumentacji
    return n; // mt19937 generator jest domyslny do miller_rabin_test
}

cpp_int getRandom() {
    mt11213b base_gen(clock());
    independent_bits_engine<mt11213b, 512, cpp_int> gen(base_gen);
    cpp_int n = gen();
    return n;
}

// calculates a^b mod m
cpp_int power_modulo_fast(cpp_int a, cpp_int b, cpp_int m) {
    int i;
    cpp_int res = 1;
    cpp_int x = a % m;
    do {
        x %= m;
        if (b % 2 ==
            1) // b&1 jest rownowazne czemus takiemu b%2==1, ale jest szybsze i lepiej odpowiada temu co robimy (sprawdzamy najmłodszy bit)
        {
            res *= x;
            res %= m;
        }
        x *= x;
    } while ((b /= 2) != 0);  // b>>=1 jest rownowazne czemus takiemu: b/=2

    return res;
}

//najwiekszy wspolny dzielnik
cpp_int gcd(cpp_int n1, cpp_int n2) {
    cpp_int pom;
    while (n2 != 0) {
        pom = n2;
        n2 = n1 % n2;
        n1 = pom;
    }
    return n1;
}

cpp_int invMod(cpp_int a, cpp_int b) {
    cpp_int u, w, x, z, q;
    u = 1;
    w = a;
    x = 0;
    z = b;
    while (w) {
        if (w < z) {
            q = u;
            u = x;
            x = q;
            q = w;
            w = z;
            z = q;
        }
        q = w / z;
        u -= q * x;
        w -= q * z;
    }
    if (z == 1) {
        if (x < 0) x += b;
        return x;
    }
    return 0;
}

struct publicKey {
    cpp_int e;
    cpp_int n;
};

struct privateKey {
    cpp_int d;
    cpp_int n;
};

void generateRSAkeys(publicKey &pub, privateKey &priv) {
    cpp_int p = GetPrime();
    cpp_int q = GetPrime();
    cpp_int euler = (p - 1) * (q - 1);
    cpp_int n = p * q;
    cpp_int e = getRandom();
//    e musi byc wzglednie pierwsza z eulerem, 1 < e < n, e musi byc nieparzysta
    while (gcd(euler, e) != 1) {
        e = getRandom();
    }
    cpp_int d = invMod(e, euler);
    pub.e = e;
    pub.n = n;
    priv.d = d;
    priv.n = n;
}

cpp_int shadowing(cpp_int hash, publicKey pub, cpp_int &k) {
    cpp_int shadowed;
    k = getRandom();
//    warunki: k < n i k wzglednie pierwsze z n
    while (gcd(pub.n, k) != 1 && k > pub.n) {
        k = getRandom();
    }
    //                                 (k^pub.e) mod pub.n
    cpp_int buffor = power_modulo_fast(k, pub.e, pub.n);
    shadowed = (buffor * hash) % pub.n;
    return shadowed;
}

cpp_int shadowedSign(cpp_int shadowed, privateKey priv) {
//                                    (shadowed^priv.d) mod priv.n
    cpp_int sPrim = power_modulo_fast(shadowed, priv.d, priv.n);
    return sPrim;
}

cpp_int unshadowSign(cpp_int sPrim, cpp_int k, publicKey pub) {
    cpp_int s;
//    odwrotnosc modulo k i n
    cpp_int kInvertN = invMod(k, pub.n);
    s = (kInvertN * sPrim) % pub.n;
    return s;
}

bool checkSign(cpp_int hash, cpp_int sign, publicKey pub) {
    //                                      sign^pub.e mod pub.n
    cpp_int signChecking = power_modulo_fast(sign, pub.e, pub.n);
    if (signChecking == hash) return true;
    return false;
}

// Function to read plain text from file
string reading_plain_text() {
    ifstream read("..\\reading_plain_text.txt");
    string plaintext;
    getline(read, plaintext);
    read.close();
    return plaintext;
}

string reading_n() {
    std::ifstream file("..\\reading_n.txt"); // open this file for input

    string line;
    string tmp;
    while (std::getline(file, line)) // for each line read from the file
    {
//        std::cout << line << '\n' ;
        tmp += line;
    }
    return tmp;
}

string reading_d() {
    std::ifstream file("..\\reading_d.txt"); // open this file for input
    string line;
    string tmp;
    while (std::getline(file, line)) // for each line read from the file
    {
        tmp += line;
    }
    return tmp;

}

string reading_k() {
    std::ifstream file("..\\reading_k.txt"); // open this file for input
    string line;
    string tmp;
    while (std::getline(file, line)) // for each line read from the file
    {
        tmp += line;
    }
    return tmp;
}

string reading_e() {
    std::ifstream file("..\\reading_e.txt"); // open this file for input
    string line;
    string tmp;
    while (std::getline(file, line)) // for each line read from the file
    {
        tmp += line;
    }
    return tmp;
}

string reading_signature() {
    ifstream read("..\\reading_signature.txt");
    string signature;
    getline(read, signature);
    read.close();
    return signature;
}

string reading_hash_m() {
    ifstream read("..\\reading_hash_m.txt");
    string hashed_m;
    getline(read, hashed_m);
    read.close();
    return hashed_m;
}

// Function to save plain text to file
void save_decrypted(string text) {
    ofstream save("..\\reading_plain_text.txt");
    save << text;
    save.close();
}

void save_n(cpp_int n) {
    ofstream save("..\\reading_n.txt");
    if (save.is_open()) {
        save << n;
    } else cout << "Nie udalo sie otworzyc pliku";
    save.close();
}

void save_e(cpp_int e) {
    ofstream save("..\\reading_e.txt");
    if (save.is_open()) {
        save << e;
    } else cout << "Nie udalo sie otworzyc pliku";
    save.close();
}

void save_d(cpp_int e) {
    ofstream save("..\\reading_d.txt");
    if (save.is_open()) {
        save << e;
    } else cout << "Nie udalo sie otworzyc pliku";
    save.close();
}

void save_k(cpp_int k) {
    ofstream save("..\\reading_k.txt");
    if (save.is_open()) {
        save << k;
    } else cout << "Nie udalo sie otworzyc pliku";
    save.close();
}

void save_signature(cpp_int sig) {
    ofstream save("..\\reading_signature.txt");
    if (save.is_open()) {
        save << sig;
    } else cout << "Nie udalo sie otworzyc pliku";
    save.close();
}

void save_hash_m(size_t hash_m) {
    ofstream save("..\\reading_hash_m.txt");
    if (save.is_open()) {
        save << hash_m;
    } else cout << "Nie udalo sie otworzyc pliku";
    save.close();
}

int main() {
    string plain_text;
    string pub_n, pub_e, priv_n, priv_d;
    publicKey pub;
    privateKey priv;
    cpp_int sprim, k;
    std::size_t m;

    char confirmation;
    int wybor_tekstu;
    // deklaracja kluczy
    do {
        // Plain text or signature
        cout << "\nCzy chcesz utworzyc slepy podpis cyfrowy(1), czy zweryfikowac juz istniejacy podpis(2)?" << endl;
        cout << "Wpisz 1/2: ";
        cin >> wybor_tekstu;
        cout << endl;
        switch (wybor_tekstu) {
            case 1: {// Plain text
                cout << "Czy chcesz wpisac tekst jawny(1) czy odczytac go z pliku(2)?" << endl;
                cout << "Wpisz 1/2:";
                int wybor_tekstu_j;
                cin >> wybor_tekstu_j;
                switch (wybor_tekstu_j) {
                    case 1: {// User input
                        cout << "Wpisz tekst jawny:";
                        std::getline(std::cin >> std::ws, plain_text);
                        cout << "Tekst jawny: " << plain_text << endl;
                        break;
                    }
                    case 2: {// Read from file
                        plain_text = reading_plain_text();
                        cout << "Tekst jawny: " << plain_text << endl;
                        break;
                    }
                    default:
                        cout << "Wpisano bledna liczbe. Sprobuj ponownie." << endl;
                        return -1;
                }
                cout << "Czy chcesz wpisac klucz publiczny recznie(1),odczytac z pliku(2) czy wygenerowac go(3)?"
                     << endl;
                int wybor_k;
                cin >> wybor_k;
                switch (wybor_k) {
                    case 1: {//User input
                        cout << "Podaj n: ";
                        cin >> pub.n;
                        cout << "Podaj e: ";
                        cin >> pub.e;
                        cout << "Klucz publiczny \n" << "n: " << pub.n << endl << "e: " << pub.e << endl;
                        break;
                    }
                    case 2: {// Reading from file
                        pub_n = reading_n();
                        pub_e = reading_e();
                        cpp_int tmp(pub_n);
                        cpp_int tmp2(pub_e);
                        pub.n = tmp;
                        pub.e = tmp2;
                        cout << "Klucz publiczny: " << "n: " << pub.n << endl << "e: " << pub.e << endl;
                        break;
                    }
                    case 3: {// Generating
                        generateRSAkeys(pub, priv);
                        cout << "Klucz publiczny: " << "n: " << pub.n << endl << "e: " << pub.e << endl;
                        break;
                    }

                    default:
                        cout << "Wpisano bledna liczbe. Sprobuj ponownie.";
                        return -1;
                }
                cout << "Czy chcesz wpisac klucz prywatny recznie(1),odczytac z pliku(2) czy wygenerowac go(3)?"
                     << endl;
                int wybor_k2;
                cin >> wybor_k2;
                switch (wybor_k2) {
                    case 1: {//User input
                        cout << "Podaj n: ";
                        cin >> priv.n;
                        cout << "Podaj d: ";
                        cin >> priv.d;
                        cout << "Klucz prywatny: " << "n: " << priv.n << endl << "d: " << priv.d << endl;
                        break;
                    }
                    case 2: {// Reading from file
                        priv_n = reading_n();
                        priv_d = reading_d();
                        cpp_int tmp(priv_n);
                        cpp_int tmp2(priv_d);
                        priv.n = tmp;
                        priv.d = tmp2;
                        cout << "Klucz prywatny: " << "n: " << priv.n << endl << "d: " << priv.d << endl;
                        break;
                    }
                    case 3: {// Generating
                        cout << "Klucz prywatny: " << "n: " << priv.n << endl << "d: " << priv.d << endl;
                        break;
                    }
                    default:
                        cout << "Wpisano bledna liczbe. Sprobuj ponownie.";
                        return -1;
                }
                cout << "Nastepuje haszowanie wiadomosci" << endl;
                m = boost::hash_value(plain_text);
                cout << "Hash: " << m << endl;
                //    zaciemnianie wiadomosci
                cout << "Nastepuje zaciemnienie wiadomosci" << endl;
                cpp_int mprim = shadowing(m, pub, k);
                cout << "K WYNOSI: " << k << endl;
                cout << "Zaciemniona wiadomosc: " << mprim << endl;
                //    podpisanie
                cout << "Nastepuje slepy podpis" << endl;
                sprim = shadowedSign(mprim, priv);
                cout << "Podpis: " << sprim << endl;
                //    zdejmowanie zaciemnienia
                cout << "Nastepuje zdjecie zaciemnienia wiadomosci" << endl;
                cpp_int s = unshadowSign(sprim, k, pub);
                cout << "Nastepuje sprawdzenie podpisu" << endl;
                if (checkSign(m, s, pub)) cout << "Podpis jest poprawny" << endl;
                break;
            }
            case 2: {//Signature
                int wybor_tekstu_p;
                cout << "Czy chcesz wpisac podpis(1) czy odczytac z pliku(2): " << endl;
                cin >> wybor_tekstu_p;
                switch (wybor_tekstu_p) {
                    case 1: {// User input
                        cout << "Podaj podpis: ";
                        cin >> sprim;
                        cout << "Podpis: " << sprim << endl;
                        break;
                    }
                    case 2: {// Read from file
                        string pom = reading_signature();
                        cpp_int tmp(pom);
                        sprim = tmp;
                        cout << "Podpis: " << sprim << endl;
                        break;
                    }
                    default:
                        cout << "Wpisano bledna liczbe. Sprobuj ponownie.";
                        return -1;
                }
                cout << "Czy chcesz wpisac klucz publiczny recznie(1) czy odczytac z pliku(2)?"
                     << endl;
                int wybor_k;
                cin >> wybor_k;
                switch (wybor_k) {
                    case 1: {//User input
                        cout << "Podaj n: ";
                        cin >> pub.n;
                        cout << "Podaj e: ";
                        cin >> pub.e;
                        cout << "Klucz publiczny: " << "n: " << pub.n << endl << "e: " << pub.e << endl;
                        break;
                    }
                    case 2: {// Reading from file
                        pub_n = reading_n();
                        pub_e = reading_e();
                        cpp_int tmp(pub_n);
                        cpp_int tmp2(pub_e);
                        pub.n = tmp;
                        pub.e = tmp2;
                        cout << "Klucz publiczny: " << "n: " << pub.n << endl << "e: " << pub.e << endl;
                        break;
                    }
                    default:
                        cout << "Wpisano bledna liczbe. Sprobuj ponownie.";
                        return -1;
                }
                cout << "Czy chcesz wpisac k(1) czy odczytac z pliku(2)" << endl;
                cin >> wybor_k;
                switch (wybor_k) {
                    case 1: {//User input
                        cout << "Podaj k: ";
                        cin >> k;
                        cout << "Liczba k: " << k << endl;
                        break;
                    }
                    case 2: {// Reading from file
                        string tmp;
                        tmp = reading_k();
                        cpp_int tmp2(tmp);
                        k = tmp2;
                        cout << "Liczba k: " << k << endl;
                        break;
                    }
                    default:
                        cout << "Wpisano bledna liczbe. Sprobuj ponownie.";
                        return -1;
                }
                cout << "Czy chcesz wpisac hash m(1) czy odczytac z pliku(2)" << endl;
                cin >> wybor_k;
                switch (wybor_k) {
                    case 1: {//User input
                        cout << "Podaj m: ";
                        cin >> m;
                        cout << "Wiadomosc m: " << m << endl;
                        break;
                    }
                    case 2: {// Reading from file
                        string tmp;
                        tmp = reading_hash_m();
                        std::stringstream sstream(tmp);
                        sstream >> m;
                        cout << "Wiadomosc m: " << m << endl;
                        break;
                    }
                    default://Error
                        cout << "Wpisano bledna liczbe. Sprobuj ponownie." << endl;
                        return -1;
                }
                //    zdejmowanie zaciemnienia
                cpp_int s = unshadowSign(sprim, k, pub);
                //    weryfikacja podpisu
                if (checkSign(m, s, pub)) cout << "Podpis zweryfikowany pomyslnie!" << endl;
                else cout << "Podpis nieprawidlowy!" << endl;
                break;
            }
            default:
                cout << "Podano bledna liczbe. " << endl;
                return -1;
        }
        cout << "Czy chcesz dokonac zapisu?" << endl;
        cout << "(U) Klucza publicznego | (R) Klucza prywatnego | (T) Tekstu jawnego | (P) Podpisu | (M) Hash wiadomosci | (K) Wartosc k | (N) Nie"
             << endl;
        cin >> confirmation;
        while (confirmation == 'U' || confirmation == 'u' || confirmation == 'R' || confirmation == 'r' ||
               confirmation == 'T' || confirmation == 't' || confirmation == 'P' || confirmation == 'p' ||
               confirmation == 'M' || confirmation == 'm' || confirmation == 'K' || confirmation == 'k') {
            switch (confirmation) {
                case 'U':
                case 'u':
                    save_n(pub.n);
                    save_e(pub.e);
                    cout << "Zapisano klucz publiczny do pliku." << endl;
                    break;
                case 'R':
                case 'r':
                    save_n(priv.n);
                    save_d(priv.d);
                    cout << "Zapisano klucz prywatny do pliku." << endl;
                    break;
                case 'T':
                case 't':
                    save_decrypted(plain_text);
                    cout << "Zapisano tekst jawny do pliku." << endl;
                    break;
                case 'P':
                case 'p':
                    save_signature(sprim);
                    cout << "Zapisano podpis do pliku." << endl;
                    break;
                case 'M':
                case 'm':
                    save_hash_m(m);
                    cout << "Zapisano hash wiadomosci do pliku." << endl;
                    break;
                case 'K':
                case 'k':
                    save_k(k);
                    cout << "Zapisano hash wiadomosci do pliku." << endl;
                    break;
                default:
                    cout << "Bledna odpowiedz" << endl;
            }
            cout << "Czy chcesz zapisac cos jeszcze?\n(U/R/T/P/M/K/N)" << endl;
            cin >> confirmation;
        }
        cout << "Czy chcesz ponownie uruchomic program?\n(T/N)" << endl;
        cin >> confirmation;
    }while (confirmation == 'y' || confirmation == 'Y' || confirmation == 't' || confirmation == 'T');
}