
#include "Kineticslc.h"
#include <cmath>
#include <math.h>

registerMooseObject("electrodepApp", Kineticslc);

template <>
InputParameters
validParams<Kineticslc>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Add in Kineticslc");
  params.addParam<Real>("constfactor", 0.0,"constfactor in liq crys, normalized");
  params.addRequiredParam<MaterialPropertyName>("f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  params.addCoupledVar("args", "Vector of arguments of the f_name");
  params.addRequiredCoupledVar("nx", "coupled variable nx");
  params.addRequiredCoupledVar("ny", "coupled variable ny");
  return params;
}

Kineticslc::Kineticslc(const InputParameters & parameters)
  : DerivativeMaterialInterface<JvarMapKernelInterface<Kernel>>(parameters),
    _Fbv(getMaterialProperty<Real>("f_name")),
    _dFbvdu(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
    _dFbvdarg(_coupled_moose_vars.size()),
    _nx_var(coupled("nx")),
    _ny_var(coupled("ny")),
    _grad_nx(coupledGradient("nx")),
    _grad_ny(coupledGradient("ny")),
    _constfactor(getParam<Real>("constfactor"))
{
  for (unsigned int i = 0; i < _dFbvdarg.size(); ++i)
    _dFbvdarg[i] = &getMaterialPropertyDerivative<Real>("f_name", _coupled_moose_vars[i]->name());
}

Real
Kineticslc::computeQpResidual()
{
  Real epen = _constfactor * (_grad_nx[_qp] * _grad_nx[_qp] + _grad_ny[_qp] * _grad_ny[_qp]);
  return exp(-epen) * _Fbv[_qp] * _test[_i][_qp];
}

Real
Kineticslc::computeQpJacobian()
{
  Real epen = _constfactor * (_grad_nx[_qp] * _grad_nx[_qp] + _grad_ny[_qp] * _grad_ny[_qp]);
  return exp(-epen) * _dFbvdu[_qp] * _phi[_j][_qp] * _test[_i][_qp];
}

Real
Kineticslc::computeQpOffDiagJacobian(unsigned int jvar)
{
  Real epen = _constfactor * (_grad_nx[_qp] * _grad_nx[_qp] + _grad_ny[_qp] * _grad_ny[_qp]);
  if (jvar == _nx_var)
    //    return 0;
    return -2.0 * _constfactor * _grad_nx[_qp] * _grad_phi[_j][_qp] * exp(-epen) * _Fbv[_qp] * _test[_i][_qp];
  else if (jvar == _ny_var)
    //    return 0;
    return -2.0 * _constfactor * _grad_ny[_qp] * _grad_phi[_j][_qp] * exp(-epen) * _Fbv[_qp] * _test[_i][_qp];
  else
    //    return -_constfactor * (_grad_phi[_j][_qp] * _grad_nx[_qp] + _grad_nx[_qp] * _grad_phi[_j][_qp]) * \
  //      exp( -epen ) * _F[_qp] * _test[_i][_qp];
    {
      const unsigned int cvar = mapJvarToCvar(jvar);
      return exp(-epen) * (*_dFbvdarg[cvar])[_qp] * _phi[_j][_qp] * _test[_i][_qp];
    }

  /*
  if (jvar == _cp_var)
    return _dF[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else if (jvar == _cv_var)
    return _dFv[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else if (jvar == _nx_var)
    {
      Real epen = _constfactor * _grad_nx[_qp] * _grad_nx[_qp] + _grad_ny[_qp] * _grad_ny[_qp];
      return -_constfactor * (_grad_phi[_j][_qp] * _grad_nx[_qp] + _grad_nx[_qp] * _grad_phi[_j][_qp]) * exp( -epen ) * _F[_qp] * _test[_i][_qp];
    }
  else if (jvar == _ny_var)
    {
      Real epen = _constfactor * _grad_nx[_qp] * _grad_nx[_qp] + _grad_ny[_qp] * _grad_ny[_qp];
      return -_constfactor * (_grad_phi[_j][_qp] * _grad_ny[_qp] + _grad_ny[_qp] * _grad_phi[_j][_qp]) * exp( -epen ) * _F[_qp] * _test[_i][_qp];
    }
  else
    return 0;
  */
}
