\name{openSBMLfile}
\alias{openSBMLfile}

\title{
  Open SBML File
}

\description{
  Retrieve a pointer to a SBML document.
}

\usage{
  openSBMLfile(fname, ptrtype = "sbml_doc")
}

\arguments{
  \item{fname}{
    A single character string containing a file name of an SBML file.
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
  \code{\link{closeSBMLfile}}, \code{\linkS4class{sbmlPtr}}
}

\keyword{ IO }
