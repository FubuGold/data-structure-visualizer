#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics.hpp>

namespace Config
{

inline sf::Font font("asset/font/SourceCodePro-Regular.ttf");

enum COLOR_TYPE {
    BACKGROUND,
    NETURAL,
    MAIN,
    HIGHLIGHT,
    DISABLE
};

inline sf::Color color_set[1][5] =  {
    {sf::Color(0xECF8F8), sf::Color(0x000000), sf::Color(0x539F64),
     sf::Color(0xF94842), sf::Color(0xA0A0A0)}
};

}

#endif // CONFIG_H