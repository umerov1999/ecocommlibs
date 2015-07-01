
#ifndef VTKIOVPIC_EXPORT_H
#define VTKIOVPIC_EXPORT_H

#ifdef VTKIOVPIC_STATIC_DEFINE
#  define VTKIOVPIC_EXPORT
#  define VTKIOVPIC_NO_EXPORT
#else
#  ifndef VTKIOVPIC_EXPORT
#    ifdef vtkIOVPIC_EXPORTS
        /* We are building this library */
#      define VTKIOVPIC_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKIOVPIC_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKIOVPIC_NO_EXPORT
#    define VTKIOVPIC_NO_EXPORT 
#  endif
#endif

#ifndef VTKIOVPIC_DEPRECATED
#  define VTKIOVPIC_DEPRECATED __declspec(deprecated)
#  define VTKIOVPIC_DEPRECATED_EXPORT VTKIOVPIC_EXPORT __declspec(deprecated)
#  define VTKIOVPIC_DEPRECATED_NO_EXPORT VTKIOVPIC_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKIOVPIC_NO_DEPRECATED
#endif



#endif
