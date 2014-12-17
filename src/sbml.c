/* sbml.c
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
/* some usefull funcitons                                                     */
/* -------------------------------------------------------------------------- */

/* make a problem description for R */
SEXP generateProblemMsg(XMLError_t *sbml_err) {

    SEXP out = R_NilValue;
    SEXP listv = R_NilValue;
    
    PROTECT(out = Rf_allocVector(VECSXP, 4));

    SET_VECTOR_ELT(out, 0, Rf_ScalarInteger(XMLError_getErrorId(sbml_err)));
    SET_VECTOR_ELT(out, 1, Rf_ScalarInteger(XMLError_getLine(sbml_err)));
    SET_VECTOR_ELT(out, 2, Rf_ScalarInteger(XMLError_getColumn(sbml_err)));
    SET_VECTOR_ELT(out, 3, Rf_mkString(XMLError_getMessage(sbml_err)));

    PROTECT(listv = Rf_allocVector(STRSXP, 4));
    SET_STRING_ELT(listv, 0, Rf_mkChar("id"));
    SET_STRING_ELT(listv, 1, Rf_mkChar("line"));
    SET_STRING_ELT(listv, 2, Rf_mkChar("column"));
    SET_STRING_ELT(listv, 3, Rf_mkChar("message"));
    Rf_setAttrib(out, R_NamesSymbol, listv);

    UNPROTECT(2);

    return out;

}


/* get species reference */
SEXP getSpeciesReference(Reaction_t *react, unsigned int nspec, unsigned int repro) {

    SEXP id        = R_NilValue;
    SEXP spec      = R_NilValue;
    SEXP stoich    = R_NilValue;
    SEXP speclist  = R_NilValue;
    SEXP speclistn = R_NilValue;

    unsigned int i;
    SpeciesReference_t *met;

    if (nspec > 0) {
    
        PROTECT(id     = Rf_allocVector(STRSXP, nspec));
        PROTECT(spec   = Rf_allocVector(STRSXP, nspec));
        PROTECT(stoich = Rf_allocVector(REALSXP, nspec));

        for (i = 0; i < nspec; i++) {
            if (0 == repro) {
                met = Reaction_getReactant(react, i);
            }
            else {
                met = Reaction_getProduct(react, i);
            }

            /* id */
            if (SpeciesReference_isSetId(met)) {
                SET_STRING_ELT(id, i, Rf_mkChar(SpeciesReference_getId(met)));
            }
            else {
                SET_STRING_ELT(id, i, Rf_mkChar("no_id"));
            }

            /* species reference */
            if (SpeciesReference_isSetSpecies(met)) {
                SET_STRING_ELT(spec, i, Rf_mkChar(SpeciesReference_getSpecies(met)));
            }
            else {
                SET_STRING_ELT(spec, i, Rf_mkChar("no_species"));
            }

            /* stoichiometry */
            if (SpeciesReference_isSetStoichiometry(met)) {
                REAL(stoich)[i] = SpeciesReference_getStoichiometry(met);
            }
            else {
                REAL(stoich)[i] = 1;
            }
        }

        PROTECT(speclist = Rf_allocVector(VECSXP, 3));
        SET_VECTOR_ELT(speclist, 0, id);
        SET_VECTOR_ELT(speclist, 1, spec);
        SET_VECTOR_ELT(speclist, 2, stoich);

        PROTECT(speclistn = Rf_allocVector(STRSXP, 3));
        SET_STRING_ELT(speclistn, 0, Rf_mkChar("id"));
        SET_STRING_ELT(speclistn, 1, Rf_mkChar("species"));
        SET_STRING_ELT(speclistn, 2, Rf_mkChar("stoichiometry"));
        Rf_setAttrib(speclist, R_NamesSymbol, speclistn);

        UNPROTECT(5);

    }
    else {
        speclist = R_NilValue;
    }
    
    return speclist;
}

    
