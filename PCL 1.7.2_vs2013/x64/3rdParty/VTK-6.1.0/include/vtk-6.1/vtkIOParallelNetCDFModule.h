
#ifndef VTKIOPARALLELNETCDF_EXPORT_H
#define VTKIOPARALLELNETCDF_EXPORT_H

#ifdef VTKIOPARALLELNETCDF_STATIC_DEFINE
#  define VTKIOPARALLELNETCDF_EXPORT
#  define VTKIOPARALLELNETCDF_NO_EXPORT
#else
#  ifndef VTKIOPARALLELNETCDF_EXPORT
#    ifdef vtkIOParallelNetCDF_EXPORTS
        /* We are building this library */
#      define VTKIOPARALLELNETCDF_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define VTKIOPARALLELNETCDF_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef VTKIOPARALLELNETCDF_NO_EXPORT
#    define VTKIOPARALLELNETCDF_NO_EXPORT 
#  endif
#endif

#ifndef VTKIOPARALLELNETCDF_DEPRECATED
#  define VTKIOPARALLELNETCDF_DEPRECATED __declspec(deprecated)
#  define VTKIOPARALLELNETCDF_DEPRECATED_EXPORT VTKIOPARALLELNETCDF_EXPORT __declspec(deprecated)
#  define VTKIOPARALLELNETCDF_DEPRECATED_NO_EXPORT VTKIOPARALLELNETCDF_NO_EXPORT __declspec(deprecated)
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define VTKIOPARALLELNETCDF_NO_DEPRECATED
#endif



#endif
