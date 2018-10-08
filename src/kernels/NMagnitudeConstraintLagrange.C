//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#include "NMagnitudeConstraintLagrange.h"

registerMooseObject("electrodepApp", NMagnitudeConstraintLagrange);

template <>
InputParameters
validParams<NMagnitudeConstraintLagrange>()
{
  InputParameters params = validParams<Kernel>();
  params.addClassDescription("Lagrange multiplier kernel to constrain the magnitude of n to 1 "
                             "This kernel acts on the lagrange "
                             "multiplier variable.");
  params.addRequiredCoupledVar("nlccomps", "nlc components, one for each dimension");
  params.addParam<Real>("epsilon", 1e-4, "Shift factor to avoid a zero pivot");
  return params;
}

NMagnitudeConstraintLagrange::NMagnitudeConstraintLagrange(const InputParameters & parameters)
  : Kernel(parameters),
    _num_comps(coupledComponents("nlccomps")),
    //    _nlc_names(getParam<std::vector<VariableName>>("nlccomps")),
    //    _num_comps(_nlc_names.size()),
    _nlc_comp(_num_comps),
    _number_of_nl_variables(_fe_problem.getNonlinearSystemBase().nVariables()),
    _j_eta(_number_of_nl_variables, -1),
    _epsilon(getParam<Real>("epsilon"))
{
  // fetch switching functions (for the residual) and h derivatives (for the Jacobian)
  for (unsigned int i = 0; i < _num_comps; ++i)
  {
    _nlc_comp[i] = &coupledValue("nlccomps", i);
    //    _nlc_comp_var[i] = coupled("nlccomps", i);

    unsigned int num = coupled("nlccomps", i);
    if (num < _number_of_nl_variables)
      _j_eta[num] = i;
  }
}

Real
NMagnitudeConstraintLagrange::computeQpResidual()
{
  Real g = -_epsilon * _u[_qp] - 1.0;
  for (unsigned int i = 0; i < _num_comps; ++i)
    g += (*_nlc_comp[i])[_qp] * (*_nlc_comp[i])[_qp];
 
  return _test[_i][_qp] * g;
}

Real
NMagnitudeConstraintLagrange::computeQpJacobian()
{
  return _test[_i][_qp] * -_epsilon * _phi[_j][_qp];
}

Real
NMagnitudeConstraintLagrange::computeQpOffDiagJacobian(unsigned int j_var)
{
  const int eta = _j_eta[j_var];

  if (eta >= 0)
    return 2.0 * (*_nlc_comp[eta])[_qp] * _phi[_j][_qp] * _test[_i][_qp];
  else
    return 0.0;
}
