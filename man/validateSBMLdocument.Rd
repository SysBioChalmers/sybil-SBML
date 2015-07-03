\name{validateSBMLdocument}
\alias{validateSBMLdocument}

\title{
  Validate SBML
}

\description{
  Validate files written in SBML.
}

\usage{
  validateSBMLdocument(sbmlf)
}

\arguments{
  \item{sbmlf}{
    A single character string containing a filename of an SBML file,
    or an object of class \code{\linkS4class{sbmlPtr}} as returned by
    \code{\link{openSBMLfile}}.
  }
}

\details{
  The given SBML file is validated by the libSBML function
  \code{checkConsistency}. Errors are retrieved by the libSBML function
  \code{getError} and returned as list.
}

\value{
  If argument \code{sbmlf} is an object of class \code{character},the
  function returns \code{TRUE}, if no infos, warnings, errors or fatals
  have been found. Otherwise a the value returned is a list.
  \item{infos }{Infos}
  \item{warnings }{Warnings}
  \item{errors }{Errors}
  \item{fatals }{Fatals}
  Each entry of one of these lists is again a list containing the message id,
  line and column number of the SBML file generating the message and the
  corresponding message text.

  If argument \code{sbmlf} is an object of class \code{\linkS4class{sbmlPtr}},
  the function returns the number of errors found in the SBML document.
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
  \code{\link{readSBMLmod}}, \code{\link{openSBMLfile}},
  \code{\link{getSBMLerrors}}, \code{\linkS4class{sbmlPtr}}
}

\examples{
  dir  <- system.file(package = "sybilSBML", "extdata")
  file <- file.path(dir, "ecoli_core_model.xml")
  err  <- validateSBMLdocument(file)
}

\keyword{ IO }
