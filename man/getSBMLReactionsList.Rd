\name{getSBMLReactionsList}
\alias{getSBMLReactionsList}

\title{
  Get Reaction List
}

\description{
  Retrieve list of reactions included in a SBML model
}
\usage{
  getSBMLReactionsList(sbmlm)
}

\arguments{
  \item{sbmlm}{
    An object of class \code{\linkS4class{sbmlPtr}} as returned by
    \code{\link{getSBMLmodel}}. This is basically a pointer to a SBML model.
  }
}

\value{
  If the SBML model contains a ListOfReactions section, a \code{list} is
  returned:
  \item{id}{
    character vector containing the reaction id's. If no id is given for a
    particular reaction, the corresponding value is set to \code{"no_id"}.
  }
  \item{name}{
    character vector containing the reaction names. If 
    not available, the value is set to the empty string \code{""}.
  }
  \item{reversible}{
    logical vector containing the reversible flag of the reactions. If no 
    reversible flag is given for a particular reaction, the corresponding value
    is set to \code{FALSE}.
  }
  \item{notes}{
    character vector containing the note strings. If no note is given for a
    particular reaction, the corresponding value is set to the empty string
    \code{""}.
  }
  \item{annotation}{
    character vector containing the annotation strings. If no annotation is
    given for a particular reaction, the corresponding value is set to the empty
    string \code{""}.
  }
  \item{reactants}{
    a list containing the reactions reactants.
    \describe{
      \item{id}{
        character vector containing the species reference id's involved as
        reactants. If no id is given for a particular species, the corresponding
        value is set to \code{"no_id"}.
      }
      \item{species}{
        character vector containing the species id's involved as reactants.
        If no id is given for a particular species, the corresponding value is
        set to \code{"no_species"}.
      }
      \item{stoichiometry}{
        numeric vector containing the stoichiometry of the species. If no value
        is given for a particular species, the corresponding value is set to
        \code{1}.
      }
    }
    If no reactant is given for a particular reaction, the corresponding value
    is set to \code{NULL}.
  }
  \item{products}{
    a list containing the reactions products.
    \describe{
      \item{id}{
        character vector containing the species reference id's involved as
        products. If no id is given for a particular species, the corresponding
        value is set to \code{"no_id"}.
      }
      \item{species}{
        character vector containing the species id's involved as products.
        If no id is given for a particular species, the corresponding value is
        set to \code{"no_species"}.
      }
      \item{stoichiometry}{
        numeric vector containing the stoichiometry of the species. If no value
        is given for a particular species, the corresponding value is set to
        \code{1}.
      }
    }
    If no product is given for a particular reaction, the corresponding value
    is set to \code{NULL}.
  }
  \item{kinetic_law}{
    a list containing the reactions parameters.
    \describe{
      \item{id}{
        character vector containing the parameter id's. If no id is given for a
        particular parameter, the corresponding value is set to \code{"no_id"}.
      }
      \item{value}{
        numeric vector containing the parameter values. If no value is given
        for a particular parameter, the corresponding value is set to \code{0}.
      }
      \item{units}{
        character vector containing the parameter units. If no unit is given for
        a particular parameter, the corresponding value is set to the empty
        string \code{""}.
      }
    }
    If no parameter is given for a particular reaction, the corresponding value
    is set to \code{NULL}.
  }
  All list elements have the same length.
  If the SBML model does not contain a ListOfReactions section, \code{NULL}
  is returned.
}

\references{
  Bornstein, B. J., Keating, S. M., Jouraku, A., and Hucka M. (2008)
  LibSBML: An API Library for SBML. \emph{Bioinformatics} \bold{24},
  880--881.
}

\author{
  Gabriel Gelius-Dietrich <geliudie@uni-duesseldorf.de>

  Maintainer: Mayo Roettger <mayo.roettger@hhu.de>
}


\seealso{
  \code{\link{getSBMLmodel}}, \code{\linkS4class{sbmlPtr}}
}

\keyword{ IO }
