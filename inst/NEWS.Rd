%% sybilSBML Version History

\name{NEWS}
\title{sybilSBML News}
\encoding{UTF-8}

\newcommand{\CRANpkg}{\href{https://cran.r-project.org/package=#1}{\pkg{#1}}}

% ---------------------------------------------------------------------------- %
\section{Changes in version 3.0.1 2016-12-16}{
  \itemize{
    \item sybilSBML now supports writing of SBML files.
    \item The new FBC additional package for SBML is now fully integrated.
    \item Urls in NEWS file are now in default style.
  }
}

% ---------------------------------------------------------------------------- %
\section{Changes in version 2.0.11 2015-07-03}{
  \itemize{
    \item \code{gprRules}-Slot in \code{modelorg} has changed. Thus 
    \code{readSBMLmod} had to be updated.
  }
}

% ---------------------------------------------------------------------------- %
\section{Changes in version 2.0.9 2015-07-03}{
  \itemize{
    \item Changed Maintainer.
  }
}

% ---------------------------------------------------------------------------- %
\section{Changes in version 2.0.8 2014-05-21}{
  \itemize{
    \item function \code{openSBMLfile} does a \code{normalizePath} on its
          argument \code{fname}
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 2.0.7 2013-11-27}{
  \itemize{
    \item added file \code{CITATION}
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 2.0.6 2013-10-26}{
  \itemize{
    \item a minor bugfix in file \file{configure.ac}
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 2.0.5 2013-09-08}{
  \itemize{
    \item function \code{readSBMLmod} returns an error if the input file does
          not contain a -- or contains an empty -- \code{listOfReactions},
          \code{listOfSpecies} or \code{listOfCompartments} section.
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 2.0.4 2013-08-01}{
  \itemize{
    \item function \code{getSBMLCompartList()} returns also the compartment
          names, if given
    \item fixed a bug in file \file{configure.ac} which occurs when
          \code{pkg-config} is not used
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 2.0.3 2013-05-30}{
  \itemize{
    \item some improvements in \file{Makevars.win} suggested by Brian D. Ripley
          and Uwe Ligges
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 2.0.2 2013-05-28}{
  \itemize{
    \item removed \CRANpkg{glpkAPI} from Suggests in the \file{DESCRIPTION} file
    \item some small changes in package vignette
    \item some changes in \file{Makevars.win}, so that precompiled libSBML
          binaries will be packed (for WIndows)
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 2.0.1 2013-05-14}{
  \itemize{
    \item added \CRANpkg{glpkAPI} to Suggests in the \file{DESCRIPTION} file
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 2.0.0 2013-05-14}{
  \itemize{
    \item changed argument \code{checkrsbml} to \code{validateSBML} in
          function \code{readSBMLmod()} and set its default value to
          \code{FALSE}
    \item added new function \code{validateSBMLdocument()}
    \item installation of \pkg{sybilSBML} requires libSBML version 5.6.0 or
          higher, \pkg{rsbml} is no longer required
    \item added functions
          \code{versionLibSBML()},
          \code{openSBMLfile()},
          \code{closeSBMLfile()},
          \code{getSBMLmodel()},
          \code{delSBMLmodel()},
          \code{getSBMLlevel()},
          \code{getSBMLversion()},
          \code{validateSBMLdocument()},
          \code{getSBMLerrors()},
          \code{getSBMLmodId()},
          \code{getSBMLmodName()},
          \code{getSBMLnumCompart()},
          \code{getSBMLnumSpecies()},
          \code{getSBMLnumReactions()},
          \code{getSBMLunitDefinitionsList()},
          \code{getSBMLCompartList()},
          \code{getSBMLSpeciesList()} and
          \code{getSBMLReactionsList()}
          as low-level interface funcitons to the C-API of libSBML
    \item added classes \code{sbmlPtr} and \code{sbmlError}
          
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.16 2013-01-16}{
  \itemize{
    \item removed old file \file{NEWS.Rd} from top level directory
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.15 2012-10-15}{
  \itemize{
    \item updated package vignette
    \item slot \code{rhs} of class \code{modelorg} is not set
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.14 2012-04-20}{
  \itemize{
    \item file \file{NEWS} is now in \file{inst/NEWS.Rd}
    \item removed arguments \code{verboseMode}, \code{loglevel}, \code{logfile}
          and \code{logfileEnc} from function \code{readSBMLmod}; use function
          \code{sink} instead
    \item fixed a bug in \code{.uglyHack}: tries no longer to fix SBMLWarnings,
          which is not necessary
    \item \code{.uglyHack} uses \code{tempfile()} to generate corrected
          SBML files 
    \item fixed a bug in \code{readSBMLmod()}: slot \code{subSys} was not of
          correct data type in rare cases
    \item better SBML id formatting in \code{readSBMLmod()}
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.13 2012-03-16}{
  \itemize{
    \item compatible with \CRANpkg{sybil} version 0.5.12: replaced
          \CRANpkg{SparseM} with \CRANpkg{Matrix}

    \item some additions to \code{.uglyHack()}

    \item fixed a small bug in \code{readSBMLmod()}, when parsing sub systems
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.12 2011-12-12}{
  \itemize{
    \item fixed a small bug in \code{readSBMLmod()} concerning the detection of
          gpr rules

    \item fixed a bug in \code{readSBMLmod()}: genes from reactions which
          are excluded from the model are not read
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.11 2011-12-09}{
  \itemize{
    \item very minor adjustments in \code{readSBMLmod()}

    \item \code{readSBMLmod()} will continue reading the model,
          if \pkg{rsbml} produces warnings. Check warnings in the log file;
          arguments \code{loglevel} and \code{logfile} in \code{readSBMLmod()}
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.10 2011-08-30}{
  \itemize{
    \item updated documentation
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.9 2011-08-30}{
  \itemize{
    \item added boolean arguments \code{remMet}, \code{contrMet},
          \code{singletonMet} and \code{deadEndmet} to \code{readSBMLmod()},
          the two boolen arguments \code{remSingleMet} and
          \code{constrSingleMet} are removed, see \code{help("readSBMLmod")}
          for details
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.8 2011-08-15}{
  \itemize{
    \item some small changes according to \CRANpkg{codetools}

    \item slots \code{met_single} and \code{react_single} of class
          \code{modelorg} are initialized with \code{NA}, not \code{FALSE}
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.7 2011-08-12}{
  \itemize{
    \item new arguments for \code{readSBMLmod()}:
          \code{mergeMet}, \code{balanceReact}, \code{remUnusedMetReact},
          \code{remSingleMet}, \code{constrSingleMet} and \code{tol}.
          See \code{help("readSBMLmod")} for details
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.6 2011-08-08}{
  \itemize{
    \item a warning is produced, if a metabolite is used only once in S
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.5 2011-08-08}{
  \itemize{
    \item \code{readSBMLmod()} removes unused metabolites and reactions
          (rows and lines without any non-zero entries)

    \item \code{readSBMLmod()} merges and balances metabolites as in
          \code{readTSVmod()} from \CRANpkg{sybil}
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.4 2011-08-05}{
  \itemize{
    \item fixed a bug in \code{readSBMLmod()} concerning the number of
          metabolite names and metabolite compartments
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.3 2011-08-05}{
  \itemize{
    \item added \code{NAMESPACE}

    \item new boolean parameter \code{bndCnd} in \code{readSBMLmod()}:
          If set to \code{TRUE}, external metabolites are those,
          having the SBML tag \code{boundaryCondition} set to \code{TRUE}.
          If set to \code{FALSE}, external metabolites are recogized via
          \code{extMetFlag}

    \item usage of class \code{sybilLog} in \code{readSBMLmod()} with four new
          parameters. See \code{help("readSBMLmod")} for details.
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.2 2011-07-20}{
  \itemize{
    \item fixed a bug in \code{readSBMLmod()}:
          if an external metabolite id is removed and it occcurs in
          a reaction, that particular reaction is read correctly.
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.1 2011-07-06}{
  \itemize{
    \item new boolean parameter \code{ignoreNoAn} in \code{readSBMLmod()}:
          if set to \code{TRUE}, any notes and annotation fields in the
          \code{listOfReactions} in an SBML file are ignored
          (default: \code{TRUE}).

    \item unused metabolites having no non-zero entry in S, are removed from S
          and the metabolites list in instances of class \code{modelorg}
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.1.0 2011-05-17}{
  \itemize{
    \item updated documentation
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.0.3 2011-05-11}{
  \itemize{
    \item added compatibility to \CRANpkg{sybil} version \eqn{\geq}{>=} 0.4.1:
          slot \code{subSys} is now of class \code{matrix}

    \item fixed a bug in \code{readSBMLmod()}: metablite id's differing
          only in the number of consecutive '\code{_}' are now recogized
          as different metabolite id's.

    \item \code{readSBMLmod()} can now read the Yeast model in the file
          \file{Sc_iND750_GlcMM.xml} from the Palsson group.
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.0.2 2011-03-02}{
  \itemize{
    \item If the SBML file contains reactions with metabolites used as
          reactands and products at the same time, the stoichiometric
          coefficient for those metabolites is the sum of reactand and product.
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.0.1 2011-03-01}{
  \itemize{
    \item fixed some regular expressions when parsing metabolite names and id's

    \item added argument \code{extMetFlag} which is used to identify external
          metabolites

    \item reads subsystem information also from annotations
  }
}


% ---------------------------------------------------------------------------- %
\section{Changes in version 1.0.0 2011-01-25}{
  \itemize{
    \item initial public release
  }
}


