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

class Kineticslc;

template <>
InputParameters validParams<Kineticslc>();

class Kineticslc: public DerivativeMaterialInterface<Kernel>
{
public:
  Kineticslc(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;
  unsigned int _cp_var, _cv_var, _nx_var, _ny_var;
  const VariableValue & _cp;
  const VariableValue & _cv;
  const VariableGradient & _grad_nx;
  const VariableGradient & _grad_ny;
  /// Mobility
  const MaterialProperty<Real> & _F;
  const MaterialProperty<Real> & _dFe;
  const MaterialProperty<Real> & _dFv;
  const MaterialProperty<Real> & _dF;
  /// Interfacial parameter
  //All components of epen are in actual units                                                                                       
  const float K = 1.0;
  const float Rmol = 8.314;
  const float T = 300.0;
  const float m3permol = 1.0;
  const float constfactor = 1/2.0 * K * m3permol * pow(10., -12)  / ( Rmol *  T );
};

#endif // KINETICSLC_H
