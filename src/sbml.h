/* sbml.h
   Link to libSBML for sybil.
 
   Copyright (C) 2010-2013 Gabriel Gelius-Dietrich, Dpt. for Bioinformatics,
   Institute for Informatics, Heinrich-Heine-University, Duesseldorf, Germany.
   All right reserved.
   Email: geliudie@uni-duesseldorf.de
 
   This file is part of sybilSBML.
 
   SybilSBML is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.
 
   SybilSBML is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.
 
   You should have received a copy of the GNU General Public License
   along with sybilSBML.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stdlib.h>

#include <sbml/SBMLDocument.h>

/* avoid remapping of Rf_<function> to <function> in R header files */
#ifndef R_NO_REMAP
#define R_NO_REMAP
#endif /* R_NO_REMAP */

/* use strict R headers */
#ifndef STRICT_R_HEADERS
#define STRICT_R_HEADERS
#endif /* STRICT_R_HEADERS */

#include <R.h>
#include <Rinternals.h>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

/* -------------------------------------------------------------------------- */
/* NULL */
#define checkIfNil(cp) do { \
    if (R_ExternalPtrAddr(cp) == NULL) \
        Rf_error("You passed a nil value!"); \
} while (0)

/* -------------------------------------------------------------------------- */
/* sbml document */
#define checkTypeOfDocument(cp) do { \
    if ( (TYPEOF(cp) != EXTPTRSXP) || (R_ExternalPtrTag(cp) != tagSBMLdocument) ) \
        Rf_error("You must pass a pointer to an sbml document structure!"); \
} while (0)

#define checkDocument(p) do { \
    checkIfNil(p); \
    checkTypeOfDocument(p); \
} while (0)


/* -------------------------------------------------------------------------- */
/* sbml model */
#define checkTypeOfModel(cp) do { \
    if ( (TYPEOF(cp) != EXTPTRSXP) || (R_ExternalPtrTag(cp) != tagSBMLmodel) ) \
        Rf_error("You must pass a pointer to an sbml model structure!"); \
} while (0)

#define checkModel(p) do { \
    checkIfNil(p); \
    checkTypeOfModel(p); \
} while (0)


/* -------------------------------------------------------------------------- */
/* some usefull funcitons                                                     */
/* -------------------------------------------------------------------------- */

/* make a problem description for R */
SEXP generateProblemMsg(XMLError_t *sbml_err);

/* get species reference */
SEXP getSpeciesReference(Reaction_t *react, unsigned int nspec, unsigned int repro);
