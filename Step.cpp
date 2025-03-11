#include "Step.hpp"

Step::Step(int v, int c)
{
    this->vertex = v;
    this->color = c;
}

Step::Step(int v, int c, int hMEWSC, int hMC)
{
    this->vertex = v;
    this->color = c;
    this->howManyEdgesWithSameColor = hMEWSC;
    this->howManyColors = hMC;
}

void Step::setHowManyColors(int hMC)
{
    this->howManyColors = hMC;
}

void Step::setHowManyEdgesWithSameColors(int hMEWSC)
{
    this->howManyEdgesWithSameColor = hMEWSC;
}

int Step::getVertex() const 
{
    return vertex;
}

int Step::getColor() const 
{
    return color;
}

int Step::getHowManyEdgesWithSameColor() const 
{
    return howManyEdgesWithSameColor;
}

int Step::getHowManyColors() const 
{
    return howManyColors;
}