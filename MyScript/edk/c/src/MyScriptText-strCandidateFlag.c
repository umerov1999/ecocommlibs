/**
 * @file MyScriptText-strCandidateFlag.c
 *
 */

#include "MyScriptText-strCandidateFlag.h"
#include <c/MyScriptText.h>


const char* strMyScriptCandidateFlag(int type)
{
#define __CASE_FROM_CANDIDATE_FLAG(x) case VO_CANDIDATE_FLAG_##x: return #x
  switch (type)
  {
#ifdef C_MYSCRIPTTEXT_H
  __CASE_FROM_CANDIDATE_FLAG(OMITTED);
  __CASE_FROM_CANDIDATE_FLAG(SUPERFLUOUS);
  __CASE_FROM_CANDIDATE_FLAG(SUBSTITUTED);
  __CASE_FROM_CANDIDATE_FLAG(UNKNOWN);
  __CASE_FROM_CANDIDATE_FLAG(ERASED);
  __CASE_FROM_CANDIDATE_FLAG(EMPHASIZED);
  __CASE_FROM_CANDIDATE_FLAG(OUT_OF_LEXICON);
#endif // end of: #ifdef C_MYSCRIPTTEXT_H

  default:
    return "<<UNKNOWN>>";
  }
#undef __CASE_FROM_CANDIDATE_FLAG
}
