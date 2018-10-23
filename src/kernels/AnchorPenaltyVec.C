
#include "AnchorPenaltyVec.h"

registerMooseObject("electrodepApp", AnchorPenaltyVec);

template <>
InputParameters
validParams<AnchorPenaltyVec>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription(
      "Penalty kernel for the energy penalty due to liquid crystal.");
  params.addParam<MaterialPropertyName>("mob_name", "L", "The mobility used with the kernel");
  params.addRequiredCoupledVar("n", "Vector whose magnitude is to be constrained to 1");
  params.addParam<Real>("penalty", 0.0, "Penalty scaling factor");
  params.addCoupledVar("args", "Vector of nonlinear variable arguments this object depends on");
  params.addParam<bool>("variable_L",
                        true,
                        "The mobility is a function of any MOOSE variable (if "
                        "this is set to false L must be constant over the "
                        "entire domain!)");
  return params;
}

AnchorPenaltyVec::AnchorPenaltyVec(const InputParameters & parameters)
  : DerivativeMaterialInterface<Kernel>(parameters),
    _L(getMaterialProperty<Real>("mob_name")),
    _variable_L(getParam<bool>("variable_L")),
    _n(coupledVectorValue("n")),
    _n_id(coupled("n")),
    _n_var(*getVectorVar("n",0)),
    _vec_phi(_assembly.phi(_n_var)),
    _penalty(getParam<Real>("penalty"))
{
}

Real
AnchorPenaltyVec::computeQpResidual()
{
  return _penalty * 2.0 * (_n[_qp] * _grad_u[_qp]) * (_n[_qp] * _grad_test[_i][_qp]);
}

Real
AnchorPenaltyVec::computeQpJacobian()
{
  return _penalty * 2.0 * (_n[_qp] * _grad_phi[_j][_qp]) * (_n[_qp] * _grad_test[_i][_qp]);
}

Real
AnchorPenaltyVec::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _n_id)
    return _penalty * 2.0 * (_vec_phi[_j][_qp] * _grad_u[_qp]) * (_n[_qp] * _grad_test[_i][_qp]) + _penalty * 2.0 * (_n[_qp] * _grad_u[_qp]) * (_vec_phi[_j][_qp] * _grad_test[_i][_qp]);
  else
    return 0.0;
}
