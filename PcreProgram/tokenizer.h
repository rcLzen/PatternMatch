//
//  tokenizer.h
//  PcreProgram
//
//  Created by Richard Lorenzen on 5/9/17.
//  Copyright © 2017 Richard Lorenzen. All rights reserved.
//

#ifndef tokenizer_h
#define tokenizer_h

class Tokenizer
{
  
public:
    std::fstream fileC;
    bool validateInpudFormat(string text);
    bool tokenCaptureSequence(string text);
    string spaceLimitation(string& text);
    string greedy(string& text);
    string evaluateExpression(string line, fstream file);
    void tokenPatternGenerator(string token, int index, string sequence);

};

#endif /* tokenizer_h */
