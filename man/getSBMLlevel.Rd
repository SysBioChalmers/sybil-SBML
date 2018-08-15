\name{getSBMLlevel}
\alias{getSBMLlevel}

\title{
  Get SBML Level
}
\description{
  Retrieve SBML level of SBML file.
}
\usage{
  getSBMLlevel(sbmlf)
}

\arguments{
  \item{sbmlf}{
    An object of class \code{\linkS4class{sbmlPtr}} as returned by
    \code{\link{openSBMLfile}}. This is basically a pointer to a SBML document.
  }
}

\value{
  A single integer value containing the SBML level of the SBML file.
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
  \code{\link{openSBMLfile}}, \code{\linkS4class{sbmlPtr}}
}

\keyword{ IO }
