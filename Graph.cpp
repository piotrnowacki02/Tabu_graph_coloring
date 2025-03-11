#include "Vertex.hpp"
#include <vector>
#include <sstream>
#include <stdio.h>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <fstream>
#include <iostream>
#include "Vertex.hpp"
#include "Graph.hpp"

Graph::Graph()
{
    this->nextColor = 1;
}

Graph::Graph(int numberOfVertices)
{
    for(int i = 0; i < numberOfVertices; ++i)
    {
        this->vertices.push_back(Vertex(i));
    }
    this->nextColor = 1;
}

void Graph::readGraphFromInput()
{
    string input;
    int numberOfVertices;
    std::cout << "Ile wierzcholkow: ";
    cin >> numberOfVertices;
    getline(cin, input);
    istringstream is(input);
    for(int i = 0; i < numberOfVertices; i++)
    {
        this->vertices.push_back(Vertex(i));
        std::cout << "Nastepnicy wierzcholka " << i << " : ";
        getline(cin, input);
        istringstream is(input);
        int nextIndex;
        while(is>>nextIndex) this->vertices[i].addNext(nextIndex);
    }
}

void Graph::readGraphFromInputEdges()
{
    int numberOfVertices, numberOfEdges;
    std::cout << "Ile wierzcholkow: ";
    cin >> numberOfVertices;
    for(int i = 0; i < numberOfVertices; i++)
    {
        this->vertices.push_back(Vertex(i));
    }
    std::cout << "Ile krawedzi: ";
    int indexA, indexB;
    cin >> numberOfEdges;
    for(int i = 0; i < numberOfEdges; i++)
    {
        std::cout << "Krawedz laczy wierzcholek A z wierzcholkiem B\n Indeks wierzcholka A: ";
        cin >> indexA;
        std::cout << "Indeks wierzcholka B: ";
        cin >> indexB;
        this->vertices[indexA].addNext(indexB);
        this->vertices[indexB].addNext(indexA);
    }
}

void Graph::readGraphFromTxt(string filename)
{
    fstream file;
    file.open(filename, ios::in );
    if (!file) {
		std::cout << "File not created!";
        return;
	}
    int numberOfVertices, v1, v2;
    file >> numberOfVertices;
    this->Adj_Matrix = new int* [numberOfVertices];
    for(int i = 0; i < numberOfVertices; i++)
    {
        this->vertices.push_back(Vertex(i+1));
        this->Adj_Matrix[i] = new int [numberOfVertices];
        for(int j = 0; j < numberOfVertices; j++)
            this->Adj_Matrix[i][j] = 0;
    }
    while(file >> v1 && file >> v2)
    {
        if(this->vertices[v1-1].isItNext(v2))
            continue;
        this->vertices[v1-1].addNext(v2);
        this->vertices[v2-1].addNext(v1);
        this->Adj_Matrix[v1 - 1][v2 - 1] = 1;
        this->Adj_Matrix[v2 - 1][v1 - 1] = 1;


    }
}

// int Graph::tabuMaxLength()
// {
//     return this->maxTabuList;
// }

void Graph::printGraph()
{
    std::cout << endl;
    for(int i = 0; i < this->vertices.size(); i++)
    {
        std::cout << this->vertices[i].getIndex() << ": ";
        this->vertices[i].printNexts();
        std::cout << endl;
    }
    std::cout << endl;
}

void Graph::printHowManyColors()
{
    std::cout << "\nKolory uzyte: " << (this->nextColor - 1) << endl << endl;
}

void Graph::printGraphColors()
{
    std::cout << endl;
    for(int i = 0; i < this->vertices.size(); i++)
    {
        std::cout << "Wierzcholek: " << this->vertices[i].getIndex() << "  Kolor: " << this->vertices[i].getColor() << endl;;
    }
    std::cout << endl;
}

void Graph::printColorGroups()
{
    for(int i = 0; i < this->Vi.size(); i++)
    {
        std::cout << "Color " << i+1 << ": ";
        for(int j = 0; j < this->Vi[i].size(); j++)
        {
            std::cout << this->Vi[i][j] << ", ";
        }
        std::cout << endl;
    }
}

void Graph::colorizeGreedy()
{
    for(int i = 1; i <= this->vertices.size(); i++)//idziesz przez wszystkie wierzcholki
    {
        for(int color = 1; color <= this->nextColor; color++)//patrzysz po kolei wszystkie kolory czy mozesz uzyc w ostatecznosci uzywasz nowego
        {
            if(canVertexUseColor(i, color))
            {
                this->vertices[i-1].setColor(color);
                if(this->vertices[i-1].getColor() == this->nextColor)//jezeli uzyles nowego to musisz zmienic skladowa nowy kolor na o 1 wieksza
                    nextColor++;
                break;
            }
        }
    }
}

bool Graph::canVertexUseColor(int vertex, int color)
{
    for(int i = 0; i < this->vertices[vertex-1].getNumberOfNexts(); i++)
    {
        if(this->vertices[this->vertices[vertex-1].getNextIndex(i)-1].getColor() == color)
            return false;
    }
    return true;
}

void Graph::UpdateVis()
{
    for(int i = 0; i < this->nextColor -1; i++)
    {
        this->Vi.push_back({});
    }
    for(int vertex_index = 0; vertex_index < this->vertices.size(); vertex_index++)
        this->Vi[this->vertices[vertex_index].getColor() - 1].push_back(this->vertices[vertex_index].getIndex());
}

bool Graph::isItTabuMove(int v, int c)
{
    for(int i = 0; i < this->steps.size(); i++)
    {
        if(v == this->steps[i].getVertex() && c == this->steps[i].getColor())
            return true;
    }
    return false;
}

int Graph::colorOfVFromSolution(int v, int solution)
{
    for(int i = 0; i < this->Solution[solution].size(); i++)
    {
        for(int j = 0; j < this->Solution[solution][i].size(); j++ )
        {
            if(this->Solution[solution][i][j] == v)
                return i+1;
        }
    }
    return 0;
}

void Graph::changeXandJinSolution(int x, int j, int solution)
{
    int colorToErase = colorOfVFromSolution(x, solution);
    this->prevColor = colorToErase;
    this->Solution[solution][j-1].push_back(x);
    this->Solution[solution][colorToErase-1].erase(std::remove(this->Solution[solution][colorToErase-1].begin(), this->Solution[solution][colorToErase-1].end(), x), this->Solution[solution][colorToErase-1].end());
}

vector<int> Graph::neighbourWithSameColor()
{
    vector <int> result;
    for(int color = 0; color < Solution[0].size(); color++)
    {
        for(int i = 0; i < Solution[0][color].size() ;i++)
        {
            for(int j = i+1; j < Solution[0][color].size() ; j++)
            {
                if(this->Adj_Matrix[this->Solution[0][color][i] - 1][this->Solution[0][color][j] - 1] == 1)
                {
                    result.push_back(this->Solution[0][color][i]);
                    result.push_back(this->Solution[0][color][j]);
                }
            }
        }
    }
    return result;
}

int Graph::worstNeighbour()
{
    int result = 0;
    int mostOccurances = 0;
    vector <int> occurances(this->vertices.size(), 0);
    for(int color = 0; color < Solution[0].size(); color++)
    {
        for(int i = 0; i < Solution[0][color].size() ;i++)
        {
            for(int j = i+1; j < Solution[0][color].size() ; j++)
            {
                if(this->Adj_Matrix[this->Solution[0][color][i] - 1][this->Solution[0][color][j] - 1] == 1)
                {
                    occurances[this->Solution[0][color][i] - 1]++;
                    occurances[this->Solution[0][color][j] - 1]++;
                }
            }
        }
    }
    for(int i = 1; i < occurances.size(); i++ )
    {
        if(occurances[i] > mostOccurances)
        {
            mostOccurances = occurances[i];
            result = i;
        }
    }
    return result+1;
}

int Graph::fs(int solution)
{
    int fs = 0;
    for(int color = 0; color < this->Solution[solution].size(); color++)
    {
        for(int i = 0; i < this->Solution[solution][color].size(); i++)
        {
            for(int j = i+1; j < this->Solution[solution][color].size(); j++)
            {
                if(this->Adj_Matrix[this->Solution[solution][color][i] - 1][this->Solution[solution][color][j] - 1] == 1)//faster way with Adj Matrix
                    fs++;
            }
        }
    }
    return fs;
}

int Graph::bestColorForV(int v)
{
    int bestColor;
    int minFs, currFs;
    for(int color = 0; color < Solution[0].size(); color++)
    {
        Solution[0][color].push_back(v);
        currFs = fs(0);

        if(currFs < minFs || color == 0)
        {
            minFs = currFs;
            bestColor = color;
        }

        Solution[0][color].pop_back();
    }
    return bestColor;
}

void Graph::tabuSearch(int k, int rep, int maxTabuList, int maxIteration)
{

    
    std::srand(std::time(0));
    int numOfVertices = vertices.size();
    int numOfColors = this->nextColor - 1;
    //create Solution from greedy Solution ereasing last colors and vertices that were inside them go to random other color
    this->Solution.push_back({});//Solution[0]
    for(int i = 0; i < k; i++)
    {
        Solution[0].push_back({});
        for(int i2 = 0; i2 < Vi[i].size(); i2++)
        {
            Solution[0][i].push_back(Vi[i][i2]);
        }
    }
    int newColor;
    for(int i = k; i < numOfColors; i++)
    {
        for(int i2 = 0; i2 < Vi[i].size(); i2++)
        {
            newColor = bestColorForV(Vi[i][i2]);
            // newColor = rand() % (k);
            Solution[0][newColor].push_back(Vi[i][i2]);
        }
    }

    int x;//x to wierzcholek 
    int idX;//id z tablicy mozliwych x'ów z ktorej jest wybierany
    int j;//kolor nowy dla wierzcholka
    int currFs = fs(0);//fs - obliczana funkcja celu oznaczająca ile istnieje krawedzi niedozwolonych (konfliktowych) chcemy zeby bylo rowne 0
    // int** moves = new int* [rep];//move[0] - index; move[1] - color; move[2] - fs;
    int tempFs;
    int minFs; //najlepsze fs jakie nam sie udalo uzyskac w danym rzucie repow
    // int indexOfMinFs; //chyba da sie odczytac z nazwy
    int x_OfMinFs;
    int j_OfMinFs;
    int prevColorOfMinFs;
    int iteration = 0; // numer iteracji 
    int prevColor; // poprzedni kolor (kolor i) wierzcholka ktory od teraz ma kolor j
    while( currFs != 0 && iteration < maxIteration)
    {
        minFs = currFs*2;
        // cout << "Iteration: " << iteration << endl;
        //tworzymy vector mozliwych wierzcholkow z ktorego losujemy jeden
        vector<int> possibleX = neighbourWithSameColor();
        for(int numOfRep = 0; numOfRep < rep ; numOfRep++)
        {
            // this->Solution.push_back({});
            // for(int color = 0; color < k; color++)//kopiowanie Solution0 do obecnie tworzonego Solution // ten for idzie po wszystkich kolorach w Solution0
            // {
            //     Solution[1+numOfRep].push_back({});
            //     for(int idOfV = 0; idOfV < Solution[0][color].size(); idOfV++)//w danym kolorze ten for idzie po wszystkich wierzcholkach ktore w nim sa 
            //     {
            //         Solution[1+numOfRep][color].push_back(Solution[0][color][idOfV]);
            //     }
            // }
            // moves[numOfRep] = new int[3];
            while(1)
            {
                idX = rand() % (possibleX.size());
                x = possibleX[idX];
                while(1)
                {
                    j = 1 + rand() % (k);
                    if( j != colorOfVFromSolution(x, 0))
                        break;
                }
                if(!isItTabuMove(x, j))
                    break;
            }
            changeXandJinSolution(x, j, 0);
            // changeXandJinSolution(x, j, 1+numOfRep);
            // moves[numOfRep][0] = x;
            // moves[numOfRep][1] = j;
            // moves[numOfRep][2] = fs(1+numOfRep);
            // moves[numOfRep][2] = fs(0);
            tempFs = fs(0);
            if(minFs > tempFs || numOfRep == 0)
            {
                // minFs = moves[numOfRep][2];
                // indexOfMinFs = numOfRep;
                minFs = tempFs;
                x_OfMinFs = x;
                j_OfMinFs = j;
                prevColorOfMinFs = this->prevColor;
            }
            changeXandJinSolution(x, this->prevColor, 0);
        }
        // prevColor = colorOfVFromSolution(moves[indexOfMinFs][0], 0);
        // this->steps.push_back(Step(moves[indexOfMinFs][0], prevColorOfMinFs));
        this->steps.push_back(Step(x_OfMinFs, prevColorOfMinFs));
        if(steps.size() >= maxTabuList)
            steps.erase(steps.begin());
        // changeXandJinSolution(moves[indexOfMinFs][0], moves[indexOfMinFs][1], 0);
        changeXandJinSolution(x_OfMinFs, j_OfMinFs, 0);
        currFs = minFs;//fs(0);

        // std::cout << "\nfs: " << currFs << "\t";
        // sort( possibleX.begin(), possibleX.end() );
        // possibleX.erase( unique( possibleX.begin(), possibleX.end() ), possibleX.end() );
        // std::cout << "conf: " << possibleX.size() << endl;
        // std::cout << "x: " << moves[indexOfMinFs][0] << endl;
        // std::cout << prevColor << " -> " << moves[indexOfMinFs][1]  << endl;
        // for(int i = 0; i < rep ; i++)
        // {
        //     // Solution[1+i].clear();
        //     delete [] moves[i];
        // }
        iteration++;

    }
    if(currFs == 0)
    {
        std::cout << "Udalo sie pokolorowac graf " << k << " kolorkami.\n";
    }
    else
    {
        std::cout << "nie udalo sie, doszlismy do f(s) = " << fs(0) << endl;
    }
}

void Graph::tabuSearch2(int rep, int maxIteration)
{

}
