#include "NodeWindow.h"
#include <LibSL.h>

// This class implements everything to handle the iceSL error that are coming back through TCP
// Makes the corresponding between an error in a code line with the problematic node

class GraphError{

private:

    std::vector<std::pair<int,Node*> > nodesByLine;
	// std::pair stocks 2 differents objects in one
	// Here, we associate an int to a Node*
	// nodesByLine corresponds to all the associations between a Node and the last line number

    int m_start;

	// Calculate the number of lines in path
    int numberOfLine(std::string path){

        int nol= 0;
        std::string line;
        std::ifstream file(path);
        while (std::getline(file, line))nol++;
        return nol;

    }

	// Calculate the number of lines before a certain Node n appears
    int numberOfLineBefore(Node* n){

        int nol= 0;
        std::string line;
        std::istringstream stream(n->codeBefore());
        while (std::getline(stream, line))nol++;
        return nol;

    }

	// Calculare the number of lines after a certain node n has appeared
    int numberOfLineAfter(Node* n){

        int nol= 0;
        std::string line;
        std::istringstream stream(n->codeAfter());
        while (std::getline(stream, line))nol++;
        return nol;

    }

public:

    GraphError(){
        m_start = 0;
    }

    void computeMap(std::vector<Node*>& orderedNode){

        std::string s(PATHTOSRC"/lua_constant/header.lua");

        int start = m_start+numberOfLine(s);
		// start = total number of lines

        int end = start;
		// end = total number of lines

        for(Node* n: orderedNode){

            start += numberOfLineBefore(n);
			// start is the number of the first line of the node

            end = start+numberOfLine(n->getPath());
            end += numberOfLineAfter(n);	
			// end is the number of the last line of the node

            nodesByLine.push_back(std::make_pair(end,n));
			// We add an element at the end of the vector nodesByLine
			// Which corresponds to the association between a node and its last line

            start = end;
			// We go ahead begining from the last line

        }

        m_start = end;

    }

    // Finds which node has a problem thanks to the line containing an error
    Node* getNodeByErrorLine(int line){

        ForIndex(i,nodesByLine.size()){

			// We go across the vector nodesByLine
            std::pair<int,Node*> pa = nodesByLine[i];

			// Once the number of the last line of the node is higher to the number of the error line
			// It means that we have found the corresponding node
            if(pa.first > line)return pa.second;
			// pa.first = number of the last line, pa.second = corresponding node

        }

        sl_assert(false);
        return nullptr; 

    }

};