\name{closeSBMLfile}
\alias{closeSBMLfile}

\title{
  Close SBML File
}

\description{
  Close SBML file and free all memory associated with the SBML document pointer.
}

\usage{
  closeSBMLfile(sbmlf)
}

\arguments{
  \item{sbmlf}{
    An object of class \code{\linkS4class{sbmlPtr}} as returned by
    \code{\link{openSBMLfile}}.
  }
}

\value{
  Returns \code{NULL} invisibly.
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
