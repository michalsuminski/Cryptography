// Including dependencies
#include <iostream>
#include <string>
#include <cmath>
#include <bitset>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <ctime>
using namespace std;

// Array to hold 16 keys
string round_keys[16];
// String to hold the plain text
string pt;
// Function to convert a number in decimal to binary
string convertDecimalToBinary(int decimal)
{
	string binary;
    while(decimal != 0) {
		binary = (decimal % 2 == 0 ? "0" : "1") + binary; // (if)? first : second -> if "if" = true then first, if it's else then second
		decimal = decimal/2;
	}
	while(binary.length() < 4){
		binary = "0" + binary;
	}
    return binary;
}
// Function to convert a number in binary to decimal
int convertBinaryToDecimal(string binary)
{
    int decimal = 0;
	int counter = 0;
	int size = binary.length();
	for(int i = size-1; i >= 0; i--) // we read binary from right side and raise two to next powers
	{
    	if(binary[i] == '1'){
        	decimal += pow(2, counter); // pow(x, y) - raising x to the power of y
    	}
    counter++;
	}
	return decimal;
}
// Function to convert a string into binary
string convertStringToBinary(string given){
    string binary;
    int helpLength;
    int size = given.length();
    for(int i=0; i<size; i++){
        // Coverts every character of string into its ASCII value
        int var = int(given[i]);
        string help;
        while(var>0){
            if(var%2==0){
                help = "0" + help;
            }
            else{
                help = "1" + help;
            }
            var=var/2;
        }
        helpLength = help.length();
        while(helpLength%8 != 0){
            help = "0" + help;
            helpLength++;
        }
        binary = binary + help;
    }
    return binary;
}
// Function to convert binary into string
string convertBinaryToString(string given){
    stringstream sstream(given);
    string result;
    // True, till end of sstream or error occurance
    while(sstream.good()){
        // Creating bitset
        bitset<8> var;
        sstream >> var;
        // Converting to unsigned long
        char letter = char(var.to_ulong());
        result = result + letter;
    }
    return result;
}
// Function to read plain text from file
string reading_plain_text(){
    ifstream read("..\\read_plain_text.txt");
    string plaintext;
    getline(read,plaintext);
    read.close();
    return plaintext;
}
// Function to read keys from file
void reading_keys(string *key1,string *key2, string *key3){
    ifstream read("..\\read_keys.txt");
    if(read.is_open())
    {
        read>>*key1;
        read>>*key2;
        read>>*key3;
    }
    else cout<<"Nie udało się otworzyć pliku";
    read.close();
}
// Function to read encrypted text from file
string reading_encrypted_text(){
//    ifstream read("..\\read_encrypted_text.txt");
    ifstream read("..\\saved_encrypted"); //.txt
    string encrypted_text;
    getline(read,encrypted_text);
    read.close();
    return encrypted_text;
}
// Function to save keys to file
void save_keys(string *key1,string *key2, string *key3){
    ofstream save("..\\saved_keys.txt");
    if(save.is_open())
    {
        save << *key1;
        save << endl;
        save << *key2;
        save << endl;
        save << *key3;
    }
    else cout<<"Nie udało się otworzyć pliku";
    save.close();
}
// Function to save encrypted text to file
void save_encrypted(string text){
    ofstream save("..\\saved_encrypted"); //.txt
    save << text;
    save.close();
}
// Function to save plain text to file
void save_decrypted(string text){
    ofstream save("..\\saved_decrypted.txt");
    save << text;
    save.close();
}
// Function to do a circular left shift by 1
string shift_left_once(string key_chunk){ 
    string shifted="";  
        for(int i = 1; i < 28; i++){ 
            shifted += key_chunk[i]; 
        } 
        shifted += key_chunk[0];   
    return shifted; 
} 
// Function to do a circular left shift by 2
string shift_left_twice(string key_chunk){ 
    string shifted=""; 
    for(int i = 0; i < 2; i++){ 
        for(int j = 1; j < 28; j++){ 
            shifted += key_chunk[j]; 
        } 
        shifted += key_chunk[0]; 
        key_chunk= shifted; 
        shifted =""; 
    } 
    return key_chunk; 
}
// Function to compute xor between two strings
string Xor(string a, string b){ 
	string result = ""; 
	int size = b.size();
	for(int i = 0; i < size; i++){ 
		if(a[i] != b[i]){ 
			result += "1"; 
		}
		else{ 
			result += "0"; 
		} 
	} 
	return result; 
} 
// Function to generate the 16 keys.
void generate_keys(string key){
	// The PC1 table
	int pc1[56] = {
	57,49,41,33,25,17,9, // right part
	1,58,50,42,34,26,18,
	10,2,59,51,43,35,27,
	19,11,3,60,52,44,36,
	63,55,47,39,31,23,15, // left part
	7,62,54,46,38,30,22,
	14,6,61,53,45,37,29,
	21,13,5,28,20,12,4
	};
	// The PC2 table
	int pc2[48] = { 
	14,17,11,24,1,5,3,28,
	15,6,21,10,23,19,12,4,
	26,8,16,7,27,20,13,2,
	41,52,31,37,47,55,30,40,
	51,45,33,48,44,49,39,56,
	34,53,46,42,50,36,29,32
	}; 
	// 1. Compressing the key using the PC1 table
	string perm_key =""; 
	for(int i = 0; i < 56; i++){ 
		perm_key+= key[pc1[i]-1]; // permutation PC-1 (we go back/left 1 place, because array starts at 0)
	} 
	// 2. Dividing the key into two equal halves
	string left= perm_key.substr(0, 28);  // from 0 to 27
	string right= perm_key.substr(28, 28); // from 28 to 55
	for(int i=0; i<16; i++){ 
		// 3.1. For rounds 1, 2, 9, 16 the key_chunk are shifted by one.
		if(i == 0 || i == 1 || i==8 || i==15 ){
			left= shift_left_once(left); 
			right= shift_left_once(right);
		} 
		// 3.2. For other rounds, the key_chunk are shifted by two
		else{
			left= shift_left_twice(left); 
			right= shift_left_twice(right);
		}
		// Combining the two chunks
		string combined_key = left + right; // merging together left and right part of key
		string round_key = ""; 
		// Finally, using the PC2 table to transpose the key bits
		for(int i = 0; i < 48; i++){ 
			round_key += combined_key[pc2[i]-1]; // choosing 48 out of 56 bits of key in permutation PC-2
		}   
		round_keys[i] = round_key; // we have array of 16 subkeys
	}
}
// Implementing the DES algorithm
string DES(){ 
	// The initial permutation table
	int initial_permutation[64] = { 
	58,50,42,34,26,18,10,2, 
	60,52,44,36,28,20,12,4, 
	62,54,46,38,30,22,14,6, 
	64,56,48,40,32,24,16,8, 
	57,49,41,33,25,17,9,1, 
	59,51,43,35,27,19,11,3, 
	61,53,45,37,29,21,13,5, 
	63,55,47,39,31,23,15,7 
	}; 
	// The expansion table (expansion permutation)
	int expansion_table[48] = { 
	32,1,2,3,4,5,4,5, 
	6,7,8,9,8,9,10,11, 
	12,13,12,13,14,15,16,17, 
	16,17,18,19,20,21,20,21, 
	22,23,24,25,24,25,26,27, 
	28,29,28,29,30,31,32,1 
	}; 
	// The substitution boxes. The should contain value from 0 to 15 in any order.
	int substition_boxes[8][4][16]=  
	{{ 
        14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7, 
        0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8, 
        4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0, 
        15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13 
    }, 
    { 
        15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10, 
        3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5, 
        0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15, 
        13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9 
    }, 
    { 
        10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8, 
        13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1, 
        13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7, 
        1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12 
    }, 
    { 
        7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15, 
        13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9, 
        10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4, 
        3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14 
    }, 
    { 
        2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9, 
        14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6, 
        4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14, 
        11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3 
    }, 
    { 
        12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11, 
        10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8, 
        9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6, 
        4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13 
    }, 
    { 
        4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1, 
        13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6, 
        1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2, 
        6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12 
    }, 
    { 
        13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7, 
        1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2, 
        7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8, 
        2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11 
    }};
	// The permutation table (funkcja permutacyjna)
	int permutation_tab[32] = { 
	16,7,20,21,29,12,28,17, 
	1,15,23,26,5,18,31,10, 
	2,8,24,14,32,27,3,9,
	19,13,30,6,22,11,4,25 
	}; 
	// The inverse permutation table (odwrotność permutacji początkowej = permutacja końcowa)
	int inverse_permutation[64]= { 
	40,8,48,16,56,24,64,32, 
	39,7,47,15,55,23,63,31, 
	38,6,46,14,54,22,62,30, 
	37,5,45,13,53,21,61,29, 
	36,4,44,12,52,20,60,28, 
	35,3,43,11,51,19,59,27, 
	34,2,42,10,50,18,58,26, 
	33,1,41,9,49,17,57,25 
	};
	//1. Applying the initial permutation
  	string perm = ""; 
	for(int i = 0; i < 64; i++){
		perm += pt[initial_permutation[i]-1]; // pt to podany tekst
	}  
	// 2. Dividing the result into two equal halves 
	string left = perm.substr(0, 32); 
	string right = perm.substr(32, 32);
	// The plain text is encrypted 16 times  
	for(int i=0; i<16; i++) { 
    	string right_expanded = ""; 
		// 3.1. The right half of the plain text is expanded
    	for(int i = 0; i < 48; i++) { 
      		right_expanded += right[expansion_table[i]-1]; 
    };  // 3.3. The result is xored with a key
		string xored = Xor(round_keys[i], right_expanded);  
		string res = ""; 
		// 3.4. The result is divided into 8 equal parts and passed 
		// through 8 substitution boxes. After passing through a 
		// substituion box, each box is reduces from 6 to 4 bits.
		for(int i=0;i<8; i++){ 
			// Finding row and column indices to lookup the
			// substituition box
      		string row1= xored.substr(i*6,1) + xored.substr(i*6 + 5,1); // 2 skrajne bity to nr wiersza
      		int row = convertBinaryToDecimal(row1);
            string col1 = xored.substr(i*6 + 1,4); // środkowe 4 bity to nr koolumny
			int col = convertBinaryToDecimal(col1);
			int val = substition_boxes[i][row][col];
			res += convertDecimalToBinary(val);  // res jest stringiem
		} 
		// 3.5. Another permutation is applied
		string perm2 =""; 
		for(int i = 0; i < 32; i++){ 
			perm2 += res[permutation_tab[i]-1]; 
		}
		// 3.6. The result is xored with the left half
		xored = Xor(perm2, left);
		// 3.7. The left and the right parts of the plain text are swapped 
		left = xored; 
		if(i < 15){ // w ostatnim przejściu już nie zamieniamy miejscami
			string temp = right;
			right = xored;
			left = temp;
		} 
	} 
	// 4. The halves of the plain text are applied
	string combined_text = left + right;   // prawa strona z xorem (CZEMU TU TAK JEST)
	string ciphertext =""; 
	// The inverse of the initial permuttaion is applied
	for(int i = 0; i < 64; i++){ 
		ciphertext+= combined_text[inverse_permutation[i]-1]; // permutacja końcowa
	}
	//And we finally get the cipher text
	return ciphertext; 
}
// Function to encrypt plain text into encrypted one
string encrytping (string text){
    int amountOfBlocks;
    if ( text.length() % 64 == 0) amountOfBlocks = text.length() / 64;
    else amountOfBlocks = text.length() / 64 +1;

    string str;

    for ( int i = 0; i < amountOfBlocks; i++){
        pt = text.substr(i*64,64);
        string ct = DES();
        ct = ct.substr(0, pt.length());
        str = str + ct;
    }
    return str;
}

string decrytping (string text){
    int i = 15;
    int j = 0;
    while (i > j) { // odwracamy kolejność podkluczy
        string temp = round_keys[i];
        round_keys[i] = round_keys[j];
        round_keys[j] = temp;
        i--;
        j++;
    }
    return encrytping(text);
}
// Function to generate keys
string generateInputKeys(){
    string key1 = "";
    int x;
    for (int i = 0; i < 64; i++){
        x = rand() % 2;
        if (x == 0){
            key1 = key1 + "0";
        }
        else key1 = key1 + "1";
    }
    return key1;
}
// Function to check whether key given by user is correct or not
bool isKeyGood(string key){
//    key = convertStringToBinary(key);
    if (key.length() == 64) return true;
    else return false;
}

string DESX(string key, string text){
    return Xor(key,text);
}

//Main function
int main(){
    srand( time( NULL ) );
    string pom;
    //Implementation of menu
    char confirmation;
    do{
        //KEYS
        std::cout << "Czy chcesz wygenerowac klucze(1), wpisac(2) czy odczytac z pliku(3)?"<<endl;
        std::cout << "Wpisz 1/2/3: ";
        int wybor_klucze;
        cin >> wybor_klucze;
        string key1,key2,key3;
        cout << endl;
        switch(wybor_klucze){
            case 1: {//GENERATING KEYS
                key1 = generateInputKeys();
                cout <<"Klucz 1: " << convertBinaryToString(key1) <<endl;
                cout <<"Klucz 1 (w postaci binarnej): " << key1 <<endl;
                key2 = generateInputKeys();
                cout <<"Klucz 2: " << convertBinaryToString(key2) <<endl;
                cout <<"Klucz 2 (w postaci binarnej): " << key2 <<endl;
                key3 = generateInputKeys();
                cout <<"Klucz 3: " << convertBinaryToString(key3) <<endl;
                cout <<"Klucz 3 (w postaci binarnej): " << key3 <<endl;
                break;
            }
            case 2: {//USER INPUT
                do {
                    cout << "Wpisz klucz 1 (wymagana postac binarna):";
                    cin >> key1;
                    if (!isKeyGood(key1)) cout << "Niepoprawny klucz, sprobuj jeszcze raz!" << endl;
                }while (!isKeyGood(key1));
                do {
                    cout << "Wpisz klucz 2 (wymagana postac binarna):";
                    cin >> key2;
                    if (!isKeyGood(key2)) cout << "Niepoprawny klucz, sprobuj jeszcze raz!" << endl;
                }while (!isKeyGood(key2));
                do {
                    cout << "Wpisz klucz 3 (wymagana postac binarna):";
                    cin >> key3;
                    if (!isKeyGood(key3)) cout << "Niepoprawny klucz, sprobuj jeszcze raz!" << endl;
                }while (!isKeyGood(key3));
                break;
            }
            case 3: {//READING FROM FILE
                reading_keys(&key1,&key2,&key3);
                cout <<"Klucz 1: " << convertBinaryToString(key1) <<endl;
                cout <<"Klucz 1 (postac binarna): " << key1 <<endl;
                cout <<"Klucz 2: " << convertBinaryToString(key2) <<endl;
                cout <<"Klucz 2 (postac binarna): " << key2 <<endl;
                cout <<"Klucz 3: " << convertBinaryToString(key3) <<endl;
                cout <<"Klucz 3 (postac binarna): " << key3 <<endl;
                break;
            }
            default://ERROR
                cout << "Wpisano bledna liczbe. Sprobuj ponownie.";
                return -1;
        }
        generate_keys(key2);
        cout << endl;
        string szyfr;
        //PLAIN TEXT OR ENCRYPTED TEXT
        std::cout << "Czy chcesz szyfrowac tekst jawny(1) czy odszyfrowywac tekst zaszyfrowany(2)?" << endl;
        std::cout << "Wpisz 1/2: ";
        int wybor_tekstu;
        cin >> wybor_tekstu;
        cout << endl;
        string plain_text;
        string encrypted_text;
        switch(wybor_tekstu){
            //PLAIN TEXT
            case 1: {
                std::cout << "Czy chcesz wpisac tekst jawny(1) czy odczytac z pliku(2)?"<<endl;
                std::cout << "Wpisz 1/2: ";
                int wybor_tekstu_j;
                cin >> wybor_tekstu_j;
                switch (wybor_tekstu_j) {
                    case 1: {//USER IMPUT
                        cout << "Wpisz tekst jawny:";
                        std::getline(std::cin >> std::ws, plain_text);
                        break;
                    }
                    case 2: {//READING FROM FILE
                        plain_text = reading_plain_text();
                        cout << "Tekst jawny: " << plain_text << endl;
                        break;
                    }
                    default://ERROR
                        cout << "Wpisano bledna liczbe. Sprobuj ponownie.";
                        return -1;
                }
                //PROCESS OF ENCRYPTING THE PLAIN TEXT
                plain_text = convertStringToBinary(plain_text); // dopełnienie bloku spacjami do 64-bitów
                while (plain_text.length() % 64 != 0) {
                    plain_text.append("00100000");
                }
                int ile_pl = plain_text.length()/64;
                string key1extra;
                for(int i = 0; i<ile_pl ; i++)
                {
                    key1extra.append(key1);
                }
                plain_text = DESX(key1extra,plain_text);
                pom = encrytping(plain_text); // pom to szyfr w postaci binarnej
                int ile = pom.length()/64;
                string key3extra;
                for(int j = 0; j<ile ; j++)
                {
                    key3extra.append(key3);
                }
                pom = DESX(key3extra,pom);
                string x = convertBinaryToString(pom);
                cout << "Zaszyfrowany tekst: " << x << endl;
                cout << "Zaszyfrowany tekst (w postaci binarnej): " << pom << endl;
                szyfr = pom;
                cout << "Czy chcesz go odrazu odszyfrowac(1) czy przejsc dalej(2)?" << endl;
                cout << "Wpisz 1 lub 2:";
                int wybor;
                cin >> wybor;
                if (wybor  == 1) {
                    pom = DESX(key3extra,pom);
                    string decrypted = decrytping(pom);
                    decrypted = DESX(key1extra,decrypted);
                    decrypted = convertBinaryToString(decrypted);
                    cout << "Rozszyfrowany tekst: " << decrypted << endl;
					break;
                }
                else break;
            }
                //ENCRYPTED TEXT
            case 2: {
                std::cout << "Czy chcesz wpisac tekst zaszyfrowany(1) czy odczytac z pliku(2)?"<<endl;
                std::cout << "Wpisz 1/2: ";
                int wybor_tekstu_sz;
                cin >> wybor_tekstu_sz;
                switch (wybor_tekstu_sz){
                    case 1: {//USER INPUT
                        cout << "Wpisz tekst zaszyfrowany (wymagana binarna postac szyfru):";
                        std::getline(std::cin >> std::ws, encrypted_text);
                        cout << "Podaj klucz1 wedlug, ktorego tekst byl szyfrowany (wymagana binarna postac klucza):";
                        cin >> key1;
                        cout << "Podaj klucz2 wedlug, ktorego tekst byl szyfrowany (wymagana binarna postac klucza):";
                        cin >> key2;
                        cout << "Podaj klucz3 wedlug, ktorego tekst byl szyfrowany (wymagana binarna postac klucza):";
                        cin >> key3;
                        break;
                    }
                    case 2:{//READING FROM FILE
                        encrypted_text = reading_encrypted_text();
                        string encrypted_text_string = convertBinaryToString(encrypted_text);
                        cout << "Tekst zaszyfrowany: " << encrypted_text_string << endl;
                        cout << "Tekst zaszyfrowany (w postaci binarnej): " << encrypted_text << endl;
                        cout << "Podaj klucz1 wedlug, ktorego tekst byl szyfrowany (wymagana binarna postac klucza):";
                        cin >> key1;
                        cout << "Podaj klucz2 wedlug, ktorego tekst byl szyfrowany (wymagana binarna postac klucza):";
                        cin >> key2;
                        cout << "Podaj klucz3 wedlug, ktorego tekst byl szyfrowany (wymagana binarna postac klucza):";
                        cin >> key3;
                        break;
                    }
                    default: {//ERROR
                        cout << "Wpisano bledna liczbe. Sprobuj ponownie.";
                        return -1;
                    }
                }
                //Decrytping ciphertext
                generate_keys(key2); // generacja podkluczy przed odszyfrowywaniem

                int ile_en1 = encrypted_text.length()/64;
                string key3en;
                for(int k=0;k<ile_en1;k++)
                {
                    key3en.append(key3);
                }
                encrypted_text = DESX(key3en,encrypted_text);
                string decrypted = decrytping(encrypted_text);
                int ile_en2 = decrypted.length()/64;
                string key1en;
                for(int l=0;l<ile_en2;l++)
                {
                    key1en.append(key1);
                }
                decrypted = DESX(key1en,decrypted);
                decrypted = convertBinaryToString(decrypted);
                cout << "Odszyfrowany tekst: " << decrypted << endl;
                break;
            }
                //ERROR
            default: {
                cout << "Wpisano bledna liczbe. Sprobuj ponownie.";
                return -1;
            }

        }
        //Reszta kodu
        cout << "Czy chcesz dokonac zapisu?" << endl;
        cout << "(K) Kluczy | (J) Tekstu jawnego | (Z) Tekstu zaszyfrowanego?| (N) Nie chce dokonac zapisu" << endl;
        cin >> confirmation;
        while(confirmation == 'k' || confirmation == 'K' || confirmation == 'j' || confirmation == 'J' || confirmation == 'z' || confirmation == 'Z'){
            switch(confirmation){
                case 'k': case 'K':
                    save_keys(&key1, &key2, &key3);
                    cout << "Zapisano klucze do pliku." << endl;
                    break;
                case 'j': case 'J':
                    save_decrypted(plain_text);
                    cout << "Zapisano tekst jawny do pliku." << endl;
                    break;
                case 'z': case 'Z':
                    save_encrypted(szyfr);
                    cout << "Zapisano tekst zaszyfrowany do pliku." << endl;
                    break;
                case 'n': case 'N':
                    cout << "Nie zapisano" << endl;
                    break;
                default:
                    cout << "Bledna odpowiedz" << endl;
            }
            cout << "Czy chcesz zapisac cos jeszcze?\n(K/J/Z/N)" << endl;
            cin >> confirmation;
        }
        cout << "Czy chcesz ponownie uruchomic program?\n(T/N)" << endl;
        cin >> confirmation;
    }while(confirmation=='y' || confirmation=='Y' || confirmation=='t' || confirmation=='T');
}