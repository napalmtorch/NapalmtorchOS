#pragma once
#include <lib/types.h>

typedef enum
{
    COL4_BLACK,
    COL4_DARKBLUE,
    COL4_DARKGREEN,
    COL4_DARKCYAN,
    COL4_DARKRED,
    COL4_DARKMAGENTA,
    COL4_DARKYELLOW,
    COL4_GRAY,
    COL4_DARKGRAY,
    COL4_BLUE,
    COL4_GREEN,
    COL4_CYAN,
    COL4_RED,
    COL4_MAGENTA,
    COL4_YELLOW,
    COL4_WHITE,
} COL4;

typedef enum
{
    COL32_TRANSPARENT                      = 0x00FFFFFF,
    COL32_ALICEBLUE                        = 0xFFF0F8FF,
    COL32_ANTIQUEWHITE                     = 0xFFFAEBD7,
    COL32_AQUA                             = 0xFF00FFFF,
    COL32_AQUAMARINE                       = 0xFF7FFFD4,
    COL32_AZURE                            = 0xFFF0FFFF,
    COL32_BEIGE                            = 0xFFF5F5DC,
    COL32_BISQUE                           = 0xFFFFE4C4,
    COL32_BLACK                            = 0xFF000000,
    COL32_BLANCHEDALMOND                   = 0xFFFFEBCD,
    COL32_BLUE                             = 0xFF0000FF,
    COL32_BLUEVIOLET                       = 0xFF8A2BE2,
    COL32_BROWN                            = 0xFFA52A2A,
    COL32_BURLYWOOD                        = 0xFFDEB887,
    COL32_CADETBLUE                        = 0xFF5F9EA0,
    COL32_CHARTREUSE                       = 0xFF7FFF00,
    COL32_CHOCOLATE                        = 0xFFD2691E,
    COL32_CORAL                            = 0xFFFF7F50,
    COL32_CORNFLOWERBLUE                   = 0xFF6495ED,
    COL32_CORNSILK                         = 0xFFFFF8DC,
    COL32_CRIMSON                          = 0xFFDC143C,
    COL32_CYAN                             = 0xFF00FFFF,
    COL32_DARKBLUE                         = 0xFF00008B,
    COL32_DARKCYAN                         = 0xFF008B8B,
    COL32_DARKGOLDENROD                    = 0xFFB8860B,
    COL32_DARKGRAY                         = 0xFF404040,
    COL32_DARKGREEN                        = 0xFF006400,
    COL32_DARKKHAKI                        = 0xFFBDB76B,
    COL32_DARKMAGENTA                      = 0xFF8B008B,
    COL32_DARKOLIVEGREEN                   = 0xFF556B2F,
    COL32_DARKORANGE                       = 0xFFFF8C00,
    COL32_DARKORCHID                       = 0xFF9932CC,
    COL32_DARKRED                          = 0xFF8B0000,
    COL32_DARKSALMON                       = 0xFFE9967A,
    COL32_DARKSEAGREEN                     = 0xFF8FBC8B,
    COL32_DARKSLATEBLUE                    = 0xFF483D8B,
    COL32_DARKSLATEGRAY                    = 0xFF2F4F4F,
    COL32_DARKTURQUOISE                    = 0xFF00CED1,
    COL32_DARKVIOLET                       = 0xFF9400D3,
    COL32_DEEPPINK                         = 0xFFFF1493,
    COL32_DEEPSKYBLUE                      = 0xFF00BFFF,
    COL32_DIMGRAY                          = 0xFF696969,
    COL32_DODGERBLUE                       = 0xFF1E90FF,
    COL32_FIREBRICK                        = 0xFFB22222,
    COL32_FLORALWHITE                      = 0xFFFFFAF0,
    COL32_FORESTGREEN                      = 0xFF228B22,
    COL32_FUCHSIA                          = 0xFFFF00FF,
    COL32_GAINSBORO                        = 0xFFDCDCDC,
    COL32_GHOSTWHITE                       = 0xFFF8F8FF,
    COL32_GOLD                             = 0xFFFFD700,
    COL32_GOLDENROD                        = 0xFFDAA520,
    COL32_GRAY                             = 0xFF808080,
    COL32_GREEN                            = 0xFF008000,
    COL32_GREENYELLOW                      = 0xFFADFF2F,
    COL32_HONEYDEW                         = 0xFFF0FFF0,
    COL32_HOTPINK                          = 0xFFFF69B4,
    COL32_INDIANRED                        = 0xFFCD5C5C,
    COL32_INDIGO                           = 0xFF4B0082,
    COL32_IVORY                            = 0xFFFFFFF0,
    COL32_KHAKI                            = 0xFFF0E68C,
    COL32_LAVENDER                         = 0xFFE6E6FA,
    COL32_LAVENDERBLUSH                    = 0xFFFFF0F5,
    COL32_LAWNGREEN                        = 0xFF7CFC00,
    COL32_LEMONCHIFFON                     = 0xFFFFFACD,
    COL32_LIGHTBLUE                        = 0xFFADD8E6,
    COL32_LIGHTCORAL                       = 0xFFF08080,
    COL32_LIGHTCYAN                        = 0xFFE0FFFF,
    COL32_LIGHTGOLDENRODYELLOW             = 0xFFFAFAD2,
    COL32_LIGHTGRAY                        = 0xFFD3D3D3,
    COL32_LIGHTGREEN                       = 0xFF90EE90,
    COL32_LIGHTPINK                        = 0xFFFFB6C1,
    COL32_LIGHTSALMON                      = 0xFFFFA07A,
    COL32_LIGHTSEAGREEN                    = 0xFF20B2AA,
    COL32_LIGHTSKYBLUE                     = 0xFF87CEFA,
    COL32_LIGHTSLATEGRAY                   = 0xFF778899,
    COL32_LIGHTSTEELBLUE                   = 0xFFB0C4DE,
    COL32_LIGHTYELLOW                      = 0xFFFFFFE0,
    COL32_LIME                             = 0xFF00FF00,
    COL32_LIMEGREEN                        = 0xFF32CD32,
    COL32_LINEN                            = 0xFFFAF0E6,
    COL32_MAGENTA                          = 0xFFFF00FF,
    COL32_MAROON                           = 0xFF800000,
    COL32_MEDIUMAQUAMARINE                 = 0xFF66CDAA,
    COL32_MEDIUMBLUE                       = 0xFF0000CD,
    COL32_MEDIUMORCHID                     = 0xFFBA55D3,
    COL32_MEDIUMPURPLE                     = 0xFF9370DB,
    COL32_MEDIUMSEAGREEN                   = 0xFF3CB371,
    COL32_MEDIUMSLATEBLUE                  = 0xFF7B68EE,
    COL32_MEDIUMSPRINGGREEN                = 0xFF00FA9A,
    COL32_MEDIUMTURQUOISE                  = 0xFF48D1CC,
    COL32_MEDIUMVIOLETRED                  = 0xFFC71585,
    COL32_MIDNIGHTBLUE                     = 0xFF191970,
    COL32_MINTCREAM                        = 0xFFF5FFFA,
    COL32_MISTYROSE                        = 0xFFFFE4E1,
    COL32_MOCCASIN                         = 0xFFFFE4B5,
    COL32_NAVAJOWHITE                      = 0xFFFFDEAD,
    COL32_NAVY                             = 0xFF000080,
    COL32_OLDLACE                          = 0xFFFDF5E6,
    COL32_OLIVE                            = 0xFF808000,
    COL32_OLIVEDRAB                        = 0xFF6B8E23,
    COL32_ORANGE                           = 0xFFFFA500,
    COL32_ORANGERED                        = 0xFFFF4500,
    COL32_ORCHID                           = 0xFFDA70D6,
    COL32_PALEGOLDENROD                    = 0xFFEEE8AA,
    COL32_PALEGREEN                        = 0xFF98FB98,
    COL32_PALETURQUOISE                    = 0xFFAFEEEE,
    COL32_PALEVIOLETRED                    = 0xFFDB7093,
    COL32_PAPAYAWHIP                       = 0xFFFFEFD5,
    COL32_PEACHPUFF                        = 0xFFFFDAB9,
    COL32_PERU                             = 0xFFCD853F,
    COL32_PINK                             = 0xFFFFC0CB,
    COL32_PLUM                             = 0xFFDDA0DD,
    COL32_POWDERBLUE                       = 0xFFB0E0E6,
    COL32_PURPLE                           = 0xFF800080,
    COL32_RED                              = 0xFFFF0000,
    COL32_ROSYBROWN                        = 0xFFBC8F8F,
    COL32_ROYALBLUE                        = 0xFF4169E1,
    COL32_SADDLEBROWN                      = 0xFF8B4513,
    COL32_SALMON                           = 0xFFFA8072,
    COL32_SANDYBROWN                       = 0xFFF4A460,
    COL32_SEAGREEN                         = 0xFF2E8B57,
    COL32_SEASHELL                         = 0xFFFFF5EE,
    COL32_SIENNA                           = 0xFFA0522D,
    COL32_SILVER                           = 0xFFC0C0C0,
    COL32_SKYBLUE                          = 0xFF87CEEB,
    COL32_SLATEBLUE                        = 0xFF6A5ACD,
    COL32_SLATEGRAY                        = 0xFF708090,
    COL32_SNOW                             = 0xFFFFFAFA,
    COL32_SPRINGGREEN                      = 0xFF00FF7F,
    COL32_STEELBLUE                        = 0xFF4682B4,
    COL32_TAN                              = 0xFFD2B48C,
    COL32_TEAL                             = 0xFF008080,
    COL32_THISTLE                          = 0xFFD8BFD8,
    COL32_TOMATO                           = 0xFFFF6347,
    COL32_TURQUOISE                        = 0xFF40E0D0,
    COL32_VIOLET                           = 0xFFEE82EE,
    COL32_WHEAT                            = 0xFFF5DEB3,
    COL32_WHITE                            = 0xFFFFFFFF,
    COL32_WHITESMOKE                       = 0xFFF5F5F5,
    COL32_YELLOW                           = 0xFFFFFF00,
    COL32_YELLOWGREEN                      = 0xFF9ACD32,
} COL32;

typedef enum
{
    COLDEPTH_4BPP       = 4,
    COLDEPTH_8BPP       = 8,
    COLDEPTH_16BPP      = 16,
    COLDEPTH_24BPP      = 24,
    COLDEPTH_32BPP      = 32,
} COLDEPTH;

typedef struct
{
    uint8_t a, r, g, b;
} PACKED rgb_t;

static inline COL32 color_4bpp_to_32bpp(COL4 color)
{
    switch (color)
    {
        case 0x0: { return COL32_BLACK; }
        case 0x1: { return COL32_DARKBLUE; }
        case 0x2: { return COL32_DARKGREEN; }
        case 0x3: { return COL32_DARKCYAN; }
        case 0x4: { return COL32_DARKRED; }
        case 0x5: { return COL32_DARKMAGENTA; }
        case 0x6: { return COL32_DARKORANGE; }
        case 0x7: { return COL32_GRAY; }
        case 0x8: { return COL32_DARKGRAY; }
        case 0x9: { return COL32_DODGERBLUE; }
        case 0xA: { return COL32_LIME; }
        case 0xB: { return COL32_CYAN; }
        case 0xC: { return COL32_RED; }
        case 0xD: { return COL32_MAGENTA; }
        case 0xE: { return COL32_YELLOW; }
        case 0xF: { return COL32_WHITE; }
        default: { return COL32_BLACK; }
    }
}

static inline uint32_t color_pack_rgb(rgb_t rgb)
{
    return (rgb.a << 24) | (rgb.r << 16) | (rgb.g << 8) | rgb.b;
}