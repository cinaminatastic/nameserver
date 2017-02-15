/***************************************************************************
* nameservercpp  -  Program to serve of last name statistics
 *
* copyright : (C) 2009 by Jim Skon
*
* This program runs as a background server to a CGI program, providinging US Census
* Data on the frequency of names in response to requestes.  The rational for this 
* is to demonstrait how a background process can serve up information to a web program,
* saving the web program from the work of intiallizing large data structures for every
* call.
* 
* 
*
***************************************************************************/
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include <sstream>
#include "fifo.h"

using namespace std;

class shakeLineClass//class to hold the lines
{
private:
    int linePos;//line number
    string line;//text of the line
public:
    int getLinePos();//returns line number
    string getLine();//returns line text
    void setLinePos(int);//sets line number
    void setLine(string);//sets line text
    shakeLineClass(int, string);//constructor
};

/* Fifo names */
string receive_fifo = "Namerequest";
string send_fifo = "Namereply";

map<string, vector<int> > refs;//map of the text file

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

vector <int> indexSearch(string word, map<string, vector<int> > refs)
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

bool wordInLine(vector<int> wordIndex, int linepos)//determines if the search word is held in a given line
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

/* Server main line,create name MAP, wait for and serve requests */
int main() {
  
  string inMessage, outMessage,name;//name is the input word
  int pos;

  refs = buildMap(refs);//construct the map
  
  cout << "Name data loaded!" << endl;

  // create the FIFOs for communication
  Fifo recfifo(receive_fifo);
  Fifo sendfifo(send_fifo);
  
  while (1) {

    /* Get a message from a client */
    recfifo.openread();
    inMessage = recfifo.recv();
	name = inMessage;
	cout << "Message: " << name << endl;
	ifstream fin;
    fin.open("Shakespeare.txt");
	vector<int> wordIndex = indexSearch(name, refs);//searches for matches
	string tempLine;
	int linePos, count = 1;
	vector<shakeLineClass> sLC;
	while(!fin.fail())//iterates through the whole file
    {
        linePos = fin.tellg();
        getline(fin, tempLine);
        if(wordInLine(wordIndex, linePos))
        {
			string str = tempLine, whitespace = " \t";
            int strBegin = str.find_first_not_of(whitespace);
            int strEnd = str.find_last_not_of(whitespace);
            int strRange = strEnd - strBegin + 1;
            tempLine = str.substr(strBegin, strRange);//creates a copy of the line with no leading or trailing whitespace
            shakeLineClass tSLC (count, tempLine);
            sLC.push_back(tSLC);//adds the match-containg line to the vector
        }
        count++;
    }
    fin.close();

	//outMessage = "";
	sendfifo.openwrite();
	for(int i = 0; i < sLC.size(); i++)
	{
		string tempMessage = sLC[i].getLine();
		int wordSize = name.size();
		int pos = tempMessage.find(name);
        string frontLine = tempMessage.substr(0, pos);//the part of the line preceding the word
        outMessage = frontLine;
		outMessage += "<b>";//starts bold
		outMessage += name;
		outMessage += "</b>";//ends bold
		string backLine = tempMessage.substr(pos+name.size(), tempMessage.size());//the part of the line following the word
		outMessage += backLine;
		outMessage += "</br>";//line break
		cout << " Results: " << outMessage << endl;
		sendfifo.send(outMessage);//sends final message
	}
	sendfifo.send("$END");
	sendfifo.fifoclose();
	recfifo.fifoclose();

  }
}

int shakeLineClass::getLinePos()
{
    return linePos;
}

string shakeLineClass::getLine()
{
    return line;
}

void shakeLineClass::setLinePos(int newLinePos)
{
    linePos = newLinePos;
}

void shakeLineClass::setLine(string newLine)
{
    line = newLine;
}

shakeLineClass::shakeLineClass(int newLinePos, string newLine)
{
    linePos = newLinePos;
    line = newLine;
}