//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "AnchorPenaltyScalar.h"

registerMooseObject("electrodepApp", AnchorPenaltyScalar);

template <>
InputParameters
validParams<AnchorPenaltyScalar>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription(
      "Penalty kernel for the energy penalty due to liquid crystal.");
  params.addParam<MaterialPropertyName>("mob_name", "L", "The mobility used with the kernel");
  params.addRequiredCoupledVar("nx", "nx component ");
  params.addRequiredCoupledVar("ny", "ny component ");
  params.addParam<Real>("penalty", 0.0, "Penalty scaling factor");
  params.addCoupledVar("args", "Vector of nonlinear variable arguments this object depends on");
  params.addParam<bool>("variable_L",
                        true,
                        "The mobility is a function of any MOOSE variable (if "
                        "this is set to false L must be constant over the "
                        "entire domain!)");
  return params;
}

AnchorPenaltyScalar::AnchorPenaltyScalar(const InputParameters & parameters)
  : DerivativeMaterialInterface<Kernel>(parameters),
    _nx(coupledValue("nx")),
    _ny(coupledValue("ny")),
    _nx_var(coupled("nx")),
    _ny_var(coupled("ny")),
    _penalty(getParam<Real>("penalty")),
    _L(getMaterialProperty<Real>("mob_name")),
    _variable_L(getParam<bool>("variable_L"))
    //_nlc(_nlc),
//    _number_of_nl_variables(_fe_problem.getNonlinearSystemBase().nVariables()),
//    _j_eta(_number_of_nl_variables, -1),

{
 //   _nlc(0) = *_nlc_comp[0];
//    _nlc(1) = *_nlc_comp[1];
}

Real
AnchorPenaltyScalar::computeQpResidual()
{
  return _penalty * 2.0 * ( _nx[_qp] * _grad_u[_qp](0) + _ny[_qp] * _grad_u[_qp](1) ) * ( _nx[_qp] * _grad_test[_i][_qp](0) + _ny[_qp] * _grad_test[_i][_qp](1) );
}

Real
AnchorPenaltyScalar::computeQpJacobian()
{
  return _penalty * 2.0 * ( _nx[_qp] * _grad_phi[_j][_qp](0) + _ny[_qp] * _grad_phi[_j][_qp](1) ) * ( _nx[_qp] * _grad_test[_i][_qp](0) + _ny[_qp] * _grad_test[_i][_qp](1) );
}

Real
AnchorPenaltyScalar::computeQpOffDiagJacobian(unsigned int jvar)
{
  if ( jvar == _nx_var )
  {
    const Real vdotu = _phi[_j][_qp] * _grad_u[_qp](0) + _ny[_qp] * _grad_u[_qp](1);
    const Real vdottest = _phi[_j][_qp] * _grad_test[_i][_qp](0) + _ny[_qp] * _grad_test[_i][_qp](1);
    const Real ndottest = _nx[_qp] * _grad_test[_i][_qp](0) + _ny[_qp] * _grad_test[_i][_qp](1);
    const Real ndotu = _nx[_qp] * _grad_u[_qp](0) + _ny[_qp] * _grad_u[_qp](1);
    return 2.0 * _penalty * ( vdotu * ndottest + ndotu * vdottest );
  }

  else if ( jvar == _ny_var )
  {
    const Real vdotu = _nx[_qp] * _grad_u[_qp](0) + _phi[_j][_qp] * _grad_u[_qp](1);
    const Real vdottest = _nx[_qp] * _grad_test[_i][_qp](0) + _phi[_j][_qp] * _grad_test[_i][_qp](1);
    const Real ndottest = _nx[_qp] * _grad_test[_i][_qp](0) + _ny[_qp] * _grad_test[_i][_qp](1);
    const Real ndotu = _nx[_qp] * _grad_u[_qp](0) + _ny[_qp] * _grad_u[_qp](1);
    return 2.0 * _penalty * ( vdotu * ndottest + ndotu * vdottest );
  }
  
  else
    return 0.0;
}
