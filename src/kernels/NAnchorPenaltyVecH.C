#include "NAnchorPenaltyVecH.h"

registerMooseObject("electrodepApp", NAnchorPenaltyVecH);

template <>
InputParameters
validParams<NAnchorPenaltyVecH>()
{
  InputParameters params = validParams<VectorKernel>();
  params.addClassDescription("Penalty kernel for the anchor energy penalty due to liquid crystal vec");
  params.addRequiredCoupledVar("eta", "gradient of this function used in penalty");
  params.addParam<Real>("penalty", 0.0, "Penalty scaling factor");
  params.addRequiredParam<MaterialPropertyName>("h_name", "The switching function");
  return params;
}

NAnchorPenaltyVecH::NAnchorPenaltyVecH(const InputParameters & parameters) 
  : DerivativeMaterialInterface<VectorKernel>(parameters),
    _penalty(getParam<Real>("penalty")),
    _h(getMaterialProperty<Real>("h_name")),
    _eta(coupledValue("eta")),
    _grad_eta(coupledGradient("eta")),
    _eta_id(coupled("eta")),
    _eta_var(*getVar("eta", 0)),
    _dhdeta(getMaterialPropertyDerivative<Real>("h_name", getVar("eta", 0)->name())),
    _grad_standard_phi(_assembly.gradPhi(_eta_var))
{
}

Real
NAnchorPenaltyVecH::computeQpResidual()
{
  return _penalty * 2.0 * (1.0 - _h[_qp]) * (_u[_qp] * _grad_eta[_qp]) * (_grad_eta[_qp] * _test[_i][_qp]);
}

Real
NAnchorPenaltyVecH::computeQpJacobian()
{
  return _penalty * 2.0 * (1.0 - _h[_qp]) * (_phi[_j][_qp] * _grad_eta[_qp]) * (_grad_eta[_qp] * _test[_i][_qp]);
}

Real
NAnchorPenaltyVecH::computeQpOffDiagJacobian(unsigned jvar)
{
  if (jvar == _eta_id)
    return _penalty * 2.0 * (-_dhdeta[_qp]) * (_u[_qp] * _grad_eta[_qp]) * (_grad_eta[_qp] * _test[_i][_qp]) + _penalty * 2.0 * (1.0 - _h[_qp]) * (_u[_qp] * _grad_standard_phi[_j][_qp]) * (_grad_eta[_qp] * _test[_i][_qp]) + _penalty * 2.0 * (1.0 - _h[_qp]) * (_u[_qp] * _grad_eta[_qp]) * (_grad_standard_phi[_j][_qp] * _test[_i][_qp]);
  else
    return 0;
}
