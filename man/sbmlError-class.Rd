\name{sbmlError-class}

\Rdversion{1.1}
\docType{class}

\alias{sbmlError-class}
\alias{sbmlError}

\alias{getNumErrors,sbmlError-method}
\alias{length,sbmlError-method}
\alias{printSlot,sbmlError,character-method}
\alias{sbmlDocKey,sbmlError-method}
\alias{sbmlErrors,sbmlError-method}
\alias{sbmlFatals,sbmlError-method}
\alias{sbmlFileName,sbmlError-method}
\alias{sbmlInfos,sbmlError-method}
\alias{sbmlWarnings,sbmlError-method}
\alias{getNumErrors}
\alias{printSlot}
\alias{sbmlErrors}
\alias{sbmlFatals}
\alias{sbmlInfos}
\alias{sbmlWarnings}

\title{Class \code{"sbmlError"}}

\description{
  Class to administrate Infos, Warnings, Errors and Fatals returned after
  validation of an SBML file.
}

\section{Objects from the Class}{
  Objects can be created by calls of the form
  
  \code{err <- sbmlError(err, sbmlf)}.
  \describe{
    \item{\code{err}:}{
      an object of class \code{sbml_error} containing a list of SBML errors.
    }
    \item{\code{sbmlf}:}{
      an object of class \code{\linkS4class{sbmlPtr}} containing a pointer to
      an SBML document object.
    }
  }
  This constructor function is only used internally, objects of class
  \code{sbmlError} are returned by the functions
  \code{\link{validateSBMLdocument}} and \code{\link{getSBMLerrors}}.
}

\section{Slots}{
  \describe{
    \item{\code{sbmlInfos}:}{
      Object of class \code{"list"} containing all Infos.
     }
    \item{\code{sbmlWarnings}:}{
      Object of class \code{"list"} containing all Warnings.
    }
    \item{\code{sbmlErrors}:}{
      Object of class \code{"list"} containing all Errors.
    }
    \item{\code{sbmlFatals}:}{
      Object of class \code{"list"} containing all Fatals.
    }
    \item{\code{sbmlFileName}:}{
      Object of class \code{"character"} containing the file name of the SBML
      file generating the errors.
    }
    \item{\code{sbmlDocKey}:}{
      Object of class \code{"character"} containing a single character string
      functioning as a unique key to a SBML document object.
    }
  }
}

\section{Methods}{
  \describe{
    \item{getNumErrors}{\code{signature(object = "sbmlError")}:
      returns a vector of length 5 containing the number of Infos, Warnings,
      Errors, Fatals and the total number of entries.
    }
    \item{length}{\code{signature(x = "sbmlError")}:
      returns the total number of entries.
    }
    \item{printSlot}{\code{signature(object = "sbmlError", ws = "character")}:
      prints the slot given in argument \code{ws}. It can be \code{"Infos"},
      \code{"Warnings"}, \code{"Errors"} or \code{"Fatals"}.
    }
    \item{sbmlDocKey}{\code{signature(object = "sbmlError")}:
      gets the \code{sbmlDocKey} slot.
    }
    \item{sbmlErrors}{\code{signature(object = "sbmlError")}:
      gets the \code{sbmlErrors} slot.
    }
    \item{sbmlFatals}{\code{signature(object = "sbmlError")}:
      gets the \code{sbmlFatals} slot.
    }
    \item{sbmlFileName}{\code{signature(object = "sbmlError")}:
      gets the \code{sbmlFileName} slot.
    }
    \item{sbmlInfos}{\code{signature(object = "sbmlError")}:
      gets the \code{sbmlInfos} slot.
    }
    \item{sbmlWarnings}{\code{signature(object = "sbmlError")}:
      gets the \code{sbmlWarnings} slot.
    }
  }
}

\author{
  Gabriel Gelius-Dietrich <geliudie@uni-duesseldorf.de>

  Maintainer: Claus Jonathan Fritzemeier <clausjonathan.fritzemeier@uni-duesseldorf.de>
}


\seealso{
  \code{\link{validateSBMLdocument}}, \code{\link{getSBMLerrors}}
}

\examples{
  showClass("sbmlError")
}

\keyword{classes}



