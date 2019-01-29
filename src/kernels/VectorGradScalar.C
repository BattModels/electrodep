//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "VectorGradScalar.h"

registerMooseObject("electrodepApp", VectorGradScalar);

template <>
InputParameters
validParams<VectorGradScalar>()
{
  InputParameters params = validParams<VectorDiffusion>();
  params.addCustomTypeParam("coef", 1.0, "CoefficientType", "The coefficient of diffusion");
  params.addRequiredCoupledVar("scalar", "gradient of this variable is used");
  return params;
}

VectorGradScalar::VectorGradScalar(const InputParameters & parameters)
  : VectorKernel(parameters), 
    _coef(getParam<Real>("coef")),
    _grad_eta(coupledGradient("scalar")),
    _scalar_id(coupled("scalar")),
    _scalar_var(*getVar("scalar", 0)),
    _grad_standard_phi(_assembly.gradPhi(_scalar_var))
{
}

Real
VectorGradScalar::computeQpResidual()
{
  return -_coef * _grad_eta[_qp] * _test[_i][_qp];
}

Real
VectorGradScalar::computeQpJacobian()
{
  return 0;
}

Real
VectorGradScalar::computeQpOffDiagJacobian(unsigned jvar)
{
  if (jvar == _scalar_id)
    return -_coef * _grad_standard_phi[_j][_qp] * _test[_i][_qp];
  else
    return 0;
}
