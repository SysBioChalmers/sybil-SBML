\name{sbmlPtr-class}

\Rdversion{1.1}
\docType{class}

\alias{sbmlPtr-class}

\alias{isNULLpointerSBML,sbmlPtr-method}
\alias{isSBMLdocpointer,sbmlPtr-method}
\alias{isSBMLmodpointer,sbmlPtr-method}
\alias{sbmlDocKey,sbmlPtr-method}
\alias{sbmlFileName,sbmlPtr-method}
\alias{sbmlPointer,sbmlPtr-method}
\alias{sbmlPtrType,sbmlPtr-method}
\alias{isNULLpointerSBML}
\alias{isSBMLdocpointer}
\alias{isSBMLmodpointer}
\alias{sbmlDocKey}
\alias{sbmlFileName}
\alias{sbmlPointer}
\alias{sbmlPtrType}
\alias{sbmlDocPointer}
\alias{sbmlModPointer}

\title{Class \code{"sbmlPtr"}}

\description{
  Containing SBML pointer objects.
}

\section{Objects from the Class}{
  Objects can be created by calls of the form
  
  \code{doc <- sbmlDocPointer(pointer)}.
  \describe{
    \item{\code{pointer}:}{
      an object of class \code{sbml_doc_ptr} containing a pointer to a SBML
      document.
    }
  }
  \code{doc <- sbmlModPointer(pointer, sbmlDoc)}.
  \describe{
    \item{\code{pointer}:}{
      an object of class \code{sbml_mod_ptr} containing a pointer to a SBML
      model.
    }
    \item{\code{sbmlDoc}:}{
      an object of class \code{sbmlPtr} containing a pointer to a SBML
      document (returned by \code{sbmlDocPointer}).
    }
  }

  This constructor functions are only used internally, objects of class
  \code{sbmlPtr} are returned by the functions \code{\link{openSBMLfile}}
  and \code{\link{getSBMLmodel}}.
}

\section{Slots}{
  \describe{
    \item{\code{sbmlPtrType}:}{Object of class \code{"character"}
      containing the type of the pointer.
    }
    \item{\code{sbmlPointer}:}{Object of class \code{"externalptr"}
      containing a pointer to a SBML document or model.
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
    \item{isNULLpointerSBML}{\code{signature(object = "sbmlPtr")}:
      returns \code{TRUE} if \code{sbmlPointer(object)} is a NULL pointer,
      otherwise \code{FALSE}.
    }
    \item{isSBMLdocpointer}{\code{signature(object = "sbmlPtr")}:
      returns \code{TRUE} if \code{sbmlPointer(object)} is a pointer to a
      SBML document, otherwise \code{FALSE}.
    }
    \item{isSBMLmodpointer}{\code{signature(object = "sbmlPtr")}:
      returns \code{TRUE} if \code{sbmlPointer(object)} is a pointer to a
      SBML model, otherwise \code{FALSE}.
    }
    \item{sbmlDocKey}{\code{signature(object = "sbmlPtr")}:
        gets the \code{sbmlDocKey} slot.
    }
    \item{sbmlFileName}{\code{signature(object = "sbmlPtr")}:
        gets the \code{sbmlFileName} slot.
    }
    \item{sbmlPointer}{\code{signature(object = "sbmlPtr")}:
        gets the \code{sbmlPointer} slot.
    }
    \item{sbmlPtrType}{\code{signature(object = "sbmlPtr")}:
        gets the \code{sbmlPtrType} slot.
    }
  }
}

\author{Gabriel Gelius-Dietrich}

\seealso{
  \code{\link{openSBMLfile}}, \code{\link{getSBMLmodel}}
}

\examples{
  showClass("sbmlError")
}

\keyword{classes}

