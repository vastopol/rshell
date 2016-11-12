#include "../header/interpreter.h"
#include <sstream>
#include <cstring> //library for strtok
#include <iostream> 
#include <cstdlib>
#include <stack>

Node* Interpreter::parse(std::string s)
{
    // REMOVE_COMMENTS
    //================================================================
    unsigned int pos = s.find("#"); // location of '#'
    
    if(pos != std::string::npos)    // if the position returned exists in the string
    {
        s = s.substr(0, pos);       // set it equal to a substring of itself to the position
    }
    //=================================================================
    
    // check if ballanced [] && ()
    //================================================================
    if(!isBalanced(s))
    {
        std::cout << "ERROR: not ballanced () OR []" << std::endl;
        exit(1);
    }
    //================================================================
    
    
    ///****************** HEY HEY HEY *****************************************************
    
    std::vector<std::string> str_vec;
    std::istringstream iss(s);
    std::string temp;
    int lefnum = 0;
    int rightnum = 0;
    std::string holder;
    
    while(iss >> temp)
    {
        if(temp.find("(") != std::string::npos)
        {
            for(unsigned i = 0; i < temp.size(); i++)
            {
                if(temp.at(i) == '(')
                {
                    lefnum++;
                }
                else if(temp.at(i) == ')')
                {
                    rightnum++;
                }
            }
            std::string ntemp;
            temp += " ";
            while(iss >> ntemp)
            {
                for(unsigned i = 0; i < ntemp.size(); i++)
                {
                    if(ntemp.at(i) == '(')
                    {
                        lefnum++;
                    }
                    else if(ntemp.at(i) == ')')
                    {
                        rightnum++;
                    }
                }
                temp += ntemp;
                if(lefnum != rightnum)
                {
                    temp = temp + " ";
                }
                else
                {
                    break;
                }
            }
            //temp.erase(temp.begin() + temp.find('(')); COME BACK HERE IF YOU FUCK UP
            //temp.erase(temp.begin() + temp.find_last_of(')'));
            lefnum = 0;
            rightnum = 0;
        }
        
        str_vec.push_back(temp);
    }

    // str_vec = to the return val from new parse function
    // parse each string in the vector individually
    
    ///***********************************************************************
    
    
    // HANDLE_SEMICOLON
    //==================================================================
    for(unsigned i = 0; i < str_vec.size(); i++)
    {
        // last is semicolon
        if( (str_vec.at(i)).at(str_vec.at(i).size() -1) == ';' )
        {
            std::string temp = ";";
            std::vector<std::string>::iterator it = str_vec.begin() + (i + 1); // iterator to next past "x;"
            str_vec.at(i) = str_vec.at(i).substr(0, str_vec.at(i).size() - 1); // remove ';' from string
            str_vec.insert( it , temp ); // insert new ';' string
            i++; // prevent infinte loop
        }
    }    
    //==================================================================
    
    
    // REJOIN COMMANDS TO THEIR ARGS && SEPARATE WITH CONNECTORS
    //==================================================================
    std::vector<Object*> final_form; // vector of Object pointers, commands joined with their arguments separated by connectors
    std::string tempString;          // for each iteration, puts together command and args and pushes
    
    // WW 
    for(unsigned i = 0; i < str_vec.size(); i++)
    {
        bool semiBool = (str_vec.at(i) == ";"); //checks if the current string is a connector
        bool andBool = (str_vec.at(i) == "&&");
        bool orBool = (str_vec.at(i) == "||");
        bool blockBool = (str_vec.at(i).find("(") != std::string::npos);
        
        // XX
        if(semiBool || andBool || orBool || blockBool) 
        {
            final_form.push_back(new Leaf(tempString)); // push command to vector
            
            if(semiBool)
            {                
                final_form.push_back(new Semicolon(";"));
            }
            else if(andBool)
            {
                final_form.push_back(new And("&&"));
            }
            else if(orBool)
            {
                final_form.push_back(new Or("||"));
            }
            else
            {
                temp.erase(temp.begin() + temp.find('('));
                temp.erase(temp.begin() + temp.find_last_of(')'));
                final_form.push_back(new Block(str_vec.at(i)));
            }
            tempString.clear();
        }
        else
        {
            tempString += str_vec.at(i);
            
            if(tempString.at(tempString.size() - 1) != ' ') // prevent double space error
            {
                tempString += " ";
            }
        } 
        // XX
        
        // YY
        if(i + 1 == str_vec.size()) 
        {
            if(tempString != "")
            {
                final_form.push_back(new Leaf(tempString)); // get the last part
            }
        } 
        // YY
        
    }// WW
    
    
    //build root of the tree
    //==========================================================================
    Node* n = 0; // init as NULL
    buildTree(n, final_form);
    //==========================================================================
    
    return n; // NULL Node* for now
}
//-------------------------------------------------------------------------------------------


bool Interpreter::isBalanced(std::string s) // check for ballanced number of separators (, [
{
    std::stack<char> stk;
    
    for(std::string::iterator it = s.begin(); it != s.end(); it++)
    {
        if(*it == '(' || *it == '[')
        {
            stk.push(*it);
        }
        else if(*it == ')' || *it == ']' )
        {
            if(stk.empty())
            {
                return false;
            }
            else
            {
                if(*it == ')' && stk.top() != '(' )
                {
                    return false;
                }
                else if(*it == ']' && stk.top() != '[' )
                {
                    return false;
                }
                else
                {
                    stk.pop();
                }
            } 
        }
    }
    
    return stk.empty();
}
//-------------------------------------------------------------------------------------------


void Interpreter::buildTree(Node*& n, std::vector<Object*> v)
{
    std::cout << "inside Build_tree" << std::endl;
    
        
    // THIS IS FOR TESTING, REMOVE LATER!!!  ***********************************
    // check to see if rejoined correctly
    for(unsigned i = 0; i < v.size(); i++)
    {
        std::cout << "\""<< v.at(i)->get() << "\" ";
    }    
    std::cout << std::endl;    
    // THIS IS FOR TESTING, REMOVE LATER!!!  ***********************************

    
    if(v.size() == 1) // single leaf is root
    {
        n = new Node(v.at(0)->get()); 
    }
    
    
}
//-------------------------------------------------------------------------------------------