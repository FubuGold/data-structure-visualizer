#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics.hpp>
#include <tuple>

namespace Global
{

inline float deltaTime;
inline sf::Clock deltaClock;

inline sf::Font textFont("asset/font/PixelifySans-Regular.ttf");
inline sf::Font numberFont("asset/font/Play-Regular.ttf");

enum COLOR_TYPE {
    BACKGROUND,
    NETURAL,
    MAIN,
    HIGHLIGHT,
    DISABLE
};

inline const sf::Color color_set[1][5] =  {
    {sf::Color(0xFFFFFF), sf::Color(0x000000), sf::Color(0x539F64), sf::Color(0xF94842), sf::Color(0xA0A0A0)}
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