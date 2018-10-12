
#include "NMagnitudeVecConstraintN.h"

registerMooseObject("electrodepApp", NMagnitudeVecConstraintN);

template <>
InputParameters
validParams<NMagnitudeVecConstraintN>()
{
  InputParameters params = validParams<VectorKernel>();
  params.addClassDescription("Lagrange multiplier kernel to constrain the magnitude of n "
                             "to 1 ");
  params.addRequiredCoupledVar("lambda", "Lagrange multiplier");
  return params;
}

NMagnitudeVecConstraintN::NMagnitudeVecConstraintN(const InputParameters & parameters) 
  : VectorKernel(parameters),
    _lambda(coupledValue("lambda")),
    _lambda_var(coupled("lambda"))
{
}

Real
NMagnitudeVecConstraintN::computeQpResidual()
{
  return _lambda[_qp] * 2.0 * _u[_qp] * _test[_i][_qp];
}

Real
NMagnitudeVecConstraintN::computeQpJacobian()
{
  return _lambda[_qp] * 2.0 * _phi[_j][_qp] * _test[_i][_qp];
}

Real
NMagnitudeVecConstraintN::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _lambda_var)
    return _phi[_j][_qp] * 2.0 * _u[_qp] * _test[_i][_qp];
  else
    return 0;
}
