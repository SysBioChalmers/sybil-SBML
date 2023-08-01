# sybil-SBML

This SBML (Systems Biology Markup Language) integration provides SBML support for [sybil](https://github.com/SysBioChalmers/sybil).

If you're facing any problems, please open an [issue](issues/new) or a [discussion](/discusions/new).

### Installation

> The guide below has worked with R 4.1.2 on Linux Mint 21, which is based on Ubuntu 22.04 LTS.

Start by installing the dependencies. Follow the [installation instructions from libSBML](https://github.com/sbmlteam/libsbml#installation). Below is a shortcut of the instructions adapted to Ubuntu, thus might require some tweaking for your system.

Start by fetching the latest libSBML and R interface from the official location:
```bash
wget https://sourceforge.net/projects/sbml/files/libsbml/5.18.0/stable/Linux/64-bit/libSBML-5.18.0-Linux-x64.deb
wget https://sourceforge.net/projects/sbml/files/libsbml/5.18.0/stable/R%20interface/libSBML_5.18.0.tar.gz
```

Then install libSBML as a system library:
```bash
sudo apt install ./libSBML-5.18.0-Linux-x64.deb 
```

If the previous step failed, it might be that some libSBML dependencies (such as lixbml2) might need for the previous step to work:
```bash
sudo yum install libxml2-devel
```

Finally, proceed to install the R interface to libSBML (will take a while):
```bash
R CMD INSTALL libSBML_5.18.0.tar.gz
```

For various reasons, `sybil-SBML` is temporarily not available on CRAN anymore. Until then, it needs to be installed straight from this GitHub repository:

Use the following commands in the R console to install this package:
```R
install.packages("remotes")
remotes::install_github("SysBioChalmers/sybil-SBML")
```

If the above fails because of failing to load `libsbml.so`, try adding the path to this file before starting R:
```bash
LD_LIBRARY_PATH=/usr/lib64 R
```
If the fix worked, make this permanent by changing this variable in your `.bashrc` file.

Remove the installation files::
```bash
rm libSBML-5.18.0-Linux-x64.deb libSBML_5.18.0.tar.gz 
```


_Note_: This repository was previously hosted at https://gitlab.cs.uni-duesseldorf.de/general/ccb/sybilSBML .