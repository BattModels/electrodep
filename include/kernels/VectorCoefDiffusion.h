//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef VECTORCOEFDIFFUSION_H
#define VECTORCOEFDIFFUSION_H

#include "VectorDiffusion.h"

// Forward Declarations
class VectorCoefDiffusion;

template <>
InputParameters validParams<VectorCoefDiffusion>();

class VectorCoefDiffusion : public VectorDiffusion
{
public:
  VectorCoefDiffusion(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;

  const Real & _coef;
};

#endif // VECTORCOEFDIFFUSION_H
