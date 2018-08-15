\name{getSBMLnumCompart}
\alias{getSBMLnumCompart}

\title{
  Get Number of Compartments
}

\description{
  Retrieve number of compartments of a SBML model.
}

\usage{
  getSBMLnumCompart(sbmlm)
}

\arguments{
  \item{sbmlm}{
    An object of class \code{\linkS4class{sbmlPtr}} as returned by
    \code{\link{getSBMLmodel}}. This is basically a pointer to a SBML model.
  }
}

\value{
  A single integer value containing the number of compartments of a SBML model.
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
