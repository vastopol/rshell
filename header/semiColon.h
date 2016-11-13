#ifndef __SEMICOLON_H__
#define __SEMICOLON_H__

#include <vector>
#include <string>

#include "connector.h"

class Semicolon : public Connector
{
    public:
    
        Semicolon(std::string);
        
        ~Semicolon(); // ?? add destructor ??
    
        std::string get();
        
        std::string type();   
};

#endif // __SEMICOLON_H__