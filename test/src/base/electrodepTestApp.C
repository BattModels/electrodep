//* This file is part of the MOOSE framework
//* https://www.mooseframework.org
//*
//* All rights reserved, see COPYRIGHT for full restrictions
//* https://github.com/idaholab/moose/blob/master/COPYRIGHT
//*
//* Licensed under LGPL 2.1, please see LICENSE for details
//* https://www.gnu.org/licenses/lgpl-2.1.html
#include "electrodepTestApp.h"
#include "electrodepApp.h"
#include "Moose.h"
#include "AppFactory.h"
#include "MooseSyntax.h"
#include "ModulesApp.h"

template <>
InputParameters
validParams<electrodepTestApp>()
{
  InputParameters params = validParams<electrodepApp>();
  return params;
}

electrodepTestApp::electrodepTestApp(InputParameters parameters) : MooseApp(parameters)
{
  Moose::registerObjects(_factory);
  ModulesApp::registerObjects(_factory);
  electrodepApp::registerObjectDepends(_factory);
  electrodepApp::registerObjects(_factory);

  Moose::associateSyntax(_syntax, _action_factory);
  ModulesApp::associateSyntax(_syntax, _action_factory);
  electrodepApp::associateSyntaxDepends(_syntax, _action_factory);
  electrodepApp::associateSyntax(_syntax, _action_factory);

  Moose::registerExecFlags(_factory);
  ModulesApp::registerExecFlags(_factory);
  electrodepApp::registerExecFlags(_factory);

  bool use_test_objs = getParam<bool>("allow_test_objects");
  if (use_test_objs)
  {
    electrodepTestApp::registerObjects(_factory);
    electrodepTestApp::associateSyntax(_syntax, _action_factory);
    electrodepTestApp::registerExecFlags(_factory);
  }
}

electrodepTestApp::~electrodepTestApp() {}

void
electrodepTestApp::registerApps()
{
  registerApp(electrodepApp);
  registerApp(electrodepTestApp);
}

void
electrodepTestApp::registerObjects(Factory & /*factory*/)
{
  /* Uncomment Factory parameter and register your new test objects here! */
}

void
electrodepTestApp::associateSyntax(Syntax & /*syntax*/, ActionFactory & /*action_factory*/)
{
  /* Uncomment Syntax and ActionFactory parameters and register your new test objects here! */
}

void
electrodepTestApp::registerExecFlags(Factory & /*factory*/)
{
  /* Uncomment Factory parameter and register your new execute flags here! */
}

/***************************************************************************************************
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************
 **************************************************************************************************/
// External entry point for dynamic application loading
extern "C" void
electrodepTestApp__registerApps()
{
  electrodepTestApp::registerApps();
}

// External entry point for dynamic object registration
extern "C" void
electrodepTestApp__registerObjects(Factory & factory)
{
  electrodepTestApp::registerObjects(factory);
}

// External entry point for dynamic syntax association
extern "C" void
electrodepTestApp__associateSyntax(Syntax & syntax, ActionFactory & action_factory)
{
  electrodepTestApp::associateSyntax(syntax, action_factory);
}

// External entry point for dynamic execute flag loading
extern "C" void
electrodepTestApp__registerExecFlags(Factory & factory)
{
  electrodepTestApp::registerExecFlags(factory);
}
