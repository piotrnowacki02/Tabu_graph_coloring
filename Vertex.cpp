#include <vector>
#include "Vertex.hpp"

Vertex::Vertex()
{
    this->index = 0;
    this->color = 0;
}

Vertex::Vertex(int index)
{
    this->index = index;
    this->color = 0;
}

Vertex::Vertex(int index, int color)
{
    this->index = index;
    this->color = color;
}

Vertex::Vertex(int index, int color, vector <int> nextIndexes)
{
    this->index = index;
    this->color = color;
    this->nextIndexes = nextIndexes;
}


int Vertex::getIndex()
{
    return this->index;
}

int Vertex::getColor()
{
    return this->color;
}


void Vertex::setColor(int color)
{
    this->color = color;
}

void Vertex::printNexts()
{
    for(int i = 0; i < this->nextIndexes.size(); i++)
        cout << nextIndexes[i] << " ";
}

void Vertex::addNext(int next)
{
    this->nextIndexes.push_back(next);
}

int Vertex::getNextIndex(int indexInVector)
{
    return this->nextIndexes[indexInVector];
}

int Vertex::getNumberOfNexts()
{
    return this->nextIndexes.size();
}

bool Vertex::isItNext(int v)
{
    for(int i = 0; i < this->nextIndexes.size(); i++)
    {
        if(this->nextIndexes[i] == v)
            return true;
    }
    return false;
}
