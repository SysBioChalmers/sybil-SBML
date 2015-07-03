\name{getSBMLunitDefinitionsList}
\alias{getSBMLunitDefinitionsList}

\title{
  Get Unit Definitions List
}
\description{
  Retrieve list of unit definitions included in a SBML model
}
\usage{
  getSBMLunitDefinitionsList(sbmlm)
}

\arguments{
  \item{sbmlm}{
    An object of class \code{\linkS4class{sbmlPtr}} as returned by
    \code{\link{getSBMLmodel}}. This is basically a pointer to a SBML model.
  }
}

\value{
  If the SBML model contains a listOfUnitDefinitions section, a \code{list} is
  returned:
  \item{definition_id}{
    character vector containing the unit definition id's. If no id is given for
    a particular unit definition, the corresponding value is set to
    \code{"no_id"}.
  }
  \item{definition}{
    a list containing the units.
    \describe{
      \item{kind}{
        character vector containing the unit kind's involved as
        unit definitions. If no kind is given for a particular unit, the
        corresponding value is set to \code{"no_kind"}.
      }
      \item{scale}{
        integer vector containing the scale. If no scale is given for a
        particular unit, the corresponding value is set to \code{0}.
      }
      \item{exponent}{
        integer vector containing the exponent. If no exponent is given for a
        particular unit, the corresponding value is set to \code{1}.
      }
      \item{multiplier}{
        numeric vector containing the multiplier. If no multiplier is given for
        a particular species, the corresponding value is set to \code{1}.
      }
    }
    If no unit is given for a particular unit definition, the corresponding
    value is set to \code{NULL}.
  }
  All list elements have the same length.
  If the SBML model does not contain a listOfUnitDefinitions section, \code{NULL}
  is returned.
}

\references{
  Bornstein, B. J., Keating, S. M., Jouraku, A., and Hucka M. (2008)
  LibSBML: An API Library for SBML. \emph{Bioinformatics} \bold{24},
  880--881.
}

\author{
  Gabriel Gelius-Dietrich <geliudie@uni-duesseldorf.de>

  Maintainer: Claus Jonathan Fritzemeier <clausjonathan.fritzemeier@uni-duesseldorf.de>
}


\seealso{
  \code{\link{getSBMLmodel}}, \code{\linkS4class{sbmlPtr}}
}

\keyword{ IO }
