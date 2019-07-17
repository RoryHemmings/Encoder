/*
	Encoder v1.2
	C++
	Author: Rory Hemmings

	Arguments:
		-h, --help: displays help menu
		-f <filepath>: feeds program whatever is in the file indicated by the path
		-o <filepath>: writes program output to file indicated by path
		-s: puts spaces in output (if converting to binary)
		
		Combine any of these two as an argument:
		i: image (can work for any file if you desire to convert raw binary data)
		t: ascii
		b: binary
		b64: base64 (newline not accepted)
		ex. <option1><option2>: converts from option1 as input to option 2 as output

		ex. encode b64t <base64text>: converts base64 input to ascii output
		ex. encode tb <ascii text> -f "input.txt": reads ascii text from "input.txt" and prints it out on the screen
		ex. encode tb <ascii text> -f "input.txt" -o "output.txt": reads ascii from "input.txt" and writes output to "output.txt"
		ex. encode ib64 <image path> -o "output.txt": reads image data from image path provided and outputs base64 into "output.txt" (Note, don't use -f for image file input)
		ex. encode b64i <b64 text> -o <output path>: writes base64 input into file specified by -o path (output.png by default)
		ex. encode b64i -f <b64input path> -o <output path>: writes base64 input into file specified by -o path with input from -f path

*/

#include <string>
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <fstream>
#include <vector>

using namespace std;

const char* HELP = "Encoder v1.2\nC++\nAuthor: Rory Hemmings\nArguments :\n\t-h, --help : displays help menu\n\t-f <filepath> : feeds program whatever is in the file indicated by the path\n\t-o <filepath> : writes program output to file indicated by path\n\t-s : puts spaces in output(if converting to binary)\nCombine any of these two as an argument :\n\ti: image(can work for any file if you desire to convert raw binary data)\n\tt : ascii\n\tb : binary\n\tb64 : base64(newline not accepted)\n\tex. <option1><option2> : converts from option1 as input to option 2 as output\n\tex.encode b64t <base64text> : converts base64 input to ascii output\n\tex.encode tb <ascii text> -f \"input.txt\" : reads ascii text from \"input.txt\" and prints it out on the screen\n\tex.encode tb <ascii text> -f \"input.txt\" - o \"output.txt\" : reads ascii from \"input.txt\" and writes output to \"output.txt\"\n\tex.encode ib64 <image path> -o \"output.txt\" : reads image data from image path provided and outputs base64 into \"output.txt\" (Note, don't use -f for image file input)\n\tex. encode b64i <b64 text> -o <output path>: writes base64 input into file specified by -o path (output.png by default)\n\tex. encode b64i -f <b64input path> -o <output path>: writes base64 input into file specified by -o path with input from -f path";

const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

bool is_base64(unsigned const char c) {		// Tests if character is a base64 character
	return (isalnum(c) || c == '+' || c == '/');
}

int indexof(const char* array, size_t size, const char c)	// Makeshift indexof function for character array
{
	const char* end = array + size;
	const char* match = std::find(array, end, c);
	return (end == match) ? -1 : (match - array);
}

// Takes string input as string
string base64_encode(unsigned const char* input, unsigned const int len) {
	string ret;
	size_t i = 0;
	unsigned char bytes[3];
	unsigned char sextets[4];

	while (i + 3 <= len) {
		bytes[0] = *(input++);
		bytes[1] = *(input++);
		bytes[2] = *(input++);

		sextets[0] = (bytes[0] & 0xfc) >> 2;	// Cuts last two bits off of first byte
		sextets[1] = ((bytes[0] & 0x03) << 4) + ((bytes[1] & 0xf0) >> 4);	// Takes last two bits from first byte and adds it to first 4 bits of 2nd byte
		sextets[2] = ((bytes[1] & 0x0f) << 2) + ((bytes[2] & 0xc0) >> 6);	// Takes last 4 bits of 2nd byte and adds it to first 2 bits of third byte
		sextets[3] = bytes[2] & 0x3f;	// takes last 6 bits of third byte

		for (size_t j = 0; j < 4; ++j) {
			ret += base64_chars[sextets[j]];
		}

		i += 3;	// increases to go to third byte
	}

	if (i != len) {
		size_t k = 0;
		size_t j = len - i;	// Find index of last byte
		while (k < j) {		// Sets first bytes
			bytes[k] = *(input++);
			++k;
		}

		while (j < 3) {		// Set last bytes to 0x00
			bytes[j] = '\0';
			++j;
		}

		sextets[0] = (bytes[0] & 0xfc) >> 2;	// Cuts last two bits off of first byte
		sextets[1] = ((bytes[0] & 0x03) << 4) + ((bytes[1] & 0xf0) >> 4);	// Takes last two bits from first byte and adds it to first 4 bits of 2nd byte
		sextets[2] = ((bytes[1] & 0x0f) << 2) + ((bytes[2] & 0xc0) >> 6);	// Takes last 4 bits of 2nd byte and adds it to first 2 bits of third byte
		// No last one is needed, because if there were 4, then (i == len) == true
		
		for (j = 0; j < (len - i) + 1; ++j) {	// Gets sextets that include data
			ret += base64_chars[sextets[j]];	// Appends them to string
		}

		while ((j++) < 4)	// Appends remaining ='s
			ret += '=';

	}

	return ret;

}

string base64_decode(const string& input) {
	string ret;
	size_t i = 0;
	unsigned char bytes[3];
	unsigned char sextets[4];

	while (i < input.size() && input[i] != '=') {
		size_t j = i % 4;	// index per sextet
		if (is_base64(input[i])) sextets[j] = input[i++];		// set sextets with characters from string
		else { cerr << "Non base64 string included in input (possibly newline)" << endl; return ""; }
		if (i % 4 == 0) {
			for (j = 0; j < 4; ++j) 			// Using j as a seperate index (not the same as it was originally used as, will later be reset)
				sextets[j] = indexof(base64_chars, strlen(base64_chars), sextets[j]);	// Change value to indicies of b64 characters and not ascii characters

			bytes[0] = (sextets[0] << 2) + ((sextets[1] & 0x30) >> 4);	// Similar bitshifting to before
			bytes[1] = ((sextets[1] & 0x0f) << 4) + ((sextets[2] & 0x3c) >> 2);
			bytes[2] = ((sextets[2] & 0x03) << 6) + sextets[3];

			for (j = 0; j < 3; ++j)		// Using j seperately again to iterate through bytes and adding them to full string
				ret += bytes[j];
		}
	}

	if (i % 4 != 0) {
		for (size_t j = 0; j < (i % 4); ++j)
			sextets[j] = indexof(base64_chars, strlen(base64_chars), sextets[j]);
		
		bytes[0] = (sextets[0] << 2) + ((sextets[1] & 0x30) >> 4);	// Similar bitshifting to before
		bytes[1] = ((sextets[1] & 0x0f) << 4) + ((sextets[2] & 0x3c) >> 2);
		for (size_t j = 0; j < (i % 4) - 1; ++j)
			ret += bytes[j];		// Add final bytes
	}
	return ret;
}

// It literally took me like 12 hours to write this function
string base64_encode_image(const string& path) {
	vector<char> temp;

	std::ifstream infile;
	infile.open(path, ios::binary);		// Open file in binary mode
	if (infile.is_open()) {
		while (!infile.eof()) {
			char c = (char)infile.get();
			temp.push_back(c);
		}
		infile.close();
	}
	else return "File could not be opened";
	string ret(temp.begin(), temp.end() - 1);	// Idk why subtracting 1 works it just does (its probably because the eof is actually not supposed to be included in the file, its just there
												// to let you know that the file is ending
	ret = base64_encode((unsigned const char*)ret.c_str(), ret.size());

	return ret;
}

// You thought the last function took a long time to write but then you saw this one which took about 50 hours to write
int base64_decode_image(const string& input, const string& path) {
	ofstream outfile;
	outfile.open(path, ofstream::binary);	// This line specifically

	string temp = base64_decode(input);
	
	if (outfile.is_open()) outfile.write(temp.c_str(), temp.size());
	else { cerr << "File can't be opened" << endl; return 1; }

	outfile.close();
	return 0;
}

bool not_bin(char c) {
	return !(c == '1' || c == '0' || c == ' ');
}

void remove_spaces(string& s) {		// Recursively removes spaces
	string::iterator it = find(s.begin(), s.end(), ' ');
	if (it == s.end()) return;
	s.erase(it);
	remove_spaces(s);
}

string binary_encode(const string& s, bool spaces = false) {
	string ret;
	for (string::size_type i = 0; i <= s.size() - 1; ++i) {
		string bin;
		int val = int(s[i]);

		while (val > 0)
		{
			(val % 2) ? bin += '1' :
				bin += '0';
			val /= 2;
		}
		reverse(bin.begin(), bin.end());
		while (bin.size() < 8) bin.insert(0, 1, '0');
		if (spaces) bin += " ";
		ret += bin;
	}
	return ret;
}

string binary_decode(string& s) {	// More efficient to change the input than to keep it const because it is never reused
	string ret;

	if (find_if(s.begin(), s.end(), not_bin) != s.end()) return "Non binary character is present";
	remove_spaces(s);

	int j = 0;
	while (j < s.size()) {
		string::size_type i = 0;
		vector<int> temp;
		while (i < 8) {
			temp.push_back(s[j] == '1' ? 1 : 0);
			++i; ++j;
		}
		reverse(temp.begin(), temp.end());
		unsigned char x = 0;
		for (vector<int>::size_type k = 0; k < temp.size(); ++k) {
			x += temp[k] * pow(2, k);
		}
		ret += x;
	}

	return ret;
}

string getInputFromTextFile(const string& path) {
	string ret;
	string s;
	ifstream in(path);
	
	while (getline(in, s))
		ret += s;

	return ret;
}

int main(int argc, char** argv) {
	string operation = "";
	string input = "";
	string file_input = "";
	string file_output = "";
	bool spaces = false;

	string output = "";
	if (argc > 0) {
		for (size_t i = 1; i < argc; ++i) {
			if (string(argv[i]) == "tb64" || string(argv[i]) == "b64t" || string(argv[i]) == "ib64" || string(argv[i]) == "b64i" || string(argv[i]) == "tb" || string(argv[i]) == "bt") { operation = argv[i]; input = argv[i + 1]; }
			else if (string(argv[i]) == "-s") spaces = true;
			else if (string(argv[i]) == "-f") file_input = getInputFromTextFile(argv[i + 1]);
			else if (string(argv[i]) == "-o") file_output = argv[i + 1];
			else if (string(argv[i]) == "-h" || string(argv[i]) == "--help") { printf("%s\n", HELP); return 0; }
		}
	}
	if (file_input.size() != 0) input = file_input;

	if (operation == "tb64") output = base64_encode((const unsigned char*)input.c_str(), input.size());
	else if (operation == "b64t") output = base64_decode(input);
	else if (operation == "ib64") output = base64_encode_image(input);
	else if (operation == "b64i") { file_output.size() != 0 ? base64_decode_image(input, file_output) : base64_decode_image(input, "output.png"); return 0; }
	else if (operation == "tb") output = binary_encode(input, spaces);
	else if (operation == "bt") output = binary_decode(input);

	if (file_output.size() == 0)
		printf("%s\n", output.c_str());
	else {
		ofstream out;
		out.open(file_output);
		out << output << endl;
		out.close();
	}

	return 0;
}

// VGhpcyBtaWdodCBiZSBpbXBvcnRhbnQgbGF0ZXI=
