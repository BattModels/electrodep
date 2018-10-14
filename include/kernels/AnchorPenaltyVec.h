//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html

#ifndef ANCHORPENALTYVEC_H
#define ANCHORPENALTYVEC_H

#include "Kernel.h"
#include "DerivativeMaterialInterface.h"

// Forward Declarations
class AnchorPenaltyVec;

template <>
InputParameters validParams<AnchorPenaltyVec>();

/**
 * AnchorPenalty is a constraint kernel adds a penalty
 * to each order parameter to
 * enforce \f$ \sum_n h_i(\eta_i) \equiv 1 \f$.
 */
class AnchorPenaltyVec : public DerivativeMaterialInterface<Kernel>
{
public:
  AnchorPenaltyVec(const InputParameters & parameters);

protected:
  virtual Real computeQpResidual();
  virtual Real computeQpJacobian();
  virtual Real computeQpOffDiagJacobian(unsigned int jvar);

  //  RealGradient gradL();
  /// the \f$ \nabla(L\psi) \f$ term
  //RealGradient NablaLPsi();

  /// Switching function names
  const VectorVariableValue & _n;
  unsigned int _n_id;
  VectorMooseVariable & _n_var;
  const VectorVariablePhiValue & _vec_phi;
  const Real _penalty;
  const MaterialProperty<Real> & _L;
  const bool _variable_L;
};

#endif // ANCHORPENALTYVEC_H
