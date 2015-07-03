\name{getSBMLCompartList}
\alias{getSBMLCompartList}

\title{
  Get Compartment List
}

\description{
  Retrieve list of compartments included in a SBML model
}

\usage{
  getSBMLCompartList(sbmlm)
}

\arguments{
  \item{sbmlm}{
    An object of class \code{\linkS4class{sbmlPtr}} as returned by
    \code{\link{getSBMLmodel}}. This is basically a pointer to a SBML model.
  }
}

\value{
  If the SBML model contains a ListOfCompartments section, a \code{list} is
  returned:
  \item{id}{
    character vector containing the compartment id's. If no id is given for a
    particular compartment, the corresponding value is set to \code{"no_id"}.
  }
  \item{name}{
    character vector containing the reaction names. If 
    not available, the value is set to the empty string \code{""}.
  }
  \item{outside}{
    character vector containing the outside argument of the compartments. If 
    not available, the value is set to the empty string \code{""}.
  }
  All list elements have the same length.
  If the SBML model does not contain a ListOfCompartments section, \code{NULL}
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
