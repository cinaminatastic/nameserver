#ifndef SHAKELINECLASS_H_INCLUDED
#define SHAKELINECLASS_H_INCLUDED

/*
***Author: Alex Seaver.
***Purpose: Constructs a class to hold lines of Shakespeare
***Last Modified: February 1, 2017
*/

#include <iostream>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
using namespace std;

class shakeLineClass
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

#endif // SHAKELINECLASS_H_INCLUDED
