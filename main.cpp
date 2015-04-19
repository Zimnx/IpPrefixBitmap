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
    IpPrefixBitmap bitmap, testBitmap;
    while(true)
    {
        int option;
        std::cout << "\n1.Add\n2.Del\n3.Check\n4.Performance Test\n5.Exit\n>>";
        std::cin  >> option;
        if(option == 5) break;
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
            using Clock = std::chrono::high_resolution_clock;
            std::cout << "Performance test: 120'000 operations(30'000 additions, 30'000 deletions, 60'000 checks)\nTime: ";
            auto t1 = Clock::now();
            for(int i = 0; i < 100000; ++i)
            {
                testBitmap.add(2887060214, 29);
                testBitmap.add(2887060206, 28);
                testBitmap.add(2887060206, 21);
                testBitmap.check(2887060205);
                testBitmap.del(2887060206, 28);
                testBitmap.check(2887060205);
                testBitmap.check(2887060205);
                testBitmap.del(2887060214, 29);
                testBitmap.del(2887060206, 21);
            }
            auto t2 = Clock::now();
            std::cout << std::chrono::duration_cast<chrono::milliseconds>(t2 - t1).count() << " milliseconds" << std::endl;
            std::cout << "Single operation:\nFirst Addition: ";
            t1 = Clock::now();
            testBitmap.add(2887060206, 28);
            t2 = Clock::now();
            std::cout << std::chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() << " nanoseconds" << std::endl;
            std::cout << "Second Addition: ";
            t1 = Clock::now();
            testBitmap.add(2887060203, 27);
            t2 = Clock::now();
            std::cout << std::chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() << " nanoseconds" << std::endl;
            std::cout << "Check: ";
            t1 = Clock::now();
            testBitmap.check(2887060205);
            t2 = Clock::now();
            std::cout << std::chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() << " nanoseconds" << std::endl;
            std::cout << "Deletion: ";
            t1 = Clock::now();
            testBitmap.del(2887060206, 28);
            t2 = Clock::now();
            std::cout << std::chrono::duration_cast<chrono::nanoseconds>(t2 - t1).count() << " nanoseconds" << std::endl;
            testBitmap.del(2887060203, 27);
            break;
        }
        std::cin.ignore();
    }
    return 0;
}
