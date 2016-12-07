/* init.c
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

#include <R.h>
#include <Rinternals.h>

#include "sybilSBML.h"

#include <R_ext/Rdynload.h>

static const R_CallMethodDef callMethods[] = {
    {"isSBMLdocptr",               (DL_FUNC) &isSBMLdocptr,               1},
    {"isSBMLmodptr",               (DL_FUNC) &isSBMLmodptr,               1},
    {"isNULLptr",                  (DL_FUNC) &isNULLptr,                  1},
    {"getLibSBMLversion",          (DL_FUNC) &getLibSBMLversion,          0},
    {"initSBML",                   (DL_FUNC) &initSBML,                   0},
    {"delDocument",                (DL_FUNC) &delDocument,                1},
    {"delModel",                   (DL_FUNC) &delModel,                   1},
    {"readSBMLfile",               (DL_FUNC) &readSBMLfile,               2},
    {"getSBMLlevel",               (DL_FUNC) &getSBMLlevel,               1},
    {"getSBMLversion",             (DL_FUNC) &getSBMLversion,             1},
    {"validateDocument",           (DL_FUNC) &validateDocument,           1},
    {"getSBMLerrors",              (DL_FUNC) &getSBMLerrors,              1},
    {"getSBMLmodel",               (DL_FUNC) &getSBMLmodel,               2},
    {"getSBMLmodId",               (DL_FUNC) &getSBMLmodId,               1},
    {"getSBMLmodName",             (DL_FUNC) &getSBMLmodName,             1},
    {"getSBMLmodNotes",            (DL_FUNC) &getSBMLmodNotes,            1},
    {"getSBMLmodAnnotation",       (DL_FUNC) &getSBMLmodAnnotation,       1},
    {"getSBMLnumCompart",          (DL_FUNC) &getSBMLnumCompart,          1},
    {"getSBMLnumSpecies",          (DL_FUNC) &getSBMLnumSpecies,          1},
    {"getSBMLnumReactions",        (DL_FUNC) &getSBMLnumReactions,        1},
    {"getSBMLunitDefinitionsList", (DL_FUNC) &getSBMLunitDefinitionsList, 1},
    {"getSBMLCompartList",         (DL_FUNC) &getSBMLCompartList,         1},
    {"getSBMLSpeciesList",         (DL_FUNC) &getSBMLSpeciesList,         1},
    {"getSBMLReactionsList",       (DL_FUNC) &getSBMLReactionsList,       1},
    {"exportSBML",                 (DL_FUNC) &exportSBML,                 33},
    {"getSBMLFbcversion",          (DL_FUNC) &getSBMLFbcversion,          1},
    {NULL, NULL, 0}
};


/* -------------------------------------------------------------------------- */

void R_init_sybilSBML(DllInfo *info) {
    R_registerRoutines(info, NULL, callMethods, NULL, NULL);
    R_useDynamicSymbols(info, FALSE);
}