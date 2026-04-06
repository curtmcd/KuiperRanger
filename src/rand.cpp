#include "rand.hpp"

std::minstd_rand *Rand::gen = nullptr;

void Rand::init()
{
    gen = new std::minstd_rand(std::random_device{}());
}

void Rand::term()
{
    delete gen;
}
