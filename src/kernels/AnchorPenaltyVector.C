
#include "AnchorPenaltyVector.h"

registerMooseObject("electrodepApp", AnchorPenaltyVector);

template <>
InputParameters
validParams<AnchorPenaltyVector>()
{
  InputParameters params = validParams<VectorKernel>();
  params.addClassDescription(
      "The Laplacian operator ($-\\nabla \\cdot \\nabla \\vec{u}$), with the weak "
      "form of $(\\nabla \\vec{\\phi_i}, \\nabla \\vec{u_h})$.");
  params.addParam<Real>("penalty", 1.0, "Penalty scaling factor");
  params.addRequiredCoupledVar("eta", "order parameter");
  return params;
}

AnchorPenaltyVector::AnchorPenaltyVector(const InputParameters & parameters) 
  : VectorKernel(parameters),
    _grad_eta(coupledGradient("eta")),
    _eta_var(coupled("eta")),
    _penalty(getParam<Real>("penalty"))
{
}

Real
AnchorPenaltyVector::computeQpResidual()
{
  return 2.0 * _penalty * _grad_u[_qp].contract(_grad_test[_i][_qp]) * _grad_u[_qp].contract(_grad_eta[_qp]) ;
}

Real
AnchorPenaltyVector::computeQpJacobian()
{
  return 2.0 * _penalty * (_grad_phi[_j][_qp].contract(_grad_test[_i][_qp]) * _grad_u[_qp].contract(_grad_eta[_qp]) + _grad_u[_qp].contract(_grad_test[_i][_qp]) * _grad_phi[_j][_qp].contract(_grad_eta[_qp]) ) ;
}

Real
AnchorPenaltyVector::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _eta_var)
    return 2.0 * _penalty * _grad_u[_qp].contract(_grad_test[_i][_qp]) * _grad_u[_qp].contract(_grad_phi[_j][_qp]);
  else
    return 0;
}
