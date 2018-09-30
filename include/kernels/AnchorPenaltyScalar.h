//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef ANCHORPENALTYSCALAR_H
#define ANCHORPENALTYSCALAR_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"

// Forward Declarations
class AnchorPenaltyScalar;

template <>
InputParameters validParams<AnchorPenaltyScalar>();

/**
 * AnchorPenalty is a constraint kernel adds a penalty
 * to each order parameter to
 * enforce \f$ \sum_n h_i(\eta_i) \equiv 1 \f$.
 */
class AnchorPenaltyScalar : public DerivativeMaterialInterface<Kernel>
{
public:
  AnchorPenaltyScalar(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  //  RealGradient gradL();
  /// the \f$ \nabla(L\psi) \f$ term
  //RealGradient NablaLPsi();

  /// Switching function names
  const VariableValue & _nx;
  const VariableValue & _ny;
  unsigned int _nx_var;
  unsigned int _ny_var;
  //std::vector<const VariableValue *> _nlc_comp;
  const Real _penalty;
  //unsigned int _num_comps;

  /// number of non-linear variables in the problem
  const MaterialProperty<Real> & _L;

    /// flag set if L is a function of non-linear variables in args
  const bool _variable_L;
};

#endif // ANCHORPENALTYSCALAR_H
