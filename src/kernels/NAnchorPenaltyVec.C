#include "NAnchorPenaltyVec.h"

registerMooseObject("electrodepApp", NAnchorPenaltyVec);

template <>
InputParameters
validParams<NAnchorPenaltyVec>()
{
  InputParameters params = validParams<VectorKernel>();
  params.addClassDescription("Penalty kernel for the anchor energy penalty due to liquid crystal vec");
  params.addRequiredCoupledVar("eta", "gradient of this function used in penalty");
  params.addParam<Real>("penalty", 0.0, "Penalty scaling factor");
  return params;
}

NAnchorPenaltyVec::NAnchorPenaltyVec(const InputParameters & parameters) 
  : VectorKernel(parameters),
    _penalty(getParam<Real>("penalty")),
    _grad_eta(coupledGradient("eta")),
    _eta_id(coupled("eta")),
    _eta_var(*getVar("eta", 0)),
    _grad_standard_phi(_assembly.gradPhi(_eta_var))
{
}

Real
NAnchorPenaltyVec::computeQpResidual()
{
  return _penalty * 2.0 * (_u[_qp] * _grad_eta[_qp]) * (_grad_eta[_qp] * _test[_i][_qp]);
}

Real
NAnchorPenaltyVec::computeQpJacobian()
{
  return _penalty * 2.0 * (_phi[_j][_qp] * _grad_eta[_qp]) * (_grad_eta[_qp] * _test[_i][_qp]);
}

Real
NAnchorPenaltyVec::computeQpOffDiagJacobian(unsigned jvar)
{
  if (jvar == _eta_id)
    return _penalty * 2.0 * (_u[_qp] * _grad_standard_phi[_j][_qp]) * (_grad_eta[_qp] * _test[_i][_qp]) + _penalty * 2.0 * (_u[_qp] * _grad_eta[_qp]) * (_grad_standard_phi[_j][_qp] * _test[_i][_qp]);
  else
    return 0;
}
