
#ifndef VTKRENDERINGPARALLELLIC_EXPORT_H
#define VTKRENDERINGPARALLELLIC_EXPORT_H

#ifdef VTKRENDERINGPARALLELLIC_STATIC_DEFINE
#  define VTKRENDERINGPARALLELLIC_EXPORT
#  define VTKRENDERINGPARALLELLIC_NO_EXPORT
#else
#  ifndef VTKRENDERINGPARALLELLIC_EXPORT
#    ifdef vtkRenderingParallelLIC_EXPORTS
        /* We are building this library */
#      define VTKRENDERINGPARALLELLIC_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKRENDERINGPARALLELLIC_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKRENDERINGPARALLELLIC_NO_EXPORT
#    define VTKRENDERINGPARALLELLIC_NO_EXPORT 
#  endif
#endif

#ifndef VTKRENDERINGPARALLELLIC_DEPRECATED
#  define VTKRENDERINGPARALLELLIC_DEPRECATED __declspec(deprecated)
#  define VTKRENDERINGPARALLELLIC_DEPRECATED_EXPORT VTKRENDERINGPARALLELLIC_EXPORT __declspec(deprecated)
#  define VTKRENDERINGPARALLELLIC_DEPRECATED_NO_EXPORT VTKRENDERINGPARALLELLIC_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKRENDERINGPARALLELLIC_NO_DEPRECATED
#endif

/* AutoInit dependencies.  */
#include "vtkRenderingLICModule.h"
#include "vtkRenderingOpenGLModule.h"

/* AutoInit implementations.  */
#if defined(vtkRenderingParallelLIC_INCLUDE)
# include vtkRenderingParallelLIC_INCLUDE
#endif
#if defined(vtkRenderingParallelLIC_AUTOINIT)
# include "vtkAutoInit.h"
VTK_AUTOINIT(vtkRenderingParallelLIC)
#endif

#endif
