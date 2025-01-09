#pragma once
#include <limits>
#include <random>

// easy to use if you want something by say 1 - 10 call Random::Float() * 10 i.e
// Simple if want a number from ( 1 - n ) call Random::Float() then * n to get a random number in range
class Random
{
    static std::mt19937 s_RandomEngine;
    static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;

public:
    //TODO: Finish Rule of Five
    Random(const Random&) = delete; // removing copy c'tor
    Random& operator = (const Random&) = delete; // removing the overload for the assignment operator

    static void Init()
    {
        s_RandomEngine.seed(std::random_device()());
    }

    static float Float()
    {
        return static_cast<float>(s_Distribution(s_RandomEngine)) / static_cast<float>(std::numeric_limits<uint32_t>::infinity());
    }
};

//TODO: Research -
// https://medium.com/codex/high-performance-random-numbers-a-mersenne-twister-alternative-cf827d89897a - High Performance Random Numbers: A Mersenne twister alternative
// cumulative probability functions - CDF
// https://hackingcpp.com/cpp/std/random.html - Random Distributions

//TODO : Test More 
template<typename TypeName>
TypeName XORShift(TypeName x, TypeName y, TypeName z, TypeName w)
{
    TypeName t = x ^ (x << 11);

    x = y; y = z; z = w;

    return w = w ^ (w >> 19) ^ t ^ (t >> 8);
}

