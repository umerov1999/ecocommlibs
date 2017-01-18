/**
 * @file MyScriptText-strTagType.c
 *
 */

#include "MyScriptText-strTagType.h"
#include <c/MyScriptText.h>


const char* strMyScriptTagType(int type)
{
#define __CASE_FROM_TAG_TYPE(x) case VO_TAG_##x: return #x
  switch (type)
  {
#ifdef C_MYSCRIPTTEXT_H
  __CASE_FROM_TAG_TYPE(TEXT_LINE);
  __CASE_FROM_TAG_TYPE(TEXT_BLOCK);
  __CASE_FROM_TAG_TYPE(CORRECTION_BLOCK);
  __CASE_FROM_TAG_TYPE(ERASURE);
  __CASE_FROM_TAG_TYPE(ERASED_TEXT);
  __CASE_FROM_TAG_TYPE(STRIKETHROUGH);
  __CASE_FROM_TAG_TYPE(DOUBLE_STRIKETHROUGH);
  __CASE_FROM_TAG_TYPE(CROSS_OUT);
  __CASE_FROM_TAG_TYPE(SCRATCH_OUT);
  __CASE_FROM_TAG_TYPE(INSERTED_TEXT);
  __CASE_FROM_TAG_TYPE(MISSING_PEN_LIFT);
  __CASE_FROM_TAG_TYPE(EMPHASIZED_CHARACTER);
  __CASE_FROM_TAG_TYPE(TEXT_COLUMN);
#endif // end of: #ifdef C_MYSCRIPTTEXT_H

  default:
    return "<<UNKNOWN>>";
  }
#undef __CASE_FROM_TAG_TYPE
}
