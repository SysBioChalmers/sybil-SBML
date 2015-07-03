\name{getSBMLmodel}
\alias{getSBMLmodel}

\title{
  Get SBML Model
}
\description{
  Retrieve a pointer to a SBML model.
}
\usage{
  getSBMLmodel(sbmlf, ptrtype = "sbml_mod")
}

\arguments{
  \item{sbmlf}{
    An object of class \code{\linkS4class{sbmlPtr}} as returned by
    \code{\link{openSBMLfile}}.
  }
  \item{ptrtype}{
    A single character string containing the pointer type.
  }
}

\value{
  A object of class \code{\linkS4class{sbmlPtr}}.
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
  \code{\link{openSBMLfile}}, \code{\linkS4class{sbmlPtr}}
}

\keyword{ IO }
