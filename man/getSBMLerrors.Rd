\name{getSBMLerrors}
\alias{getSBMLerrors}

\title{
  Get SBML Errors
}

\description{
  Retrieve list of errors in a SBML file.
}

\usage{
  getSBMLerrors(sbmlf)
}

\arguments{
  \item{sbmlf}{
    An object of class \code{\linkS4class{sbmlPtr}} as returned by
    \code{\link{openSBMLfile}}. This is basically a pointer to a SBML document.
  }
}

\value{
  If the SBML document contains errors, a \code{list} is returned:
  \item{infos}{
    a list of infos.
  }
  \item{warnings}{
    a list of warnings.
  }
  \item{errors}{
    a list of errors.
  }
  \item{fatals}{
    a list of fatals.
  }
  Each elements of the lists above, is again a list:
  \describe{
    \item{id}{
      A single integer value containing the error id.
    }
    \item{line}{
      A single integer value containing the row number of the SBML file
      producing the error.
    }
    \item{column}{
      A single integer value containing the column number of the SBML file
      producing the error.
    }
    \item{message}{
      A single character string containing the error message.
    }
  }
  If the SBML document does not contain errors, \code{TRUE} is returned.
}

\references{
  Bornstein, B. J., Keating, S. M., Jouraku, A., and Hucka M. (2008)
  LibSBML: An API Library for SBML. \emph{Bioinformatics} \bold{24},
  880--881.
}

\author{Gabriel Gelius-Dietrich}

\seealso{
  \code{\link{openSBMLfile}}, \code{\link{validateSBMLdocument}},
  \code{\linkS4class{sbmlPtr}}
}

\keyword{ IO }
