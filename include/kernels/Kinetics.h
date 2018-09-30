/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef KINETICS_H
#define KINETICS_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"

class Kinetics;

template <>
InputParameters validParams<Kinetics>();

class Kinetics: public DerivativeMaterialInterface<Kernel>
{
public:
  Kinetics(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);
  unsigned int _cp_var;
  const VariableValue & _cp;
  unsigned int _cv_var;
  const VariableValue & _cv;
  /// Mobility
  const MaterialProperty<Real> & _F;
  const MaterialProperty<Real> & _dFe;
  const MaterialProperty<Real> & _dFv;
  const MaterialProperty<Real> & _dF;
  /// Interfacial parameter
};

#endif // KINETICS_H
