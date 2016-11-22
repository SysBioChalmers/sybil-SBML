#------------------------------------------------------------------------------#
#                          Link to libSBML for sybil                           #
#------------------------------------------------------------------------------#

#  sybilSBML.R
#  Link to libSBML for sybil.
#
#  Copyright (C) 2010-2013 Gabriel Gelius-Dietrich, Dpt. for Bioinformatics,
#  Institute for Informatics, Heinrich-Heine-University, Duesseldorf, Germany.
#  All right reserved.
#  Email: geliudie@uni-duesseldorf.de
#
#  This file is part of sybilSBML.
#
#  SybilSBML is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  SybilSBML is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with SybilSBML.  If not, see <http://www.gnu.org/licenses/>.


#------------------------------------------------------------------------------#

versionLibSBML <- function() {

    version <- .Call("getLibSBMLversion", PACKAGE = "sybilSBML")
    return(version)

}


#------------------------------------------------------------------------------#

openSBMLfile <- function(fname, ptrtype = "sbml_doc") {

    if ( file.exists(fname) == FALSE ) {
        stop("file not found: ", sQuote(fname))
    }

    sbmlf <- .Call("readSBMLfile", PACKAGE = "sybilSBML",
                   as.character(normalizePath(fname)[1]),
                   as.character(ptrtype)
             )

    sbmlfP <- sbmlDocPointer(sbmlf)

    return(sbmlfP)
}


#------------------------------------------------------------------------------#

closeSBMLfile <- function(sbmlf) {

    invisible(
        .Call("delDocument", PACKAGE = "sybilSBML",
              sbmlPointer(sbmlf)
        )
    )

}


#------------------------------------------------------------------------------#

getSBMLmodel <- function(sbmlf, ptrtype = "sbml_mod") {

    sbmlm <- .Call("getSBMLmodel", PACKAGE = "sybilSBML",
                   sbmlPointer(sbmlf),
                   as.character(ptrtype)
             )

    sbmlmP <- sbmlModPointer(sbmlm, sbmlf)

    if (isTRUE(isNULLpointerSBML(sbmlmP))) {
        sbmlmP <- NULL
    }

    return(sbmlmP)
}


#------------------------------------------------------------------------------#

delSBMLmodel <- function(sbmlm) {

    invisible(
        .Call("delModel", PACKAGE = "sybilSBML",
              sbmlPointer(sbmlm)
        )
    )

}


#------------------------------------------------------------------------------#

getSBMLlevel <- function(sbmlf) {

    level <- .Call("getSBMLlevel", PACKAGE = "sybilSBML",
                   sbmlPointer(sbmlf)
             )

    return(level)
}



#------------------------------------------------------------------------------#

getSBMLversion <- function(sbmlf) {

    version <- .Call("getSBMLversion", PACKAGE = "sybilSBML",
                     sbmlPointer(sbmlf)
               )

    return(version)
}

#------------------------------------------------------------------------------#

getSBMLFbcversion <- function(sbmlf) {
  
  version <- .Call("getSBMLFbcversion", PACKAGE = "sybilSBML",
                   sbmlPointer(sbmlf)
  )
  
  return(version)
}


#------------------------------------------------------------------------------#

validateSBMLdocument <- function(sbmlf) {

    if (is(sbmlf, "character")) {
        sbmlff <- openSBMLfile(fname = sbmlf)
    }
    else {
        sbmlff <- sbmlf
    }
    
    val <- .Call("validateDocument", PACKAGE = "sybilSBML",
                 sbmlPointer(sbmlff)
           )

    if (is(sbmlf, "character")) {
        val <- getSBMLerrors(sbmlff)
        closeSBMLfile(sbmlff)
    }

    return(val)
}


#------------------------------------------------------------------------------#

getSBMLerrors <- function(sbmlf) {

    err <- .Call("getSBMLerrors", PACKAGE = "sybilSBML",
                 sbmlPointer(sbmlf)
           )

    err <- sbmlError(err, sbmlf)

    return(err)
}


#------------------------------------------------------------------------------#

getSBMLmodId <- function(sbmlm) {

    modid <- .Call("getSBMLmodId", PACKAGE = "sybilSBML",
                   sbmlPointer(sbmlm)
             )

    return(modid)
}


#------------------------------------------------------------------------------#

getSBMLmodName <- function(sbmlm) {

    modn <- .Call("getSBMLmodName", PACKAGE = "sybilSBML",
                  sbmlPointer(sbmlm)
            )

    return(modn)
}

#------------------------------------------------------------------------------#

getSBMLmodNotes <- function(sbmlm) {
  
  modnotes <- .Call("getSBMLmodNotes", PACKAGE = "sybilSBML",
                sbmlPointer(sbmlm)
  )
  
  return(modnotes)
}

#------------------------------------------------------------------------------#

getSBMLmodAnnotation <- function(sbmlm) {
  
  modanno <- .Call("getSBMLmodAnnotation", PACKAGE = "sybilSBML",
                sbmlPointer(sbmlm)
  )
  
  return(modanno)
}


#------------------------------------------------------------------------------#

getSBMLnumCompart <- function(sbmlm) {

    num <- .Call("getSBMLnumCompart", PACKAGE = "sybilSBML",
                  sbmlPointer(sbmlm)
            )

    return(num)
}


#------------------------------------------------------------------------------#

getSBMLnumSpecies <- function(sbmlm) {

    num <- .Call("getSBMLnumSpecies", PACKAGE = "sybilSBML",
                  sbmlPointer(sbmlm)
            )

    return(num)
}


#------------------------------------------------------------------------------#

getSBMLnumReactions <- function(sbmlm) {

    num <- .Call("getSBMLnumReactions", PACKAGE = "sybilSBML",
                  sbmlPointer(sbmlm)
            )

    return(num)
}


#------------------------------------------------------------------------------#

getSBMLunitDefinitionsList <- function(sbmlm) {

    units <- .Call("getSBMLunitDefinitionsList", PACKAGE = "sybilSBML",
                   sbmlPointer(sbmlm)
             )

    return(units)
}


#------------------------------------------------------------------------------#

getSBMLCompartList <- function(sbmlm) {

    comp <- .Call("getSBMLCompartList", PACKAGE = "sybilSBML",
                  sbmlPointer(sbmlm)
            )

    return(comp)
}


#------------------------------------------------------------------------------#

getSBMLSpeciesList <- function(sbmlm) {

    spec <- .Call("getSBMLSpeciesList", PACKAGE = "sybilSBML",
                  sbmlPointer(sbmlm)
            )

    return(spec)
}


#------------------------------------------------------------------------------#

getSBMLReactionsList <- function(sbmlm) {

    react <- .Call("getSBMLReactionsList", PACKAGE = "sybilSBML",
                   sbmlPointer(sbmlm)
             )

    return(react)
}


#------------------------------------------------------------------------------#
#export mod to SBML
deformatSBMLid <- function(idstr) {
  idstr <- gsub("-", "_DASH_", idstr, fixed = TRUE)
  idstr <- gsub("(", "_LPAREN_", idstr, fixed = TRUE)
  idstr <- gsub(")", "_RPAREN_", idstr, fixed = TRUE)
  idstr <- gsub("[", "_", idstr, fixed = TRUE)
  idstr <- gsub("]", "", idstr, fixed = TRUE)
  idstr <- gsub(",", "_COMMA_", idstr, fixed = TRUE)
  idstr <- gsub(".", "_PERIOD_", idstr, fixed = TRUE)
  idstr <- gsub("'", "_APOS_", idstr, fixed = TRUE)
  idstr <- sub("\\(e\\)$", "_e_", idstr)
  idstr <- gsub("-", "_", idstr, fixed = TRUE)
  return(idstr)
}

deformatGene<-function(idstr) {
  # idstr <- gsub("and", "&&", idstr, fixed = TRUE)
  #  idstr <- gsub("or", "||", idstr, fixed = TRUE)
  idstr <- gsub("( ", "(", idstr, fixed = TRUE)
  idstr <- gsub(" (", "(", idstr, fixed = TRUE)
  idstr <- gsub(") ", ")", idstr, fixed = TRUE)
  idstr <- gsub(" )", ")", idstr, fixed = TRUE)
  idstr <- gsub(":", "_", idstr, fixed = TRUE)
  return(idstr)
}

exportSBML<- function(morg=NULL,level=2,version=4,FbcLevel=0,filename="export.xml",recoverExtMet=TRUE,printNotes=TRUE,printAnnos=TRUE,validation=FALSE ){
  if(class(morg)!="modelorg"){
    stop("morg has to be of class modelorg\n")
  }
  
  ###right 
  if(level==1)
  { 
    # test if Matrix has no double values
    if( !all( S(morg) == floor(S(morg))) ) warning("Level 1 does not support double values")
    FbcLevel=0
      if(version != 2)
      {
        warning("just Level 1 Version 2 will be supported")
        version=2
      }
  }else if (level==2)
  { 
    FbcLevel=0
    if(version >5)
    {
      warning("Level 2 Version 5 will be supported")
      version=5 
    }
    if(version < 1)
    {
      warning("Level 2 Version 1 will be supported")
      version=1 
    }  
  }
  else if (level==3)
  { if(FbcLevel >2)FbcLevel=2
  if(version != 1)
  {
    print("Level 3 Version 1 will be supported")
    version=1 
  }
  
  }else {
    stop(" Support just for Level 1,2 and 3 \n")
  }
  
  ##All GENES### 
  allgenes<- unique(unlist(genes(morg)))
  allgenes<-allgenes[allgenes != ""]
  
  
  ##EXCHANGE REACTIONS##
  ex <- findExchReact(morg)
  ex_react<-NULL
  # if recoverExtMet== FALSE => null for ex_react
  if( (!is.null(ex)) && (recoverExtMet) )
  {
    if(!(all(diag(S(morg)[met_pos(ex), react_pos(ex)])==-1)))
      stop("exchange reactions with Scoeff different than -1\n")
    ex_react<-as.integer(react_pos(ex))
  }
  
  ### Build wrapper for C Function #####
  
  
  ##Met Attribute###
  com_meta_id<- NULL
  met_meta_id<- NULL
  com_notes <- NULL
  com_annotation<- NULL
  met_notes<- NULL
  met_anno<- NULL
  met_bnd<- NULL
  met_charge<-NULL
  met_formula<-NULL
  
  react_notes<-NULL
  react_anno<-NULL
  
  mod_notes<-NULL
  mod_anno<-NULL
  
  #subsystem
  # is subsystem Empty
  if(length( colnames(subSys(morg)))==1 && colnames(subSys(morg))[1]=="" ) modhasubSys<-FALSE
  else modhasubSys<-TRUE
  
  newsubS<- NULL

  if( .hasSlot(morg,"mod_attr") &&  .hasSlot(morg,"comp_attr") &&  .hasSlot(morg,"met_attr")  &&  .hasSlot(morg,"react_attr") )
    newSybil<-TRUE
  else newSybil<-FALSE
  ### Start newSybil attr
  if(newSybil)
  { 
    if(("notes" %in% colnames(mod_attr(morg))) && (printNotes) ) mod_notes<-as.character(mod_attr(morg)[['notes']])
    if(("annotation" %in% colnames(mod_attr(morg))) && (printAnnos) ) mod_annotation<-as.character(mod_attr(morg)[['annotation']])
    
    if(("notes" %in% colnames(comp_attr(morg)))    && (printNotes) ) com_notes<-as.character(as.list((comp_attr(morg)[['notes']])))
    if(("annotation" %in% colnames(comp_attr(morg))) && (printAnnos)  ) com_annotation<-as.character(as.list((comp_attr(morg)[['annotation']])))
    
    if("charge" %in% colnames(met_attr(morg))) met_charge<- as.integer(as.list((met_attr(morg)[['charge']])))
    if("chemicalFormula" %in% colnames(met_attr(morg))) met_formula<-as.character(as.list((met_attr(morg)[['chemicalFormula']])))
    if(("annotation" %in% colnames(met_attr(morg))) && (printAnnos)) met_anno<-as.character(as.list((met_attr(morg)[['annotation']])))
    if("boundaryCondition" %in% colnames(met_attr(morg))) met_bnd<-as.logical(as.list((met_attr(morg)[['boundaryCondition']])))
    
    if(("notes" %in% colnames(met_attr(morg))) && (printNotes) )
    {   # delete Formular and charge from notes to do
      met_notes<-as.character(as.list((met_attr(morg)[['notes']])))
      if (!is.null(met_charge) || !is.null(met_formula))
      {  
        for ( i in 1:met_num(morg)) 
        {
          
          if(nchar(met_notes[i])> 8)
          { 
            # Have Assocation in Notes?yes: replace no: append
            
            if (regexpr("html:p", met_notes[i], fixed = TRUE) == -1)tag <- "p"
            else tag <- "html:p"
            
            
            if (!is.null(met_formula))
            {    
              haveform<-grepl("FORMULA: [^<]+",met_notes[i])
              #Have Gene if not ->no need to write in FBC2
              if(haveform)
              {  
                if(FbcLevel==0)met_notes[i]<-sub("FORMULA: [^<]+",paste("FORMULA: ",met_formula[i], sep = ""), met_notes[i], perl = TRUE)
                else met_notes[i]<-sub(paste("<",tag,">","FORMULA: [^<]+","</",tag,">",sep = ""),"",met_notes[i], perl = TRUE)
              }    
              else if(FbcLevel==0) met_notes[i]<-gsub("</notes>",paste("<",tag,">","FORMULA: ",met_formula[i],"</",tag,">","\n</notes>",sep = ""),met_notes[i])
            }
            if (!is.null(met_charge))
            {    
              havecharge<-grepl("CHARGE: [^<]+",met_notes[i])
              #Have Subsystem
              if(havecharge)
              {  
                if(FbcLevel !=0 || (level==2 && version==1 ))
                  met_notes[i]<-sub(paste("<",tag,">","CHARGE: [^<]+","</",tag,">",sep = ""),"",met_notes[i], perl = TRUE)
                else met_notes[i]<-sub("CHARGE: [^<]+",paste("CHARGE: ",met_charge[i], sep = ""), met_notes[i], perl = TRUE)
                
                
              }  
              else if(FbcLevel==0) if(level!=2 && version!=1) met_notes[i]<-gsub("</notes>",paste("<",tag,">","CHARGE: ",met_charge[i],"</",tag,">","\n</notes>",sep = ""),met_notes[i])
            }
          }
        }  
      }
      
    }
    if(("annotation" %in% colnames(react_attr(morg))) && (printAnnos)) react_anno<-as.character(as.list((react_attr(morg)[['annotation']])))
    
    # Merge Notes with "our" Notes and make sure gpr Rules from gpr
    if(("notes" %in% colnames(react_attr(morg))) && (printNotes))
    {
      react_notes<-as.character(as.list((react_attr(morg)[['notes']])))
      # using
      #  SubSystem EXISTIERT nicht colnames(subSys(ec))
      
      for ( i in 1:react_num(morg)) 
      {
        # using the for loop
        if(modhasubSys)newsubS[i]<- paste(names(which(subSys(morg)[i,])), collapse=", ")   
        if(nchar(react_notes[i])> 8)
        { 
          # Have Association in Notes? yes: replace no: append
          
          if (regexpr("html:p", react_notes[i], fixed = TRUE) == -1)tag <- "p"
          else tag <- "html:p"
          
          havegene<-grepl("GENE[_ ]?ASSOCIATION: [^<]+",react_notes[i])
          havesub<-grepl("SUBSYSTEM: [^<]+",react_notes[i])
          
          #Have Gene if not ->no need to write in FBC2
          if(havegene)
          {
            if(FbcLevel==2) react_notes[i]<-sub(paste("<",tag,">","GENE[_ ]?ASSOCIATION: [^<]+","</",tag,">",sep = ""),"",react_notes[i], perl = TRUE)
            else react_notes[i]<-sub("GENE[_ ]?ASSOCIATION: [^<]+",paste("GENE_ASSOCIATION: ",gpr(morg)[i], sep = ""), react_notes[i], perl = TRUE)
          }
          else if(FbcLevel!=2)react_notes[i]<-gsub("</notes>",paste("<",tag,">","GENE_ASSOCIATION: ",gpr(morg)[i],"</",tag,">","\n</notes>",sep = ""),react_notes[i])
          
          #Have Subsystem
          if(havesub)react_notes[i]<-sub("SUBSYSTEM: [^<]+",paste("SUBSYSTEM: ",newsubS[i], sep = ""), react_notes[i], perl = TRUE)
          else if(modhasubSys) react_notes[i]<-gsub("</notes>",paste("<",tag,">","SUBSYSTEM: ",newsubS[i],"</",tag,">","\n</notes>",sep = ""),react_notes[i])
        }
      }  
    }  
    
    
  } ####END newSybil attr
  
  # Subsystem
  if(is.null(newsubS) && !(modhasubSys) ) for ( i in 1:react_num(morg)) {newsubS[i]<- paste(names(which(subSys(morg)[i,])), collapse=", ")}
  
  newmet_id <- paste0("M_", (deformatSBMLid(met_id(morg))))
  #newmet_id <- sub("\\[(\\w)\\]$", "_\\1",  newmet_id) # append compartment id, if in postfix with square brkts
  
  newreact_id <- paste0("R_", deformatSBMLid(react_id(morg)))
  newmet_comp<-mod_compart(morg)[met_comp(morg)]
  
  
  success <-.Call("exportSBML", PACKAGE = "sybilSBML",
                  as.integer(version),
                  as.integer(level),
                  as.integer(FbcLevel),
                  as.character(filename),
                  SYBIL_SETTINGS("MAXIMUM"),
                  as.character(mod_desc(morg)),
                  as.character(mod_name(morg)),
                  as.character(mod_compart(morg)),
                  as.character(newmet_id),
                  as.character(met_name(morg)),
                  as.character(newmet_comp),
                  met_formula,
                  met_charge,
                  as.character(newreact_id),
                  as.character(react_name(morg)),
                  as.logical(react_rev(morg)),
                  as.numeric(lowbnd(morg)),
                  as.numeric(uppbnd(morg)),
                  as.integer(obj_coef(morg)),
                  as.character(newsubS),
                  as.character(deformatGene(gpr(morg))),
                  as.numeric(shrinkMatrix(morg,j=1:react_num(morg))),
                  mod_notes,
                  mod_anno,
                  com_notes,   
                  com_annotation, 
                  met_notes,      
                  met_anno,
                  met_bnd,
                  react_notes,
                  react_anno,
                  ex_react,
                  as.logical(validation),
                  as.character(deformatGene(allgenes))
  )
  return (success)
}








