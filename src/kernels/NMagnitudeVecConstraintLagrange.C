
#include "NMagnitudeVecConstraintLagrange.h"

registerMooseObject("electrodepApp", NMagnitudeVecConstraintLagrange);

template <>
InputParameters
validParams<NMagnitudeVecConstraintLagrange>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Lagrange multiplier kernel to constrain the magnitude of n to 1 "
                             "This kernel acts on the lagrange "
                             "multiplier variable.");
  params.addRequiredCoupledVar("vec", "Vector whose magnitude is to be constrained to 1");
  params.addParam<Real>("epsilon", 1e-4, "Shift factor to avoid a zero pivot");
  return params;
}

NMagnitudeVecConstraintLagrange::NMagnitudeVecConstraintLagrange(const InputParameters & parameters)
  : Kernel(parameters),
    _vec(coupledVectorValue("vec")),
    _epsilon(getParam<Real>("epsilon")),
    _vec_id(coupled("vec"))
{
}

Real
NMagnitudeVecConstraintLagrange::computeQpResidual()
{
  Real g = -_epsilon * _u[_qp] - 1.0;
  g += _vec[_qp] * _vec[_qp]; 
  return _test[_i][_qp] * g;
}

Real
NMagnitudeVecConstraintLagrange::computeQpJacobian()
{
  return _test[_i][_qp] * -_epsilon * _phi[_j][_qp];
}

Real
NMagnitudeVecConstraintLagrange::computeQpOffDiagJacobian(unsigned jvar)
{
  if (jvar == _vec_id)
    return 2.0 * _phi[_j][_qp] * _vec[_qp] * _test[_i][_qp];
  else
    return 0.0;
}
