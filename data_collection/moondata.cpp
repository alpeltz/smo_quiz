#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
using namespace std;

//USAGE: ./exe file.tsv > output.json
//this is formatted to work with output redirection, will rewrite to work with file writing
int main(int argc,char** argv) {
	fstream fin;
	fin.open(argv[1]);
	if (!fin.is_open()) {
		cout << "Cannot open file." << endl;
		return 1;

	}
	
	cout << "[\n";
	vector<string> row;
	string temp;
	getline(fin, temp);//we want to ignore the first row (column headings)
	string line;
	while (getline(fin, line)) {
		row.clear();
		stringstream s(line);
		string word;
		while (getline(s, word, '\t')) {
			row.push_back(word);
		}
		cout << "  {\n";
		cout << "    \"id\": \"" << row[0] << "\",\n";
		cout << "    \"name\": \"" << row[1] << "\",\n";
		cout << "    \"kingdom\": \"" << row[2] << "\"";
		if (row[3] == "1") {
			cout << ",\n";
			cout << "    \"any_percent\": true";
		}
		if (row[4] == "1") {
			cout << ",\n";
			cout << "    \"dark\": true";
		}
		if (row[5] == "1") {
			cout << ",\n";
			cout << "    \"darker\": true";
		}
		if (row[6] == "1") {
			cout << ",\n";
			cout << "    \"post_game\": true";
		}
		else {
			cout << ",\n";
			cout << "    \"post_game\": false";
		}
		if (row[7] == "none") {
			cout << ",\n";
			cout << "    \"missing_image\": true";
		}
		cout << "\n";
		
		cout << "  },\n";
	}
	cout << "]";

}
