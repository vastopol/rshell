#ifndef AND_H
#define AND_H
#include <vector>
#include <string>
#include "connector.h"

class And : public Connector
{
    
    private:
    
        std::vector<std::string> v;
    
    public:
    
        And(std::vector<std::string> vs);
        
        std::vector<std::string> get();

};
#endif
