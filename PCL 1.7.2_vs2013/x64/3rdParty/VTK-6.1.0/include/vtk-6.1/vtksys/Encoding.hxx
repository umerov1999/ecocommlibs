/*============================================================================
  KWSys - Kitware System Library
  Copyright 2000-2009 Kitware, Inc., Insight Software Consortium

  Distributed under the OSI-approved BSD License (the "License");
  see accompanying file Copyright.txt for details.

  This software is distributed WITHOUT ANY WARRANTY; without even the
  implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
  See the License for more information.
============================================================================*/
#ifndef vtksys_Encoding_hxx
#define vtksys_Encoding_hxx

#include <vtksys/Configure.hxx>
#include <vtksys/stl/string>

/* Define these macros temporarily to keep the code readable.  */
#if !defined (KWSYS_NAMESPACE) && !vtksys_NAME_IS_KWSYS
# define kwsys_stl vtksys_stl
#endif

namespace vtksys
{
class vtksys_EXPORT Encoding
{
public:
  /**
   * Convert between char and wchar_t
   */

#if vtksys_STL_HAS_WSTRING

  // Convert a narrow string to a wide string.
  // On Windows, UTF-8 is assumed, and on other platforms,
  // the current locale is assumed.
  static kwsys_stl::wstring ToWide(const kwsys_stl::string& str);
  static kwsys_stl::wstring ToWide(const char* str);

  // Convert a wide string to a narrow string.
  // On Windows, UTF-8 is assumed, and on other platforms,
  // the current locale is assumed.
  static kwsys_stl::string ToNarrow(const kwsys_stl::wstring& str);
  static kwsys_stl::string ToNarrow(const wchar_t* str);

#endif // vtksys_STL_HAS_WSTRING

}; // class Encoding
} // namespace vtksys

/* Undefine temporary macros.  */
#if !defined (KWSYS_NAMESPACE) && !vtksys_NAME_IS_KWSYS
# undef kwsys_stl
#endif

#endif
