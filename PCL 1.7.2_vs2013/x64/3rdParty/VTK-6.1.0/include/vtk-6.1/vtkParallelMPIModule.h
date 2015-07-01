
#ifndef VTKPARALLELMPI_EXPORT_H
#define VTKPARALLELMPI_EXPORT_H

#ifdef VTKPARALLELMPI_STATIC_DEFINE
#  define VTKPARALLELMPI_EXPORT
#  define VTKPARALLELMPI_NO_EXPORT
#else
#  ifndef VTKPARALLELMPI_EXPORT
#    ifdef vtkParallelMPI_EXPORTS
        /* We are building this library */
#      define VTKPARALLELMPI_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKPARALLELMPI_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKPARALLELMPI_NO_EXPORT
#    define VTKPARALLELMPI_NO_EXPORT 
#  endif
#endif

#ifndef VTKPARALLELMPI_DEPRECATED
#  define VTKPARALLELMPI_DEPRECATED __declspec(deprecated)
#  define VTKPARALLELMPI_DEPRECATED_EXPORT VTKPARALLELMPI_EXPORT __declspec(deprecated)
#  define VTKPARALLELMPI_DEPRECATED_NO_EXPORT VTKPARALLELMPI_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKPARALLELMPI_NO_DEPRECATED
#endif

#define VTK_USE_MPI

#endif
