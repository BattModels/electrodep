
#include "AnchorPenaltyVecH.h"

registerMooseObject("electrodepApp", AnchorPenaltyVecH);

template <>
InputParameters
validParams<AnchorPenaltyVecH>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription(
      "Penalty kernel for the energy penalty due to liquid crystal.");
  //  params.addParam<MaterialPropertyName>("mob_name", "L", "The mobility used with the kernel");
  params.addRequiredCoupledVar("n", "Vector whose magnitude is to be constrained to 1");
  params.addParam<Real>("penalty", 0.0, "Penalty scaling factor");
  params.addRequiredParam<MaterialPropertyName>("h_name", "The switching function");
  //  params.addCoupledVar("args", "Vector of nonlinear variable arguments this object depends on");
  //  params.addParam<bool>("variable_L",
  //                        true,
  //                        "The mobility is a function of any MOOSE variable (if "
  //                        "this is set to false L must be constant over the "
  //                        "entire domain!)");
  return params;
}

AnchorPenaltyVecH::AnchorPenaltyVecH(const InputParameters & parameters)
  : DerivativeMaterialInterface<Kernel>(parameters),
    _n(coupledVectorValue("n")),
    _n_id(coupled("n")),
    _n_var(*getVectorVar("n",0)),
    _vec_phi(_assembly.phi(_n_var)),
    _penalty(getParam<Real>("penalty")),
    _h(getMaterialProperty<Real>("h_name")),
    _dhdu(getMaterialPropertyDerivative<Real>("h_name", _var.name()))
    //    _L(getMaterialProperty<Real>("mob_name")),
    //    _variable_L(getParam<bool>("variable_L"))
{
}

Real
AnchorPenaltyVecH::computeQpResidual()
{
  return _penalty * 2.0 * (1.0 - _h[_qp]) * (_n[_qp] * _grad_u[_qp]) * (_n[_qp] * _grad_test[_i][_qp]);
}

Real
AnchorPenaltyVecH::computeQpJacobian()
{
  return _penalty * 2.0 * (-_dhdu[_qp]) * (_n[_qp] * _grad_u[_qp]) * (_n[_qp] * _grad_test[_i][_qp]) + _penalty * 2.0 * (1.0 - _h[_qp]) * (_n[_qp] * _grad_phi[_j][_qp]) * (_n[_qp] * _grad_test[_i][_qp]);
}

Real
AnchorPenaltyVecH::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _n_id)
    return _penalty * 2.0 * (1.0 - _h[_qp]) * (_vec_phi[_j][_qp] * _grad_u[_qp]) * (_n[_qp] * _grad_test[_i][_qp]) + _penalty * 2.0 * (1.0 - _h[_qp]) * (_n[_qp] * _grad_u[_qp]) * (_vec_phi[_j][_qp] * _grad_test[_i][_qp]);
  else
    return 0.0;
}
