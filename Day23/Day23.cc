#include <iostream>
#include <fstream>
#include <vector>
#include <array>

void parseInput(std::vector<std::array<int64_t, 4>> &bots)
{
    std::string line;
    std::array<int64_t, 4> bot;
    size_t pos = 0;
    size_t endpos = 0;
    std::ifstream input("input.txt");

    if(input.is_open())
    {
        while(getline(input,line))
        {
            pos = line.find_first_of("-1234567890");
            endpos = line.find_first_of(",",pos);
            bot[0] = std::stoi(line.substr(pos, endpos-pos));
           
            pos = line.find_first_of("-1234567890", endpos);
            endpos = line.find_first_of(",", pos);
            bot[1] = std::stoi(line.substr(pos, endpos-pos));
           
            pos = line.find_first_of("-1234567890", endpos);
            endpos = line.find_first_of(",", pos);
            bot[2] = std::stoi(line.substr(pos, endpos-pos));
           
            pos = line.find_first_of("1234567890", endpos);
            bot[3] = std::stoi(line.substr(pos));
           
            bots.push_back(bot);
        }
    }
    input.close();
}

uint64_t findStrongestRange(const std::vector<std::array<int64_t, 4>> &bots)
{
    uint64_t result = 0;
    std::array<int64_t,4> strongestBot{0,0,0,0};
   
    for(const std::array<int64_t, 4> &bot : bots)
    {
        if(strongestBot[3] < bot[3])
        {
            strongestBot = bot;
        }
    }

    int64_t dist = 0;
    for(const std::array<int64_t, 4> &bot : bots)
    {
        dist = abs(bot[0] - strongestBot[0]) + abs(bot[1] - strongestBot[1]) + abs(bot[2] - strongestBot[2]);
        if(dist <= strongestBot[3])
        {
            result++;
        }
    }
    return result;
}

uint64_t findPeak(const std::vector<std::array<int64_t, 4>> &bots)
{
    uint64_t result = 0;

    std::array<int64_t, 5> mostInRange{0,0,0,0,0};
   
    for(int64_t i=-100000000; i<100000000; i+=5000000)
    {
        for(int64_t j=-100000000; j<100000000; j+=5000000)
        {
            for(int64_t k=-100000000; k<100000000; k+=5000000)


            {
                unsigned inRange = 0;
                int64_t distToCentre = 0;
                for(const std::array<int64_t, 4> &bot : bots)
                {
                    if(bot[3] >= abs(bot[0] - i) + abs(bot[1] - j) + abs(bot[2] - k))
                    {
                        inRange++;
                    }
                }
               
                distToCentre = abs(i) + abs(j) + abs(k);
               
                if(inRange > mostInRange[3] || (inRange == mostInRange[3] && distToCentre < mostInRange[4]))
                {
                    mostInRange[0] = i;
                    mostInRange[1] = j;
                    mostInRange[2] = k;
                    mostInRange[3] = inRange;
                    mostInRange[4] = distToCentre;
                }
            }
        }
    }
   
    for(int64_t i= mostInRange[0]-10000000; i<mostInRange[0]+10000000; i+=500000)
    {
        for(int64_t j=mostInRange[1]-10000000; j<mostInRange[1]+10000000; j+=500000)
        {
            for(int64_t k=mostInRange[2]-10000000; k<mostInRange[2]+10000000; k+=500000)
            {
                unsigned inRange = 0;
                int64_t distToCentre = 0;
                for(const std::array<int64_t, 4> &bot : bots)
                {
                    if(bot[3] >= abs(bot[0] - i) + abs(bot[1] - j) + abs(bot[2] - k))
                    {
                        inRange++;
                    }
                }
               
                distToCentre = abs(i) + abs(j) + abs(k);
               
                if(inRange > mostInRange[3] || (inRange == mostInRange[3] && distToCentre < mostInRange[4]))
                {
                    mostInRange[0] = i;
                    mostInRange[1] = j;
                    mostInRange[2] = k;
                    mostInRange[3] = inRange;
                    mostInRange[4] = distToCentre;
                }
            }
        }
    }
   
    for(int64_t i= mostInRange[0]-1000000; i<mostInRange[0]+1000000; i+=50000)
    {
        for(int64_t j=mostInRange[1]-1000000; j<mostInRange[1]+1000000; j+=50000)
        {
            for(int64_t k=mostInRange[2]-1000000; k<mostInRange[2]+1000000; k+=50000)
            {
                unsigned inRange = 0;
                int64_t distToCentre = 0;
                for(const std::array<int64_t, 4> &bot : bots)
                {
                    if(bot[3] >= abs(bot[0] - i) + abs(bot[1] - j) + abs(bot[2] - k))
                    {
                        inRange++;
                    }
                }
               
                distToCentre = abs(i) + abs(j) + abs(k);
               
                if(inRange > mostInRange[3] || (inRange == mostInRange[3] && distToCentre < mostInRange[4]))
                {
                    mostInRange[0] = i;
                    mostInRange[1] = j;
                    mostInRange[2] = k;
                    mostInRange[3] = inRange;
                    mostInRange[4] = distToCentre;
                }
            }
        }
    }
   
    for(int64_t i= mostInRange[0]-100000; i<mostInRange[0]+100000; i+=10000)
    {
        for(int64_t j=mostInRange[1]-100000; j<mostInRange[1]+100000; j+=10000)
        {
            for(int64_t k=mostInRange[2]-100000; k<mostInRange[2]+100000; k+=10000)
            {
                unsigned inRange = 0;
                int64_t distToCentre = 0;
                for(const std::array<int64_t, 4> &bot : bots)
                {
                    if(bot[3] >= abs(bot[0] - i) + abs(bot[1] - j) + abs(bot[2] - k))
                    {
                        inRange++;
                    }
                }
               
                distToCentre = abs(i) + abs(j) + abs(k);
               
                if(inRange > mostInRange[3] || (inRange == mostInRange[3] && distToCentre < mostInRange[4]))
                {
                    mostInRange[0] = i;
                    mostInRange[1] = j;
                    mostInRange[2] = k;
                    mostInRange[3] = inRange;
                    mostInRange[4] = distToCentre;
                }
            }
        }
    }
   
    for(int64_t i= mostInRange[0]-10000; i<mostInRange[0]+10000; i+=1000)
    {
        for(int64_t j=mostInRange[1]-10000; j<mostInRange[1]+10000; j+=1000)
        {
            for(int64_t k=mostInRange[2]-10000; k<mostInRange[2]+10000; k+=1000)
            {
                unsigned inRange = 0;
                int64_t distToCentre = 0;
                for(const std::array<int64_t, 4> &bot : bots)
                {
                    if(bot[3] >= abs(bot[0] - i) + abs(bot[1] - j) + abs(bot[2] - k))
                    {
                        inRange++;
                    }
                }
               
                distToCentre = abs(i) + abs(j) + abs(k);
               
                if(inRange > mostInRange[3] || (inRange == mostInRange[3] && distToCentre < mostInRange[4]))
                {
                    mostInRange[0] = i;
                    mostInRange[1] = j;
                    mostInRange[2] = k;
                    mostInRange[3] = inRange;
                    mostInRange[4] = distToCentre;
                }
            }
        }
    }

    for(int64_t i= mostInRange[0]-1000; i<mostInRange[0]+1000; i+=100)
    {
        for(int64_t j=mostInRange[1]-1000; j<mostInRange[1]+1000; j+=100)
        {
            for(int64_t k=mostInRange[2]-1000; k<mostInRange[2]+1000; k+=100)
            {
                unsigned inRange = 0;
                int64_t distToCentre = 0;
                for(const std::array<int64_t, 4> &bot : bots)
                {
                    if(bot[3] >= abs(bot[0] - i) + abs(bot[1] - j) + abs(bot[2] - k))
                    {
                        inRange++;
                    }
                }
               
                distToCentre = abs(i) + abs(j) + abs(k);
               
                if(inRange > mostInRange[3] || (inRange == mostInRange[3] && distToCentre < mostInRange[4]))
                {
                    mostInRange[0] = i;
                    mostInRange[1] = j;
                    mostInRange[2] = k;
                    mostInRange[3] = inRange;
                    mostInRange[4] = distToCentre;
                }
            }
        }
    }
   
    for(int64_t i= mostInRange[0]-100; i<mostInRange[0]+100; i+=10)
    {
        for(int64_t j=mostInRange[1]-100; j<mostInRange[1]+100; j+=10)
        {
            for(int64_t k=mostInRange[2]-100; k<mostInRange[2]+100; k+=10)
            {
                unsigned inRange = 0;
                int64_t distToCentre = 0;
                for(const std::array<int64_t, 4> &bot : bots)
                {
                    if(bot[3] >= abs(bot[0] - i) + abs(bot[1] - j) + abs(bot[2] - k))
                    {
                        inRange++;
                    }
                }
               
                distToCentre = abs(i) + abs(j) + abs(k);
               
                if(inRange > mostInRange[3] || (inRange == mostInRange[3] && distToCentre < mostInRange[4]))
                {
                    mostInRange[0] = i;
                    mostInRange[1] = j;
                    mostInRange[2] = k;
                    mostInRange[3] = inRange;
                    mostInRange[4] = distToCentre;
                }
            }
        }
    }
   
    for(int64_t i= mostInRange[0]-10; i<mostInRange[0]+10; i+=1)
    {
        for(int64_t j=mostInRange[1]-10; j<mostInRange[1]+10; j+=1)
        {
            for(int64_t k=mostInRange[2]-10; k<mostInRange[2]+10; k+=1)
            {
                unsigned inRange = 0;
                int64_t distToCentre = 0;
                for(const std::array<int64_t, 4> &bot : bots)
                {
                    if(bot[3] >= abs(bot[0] - i) + abs(bot[1] - j) + abs(bot[2] - k))
                    {
                        inRange++;
                    }
                }
               
                distToCentre = abs(i) + abs(j) + abs(k);
               
                if(inRange > mostInRange[3] || (inRange == mostInRange[3] && distToCentre < mostInRange[4]))
                {
                    mostInRange[0] = i;
                    mostInRange[1] = j;
                    mostInRange[2] = k;
                    mostInRange[3] = inRange;
                    mostInRange[4] = distToCentre;
                }
            }
        }
    }
    result = abs(mostInRange[0]) + abs(mostInRange[1]) + abs(mostInRange[2]);
   
    return result;
}

int main()
{
    uint64_t resultA = 0;
    uint64_t resultB = 0;
    std::vector<std::array<int64_t, 4>> bots;
   
    parseInput(bots);

    resultA = findStrongestRange(bots);
    resultB = findPeak(bots);
   
    std::cout << "result A: " << resultA << '\n';
    std::cout << "result B: " << resultB << std::endl;
    return 0;
}
