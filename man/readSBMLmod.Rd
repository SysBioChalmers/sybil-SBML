\name{readSBMLmod}
\alias{readSBMLmod}

\encoding{utf8}

\title{Reads a Metabolic Network in SBML Format}

\description{
  The function reads a metabolic network in SBML format. The function
  returns an S4 object of the class \code{\link{modelorg}}
}

\usage{
  readSBMLmod(filename, description,
              def_bnd = SYBIL_SETTINGS("MAXIMUM"),
              validateSBML = FALSE,
              extMetFlag = "b",
              bndCond = TRUE,
              ignoreNoAn = FALSE,
              mergeMet = TRUE,
              balanceReact = TRUE,
              remUnusedMetReact = TRUE,
              singletonMet = FALSE,
              deadEndMet = FALSE,
              remMet = FALSE,
              constrMet = FALSE,
              tol = SYBIL_SETTINGS("TOLERANCE"))
}

\arguments{
  \item{filename}{
    SBML file containing the model
  }
  \item{description}{
    Character vector containing a description of the model.\cr
    Default: \code{filename}.
  }
  \item{def_bnd}{
    Single numeric value. Absolute value for upper and lower bounds for
    reaction contraints -- if they are missing in the SBML file.\cr
    Default: \code{MAXIMUM(SYBIL_SETTINGS)}.
  }
  \item{validateSBML}{
    Boolean: validate the xml file.\cr
    Default: \code{TRUE}.
  }
  \item{extMetFlag}{
    A single character string how external metabolites were identified. If the
    metabolite id ends in \code{"_extMetFlag"}, the corresponding metabolite is
    considered to be external and will be removed if \code{bndCond} is set to
    \code{FALSE}.\cr
    Default: \code{"b"}.
  }
  \item{bndCond}{
    Boolean: use the value of SBML tag \code{boundaryCondition} in order to
    identify external metabolites.\cr
    Default: \code{TRUE}.
  }
  \item{ignoreNoAn}{
    Boolean: if set to \code{TRUE}, any notes and annotation fields in the
    listOfReactions in an SBML file are ignored.\cr
    Default: \code{FALSE}.
  }
  \item{mergeMet}{
    Boolean: if set to \code{TRUE}, metabolites used more than once as reactant
    or product in a particular reaction are added up, see details below. If set
    to \code{FALSE}, the last value is used without warning.\cr
    Default: \code{TRUE}.
  }
  \item{balanceReact}{
    Boolean: if set to \code{TRUE}, metabolites used as reactant and product in
    a particular reaction at the same time are balanced, see details below. If
    set to \code{FALSE} the last value is used without warning (reactants before
    products).\cr
    Default: \code{TRUE}.
  }
  \item{remUnusedMetReact}{
    Boolean: if set to TRUE, metabolites and reactions which are not used in the
    stoichiometric matrix will be removed. A metabolite or a reaction is
    considered as unused, if the corresponding element of \code{rowSums}
    (metabolites) or \code{colSums} (reactions) of the binary version of the
    stoichiometric matrix is zero, see details below.  If set to \code{FALSE},
    only a warning is given.\cr
    Default: \code{FALSE}.
  }
  \item{singletonMet}{
    Boolean: if set to TRUE, metabolites appearing only once in the
    stoichiometric matrix are identified. Metabolites appear only
    once, if \code{\link{rowSums}} of the binary stoichiometric matrix is one in
    the corresponding row, see details below.\cr
    Default: \code{FALSE}.
  }
  \item{deadEndMet}{
    Boolean: if set to TRUE, metabolites which are produced but not consumed, or
    vice versa are identified, see details below. If both arguments
    \code{singletonMet} and \code{deadEndMet} are set to \code{TRUE}, the
    function will first look for singleton metabolites, and exclude them (and
    the corresponding reactions) from the search list. Afterwards, dead end
    metabolites are searched only in the smaller model.\cr
    Default: \code{FALSE}.
  }
  \item{remMet}{
    Boolean: if set to TRUE, metabolites identified as singleton or dead end
    metabolites will be removed from the model. Additionally, reactions
    containing such metabolites will be removed also.\cr
    Default: \code{FALSE}.
  }
  \item{constrMet}{
    Boolean: if set to TRUE, reactions containing metabolites identified as
    singleton or dead end metabolites will be constrained to zero.\cr
    Default: \code{FALSE}.
  }
  \item{tol}{
    A single numeric value, giving the smallest positive floating point number
    unequal to zero, see details below.\cr
    Default: \code{TOLERANCE(SYBIL_SETTINGS)}.
  }
}

\details{
  The library \code{libSBML} is used to read an SBML file and to collect
  the information in an object of the class \code{\link[sybil]{modelorg}}.

  If a metabolite is used more than once as product or
  reactant of a particular reaction, it is merged:
  \code{a + (2) a} is converted to \code{(3) a}
  and a warning will be given.

  If a metabolite is used first as reactant and then as
  product of a particular reaction, the reaction is
  balanced:
      \code{(2) b + a -> b + c}
  is converted to
      \code{    b + a ->     c}

  A binary version of the stoichiometric matrix \eqn{S} is constructed
  via \eqn{\left|S\right| > tol}{abs(S) > tol}.

  A binary version of the stoichiometric matrix \eqn{S} is scanned for reactions
  and metabolites which are not used in S. If there are some, a warning will be
  given and the corresponding reactions and metabolites will be removed from
  the model if \code{remUnusedMetReact} is set to \code{TRUE}.

  The binary version of the stoichiometric matrix \eqn{S} is scanned for
  metabolites, which are used only once in S. If there are some, at least a
  warning will be given. If either \code{constrMet} or \code{remMet} is set to
  \code{TRUE}, the binary version of \eqn{S} is scanned for paths of singleton
  metabolites. If \code{constrMet} is set to \code{TRUE}, reactions containing
  those metabolites will be constrained to zero; if \code{remMet} is set to
  \code{TRUE}, the metabolites and the reactions containing those metabolites
  will be removed from the network.

  In order to find path of singleton metabolites a binary version of the
  stoichiometric matrix \eqn{S} is used. Sums of rows gives the vector of
  metabolite usage, each element is the number of reactions a metabolite
  participates.  A single metabolite (singleton) is a metabolite with a row sum
  of zero. All columns in \eqn{S} (reactions) containing singleton metabolites
  will be set to zero. And again, singleton metabolites will be searched until
  none are found.

  The algorithm to find dead end metabolites works in a quite similar way, but
  not in the binary version of the stoichiometric matrix. Here, metabolite
  \code{i} is considered as dead end, if it is for example produced by reaction
  \code{j} but not used by any other reaction \code{k}.
}

\value{
  An S4 object of the class \code{\link[sybil]{modelorg}}.
}

\references{
  The BiGG database \url{http://bigg.ucsd.edu/}.

  Bornstein, B. J., Keating, S. M., Jouraku, A., and Hucka M. (2008)
  LibSBML: An API Library for SBML. \emph{Bioinformatics} \bold{24},
  880--881.

  Schellenberger, J., Park, J. O., Conrad, T. C., and Palsson, B. Ø., (2010)
  BiGG: a Biochemical Genetic and Genomic knowledgebase of large scale metabolic
  reconstructions. \emph{BMC Bioinformatics} \bold{11}, 213.

  Becker, S. A., Feist, A. M., Mo, M. L., Hannum, G., Palsson, B. Ø.  and
  Herrgard, M. J. (2007) Quantitative prediction of cellular metabolism with
  constraint-based models: the COBRA Toolbox. \emph{Nat Protoc} \bold{2},
  727--738.

  Schellenberger, J., Que, R., Fleming, R. M. T., Thiele, I., Orth, J. D.,
  Feist, A. M., Zielinski, D. C., Bordbar, A., Lewis, N. E., Rahmanian, S.,
  Kang, J., Hyduke, D. R. and Palsson, B. Ø. (2011) Quantitative prediction of
  cellular metabolism with constraint-based models: the COBRA Toolbox v2.0.
  \emph{Nat Protoc} \bold{6}, 1290--1307.
}

\author{
  Gabriel Gelius-Dietrich
}

\note{
  The function \code{readSBMLmod} makes use of the library
  \code{libSBML} (\url{http://www.sbml.org}).
}

\seealso{
  \code{\link{validateSBMLdocument}}
}

\examples{
  dir  <- system.file(package = "sybilSBML", "extdata")
  file <- file.path(dir, "ecoli_core_model.xml")
  mod  <- readSBMLmod(file, bndCond = FALSE)
}

\keyword{ IO }

