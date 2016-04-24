// lab1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <bitset>
#include <fstream>
#include <vector>

using namespace std;

/*! A class containing variables and methods to generate the decryption database */
class decryptionDatabase {
	private:
		string characters[46] = { "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "'", "@", ":", ",", "$", "=", "!", ".", "?", "\"" };
		string dotDashBinary[46] = { "1001", "01101010", "01100110", "011010", "10", "10100110", "010110", "10101010", "1010", "10010101", "011001", "10011010", "0101", "0110", "010101", "10010110", "01011001", "100110", "101010", "01", "101001", "10101001", "100101", "01101001", "01100101", "01011010", "0101010101", "1001010101", "1010010101", "1010100101", "1010101001", "1010101010", "0110101010", "0101101010", "0101011010", "0101010110", "100101010110", "100101100110", "010101101010", "010110100101", "10101001101001", "0110101001", "011001100101", "100110011001", "101001011010", "100110100110" };
		int dotDashDecimal[46];
	public:
		decryptionDatabase() {
			for (int i = 0; i < 46; i++) {
				bitset<16> bs(dotDashBinary[i]);
				dotDashDecimal[i] = bs.to_ulong();
			}
		}
		string getCorrespondingValue(int key);
};

/**
* Get corresponding value for a key
*
* @param key int variable which points to a certain character in the database
* @return the value which the key points to
*/
string decryptionDatabase::getCorrespondingValue(int key) {
	for (int i = 0; i < 46; i++) {
		if (key == dotDashDecimal[i]) {
			return characters[i];
		}
	}
	return "";
}

/*! A class with variables and methods to decrypt the code and output the decrypted string  */
class decryptionMethod {
	private:
		string toDecrypt;
		string toDecryptBinary;
		string decrypted;
	public:
		void input(string filepath);
		string decrypt(decryptionDatabase data);
};

/**
* Function that handles the file input
*
* @param filepath binary file that will be decrypted
*/
void decryptionMethod::input(string filepath) {
	ifstream fin(filepath, ios::binary);
	if (fin.fail()) {
		cout << "Input file failed to open." << endl;
		exit(1);
	}
	while (!fin.eof()) {
		fin >> toDecrypt;
	}
}             

/**
* Decryption algorithm
*
* @param data database object that will be used as a lookup table
* @return decrypted string
*/
string decryptionMethod::decrypt(decryptionDatabase data) {
	string temp;
	vector<string> vec;
	//convert the content of the binary file to 0 and 1 string format
	for (size_t i = 0; i < toDecrypt.length(); i++) {
		bitset<8> bs(toDecrypt[i]);
		toDecryptBinary += bs.to_string();
	}
	//break the 0 and 1 string into two digits and put each one in a position in a vector
	for (size_t i = 0; i < toDecryptBinary.length(); i++) {
		temp += toDecryptBinary[i];
		if (temp.length() >= 2) {
			vec.push_back(temp);
			temp = "";
		}
	}
	string temp1;
	int key;
	//process the vector, 00 is end of character, 11 is end of word
	for (size_t i = 0; i < vec.size(); i++) {
		if (vec[i] != "00" && vec[i] != "11") {
			temp1 += vec[i];
		} 
		else if (vec[i] == "00") {
			bitset<16> bs(temp1);
			key = bs.to_ulong();
			decrypted += data.getCorrespondingValue(key);
			temp1 = "";
		}
		else if (vec[i] == "11") {
			decrypted += " ";
			bitset<16> bs(temp1);
			key = bs.to_ulong();
			decrypted += data.getCorrespondingValue(key);
			temp1 = "";
		}
	}
	return decrypted;
}

/**
* Enable debugging in Visual Studio
*/
void enableDebug(bool bvalue)
{
	if (!bvalue) return;
	int tmpFlag = _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG);
	// Turn on leak-checking bit.
	tmpFlag |= _CRTDBG_LEAK_CHECK_DF;
	// Turn off CRT block checking bit.
	tmpFlag &= ~_CRTDBG_CHECK_CRT_DF;
	// Set flag to the new value.
	_CrtSetDbgFlag(tmpFlag);
}

int main(){
	enableDebug(true);
	decryptionDatabase data;
	decryptionMethod cipher;
	cipher.input("Morse.bin");
	string result = cipher.decrypt(data);
	cout << result << endl;
}

