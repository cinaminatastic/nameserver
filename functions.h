#ifndef FUNCTIONS_H_INCLUDED
#define FUNCTIONS_H_INCLUDED

/*
***Authors: Alex Seaver, Jim Skon, Identicon on StackOverflow.
***Purpose: Constructs a map of the complete works of William Shakespeare and searches for a given word.
***Last Modified: February 1, 2017
*/

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <dos.h>
#include <dir.h>
#include <algorithm>
#include "shakeLineClass.h"
using namespace std;

vector <int> upperIndexSearch(string word, map<string, vector<int> > refs);//searches for the word with the first letter capitalized (used if the word is at the beginning of the line)
vector <int> lowerIndexSearch(string word, map<string, vector<int> > refs);//searches for the word with the first letter lowercase
bool wordInLine(vector<int> wordIndex, int linePos);//determines if the desired word is present in a line
map<string, vector<int> > buildMap (map<string, vector<int> > refs);//builds the map of the complete works of William Shakespeare
string input();//asks the user for the desired word
void output(string, map<string, vector<int> >);//prints the list of lines to the screen
void SetColor(int ForgC); // Authored by Identicon on StackOverflow http://stackoverflow.com/a/29578507;

vector <int> upperIndexSearch(string word, map<string, vector<int> > refs)
{
     map<string, vector<int> >::iterator it;
     map<string, vector<int> >::iterator it2;  // iterator for find
     vector<int> blank; // return for no matches
    /* find the word and get the vector of references */
    /* First use find, so as to NOT create a new entry */
    word[0] = toupper(word[0]);
    it = refs.find(word);
    if (it == refs.end())
    {
         return(blank);
    }
    else
    {
        return (refs[word]);
    }
}

vector <int> lowerIndexSearch(string word, map<string, vector<int> > refs)
{
     map<string, vector<int> >::iterator it;  // iterator for find
     vector<int> blank; // return for no matches
    /* find the word and get the vector of references */
    /* First use find, so as to NOT create a new entry */
    word[0] = tolower(word[0]);
    it = refs.find(word);
    if (it == refs.end())
    {
         return(blank);
    }
    else
    {
        return (refs[word]);
    }
}

bool wordInLine(vector<int> wordIndex, int linepos)
{
    int count = 0;
    for (int i = 0; i <= wordIndex.size(); i++)
    {
        if(linepos == wordIndex[i])
            {
                count++;
            }
    }
    if (count > 0)
    {
        return true;
    }
    return false;
}

map<string, vector<int> > buildMap (map<string, vector<int> > refs)
{
    string word, line;
	ifstream fin("Shakespeare.txt");// open the file

	if (fin.fail())
    {
        cout << "Loading of the text file failed. Exiting\n";
        exit(0);
    }

	int position = 0;

	while (!fin.fail())
    {
		getline(fin, line); // Get the next line of code
		istringstream lineStream(line); // Create a string stream of the line
		while (lineStream >> word) // Get the next word
		{
			refs[word].push_back(position); //Push the word and the line position on the vector for this word
		}
		position = fin.tellg(); // Get the position of the next line
	}

	fin.close();
	return refs;
}

string input()
{
    string inWord;
    cout << "This program searches for a chosen word within the complete works of \nWilliam Shakespeare and returns every line containing the chosen word.\n";
    cout << "What is the word you would like to search for?\nWord: ";
    cin >> inWord;
    cin.clear();
    return inWord;
}

void output(string inWord, map<string, vector<int> > refs)
{
    ifstream fin;
    fin.open("Shakespeare.txt");
    int x = lowerIndexSearch(inWord, refs).size();
    int y = upperIndexSearch(inWord, refs).size();
    if(x == 0 && y == 0)
    {
        cout << "Word not found. Exiting.\n";
        exit(0);
    }
    vector<int> wordIndex = upperIndexSearch(inWord, refs);
	string tempLine;
	int linePos, count = 1;
	vector<shakeLineClass> sLC;
	while(!fin.fail())
    {
        linePos = fin.tellg();
        getline(fin, tempLine);
        if(wordInLine(wordIndex, linePos))
        {
            shakeLineClass tSLC (count, tempLine);
            sLC.push_back(tSLC);
        }
        count++;
    }
    fin.close();
    fin.open("Shakespeare.txt");
    wordIndex = lowerIndexSearch(inWord, refs);
    count = 1;
	while(!fin.fail())
    {
        linePos = fin.tellg();
        getline(fin, tempLine);
        if(wordInLine(wordIndex, linePos))
        {
            shakeLineClass tSLC (count, tempLine);
            sLC.push_back(tSLC);
        }
        count++;
    }
    for(int i = 0; i < sLC.size(); i++)
    {
        string outLine = sLC[i].getLine();
        int wordSize = inWord.size();
        int pos = outLine.find(inWord);
        SetColor(15);
        cout << "Line " << sLC[i].getLinePos() << ": ";
        if(pos != -1)
        {
            string frontLine = outLine.substr(0, pos);
            cout << frontLine;
        }
        else
        {
            inWord[0] = toupper(inWord[0]);
            pos = outLine.find(inWord);
            string frontLine = outLine.substr(0, pos);
            cout << frontLine;
        }
        SetColor(14);
        cout << inWord;
        SetColor(15);
        string backLine = outLine.substr(pos+inWord.size(), outLine.size());
        cout << backLine;
        cout << endl;
        inWord[0] = tolower(inWord[0]);
        pos = outLine.find(inWord);
    }
}

void SetColor(int ForgC)
{
    WORD wColor;

    HANDLE hStdOut = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi; //We use csbi for the wAttributes word.
    if(GetConsoleScreenBufferInfo(hStdOut, &csbi)) //Mask out all but the background attribute, and add in the forgournd color
    {
        wColor = (csbi.wAttributes & 0xF0) + (ForgC & 0x0F);
        SetConsoleTextAttribute(hStdOut, wColor);
    }
    return;
}

#endif // FUNCTIONS_H_INCLUDED
