#include <time.h>
#include <iostream>
#include <sstream>
#include <bitset>
#include <fstream>
#include <vector>

using namespace std;

// Function to calculate gcd
// gcd = greatest common divisor (NWD)
int gcd(int m, int n) {
    // Till m == n
    while (m != n) {
        // Large - small; store result in variable, from which we subtracted
        if (m > n)
            m = m - n;
        else
            n = n - m;
    }
    // gcd is either m or n
    return m;
}

// Function to sort an array of values in ascending order (used for super increasing sequence) (bubble sort)
void sort(int arr[]) {
    int i, j, temp;
    for (i = 0; i <= 7; i++) {
        for (j = i + 1; j <= 8; j++) {
            if (arr[i] > arr[j]) {
                temp = arr[i];
                arr[i] = arr[j];
                arr[j] = temp;
            }
        }
    }
}

// Function to summarise all numbers in an array (sigma)
int sum(int a[]) {
    int i, sum = 0;
    for (i = 0; i < 8; i++) {
        sum = sum + a[i];
    }
    return (sum);
}

// Function to convert decimal into a string
string convertDecimalToString(int x) {
    stringstream ss;
    ss << x;
    return ss.str();
}

// Function to convert string into binary
string convertStringToBinary(string given) {
    string binary = "";
    for (char &_char : given) {
        binary += bitset<8>(_char).to_string();
    }
    return binary;
}

// Function to convert binary into string
string convertBinaryToString(string binary) {
    string text = "";
    stringstream sstream(binary);
    while (sstream.good()) {
        bitset<8> bits;
        sstream >> bits;
        text += char(bits.to_ulong());
    }
    return text;
}

// Text is given in binary form, like 0110101001110
int *encrytping(string text, int public_key[], int tab[]) {
//    while (text.length() % 8 != 0) {
//        text.append("00100000");
//    }
    int amountOfBlocks;
    if (text.length() % 8 == 0) amountOfBlocks = text.length() / 8;
    else amountOfBlocks = text.length() / 8 + 1;

    string block_of_cipher_text;
    int x = 0;

    for (int i = 0; i < amountOfBlocks; i++) {
        block_of_cipher_text = text.substr(i * 8, 8);
        x = 0;
        for (int j = 0; j < 8; j++) {
            // substracting 48; case is conversion from ASCII to int
            x += (block_of_cipher_text[j] - 48) * public_key[j];
        }
        tab[i] = x;
//        cipher += convertDecimalToString(x);
    }
    return tab;
}

// Function to calulate the inverse modulo
int odwrotnosc_modulo(int a, int b) {
    int u, w, x, z, q;
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

// Function to read plain text from file
string reading_plain_text() {
    ifstream read("..\\reading_plain_text.txt");
    string plaintext;
    getline(read, plaintext);
    read.close();
    return plaintext;
}

// Function to read keys from file
void
reading_public_key(string *key1, string *key2, string *key3, string *key4, string *key5, string *key6, string *key7,
                   string *key8) {
    ifstream read("..\\reading_public_key.txt");
    if (read.is_open()) {
        read >> *key1;
        read >> *key2;
        read >> *key3;
        read >> *key4;
        read >> *key5;
        read >> *key6;
        read >> *key7;
        read >> *key8;
    } else cout << "Nie udalo sie otworzyc pliku";
    read.close();
}

void
reading_private_key(string *key1, string *key2, string *key3, string *key4, string *key5, string *key6, string *key7,
                   string *key8) {
    ifstream read("..\\reading_private_key.txt");
    if (read.is_open()) {
        read >> *key1;
        read >> *key2;
        read >> *key3;
        read >> *key4;
        read >> *key5;
        read >> *key6;
        read >> *key7;
        read >> *key8;
    } else cout << "Nie udalo sie otworzyc pliku";
    read.close();
}

int reading_variable(string txt) {
    ifstream read(txt);
    int variable;
    string variable_str;
    getline(read, variable_str);
    read.close();
    variable = stoi(variable_str);
    return variable;
}
void save_m(int m) {
    ofstream save("..\\reading_m.txt");
    if (save.is_open()) {
        save << m;
    } else cout << "Nie udalo sie otworzyc pliku";
    save.close();
}

void save_w(int w) {
    ofstream save("..\\reading_w.txt");
    if (save.is_open()) {
        save << w;
    } else cout << "Nie udalo sie otworzyc pliku";
    save.close();
}

// Function to save encrypted text to file
void save_encrypted(string text) {
    ofstream save("..\\reading_encrypted.txt");
    save << text;
    save.close();
}

// Function to save plain text to file
void save_decrypted(string text) {
    ofstream save("..\\reading_plain_text.txt");
    save << text;
    save.close();
}

void save_public_key(int pub[]) {
    ofstream save("..\\reading_public_key.txt");
    for (int i = 0; i < 8; i++) {
        save << pub[i] << '\n';
    }
    save.close();
}

void save_private_key(int priv[]) {
    ofstream save("..\\reading_private_key.txt");
    for (int i = 0; i < 8; i++) {
        save << priv[i] << '\n';
    }
    save.close();
}

// Main function
int main() {
    // Implementacja menu
    int i, j, sum_pk, m, w;
    string plain_text;
    string encrypted_text;
    int public_key[8];
    int private_key[8];
    int amount_of_blocks;
    int *cipher;
    vector<string> ss;
    string decrypted;
    char confirmation;
    do {
        // Plain text or encrypted
        cout << "\nCzy chcesz szyfrowac tekst jawny(1) czy odszyfrowywac kryptogram(2)?" << endl;
        cout << "Wpisz 1/2: ";
        int wybor_tekstu;
        cin >> wybor_tekstu;
        cout << endl;

//        int inverse = odwrotnosc_modulo(w,m);
        switch (wybor_tekstu) {
            case 1: {// Plain text
                cout << "Czy chcesz wpisac tekst jawny(1) czy odczytac z pliku(2)?" << endl;
                cout << "Wpisz 1/2: ";
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
                    default:// Error
                        cout << "Wpisano bledna liczbe. Sprobuj ponownie." << endl;;
                        return -1;
                }
                cout << "Czy chcesz wpisac klucz publiczny recznie(1),odczytac z pliku(2) czy wygenerowac go(3)?"
                     << endl;
                int wybor_k;
                string pk1, pk2, pk3, pk4, pk5, pk6, pk7, pk8;
                cin >> wybor_k;
                switch (wybor_k) {
                    case 1: {// User input
                        cout << "Wpisz klucz publiczny:";
                        for (int i = 0; i < 8; i++) {
                            cout << "Wpisz " << i + 1 << " wartosc: ";
                            cin >> public_key[i];
                        }
                        cout << "Publiczny klucz: ";
                        for (int i = 0; i < 8; i++) {
                            cout << public_key[i];
                        }
                        break;
                    }
                    case 2: {// Read from file
                        reading_public_key(&pk1, &pk2, &pk3, &pk4, &pk5, &pk6, &pk7, &pk8);
                        cout << "Publiczny klucz: ";
                        cout << pk1 << " " << pk2 << " " << pk3 << " " << pk4 << " " << pk5 << " " << pk6 << " " << pk7
                             << " " << pk8 << endl;
                        public_key[0] = stoi(pk1);
                        public_key[1] = stoi(pk2);
                        public_key[2] = stoi(pk3);
                        public_key[3] = stoi(pk4);
                        public_key[4] = stoi(pk5);
                        public_key[5] = stoi(pk6);
                        public_key[6] = stoi(pk7);
                        public_key[7] = stoi(pk8);

                        for (int i = 0; i < 8; i++) {
                            cout << public_key[i] << " ";
                        }
                        break;
                    }
                    case 3: {// Generating public key
                        srand(time(NULL));
                        int counter = 0;
                        for (int i = 0; i < 8; i++) {
                            private_key[i] = (rand() % 100) + counter;
                            counter += private_key[i];
                        }

                        sum_pk = sum(private_key);
                        // Creating M - one of two prime numbers (from modulo of m)
                        m = rand();

                        while (m < sum_pk) {
                            m = rand();
                        }

                        sort(private_key);

                        cout << "Prywatny klucz(superrosnacy wektor): ";

                        for (j = 0; j < 8; j++)
                            cout << private_key[j] << "   ";

                        cout << "\nWartosc wybranego M: " << m << endl;

                        // Creating "W" - one of prime numbers
                        // Making sure that "W" is a prime number
                        for (int k = (m / 2 + 1); k < m; k++) {
                            w = k;
                            if (gcd(m, w) == 1 && m > w && w > (m / 2)) break;
                        }
                        cout << "Wybrana wartosc W: " << w;

                        for (i = 0; i < 8; i++) {
                            public_key[i] = (w * private_key[i]) % m;
                        }

                        cout << "\n\nPubliczny klucz: ";

                        for (j = 0; j < 8; j++)
                            cout << public_key[j] << "   ";
                        break;
                    }
                    default:
                        cout << "Wpisano bledna liczbe. Sprobuj ponownie.";
                        return -1;
                }
                string binary_text = convertStringToBinary(plain_text);

                amount_of_blocks = binary_text.length() / 8;
                cout << "Szyfr: " << endl;
                int tab[amount_of_blocks];
                cipher = encrytping(binary_text, public_key, tab);

                for (int i = 0; i < amount_of_blocks; i++) {
                    cout << cipher[i] << " ";
                    encrypted_text += convertDecimalToString(cipher[i]) + '\n';
                }
                cout << endl;
                cout << "PLAIN TEXT: " << plain_text;
                break;
            }
            case 2: {// Encrypted
                cout << "Czy chcesz wpisac tekst zaszyfrowany(1) czy odczytac z pliku(2)?" << endl;
                cout << "Wpisz 1/2: ";
                int wybor_tekstu_sz;
                cin >> wybor_tekstu_sz;
                switch (wybor_tekstu_sz) {
                    case 1: {
                        string s;
                        getline(cin >> ws, s);
                        istringstream iss(s);
                        string word;
//                        string* ss1;
                        vector<string> ss1;
                        int x = 0;
                        while (getline(iss, s, ' ')) {
                            ss1.push_back(s);
                        }
                        int z[ss1.size()];
                        for (int k = 0; k < ss1.size(); ++k) {
                            istringstream(ss1[k]) >> z[k];
                        }
                        cipher = z;
                        cout << "Cipher: ";
//                        for (int k = 0; k < ss1.size(); ++k) {
//                            cout << cipher[k] << " ";
//                        }
                        for (int k = 0; k < ss1.size(); ++k) {
                            cout << cipher[k] << " ";
                            amount_of_blocks = k;
                        }
                        break;
                    }
                    case 2: {
                        ifstream file("..\\reading_encrypted.txt");
                        string s;
                        string word;
//                        vector<string> ss;
                        while (getline(file, s)) {
                            ss.push_back(s);
                        }
                        int z[ss.size()];
                        for (int k = 0; k < ss.size(); ++k) {
                            istringstream(ss[k]) >> z[k];
                        }
                        cipher = z;
                        cout << "Cipher: ";
                        for (int k = 0; k < ss.size(); ++k) {
                            cout << cipher[k] << " ";
                        }
                        amount_of_blocks = ss.size();
                        file.close();
                        break;
                    }
                    default:
                        cout << "Wpisano bledna liczbe. Sprobuj ponownie.";
                        return -1;
                }
                cout << endl;
                cout << "Czy chcesz wpisac klucz prywatny(1) czy odczytac z pliku(2)?" << endl;
                cout << "Wpisz 1/2: ";
                cin >> wybor_tekstu_sz;
                string pkr1, pkr2, pkr3, pkr4, pkr5, pkr6, pkr7, pkr8;
                switch (wybor_tekstu_sz) {
                    case 1: {
                        cout << "Wpisz klucz prywatny:";
                        for (int i = 0; i < 8; i++) {
                            cout << "Wpisz " << i + 1 << " wartosc: ";
                            cin >> private_key[i];
                        }
                        cout << "Prywatny klucz: ";
                        for (int i = 0; i < 8; i++) {
                            cout << private_key[i];
                        }
                        break;
                    }
                    case 2: {
                        reading_private_key(&pkr1, &pkr2, &pkr3, &pkr4, &pkr5, &pkr6, &pkr7, &pkr8);
                        private_key[0] = stoi(pkr1);
                        private_key[1] = stoi(pkr2);
                        private_key[2] = stoi(pkr3);
                        private_key[3] = stoi(pkr4);
                        private_key[4] = stoi(pkr5);
                        private_key[5] = stoi(pkr6);
                        private_key[6] = stoi(pkr7);
                        private_key[7] = stoi(pkr8);
                        cout << "Prywatny klucz: ";
                        for (int i = 0; i < 8; i++) {
                            cout << private_key[i] << " ";
                        }
                        break;
                    }
                    default:
                        cout << "Wpisano bledna liczbe. Sprobuj ponownie.";
                        return -1;
                }
                cout << endl << "Czy chcesz wpisac w(1) czy odczytac z pliku(2)?" << endl;
                cout << "Wpisz 1/2: ";
                int wr;
                cin >> wybor_tekstu_sz;
                switch (wybor_tekstu_sz) {
                    case 1: {
                        cout << "Podaj w: ";
                        cin >> w;
                        cout << "W wynosi: " << w;
                        break;
                    }
                    case 2: {
                        w = reading_variable("..\\reading_w.txt");
                        cout << "W wynosi: " << w;
                        break;
                    }
                    default:
                        cout << "Wpisano bledna liczbe. Sprobuj ponownie.";
                        return -1;
                }
                cout << endl << "Czy chcesz wpisac m(1) czy odczytac z pliku(2)?" << endl;
                cout << "Wpisz 1/2: ";
                cin >> wybor_tekstu_sz;
                switch (wybor_tekstu_sz) {
                    case 1: {
                        cout << "Podaj m: ";
                        cin >> m;
                        cout << "M wynosi: " << m;
                        break;
                    }
                    case 2: {
                        m = reading_variable("..\\reading_m.txt");
                        cout << "M wynosi: " << m;
                        break;
                    }
                    default:
                        cout << "Wpisano bledna liczbe. Sprobuj ponownie.";
                        return -1;
                }

                int result[8];
                int inverse = odwrotnosc_modulo(w, m);
//                cout << endl << "Private key: ";
//                for (int k = 0; k < 8; ++k) {
//                    cout << private_key[k] << " ";
//                }
//                cout << endl << "W: " << w;
//                cout << endl << "M: " << m;
                int z[ss.size()];
                for (int k = 0; k < ss.size(); ++k) {
                    istringstream(ss[k]) >> z[k];
                }
                cipher = z;
////                JEST PROBLEM Z SZYFREM
//                cout << endl << "Adres ciphera: " << cipher;
//                cout << endl << "Cipher: ";
//                for (int k = 0; k < amount_of_blocks; ++k) {
//                    cout << k << " = " << cipher[k] << " ";
//                }
                int tab[amount_of_blocks+1];
                for (int b = 0; b < amount_of_blocks+1; b++) {
                    tab[b] = (cipher[b] * inverse) % m;
                    for (int k = 0; k < 8; k++) {
                        if (tab[b] >= private_key[7 - k]) {
                            tab[b] -= private_key[7 - k];
                            result[7 - k] = 1;
                        } else result[7 - k] = 0;
                    }
                    for (int l = 0; l < 8; l++) {
                        decrypted += convertDecimalToString(result[l]);
                    }
                }
                cout << endl << "Odszyfrowany tekst: " << convertBinaryToString(decrypted) << endl;

                break;
            }
            default:
                cout << "Wpisano bledna liczbe. Sprobuj ponownie.";
                return -1;
        }
        cout << "Czy chcesz dokonac zapisu?" << endl;
        cout
                << "(U) Klucza publicznego | (R) Klucza prywatnego | (J) Tekstu jawnego | (C) Kryptografu?| (M) m | (W) w (N) Nie chce dokonac zapisu"
                << endl;
        cin >> confirmation;
        while (confirmation == 'U' || confirmation == 'u' || confirmation == 'R' || confirmation == 'r' ||
               confirmation == 'J' || confirmation == 'j' || confirmation == 'C' || confirmation == 'c' ||
               confirmation == 'W' || confirmation == 'w' || confirmation == 'M' || confirmation == 'm') {
            switch (confirmation) {
                case 'U':
                case 'u':
                    save_public_key(public_key);
                    cout << "Zapisano klucz publiczny do pliku." << endl;
                    break;
                case 'R':
                case 'r':
                    save_private_key(private_key);
                    cout << "Zapisano klucz prywatny do pliku." << endl;
                    break;
                case 'J':
                case 'j':
                    save_decrypted(plain_text);
                    cout << "Zapisano tekst jawny do pliku." << endl;
                    break;
                case 'C':
                case 'c':
                    save_encrypted(encrypted_text);
                    cout << "Zapisano kryptograf do pliku." << endl;
                    break;
                case 'W':
                case 'w':
                    save_w(w);
                    cout << "Zapisano w do pliku." << endl;
                    break;
                case 'M':
                case 'm':
                    save_m(m);
                    cout << "Zapisano m do pliku." << endl;
                    break;
                default:
                    cout << "Bledna odpowiedz" << endl;
            }
            cout << "Czy chcesz zapisac cos jeszcze?\n(U/R/J/C/M/W/N)" << endl;
            cin >> confirmation;
        }
        cout << "Czy chcesz ponownie uruchomic program?\n(T/N)" << endl;
        cin >> confirmation;
    } while (confirmation == 'y' || confirmation == 'Y' || confirmation == 't' || confirmation == 'T');
    return 0;

}