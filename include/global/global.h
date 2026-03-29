#ifndef CONFIG_H
#define CONFIG_H

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
inline sf::Font numberFont("asset/font/Play-Regular.ttf");
inline float animationSpeed = 1;

enum COLOR_TYPE {
    BACKGROUND,
    NETURAL,
    MAIN,
    HIGHLIGHT,
    DISABLE
};

inline const sf::Color colorSet[1][5] =  {
    {sf::Color(0xFFFFFFFF), sf::Color(0x000000FF), sf::Color(0x539F64FF), sf::Color(0xF94842FF), sf::Color(0xA0A0A0FF)}
};

struct TreeStructure
{
    int rootId;
    std::map<int,std::string> valueMap;
    std::map<int,std::tuple<int,int,bool>> structureMap;
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

}

#endif // CONFIG_H