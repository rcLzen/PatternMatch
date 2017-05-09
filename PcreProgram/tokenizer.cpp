//
//  tokenizer.cpp
//

#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sys/stat.h>
#include <unistd.h>
#include <algorithm>
#include <cctype>
#include <queue>
#include "pcrecpp.h"
#include "tokenizer.h"
using namespace std;

std::queue<std::string> patternValues;
std::queue<std::string> sequenceValues;

bool validateInpudFormat(string text) {
    string match;
    string pattern ="(\\{.*?\\})";
    pcrecpp::RE sequence(pattern);
    pcrecpp::StringPiece input(text);
    bool test = sequence.FindAndConsume(&input, &match);
    return test;
}



string spaceLimitation(string& text)
{
    size_t pos = 0;
    while ((pos = text.find("S")) != string::npos) {
        text.substr(0, pos + 1);
        text.erase(0, pos + 1);
    }
    return "\\s{"+text+"}";
}
string greedy(string& text)
{
    size_t pos = 0;
    string temp;
    while ((pos = text.find("G")) != string::npos) {
        temp = text.substr(0, pos + 1);
        text.erase(0, pos + 2);
    }
    return "(.*)";
}
void tokenPatternGenerator(string token, int index, string sequence)
{
    string pattern;
    string textS;
    string textG;
    string delimeters = {' '};
    bool isSpacelimitation = false;
    bool isGreedy = false;
    string regex[3] = {"(\\w+|\\d+)", "((?:[a-z][a-z]+|\\d+))"};
    pcrecpp::StringPiece input(sequence);
    pcrecpp::RE sequenceS("\\%\\{((\\d[S]\\d|\\d+[S]\\d+))");
    
    isSpacelimitation = sequenceS.FindAndConsume(&input, &textS);
    
    pcrecpp::RE sequenceG("\\%\\{((\\d[G]|\\d+[G]))");
    isGreedy = sequenceG.FindAndConsume(&input, &textG);
    if(token.length() > 0) {
        if(index == 0 || isGreedy) {
            if(isGreedy) {
                pattern = token += greedy(textG);
                patternValues.push(pattern);
            } else {
                pattern = token += regex[0];
                patternValues.push(pattern);
            }
        }
        if(index >= 1) {
            pattern = token += regex[1];
            patternValues.push(pattern);
        }
        if(isSpacelimitation)
            patternValues.push(spaceLimitation(textS));
    }
}
bool tokenCaptureSequence(string text)
{
    string delimiter = "%";
    string temp;
    size_t pos = 0;
    string token;
    string sequence;
    int i = 0;
    size_t posDel = 0;
    while ((pos = text.find(delimiter)) != string::npos) {
        sequence = text.substr(pos, pos + 1);
        if(!validateInpudFormat(sequence))
            return false;
        token = text.substr(0, pos + 1);
        if((posDel = token.find("}")) != string::npos)
            token = token.erase(posDel, posDel + 1);
        if((posDel = token.find("%")) != string::npos)
            token = token.erase(posDel, posDel + 1);
        text.erase(0, pos + (sequence.length() - 1));
        tokenPatternGenerator(token, i, sequence);
        i++;
    }
    return true;
}
string evaluateExpression(string line, fstream file)
{
    string pattern;
    string contents;

    if(validateInpudFormat(line)) {
        if(tokenCaptureSequence(line)) {
            if(patternValues.size() > 0) {
                while(!patternValues.empty()) {
                    pattern += patternValues.front();
                    patternValues.pop();
                }
            }
            if(file.is_open()) {
                while (getline(file, contents)) {
                    
                    pcrecpp::StringPiece input(contents);
                    pcrecpp::RE re(pattern);
                    if (re.PartialMatch(input)) {
                        std::cout << input <<"\n";
                    }
                }
                file.close();
            }
            else {
                cout << "file did not open\n";
            }
        } else {
            cout << "Not a valid input!\n";
        }
    }else {
        cout << "Not a valid input!\n";
    }

}
