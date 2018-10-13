
#include "NMagnitudeConstraintLagrangeComponent.h"

registerMooseObject("electrodepApp", NMagnitudeConstraintLagrangeComponent);

template <>
InputParameters
validParams<NMagnitudeConstraintLagrangeComponent>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Lagrange multiplier kernel to constrain the magnitude of n to 1 "
                             "This kernel acts on the lagrange "
                             "multiplier variable.");
  params.addRequiredCoupledVar("nx", "nlc x component");
  params.addRequiredCoupledVar("ny", "nlc y component");
  params.addParam<Real>("epsilon", 1e-4, "Shift factor to avoid a zero pivot");
  return params;
}

NMagnitudeConstraintLagrangeComponent::NMagnitudeConstraintLagrangeComponent(const InputParameters & parameters)
  : Kernel(parameters),
    _nx_var(coupled("nx")),
    _nx(coupledValue("nx")),
    _ny_var(coupled("ny")),
    _ny(coupledValue("ny")),
    _epsilon(getParam<Real>("epsilon"))
{
}

Real
NMagnitudeConstraintLagrangeComponent::computeQpResidual()
{
  Real g = (-_epsilon) * _u[_qp] - 1.0;
  g += _nx[_qp] * _nx[_qp] + _ny[_qp] * _ny[_qp];
  return _test[_i][_qp] * g;
}

Real
NMagnitudeConstraintLagrangeComponent::computeQpJacobian()
{
  return _test[_i][_qp] * (-_epsilon) * _phi[_j][_qp];
}

Real
NMagnitudeConstraintLagrangeComponent::computeQpOffDiagJacobian(unsigned int jvar)
{
  if (jvar == _nx_var)
    return 2.0 * _nx[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else if (jvar == _ny_var)
    return 2.0 * _ny[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else
    return 0.0;
}
