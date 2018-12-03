
#include "Kinetics.h"
#include <cmath>

registerMooseObject("electrodepApp", Kinetics);

template <>
InputParameters
validParams<Kinetics>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Add in Kinetics");
  params.addRequiredParam<MaterialPropertyName>("f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  params.addCoupledVar("args", "Vector of arguments of the f_name");
  return params;
}

Kinetics::Kinetics(const InputParameters & parameters)
  : DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>(parameters),
    _Fbv(getMaterialProperty<Real>("f_name")),
    _dFbvdu(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
    _dFbvdarg(_coupled_moose_vars.size())
{
  for (unsigned int i = 0; i < _dFbvdarg.size(); ++i)
    _dFbvdarg[i] = &getMaterialPropertyDerivative<Real>("f_name", _coupled_moose_vars[i]->name());
}

Real
Kinetics::computeQpResidual()
{
  return _Fbv[_qp] * _test[_i][_qp];
}

Real
Kinetics::computeQpJacobian()
{
  return _dFbvdu[_qp] * _phi[_j][_qp] * _test[_i][_qp];
}

Real
Kinetics::computeQpOffDiagJacobian(unsigned int jvar)
{
  const unsigned int cvar = mapJvarToCvar(jvar);
  return (*_dFbvdarg[cvar])[_qp] * _phi[_j][_qp] * _test[_i][_qp];     
}
