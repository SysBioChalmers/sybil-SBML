/* sybilSBML.c
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


#include "sybilSBML.h"


static SEXP tagSBMLmodel;
static SEXP tagSBMLdocument;


/* -------------------------------------------------------------------------- */
/* Finalizer                                                                  */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* finalizer for sbml document objects */
static void sbmlDocumentFinalizer (SEXP sbmldoc) {
    if (!R_ExternalPtrAddr(sbmldoc)) {
        return;
    }
    else {
        delDocument(sbmldoc);
    }
}


/* -------------------------------------------------------------------------- */
/* finalizer for sbml model objects */
/*
static void sbmlModelFinalizer (SEXP sbmlmodel) {
    if (!R_ExternalPtrAddr(sbmlmodel)) {
        return;
    }
    else {
        delModel(sbmlmodel);
    }
}
*/


/* -------------------------------------------------------------------------- */
/* help functions                                                             */
/* -------------------------------------------------------------------------- */

/* check for pointer to sbml document */
SEXP isSBMLdocptr(SEXP ptr) {

    SEXP out = R_NilValue;

    if ( (TYPEOF(ptr) == EXTPTRSXP) &&
         (R_ExternalPtrTag(ptr) == tagSBMLdocument) ) {
        out = Rf_ScalarLogical(1);
    }
    else {
        out = Rf_ScalarLogical(0);
    }

    return out;
}

/* check for pointer to sbml model */
SEXP isSBMLmodptr(SEXP ptr) {

    SEXP out = R_NilValue;

    if ( (TYPEOF(ptr) == EXTPTRSXP) &&
         (R_ExternalPtrTag(ptr) == tagSBMLmodel) ) {
        out = Rf_ScalarLogical(1);
    }
    else {
        out = Rf_ScalarLogical(0);
    }

    return out;
}

/* check for NULL pointer */
SEXP isNULLptr(SEXP ptr) {

    SEXP out = R_NilValue;

    if ( (TYPEOF(ptr) == EXTPTRSXP) &&
         (R_ExternalPtrAddr(ptr) == NULL) ) {
        out = Rf_ScalarLogical(1);
    }
    else {
        out = Rf_ScalarLogical(0);
    }

    return out;
}


/* -------------------------------------------------------------------------- */
/* API-Functions                                                              */
/* -------------------------------------------------------------------------- */

/* -------------------------------------------------------------------------- */
/* initialize sybilSBML */
SEXP initSBML(void) {
    tagSBMLmodel    = Rf_install("TYPE_SBML_MODEL");
    tagSBMLdocument = Rf_install("TYPE_SBML_DOCUMENT");
    return R_NilValue;
}


/* -------------------------------------------------------------------------- */
/* get libsbml version number (dotted version) */
SEXP getLibSBMLversion() {

    SEXP out = R_NilValue;

    const char *vstr = getLibSBMLDottedVersion();

    out = Rf_mkString(vstr);

    return out;

}


/* -------------------------------------------------------------------------- */
/* remove sbml document pointer */
SEXP delDocument(SEXP sbmldoc) {

    SEXP out = R_NilValue;
    SBMLDocument_t *del = NULL;

    checkDocument(sbmldoc);

    del = R_ExternalPtrAddr(sbmldoc);

    SBMLDocument_free(del);
    R_ClearExternalPtr(sbmldoc);

    return out;
}


/* -------------------------------------------------------------------------- */
/* remove model pointer */
SEXP delModel(SEXP sbmlmodel) {

    SEXP out = R_NilValue;
    /* Model_t *del = NULL; */

    checkModel(sbmlmodel);

    /* del = R_ExternalPtrAddr(sbmlmodel); */

    /* Model_free(del); */
    R_ClearExternalPtr(sbmlmodel);

    return out;
}


/* -------------------------------------------------------------------------- */
/* read SBML file */
SEXP readSBMLfile(SEXP fname, SEXP ptrtype) {

    SEXP sfext = R_NilValue;
    SEXP ptr, class, file;
    const char *rfname = CHAR(STRING_ELT(fname, 0));
    
    SBMLDocument_t *sbmldoc;

    /* create sbml document pointer */
    PROTECT(ptr = Rf_allocVector(STRSXP, 1));
    SET_STRING_ELT(ptr, 0, STRING_ELT(ptrtype, 0));

    PROTECT(class = Rf_allocVector(STRSXP, 1));
    SET_STRING_ELT(class, 0, Rf_mkChar("sbml_doc_ptr"));

    PROTECT(file = Rf_allocVector(STRSXP, 1));
    SET_STRING_ELT(file, 0, Rf_mkChar("file_name"));

    /* read the model xml file */
    sbmldoc = readSBML(rfname);
    
    sfext = R_MakeExternalPtr(sbmldoc, tagSBMLdocument, R_NilValue);
    PROTECT(sfext);
    R_RegisterCFinalizerEx(sfext, sbmlDocumentFinalizer, TRUE);
    Rf_setAttrib(ptr, class, sfext);
    Rf_setAttrib(ptr, file, fname);
    Rf_classgets(ptr, class);

    UNPROTECT(4);

    return ptr;
}


/* -------------------------------------------------------------------------- */
/* get sbml document level */
SEXP getSBMLlevel(SEXP sbmldoc) {

    SEXP out = R_NilValue;
    unsigned int level;
    
    checkDocument(sbmldoc);

    level = SBMLDocument_getLevel(R_ExternalPtrAddr(sbmldoc));

    out = Rf_ScalarInteger(level);

    return out;
}


/* -------------------------------------------------------------------------- */
/* get sbml document version */
SEXP getSBMLversion(SEXP sbmldoc) {

    SEXP out = R_NilValue;
    unsigned int version;
    
    checkDocument(sbmldoc);

    version = SBMLDocument_getVersion(R_ExternalPtrAddr(sbmldoc));

    out = Rf_ScalarInteger(version);

    return out;
}


/* -------------------------------------------------------------------------- */
/* validate SBML document */
SEXP validateDocument(SEXP sbmldoc) {

    /*
        this is adopted from Michael Lawrence: rsbml
        Michael Lawrence (). rsbml: R support for SBML, using libsbml.
        R package version 2.18.0. http://www.sbml.org
        http://www.bioconductor.org/packages/release/bioc/html/rsbml.html
    */
    
    SEXP out   = R_NilValue;

    unsigned int validation;
    
    checkDocument(sbmldoc);

    /* number or errors logged when reading SBML file */
    validation  = SBMLDocument_getNumErrors(R_ExternalPtrAddr(sbmldoc));
    /* number or errors from structural and mathematical tests */
    validation += SBMLDocument_checkConsistency(R_ExternalPtrAddr(sbmldoc));

    if (validation > 0) {
        out = Rf_ScalarLogical(0);
    }
    else {
        out = Rf_ScalarLogical(1);
    }

    return out;
}


/* -------------------------------------------------------------------------- */
/* get SBML errors */
SEXP getSBMLerrors(SEXP sbmldoc) {

    /*
        this is adopted from Michael Lawrence: rsbml
        Michael Lawrence (). rsbml: R support for SBML, using libsbml.
        R package version 2.18.0. http://www.sbml.org
        http://www.bioconductor.org/packages/release/bioc/html/rsbml.html
    */
    
    SEXP out   = R_NilValue;
    SEXP listv = R_NilValue;
    SEXP info  = R_NilValue;
    SEXP warn  = R_NilValue;
    SEXP error = R_NilValue;
    SEXP fatal = R_NilValue;
    SEXP class = R_NilValue;
    SEXP el    = R_NilValue;

    XMLError_t *sbml_err;

    unsigned int nprob, i, ind;
    int ninfo = 0, nwarn = 0, nerror = 0, nfatal = 0, nunknown = 0;
    
    checkDocument(sbmldoc);

    nprob = SBMLDocument_getNumErrors(R_ExternalPtrAddr(sbmldoc));

    /*
    Rprintf("Model Errors: %i\n", nprob);
    */

    if (nprob > 0) {

        /* check how many infos, warnings, errors and fatals we have */
        for (i = 0; i < nprob; i++) {
            sbml_err = (XMLError_t *) SBMLDocument_getError(R_ExternalPtrAddr(sbmldoc), i);
            if (XMLError_isInfo(sbml_err)) {
                ninfo++;
            }
            else if (XMLError_isWarning(sbml_err)) {
                nwarn++;
            }
            else if (XMLError_isError(sbml_err)) {
                nerror++;
            }
            else if (XMLError_isFatal(sbml_err)) {
                nfatal++;
            }
            else {
                nunknown++;
            }
        }

        /*
        Rprintf("number of infos: %i, warnings: %i, errors: %i, falals: %i, unknowns: %i\n",
                ninfo, nwarn, nerror, nfatal, nunknown);
        */

        /*
            out will be a list of four elements:
              infos
              warnings
              errors
              fatals
            each of them is a list of three elements:
              id
              line
              column
              msg
        */
    
        PROTECT(out =   Rf_allocVector(VECSXP, 4));

        /* allocate space for each error list */
        PROTECT(info  = Rf_allocVector(VECSXP, ninfo));
        PROTECT(warn  = Rf_allocVector(VECSXP, nwarn));
        PROTECT(error = Rf_allocVector(VECSXP, nerror));
        PROTECT(fatal = Rf_allocVector(VECSXP, nfatal));

        SET_VECTOR_ELT(out, 0, info);
        SET_VECTOR_ELT(out, 1, warn);
        SET_VECTOR_ELT(out, 2, error);
        SET_VECTOR_ELT(out, 3, fatal);

        PROTECT(listv = Rf_allocVector(STRSXP, 4));
        SET_STRING_ELT(listv, 0, Rf_mkChar("infos"));
        SET_STRING_ELT(listv, 1, Rf_mkChar("warnings"));
        SET_STRING_ELT(listv, 2, Rf_mkChar("errors"));
        SET_STRING_ELT(listv, 3, Rf_mkChar("fatals"));
        Rf_setAttrib(out, R_NamesSymbol, listv);

        /* get the error messages */
        ninfo = 0, nwarn = 0, nerror = 0, nfatal = 0, nunknown = 0;
        for (i = 0; i < nprob; i++) {
            sbml_err = (XMLError_t *) SBMLDocument_getError(R_ExternalPtrAddr(sbmldoc), i);
            el = R_NilValue;
            ind = 0;
            if (XMLError_isInfo(sbml_err)) {
                ind = ninfo++;
                el  = info;
            }
            else if (XMLError_isWarning(sbml_err)) {
                ind = nwarn++;
                el  = warn;
            }
            else if (XMLError_isError(sbml_err)) {
                ind = nerror++;
                el  = error;
            }
            else if (XMLError_isFatal(sbml_err)) {
                ind = nfatal++;
                el  = fatal;
            }
            else {
                nunknown++;
            }
            SET_VECTOR_ELT(el, ind, generateProblemMsg(sbml_err));
        }

        /* make the list to be an instance of 'sbml_error' */
        PROTECT(class = Rf_allocVector(STRSXP, 1));
        SET_STRING_ELT(class, 0, Rf_mkChar("sbml_error"));
        Rf_classgets(out, class);

        UNPROTECT(7);

    } /* end if nprob > 0 */
    else {
        out = Rf_ScalarLogical(1);
    }

    return out;
}


/* -------------------------------------------------------------------------- */
/* get sbml model from sbml document */
SEXP getSBMLmodel(SEXP sbmldoc, SEXP ptrtype) {

    SEXP smext = R_NilValue;
    SEXP ptr, class;
    
    Model_t *sbmlmodel;

    checkDocument(sbmldoc);

    /* create model pointer */
    PROTECT(ptr = Rf_allocVector(STRSXP, 1));
    SET_STRING_ELT(ptr, 0, STRING_ELT(ptrtype, 0));

    PROTECT(class = Rf_allocVector(STRSXP, 1));
    SET_STRING_ELT(class, 0, Rf_mkChar("sbml_model_ptr"));

    /* get sbml model */
    sbmlmodel = SBMLDocument_getModel(R_ExternalPtrAddr(sbmldoc));

    smext = R_MakeExternalPtr(sbmlmodel, tagSBMLmodel, R_NilValue);
    PROTECT(smext);
    /* R_RegisterCFinalizerEx(smext, sbmlModelFinalizer, TRUE); */
    Rf_setAttrib(ptr, class, smext);
    Rf_classgets(ptr, class);

    UNPROTECT(3);

    return ptr;
}


/* -------------------------------------------------------------------------- */
/* get model id */
SEXP getSBMLmodId(SEXP sbmlmod) {

    SEXP out = R_NilValue;
    const char *mid;
    
    checkModel(sbmlmod);
    
    if (Model_isSetId(R_ExternalPtrAddr(sbmlmod))) {
        mid = Model_getId(R_ExternalPtrAddr(sbmlmod));
    }
    else {
        mid = "no_id";
    }

    out = Rf_mkString(mid);

    return out;
}


/* -------------------------------------------------------------------------- */
/* get model name */
SEXP getSBMLmodName(SEXP sbmlmod) {

    SEXP out = R_NilValue;
    const char *mnm;
    
    checkModel(sbmlmod);

    if (Model_isSetName(R_ExternalPtrAddr(sbmlmod))) {
        mnm = Model_getName(R_ExternalPtrAddr(sbmlmod));
    }
    else {
        mnm = "";
    }

    out = Rf_mkString(mnm);

    return out;
}


/* -------------------------------------------------------------------------- */
/* get number of compartments */
SEXP getSBMLnumCompart(SEXP sbmlmod) {

    SEXP out = R_NilValue;
    unsigned int nc;
    
    checkModel(sbmlmod);

    nc = Model_getNumCompartments(R_ExternalPtrAddr(sbmlmod));

    out = Rf_ScalarInteger(nc);

    return out;
}


/* -------------------------------------------------------------------------- */
/* get number of species (metabolites) */
SEXP getSBMLnumSpecies(SEXP sbmlmod) {

    SEXP out = R_NilValue;
    unsigned int nsp;
    
    checkModel(sbmlmod);

    nsp = Model_getNumSpecies(R_ExternalPtrAddr(sbmlmod));

    out = Rf_ScalarInteger(nsp);

    return out;
}


/* -------------------------------------------------------------------------- */
/* get number of reactions */
SEXP getSBMLnumReactions(SEXP sbmlmod) {

    SEXP out = R_NilValue;
    unsigned int nr;
    
    checkModel(sbmlmod);

    nr = Model_getNumReactions(R_ExternalPtrAddr(sbmlmod));

    out = Rf_ScalarInteger(nr);

    return out;
}


/* -------------------------------------------------------------------------- */
/* get list of unit definitions */
SEXP getSBMLunitDefinitionsList(SEXP sbmlmod) {

    SEXP out       = R_NilValue;
    SEXP class     = R_NilValue;
    SEXP unl       = R_NilValue;
    SEXP listv     = R_NilValue;
    SEXP listn     = R_NilValue;

    SEXP unitdefid = R_NilValue;
    SEXP unitdef   = R_NilValue;

    SEXP unitkind  = R_NilValue;
    SEXP unitscale = R_NilValue;
    SEXP unitexp   = R_NilValue;
    SEXP unitmult  = R_NilValue;

    unsigned int nud, nu, i, j;

    /* ListOf_t *udl; */
    UnitDefinition_t *udlel;
    Unit_t *uel;

    checkModel(sbmlmod);

    /* udl = Model_getListOfUnitDefinitions(R_ExternalPtrAddr(sbmlmod)); */
    nud = Model_getNumUnitDefinitions(R_ExternalPtrAddr(sbmlmod));
    
    if (nud > 0) {
        PROTECT(unitdefid = Rf_allocVector(STRSXP, nud));
        PROTECT(unitdef   = Rf_allocVector(VECSXP, nud));
        for (i = 0; i < nud; i++) {
            /* udlel = (UnitDefinition_t *) ListOf_get(udl, i); */
            udlel = Model_getUnitDefinition(R_ExternalPtrAddr(sbmlmod), i);

            /* id and unit */
            if (UnitDefinition_isSetId(udlel)) {
                SET_STRING_ELT(unitdefid, i, Rf_mkChar(UnitDefinition_getId(udlel)));

                nu = UnitDefinition_getNumUnits(udlel);
        
                PROTECT(unitkind  = Rf_allocVector(STRSXP, nu));
                PROTECT(unitscale = Rf_allocVector(INTSXP, nu));
                PROTECT(unitexp   = Rf_allocVector(INTSXP, nu));
                PROTECT(unitmult  = Rf_allocVector(REALSXP, nu));
            
                for (j = 0; j < nu; j++) {
                    uel = UnitDefinition_getUnit(udlel, j);

                    /* kind */
                    if (Unit_isSetKind(uel)) {
                        SET_STRING_ELT(unitkind, j, Rf_mkChar(UnitKind_toString(Unit_getKind(uel))));
                    }
                    else {
                        SET_STRING_ELT(unitkind, j, Rf_mkChar("no_kind"));
                    }
            
                    /* scale */
                    if (Unit_isSetScale(uel)) {
                        INTEGER(unitscale)[j] = Unit_getScale(uel);
                    }
                    else {
                        INTEGER(unitscale)[j] = 0;
                    }

                    /* exponent */
                    if (Unit_isSetExponent(uel)) {
                        INTEGER(unitexp)[j] = Unit_getExponent(uel);
                    }
                    else {
                        INTEGER(unitexp)[j] = 1;
                    }

                    /* multiplier */
                    if (Unit_isSetMultiplier(uel)) {
                        REAL(unitmult)[j] = Unit_getMultiplier(uel);
                    }
                    else {
                        REAL(unitmult)[j] = 1;
                    }

                }

                PROTECT(unl = Rf_allocVector(VECSXP, 4));
                SET_VECTOR_ELT(unl, 0, unitkind);
                SET_VECTOR_ELT(unl, 1, unitscale);
                SET_VECTOR_ELT(unl, 2, unitexp);
                SET_VECTOR_ELT(unl, 3, unitmult);

                PROTECT(listn = Rf_allocVector(STRSXP, 4));
                SET_STRING_ELT(listn, 0, Rf_mkChar("kind"));
                SET_STRING_ELT(listn, 1, Rf_mkChar("scale"));
                SET_STRING_ELT(listn, 2, Rf_mkChar("exponent"));
                SET_STRING_ELT(listn, 3, Rf_mkChar("multiplier"));
                Rf_setAttrib(unl, R_NamesSymbol, listn);
            
                SET_VECTOR_ELT(unitdef, i, unl);

                UNPROTECT(6);
            
            }
            else {
                SET_STRING_ELT(unitdefid, i, Rf_mkChar("no_id"));
            }
        }
    
        PROTECT(out = Rf_allocVector(VECSXP, 2));
        SET_VECTOR_ELT(out, 0, unitdefid);
        SET_VECTOR_ELT(out, 1, unitdef);

        PROTECT(listv = Rf_allocVector(STRSXP, 2));
        SET_STRING_ELT(listv, 0, Rf_mkChar("definition_id"));
        SET_STRING_ELT(listv, 1, Rf_mkChar("definition"));

        Rf_setAttrib(out, R_NamesSymbol, listv);

        /* make the list to be an instance of 'unit_definition' */
        PROTECT(class = Rf_allocVector(STRSXP, 1));
        SET_STRING_ELT(class, 0, Rf_mkChar("unit_definition"));
        Rf_classgets(out, class);

        UNPROTECT(5);
    }
    else {
        out = R_NilValue;
    }

    return out;
}


/* -------------------------------------------------------------------------- */
/* get list of compartments */
SEXP getSBMLCompartList(SEXP sbmlmod) {

    SEXP out      = R_NilValue;
    SEXP class    = R_NilValue;
    SEXP listv    = R_NilValue;
    SEXP compid   = R_NilValue;
    SEXP compname = R_NilValue;
    SEXP compout  = R_NilValue;

    unsigned int nc, i;

    /* ListOf_t *cl; */
    Compartment_t *clel;

    checkModel(sbmlmod);

    /* cl = Model_getListOfCompartments(R_ExternalPtrAddr(sbmlmod)); */
    nc = Model_getNumCompartments(R_ExternalPtrAddr(sbmlmod));
    
    if (nc > 0) {
        PROTECT(compid   = Rf_allocVector(STRSXP, nc));
        PROTECT(compname = Rf_allocVector(STRSXP, nc));
        PROTECT(compout  = Rf_allocVector(STRSXP, nc));

        for (i = 0; i < nc; i++) {
            /* clel = (Compartment_t *) ListOf_get(cl, i); */
            clel = Model_getCompartment(R_ExternalPtrAddr(sbmlmod), i);
            /* id */
            if (Compartment_isSetId(clel)) {
                SET_STRING_ELT(compid, i, Rf_mkChar(Compartment_getId(clel)));
            }
            else {
                SET_STRING_ELT(compid, i, Rf_mkChar("no_id"));
            }
            /* name */
            if (Compartment_isSetName(clel)) {
                SET_STRING_ELT(compname, i, Rf_mkChar(Compartment_getName(clel)));
            }
            else {
                SET_STRING_ELT(compname, i, Rf_mkChar(""));
            }
            /* outside */
            if (Compartment_isSetOutside(clel)) {
                SET_STRING_ELT(compout, i, Rf_mkChar(Compartment_getOutside(clel)));
            }
            else {
                SET_STRING_ELT(compout, i, Rf_mkChar(""));
            }
        }
    
        PROTECT(out = Rf_allocVector(VECSXP, 3));
        SET_VECTOR_ELT(out, 0, compid);
        SET_VECTOR_ELT(out, 1, compname);
        SET_VECTOR_ELT(out, 2, compout);

        PROTECT(listv = Rf_allocVector(STRSXP, 3));
        SET_STRING_ELT(listv, 0, Rf_mkChar("id"));
        SET_STRING_ELT(listv, 1, Rf_mkChar("name"));
        SET_STRING_ELT(listv, 2, Rf_mkChar("outside"));

        Rf_setAttrib(out, R_NamesSymbol, listv);

        /* make the list to be an instance of 'compartments_list' */
        PROTECT(class = Rf_allocVector(STRSXP, 1));
        SET_STRING_ELT(class, 0, Rf_mkChar("compartments_list"));
        Rf_classgets(out, class);

        UNPROTECT(6);
    }
    else {
        out = R_NilValue;
    }

    return out;
}


/* -------------------------------------------------------------------------- */
/* get list of species (metabolites) */
SEXP getSBMLSpeciesList(SEXP sbmlmod) {

    SEXP out       = R_NilValue;
    SEXP class     = R_NilValue;
    SEXP listv     = R_NilValue;
    SEXP metid     = R_NilValue;
    SEXP metname   = R_NilValue;
    SEXP metcomp   = R_NilValue;
    SEXP metcharge = R_NilValue;
    SEXP metbndcnd = R_NilValue;

    unsigned int nsp, i;

    /* ListOf_t *spl; */
    Species_t *splel;

    checkModel(sbmlmod);

    /* spl = Model_getListOfSpecies(R_ExternalPtrAddr(sbmlmod)); */
    nsp = Model_getNumSpecies(R_ExternalPtrAddr(sbmlmod));
    
    if (nsp > 0) {
        PROTECT(metid     = Rf_allocVector(STRSXP, nsp));
        PROTECT(metname   = Rf_allocVector(STRSXP, nsp));
        PROTECT(metcomp   = Rf_allocVector(STRSXP, nsp));
        PROTECT(metcharge = Rf_allocVector(INTSXP, nsp));
        PROTECT(metbndcnd = Rf_allocVector(LGLSXP, nsp));

        for (i = 0; i < nsp; i++) {
            /* splel = (Species_t *) ListOf_get(spl, i); */
            splel = Model_getSpecies(R_ExternalPtrAddr(sbmlmod), i);
            /* id */
            if (Species_isSetId(splel)) {
                SET_STRING_ELT(metid, i, Rf_mkChar(Species_getId(splel)));
            }
            else {
                SET_STRING_ELT(metid, i, Rf_mkChar("no_id"));
            }
            /* name */
            if (Species_isSetName(splel)) {
                SET_STRING_ELT(metname, i, Rf_mkChar(Species_getName(splel)));
            }
            else {
                SET_STRING_ELT(metname, i, Rf_mkChar(""));
            }
            /* compartment */
            if (Species_isSetCompartment(splel)) {
                SET_STRING_ELT(metcomp, i, Rf_mkChar(Species_getCompartment(splel)));
            }
            else {
                SET_STRING_ELT(metcomp, i, Rf_mkChar(""));
            }
            /* charge */
            if (Species_isSetCharge(splel)) {
                INTEGER(metcharge)[i] = Species_getCharge(splel);
            }
            else {
                INTEGER(metcharge)[i] = 0;
            }
            /* boundary condition */
            if (Species_isSetBoundaryCondition(splel)) {
                LOGICAL(metbndcnd)[i] = Species_getBoundaryCondition(splel);
            }
            else {
                LOGICAL(metbndcnd)[i] = 0;
            }
        }
    
        PROTECT(out = Rf_allocVector(VECSXP, 5));
        SET_VECTOR_ELT(out, 0, metid);
        SET_VECTOR_ELT(out, 1, metname);
        SET_VECTOR_ELT(out, 2, metcomp);
        SET_VECTOR_ELT(out, 3, metcharge);
        SET_VECTOR_ELT(out, 4, metbndcnd);

        PROTECT(listv = Rf_allocVector(STRSXP, 5));
        SET_STRING_ELT(listv, 0, Rf_mkChar("id"));
        SET_STRING_ELT(listv, 1, Rf_mkChar("name"));
        SET_STRING_ELT(listv, 2, Rf_mkChar("compartment"));
        SET_STRING_ELT(listv, 3, Rf_mkChar("charge"));
        SET_STRING_ELT(listv, 4, Rf_mkChar("boundaryCondition"));

        Rf_setAttrib(out, R_NamesSymbol, listv);

        /* make the list to be an instance of 'species_list' */
        PROTECT(class = Rf_allocVector(STRSXP, 1));
        SET_STRING_ELT(class, 0, Rf_mkChar("species_list"));
        Rf_classgets(out, class);

        UNPROTECT(8);
    }
    else {
        out = R_NilValue;
    }
    
    return out;
}


/* -------------------------------------------------------------------------- */
/* get list of reactions */
SEXP getSBMLReactionsList(SEXP sbmlmod) {

    SEXP out         = R_NilValue;
    SEXP class       = R_NilValue;
    SEXP listv       = R_NilValue;
    SEXP reactid     = R_NilValue;
    SEXP reactname   = R_NilValue;
    SEXP reactrev    = R_NilValue;
    SEXP reactnotes  = R_NilValue;
    SEXP reactannot  = R_NilValue;
    SEXP reactreact  = R_NilValue;
    SEXP reactprod   = R_NilValue;
    SEXP reactkl     = R_NilValue;

    SEXP parml       = R_NilValue;
    SEXP parmn       = R_NilValue;
    SEXP parmid      = R_NilValue;
    SEXP parmval     = R_NilValue;
    SEXP parmunit    = R_NilValue;

    unsigned int nre, i, j, nreactant, nproduct, nparm;

    /* ListOf_t *rel; */
    Reaction_t *relel;
    KineticLaw_t *kl;
    Parameter_t *parm;

    checkModel(sbmlmod);

    /* rel = Model_getListOfReactions(R_ExternalPtrAddr(sbmlmod)); */
    nre = Model_getNumReactions(R_ExternalPtrAddr(sbmlmod));

    if (nre > 0) {
        PROTECT(reactid     = Rf_allocVector(STRSXP, nre));
        PROTECT(reactname   = Rf_allocVector(STRSXP, nre));
        PROTECT(reactrev    = Rf_allocVector(LGLSXP, nre));
        PROTECT(reactnotes  = Rf_allocVector(STRSXP, nre));
        PROTECT(reactannot  = Rf_allocVector(STRSXP, nre));
        PROTECT(reactreact  = Rf_allocVector(VECSXP, nre));
        PROTECT(reactprod   = Rf_allocVector(VECSXP, nre));
        PROTECT(reactkl     = Rf_allocVector(VECSXP, nre));

        for (i = 0; i < nre; i++) {
            /* relel = (Reaction_t *) ListOf_get(rel, i); */
            relel = Model_getReaction(R_ExternalPtrAddr(sbmlmod), i);

            nreactant = Reaction_getNumReactants(relel);
            nproduct  = Reaction_getNumProducts(relel);

            /* id */
            if (Reaction_isSetId(relel)) {
                SET_STRING_ELT(reactid, i, Rf_mkChar(Reaction_getId(relel)));
            }
            else {
                SET_STRING_ELT(reactid, i, Rf_mkChar("no_id"));
            }
            /* name */
            if (Reaction_isSetName(relel)) {
                SET_STRING_ELT(reactname, i, Rf_mkChar(Reaction_getName(relel)));
            }
            else {
                SET_STRING_ELT(reactname, i, Rf_mkChar(""));
            }
            /* reversible */
            if (Reaction_isSetReversible(relel)) {
                LOGICAL(reactrev)[i] = Reaction_getReversible(relel);
            }
            else {
                LOGICAL(reactrev)[i] = 0;
            }
            /* notes */
            if (SBase_isSetNotes((SBase_t *) relel)) {
                SET_STRING_ELT(reactnotes, i, Rf_mkChar(SBase_getNotesString((SBase_t *) relel)));
            }
            else {
                SET_STRING_ELT(reactnotes, i, Rf_mkChar(""));
            }
            /* annotation */
            if (SBase_isSetAnnotation((SBase_t *) relel)) {
                SET_STRING_ELT(reactannot, i, Rf_mkChar(SBase_getAnnotationString((SBase_t *) relel)));
            }
            else {
                SET_STRING_ELT(reactannot, i, Rf_mkChar(""));
            }

            /* reactants */
            SET_VECTOR_ELT(reactreact, i, getSpeciesReference(relel, nreactant, 0));

            /* products */
            SET_VECTOR_ELT(reactprod, i, getSpeciesReference(relel, nproduct, 1));

            /* kineticLaw */
            if (Reaction_isSetKineticLaw(relel)) {
                kl = Reaction_getKineticLaw(relel);
                nparm = KineticLaw_getNumParameters(kl);

                if (nparm > 0) {
                    PROTECT(parmid   = Rf_allocVector(STRSXP, nparm));
                    PROTECT(parmval  = Rf_allocVector(REALSXP, nparm));
                    PROTECT(parmunit = Rf_allocVector(STRSXP, nparm));

                    for (j = 0; j < nparm; j++) {
                        parm = KineticLaw_getParameter(kl, j);

                        /* id */
                        if (Parameter_isSetId(parm)) {
                            SET_STRING_ELT(parmid, j, Rf_mkChar(Parameter_getId(parm)));
                        }
                        else {
                            SET_STRING_ELT(parmid, j, Rf_mkChar("no_id"));
                        }
                        /* value */
                        if (Parameter_isSetValue(parm)) {
                            REAL(parmval)[j] = Parameter_getValue(parm);
                        }
                        else {
                            REAL(parmval)[j] = 0;
                        }
                        /* units */
                        if (Parameter_isSetUnits(parm)) {
                            SET_STRING_ELT(parmunit, j, Rf_mkChar(Parameter_getUnits(parm)));
                        }
                        else {
                            SET_STRING_ELT(parmunit, j, Rf_mkChar(""));
                        }
                    }
            
                    PROTECT(parml = Rf_allocVector(VECSXP, 3));
                    SET_VECTOR_ELT(parml, 0, parmid);
                    SET_VECTOR_ELT(parml, 1, parmval);
                    SET_VECTOR_ELT(parml, 2, parmunit);

                    PROTECT(parmn = Rf_allocVector(STRSXP, 3));
                    SET_STRING_ELT(parmn, 0, Rf_mkChar("id"));
                    SET_STRING_ELT(parmn, 1, Rf_mkChar("value"));
                    SET_STRING_ELT(parmn, 2, Rf_mkChar("units"));
                    Rf_setAttrib(parml, R_NamesSymbol, parmn);

                    UNPROTECT(5);
                }
                else {
                    parml = R_NilValue;
                }
                SET_VECTOR_ELT(reactkl, i, parml);
            }
            else {
                SET_VECTOR_ELT(reactkl, i, R_NilValue);
            }

        }
    
        PROTECT(out = Rf_allocVector(VECSXP, 8));
        SET_VECTOR_ELT(out, 0, reactid);
        SET_VECTOR_ELT(out, 1, reactname);
        SET_VECTOR_ELT(out, 2, reactrev);
        SET_VECTOR_ELT(out, 3, reactnotes);
        SET_VECTOR_ELT(out, 4, reactannot);
        SET_VECTOR_ELT(out, 5, reactreact);
        SET_VECTOR_ELT(out, 6, reactprod);
        SET_VECTOR_ELT(out, 7, reactkl);

        PROTECT(listv = Rf_allocVector(STRSXP, 8));
        SET_STRING_ELT(listv, 0, Rf_mkChar("id"));
        SET_STRING_ELT(listv, 1, Rf_mkChar("name"));
        SET_STRING_ELT(listv, 2, Rf_mkChar("reversible"));
        SET_STRING_ELT(listv, 3, Rf_mkChar("notes"));
        SET_STRING_ELT(listv, 4, Rf_mkChar("annotation"));
        SET_STRING_ELT(listv, 5, Rf_mkChar("reactants"));
        SET_STRING_ELT(listv, 6, Rf_mkChar("products"));
        SET_STRING_ELT(listv, 7, Rf_mkChar("kinetic_law"));

        Rf_setAttrib(out, R_NamesSymbol, listv);

        /* make the list to be an instance of 'reactions_list' */
        PROTECT(class = Rf_allocVector(STRSXP, 1));
        SET_STRING_ELT(class, 0, Rf_mkChar("reactions_list"));
        Rf_classgets(out, class);

        UNPROTECT(11);
    }
    else {
        out = R_NilValue;
    }

    return out;
}
