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
  electrodepTestApp::registerAll(
				 _factory, _action_factory, _syntax, getParam<bool>("allow_test_objects"));
}

electrodepTestApp::~electrodepTestApp() {}

void
electrodepTestApp::registerAll(Factory & f, ActionFactory & af, Syntax & s, bool use_test_objs)
{
  electrodepApp::registerAll(f, af, s);
  if (use_test_objs)
    {
      Registry::registerObjectsTo(f, {"electrodepTestApp"});
      Registry::registerActionsTo(af, {"electrodepTestApp"});
    }
}

void
electrodepTestApp::registerApps()
{
  registerApp(electrodepApp);
  registerApp(electrodepTestApp);
}

/***************************************************************************************************                                                                     
 *********************** Dynamic Library Entry Points - DO NOT MODIFY ******************************                                                                     
 **************************************************************************************************/
// External entry point for dynamic application loading                                                                                                                  
extern "C" void
electrodepTestApp__registerAll(Factory & f, ActionFactory & af, Syntax & s)
{
  electrodepTestApp::registerAll(f, af, s);
}
extern "C" void
electrodepTestApp__registerApps()
{
  electrodepTestApp::registerApps();
}
