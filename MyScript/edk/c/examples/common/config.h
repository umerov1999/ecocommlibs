#ifndef EDK_C_EXAMPLES_COMMON_CONFIG_H
/**
 * @file edk/c/examples/common/config.h
 *
 * This file contains common configuraton parameters used by all examples.
 */


// -- path to the MyScript SDK directory ---------------------------------------

#ifndef SDK_DIR
  #define SDK_DIR "./MyScript" //${executable}/../../../../.."
#endif // #ifndef SDK_DIR


// -- MyScript Engine initialization  ------------------------------------------

#include <c/MyScriptEngine.h>

#ifndef CONF_SUFFIX
  #define CONF_SUFFIX
#endif

#include <../src/MyScriptEngine-dlHelper.c>


// -- path to the MyScript SDK resource directory ------------------------------

#ifndef RESOURCE_DIR
  #define RESOURCE_DIR SDK_DIR "/resources"
#endif // #ifndef RESOURCE_DIR


// -- language -----------------------------------------------------------------


// TODO: set this, e.g.: "en_GB" for English (United Kingdom).
/*
#ifndef LANG
  #define LANG "en_US"
#endif // #ifndef LANG
*/  

// -- handwriting style  -------------------------------------------------------

// TODO: set this to "iso", "hpr", "cur" or "superimposed"
#ifndef HWSTYLE
  #define HWSTYLE "cur"
#endif // #ifndef HWSTYLE


// -- resources ----------------------------------------------------------------
/*
#ifndef AK_ISO_RES
  #define AK_ISO_RES RESOURCE_DIR "/" LANG "/" LANG "-ak-iso.res"
#endif // #ifndef AK_ISO_RES

#ifndef AK_HPR_RES
  #define AK_HPR_RES RESOURCE_DIR "/" LANG "/" LANG "-ak-hpr.res"
#endif // #ifndef AK_HPR_RES

#ifndef AK_CUR_RES
  #define AK_CUR_RES RESOURCE_DIR "/" LANG "/" LANG "-ak-cur.res"
#endif // #ifndef AK_CUR_RES

#ifndef AK_SUPERIMPOSED_RES
  #define AK_SUPERIMPOSED_RES RESOURCE_DIR "/" LANG "/" LANG "-ak-superimposed.res"
#endif // #ifndef AK_RES

#ifndef AK_RES
  #define AK_RES RESOURCE_DIR "/" LANG "/" LANG "-ak-" HWSTYLE ".res"
#endif // #ifndef AK_RES

#ifndef LK_TEXT_RES
  #define LK_TEXT_RES RESOURCE_DIR "/" LANG "/" LANG "-lk-text.res"
#endif // #ifndef LK_TEXT_RES
 */


#ifndef LK_ENGLISH_SECONDARY_1_RES
  #define LK_ENGLISH_SECONDARY_1_RES RESOURCE_DIR "/mul/mul-lk-english_secondary_1.res"
#endif // #ifndef LK_ENGLISH_SECONDARY_1_RES

#ifndef LK_ENGLISH_SECONDARY_2_RES
  #define LK_ENGLISH_SECONDARY_2_RES RESOURCE_DIR "/mul/mul-lk-english_secondary_2.res"
#endif // #ifndef LK_ENGLISH_SECONDARY_2_RES

#ifndef LK_ENGLISH_SECONDARY_3_RES
  #define LK_ENGLISH_SECONDARY_3_RES RESOURCE_DIR "/mul/mul-lk-english_secondary_3.res"
#endif // #ifndef LK_ENGLISH_SECONDARY_3_RES

#ifndef SHAPE_KNOWLEDGE_RES
  #define SHAPE_KNOWLEDGE_RES RESOURCE_DIR "/shape/shk-standard.res"
#endif // #ifndef SHAPE_KNOWLEDGE_RES

#ifndef MATH_AK_RES
  #define MATH_AK_RES RESOURCE_DIR "/math/math-ak.res"
#endif // #ifndef MATH_AK_RES

#ifndef MATH_GRM_RES
  #define MATH_GRM_RES RESOURCE_DIR "/math/math-grm-standard.res"
#endif // #ifndef MATH_GRM_RES

#ifndef ANALYZER_KNOWLEDGE_RES
  #define ANALYZER_KNOWLEDGE_RES RESOURCE_DIR "/analyzer/ank-standard.res"
#endif // #ifndef ANALYZER_KNOWLEDGE_RES

#ifndef ANALYZER_SHAPE_KNOWLEDGE_RES
  #define ANALYZER_SHAPE_KNOWLEDGE_RES RESOURCE_DIR "/analyzer/shk-analyzer.res"
#endif // #ifndef ANALYZER_SHAPE_KNOWLEDGE_RES

#ifndef MUSIC_AK_RES
  #define MUSIC_AK_RES RESOURCE_DIR "/music/music-ak.res"
#endif // #ifndef MUSIC_AK_RES

#ifndef MUSIC_GRM_RES
  #define MUSIC_GRM_RES RESOURCE_DIR "/music/music-grm-standard.res"
#endif // #ifndef MUSIC_GRM_RES


// -- path to the MyScript SDK data directory ----------------------------------

#ifndef DATA_DIR
  #define DATA_DIR SDK_DIR "/edk/examples/data"
#endif // #ifndef DATA_DIR


// -- path to the MyScript SDK data directory ----------------------------------

#ifndef OUTPUT_DIR
  #define OUTPUT_DIR "${executable}"
#endif // #ifndef OUTPUT_DIR


#endif // #ifndef EDK_C_EXAMPLES_COMMON_CONFIG_H
