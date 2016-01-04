/******************************************************************************
 *
 * MantaFlow fluid solver framework
 * Copyright 2011-2014 Tobias Pfaff, Nils Thuerey 
 *
 * This program is free software, distributed under the terms of the
 * GNU General Public License (GPL) 
 * http://www.gnu.org/licenses
 *
 * Main
 *
 ******************************************************************************/

#ifndef _MANTA_H
#define _MANTA_H

// Remove preprocessor keywords, so there won't infere with autocompletion etc.
#define KERNEL
#define PYTHON

// Define plugin documentation group
// all plugin functions and classes will automatically be added to this group
//! @defgroup Plugins Plugins
//! @defgroup PyClasses Classes exposed to Python

#include "general.h"
#include "vectorbase.h"
#include "registry.h"
#include "pclass.h"
#include "pconvert.h"
#include "fluidsolver.h"

#endif