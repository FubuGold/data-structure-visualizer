#ifndef CONFIG_H
#define CONFIG_H

#include "../../lib/tinyfiledialogs/tinyfiledialogs.h"

#include <SFML/Graphics.hpp>
#include <tuple>

namespace Global
{

inline const int numScene = 8;

enum class SceneState
{
    MENU,
    SLL, HEAP, AVL,
    TRIE, SP, MST,
    SETTING
};
template <typename T>
int toInt(T s) {return static_cast<int>(s);}
inline SceneState curAppState = SceneState::MENU;

inline float deltaTime;
inline sf::Clock deltaClock;

inline sf::Font textFont("asset/font/PixelifySans-Regular.ttf");
inline sf::Font codeFont("asset/font/IBMPlexMono-Regular.ttf");
inline sf::Font numberFont("asset/font/Play-Regular.ttf");

inline float animationSpeed = 1;

enum COLOR_TYPE {
    BACKGROUND,
    NETURAL,
    MAIN,
    HIGHLIGHT,
    DISABLE,
    CODE_HIGHLIGHT,
    SPECIAL
};

inline const sf::Color colorSet[1][7] =  {
    {sf::Color(0xFFFFFFFF), sf::Color(0x000000FF), sf::Color(0x539F64FF), sf::Color(0xF94842FF),
     sf::Color(0xA0A0A0FF), sf::Color(0xFFFF77FF), sf::Color(0x91DAA180)}
};

//======================================================//

enum class SLL_FUNC {
    INSERT,
    FIND,
    REMOVE,
    UPDATE
};

enum class HEAP_FUNC {
    INSERT,
    POP,
    REMOVE_BY_ID,
    UPDATE_BY_ID,
    DOWNHEAP,
    UPHEAP
};

enum class AVL_FUNC {
    INSERT,
    FIND,
    REMOVE,
    UPDATE,
    BALANCE
};

using TrieChild_t = std::tuple<bool, bool, std::vector<std::pair<int,char>>>;

struct TreeStructure
{
    int codeFunction;
    int codeLine;
    int rootId;
    std::map<int,std::string> valueMap;
    std::map<int,std::tuple<int,int,bool>> structureMap;
    std::map<int,TrieChild_t> trieMap;
    friend std::ostream& operator << (std::ostream &out, TreeStructure treeStruct) {
        out << "Tree structure debug\n";
        out << treeStruct.rootId << ' ' << treeStruct.valueMap.size() << ' ' << treeStruct.structureMap.size() << '\n';
        out << "Internal structure:\n";
        for (auto v : treeStruct.structureMap) {
            out << v.first << ' ' << std::get<0>(v.second) << ' ' << std::get<1>(v.second) << '\n';
        }
        return out;
    }
};

//======================================================//

static std::string openFile()
{
    char const * lFilterPatterns[2]={"*.txt","*.inp"};
    char * res = tinyfd_openFileDialog("Choose input file","",2,lFilterPatterns, "Text file", 0);
    if (res == nullptr) return "";
    return std::string(res);
}

}

#endif // CONFIG_H