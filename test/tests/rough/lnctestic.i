# implementation of the grand-potential phase-field model based on M.Plapp PRE 84,031601(2011)+D.Cogswell PRE92, 011301(R) (2015)
# w is the chemical potential, eta is the phase-field, pot is the electric potential, op is the overpotential
# add Bulter-Volmer equation with non-linear kinetics by ZJH Oct 2nd 2017
# major modifs and liquid electrolyte with orientation order by Zeeshan Ahmad
[Mesh]
  type = GeneratedMesh
  dim = 2
  nx = 200
  xmax = 200
  ny = 200
  ymax = 200
#  elem_type = QUAD9
[]

[GlobalParams]
  enable_jit = true
  penalty = 1.0 
  constfactor = 1.007e-8
[]

[Variables]
  [./w]
# order=SECOND
# family= LAGRANGE
  [../]
  [./eta]
#order=SECOND
# family= LAGRANGE
#  scaling = 1e+1
  [../]
  [./pot]
  #order=SECOND
  # family= LAGRANGE
#  scaling = 1e+1
  [../]
  [./nlcx]
  order = FIRST
  family = LAGRANGE
#  scaling = 1e+06
  [../]
  [./nlcy]
  order = FIRST
  family = LAGRANGE
#  scaling = 1e+06
  [../]
  [./lambda]
  [../]
[]

[AuxVariables]
  [./epen]
  order = FIRST
  family = MONOMIAL
  [../]
  [./surfpen]
  order = FIRST
  family = MONOMIAL
  [../]
[]

[Functions]
  [./ic_func_eta]
  type = ParsedFunction
  value = 0.5*(1.0-1.0*tanh((x-20)*2))
  [../]
  [./ic_func_c]
  type = ParsedFunction
  value = 0
  [../]
  [./ic_func_pot]
  type = ParsedFunction
#value=0.001*x-0.1
  value = -0.1*(1.0-tanh((x-20)*2)) 
  [../]
#  [./ic_func_op]
#  type = ParsedFunction
#  value = 0
#  [../]
[]
  
[ICs]
  [./nlcx]
  variable = nlcx
  type = ConstantIC
  value = 0.0
  [../]
  [./nlcy]
  variable = nlcy
  type = ConstantIC
  value = 1.0
  [../]
  [./eta]
  variable = eta
  type = FunctionIC
  function = ic_func_eta
  [../]
  [./w]
  variable = w
  type = FunctionIC
  function = ic_func_c
  [../]
  [./pot]
  variable = pot
  type = FunctionIC
  function = ic_func_pot
  [../]
[]

[BCs]
  active = 'bottom_eta top_eta left_eta right_eta bottom_w top_w left_w right_w left_pot right_pot right_directorx right_directory'
  [./bottom_eta]
  type = NeumannBC
  variable = 'eta'
  boundary = 'bottom'
  value = 0
  [../]
  [./top_eta]
  type = NeumannBC
  variable = 'eta'
  boundary = 'top'
  value = 0
  [../]
  [./left_eta]
  type = DirichletBC
  variable = 'eta'
  boundary = 'left'
  value = 1
  [../]
  [./right_eta]
  type = DirichletBC
  variable = 'eta'
  boundary = 'right'
  value = 0
  [../]
  [./bottom_w]
  type = NeumannBC
  variable = 'w'
  boundary = 'bottom'
  value = 0
  [../]
  [./top_w]
  type = NeumannBC
  variable = 'w'
  boundary = 'top'
  value = 0.0
  [../]
  [./left_w]
  type = NeumannBC
  variable = 'w'
  boundary = 'left'
  value = 0
  [../]
  [./right_w]
  type = DirichletBC
  variable = 'w'
  boundary = 'right'
  value = 0.0
  [../]
  [./left_pot]
  type = DirichletBC
  variable = 'pot'
  boundary = 'left'
  value = -0.2
  # twice the value in pre ic_func_eta
  [../]
  [./right_pot]
  type = DirichletBC
  variable = 'pot'
  boundary = 'right'
  value = 0
  [../]
  [./right_directorx]
  type = DirichletBC
  variable = 'nlcx'
  boundary = 'right'
  value = 0
  [../]
  [./right_directory]
  type = DirichletBC
  variable = 'nlcy'
  boundary = 'right'
  value = 1
  [../]
  [./left_directorx]
  type = DirichletBC
  variable = 'nlcx'
  boundary = 'left'
  value = 0
  [../]
  [./left_directory]
  type = DirichletBC
  variable = 'nlcy'
  boundary = 'left'
  value = 1
  [../]
[]

[Kernels]
  active = 'nlcx nlcy nlcxmag nlcymag lagrange w_dot Diffusion1 Diffusion2 poteqn coupled_pot BVnlc coupled_etadot AC_bulk AC_int e_dot AC_bulk_lc_penalty Noiseeta'
  [./nlcx]
  type = Diffusion #Take care of the minus sign if adding any other terms
  variable = nlcx
  [../]
  [./nlcy]
  type = Diffusion
  variable = nlcy
  [../]
  [./nlcxmag]
  type = NMagnitudeConstraintN
  variable = nlcx
  lambda = lambda
  [../]
  [./nlcymag]
  type = NMagnitudeConstraintN
  variable = nlcy
  lambda = lambda
  [../]
  [./lagrange]
  type = NMagnitudeConstraintLagrangeComponent
  variable = lambda
  nx = nlcx
  ny = nlcy
#  nlccomps    = 'nlcx nlcy'
  epsilon = 1e-3
  [../]
  [./w_dot]
  type = SusceptibilityTimeDerivative
  variable = w
  f_name = chi
  args = 'w' # in this case chi (the susceptibility) is simply a constant
  [../]
  [./Diffusion1]
  type = MatDiffusion
  variable = w
  D_name = D
  args = 'eta w'
  [../]
  [./Diff2]
  type = MatDiffusion
  variable = w
  args = 'eta w'
  D_name = DN
  [../]
  [./Diffusion2]
  type = Diff
  variable = w
  cv = eta
  Q_name = zz
  QM_name = DN
  cp = pot
 #args = 'w'
  [../]
  [./Noisew]
  type = LangevinNoise
  variable = w
  amplitude = 0.0
  [../]
  [./coupled_etadot]
  type = CoupledSusceptibilityTimeDerivative
  variable = w
  v = eta
  f_name = ft
  args = 'eta'
  [../]
  [./elecN]
  type = Electroneutrality
  variable = pot
  eta = eta
  sigma = Le1    #before phi
#  QM_name = 0.
  [../]
  [./poteqn]
  type = MatDiffusion
  variable = pot
  D_name = Le1
  args = 'eta'
  [../]
  [./coupled_pot]
  type = CoupledSusceptibilityTimeDerivative
  variable = pot
  v = eta
  f_name = ft2
  args = 'eta'
  [../]
  [./BVnlc]
  type = Kineticslc
  variable = eta
  f_name = G
  args = 'pot w'
  nx = 'nlcx'
  ny = 'nlcy'
  [../]
  [./AC_bulk]
  type = AllenCahn
  variable = eta
  f_name = FF
  args = 'w'
  [../]
  [./AC_bulk_lc_penalty]
  type = AnchorPenaltyScalar
  variable = eta
  nx = 'nlcx'
  ny = 'nlcy'
  [../]
  [./AC_int]
  type = ACInterface
  variable = eta
#  kappa_name = kappa_op
  [../]
  [./Noiseeta]
  type = ConservedLangevinNoise
  variable = eta
  amplitude = 0.02
  noise = normal_masked_noise
  [../]
  [./e_dot]
  type = TimeDerivative
  variable = eta
  [../]
[]

[AuxKernels]
  [./epen]
  type = EnergyPenalty
  variable = epen
  nx = nlcx
  ny = nlcy
  [../]
  [./surfpenalty]
  type = SurfacePenalty
  variable = surfpen
  nx = nlcx
  ny = nlcy
  eta = eta
  [../]
[]

[Materials]
  [./constants]
  type = GenericConstantMaterial
#ul is the free energy density in the liquid phase
  prop_names  = 'kappa_op  M0    S1   S2      CL     L        Ls        B      es      el     zz    ul         us   dv   RT    Fconst      K     Liplusm3permol ft2'
  prop_values = '0.33    319.7  1e7  1.19  14.4   6.318   1.347e-1  2.64  -13.64   2.598    0   6.854e-2   13.64  5.34  0.987   9.649e-5  3.957e-7  0.05022 7.422e-3'
# Ls value changed after ZJH suggested 100 times the value for limiting current i0
#  M0 M1 Normalized diffusion coefficient for liquid and solid B normalized constant for nF
#  M0=10^12*D    B=zF/(1000*R*T)   length 1 um, time 1s, energy is normalized by RT
# t transference number D+=D/(n*(1-t)) D-=D/(n*t)
  [../]
#  [./constfactor_lc]
#  type = ParsedMaterial
#  f_name = constfactor
#  function = '1/2 * K * Liplusm3permol / RT'
#  material_property_names = 'K Liplusm3permol RT'
#  [../]
  [./liquid_GrandPotential]
  type = DerivativeParsedMaterial
  function = 'ul - RT * log(1 + exp((w - el)/RT))'
  args = 'w'
  f_name = f1
  material_property_names = 'ul el RT'
  derivative_order = 2
    #outputs = exodus
  [../]
  [./solid_GrandPotential]
  type = DerivativeParsedMaterial
  function = 'us - RT * log(1 + exp((w - es) / RT))'
  args = 'w'
  f_name = f2
  material_property_names = 'us es RT'
  derivative_order = 2
   # outputs = exodus
  [../]
  [./switching_function]
  type = SwitchingFunctionMaterial
  eta ='eta'
  h_order = HIGH
  function_name = h
  # outputs = exodus
  [../]
  [./barrier_function]
  type = BarrierFunctionMaterial
  eta = eta
  [../]
  [./total_GrandPotential]
  type = DerivativeTwoPhaseMaterial
  args = 'w'
  eta = eta
  fa_name = f1
  fb_name = f2
  derivative_order = 2
  W = 2.64
   # outputs = exodus
  [../]
  [./coupled_eta_function]
  type = DerivativeParsedMaterial
  function = '-(cs * dv - cl) * dh'  # in this code cs=-cs h=eta dh=1
  args = 'w eta'
  f_name = ft
  material_property_names = 'dh:=D[h(eta),eta] h(eta) dv cs:=D[f2(w),w] cl:=D[f1(w),w]'
  derivative_order = 1
  [../]
  [./susceptibility]
  type = DerivativeParsedMaterial
  function = '-d2F1 * (1-h) - d2F2 * h * dv'
  args = 'w'
  f_name = chi
  derivative_order = 1
#outputs = exodus
  material_property_names = 'h(eta) dv d2F1:=D[f1(w),w,w] d2F2:=D[f2(w),w,w]'
  [../]
  [./Diffusion_coefficient]
  type = DerivativeParsedMaterial     #cn=h*(1-cs)+(1-h)*(1-2*cl)
  function = '-M0 * cl * (1 - h)'  #c is -c, removed 1-h extra factor which was there for convergence
  f_name = D
  args = 'eta w'
  derivative_order = 2
  material_property_names = 'M0 cl:=D[f1,w] h(eta)'
  #  outputs = exodus
  [../]
  [./Free]
  type = DerivativeParsedMaterial
  f_name = FF
  material_property_names = 'B'
  args = 'eta'
  function = 'B*eta*eta*(1-eta)*(1-eta)'
  derivative_order = 2
  #outputs = exodus
  [../]
  [./Convection_coefficient]
  type = DerivativeParsedMaterial     #cn=h*(1-cs)+(1-h)*(1-2*cl)
  function = '-cl * (1-h) * Fconst / RT * M0 * (1-h)'  #c is -c
  args = 'eta w'
  f_name = DN
  derivative_order = 1
  material_property_names = 'M0 cl:=D[f1(w),w] h(eta) RT Fconst'
  #   outputs = exodus
  [../]
  [./ButlerVolmer]
  type = DerivativeParsedMaterial
  function = 'Ls * (exp(pot * Fconst/RT /2.) + cl * (1-h) * exp(-pot * Fconst/RT /2.)) * dh'
  #    function = 'L * (op)'
  args = 'pot w'
  f_name = G
  derivative_order = 1
  material_property_names = 'Ls dh:=D[h(eta),eta] h(eta) cl:=D[f1(w),w] RT Fconst'
  outputs = other
  [../]
  [./eta]
  type = ParsedMaterial
  f_name = etao
  args='eta'
  function = 'eta'
  outputs = other
  [../]
  [./concentration]
  type = ParsedMaterial
  f_name = c
  args='eta w'
  material_property_names = 'h(eta) dFl:=D[f1(w),w]'
  function = '-dFl * (1-h)'
  outputs = other
  [../]
  [./Le1]
  type = DerivativeParsedMaterial
  f_name = Le1
  function = 'S1*h + S2*(1-h)'
  material_property_names = 'S1 S2 h'
  #outputs = exodus
  derivative_order = 2
  [../]
#  [./mask_material]
#  type = ParsedMaterial
#  prop_names = 'mask_prop'
#  prop_values = 'mask_func'
#  [../]
  [./mask_func]
  type = ParsedMaterial
  f_name = mask_func
  args = eta
  material_property_names = ''
  function = 'if( eta<0.8, if(eta>0.2, 1.0, 0.0), 0.0 )'
  outputs = other
  [../]
[]

[UserObjects]
  [./normal_masked_noise]
    type = ConservedMaskedNormalNoise
    mask = mask_func
  [../]
[]

[Postprocessors]
  [./ETA]
  type = ElementIntegralMaterialProperty
  mat_prop = etao
  execute_on = 'initial timestep_end'
  [../]
  [./CONC]
  type = ElementIntegralMaterialProperty
  mat_prop = c
  execute_on = 'initial timestep_end'
  [../]
[]

[Preconditioning]
  [./SMP]
  type = SMP
  full = true
  petsc_options_iname = '-pc_type -ksp_grmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      121                  preonly       lu           4'
  [../]
[]

[Executioner]
  type = Transient
  scheme = bdf2
  verbose = True
  solve_type = 'Newton'
  dtmin = 1e-8
  l_max_its = 50
  l_tol = 1e-4
  nl_max_its = 50
  nl_rel_tol = 1e-6
  nl_abs_tol = 1e-8
  petsc_options_iname = '-pc_type -ksp_gmres_restart -sub_ksp_type -sub_pc_type -pc_asm_overlap'
  petsc_options_value = 'asm      31                  preonly      lu          4'
#  [./TimeStepper]
#    type = FunctionDT
#    time_t = '0        0.001    1    20'
#    time_dt ='1e-5    1.e-3   1.e-2 2.e-2 '
#     [../]
#  type = IterationAdaptiveDT
  dt=0.1
#  growth_factor = 1.5
#  optimal_iterations = 7
#  cutback_factor = 0.8
  end_time = 800
#  num_steps = 2
[]

[Outputs]
  exodus = false
  csv = true
  execute_on = 'TIMESTEP_END'
  [./other]        # creates input_other.e
  type = Exodus
  interval = 1
  [../]
  [./checkpt]
  type = Checkpoint
  num_files = 2
  interval = 20
  [../]
[]

[Debug]
  show_var_residual_norms = true
[]
