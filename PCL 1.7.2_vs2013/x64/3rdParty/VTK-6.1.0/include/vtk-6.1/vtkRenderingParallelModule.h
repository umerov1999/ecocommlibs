
#ifndef VTKRENDERINGPARALLEL_EXPORT_H
#define VTKRENDERINGPARALLEL_EXPORT_H

#ifdef VTKRENDERINGPARALLEL_STATIC_DEFINE
#  define VTKRENDERINGPARALLEL_EXPORT
#  define VTKRENDERINGPARALLEL_NO_EXPORT
#else
#  ifndef VTKRENDERINGPARALLEL_EXPORT
#    ifdef vtkRenderingParallel_EXPORTS
        /* We are building this library */
#      define VTKRENDERINGPARALLEL_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKRENDERINGPARALLEL_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKRENDERINGPARALLEL_NO_EXPORT
#    define VTKRENDERINGPARALLEL_NO_EXPORT 
#  endif
#endif

#ifndef VTKRENDERINGPARALLEL_DEPRECATED
#  define VTKRENDERINGPARALLEL_DEPRECATED __declspec(deprecated)
#  define VTKRENDERINGPARALLEL_DEPRECATED_EXPORT VTKRENDERINGPARALLEL_EXPORT __declspec(deprecated)
#  define VTKRENDERINGPARALLEL_DEPRECATED_NO_EXPORT VTKRENDERINGPARALLEL_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKRENDERINGPARALLEL_NO_DEPRECATED
#endif

/* AutoInit dependencies.  */
#include "vtkFiltersParallelModule.h"
#include "vtkRenderingOpenGLModule.h"

#endif
