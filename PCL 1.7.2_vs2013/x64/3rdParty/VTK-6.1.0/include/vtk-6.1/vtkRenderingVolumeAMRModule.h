
#ifndef VTKRENDERINGVOLUMEAMR_EXPORT_H
#define VTKRENDERINGVOLUMEAMR_EXPORT_H

#ifdef VTKRENDERINGVOLUMEAMR_STATIC_DEFINE
#  define VTKRENDERINGVOLUMEAMR_EXPORT
#  define VTKRENDERINGVOLUMEAMR_NO_EXPORT
#else
#  ifndef VTKRENDERINGVOLUMEAMR_EXPORT
#    ifdef vtkRenderingVolumeAMR_EXPORTS
        /* We are building this library */
#      define VTKRENDERINGVOLUMEAMR_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKRENDERINGVOLUMEAMR_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKRENDERINGVOLUMEAMR_NO_EXPORT
#    define VTKRENDERINGVOLUMEAMR_NO_EXPORT 
#  endif
#endif

#ifndef VTKRENDERINGVOLUMEAMR_DEPRECATED
#  define VTKRENDERINGVOLUMEAMR_DEPRECATED __declspec(deprecated)
#  define VTKRENDERINGVOLUMEAMR_DEPRECATED_EXPORT VTKRENDERINGVOLUMEAMR_EXPORT __declspec(deprecated)
#  define VTKRENDERINGVOLUMEAMR_DEPRECATED_NO_EXPORT VTKRENDERINGVOLUMEAMR_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKRENDERINGVOLUMEAMR_NO_DEPRECATED
#endif

/* AutoInit dependencies.  */
#include "vtkRenderingVolumeModule.h"

#endif
