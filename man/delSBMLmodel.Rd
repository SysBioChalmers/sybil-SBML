\name{delSBMLmodel}
\alias{delSBMLmodel}

\title{
  Delete Pointer to SBML Model
}

\description{
  Delete Pointer to SBML Model
}

\usage{
  delSBMLmodel(sbmlm)
}

\arguments{
  \item{sbmlm}{
    An object of class \code{\linkS4class{sbmlPtr}} as returned by
    \code{\link{getSBMLmodel}}. This is basically a pointer to a SBML model.
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

\author{Gabriel Gelius-Dietrich}

\seealso{
  \code{\link{getSBMLmodel}}, \code{\linkS4class{sbmlPtr}}
}

\keyword{ IO }
