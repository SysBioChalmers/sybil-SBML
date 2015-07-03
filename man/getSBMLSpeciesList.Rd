\name{getSBMLSpeciesList}
\alias{getSBMLSpeciesList}

\title{
  Get Species List
}

\description{
  Retrieve list of species included in a SBML model
}

\usage{
  getSBMLSpeciesList(sbmlm)
}

\arguments{
  \item{sbmlm}{
    An object of class \code{\linkS4class{sbmlPtr}} as returned by
    \code{\link{getSBMLmodel}}. This is basically a pointer to a SBML model.
  }
}

\value{
  If the SBML model contains a ListOfSpecies section, a \code{list} is
  returned:
  \item{id}{
    character vector containing the species id's. If no id is given for a
    particular species, the corresponding value is set to \code{"no_id"}.
  }
  \item{name}{
    character vector containing the species names. If 
    not available, the value is set to the empty string \code{""}.
  }
  \item{compartment}{
    character vector containing the species compartments. If 
    not available, the value is set to the empty string \code{""}.
  }
  \item{charge}{
    integer vector containing the charge of the species. If no charge is given
    for a particular species, the corresponding value is set to \code{0}.
  }
  \item{boundaryCondition}{
    logical vector containing the boundaryCondition flag of the species. If no 
    boundaryCondition flag is given for a particular species, the corresponding
    value is set to \code{FALSE}.
  }
  All list elements have the same length.
  If the SBML model does not contain a ListOfSpecies section, \code{NULL}
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
