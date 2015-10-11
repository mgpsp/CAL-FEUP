#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <algorithm>
#include <stdlib.h>
#include <stdio.h>
#include <cmath>
#include <sstream>
#include <cctype>

using namespace std;

struct Suggestion {
	string word;
	int line;
	int distance;

	Suggestion(string word, int line, int distance) : word(word), line(line), distance(distance) {};
	bool operator<(const Suggestion &s) const { return distance < s.distance; };
};

void loadDictionary(string fileName, vector<string> &dictionary)
{
	ifstream file(fileName.c_str());
	string line;

	if (file.is_open())
	{
		while (getline(file, line))
			dictionary.push_back(line);
	}

	file.close();
}

int editDistance(string pattern, string text)
{
	vector<vector<int> > D(pattern.length() + 1, vector<int>(text.length() + 1));
	int niw;

	for (size_t i = 0; i <= pattern.length(); i++)
		D[i][0] = i;

	for (size_t j = 0; j <= text.length(); j++)
		D[0][j] = j;

	for (size_t i = 1; i <= pattern.length(); i++)
	{
		for (size_t j = 1; j <= text.length(); j++)
		{
			if (pattern[i - 1] == text[j - 1])
				D[i][j] = D[i - 1][j - 1];
			else if (i > 1 && j > 1 && pattern[i - 2] == text[j - 1] && pattern[i - 1] == text[j - 2])
			{
				niw = min(D[i - 2][j - 2], D[i - 1][j]);
				niw = min(niw, D[i][j - 1]);
				niw = min(niw, D[i - 1][j - 1]);
				niw += 1;
				D[i][j] = niw;
			}
			else
			{
				niw = min(D[i - 1][j - 1], D[i - 1][j]);
				niw = min(niw, D[i][j - 1]);
				niw += 1;
				D[i][j] = niw;
			}
		}
	}
	return D[pattern.length()][text.length()];
}

bool find(string word, const vector<string> &dictionary)
{
	size_t i = 0;

	string wordL = word;
	transform(wordL.begin(), wordL.end(), wordL.begin(), ::tolower);
	while (i < dictionary.size() && dictionary[i] != word && dictionary[i] != wordL)
		i++;

	if (i == dictionary.size())
		return false;
	else
		return true;
}

string removePunctuation(string word)
{
	int l = word.length();
	for (int i = 0; i < l; i++)
	{
		if (ispunct(word[i]) && word[i] != '\'' && word[i] != '-')
		{
			word.erase(i--, 1);
			l = word.length();
		}
	}

	return word;

}

void spellChecker(string fileName, const vector<string> &dictionary, vector<vector<Suggestion> > &suggestions)
{
	ifstream file(fileName.c_str());
	string line;
	int lineNr = 0;

	if (file.is_open())
	{
		while (getline(file, line))
		{
			stringstream ss(line);
			string word;
			lineNr++;
			while (ss >> word)
			{
				int isNumber = strtol(word.c_str(), NULL, 0);
				if (!isNumber && word != "-")
				{
					string tmp = word;
					word = removePunctuation(word);
					int maxDistance = word.length() * 0.45;
					if (maxDistance > 5)
						maxDistance = 5;
					if (!find(word, dictionary))
					{
						vector<Suggestion> words;
						words.push_back(Suggestion(word, lineNr, 0));
						for (size_t i = 0; i < dictionary.size(); i++)
						{
							if (abs((word.length() - dictionary[i].length())) < maxDistance)
							{
								int d = editDistance(word, dictionary[i]);
								if (d < maxDistance)
									words.push_back(Suggestion(dictionary[i], lineNr, d));
							}
						}
						sort(words.begin() + 1, words.end());
						suggestions.push_back(words);
					}
				}
			}
		}
		file.close();
	}
	else
	{
		cout << endl;
		cout << "\tFile doesn't exist." << endl;
	}
}


int main()
{
	cout << endl;
	cout << "\tDictionary:" << endl;
	cout << endl;
	cout << "\t   1. English" << endl;
	cout << "\t   2. Dutch" << endl;
	cout << endl;
	cout << "\t   > ";
	int op;
	cin >> op;

	while (op < 1 || op > 2)
	{
		cin.clear();
		cin.ignore(100, '\n');
		cout << "\t   > ";
		cin >> op;
	}

	vector<string> dictionary;
	switch(op)
	{
	case 1:
		loadDictionary("english.txt", dictionary);
		break;
	case 2:
		loadDictionary("dutch.txt", dictionary);
		break;
	default:
		break;
	}

	string fileName;
	cout << endl;
	cout << "\tFile to check: ";
	cin.ignore(100, '\n');
	getline(cin, fileName);

	cout << endl;
	cout << "\tChecking spelling..." << endl;
	vector<vector <Suggestion> > suggestions;
	spellChecker(fileName, dictionary, suggestions);
	cout << endl;
	cout << "\tFound " << suggestions.size() << " errors." << endl;
	cout << endl;
	for (size_t i = 0; i < suggestions.size(); i++)
	{
		for (size_t j = 0; j < suggestions[i].size(); j++)
		{
			if (j == 0)
			{
				cout << "\tWord: " << suggestions[i][j].word << " (l. " << suggestions[i][j].line << ")" << endl;
				if (suggestions[i].size() > 1)
					cout << "\tSuggestions:" << endl;
				else
					cout << "\tNo suggestions." << endl;
			}
			else
				cout << "\t   " << suggestions[i][j].word << endl;
		}
		cout << endl;
	}
	return 0;
}
