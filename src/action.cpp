#include "../header/action.h"

#include <stdexcept>
#include <stdio.h>
#include <cstdlib>  // exit
#include <iostream> // echo
#include <cstring>
#include <sys/wait.h>
#include <unistd.h> // getcwd()
#include "sys/stat.h" // chdir()
#include "dirent.h" // closedir(), opendir()

void Action::exec(std::vector<Object*> o)
{
    bool b = true;
    for(unsigned i = 0; i < o.size(); i++)
    {
        
        if(o.at(i)->type() == "Command") //if its a command execute it
        {
            
            if(o.at(i)->type() == "Command") //if its a command execute it
            {
                std::string com = o.at(i)->get(); //gets string literal
    
                std::string args = com.substr(com.find(" ") + 1, com.size()); //retrieves only the arguments(might not have anything)
    
            
                com = com.substr(0, com.find(" ") - 1); //retrieves only the command
                
                //try
        
                if(com == "exit")
                {
                    exitr(); //might just use cstdlib exit
                }
                else if(com == "cd")
                {
                    b = cd(args);
                }
                else if(com == "pwd")
                {
                    b = pwd(args);
                }
                else if(com == "echo")
                {
                    b = echo(args);
                }
                else
                {
                    //bin
                    std::string in = o.at(i)->get();
                    const char* in1 = in.c_str();
                    int flag = executr((char*)(in1));
                    if(flag == 1)
                    {
                        b = true;
                    }
                    else
                    {
                        false;
                    }
                }
            }
        }
        else //if it is a connector
        {
            if(o.at(i)->type() == "And" && b == false)
            {
                i++;
            }
            else if(o.at(i)->type() == "Or" && b == true)
            {
                i++;
            }
            else
            {
                //do nothing if semicolon
            }
        }
    }
}

int Action::executr(char* cmd)
{
    char* argv[64];
    int flag = 0;
    char* tempC;
    unsigned c = 1;
    tempC = strtok(cmd, " ");
    argv[0] = tempC;
    
    while(tempC != NULL)
    {
        //cout << c << endl;
        tempC = strtok(NULL, " ");
        //cout << tempC << endl;
        argv[c] = tempC;
        //cout << argv[c] << endl;
        c++;
    }
    
    pid_t pid;
    pid_t wpid;
    
    int status;
    
    pid = fork();
    
    if(pid == 0) //if child
    {
        //cout << "in child first" << endl;
        flag = execvp(argv[0], argv);
        //cout << flag;
        //cout << "after execvp " << flag << endl;
        if(flag == -1) //attempt to execute but if -1 calls perror
        {
            //cout << flag << " in child (err)" << endl;
            std::string restring(argv[0]);
            perror(restring.c_str());
            exit(-1);
        }
        //cout << flag << " in child (norm)" << endl;
        exit(0);
    }
    else if (pid < 0)
    {
        perror("fork");
        exit(-1);
    }
    else
    {
        int i = 0;
        do
        {
            wpid = waitpid(pid, &status, WUNTRACED); //waits on child and retrieves status
            //cout << status << " in parent process" << endl;
            i++;
        }
        while(!WIFEXITED(status) && !WIFSIGNALED(status)); //waits while child is not exited or killed by a signal
    }
    
    if(status != 0)
    {
        return -1;
    }
    
    return 1;
    
}
//-----------------------------------------------

void Action::exitr()
{
    exit(0); // ?? seems too simple...
}
//------------------------------------------------

bool Action::echo(std::string str)
{
    if(str.empty()) // if no str, then echo "\n"
    {
        std::cout << std::endl;
        return true;
    }
    else if(!str.empty()) // NO HANDLE FLAGS HERE***
    {
        std::cout << str << std::endl;    
        return true;
    }
    else
    {        
        std::cout << "ERROR UNKNOWN echo(std::string) FAIL" << std::endl;
    }
    return false; // catch
}
//------------------------------------------------

bool Action::cd(std::string str)  // need to use: chdir(), opendir(), closedir()
{
    int flag = -1;
    char curDir[64]; // not sure about what size
    getcwd(curDir, sizeof(curDir)); // should return pointer to current working directory
    
    if(str.empty()) 
    {
        // if no str, then cd ~/
        std::string q = "~";
        flag = chdir(q.c_str());
        if(flag != 0)
        {
            perror("chdir()");
            return false;
        }
        return true;
    }
    else
    {
        if(str == std::string(curDir)) // fail if try to change to the directory already in
        {
            std::cout << "rshell: cd: " << str << ": No such file or directory" << std::endl;
            return false;
        }
        
        flag = chdir(str.c_str());
        
        if(flag != 0)
        {
            std::cout << "rshell: cd: " << str << ": No such file or directory" << std::endl;
            perror("chdir()");
            return false;
        }
        return true;
    }
    
    return false;
}
//------------------------------------------------

bool Action::pwd(std::string str)
{
    if(!str.empty())  // NO HANDLE FLAGS HERE***
    {
        std::cout << "ERROR UNKNOWN FLAG" << std::endl;
        return false;
    }
    
    char curDir[64]; // not sure about what size
    getcwd(curDir, sizeof(curDir)); // should return pointer to current working directory
    
    if(!curDir)
    {
        std::cout << "ERROR UNKNOWN PWD FAIL" << std::endl;
    }
    else
    {
        std::cout << std::string(curDir) << std::endl;
        return true;
    }
    return false; // catch
}
//------------------------------------------------

