/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkRenderingParallelLICObjectFactory.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/

#ifndef __vtkRenderingParallelLICObjectFactory_h
#define __vtkRenderingParallelLICObjectFactory_h

#include "vtkRenderingParallelLICModule.h" // For export macro
#include "vtkObjectFactory.h"

class VTKRENDERINGPARALLELLIC_EXPORT vtkRenderingParallelLICObjectFactory : public vtkObjectFactory
{
public:
  static vtkRenderingParallelLICObjectFactory * New();
  vtkTypeMacro(vtkRenderingParallelLICObjectFactory, vtkObjectFactory)

  const char * GetDescription() { return "vtkRenderingParallelLIC factory overrides."; }

  const char * GetVTKSourceVersion();

  void PrintSelf(ostream &os, vtkIndent indent);

protected:
  vtkRenderingParallelLICObjectFactory();

private:
  vtkRenderingParallelLICObjectFactory(const vtkRenderingParallelLICObjectFactory&); // Not implemented
  void operator=(const vtkRenderingParallelLICObjectFactory&);      // Not implemented
};

#endif // __vtkRenderingParallelLICObjectFactory_h
