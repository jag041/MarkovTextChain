#include <ctime>
#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <vector>
#include <map>



struct Node {

    Node(std::string w1, std::string w2) { word1 = w1; word2 = w2; }
    std::string word1{};
    std::string word2{};

    std::vector<size_t> _children;
    std::vector<size_t> _parents;
    std::vector<float> cWeights;
    std::vector<float> pWeights;

    unsigned int numChildren{ 0 };
    unsigned int numParents{ 0 };


    void addChild(size_t _node) { 
        // std::cout << "adding child: " << _node << std::endl;
        if (numChildren == 0)
        {
            // std::cout << "pushing back node" << std::endl;
            _children.push_back(_node);
            cWeights.push_back(1.f);
            numChildren++;
            return;
        }

        for (size_t i = 0; i < _children.size(); ++i)
        {
            // std::cout << "iterating thru children" << std::endl;
            if (_children[i] == _node)
            {
                // std::cout << "children equal" << std::endl;
                numChildren++;
                cWeights[i] /= (float)numChildren;
                return;
            }
        }
        // std::cout << "pushing back node" << std::endl;
        _children.push_back(_node);
        numChildren++;
        for(size_t i = 0; i < cWeights.size(); ++i)
        {
            cWeights[i] /= (float)numChildren;
        }
        cWeights.push_back(1.f / (float)numChildren);
        // std::cout << "adding child complete" << std::endl;
    }

    void addParent(size_t _node)
    {
        if (numParents == 0)
        {
            _parents.push_back(_node);
            pWeights.push_back(1.f);
            numParents++;
            return;
        }

        for (size_t i = 0; i < _parents.size(); ++i)
        {
            // std::cout << "iterating thru parents" << std::endl;
            if (_parents[i] == _node)
            {
                // std::cout << "parents equal" << std::endl;
                numParents++;
                pWeights[i] /= (float)numParents;
                return;
            }
        }
        // std::cout << "pushing back node" << std::endl;
        _parents.push_back(_node);
        numParents++;
        for(size_t i = 0; i < pWeights.size(); ++i)
        {
            pWeights[i] /= (float)numParents;
        }
        pWeights.push_back(1.f / (float)numParents);
        // std::cout << "adding parent complete" << std::endl;
    }
    

    bool operator==(Node& rhs) {
    if (rhs.word1 == word1 && rhs.word2 == word2) return true;
     return false;
    }

    float getAverage(unsigned int num) {
        return (float)num / (float)numChildren;
    }

};


// struct Edge {

//     Edge(Node* p, Node* c, std::string newWord) { parent = p; child = c; word = newWord; count++; }
//     Edge(std::string newWord) { word = newWord; }
//     std::string word{};
//     Node* parent{};
//     Node* child{};
//     float weight{ 1.f };
//     unsigned int count{ 0 };

//     void operator++() {
//         count++;
//         (*parent)++;
//     }

//     bool operator==(Edge& rhs) {
//         if (rhs.word == word && rhs.parent == parent) return true;
//         return false;
//     }

//     void calculateWeight()
//     {
//         weight = (float)count / (float)(parent->numChildren);
//     }

// };

struct Graph {

    std::vector<Node> nodes;
    // std::vector<Edge> edges;
    std::string fileBuffer;
    std::map<std::string, std::vector<std::string> > dictionary;

    void create(std::string& file) {
        const char* str{ file.c_str() };
        std::cout << "file c-string: " << str << std::endl;
        std::ifstream f(str, std::ios_base::in | std::ios_base::binary);
        if (!f.is_open()) {
            std::cout << "file failed to open" << std::endl;
            return;
        }
        fileBuffer = std::string((std::istreambuf_iterator<char>(f)), std::istreambuf_iterator<char>());
        f.close();
        if (fileBuffer.length() < 1) {
            std::cout << "failed to read file" << std::endl;
            return;
        }



        createDictionary();
        //createText(words - keyLen);
    }

    void createText(int w) {
        // std::cout << "beginning write text" << std::endl;
        std::string key, first, second;
        // std::map<std::string, std::vector<std::string> >::iterator it = dictionary.begin();
        // std::advance(it, rand() % dictionary.size());
        auto num = rand() % nodes.size();
        Node firstNode = nodes[num];
        // std::cout << firstNode.word1 << ' ' << firstNode.word2;
        key = firstNode.word1 + ' ' + firstNode.word2;
        std::cout << key;
        while (true) {

            std::vector<size_t> d = nodes[num]._children;

            if (d.size() < 1) break;

            // add weights here
            // currently just randomly chooses term, so output is identical to the hash table version in the Markov class

            auto num2 = rand() % d.size();
            size_t node = d[num2];
            second = nodes[node].word2;

            if (second.length() < 1) break;

            std::cout << " " << second;

            if (--w < 0) break;

            num = node;
        }
        std::cout << "\n";
    }

    void createDictionary() {
        std::cout << "writing graph" << std::endl;
        std::string w1, key{};
        size_t wc = 0, pos, next;
        next = fileBuffer.find_first_not_of(32, 0);
        if (next == std::string::npos) return;
        while (wc < 2) {
            pos = fileBuffer.find_first_of(' ', next);
            // std::cout << "char pos: " << pos << std::endl;
            w1 = fileBuffer.substr(next, pos - next);
            // edges push back w1
            // std::cout << "word 1: " << w1 << std::endl;
            key += w1 + " ";
            next = fileBuffer.find_first_not_of(32, pos + 1);
            if (next == std::string::npos) {
                std::cout << "found string::npos... exiting..." << std::endl;
                return;
            }
            wc++;
        }

        key = key.substr(0, key.size() - 1);
        // std::cout << key << std::endl;
        while (true) {
            std::string word1 = key.substr(0, key.find_first_of(' ', 0));
            std::string word2 = key.substr(key.find_first_of(' ', 0) + 1, key.size() - 1);
            size_t firstNode = getNode(word1, word2);
            // std::cout << "Key: " << key << std::endl;


            next = fileBuffer.find_first_not_of(32, pos + 1);
            if (next == std::string::npos) return;
            w1.clear();
            for (size_t i = next; fileBuffer[i] != ' '; ++i) {
                if (fileBuffer[i] > 127)
                    continue;
                w1.push_back(fileBuffer[i]);
                // std::cout << w1 << std::endl;
            }

            
            
            // std::cout << "Length to end: " << fileBuffer.length() - pos << std::endl;
            // std::cout << "Word 1: " << w1 << std::endl;
            // std::cout << "W1 length: " << w1.length() << std::endl;
            if (w1.length() < 1) break;

            pos = fileBuffer.find_first_of(' ', next + 1);

            if (w1.length() + 2 >= (fileBuffer.length() - pos))
                break;
            std::string w = key.substr(key.find_first_of(' ', 0) + 1, key.size() - 1);
            key = w + " " + w1;
            // std::cout << "key 2: " << key << std::endl;
            size_t secondNode = addNode(w, w1);
            nodes[firstNode].addChild(secondNode);
            nodes[secondNode].addParent(firstNode);
            //addEdge(&(nodes[nodes.size() - 2]), &(nodes[nodes.size() - 1]), w1);

        }
        std::cout << "Graph building complete: " << nodes.size() << " nodes in graph" << std::endl;
    }

    private:
    /*    bool addEdge(Node* p, Node* c, std::string w) {

            Edge edge(p, c, w);
            for (size_t i = 0; i < edges.size(); ++i)
            {
                if (edge == edges[i])
                {
                    edges[i]++;
                    edges[i].calculateWeight();
                    return false;
                }
            }
            edges.push_back(edge);
            return true;
        }*/

        size_t addNode(std::string w1, std::string w2) {
            // std::cout << "adding node " << w1 + ' ' + w2 << std::endl;
            Node node(w1, w2);
            for (size_t i = 0; i < nodes.size(); ++i)
            {
                if (node == nodes[i])
                {
                    return i;
                }
            }
            nodes.push_back(node);
            // std::cout << "adding node finished" << std::endl;
            return (size_t)nodes.size() - 1;
        }

        size_t getNode(std::string w1, std::string w2) {
            Node node(w1, w2);
            for (size_t i = 0; i < nodes.size(); ++i)
            {
                if (node == nodes[i])
                {
                    return i;
                }
            }
            return addNode(w1, w2);
        }
};


class Markov {
public:
    void create( std::string& file, unsigned int keyLen, unsigned int words ) {
        const char* str { file.c_str() };
        std::cout << "file c-string: " << str << std::endl;
        std::ifstream f( str, std::ios_base::in | std::ios_base::binary );
        if (!f.is_open()){
            std::cout << "file failed to open" << std::endl;
            return;
        }
        fileBuffer = std::string( ( std::istreambuf_iterator<char>( f ) ), std::istreambuf_iterator<char>() );
        f.close();
        if( fileBuffer.length() < 1 ) {
            std::cout << "failed to read file" << std::endl;
            return;
        }

        createDictionary( keyLen );
        createText( words - keyLen );
    }

private:
    void createText( int w ) {
        std::string key, first, second;
        size_t next;
        std::map<std::string, std::vector<std::string> >::iterator it = dictionary.begin();
        std::advance( it, rand() % dictionary.size() );
        key = ( *it ).first;
        std::cout << key;
        while( true ) {
            std::vector<std::string> d = dictionary[key];
            if( d.size() < 1 ) break;
            second = d[rand() % d.size()];
            if( second.length() < 1 ) break;
            std::cout << " " << second;
            if( --w < 0 ) break;
            next = key.find_first_of( 32, 0 );
            first = key.substr( next + 1 );
            key = first + " " + second;
        }
        std::cout << "\n";
    }
    void createDictionary( unsigned int kl ) {
        std::string w1, key{};
        size_t wc = 0, pos, next;
        next = fileBuffer.find_first_not_of( 32, 0 );
        if( next == std::string::npos ) return;
        while( wc < kl ) {
            pos = fileBuffer.find_first_of( ' ', next );
            // std::cout << "char pos: " << pos << std::endl;
            w1 = fileBuffer.substr( next, pos - next );
            // std::cout << "word 1: " << w1 << std::endl;
            key += w1 + " ";
            next = fileBuffer.find_first_not_of( 32, pos + 1 );
            if( next == std::string::npos ) {
                std::cout << "found string::npos... exiting..." << std::endl;
                return;
                }
            std::cout << wc++ << std::endl;

        }
        key = key.substr( 0, key.size() - 1 );
        // std::cout << key << std::endl;
        while( true ) {
            // std::cout << "Key: " << key << std::endl;
            next = fileBuffer.find_first_not_of( 32, pos + 1 );
            if( next == std::string::npos ) return;
            w1.clear();
            for (size_t i = next; fileBuffer[i] != 32; ++i){
                if (fileBuffer[i] > 127)
                    continue;
                w1.push_back(fileBuffer[i]);
            }
            pos = fileBuffer.find_first_of( 32, next );
            // std::cout << "Length to end: " << fileBuffer.length() - pos << std::endl;
            // std::cout << "Word 1: " << w1 << std::endl;
            // std::cout << "W1 length: " << w1.length() << std::endl;
            if( w1.length() < 1 ) break;
            if( std::find( dictionary[key].begin(), dictionary[key].end(), w1 ) == dictionary[key].end() ) {
                dictionary[key].push_back( w1 );
                // std::cout << w1 << std::endl;
            }
            if(w1.length() + 2 >= (fileBuffer.length() - pos))
                break;
            key = key.substr( key.find_first_of( 32 ) + 1 ) + " " + w1;
        }
    }
    std::string fileBuffer;
    std::map<std::string, std::vector<std::string> > dictionary;
};


int main( int argc, char* argv[] ) {
    srand( unsigned( time( 0 ) ) );
    Graph g;

    std::string file_name {"../Nadrix_copy.txt"};               // put file path for training data here
    std::cout << "file name input: " << file_name << std::endl;
    g.create( file_name );
    g.createText( 50 );                                         // put number of words to be returned
    return 0;
}
