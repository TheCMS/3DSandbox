#ifndef APPTEMPLATES_H
#define APPTEMPLATES_H

#include <vector>
#include <functional>
#include <string.h>


template <typename T>

class TSingleton
{
public:
    static T& GetInstance()
    {

        static T s_instance;
        return s_instance;
    }

protected:

    TSingleton(){}
    virtual ~TSingleton(){}


};


#endif // APPTEMPLATES_H
