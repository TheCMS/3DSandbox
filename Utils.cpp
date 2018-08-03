#include "Utils.h"

#include <chrono>

long long getCurrentTimeMillis()
{
    auto time = std::chrono::system_clock::now();
    auto since_epoch = time.time_since_epoch();

    auto millis = std::chrono::duration_cast<std::chrono::microseconds>(since_epoch);

    long long  ms = millis.count();

    return ms;
}

std::vector<std::string> getLinesFromChar(const char* sz)
{
    std::vector<std::string> rez;
    std::string fullStr(sz);
    int len= fullStr.size();
    int curPos=0;
    int curHead=0;

    while (curPos < len)
    {
        if (fullStr[curPos]=='\n')
        {
            if (curHead<curPos)
            {
                rez.push_back(fullStr.substr(curHead,curPos-curHead));
            }
            curHead=curPos+1;
        }
        curPos++;
    }
    if (curHead < curPos)
    {
        rez.push_back(fullStr.substr(curHead,curPos-curHead));
    }
    return rez;
}

std::string trimmLead(std::string& aStr, const std::string& whiteSpace)
{

    int pos = aStr.find_first_not_of(whiteSpace);
    if (pos !=std::string::npos)
    {
        return aStr.substr(pos);
    }
    return "";
}
std::string trimmEnd(std::string& aStr, const std::string& whiteSpace)
{
    int pos = aStr.find_last_not_of(whiteSpace);
    if (pos!=std::string::npos)
    {
        return aStr.substr(0,pos);
    }
    return "";

}
std::string trimm(std::string& aStr, const std::string& whitespace)
{
    int pos = aStr.find_first_not_of(whitespace);
    int end = aStr.find_last_not_of(whitespace);

    if (pos==std::string::npos)
    {
        return "";
    }
    return aStr.substr(pos,end-pos+1);
}


