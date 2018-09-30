/****************************************************************/
/* MOOSE - Multiphysics Object Oriented Simulation Environment  */
/*                                                              */
/*          All contents are licensed under LGPL V2.1           */
/*             See LICENSE for full restrictions                */
/****************************************************************/

#include "Electroneutrality.h"

registerMooseObject("electrodepApp", Electroneutrality);

template <>
InputParameters
validParams<Electroneutrality>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription(
      "Add in electronutrality");
  params.addParam<MaterialPropertyName>("Q_name","The mobility for electronutrality used with the kernel for phi");
  params.addParam<MaterialPropertyName>("QM_name","The mobility for electronutrality used with th kernel for c");
  params.addRequiredCoupledVar(
      "cp", "chemical potential");
  params.addRequiredCoupledVar(
      "cv", "coupled variable");
  return params;
}

Electroneutrality::Electroneutrality(const InputParameters & parameters)
: DerivativeMaterialInterface<Kernel>(parameters),
	_cp_var(coupled("cp")),
	_cp(coupledValue("cp")),
        _cv_var(coupled("cv")),
        _cv(coupledValue("cv")),
	_grad_cp(coupledGradient("cp")),
	_Q(getMaterialProperty<Real>("Q_name")),
	_QM(getMaterialProperty<Real>("QM_name")),
	_dQ(getMaterialPropertyDerivative<Real>("Q_name", getVar("cp", 0)->name())),
        _dQv(getMaterialPropertyDerivative<Real>("Q_name", getVar("cv", 0)->name())),
        _dQMv(getMaterialPropertyDerivative<Real>("QM_name", getVar("cv", 0)->name())),
        _dQM(getMaterialPropertyDerivative<Real>("QM_name", getVar("cp", 0)->name()))
{
}

Real
Electroneutrality::computeQpResidual()
{
  return _Q[_qp]*_grad_u[_qp]*_grad_test[_i][_qp]+_QM[_qp]*_grad_cp[_qp]*_grad_test[_i][_qp];
}

Real
Electroneutrality::computeQpJacobian()
{
  return _Q[_qp]*_grad_phi[_j][_qp]* _grad_test[_i][_qp];
}
Real
Electroneutrality::computeQpOffDiagJacobian(unsigned int jvar)
{
   if (jvar == _cp_var)
	     return _QM[_qp]*_grad_phi[_j][_qp]*_grad_test[_i][_qp]+_dQ[_qp]*_grad_u[_qp]*_grad_test[_i][_qp]*_phi[_j][_qp]
     +_dQM[_qp]*_grad_cp[_qp]*_grad_test[_i][_qp]*_phi[_j][_qp];
    else if (jvar == _cv_var)
        return  _dQv[_qp]*_grad_u[_qp]*_grad_test[_i][_qp]*_phi[_j][_qp]
     +_dQMv[_qp]*_grad_cp[_qp]*_grad_test[_i][_qp]*_phi[_j][_qp];
     else
        return 0;
}
