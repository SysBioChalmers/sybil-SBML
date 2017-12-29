/* sybilSBML.h
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

#include "sbml.h"

/* -------------------------------------------------------------------------- */
/* help functions                                                             */
/* -------------------------------------------------------------------------- */

/* check for pointer to sbml document */
SEXP isSBMLdocptr(SEXP ptr);

/* check for pointer to sbml model */
SEXP isSBMLmodptr(SEXP ptr);

/* check for NULL pointer */
SEXP isNULLptr(SEXP ptr);


/* -------------------------------------------------------------------------- */
/* API functions                                                              */
/* -------------------------------------------------------------------------- */

/* get libsbml version number (dotted version) */
SEXP getLibSBMLversion();

/* initialize sybilSBML */
SEXP initSBML(void);

/* remove sbml document pointer */
SEXP delDocument(SEXP sbmldoc);

/* remove model pointer */
SEXP delModel(SEXP sbmlmodel);

/* read SBML file */
SEXP readSBMLfile(SEXP fname, SEXP ptrtype);

/* get sbml document level */
SEXP getSBMLlevel(SEXP sbmldoc);

/* get sbml document version */
SEXP getSBMLversion(SEXP sbmldoc);

/* get sbml document FBC version */
SEXP getSBMLFbcversion(SEXP sbmldoc);

/* validate SBML document */
SEXP validateDocument(SEXP sbmldoc);

/* get SBML errors */
SEXP getSBMLerrors(SEXP sbmldoc);

/* get sbml model from sbml document */
SEXP getSBMLmodel(SEXP sbmldoc, SEXP ptrtype);

/* get model id */
SEXP getSBMLmodId(SEXP sbmlmod);

/* get model name */
SEXP getSBMLmodName(SEXP sbmlmod);

/* get model notes */
SEXP getSBMLmodNotes(SEXP sbmlmod);

/* get model annotation */
SEXP getSBMLmodAnnotation(SEXP sbmlmod);

/* get number of compartments */
SEXP getSBMLnumCompart(SEXP sbmlmod);

/* get number of species (metabolites) */
SEXP getSBMLnumSpecies(SEXP sbmlmod);

/* get number of reactions */
SEXP getSBMLnumReactions(SEXP sbmlmod);

/* get list of unit definitions */
SEXP getSBMLunitDefinitionsList(SEXP sbmlmod);

/* get list of compartments */
SEXP getSBMLCompartList(SEXP sbmlmod);

/* get list of species (metabolites) */
SEXP getSBMLSpeciesList(SEXP sbmlmod);

/* get list of groups */
SEXP getSBMLGroupsList(SEXP sbmlmod);

/* get list of reactions */
SEXP getSBMLReactionsList(SEXP sbmlmod);

/* export Modelorg to SBML*/
SEXP exportSBML (SEXP version, SEXP level, SEXP FbcLevel, SEXP filename, SEXP sybil_max, SEXP mod_desc, SEXP mod_name, SEXP mod_compart, SEXP met_id, SEXP met_name, SEXP met_comp, SEXP met_form, SEXP met_charge, SEXP react_id, SEXP react_name, SEXP react_rev, SEXP lowbnd, SEXP uppbnd, SEXP obj_coef, SEXP subSys, SEXP subSysGroups, SEXP gpr, SEXP SMatrix, SEXP mod_notes, SEXP mod_anno, SEXP com_notes , SEXP com_anno, SEXP met_notes, SEXP met_anno, SEXP met_bnd , SEXP react_notes, SEXP react_anno, SEXP ex_react, SEXP allgenes);
