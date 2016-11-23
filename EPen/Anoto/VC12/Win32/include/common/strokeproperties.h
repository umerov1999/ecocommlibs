/*****************************************************************************
 * Copyright (c) Anoto AB 2007-2009.
 * All rights reserved. This code is Anoto proprietary and confidential, and
 * may be protected by patents. Any use of the code for whatever purpose is
 * subject to separate license terms and conditions.
 ************************************************************************V30*/

#ifndef _strokeproperties_h_
#define _strokeproperties_h_

#include <os/typedefs.h>

#ifdef __cplusplus
extern "C"
{
#endif

/**
 * \defgroup StrokeProperties	Stroke properties defines.
 * \ingroup Anotocommon
 *
 * These are defines of properties that can be set for a PenStroke.
 * @{
 */

/**
 * \defgroup X11Color	X11Color
 * \ingroup StrokeProperties
 *
 * The X11 color set according to CSS3.
 *
 * Usage example:
 *
 *		- <code> struct AnotoColor* ptColor = colCreateRGB(SP_COLOR_AZURE_RGB); </code>
 *		- <code> struct AnotoCOlor* ptColor = colCreateX11Color(SP_COLOR_AZURE_ID); </code>
 *
 * @{
 */

/** aliceblue (R = 240, G = 248, B = 255)*/
#define SP_COLOR_ALICEBLUE_RGB              240, 248, 255
/** aliceblue (R = 240, G = 248, B = 255)*/
#define SP_COLOR_ALICEBLUE_ID                 0

/** antiquewhite (R = 250, G = 235, B = 215)*/
#define SP_COLOR_ANTIQUEWHITE_RGB           250, 235, 215
/** antiquewhite (R = 250, G = 235, B = 215)*/
#define SP_COLOR_ANTIQUEWHITE_ID              1

/** aqua (R = 0, G = 255, B = 255)*/
#define SP_COLOR_AQUA_RGB                     0, 255, 255
/** aqua (R = 0, G = 255, B = 255)*/
#define SP_COLOR_AQUA_ID                      2

/** aquamarine (R = 127, G = 255, B = 212)*/
#define SP_COLOR_AQUAMARINE_RGB             127, 255, 212
/** aquamarine (R = 127, G = 255, B = 212)*/
#define SP_COLOR_AQUAMARINE_ID                3

/** azure (R = 240, G = 255, B = 255)*/
#define SP_COLOR_AZURE_RGB                  240, 255, 255
/** azure (R = 240, G = 255, B = 255)*/
#define SP_COLOR_AZURE_ID                     4

/** beige (R = 245, G = 245, B = 220)*/
#define SP_COLOR_BEIGE_RGB                  245, 245, 220
/** beige (R = 245, G = 245, B = 220)*/
#define SP_COLOR_BEIGE_ID                     5

/** bisque (R = 255, G = 228, B = 196)*/
#define SP_COLOR_BISQUE_RGB                 255, 228, 196
/** bisque (R = 255, G = 228, B = 196)*/
#define SP_COLOR_BISQUE_ID                    6

/** black (R = 0, G = 0, B = 0)*/
#define SP_COLOR_BLACK_RGB                    0,   0,   0
/** black (R = 0, G = 0, B = 0)*/
#define SP_COLOR_BLACK_ID                     7

/** blanchedalmond (R = 255, G = 235, B = 205)*/
#define SP_COLOR_BLANCHEDALMOND_RGB         255, 235, 205
/** blanchedalmond (R = 255, G = 235, B = 205)*/
#define SP_COLOR_BLANCHEDALMOND_ID            8

/** blue (R = 0, G = 0, B = 255)*/
#define SP_COLOR_BLUE_RGB                     0,   0, 255
/** blue (R = 0, G = 0, B = 255)*/
#define SP_COLOR_BLUE_ID                      9

/** blueviolet (R = 138, G = 43, B = 226)*/
#define SP_COLOR_BLUEVIOLET_RGB             138,  43, 226
/** blueviolet (R = 138, G = 43, B = 226)*/
#define SP_COLOR_BLUEVIOLET_ID               10

/** brown (R = 165, G = 42, B = 42)*/
#define SP_COLOR_BROWN_RGB                  165,  42,  42
/** brown (R = 165, G = 42, B = 42)*/
#define SP_COLOR_BROWN_ID                    11

/** burlywood (R = 222, G = 184, B = 135)*/
#define SP_COLOR_BURLYWOOD_RGB              222, 184, 135
/** burlywood (R = 222, G = 184, B = 135)*/
#define SP_COLOR_BURLYWOOD_ID                12

/** cadetblue (R = 95, G = 158, B = 160)*/
#define SP_COLOR_CADETBLUE_RGB               95, 158, 160
/** cadetblue (R = 95, G = 158, B = 160)*/
#define SP_COLOR_CADETBLUE_ID                13

/** chartreuse (R = 127, G = 255, B = 0)*/
#define SP_COLOR_CHARTREUSE_RGB             127, 255,   0
/** chartreuse (R = 127, G = 255, B = 0)*/
#define SP_COLOR_CHARTREUSE_ID               14

/** chocolate (R = 210, G = 105, B = 30)*/
#define SP_COLOR_CHOCOLATE_RGB              210, 105,  30
/** chocolate (R = 210, G = 105, B = 30)*/
#define SP_COLOR_CHOCOLATE_ID                15

/** coral (R = 255, G = 127, B = 80)*/
#define SP_COLOR_CORAL_RGB                  255, 127,  80
/** coral (R = 255, G = 127, B = 80)*/
#define SP_COLOR_CORAL_ID                    16

/** cornflowerblue (R = 100, G = 149, B = 237)*/
#define SP_COLOR_CORNFLOWERBLUE_RGB         100, 149, 237
/** cornflowerblue (R = 100, G = 149, B = 237)*/
#define SP_COLOR_CORNFLOWERBLUE_ID           17

/** cornsilk (R = 255, G = 248, B = 220)*/
#define SP_COLOR_CORNSILK_RGB               255, 248, 220
/** cornsilk (R = 255, G = 248, B = 220)*/
#define SP_COLOR_CORNSILK_ID                 18

/** crimson (R = 220, G = 20, B = 60)*/
#define SP_COLOR_CRIMSON_RGB                220,  20,  60
/** crimson (R = 220, G = 20, B = 60)*/
#define SP_COLOR_CRIMSON_ID                  19

/** cyan (R = 0, G = 255, B = 255)*/
#define SP_COLOR_CYAN_RGB                     0, 255, 255
/** cyan (R = 0, G = 255, B = 255)*/
#define SP_COLOR_CYAN_ID                     20

/** darkblue (R = 0, G = 0, B = 139)*/
#define SP_COLOR_DARKBLUE_RGB                 0,   0, 139
/** darkblue (R = 0, G = 0, B = 139)*/
#define SP_COLOR_DARKBLUE_ID                 21

/** darkcyan (R = 0, G = 139, B = 139)*/
#define SP_COLOR_DARKCYAN_RGB                 0, 139, 139
/** darkcyan (R = 0, G = 139, B = 139)*/
#define SP_COLOR_DARKCYAN_ID                 22

/** darkgoldenrod (R = 184, G = 134, B = 11)*/
#define SP_COLOR_DARKGOLDENROD_RGB          184, 134,  11
/** darkgoldenrod (R = 184, G = 134, B = 11)*/
#define SP_COLOR_DARKGOLDENROD_ID            23

/** darkgray (R = 169, G = 169, B = 169)*/
#define SP_COLOR_DARKGRAY_RGB               169, 169, 169
/** darkgray (R = 169, G = 169, B = 169)*/
#define SP_COLOR_DARKGRAY_ID                 24

/** darkgreen (R = 0, G = 100, B = 0)*/
#define SP_COLOR_DARKGREEN_RGB                0, 100,   0
/** darkgreen (R = 0, G = 100, B = 0)*/
#define SP_COLOR_DARKGREEN_ID                25

/** darkgrey (R = 169, G = 169, B = 169)*/
#define SP_COLOR_DARKGREY_RGB               169, 169, 169
/** darkgrey (R = 169, G = 169, B = 169)*/
#define SP_COLOR_DARKGREY_ID                 26

/** darkkhaki (R = 189, G = 183, B = 107)*/
#define SP_COLOR_DARKKHAKI_RGB              189, 183, 107
/** darkkhaki (R = 189, G = 183, B = 107)*/
#define SP_COLOR_DARKKHAKI_ID                27

/** darkmagenta (R = 139, G = 0, B = 139)*/
#define SP_COLOR_DARKMAGENTA_RGB            139,   0, 139
/** darkmagenta (R = 139, G = 0, B = 139)*/
#define SP_COLOR_DARKMAGENTA_ID              28

/** darkolivegreen (R = 85, G = 107, B = 47)*/
#define SP_COLOR_DARKOLIVEGREEN_RGB          85, 107,  47
/** darkolivegreen (R = 85, G = 107, B = 47)*/
#define SP_COLOR_DARKOLIVEGREEN_ID           29

/** darkorange (R = 255, G = 140, B = 0)*/
#define SP_COLOR_DARKORANGE_RGB             255, 140,   0
/** darkorange (R = 255, G = 140, B = 0)*/
#define SP_COLOR_DARKORANGE_ID               30

/** darkorchid (R = 153, G = 50, B = 204)*/
#define SP_COLOR_DARKORCHID_RGB             153,  50, 204
/** darkorchid (R = 153, G = 50, B = 204)*/
#define SP_COLOR_DARKORCHID_ID               31

/** darkred (R = 139, G = 0, B = 0)*/
#define SP_COLOR_DARKRED_RGB                139,   0,   0
/** darkred (R = 139, G = 0, B = 0)*/
#define SP_COLOR_DARKRED_ID                  32

/** darksalmon (R = 233, G = 150, B = 122)*/
#define SP_COLOR_DARKSALMON_RGB             233, 150, 122
/** darksalmon (R = 233, G = 150, B = 122)*/
#define SP_COLOR_DARKSALMON_ID               33

/** darkseagreen (R = 143, G = 188, B = 143)*/
#define SP_COLOR_DARKSEAGREEN_RGB           143, 188, 143
/** darkseagreen (R = 143, G = 188, B = 143)*/
#define SP_COLOR_DARKSEAGREEN_ID             34

/** darkslateblue (R = 72, G = 61, B = 139)*/
#define SP_COLOR_DARKSLATEBLUE_RGB           72,  61, 139
/** darkslateblue (R = 72, G = 61, B = 139)*/
#define SP_COLOR_DARKSLATEBLUE_ID            35

/** darkslategray (R = 47, G = 79, B = 79)*/
#define SP_COLOR_DARKSLATEGRAY_RGB           47,  79,  79
/** darkslategray (R = 47, G = 79, B = 79)*/
#define SP_COLOR_DARKSLATEGRAY_ID            36

/** darkslategrey (R = 47, G = 79, B = 79)*/
#define SP_COLOR_DARKSLATEGREY_RGB           47,  79,  79
/** darkslategrey (R = 47, G = 79, B = 79)*/
#define SP_COLOR_DARKSLATEGREY_ID            37

/** darkturquoise (R = 0, G = 206, B = 209)*/
#define SP_COLOR_DARKTURQUOISE_RGB            0, 206, 209
/** darkturquoise (R = 0, G = 206, B = 209)*/
#define SP_COLOR_DARKTURQUOISE_ID            38

/** darkviolet (R = 148, G = 0, B = 211)*/
#define SP_COLOR_DARKVIOLET_RGB             148,   0, 211
/** darkviolet (R = 148, G = 0, B = 211)*/
#define SP_COLOR_DARKVIOLET_ID               39

/** deeppink (R = 255, G = 20, B = 147)*/
#define SP_COLOR_DEEPPINK_RGB               255,  20, 147
/** deeppink (R = 255, G = 20, B = 147)*/
#define SP_COLOR_DEEPPINK_ID                 40

/** deepskyblue (R = 0, G = 191, B = 255)*/
#define SP_COLOR_DEEPSKYBLUE_RGB              0, 191, 255
/** deepskyblue (R = 0, G = 191, B = 255)*/
#define SP_COLOR_DEEPSKYBLUE_ID              41

/** dimgray (R = 105, G = 105, B = 105)*/
#define SP_COLOR_DIMGRAY_RGB                105, 105, 105
/** dimgray (R = 105, G = 105, B = 105)*/
#define SP_COLOR_DIMGRAY_ID                  42

/** dimgrey (R = 105, G = 105, B = 105)*/
#define SP_COLOR_DIMGREY_RGB                105, 105, 105
/** dimgrey (R = 105, G = 105, B = 105)*/
#define SP_COLOR_DIMGREY_ID                  43

/** dodgerblue (R = 30, G = 144, B = 255)*/
#define SP_COLOR_DODGERBLUE_RGB              30, 144, 255
/** dodgerblue (R = 30, G = 144, B = 255)*/
#define SP_COLOR_DODGERBLUE_ID               44

/** firebrick (R = 178, G = 34, B = 34)*/
#define SP_COLOR_FIREBRICK_RGB              178,  34,  34
/** firebrick (R = 178, G = 34, B = 34)*/
#define SP_COLOR_FIREBRICK_ID                45

/** floralwhite (R = 255, G = 250, B = 240)*/
#define SP_COLOR_FLORALWHITE_RGB            255, 250, 240
/** floralwhite (R = 255, G = 250, B = 240)*/
#define SP_COLOR_FLORALWHITE_ID              46

/** forestgreen (R = 34, G = 139, B = 34)*/
#define SP_COLOR_FORESTGREEN_RGB             34, 139,  34
/** forestgreen (R = 34, G = 139, B = 34)*/
#define SP_COLOR_FORESTGREEN_ID              47

/** fuchsia (R = 255, G = 0, B = 255)*/
#define SP_COLOR_FUCHSIA_RGB                255,   0, 255
/** fuchsia (R = 255, G = 0, B = 255)*/
#define SP_COLOR_FUCHSIA_ID                  48

/** gainsboro (R = 220, G = 220, B = 220)*/
#define SP_COLOR_GAINSBORO_RGB              220, 220, 220
/** gainsboro (R = 220, G = 220, B = 220)*/
#define SP_COLOR_GAINSBORO_ID                49

/** ghostwhite (R = 248, G = 248, B = 255)*/
#define SP_COLOR_GHOSTWHITE_RGB             248, 248, 255
/** ghostwhite (R = 248, G = 248, B = 255)*/
#define SP_COLOR_GHOSTWHITE_ID               50

/** gold (R = 255, G = 215, B = 0)*/
#define SP_COLOR_GOLD_RGB                   255, 215,   0
/** gold (R = 255, G = 215, B = 0)*/
#define SP_COLOR_GOLD_ID                     51

/** goldenrod (R = 218, G = 165, B = 32)*/
#define SP_COLOR_GOLDENROD_RGB              218, 165,  32
/** goldenrod (R = 218, G = 165, B = 32)*/
#define SP_COLOR_GOLDENROD_ID                52

/** gray (R = 128, G = 128, B = 128)*/
#define SP_COLOR_GRAY_RGB                   128, 128, 128
/** gray (R = 128, G = 128, B = 128)*/
#define SP_COLOR_GRAY_ID                     53

/** green (R = 0, G = 128, B = 0)*/
#define SP_COLOR_GREEN_RGB                    0, 128,   0
/** green (R = 0, G = 128, B = 0)*/
#define SP_COLOR_GREEN_ID                    54

/** greenyellow (R = 173, G = 255, B = 47)*/
#define SP_COLOR_GREENYELLOW_RGB            173, 255,  47
/** greenyellow (R = 173, G = 255, B = 47)*/
#define SP_COLOR_GREENYELLOW_ID              55

/** grey (R = 128, G = 128, B = 128)*/
#define SP_COLOR_GREY_RGB                   128, 128, 128
/** grey (R = 128, G = 128, B = 128)*/
#define SP_COLOR_GREY_ID                     56

/** honeydew (R = 240, G = 255, B = 240)*/
#define SP_COLOR_HONEYDEW_RGB               240, 255, 240
/** honeydew (R = 240, G = 255, B = 240)*/
#define SP_COLOR_HONEYDEW_ID                 57

/** hotpink (R = 255, G = 105, B = 180)*/
#define SP_COLOR_HOTPINK_RGB                255, 105, 180
/** hotpink (R = 255, G = 105, B = 180)*/
#define SP_COLOR_HOTPINK_ID                  58

/** indianred (R = 205, G = 92, B = 92)*/
#define SP_COLOR_INDIANRED_RGB              205,  92,  92
/** indianred (R = 205, G = 92, B = 92)*/
#define SP_COLOR_INDIANRED_ID                59

/** indigo (R = 75, G = 0, B = 130)*/
#define SP_COLOR_INDIGO_RGB                  75,   0, 130
/** indigo (R = 75, G = 0, B = 130)*/
#define SP_COLOR_INDIGO_ID                   60

/** ivory (R = 255, G = 255, B = 240)*/
#define SP_COLOR_IVORY_RGB                  255, 255, 240
/** ivory (R = 255, G = 255, B = 240)*/
#define SP_COLOR_IVORY_ID                    61

/** khaki (R = 240, G = 230, B = 140)*/
#define SP_COLOR_KHAKI_RGB                  240, 230, 140
/** khaki (R = 240, G = 230, B = 140)*/
#define SP_COLOR_KHAKI_ID                    62

/** lavender (R = 230, G = 230, B = 250)*/
#define SP_COLOR_LAVENDER_RGB               230, 230, 250
/** lavender (R = 230, G = 230, B = 250)*/
#define SP_COLOR_LAVENDER_ID                 63

/** lavenderblush (R = 255, G = 240, B = 245)*/
#define SP_COLOR_LAVENDERBLUSH_RGB          255, 240, 245
/** lavenderblush (R = 255, G = 240, B = 245)*/
#define SP_COLOR_LAVENDERBLUSH_ID            64

/** lawngreen (R = 124, G = 252, B = 0)*/
#define SP_COLOR_LAWNGREEN_RGB              124, 252,   0
/** lawngreen (R = 124, G = 252, B = 0)*/
#define SP_COLOR_LAWNGREEN_ID                65

/** lemonchiffon (R = 255, G = 250, B = 205)*/
#define SP_COLOR_LEMONCHIFFON_RGB           255, 250, 205
/** lemonchiffon (R = 255, G = 250, B = 205)*/
#define SP_COLOR_LEMONCHIFFON_ID             66

/** lightblue (R = 173, G = 216, B = 230)*/
#define SP_COLOR_LIGHTBLUE_RGB              173, 216, 230
/** lightblue (R = 173, G = 216, B = 230)*/
#define SP_COLOR_LIGHTBLUE_ID                67

/** lightcoral (R = 240, G = 128, B = 128)*/
#define SP_COLOR_LIGHTCORAL_RGB             240, 128, 128
/** lightcoral (R = 240, G = 128, B = 128)*/
#define SP_COLOR_LIGHTCORAL_ID               68

/** lightcyan (R = 224, G = 255, B = 255)*/
#define SP_COLOR_LIGHTCYAN_RGB              224, 255, 255
/** lightcyan (R = 224, G = 255, B = 255)*/
#define SP_COLOR_LIGHTCYAN_ID                69

/** lightgoldenrodyellow (R = 250, G = 250, B = 210)*/
#define SP_COLOR_LIGHTGOLDENRODYELLOW_RGB   250, 250, 210
/** lightgoldenrodyellow (R = 250, G = 250, B = 210)*/
#define SP_COLOR_LIGHTGOLDENRODYELLOW_ID     70

/** lightgray (R = 211, G = 211, B = 211)*/
#define SP_COLOR_LIGHTGRAY_RGB              211, 211, 211
/** lightgray (R = 211, G = 211, B = 211)*/
#define SP_COLOR_LIGHTGRAY_ID                71

/** lightgreen (R = 144, G = 238, B = 144)*/
#define SP_COLOR_LIGHTGREEN_RGB             144, 238, 144
/** lightgreen (R = 144, G = 238, B = 144)*/
#define SP_COLOR_LIGHTGREEN_ID               72

/** lightgrey (R = 211, G = 211, B = 211)*/
#define SP_COLOR_LIGHTGREY_RGB              211, 211, 211
/** lightgrey (R = 211, G = 211, B = 211)*/
#define SP_COLOR_LIGHTGREY_ID                73

/** lightpink (R = 255, G = 182, B = 193)*/
#define SP_COLOR_LIGHTPINK_RGB              255, 182, 193
/** lightpink (R = 255, G = 182, B = 193)*/
#define SP_COLOR_LIGHTPINK_ID                74

/** lightsalmon (R = 255, G = 160, B = 122)*/
#define SP_COLOR_LIGHTSALMON_RGB            255, 160, 122
/** lightsalmon (R = 255, G = 160, B = 122)*/
#define SP_COLOR_LIGHTSALMON_ID              75

/** lightseagreen (R = 32, G = 178, B = 170)*/
#define SP_COLOR_LIGHTSEAGREEN_RGB           32, 178, 170
/** lightseagreen (R = 32, G = 178, B = 170)*/
#define SP_COLOR_LIGHTSEAGREEN_ID            76

/** lightskyblue (R = 135, G = 206, B = 250)*/
#define SP_COLOR_LIGHTSKYBLUE_RGB           135, 206, 250
/** lightskyblue (R = 135, G = 206, B = 250)*/
#define SP_COLOR_LIGHTSKYBLUE_ID             77

/** lightslategray (R = 119, G = 136, B = 153)*/
#define SP_COLOR_LIGHTSLATEGRAY_RGB         119, 136, 153
/** lightslategray (R = 119, G = 136, B = 153)*/
#define SP_COLOR_LIGHTSLATEGRAY_ID           78

/** lightslategrey (R = 119, G = 136, B = 153)*/
#define SP_COLOR_LIGHTSLATEGREY_RGB         119, 136, 153
/** lightslategrey (R = 119, G = 136, B = 153)*/
#define SP_COLOR_LIGHTSLATEGREY_ID           79

/** lightsteelblue (R = 176, G = 196, B = 222)*/
#define SP_COLOR_LIGHTSTEELBLUE_RGB         176, 196, 222
/** lightsteelblue (R = 176, G = 196, B = 222)*/
#define SP_COLOR_LIGHTSTEELBLUE_ID           80

/** lightyellow (R = 255, G = 255, B = 224)*/
#define SP_COLOR_LIGHTYELLOW_RGB            255, 255, 224
/** lightyellow (R = 255, G = 255, B = 224)*/
#define SP_COLOR_LIGHTYELLOW_ID              81

/** lime (R = 0, G = 255, B = 0)*/
#define SP_COLOR_LIME_RGB                     0, 255,   0
/** lime (R = 0, G = 255, B = 0)*/
#define SP_COLOR_LIME_ID                     82

/** limegreen (R = 50, G = 205, B = 50)*/
#define SP_COLOR_LIMEGREEN_RGB               50, 205,  50
/** limegreen (R = 50, G = 205, B = 50)*/
#define SP_COLOR_LIMEGREEN_ID                83

/** linen (R = 250, G = 240, B = 230)*/
#define SP_COLOR_LINEN_RGB                  250, 240, 230
/** linen (R = 250, G = 240, B = 230)*/
#define SP_COLOR_LINEN_ID                    84

/** magenta (R = 255, G = 0, B = 255)*/
#define SP_COLOR_MAGENTA_RGB                255,   0, 255
/** magenta (R = 255, G = 0, B = 255)*/
#define SP_COLOR_MAGENTA_ID                  85

/** maroon (R = 128, G = 0, B = 0)*/
#define SP_COLOR_MAROON_RGB                 128,   0,   0
/** maroon (R = 128, G = 0, B = 0)*/
#define SP_COLOR_MAROON_ID                   86

/** mediumaquamarine (R = 102, G = 205, B = 170)*/
#define SP_COLOR_MEDIUMAQUAMARINE_RGB       102, 205, 170
/** mediumaquamarine (R = 102, G = 205, B = 170)*/
#define SP_COLOR_MEDIUMAQUAMARINE_ID         87

/** mediumblue (R = 0, G = 0, B = 205)*/
#define SP_COLOR_MEDIUMBLUE_RGB               0,   0, 205
/** mediumblue (R = 0, G = 0, B = 205)*/
#define SP_COLOR_MEDIUMBLUE_ID               88

/** mediumorchid (R = 186, G = 85, B = 211)*/
#define SP_COLOR_MEDIUMORCHID_RGB           186,  85, 211
/** mediumorchid (R = 186, G = 85, B = 211)*/
#define SP_COLOR_MEDIUMORCHID_ID             89

/** mediumpurple (R = 147, G = 112, B = 219)*/
#define SP_COLOR_MEDIUMPURPLE_RGB           147, 112, 219
/** mediumpurple (R = 147, G = 112, B = 219)*/
#define SP_COLOR_MEDIUMPURPLE_ID             90

/** mediumseagreen (R = 60, G = 179, B = 113)*/
#define SP_COLOR_MEDIUMSEAGREEN_RGB          60, 179, 113
/** mediumseagreen (R = 60, G = 179, B = 113)*/
#define SP_COLOR_MEDIUMSEAGREEN_ID           91

/** mediumslateblue (R = 123, G = 104, B = 238)*/
#define SP_COLOR_MEDIUMSLATEBLUE_RGB        123, 104, 238
/** mediumslateblue (R = 123, G = 104, B = 238)*/
#define SP_COLOR_MEDIUMSLATEBLUE_ID          92

/** mediumspringgreen (R = 0, G = 250, B = 154)*/
#define SP_COLOR_MEDIUMSPRINGGREEN_RGB        0, 250, 154
/** mediumspringgreen (R = 0, G = 250, B = 154)*/
#define SP_COLOR_MEDIUMSPRINGGREEN_ID        93

/** mediumturquoise (R = 72, G = 209, B = 204)*/
#define SP_COLOR_MEDIUMTURQUOISE_RGB         72, 209, 204
/** mediumturquoise (R = 72, G = 209, B = 204)*/
#define SP_COLOR_MEDIUMTURQUOISE_ID          94

/** mediumvioletred (R = 199, G = 21, B = 133)*/
#define SP_COLOR_MEDIUMVIOLETRED_RGB        199,  21, 133
/** mediumvioletred (R = 199, G = 21, B = 133)*/
#define SP_COLOR_MEDIUMVIOLETRED_ID          95

/** midnightblue (R = 25, G = 25, B = 112)*/
#define SP_COLOR_MIDNIGHTBLUE_RGB            25,  25, 112
/** midnightblue (R = 25, G = 25, B = 112)*/
#define SP_COLOR_MIDNIGHTBLUE_ID             96

/** mintcream (R = 245, G = 255, B = 250)*/
#define SP_COLOR_MINTCREAM_RGB              245, 255, 250
/** mintcream (R = 245, G = 255, B = 250)*/
#define SP_COLOR_MINTCREAM_ID                97

/** mistyrose (R = 255, G = 228, B = 225)*/
#define SP_COLOR_MISTYROSE_RGB              255, 228, 225
/** mistyrose (R = 255, G = 228, B = 225)*/
#define SP_COLOR_MISTYROSE_ID                98

/** moccasin (R = 255, G = 228, B = 181)*/
#define SP_COLOR_MOCCASIN_RGB               255, 228, 181
/** moccasin (R = 255, G = 228, B = 181)*/
#define SP_COLOR_MOCCASIN_ID                 99

/** navajowhite (R = 255, G = 222, B = 173)*/
#define SP_COLOR_NAVAJOWHITE_RGB            255, 222, 173
/** navajowhite (R = 255, G = 222, B = 173)*/
#define SP_COLOR_NAVAJOWHITE_ID             100

/** navy (R = 0, G = 0, B = 128)*/
#define SP_COLOR_NAVY_RGB                     0,   0, 128
/** navy (R = 0, G = 0, B = 128)*/
#define SP_COLOR_NAVY_ID                    101

/** oldlace (R = 253, G = 245, B = 230)*/
#define SP_COLOR_OLDLACE_RGB                253, 245, 230
/** oldlace (R = 253, G = 245, B = 230)*/
#define SP_COLOR_OLDLACE_ID                 102

/** olive (R = 128, G = 128, B = 0)*/
#define SP_COLOR_OLIVE_RGB                  128, 128,   0
/** olive (R = 128, G = 128, B = 0)*/
#define SP_COLOR_OLIVE_ID                   103

/** olivedrab (R = 107, G = 142, B = 35)*/
#define SP_COLOR_OLIVEDRAB_RGB              107, 142,  35
/** olivedrab (R = 107, G = 142, B = 35)*/
#define SP_COLOR_OLIVEDRAB_ID               104

/** orange (R = 255, G = 165, B = 0)*/
#define SP_COLOR_ORANGE_RGB                 255, 165,   0
/** orange (R = 255, G = 165, B = 0)*/
#define SP_COLOR_ORANGE_ID                  105

/** orangered (R = 255, G = 69, B = 0)*/
#define SP_COLOR_ORANGERED_RGB              255,  69,   0
/** orangered (R = 255, G = 69, B = 0)*/
#define SP_COLOR_ORANGERED_ID               106

/** orchid (R = 218, G = 112, B = 214)*/
#define SP_COLOR_ORCHID_RGB                 218, 112, 214
/** orchid (R = 218, G = 112, B = 214)*/
#define SP_COLOR_ORCHID_ID                  107

/** palegoldenrod (R = 238, G = 232, B = 170)*/
#define SP_COLOR_PALEGOLDENROD_RGB          238, 232, 170
/** palegoldenrod (R = 238, G = 232, B = 170)*/
#define SP_COLOR_PALEGOLDENROD_ID           108

/** palegreen (R = 152, G = 251, B = 152)*/
#define SP_COLOR_PALEGREEN_RGB              152, 251, 152
/** palegreen (R = 152, G = 251, B = 152)*/
#define SP_COLOR_PALEGREEN_ID               109

/** paleturquoise (R = 175, G = 238, B = 238)*/
#define SP_COLOR_PALETURQUOISE_RGB          175, 238, 238
/** paleturquoise (R = 175, G = 238, B = 238)*/
#define SP_COLOR_PALETURQUOISE_ID           110

/** palevioletred (R = 219, G = 112, B = 147)*/
#define SP_COLOR_PALEVIOLETRED_RGB          219, 112, 147
/** palevioletred (R = 219, G = 112, B = 147)*/
#define SP_COLOR_PALEVIOLETRED_ID           111

/** papayawhip (R = 255, G = 239, B = 213)*/
#define SP_COLOR_PAPAYAWHIP_RGB             255, 239, 213
/** papayawhip (R = 255, G = 239, B = 213)*/
#define SP_COLOR_PAPAYAWHIP_ID              112

/** peachpuff (R = 255, G = 218, B = 185)*/
#define SP_COLOR_PEACHPUFF_RGB              255, 218, 185
/** peachpuff (R = 255, G = 218, B = 185)*/
#define SP_COLOR_PEACHPUFF_ID               113

/** peru (R = 205, G = 133, B = 63)*/
#define SP_COLOR_PERU_RGB                   205, 133,  63
/** peru (R = 205, G = 133, B = 63)*/
#define SP_COLOR_PERU_ID                    114

/** pink (R = 255, G = 192, B = 203)*/
#define SP_COLOR_PINK_RGB                   255, 192, 203
/** pink (R = 255, G = 192, B = 203)*/
#define SP_COLOR_PINK_ID                    115

/** plum (R = 221, G = 160, B = 221)*/
#define SP_COLOR_PLUM_RGB                   221, 160, 221
/** plum (R = 221, G = 160, B = 221)*/
#define SP_COLOR_PLUM_ID                    116

/** powderblue (R = 176, G = 224, B = 230)*/
#define SP_COLOR_POWDERBLUE_RGB             176, 224, 230
/** powderblue (R = 176, G = 224, B = 230)*/
#define SP_COLOR_POWDERBLUE_ID              117

/** purple (R = 128, G = 0, B = 128)*/
#define SP_COLOR_PURPLE_RGB                 128,   0, 128
/** purple (R = 128, G = 0, B = 128)*/
#define SP_COLOR_PURPLE_ID                  118

/** red (R = 255, G = 0, B = 0)*/
#define SP_COLOR_RED_RGB                    255,   0,   0
/** red (R = 255, G = 0, B = 0)*/
#define SP_COLOR_RED_ID                     119

/** rosybrown (R = 188, G = 143, B = 143)*/
#define SP_COLOR_ROSYBROWN_RGB              188, 143, 143
/** rosybrown (R = 188, G = 143, B = 143)*/
#define SP_COLOR_ROSYBROWN_ID               120

/** royalblue (R = 65, G = 105, B = 225)*/
#define SP_COLOR_ROYALBLUE_RGB               65, 105, 225
/** royalblue (R = 65, G = 105, B = 225)*/
#define SP_COLOR_ROYALBLUE_ID               121

/** saddlebrown (R = 139, G = 69, B = 19)*/
#define SP_COLOR_SADDLEBROWN_RGB            139,  69,  19
/** saddlebrown (R = 139, G = 69, B = 19)*/
#define SP_COLOR_SADDLEBROWN_ID             122

/** salmon (R = 250, G = 128, B = 114)*/
#define SP_COLOR_SALMON_RGB                 250, 128, 114
/** salmon (R = 250, G = 128, B = 114)*/
#define SP_COLOR_SALMON_ID                  123

/** sandybrown (R = 244, G = 164, B = 96)*/
#define SP_COLOR_SANDYBROWN_RGB             244, 164,  96
/** sandybrown (R = 244, G = 164, B = 96)*/
#define SP_COLOR_SANDYBROWN_ID              124

/** seagreen (R = 46, G = 139, B = 87)*/
#define SP_COLOR_SEAGREEN_RGB                46, 139,  87
/** seagreen (R = 46, G = 139, B = 87)*/
#define SP_COLOR_SEAGREEN_ID                125

/** seashell (R = 255, G = 245, B = 238)*/
#define SP_COLOR_SEASHELL_RGB               255, 245, 238
/** seashell (R = 255, G = 245, B = 238)*/
#define SP_COLOR_SEASHELL_ID                126

/** sienna (R = 160, G = 82, B = 45)*/
#define SP_COLOR_SIENNA_RGB                 160,  82,  45
/** sienna (R = 160, G = 82, B = 45)*/
#define SP_COLOR_SIENNA_ID                  127

/** silver (R = 192, G = 192, B = 192)*/
#define SP_COLOR_SILVER_RGB                 192, 192, 192
/** silver (R = 192, G = 192, B = 192)*/
#define SP_COLOR_SILVER_ID                  128

/** skyblue (R = 135, G = 206, B = 235)*/
#define SP_COLOR_SKYBLUE_RGB                135, 206, 235
/** skyblue (R = 135, G = 206, B = 235)*/
#define SP_COLOR_SKYBLUE_ID                 129

/** slateblue (R = 106, G = 90, B = 205)*/
#define SP_COLOR_SLATEBLUE_RGB              106,  90, 205
/** slateblue (R = 106, G = 90, B = 205)*/
#define SP_COLOR_SLATEBLUE_ID               130

/** slategray (R = 112, G = 128, B = 144)*/
#define SP_COLOR_SLATEGRAY_RGB              112, 128, 144
/** slategray (R = 112, G = 128, B = 144)*/
#define SP_COLOR_SLATEGRAY_ID               131

/** slategrey (R = 112, G = 128, B = 144)*/
#define SP_COLOR_SLATEGREY_RGB              112, 128, 144
/** slategrey (R = 112, G = 128, B = 144)*/
#define SP_COLOR_SLATEGREY_ID               132

/** snow (R = 255, G = 250, B = 250)*/
#define SP_COLOR_SNOW_RGB                   255, 250, 250
/** snow (R = 255, G = 250, B = 250)*/
#define SP_COLOR_SNOW_ID                    133

/** springgreen (R = 0, G = 255, B = 127)*/
#define SP_COLOR_SPRINGGREEN_RGB              0, 255, 127
/** springgreen (R = 0, G = 255, B = 127)*/
#define SP_COLOR_SPRINGGREEN_ID             134

/** steelblue (R = 70, G = 130, B = 180)*/
#define SP_COLOR_STEELBLUE_RGB               70, 130, 180
/** steelblue (R = 70, G = 130, B = 180)*/
#define SP_COLOR_STEELBLUE_ID               135

/** tan (R = 210, G = 180, B = 140)*/
#define SP_COLOR_TAN_RGB                    210, 180, 140
/** tan (R = 210, G = 180, B = 140)*/
#define SP_COLOR_TAN_ID                     136

/** teal (R = 0, G = 128, B = 128)*/
#define SP_COLOR_TEAL_RGB                     0, 128, 128
/** teal (R = 0, G = 128, B = 128)*/
#define SP_COLOR_TEAL_ID                    137

/** thistle (R = 216, G = 191, B = 216)*/
#define SP_COLOR_THISTLE_RGB                216, 191, 216
/** thistle (R = 216, G = 191, B = 216)*/
#define SP_COLOR_THISTLE_ID                 138

/** tomato (R = 255, G = 99, B = 71)*/
#define SP_COLOR_TOMATO_RGB                 255,  99,  71
/** tomato (R = 255, G = 99, B = 71)*/
#define SP_COLOR_TOMATO_ID                  139

/** turquoise (R = 64, G = 224, B = 208)*/
#define SP_COLOR_TURQUOISE_RGB               64, 224, 208
/** turquoise (R = 64, G = 224, B = 208)*/
#define SP_COLOR_TURQUOISE_ID               140

/** violet (R = 238, G = 130, B = 238)*/
#define SP_COLOR_VIOLET_RGB                 238, 130, 238
/** violet (R = 238, G = 130, B = 238)*/
#define SP_COLOR_VIOLET_ID                  141

/** wheat (R = 245, G = 222, B = 179)*/
#define SP_COLOR_WHEAT_RGB                  245, 222, 179
/** wheat (R = 245, G = 222, B = 179)*/
#define SP_COLOR_WHEAT_ID                   142

/** white (R = 255, G = 255, B = 255)*/
#define SP_COLOR_WHITE_RGB                  255, 255, 255
/** white (R = 255, G = 255, B = 255)*/
#define SP_COLOR_WHITE_ID                   143

/** whitesmoke (R = 245, G = 245, B = 245)*/
#define SP_COLOR_WHITESMOKE_RGB             245, 245, 245
/** whitesmoke (R = 245, G = 245, B = 245)*/
#define SP_COLOR_WHITESMOKE_ID              144

/** yellow (R = 255, G = 255, B = 0)*/
#define SP_COLOR_YELLOW_RGB                 255, 255,   0
/** yellow (R = 255, G = 255, B = 0)*/
#define SP_COLOR_YELLOW_ID                  145

/** yellowgreen (R = 154, G = 205, B = 50)*/
#define SP_COLOR_YELLOWGREEN_RGB            154, 205,  50
/** yellowgreen (R = 154, G = 205, B = 50)*/
#define SP_COLOR_YELLOWGREEN_ID             146

/** Last defined color id */
#define SP_COLOR_MAX 147

/** Color is not defined. 65535 corresponds to the maximum value of an u16 */
#define SP_COLOR_UNDEFINED 65535


/** @} X11Color*/



////////////////////////////////////////////////////////////////////////
// Tip Type
////////////////////////////////////////////////////////////////////////
/** 
 * \defgroup TipType Type of pentip
 *
 * This property defines the type of tip used to produce a stroke. Type
 * means fx ball point or a marker pen tip. The type does not in any way
 * define the geometry of the tip, just the mechanism used.
 *
 * @{
 */

/** Type is not known */
#define SP_TIP_TYPE_UNKNOWN		0	
/** Standard ball point tip. */
#define SP_TIP_TYPE_BALLPOINT	1	
/** Tip used by whiteboard and flipchart pens */
#define SP_TIP_TYPE_MARKER		2	

/** @} TipType */

/** @} StrokeProperties */

#ifdef __cplusplus
}
#endif

#endif  // _strokeproperties_h_


