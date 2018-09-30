/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#include "Kinetics.h"

registerMooseObject("electrodepApp", Kinetics);

template <>
InputParameters
validParams<Kinetics>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription(
      "Add in Kinetics");
  params.addRequiredCoupledVar(
      "cp", "coupled variable");
  params.addRequiredCoupledVar(
          "cv", "coupled variable 2");
  params.addRequiredParam<MaterialPropertyName>(
          "f_name", "Base name of the free energy function F defined in a DerivativeParsedMaterial");
  return params;
}

Kinetics::Kinetics(const InputParameters & parameters)
: DerivativeMaterialInterface<Kernel>(parameters),
	_cp_var(coupled("cp")),
	_cp(coupledValue("cp")),
  _cv_var(coupled("cv")),
	_cv(coupledValue("cv")),
  _F(getMaterialProperty<Real>("f_name")),
  _dFe(getMaterialPropertyDerivative<Real>("f_name", _var.name())),
  _dFv(getMaterialPropertyDerivative<Real>("f_name", getVar("cv", 0)->name())),
	_dF(getMaterialPropertyDerivative<Real>("f_name", getVar("cp", 0)->name()))
{
}

Real
Kinetics::computeQpResidual()
{
  return _F[_qp]*_test[_i][_qp];
}

Real
Kinetics::computeQpJacobian()
{
  return _dFe[_qp]*_phi[_j][_qp]*_test[_i][_qp];
}
Real
Kinetics::computeQpOffDiagJacobian(unsigned int jvar)
{
   if (jvar == _cp_var)
	   return _dF[_qp]*_phi[_j][_qp]*_test[_i][_qp];
    else if (jvar == _cv_var)
      return _dFv[_qp]*_phi[_j][_qp]*_test[_i][_qp];
	else
     return 0;
}
