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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif /* HAVE_CONFIG_H */

#include "sybilSBML.h"

//new includes @Ardalan Habil
#include <stdlib.h>
#include <string.h>
#include <sbml/xml/XMLTriple.h>
#include <sbml/annotation/CVTerm.h>
#include <sbml/annotation/RDFAnnotationParser.h>
#include <sbml/annotation/ModelHistory.h>
#include <sbml/math/ASTNode.h>
#include <math.h>

#ifdef HAVE_FBC_PLUGIN
/* FBCv1includes  */

#include <sbml/extension/SBMLExtensionRegister.h>
#include <sbml/extension/SBMLDocumentPlugin.h>

#include <sbml/packages/fbc/common/fbcfwd.h>

/* FBC PLUGINS*/
#include <sbml/packages/fbc/extension/FbcSBMLDocumentPlugin.h>
#include <sbml/packages/fbc/extension/FbcModelPlugin.h>
#include <sbml/packages/fbc/extension/FbcReactionPlugin.h>
#include <sbml/packages/fbc/extension/FbcSpeciesPlugin.h>

#include <sbml/packages/fbc/sbml/FluxBound.h>
#include <sbml/packages/fbc/sbml/Objective.h>
#include <sbml/packages/fbc/sbml/FluxObjective.h>
#include <sbml/packages/fbc/sbml/GeneProduct.h>
#include <sbml/packages/fbc/sbml/GeneProductRef.h>
#include <sbml/packages/fbc/sbml/GeneProductAssociation.h>
#include <sbml/packages/fbc/sbml/FbcAssociation.h>
#include <sbml/packages/fbc/sbml/FbcAnd.h>
#include <sbml/packages/fbc/sbml/FbcOr.h>
#endif

#ifdef HAVE_GROUPS_PLUGIN
/*groups plugin*/
#include <sbml/packages/groups/common/GroupsExtensionTypes.h>
#include <sbml/packages/groups/extension/GroupsSBMLDocumentPlugin.h>
#include <sbml/packages/groups/extension/GroupsExtension.h>
#include <sbml/packages/groups/extension/GroupsModelPlugin.h>

#include <sbml/packages/groups/sbml/Group.h>
#include <sbml/packages/groups/sbml/ListOfGroups.h>
#include <sbml/packages/groups/sbml/ListOfMembers.h>
#include <sbml/packages/groups/sbml/Member.h>
#endif


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
/* Helper functions                                                           */
/* -------------------------------------------------------------------------- */


// append two string
char * append_strings(const char * old, const char * new, const char* delim)
{
  // find the size of the string to allocate
  size_t len = strlen(old) + strlen(new) + strlen(delim) + 1;
  
  // allocate a pointer to the new string
  char *out = malloc(len);
  
  // concat both strings and return
  sprintf(out, "%s%s%s", old, delim , new);
  
  return out;
}



// Parse Anntatation to modelorg
const char* parseAnnotationTomorg(XMLNode_t* xml)
{
  const XMLNode_t* rdf =NULL;
  const XMLNode_t* desc = NULL;
  rdf = XMLNode_getChildForName(xml,"RDF");
  if(rdf !=NULL) desc= XMLNode_getChildForName(rdf,"Description");
  const char* annoString = "";
  int firstelement=1;
  if (desc!=NULL)
  {
    
    int num=0;
    for(num=0;num<XMLNode_getNumChildren(desc);num++)
    {
      const char* pref= XMLNode_getPrefix(XMLNode_getChild(desc,num));
      
      if( strcmp(pref,"bqbiol")==0 || strcmp(pref,"bqmodel")==0 )
      { 
        CVTerm_t* cv= CVTerm_createFromNode(XMLNode_getChild(desc,num));
        if( CVTerm_getNumResources(cv)>0 )
        { 
          int numR=0;
          if(strcmp(pref,"bqbiol")==0)
          { 
            const char* bioQual = append_strings("bqbiol",BiolQualifierType_toString( CVTerm_getBiologicalQualifierType(cv)),"_");
            if(firstelement==0) annoString = append_strings(annoString,bioQual,";");
            else
            {
              annoString = append_strings(annoString,bioQual,"");
              firstelement=0;
            }
            
          }
          if(strcmp(pref,"bqmodel")==0)
          { 
            const char* modQual = append_strings("bqmodel",ModelQualifierType_toString( CVTerm_getModelQualifierType(cv)),"_");
            if(firstelement==0)annoString = append_strings(annoString,modQual,";");
            
            else
            {
              annoString = append_strings(annoString,modQual,"");
              firstelement=0;
            }
          }
          
          for(numR=0;numR<CVTerm_getNumResources(cv);numR++)
          {
            // sprintf(annoString+strlen(annoString),"__%s", CVTerm_getResourceURI(cv,numR));
            annoString = append_strings(annoString,CVTerm_getResourceURI(cv,numR),";");  
          }
          
        }
      }
      
      
    }  
    return annoString;
  }
  else
  {
    return "";
  }  
  
  
  return "";
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

/* get sbml document  FBC version */
SEXP getSBMLFbcversion(SEXP sbmldoc) {
  SEXP out = R_NilValue;
  
#ifdef HAVE_FBC_PLUGIN
  unsigned int version;
  
  checkDocument(sbmldoc);
  
  SBasePlugin_t * modelPlug= NULL;
  modelPlug = SBase_getPlugin((SBase_t *)(R_ExternalPtrAddr(sbmldoc)), "fbc");
  if( modelPlug != NULL) 
  { 
    if(strcmp("fbc",SBasePlugin_getPackageName(modelPlug) ) ==0)
      version = SBasePlugin_getPackageVersion(modelPlug);
    else version=0;
  } else version=0;  
  
  out = Rf_ScalarInteger(version);
#else
  out = Rf_ScalarInteger(0);
#endif
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
/* get model notes */
SEXP getSBMLmodNotes(SEXP sbmlmod) {
  
  SEXP out = R_NilValue;
  const char *mnotes;
  
  checkModel(sbmlmod);
  
  if (SBase_isSetNotes((SBase_t *) R_ExternalPtrAddr(sbmlmod))) {
  mnotes = SBase_getNotesString((SBase_t *)  R_ExternalPtrAddr(sbmlmod));
  }
  else {
    mnotes = "";
  }
  
  out = Rf_mkString(mnotes);
  
  return out;
}

/* -------------------------------------------------------------------------- */
/* get model name */
SEXP getSBMLmodAnnotation(SEXP sbmlmod) {
  
  SEXP out = R_NilValue;
  const char *manno;
  
  checkModel(sbmlmod);
  
  if (SBase_isSetAnnotation((SBase_t *)  R_ExternalPtrAddr(sbmlmod))) {
    XMLNode_t* xml = SBase_getAnnotation((SBase_t *)  R_ExternalPtrAddr(sbmlmod));
    manno = parseAnnotationTomorg(xml);
  }
  else {
    manno = "";
  }
  
  out = Rf_mkString(manno);
  
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
  
  SEXP out        = R_NilValue;
  SEXP class      = R_NilValue;
  SEXP listv      = R_NilValue;
  SEXP compid     = R_NilValue;
  SEXP compname   = R_NilValue;
  SEXP compannot  = R_NilValue;
  SEXP compnotes  = R_NilValue;
  SEXP compout    = R_NilValue;
  
  unsigned int nc, i;
  
  /* ListOf_t *cl; */
  Compartment_t *clel;
  
  checkModel(sbmlmod);
  
  /* cl = Model_getListOfCompartments(R_ExternalPtrAddr(sbmlmod)); */
  nc = Model_getNumCompartments(R_ExternalPtrAddr(sbmlmod));
  
  if (nc > 0) {
    PROTECT(compid     = Rf_allocVector(STRSXP, nc));
    PROTECT(compname   = Rf_allocVector(STRSXP, nc));
    PROTECT(compannot  = Rf_allocVector(STRSXP, nc));
    PROTECT(compnotes  = Rf_allocVector(STRSXP, nc));
    
    // Counter Variables	
    int annocount=0;
    int notescount=0;
    
    PROTECT(compout   = Rf_allocVector(STRSXP, nc));
    
    for (i = 0; i < nc; i++) {
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
      
      /* NEW INFORMATIONS*/	
      /* notes */
      if (SBase_isSetNotes((SBase_t *) clel)) {
        SET_STRING_ELT(compnotes, i, Rf_mkChar(SBase_getNotesString((SBase_t *) clel)));
        notescount=notescount+1;
      }
      else {
        SET_STRING_ELT(compnotes, i, Rf_mkChar(""));
      }
      
      /* annotation */
      if (SBase_isSetAnnotation((SBase_t *) clel)) {
        XMLNode_t* xml = SBase_getAnnotation((SBase_t *) clel);
        SET_STRING_ELT(compannot, i, Rf_mkChar(parseAnnotationTomorg(xml)));
        annocount=annocount+1;
      }
      else {
        SET_STRING_ELT(compannot, i, Rf_mkChar(""));
      }
      
      
    }
    
    // NULL if empty 
    if (annocount==0)   compannot = R_NilValue;
    if (notescount==0) compnotes  = R_NilValue;
    
    PROTECT(out = Rf_allocVector(VECSXP, 5));
    SET_VECTOR_ELT(out, 0, compid);
    SET_VECTOR_ELT(out, 1, compname);
    SET_VECTOR_ELT(out, 2, compout);
    SET_VECTOR_ELT(out, 3, compannot);
    SET_VECTOR_ELT(out, 4, compnotes);
    
    
    PROTECT(listv = Rf_allocVector(STRSXP, 5));
    SET_STRING_ELT(listv, 0, Rf_mkChar("id"));
    SET_STRING_ELT(listv, 1, Rf_mkChar("name"));
    SET_STRING_ELT(listv, 2, Rf_mkChar("outside"));
    SET_STRING_ELT(listv, 3, Rf_mkChar("annotation"));
    SET_STRING_ELT(listv, 4, Rf_mkChar("notes"));
    
    
    
    Rf_setAttrib(out, R_NamesSymbol, listv);
    
    /* make the list to be an instance of 'compartments_list' */
    PROTECT(class = Rf_allocVector(STRSXP, 1));
    SET_STRING_ELT(class, 0, Rf_mkChar("compartments_list"));
    Rf_classgets(out, class);
    
    UNPROTECT(8);
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
  SEXP metchemic = R_NilValue;
  SEXP metbndcnd = R_NilValue;
  SEXP metannot  = R_NilValue;
  SEXP metnotes  = R_NilValue;
  
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
    PROTECT(metchemic = Rf_allocVector(STRSXP, nsp));
    PROTECT(metbndcnd = Rf_allocVector(LGLSXP, nsp));
    PROTECT(metannot  = Rf_allocVector(STRSXP, nsp));
    PROTECT(metnotes  = Rf_allocVector(STRSXP, nsp));
    
    int chargecount = 0;
    int chcount=0;
    int notescount=0;
    int annotcount=0;
    
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
      
      
      
      /* notes */
      if (SBase_isSetNotes((SBase_t *) splel)) {
        SET_STRING_ELT(metnotes, i, Rf_mkChar(SBase_getNotesString((SBase_t *) splel)));
        notescount=notescount+1;
        
      }
      else {
        SET_STRING_ELT(metnotes, i, Rf_mkChar(""));
      }
      
      /* annotation */
      if (SBase_isSetAnnotation((SBase_t *) splel)) {
        //SET_STRING_ELT(metannot, i, Rf_mkChar(SBase_getAnnotationString((SBase_t *) splel)));
        XMLNode_t* xml = SBase_getAnnotation((SBase_t *) splel);
        annotcount=annotcount+1;
        SET_STRING_ELT(metannot, i, Rf_mkChar(parseAnnotationTomorg(xml)));
      }
      else {
        SET_STRING_ELT(metannot, i, Rf_mkChar(""));
      }


#ifdef HAVE_FBC_PLUGIN
      /* FBC PLUGIN @ Ardalan */
      SBasePlugin_t *SpeciesPlug = SBase_getPlugin((SBase_t *)(splel), "fbc");
      
      /* get charge and chemical formula from FBC plugin: */
      if (SpeciesPlug != NULL) {
          /* FBCcharge */
          if (FbcSpeciesPlugin_isSetCharge(SpeciesPlug)) {
              INTEGER(metcharge)[i] = FbcSpeciesPlugin_getCharge(SpeciesPlug);
              chargecount = chargecount + 1;
          }
          
          /* FBC chemicalFormula */
          if (FbcSpeciesPlugin_isSetChemicalFormula(SpeciesPlug)) {
              SET_STRING_ELT(metchemic, i, Rf_mkChar(FbcSpeciesPlugin_getChemicalFormula(SpeciesPlug)));
              chcount=chcount+1;
          }
          else {
              SET_STRING_ELT(metchemic, i, Rf_mkChar(""));
          }
      }
#endif
    }
    
    // NULL if empty
    if (chargecount == 0) metcharge = R_NilValue;
    if (chcount==0) metchemic = R_NilValue;
    if (notescount==0) metnotes  = R_NilValue;
    if (annotcount==0)  metannot  = R_NilValue;
    
    PROTECT(out = Rf_allocVector(VECSXP, 8));
    SET_VECTOR_ELT(out, 0, metid);
    SET_VECTOR_ELT(out, 1, metname);
    SET_VECTOR_ELT(out, 2, metcomp);
    SET_VECTOR_ELT(out, 3, metcharge);
    SET_VECTOR_ELT(out, 4, metchemic);
    SET_VECTOR_ELT(out, 5, metbndcnd);
    SET_VECTOR_ELT(out, 6, metannot);
    SET_VECTOR_ELT(out, 7, metnotes);
    
    
    PROTECT(listv = Rf_allocVector(STRSXP, 8));
    SET_STRING_ELT(listv, 0, Rf_mkChar("id"));
    SET_STRING_ELT(listv, 1, Rf_mkChar("name"));
    SET_STRING_ELT(listv, 2, Rf_mkChar("compartment"));
    SET_STRING_ELT(listv, 3, Rf_mkChar("charge"));
    SET_STRING_ELT(listv, 4, Rf_mkChar("chemicalFormula"));
    SET_STRING_ELT(listv, 5, Rf_mkChar("boundaryCondition"));
    SET_STRING_ELT(listv, 6, Rf_mkChar("annotation"));
    SET_STRING_ELT(listv, 7, Rf_mkChar("notes"));
    
    
    Rf_setAttrib(out, R_NamesSymbol, listv);
    
    /* make the list to be an instance of 'species_list' */
    PROTECT(class = Rf_allocVector(STRSXP, 1));
    SET_STRING_ELT(class, 0, Rf_mkChar("species_list"));
    Rf_classgets(out, class);
    
    UNPROTECT(11);
  }
  else {
    out = R_NilValue;
  }
  
  return out;
}


SEXP getSBMLGroupsList(SEXP sbmlmod) {
#ifdef HAVE_GROUPS_PLUGIN
	GroupsModelPlugin_t  * modelPlug = NULL;
	modelPlug = (GroupsModelPlugin_t  *) SBase_getPlugin((SBase_t *)(R_ExternalPtrAddr(sbmlmod)), "groups");
	
	if(modelPlug != NULL){
		int n = GroupsModelPlugin_getNumGroups(modelPlug);
		SEXP rgroups = PROTECT(Rf_allocVector(VECSXP, n));
		SEXP groupnames = PROTECT(Rf_allocVector(STRSXP, n));
		for(int i=0; i<n; i++){
			Group_t* group = GroupsModelPlugin_getGroup(modelPlug, i);
			if(Group_isSetName(group) == 1){ // skip group if no name is set
				SET_STRING_ELT(groupnames, i, Rf_mkChar(Group_getName(group)));
				
				int m = Group_getNumMembers(group);
				SEXP rmembers = PROTECT(Rf_allocVector(STRSXP, m));
				for(int j=0; j < m; j++){
					Member_t * member = Group_getMember(group, j);
					char * memref = Member_getIdRef(member);
					SET_STRING_ELT(rmembers, j, Rf_mkChar(memref));
				}
				SET_VECTOR_ELT(rgroups, i, rmembers);
				UNPROTECT(1);
			}else{
				SET_VECTOR_ELT(rgroups, i, R_NilValue);
			}
		}
		Rf_namesgets(rgroups, groupnames);
		UNPROTECT(2);
		return rgroups;
	}else{
		return R_NilValue;
	}
#endif
	return R_NilValue;
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
  
  SEXP fbclb       = R_NilValue; //lowerbnd
  SEXP fbcup       = R_NilValue; //upperbnd
  SEXP fbcgene     = R_NilValue; //fbc gene Rules
  SEXP fbcobj      = R_NilValue; // fbc objective
  
  unsigned int nre, i, j, nreactant, nproduct, nparm;
  
  /* ListOf_t *rel; */
  Reaction_t *relel;
  KineticLaw_t *kl;
  Parameter_t *parm;
  
  checkModel(sbmlmod);
  
  /* rel = Model_getListOfReactions(R_ExternalPtrAddr(sbmlmod)); */
  /* get number of ractions: */
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
    
    PROTECT(fbclb       = Rf_allocVector(REALSXP, nre));
    PROTECT(fbcup       = Rf_allocVector(REALSXP, nre));
    PROTECT(fbcgene     = Rf_allocVector(STRSXP , nre));
    PROTECT(fbcobj      = Rf_allocVector(REALSXP, nre));
    
    
    int lbcount=0;
    int upcount=0;
    int genecount=0;
    int objcount=0;
    int annocount=0;
    int notescount=0;
 
    
#ifdef HAVE_FBC_PLUGIN
    /* Help Var for Fbc Objective*/
    double Objcoeff = 0;
    const char* Objreaction = NULL;
    char* objActiv = NULL;
    int fbcversion = 0;
        
    SBasePlugin_t * modelPlug= NULL;
    
    /* FBC OBJECTIV @Ardalan*/
    Objective_t * objective = NULL;
    FluxObjective_t * fluxObjective = NULL;
    modelPlug = SBase_getPlugin((SBase_t *)(R_ExternalPtrAddr(sbmlmod)), "fbc");
    
    // Read the Objectives when FBCPlugin for the model exists
    // (Save only active objective)
    if( modelPlug != NULL)
    { 
      
      objActiv = FbcModelPlugin_getActiveObjectiveId(modelPlug);
      if(strcmp(objActiv,"") !=0)
      {
        for(int ob = 0; ob < FbcModelPlugin_getNumObjectives(modelPlug); ob++)
        {
          objective = FbcModelPlugin_getObjective(modelPlug,ob);
          //printf("ObjectiveID: %s \n",  Objective_getId(objective) );
          if(strcmp(objActiv,Objective_getId(objective))==0)
          { // TODO mehrer FLUXOBJECTIVE; MAXimierung Minimirung? 
            
            //  int fob=0;
            //  for(fob; ob<FbcModelPlugin_getNumObjectives(modelPlug);fob++ )
            //  {  
            fluxObjective = Objective_getFluxObjective(objective,0);
            Objreaction =  FluxObjective_getReaction(fluxObjective) ;
            Objcoeff =  FluxObjective_getCoefficient(fluxObjective);
            
            //printf("ReactionObjectiveID: %s \n",  Objreaction);
            //printf("Coefficient: %f \n",  Objcoeff);
            //  }
          }  
          
        }  
        
      }
      
      
      /* is FBC 1 */
      
      if(strcmp("fbc",SBasePlugin_getPackageName(modelPlug) ) ==0)
        fbcversion = SBasePlugin_getPackageVersion(modelPlug);
    } 
#endif 
    
    
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
        notescount=notescount+1;
      }
      else {
        SET_STRING_ELT(reactnotes, i, Rf_mkChar(""));
      }
      /* annotation */
      if (SBase_isSetAnnotation((SBase_t *) relel)) {
        //SET_STRING_ELT(reactannot, i, Rf_mkChar(SBase_getAnnotationString((SBase_t *) relel)));
        XMLNode_t* xml = SBase_getAnnotation((SBase_t *) relel);
        SET_STRING_ELT(reactannot, i, Rf_mkChar(parseAnnotationTomorg(xml)));
        annocount=annocount+1;
      }
      else {
        SET_STRING_ELT(reactannot, i, Rf_mkChar(""));
      }
      
      
#ifdef HAVE_FBC_PLUGIN
      /* FBC LEVEL 2 @Ardalan Habil*/
      
      /* ReactionPLugin for FBC 2 */
      SBasePlugin_t *reactionPlug = SBase_getPlugin((SBase_t *)(relel), "fbc");
      
      if (reactionPlug != NULL) {
          /* LOWERFLUXBOUND */
          if (FbcReactionPlugin_isSetLowerFluxBound(reactionPlug)) 
          {
              parm = Model_getParameterById(R_ExternalPtrAddr (sbmlmod) ,FbcReactionPlugin_getLowerFluxBound(reactionPlug));
              //printf("LowerFLUXBOUND: %f \n",  Parameter_getValue(parm));
              REAL(fbclb)[i] = Parameter_getValue(parm);
              lbcount=lbcount+1;
          }
          else{
              REAL(fbclb)[i] = 0;
          }      
          
          /* UPPERFLUXBOUND*/
          if (FbcReactionPlugin_isSetUpperFluxBound(reactionPlug)) 
          {
              parm = Model_getParameterById(R_ExternalPtrAddr (sbmlmod) ,FbcReactionPlugin_getUpperFluxBound(reactionPlug));
              //printf("UPPERFLUXBOUND: %f \n",  Parameter_getValue(parm));
              REAL(fbcup)[i] = Parameter_getValue(parm);
              upcount=upcount+1;
          }
          else{
              REAL(fbcup)[i] = 0;
          }
          
          /*FBC 1 read */
          if (fbcversion==1)
          {
              /* Storing FBC1Bounds */
              double fbc1lb=0;
              double fbc1up=0;
              
              for(int fluxb = 0; fluxb < FbcModelPlugin_getNumFluxBounds(modelPlug); fluxb++)
              {
                  FluxBound_t * currentFlux = FbcModelPlugin_getFluxBound(modelPlug,fluxb);
                  
                  const char * currentFluxType ;
                  const char * currentFluxReaction; 
                  
                  if (FluxBound_isSetReaction(currentFlux))  currentFluxReaction = FluxBound_getReaction(currentFlux);
                  else continue;
                  
                  if(strcmp(currentFluxReaction , Reaction_getId(relel) ) !=0) continue;
                  
                  
                  if (FluxBound_isSetOperation(currentFlux)) currentFluxType = FluxBound_getOperation(currentFlux);
                  else continue;  
                  
                  
                  if(strcmp("greaterEqual" , currentFluxType ) ==0)
                  {
                      lbcount=lbcount+1;
                      if (FluxBound_isSetValue(currentFlux))  fbc1lb = FluxBound_getValue(currentFlux);
                      else continue;
                  }  
                  
                  else if(strcmp("lessEqual" , currentFluxType ) ==0)
                  { 
                      upcount=upcount+1;
                      if (FluxBound_isSetValue(currentFlux))  fbc1up = FluxBound_getValue(currentFlux);
                      else continue;
                  }  
                  
                  else if(strcmp("equal" , currentFluxType ) ==0)  
                  {
                      if (FluxBound_isSetValue(currentFlux))
                      {
                          lbcount=lbcount+1;
                          upcount=upcount+1;
                          fbc1lb = FluxBound_getValue(currentFlux); 
                          fbc1up = FluxBound_getValue(currentFlux);
                      }
                      else continue;
                  }
                  
              }  
              
              /* FBC 1 save Bounds */
              REAL(fbclb)[i] = fbc1lb;
              REAL(fbcup)[i] = fbc1up;
              
          }
          
          
          /* FBC GENE */
          if( FbcReactionPlugin_isSetGeneProductAssociation(reactionPlug) ) {
              GeneProductAssociation_t* gpa = FbcReactionPlugin_getGeneProductAssociation(reactionPlug);
              FbcAssociation_t* asso= (FbcAssociation_t*) GeneProductAssociation_getAssociation(gpa);
              SET_STRING_ELT(fbcgene, i, Rf_mkChar(FbcAssociation_toInfix(asso)));
              //printf("Gene: %s \n",   FbcAssociation_toInfix(asso));
              genecount=genecount+1;
          }
          else {
              SET_STRING_ELT(fbcgene, i, Rf_mkChar(""));
          }
          
          /* FBC OBJECTIVES*/
          if (Objreaction != NULL && strcmp(Objreaction , Reaction_getId(relel) )==0)     
          {
              
              REAL(fbcobj)[i] = Objcoeff;
              objcount=objcount+1;
          }
          else{
              REAL(fbcobj)[i] = 0;
          }      
      } else {
          REAL(fbclb)[i] = 0;
          REAL(fbcup)[i] = 0;
          SET_STRING_ELT(fbcgene, i, Rf_mkChar(""));
          REAL(fbcobj)[i] = 0;
      }
#else
        REAL(fbclb)[i] = 0;
        REAL(fbcup)[i] = 0;
        SET_STRING_ELT(fbcgene, i, Rf_mkChar(""));
        REAL(fbcobj)[i] = 0;
#endif
      
      
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
    
    // NULL if empty
    if(lbcount==0)   fbclb     = R_NilValue;
    if(upcount==0)   fbcup     = R_NilValue;
    if(genecount==0) fbcgene   = R_NilValue;
    if(objcount==0)  fbcobj    = R_NilValue;
    if(annocount==0) reactannot= R_NilValue;
    if(notescount==0) reactnotes= R_NilValue;
    
    
    PROTECT(out = Rf_allocVector(VECSXP, 12));
    SET_VECTOR_ELT(out, 0, reactid);
    SET_VECTOR_ELT(out, 1, reactname);
    SET_VECTOR_ELT(out, 2, reactrev);
    SET_VECTOR_ELT(out, 3, reactnotes);
    SET_VECTOR_ELT(out, 4, reactannot);
    SET_VECTOR_ELT(out, 5, reactreact);
    SET_VECTOR_ELT(out, 6, reactprod);
    SET_VECTOR_ELT(out, 7, reactkl);
    SET_VECTOR_ELT(out, 8, fbclb);
    SET_VECTOR_ELT(out, 9, fbcup);
    SET_VECTOR_ELT(out, 10, fbcgene);
    SET_VECTOR_ELT(out, 11, fbcobj);
    
    
    PROTECT(listv = Rf_allocVector(STRSXP, 12));
    SET_STRING_ELT(listv, 0, Rf_mkChar("id"));
    SET_STRING_ELT(listv, 1, Rf_mkChar("name"));
    SET_STRING_ELT(listv, 2, Rf_mkChar("reversible"));
    SET_STRING_ELT(listv, 3, Rf_mkChar("notes"));
    SET_STRING_ELT(listv, 4, Rf_mkChar("annotation"));
    SET_STRING_ELT(listv, 5, Rf_mkChar("reactants"));
    SET_STRING_ELT(listv, 6, Rf_mkChar("products"));
    SET_STRING_ELT(listv, 7, Rf_mkChar("kinetic_law"));
    SET_STRING_ELT(listv, 8, Rf_mkChar("fbc_lowbnd"));
    SET_STRING_ELT(listv, 9, Rf_mkChar("fbc_uppbnd"));
    SET_STRING_ELT(listv, 10, Rf_mkChar("fbc_gprRules"));
    SET_STRING_ELT(listv, 11, Rf_mkChar("fbc_Objectives"));
    
    Rf_setAttrib(out, R_NamesSymbol, listv);
    
    /* make the list to be an instance of 'reactions_list' */
    PROTECT(class = Rf_allocVector(STRSXP, 1));
    SET_STRING_ELT(class, 0, Rf_mkChar("reactions_list"));
    Rf_classgets(out, class);
    
    UNPROTECT(15);
  }
  else {
    out = R_NilValue;
  }
  
  return out;
}

/* -------------------------------------------------------------------------- */
/* export SBML*/

void ParseModtoAnno  (SBase_t* comp , char* Mannocopy)
  
{
  
  char delimiter[] = ";";
  char *ptr;
  ptr = strtok(Mannocopy, delimiter);
  char* quali;
  CVTerm_t *cv;
  int first=0;
  
  while(ptr != NULL) {
    
    if(strncmp("bqbiol", ptr, strlen("bqbiol")) == 0)
    {
      if (first != 0){ SBase_addCVTerm((SBase_t*)comp, cv);CVTerm_free(cv);}
      //CVTerm_free(cv);
      quali  = strcpy(&ptr[0],&ptr[7]);
      cv = CVTerm_createWithQualifierType(BIOLOGICAL_QUALIFIER);
      CVTerm_setBiologicalQualifierTypeByString( cv, (const char*) quali); 
      first=1;
    }
    else  if(strncmp("bqmodel", ptr, strlen("bqmodel")) == 0)
    {
      if (first != 0) { SBase_addCVTerm((SBase_t*)comp, cv);CVTerm_free(cv);}
      //CVTerm_free(cv);
      quali  = strcpy(&ptr[0],&ptr[8]);
      cv = CVTerm_createWithQualifierType(MODEL_QUALIFIER);
      CVTerm_setModelQualifierTypeByString( cv, (const char*) quali); 
    }  
    else
    {
      CVTerm_addResource(cv,ptr );
    }  
    
    // naechsten Abschnitt erstellen
    ptr = strtok(NULL, delimiter);
  }
  SBase_addCVTerm((SBase_t*)comp, cv);
  
}



SEXP exportSBML (SEXP version, SEXP level, SEXP FbcLevel, SEXP filename, SEXP sybil_max, SEXP mod_desc, SEXP mod_name, SEXP mod_compart, SEXP met_id, SEXP met_name, SEXP met_comp, SEXP met_form, SEXP met_charge, SEXP react_id, SEXP react_name, SEXP react_rev, SEXP lowbnd, SEXP uppbnd, SEXP obj_coef, SEXP subSys, SEXP subSysGroups, SEXP subSysGroupsNames, SEXP gpr, SEXP SMatrix, SEXP mod_notes, SEXP mod_anno, SEXP com_notes , SEXP com_anno, SEXP met_notes, SEXP met_anno, SEXP met_bnd , SEXP react_notes, SEXP react_anno, SEXP ex_react, SEXP allgenes)
{
#if defined(HAVE_FBC_PLUGIN) && defined(HAVE_GROUPS_PLUGIN)
  //Variables from R
  const char* fname = CHAR(STRING_ELT(filename, 0));
  //unused: const char* model_desc = CHAR(STRING_ELT(mod_desc, 0));
  const char* model_name = CHAR(STRING_ELT(mod_name, 0));
  
  int SBMLlevel = INTEGER(level)[0];
  int SBMLversion = INTEGER(version)[0];
  int SBMLfbcversion = INTEGER(FbcLevel)[0];
  int SBMLgroupsversion = 1;
  double sybilmax = REAL(sybil_max)[0];
  double sybilmin = sybilmax*(-1);
  
  // variable FBC
  //currently unused: XMLNamespaces_t * fbc;
  SBMLNamespaces_t * sbmlns;
  FluxBound_t * fluxBound;
  Objective_t * objective;
  FluxObjective_t * fluxObjective;
  SBMLDocumentPlugin_t * docPlug;
  SBasePlugin_t * modelPlug;
  SBasePlugin_t *reactionPlug;
  SBasePlugin_t *SpeciesPlug ;
  
  FbcAssociation_t* asso;
  
  
  // Variable inital
  SBMLDocument_t* sbmlDoc = NULL;

  Model_t* model;
  XMLNamespaces_t* xmlns;
  
  //currently unused: UnitDefinition_t* unitdef;
  //currently unused: Unit_t* unit;
  
  Species_t *sp;
  Reaction_t* reaction;
  SpeciesReference_t* spr;
  Compartment_t* comp;
  KineticLaw_t* kl;
  Parameter_t* para;
  
  // ASTNode_t* flux;
  ASTNode_t* astMath;  
  //ASTNode_t* ast;
  //char* mathXMLString;
  
  /*---------------------------------------------------------------------------
  *
  * Creates an SBMLDocument object 
  *
  *---------------------------------------------------------------------------*/
  
  if (SBMLlevel == 1 || SBMLlevel == 2)
  { 
    sbmlDoc = SBMLDocument_createWithLevelAndVersion(SBMLlevel,SBMLversion);  
    xmlns = (XMLNamespaces_t*) SBMLDocument_getNamespaces(sbmlDoc);
    XMLNamespaces_add(xmlns, "http://www.w3.org/1999/xhtml", "html");
  }
  else if(SBMLlevel == 3) 
  {
    if(SBMLfbcversion == 0)
    {
      sbmlDoc = SBMLDocument_createWithLevelAndVersion(3,1);  
      xmlns = (XMLNamespaces_t*) SBMLDocument_getNamespaces(sbmlDoc);
      XMLNamespaces_add(xmlns, "http://www.w3.org/1999/xhtml", "html");
      
    } else {
      
      /* FBC LEVEL  */
      /* get fbc registry entry */
      SBMLExtension_t *sbmlext = SBMLExtensionRegistry_getExtension("fbc");
      
      /* create the sbml namespaces object with fbc */
      XMLNamespaces_t * fbc = XMLNamespaces_create();
      XMLNamespaces_add(fbc, SBMLExtension_getURI(sbmlext, 3, 1,  SBMLfbcversion), "fbc");
      
      sbmlns = SBMLNamespaces_create(3, 1);
      SBMLNamespaces_addNamespaces(sbmlns, fbc);
      
      /* add groups extention */
      SBMLExtension_t *sbmlgext = SBMLExtensionRegistry_getExtension("groups");
      XMLNamespaces_t * groups = XMLNamespaces_create();
      XMLNamespaces_add(groups, SBMLExtension_getURI(sbmlgext, SBMLlevel, SBMLversion, SBMLgroupsversion), "groups");
      SBMLNamespaces_addNamespaces(sbmlns, groups);
      
      /* create the document */
      sbmlDoc = SBMLDocument_createWithSBMLNamespaces(sbmlns);
      
      /* XHTML for notes*/
      xmlns = (XMLNamespaces_t*) SBMLDocument_getNamespaces(sbmlDoc);
      XMLNamespaces_add(xmlns, "http://www.w3.org/1999/xhtml", "html");
      
      /* set the fbc reqd attribute to false */
      docPlug = (SBMLDocumentPlugin_t*)(SBase_getPlugin((SBase_t*)(sbmlDoc), "fbc"));
      SBMLDocumentPlugin_setRequired(docPlug, 0);
      
    }
    
  }
  
  
  /*---------------------------------------------------------------------------
  *
  * Creates a Model object inside the SBMLDocument object. 
  *
  *---------------------------------------------------------------------------*/
  
  model = SBMLDocument_createModel(sbmlDoc);
  Model_setId(model,model_name);
  
  // Get a SBasePlugin_t object plugged in the model object.
  if(SBMLfbcversion == 2)
  {  
    modelPlug = SBase_getPlugin((SBase_t *)(model), "fbc");
    
    // set the fbc strict attribute
    FbcModelPlugin_setStrict(modelPlug, 1);
  }
  
  // Model NOTERS
  if (!Rf_isNull(mod_notes)) 
  {  
    char *Modnotes = (char*) CHAR(STRING_ELT(mod_notes, 0));
    if ((Modnotes != NULL) && (Modnotes[0] != '\0')) 
    {
      //printf("Canno: %s", Cnotes);
      SBase_setNotesString((SBase_t*) model , Modnotes);
    }
  }
  
  /*Annotation*/
  if (!Rf_isNull(mod_anno) ) 
  {  char *Manno   = (char*) CHAR(STRING_ELT(mod_anno, 0));
    if((Manno != NULL) && (Manno[0] != '\0' )) 
    {  
      SBase_setMetaId((SBase_t*)model,model_name);
      char Mannocopy[strlen(Manno)+1];
      strcpy(Mannocopy,Manno);
      // PARSING
      ParseModtoAnno((SBase_t*)model, Mannocopy);
      
    }
  }
  
  /*---------------------------------------------------------------------------
  *
  * Creates UnitDefinition objects inside the Model object.
  *
  *---------------------------------------------------------------------------*/
  /*
  unitdef = Model_createUnitDefinition(model);
  UnitDefinition_setId(unitdef,"litre_per_mole_per_second");
  
  //  Creates an Unit inside the UnitDefinition object ("litre_per_mole_per_second") 
  
  unit = UnitDefinition_createUnit(unitdef);
  Unit_setKind(unit,UNIT_KIND_MOLE);
  Unit_setExponent(unit,-1);
  
  //  Creates an Unit inside the UnitDefinition object ("litre_per_mole_per_second") 
  
  unit = UnitDefinition_createUnit(unitdef);
  Unit_setKind(unit,UNIT_KIND_LITRE);
  Unit_setExponent(unit,1);
  
  //  Creates an Unit inside the UnitDefinition object ("litre_per_mole_per_second") 
  
  unit = UnitDefinition_createUnit(unitdef);
  Unit_setKind(unit,UNIT_KIND_SECOND);
  Unit_setExponent(unit,-1);
  
  */
  
  /*---------------------------------------------------------------------------
  *
  * Creates a Compartment object inside the Model object. 
  *
  *---------------------------------------------------------------------------*/
  
  const char *sName;
  int i;
  int hasBoundary=0;
  for (i=0; i<LENGTH(mod_compart); i++) 
  {
    sName = CHAR(STRING_ELT(mod_compart, i));
    comp = Model_createCompartment(model);
    Compartment_setId(comp,sName);
    Compartment_setConstant(comp,1);
    if( strcmp(sName,"BOUNDARY")==0 || strcmp(sName,"Boundary")==0  || strcmp(sName,"boundary")==0)hasBoundary=1;
    if (!Rf_isNull(com_notes) && Rf_length(com_notes) > 1) 
    {  
      char *Cnotes = (char*) CHAR(STRING_ELT(com_notes, i));
      if ((Cnotes != NULL) && (Cnotes[0] != '\0')) 
      {
        //printf("Canno: %s", Cnotes);
        SBase_setNotesString((SBase_t*)comp, Cnotes);
      }
    }
    
    
    if (!Rf_isNull(com_anno) && Rf_length(com_anno) > 1  ) 
    {  char *Manno   = (char*) CHAR(STRING_ELT(com_anno, i));
      if((Manno != NULL) && (Manno[0] != '\0' )) 
      {  
        SBase_setMetaId((SBase_t*)comp, CHAR(STRING_ELT(mod_compart, i)));
        char Mannocopy[strlen(Manno)+1];
        strcpy(Mannocopy,Manno);
        ParseModtoAnno((SBase_t*) comp,Mannocopy);
        
      }
    }
    
  }
  
  
  /* Boundary Compartment */
  if(hasBoundary==0 && Rf_isNull(met_bnd) && Rf_length(met_bnd) <= 1 && !Rf_isNull(ex_react))
  {  
  comp = Model_createCompartment(model);
  Compartment_setId(comp,"BOUNDARY");
  Compartment_setConstant(comp,1);
  hasBoundary=1;
  }
  /*---------------------------------------------------------------------------
  *
  * Creates Species objects inside the Model object. 
  *
  *---------------------------------------------------------------------------*/
  for (i=0; i<LENGTH(met_name); i++) 
  {
    
    
    sp = Model_createSpecies(model);
    Species_setId(sp,CHAR(STRING_ELT(met_id, i)));
    Species_setName(sp,CHAR(STRING_ELT(met_name, i)));
    Species_setCompartment(sp,CHAR(STRING_ELT(met_comp, i)));
    Species_setHasOnlySubstanceUnits(sp, 0);
    Species_setConstant(sp, 0);
    
    //Bondary Condition
    if (!Rf_isNull(met_bnd) && Rf_length(met_bnd) > 1) Species_setBoundaryCondition(sp, LOGICAL(met_bnd)[i]);
    else Species_setBoundaryCondition(sp, 0);
      
    
    int hasNotes=0;
    const char* metnote="";
    if (!Rf_isNull(met_form) && Rf_length(met_form) > 1) 
    {
      if (SBMLfbcversion >0)
      {  
        SpeciesPlug = SBase_getPlugin((SBase_t *)(sp), "fbc");
        FbcSpeciesPlugin_setChemicalFormula(SpeciesPlug, CHAR(STRING_ELT(met_form, i)));
      }
      
      else{
        metnote = append_strings(metnote,"<html:p>FORMULA: ","");
        metnote =append_strings(metnote,CHAR(STRING_ELT(met_form, i)),"");
        metnote =append_strings(metnote," </html:p>","");
        hasNotes=1;
      }
    }
    
    if (!Rf_isNull(met_charge) && Rf_length(met_charge) > 1) 
    {
      if (SBMLfbcversion >0)
      {
        SpeciesPlug = SBase_getPlugin((SBase_t *)(sp), "fbc");
        FbcSpeciesPlugin_setCharge(SpeciesPlug, INTEGER(met_charge)[i]);
      }
      else
      {  
        metnote = append_strings(metnote,"<html:p>CHARGE: ","");
        char chint[256];
        sprintf(chint, "%d", INTEGER(met_charge)[i]);
        metnote =append_strings(metnote,chint,"");
        metnote =append_strings(metnote," </html:p>","");
        hasNotes=1;
      }
    }
    
    
    if (!Rf_isNull(met_notes) && Rf_length(met_notes) > 1) 
    {
      char *Mnotes = (char*) CHAR(STRING_ELT(met_notes, i));
      if ((Mnotes != NULL) && (Mnotes[0] != '\0')) 
      {
        SBase_setNotesString((SBase_t*)sp, Mnotes);
        hasNotes=0;
      }  
      
    }
    
    
    if(hasNotes !=0 )SBase_setNotesString((SBase_t*)sp, metnote);
    
    if (!Rf_isNull(met_anno) && Rf_length(met_anno) > 1  ) 
    {  char *Manno   = (char*) CHAR(STRING_ELT(met_anno, i));
      
      if((Manno != NULL) && (Manno[0] != '\0' )) 
      {  

        SBase_setMetaId((SBase_t*)sp, CHAR(STRING_ELT(met_id, i)));
        
        // COPY STRING  
        char *Manno   = (char*) CHAR(STRING_ELT(met_anno, i));
        char Mannocopy[strlen(Manno)+1];
        strcpy(Mannocopy,Manno);
        // PARSING
        ParseModtoAnno((SBase_t*)sp, Mannocopy);
        
      }
    }
    
    
    
  } 
  /*---------------------------------------------------------------------------
  *
  * Creates Reaction objects inside the Model object. 
  *
  *---------------------------------------------------------------------------*/
  
  /*  LISTOFGENES
  * 
  */
  if(SBMLfbcversion == 2 && Rf_length(gpr) > 1)
  { 
    if (!Rf_isNull(gpr) && Rf_length(gpr) > 1)
    {  
      modelPlug = SBase_getPlugin((SBase_t *)(model), "fbc");
      //GeneProduct_t* gene;
      char* genid = malloc( 100 );
      
      for (i=0; i<LENGTH(allgenes); i++)
      {
        GeneProduct_t* gene = GeneProduct_create(3,1,2);
        sprintf(genid,"G_%s", CHAR(STRING_ELT(allgenes, i)));
        GeneProduct_setId(gene ,genid);
        GeneProduct_setLabel(gene ,CHAR(STRING_ELT(allgenes, i)));
        FbcModelPlugin_addGeneProduct(modelPlug,gene);
      }
      free(genid);
    }
    /* 
    ListOfParemters
    */
    
    // create the Parameters
    
    para = Model_createParameter(model);
    Parameter_setId(para, "default_lb");
    Parameter_setConstant(para, 1);
    Parameter_setValue(para, sybilmin);
    SBase_setSBOTerm((SBase_t *)para,626);
    
    para = Model_createParameter(model);
    Parameter_setId(para, "default_ub");
    Parameter_setConstant(para, 1);
    Parameter_setValue(para, sybilmax);
    SBase_setSBOTerm((SBase_t *)para,626);
    
    para = Model_createParameter(model);
    Parameter_setId(para, "default_0");
    Parameter_setConstant(para, 1);
    Parameter_setValue(para, 0);
    SBase_setSBOTerm((SBase_t *)para,626);

  }
  
  
  // LOOP FOR REACTION
  for (i=0; i<LENGTH(react_name); i++)
  {
    reaction = Model_createReaction(model);
    
    Reaction_setId(reaction, CHAR(STRING_ELT(react_id, i)));
    Reaction_setName(reaction, CHAR(STRING_ELT(react_name, i)));
    Reaction_setReversible(reaction,LOGICAL(react_rev)[i]);
    Reaction_setFast(reaction, 0);
    
    if(SBMLfbcversion == 2)
    {  
      // FBCv2
      reactionPlug = SBase_getPlugin((SBase_t *)(reaction), "fbc");    
      GeneProductAssociation_t* gpa = GeneProductAssociation_create(3,1,2);
      
      //  FbcAnd_t * und= FbcAnd_create(3,1,2);
      asso= FbcAssociation_parseFbcInfixAssociation(CHAR(STRING_ELT(gpr, i)),modelPlug);
      GeneProductAssociation_setAssociation(gpa,asso);
      FbcReactionPlugin_setGeneProductAssociation(reactionPlug,gpa);
    }
    
    
    
    const char* notesString = "";
    
    
    
    if (!Rf_isNull(react_notes) && Rf_length(react_notes) > 1) 
    {
      char *Rnotes = (char*) CHAR(STRING_ELT(react_notes, i));
      if ((Rnotes != NULL) && (Rnotes[0] != '\0')) 
      {
        //printf("Reaction Nores:%s ",Rnotes);
        SBase_setNotesString((SBase_t*)reaction, Rnotes);
      }  
      
    } 
    else
    { 
      if(SBMLfbcversion == 0)
      {
        if (!Rf_isNull(gpr) && Rf_length(gpr) > 1)
        {  
          notesString = append_strings(notesString,"<html:p>GENE_ASSOCIATION: ","");
          notesString =append_strings(notesString,CHAR(STRING_ELT(gpr, i)),"");
          notesString =append_strings(notesString," </html:p>","");
        }
        if (!Rf_isNull(subSys) && Rf_length(subSys) > 1)
        {  
          notesString = append_strings(notesString,"<html:p>SUBSYSTEM: ","");
          notesString =append_strings(notesString,CHAR(STRING_ELT(subSys, i)),"");
          notesString =append_strings(notesString," </html:p>","");
        }
        
        SBase_setNotesString((SBase_t*)reaction, notesString);
      }
    }
    
    const double *lower_bnd = REAL(lowbnd);
    const double *upper_bnd = REAL(uppbnd);
    
    // KineticLaw
    if(SBMLfbcversion == 0)
    {  
      kl = Reaction_createKineticLaw(reaction);
      
      astMath = ASTNode_createWithType(AST_NAME);
      ASTNode_setName(astMath, "FLUX_VALUE");
      
      KineticLaw_setMath( kl, astMath);
      
      
      para = KineticLaw_createParameter( kl );
      Parameter_setId(para, "LOWER_BOUND");
      Parameter_setValue( para, lower_bnd[i]);
      //Parameter_setUnits( para, "litre_per_mole_per_second");
      
      para = KineticLaw_createParameter( kl );
      Parameter_setId(para, "UPPER_BOUND");
      Parameter_setValue( para, upper_bnd[i]);
      //Parameter_setUnits( para, "litre_per_mole_per_second");
      
      para = KineticLaw_createParameter( kl );
      Parameter_setId(para, "OBJECTIVE_COEFFICIENT");
      Parameter_setValue( para, INTEGER(obj_coef)[i]);
      
      para = KineticLaw_createParameter( kl );
      Parameter_setId(para, "FLUX_VALUE");
      Parameter_setValue( para, 0);
    }
    

    int isexchange=0;
    int k;
    if (Rf_isNull(met_bnd) && Rf_length(met_bnd) <= 1 && !Rf_isNull(ex_react))
      for (k=0; k<LENGTH(ex_react); k++) 
        if( i+1 == INTEGER(ex_react)[k]) 
          isexchange=1;
        
    int j=0;
    for (j=0; j<LENGTH(met_id); j++)
    {
    
      int hash = LENGTH(met_id) * i + j;
      if (REAL(SMatrix)[hash] != 0.00 )
      {
        
        if(REAL(SMatrix)[hash] < 0.00)
        {
          spr = Reaction_createReactant(reaction);
          SpeciesReference_setConstant(spr, 1);
          SpeciesReference_setSpecies(spr,CHAR(STRING_ELT(met_id, j)));
          SpeciesReference_setStoichiometry(spr, fabs(REAL(SMatrix)[hash]));
          
          //is Exchange Reaction
          if(isexchange==1 && !Rf_isNull(ex_react))
          {
            /* Create boundary Species */
            sp = Model_createSpecies(model);
            
            Species_setId(sp, append_strings(CHAR(STRING_ELT(met_id, j)),"BOUNDARY","_") );
            Species_setName(sp,append_strings(CHAR(STRING_ELT(met_name, j)),"BOUNDARY"," ") );
            
            Species_setCompartment(sp,"BOUNDARY");
            Species_setHasOnlySubstanceUnits(sp, 0);
            Species_setBoundaryCondition(sp, 1);
            Species_setConstant(sp, 1);
            
            /* Add boundary Species as Product */
            spr = Reaction_createProduct(reaction);
            SpeciesReference_setSpecies(spr,append_strings(CHAR(STRING_ELT(met_id, j)),"BOUNDARY","_") );
            SpeciesReference_setStoichiometry(spr,1);
            
            SpeciesReference_setConstant(spr, 1);
          }
            
            
        }else{
          spr = Reaction_createProduct(reaction);
          SpeciesReference_setConstant(spr, 1);
          SpeciesReference_setSpecies(spr,CHAR(STRING_ELT(met_id, j)));
          SpeciesReference_setStoichiometry(spr, fabs(REAL(SMatrix)[hash]));  
        }
          
      }  
    }
      
      
      
    /*Annotation*/
    if (!Rf_isNull(react_anno) && Rf_length(react_anno) > 1  ) 
    {
      char *Manno   = (char*) CHAR(STRING_ELT(react_anno, i));
      if((Manno != NULL) && (Manno[0] != '\0' )) 
      {  
        SBase_setMetaId((SBase_t*)reaction, CHAR(STRING_ELT(react_id, i)));
        char Mannocopy[strlen(Manno)+1];
        strcpy(Mannocopy,Manno);
        // PARSING
        ParseModtoAnno((SBase_t*)reaction, Mannocopy);
        
      }
    }
    
    
    
    /* FBC LEVEL 2 */
    if(SBMLfbcversion == 2)
    {  
      // Get a SBasePlugin_t object plugged in the reaction object.
      
      reactionPlug = SBase_getPlugin((SBase_t *)(reaction), "fbc");
      
      const char* para_lb;
      const char* para_ub;
      
      //default Parameter or new one
      if (lower_bnd[i]<= sybilmin)
      {
        para_lb="default_lb";
      }  
      else if (lower_bnd[i] == 0)
      {
        para_lb="default_0";
      }  
      else
      { //creacte Lower_bound Paramater
        para = Model_createParameter(model);
        Parameter_setId(para, append_strings(CHAR(STRING_ELT(react_id, i)),"lower_bound","_"));
        Parameter_setConstant(para, 1);
        Parameter_setValue(para, lower_bnd[i]);
        SBase_setSBOTerm((SBase_t *)para,625);
        
        para_lb=append_strings(CHAR(STRING_ELT(react_id, i)),"lower_bound","_");
        
      }  
      
      if (upper_bnd[i] >= sybilmax)
      {
        para_ub="default_ub";
      }
      
      else if (upper_bnd[i] == 0)
      {
        para_ub="default_0";
      }  
      
      else
      {
        //creacte upper_bound Paramater
        para = Model_createParameter(model);
        Parameter_setId(para, append_strings(CHAR(STRING_ELT(react_id, i)),"upper_bound","_"));
        Parameter_setConstant(para, 1);
        Parameter_setValue(para, upper_bnd[i]);
        SBase_setSBOTerm((SBase_t *)para,625);
        
        para_ub=append_strings(CHAR(STRING_ELT(react_id, i)),"upper_bound","_");
      }  
      
      // set the flux bounds for this reaction
      FbcReactionPlugin_setLowerFluxBound(reactionPlug, para_lb);
      FbcReactionPlugin_setUpperFluxBound(reactionPlug, para_ub);
      
      // OBJECTIVES
      if (INTEGER(obj_coef)[i]!=0)
      {
        objective = Objective_create(3, 1, 2);
        Objective_setId(objective, "obj");
        Objective_setType(objective, "maximize");
        
        fluxObjective = Objective_createFluxObjective(objective);
        FluxObjective_setReaction(fluxObjective, CHAR(STRING_ELT(react_id, i)));
        FluxObjective_setCoefficient(fluxObjective, INTEGER(obj_coef)[i]);
        
        FbcModelPlugin_addObjective(modelPlug, objective);
        
        // mark obj1 as active objective
        FbcModelPlugin_setActiveObjectiveId(modelPlug, "obj");
        
      }  
    }
      
      
  }// ENDE REACTION
  
  if(SBMLfbcversion == 1)
  {  
    
    // FBC LEVEL 1 
    
    // Get a SBasePlugin_t object plugged in the model object.
    
    modelPlug = SBase_getPlugin((SBase_t *)(model), "fbc");
    int ObjCounter = 0;
    for (i=0; i<LENGTH(react_name); i++)
    {
      
      const double *lower_bnd = REAL(lowbnd);
      const double *upper_bnd = REAL(uppbnd);
      
      char buf[24]; // 11 + 10 + 1 (signed, even if not used here) + nul = 23 (24 to be absolutely safe)
      // FBC1 FLUXBOUNDS
      sprintf(buf, "LOWER_BOUND%d", i);
      if (INTEGER(obj_coef)[i] != 1)
      {  
        fluxBound = FluxBound_create(3, 1, 1);
        FluxBound_setId(fluxBound, buf);
        FluxBound_setReaction(fluxBound, CHAR(STRING_ELT(react_id, i)));
        FluxBound_setOperation(fluxBound, "greaterEqual");
        FluxBound_setValue(fluxBound, lower_bnd[i]);
        FbcModelPlugin_addFluxBound(modelPlug, fluxBound);
        
        //printf("Upper: %s\n", buf);
        sprintf(buf, "UPPER_BOUND%d", i);
        fluxBound = FluxBound_create(3, 1, 1);
        FluxBound_setId(fluxBound, buf);
        FluxBound_setReaction(fluxBound, CHAR(STRING_ELT(react_id, i)));
        FluxBound_setOperation(fluxBound, "lessEqual");
        FluxBound_setValue(fluxBound, upper_bnd[i]);
        
        FbcModelPlugin_addFluxBound(modelPlug, fluxBound);
      }
      
      if (INTEGER(obj_coef)[i] == 1 && ObjCounter == 0)
      {
        sprintf(buf, "OBJ%d", i);
        objective = Objective_create(3, 1, 1);
        Objective_setId(objective, buf);
        Objective_setType(objective, "maximize");
        
        fluxObjective = Objective_createFluxObjective(objective);
        FluxObjective_setReaction(fluxObjective, CHAR(STRING_ELT(react_id, i)));
        FluxObjective_setCoefficient(fluxObjective, 1);
        
        FbcModelPlugin_addObjective(modelPlug, objective);
        FbcModelPlugin_setActiveObjectiveId(modelPlug, buf);
        ObjCounter = ObjCounter +1;
      }
      
      
      
    }  
  }

  /* add subsystem as groups if fbc is >= 2 */
  if(SBMLfbcversion >= 2){
  	if(!Rf_isNull(subSysGroups)){
        GroupsModelPlugin_t* groupsPlug = NULL;
  		groupsPlug = (GroupsModelPlugin_t*) SBase_getPlugin((SBase_t *)(model), "groups");
 		
  		for(int i=0; i < Rf_length(subSysGroups); i++){
  			Group_t* newGroup = GroupsModelPlugin_createGroup(groupsPlug);
			
			Group_setKindAsString(newGroup, "partonomy");
            Group_setName(newGroup, CHAR(STRING_ELT(subSysGroupsNames, i)));
			SBase_setSBOTerm((SBase_t *) newGroup, 0000633);

  			for(int j=0; j < Rf_length(VECTOR_ELT(subSysGroups, i)); j++){
				Member_t* newMember = Member_create(SBMLlevel, SBMLversion, SBMLgroupsversion);
				Member_setIdRef(newMember, CHAR(STRING_ELT(VECTOR_ELT(subSysGroups, i), j)));
				Group_addMember(newGroup, newMember);
  			}

  			//GroupsModelPlugin_addGroup(groupsPlug, newGroup);
  		}
  	}
  }

  // write SBML file
  int result = writeSBML(sbmlDoc, fname);
  SEXP out = R_NilValue;
  if (result)out = Rf_ScalarLogical(1);
  else out = Rf_ScalarLogical(0);
#else
  SEXP out = Rf_ScalarLogical(0);/* no success */
#endif
  //UNPROTECT(1);
  return out;
}


/* -------------------------------------------------------------------------- */
/* check, if FBC-Plugin is available */
SEXP isAvailableFbcPlugin() {
    SEXP out = R_NilValue;
#ifdef HAVE_FBC_PLUGIN
    out = Rf_ScalarLogical(1);
#else
    out = Rf_ScalarLogical(0);
#endif
    return out;
}


/* -------------------------------------------------------------------------- */
/* check, if Groups-Plugin is available */
SEXP isAvailableGroupsPlugin() {
    SEXP out = R_NilValue;
#ifdef HAVE_GROUPS_PLUGIN
    out = Rf_ScalarLogical(1);
#else
    out = Rf_ScalarLogical(0);
#endif
    return out;
}


/* -------------------------------------------------------------------------- */
