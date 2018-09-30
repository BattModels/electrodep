//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NMagnitudeConstraintN.h"

registerMooseObject("electrodepApp", NMagnitudeConstraintN);

template <>
InputParameters
validParams<NMagnitudeConstraintN>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Lagrange multiplier kernel to constrain the magnitude of n "
                             "to 1 ");
  params.addRequiredCoupledVar("lambda", "Lagrange multiplier");
  return params;
}

NMagnitudeConstraintN::NMagnitudeConstraintN(const InputParameters & parameters)
  : Kernel(parameters),
    _lambda(coupledValue("lambda")),
    _lambda_var(coupled("lambda"))
{
}

Real
NMagnitudeConstraintN::computeQpResidual()
{
  return _lambda[_qp] * 2.0 * _u[_qp] * _test[_i][_qp];
}

Real
NMagnitudeConstraintN::computeQpJacobian()
{
  return _lambda[_qp] * 2.0 * _phi[_j][_qp] * _test[_i][_qp];
}

Real
NMagnitudeConstraintN::computeQpOffDiagJacobian(unsigned int j_var)
{
  if (j_var == _lambda_var)
    return _phi[_j][_qp] * 2.0 * _u[_qp] * _test[_i][_qp];
  else
    return 0.0;
}
