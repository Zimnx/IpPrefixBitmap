#include <iostream>
#include "IpPrefixBitmap.h"

#include <chrono>
#include <cassert>

using namespace std;

std::vector<unsigned> readIp()
{
    std::vector<unsigned> ip(4,0);
    char dot;
    std::cout << "Enter Ip(x.x.x.x)\n>> ";
    std::cin >> ip[0] >> dot >> ip[1] >> dot >> ip[2] >> dot >> ip[3];
    return ip;
}
unsigned ipToUint(const std::vector<unsigned>& ip)
{
    return (ip[0] * 0x1000000) + (ip[1] * 0x10000) + (ip[2] * 0x100) + ip[3];
}

int readMask()
{
    std::cout << "Enter mask\n>>";
    int mask;
    std::cin >> mask;
    return mask;
}

int main()
{
    IpPrefixBitmap bitmap;
    while(true)
    {
        int option;
        std::cout << "\n1.Add\n2.Del\n3.Check\n4.Exit\n>>";
        std::cin  >> option;
        int mask = 0;
        std::vector<unsigned> ip;
        switch(option)
        {
        case 1:
            ip = readIp();
            mask = readMask();
            bitmap.add(ipToUint(ip), mask);
            break;
        case 2:
            ip = readIp();
            mask = readMask();
            bitmap.del(ipToUint(ip), mask);
            break;
        case 3:
            ip = readIp();
            mask = bitmap.check(ipToUint(ip));
            if(mask == 0)
            {
                std::cout << "Prefix not found" << std::endl;
            }
            else
            {
                std::cout << "Found mask: " << mask << std::endl;
            }
            break;
        case 4:
            return 0;
        }
        std::cin.ignore();
    }
    return 0;
}
