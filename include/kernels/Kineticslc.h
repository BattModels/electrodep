/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef KINETICSLC_H
#define KINETICSLC_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"
#include "JvarMapInterface.h"

class Kineticslc;

template <>
InputParameters validParams<Kineticslc>();

class Kineticslc: public DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>
{
public:
  Kineticslc(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;
  
  const MaterialProperty<Real> & _Fbv;
  const MaterialProperty<Real> & _dFbvdu;
  std::vector<const MaterialProperty<Real> *> _dFbvdarg;
  unsigned int  _nx_var, _ny_var;
  const VariableGradient & _grad_nx;
  const VariableGradient & _grad_ny;
  Real _constfactor;
};

#endif // KINETICSLC_H
