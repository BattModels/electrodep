/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/
#ifndef ELECTRONEUTRALITY_H
#define ELECTRONEUTRALITY_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"

class Electroneutrality;

template <>
InputParameters validParams<Electroneutrality>();

class Electroneutrality: public DerivativeMaterialInterface<Kernel>
{
public:
  Electroneutrality(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual() override;
  virtual Real computeQpJacobian() override;
  virtual Real computeQpOffDiagJacobian(unsigned int jvar) override;
  unsigned int _cp_var;
  const VariableValue & _cp;
  unsigned int _cv_var;
  const VariableValue & _cv;
  const VariableGradient & _grad_cp;
  /// Mobility
  const MaterialProperty<Real> & _Q;
  const MaterialProperty<Real> & _QM;
  const MaterialProperty<Real> & _dQ;
  const MaterialProperty<Real> & _dQv;
  const MaterialProperty<Real> & _dQMv;
  const MaterialProperty<Real> & _dQM;
  /// Interfacial parameter
};

#endif // ELECTRONEUTRALITY_H
