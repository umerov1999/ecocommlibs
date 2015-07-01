
#ifndef VTKFILTERSPARALLELMPI_EXPORT_H
#define VTKFILTERSPARALLELMPI_EXPORT_H

#ifdef VTKFILTERSPARALLELMPI_STATIC_DEFINE
#  define VTKFILTERSPARALLELMPI_EXPORT
#  define VTKFILTERSPARALLELMPI_NO_EXPORT
#else
#  ifndef VTKFILTERSPARALLELMPI_EXPORT
#    ifdef vtkFiltersParallelMPI_EXPORTS
        /* We are building this library */
#      define VTKFILTERSPARALLELMPI_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKFILTERSPARALLELMPI_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKFILTERSPARALLELMPI_NO_EXPORT
#    define VTKFILTERSPARALLELMPI_NO_EXPORT 
#  endif
#endif

#ifndef VTKFILTERSPARALLELMPI_DEPRECATED
#  define VTKFILTERSPARALLELMPI_DEPRECATED __declspec(deprecated)
#  define VTKFILTERSPARALLELMPI_DEPRECATED_EXPORT VTKFILTERSPARALLELMPI_EXPORT __declspec(deprecated)
#  define VTKFILTERSPARALLELMPI_DEPRECATED_NO_EXPORT VTKFILTERSPARALLELMPI_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKFILTERSPARALLELMPI_NO_DEPRECATED
#endif

/* AutoInit dependencies.  */
#include "vtkFiltersExtractionModule.h"
#include "vtkFiltersParallelModule.h"

#endif
