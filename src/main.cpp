#include <iostream>
#include <string>
#include "./value.h"
#include "./tokenizer.h"
#include "./token.h"
#include "./error.h"
#include "./parser.h"
#include "./eval_env.h"
#include "rjsj_test.hpp"
#include <fstream>
#include <windows.h>
#include <algorithm>
#include <cctype>
using ValuePtr = std::shared_ptr<Value>;


// 函数用于设置控制台光标的位置
void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

// 函数用于设置控制台文本颜色和样式
void setConsoleTextColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}

// 函数用于清空整个控制台的输出
void clearConsole() {
    COORD coord = {0, 0};
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsole, &screenBufferInfo);
    DWORD consoleSize = screenBufferInfo.dwSize.X * screenBufferInfo.dwSize.Y;
    DWORD charsWritten;
    FillConsoleOutputCharacter(hConsole, ' ', consoleSize, coord, &charsWritten);
    FillConsoleOutputAttribute(hConsole, screenBufferInfo.wAttributes, consoleSize, coord, &charsWritten);
    setCursorPosition(0, 0);
}
struct TestCtx {
    //define envptr
    std::shared_ptr<EvalEnv> envptr = std::shared_ptr<EvalEnv>(new EvalEnv());
    std::string eval(std::string input) {
        auto tokens = Tokenizer::tokenize(input);
        Parser parser(std::move(tokens));
        auto value = parser.parse();
        auto result = envptr->eval(std::move(value));
        return result->toString();
    }
};



bool isAllSpaces(const std::string& str) {
    return std::all_of(str.begin(), str.end(), [](unsigned char c) {
        return std::isspace(c);
    });
}


COORD getCursorPosition() {
    CONSOLE_SCREEN_BUFFER_INFO info;
    HANDLE hConsoleOutput = GetStdHandle(STD_OUTPUT_HANDLE);
    GetConsoleScreenBufferInfo(hConsoleOutput, &info);
    return info.dwCursorPosition;
}

bool ismatchparenthesis(std::string & str)
{
    int left=0;
    int right=0;
    for(int i=0;i<str.size();i++)
    {
        // omit the ()in quote
        if(str[i]=='\"')
        {
            i++;
            while(str[i]!='\"')
            {
                i++;
            }
        }
        
        if(str[i]=='(')
        {
            left++;
        }
        else if(str[i]==')')
        {
            right++;
        }
    }
    if(left==right)
    {
        return true;
    }
    else
    {
        return false;
    }
}

int main(int argc,char** argv) {

   
    if (argc<2)
    {
        //RJSJ_TEST(TestCtx, Lv2, Lv3, Lv4, Lv5,Lv5Extra, Lv6,Lv7,Lv7Lib);
        clearConsole();
        setConsoleTextColor( FOREGROUND_GREEN | FOREGROUND_BLUE);
        std::cout << R"(                                                                 ___                                     )" << std::endl;
        std::cout << R"(\    / _  |  _  _  ._ _   _   _|_  _   _|_ |_   _   |  o  _ ._    |  ._ _|_  _  ._ ._  ._ _ _|_  _  ._ | )" << std::endl;
        std::cout << R"(\/\/ (/_ | (_ (_) | | | (/_   |_ (_)   |_ | | (/_  |_ | _> |_)  _|_ | | |_ (/_ |  |_) | (/_ |_ (/_ |  o )" << std::endl;
        std::cout << R"(                                                           |                      |                     )" << std::endl;
        setConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

  
    auto envptr = std::shared_ptr<EvalEnv>(new EvalEnv());
  

    while (true) {
        try {
         
          
            std::cout << ">>> " ;
            setCursorPosition(4, getCursorPosition().Y);
    
            std::string line;
            setConsoleTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
            std::getline(std::cin, line);
            setConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
            if (std::cin.eof()) {
                std::exit(0);
            }
            
            
            if (line == ""||line=="\n" || line=="\r\n"||line=="\r"||line=="\t"||line==" "||isAllSpaces(line)) {
                continue;
            }
        
            auto tokens = Tokenizer::tokenize(line);
            
            while (!ismatchparenthesis(line)) {
                std::cout << "... ";
              
                auto newline = std::string();
                setConsoleTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);
                std::getline(std::cin, newline);
                setConsoleTextColor(FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
                line += newline;
               
                if (std::cin.eof()) {
                    std::exit(0);
                }
                if (newline == "") {
                    continue;
                }
                if (newline == ""||newline=="\n"||newline=="\r\n"||newline=="\r"||newline=="\t"||newline==" "||isAllSpaces(newline)) {
                    continue;
                }
                auto tokens2 = Tokenizer::tokenize(newline);
                for (auto& token : tokens2) {
                    tokens.push_back(std::move(token));
                }
               
             

               
            }
     
            Parser parser(std::move(tokens));
            auto value = parser.parse();
           
            auto result = envptr->eval(std::move(value));
          
            std::cout << result->toString() << std::endl;
            
          

        } catch (std::runtime_error& e) {
        
            std::cout <<"Error:"<< e.what() << std::endl;
        }
    }
}
else
{
    
    
    try
    {
        std::ifstream istrm(argv[1], std::ios::binary);
       
        if (!istrm)
        {
            throw std::runtime_error("no such file");
        }
        std::string line;
        auto envptr = std::shared_ptr<EvalEnv>(new EvalEnv());
        while (std::getline(istrm, line))
        {
        
            if (line == ""||line=="\n" || line=="\r\n"||line=="\r"||line=="\t"||line==" "||isAllSpaces(line)) {
                continue;
            }
          
            auto tokens = Tokenizer::tokenize(line);

            while (std::count(line.begin(), line.end(), '(') != std::count(line.begin(), line.end(), ')')) {
                auto newline = std::string();
                std::getline(istrm, newline);
              
                line += newline;
                if (newline == ""||newline=="\n"||newline=="\r\n"||newline=="\r"||newline=="\t"||newline==" "||isAllSpaces(newline)) {
                    continue;
                }
                auto tokens2 = Tokenizer::tokenize(newline);
                for (auto& token : tokens2) {
                    tokens.push_back(std::move(token));
                }
            }
            Parser parser(std::move(tokens));
            auto value = parser.parse();
            auto result = envptr->eval(std::move(value));
         
        }
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    
}
   
 
}