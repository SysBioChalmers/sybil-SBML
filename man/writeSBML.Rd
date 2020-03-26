\name{writeSBML}
\alias{writeSBML}

\encoding{utf8}

\title{Exports a Metabolic Network in SBML Format}

\description{
  The function exports a metabolic network from S4 object of the class \code{\link{modelorg}} in SBML format. The function returns TRUE if the SBML file is successfully built.
}

\usage{
  writeSBML(morg=NULL,level=2,version=4,fbcLevel=0,
             filename="export.xml",
             recoverExtMet=FALSE,
             printNotes=TRUE,
             printAnnos=TRUE,
             validation=FALSE)

}

\arguments{
  \item{morg}{
    An S4 object of the class \code{\link[sybil]{modelorg}}.
  }
  \item{level}{
    A single integer value containing the SBML level for the exporting SBML file. \cr
    Default: \code{2}.
  }
   \item{version}{
    A single integer value containing the SBML version for the exporting SBML file. \cr
    Default: \code{4}.
  }
  \item{fbcLevel}{
    A single integer value containing the fbc package version for the exporting SBML file. \cr
    Default: \code{2}.
  }
   \item{filename}{
    SBML filename for exporting the model.
    Default: \code{"export.xml"}.
  }
  \item{recoverExtMet}{
    Boolean: recover external metabolites and refer them to compartment \code{"BOUNDARY"}.\cr 
    Default: \code{FALSE}.
  }
  \item{printNotes}{
    Boolean: print Notes from original SBML file .\cr
    Default: \code{TRUE}.
  }
  \item{printAnnos}{
    Boolean: print Annotations from original SBML file .\cr
    Default: \code{TRUE}.
  }
  \item{validation}{
    Boolean: print containing errors for xml file .\cr
    Default: \code{TRUE}.
  }

}

\details{
  The library \code{libSBML} is used to export a \code{\link[sybil]{modelorg}}
  to a SBML file.
  
  If sybilSBML was built with a libSBML version, that was not including the FBC plugin
  and the Groups plugin, the function is not writing an output file and returns FALSE.
  Please make sure that you build sybilSBML with the libSBML version containing the
  FBC and Group plugins (check out installation details).
}

\value{
  A single boolean value for a successful export.
}

\references{
  The BiGG database \url{http://bigg.ucsd.edu/}.

  Bornstein, B. J., Keating, S. M., Jouraku, A., and Hucka M. (2008)
  LibSBML: An API Library for SBML. \emph{Bioinformatics} \bold{24},
  880--881.

  Schellenberger, J., Park, J. O., Conrad, T. C., and Palsson, B. Ø., (2010)
  BiGG: a Biochemical Genetic and Genomic knowledgebase of large scale metabolic
  reconstructions. \emph{BMC Bioinformatics} \bold{11}, 213.

  Becker, S. A., Feist, A. M., Mo, M. L., Hannum, G., Palsson, B. Ø.  and
  Herrgard, M. J. (2007) Quantitative prediction of cellular metabolism with
  constraint-based models: the COBRA Toolbox. \emph{Nat Protoc} \bold{2},
  727--738.

  Schellenberger, J., Que, R., Fleming, R. M. T., Thiele, I., Orth, J. D.,
  Feist, A. M., Zielinski, D. C., Bordbar, A., Lewis, N. E., Rahmanian, S.,
  Kang, J., Hyduke, D. R. and Palsson, B. Ø. (2011) Quantitative prediction of
  cellular metabolism with constraint-based models: the COBRA Toolbox v2.0.
  \emph{Nat Protoc} \bold{6}, 1290--1307.
}

\author{
  Gabriel Gelius-Dietrich <geliudie@uni-duesseldorf.de>

  Maintainer: Mayo Roettger <mayo.roettger@hhu.de>
}


\note{
  The function \code{writeSBML} makes use of the library
  \code{libSBML} (\url{http://www.sbml.org}).
}

\seealso{
  \code{\link{readSBMLmod}}
}

\examples{
  dir  <- system.file(package = "sybilSBML", "extdata")
  file <- file.path(dir, "ecoli_core_model.xml")
  mod  <- readSBMLmod(file, bndCond = FALSE)
  # write SBML file to tempdir():
  writeSBML(mod,level=3,version=1,fbcLevel=2,filename=file.path(tempdir(), "export.xml"))
}

\keyword{ IO }

