[![Language Machines Badge](http://applejack.science.ru.nl/lamabadge.php/tscan)](http://applejack.science.ru.nl/languagemachines/)

========================================
T-scan
========================================

tscan 0.3 (c) TiCC/ 1998 - 2015

    Tilburg centre for Cognition and Communication, Tilburg University.
    UiL-OTS, Utrecht University
    Language Machines, Centre for Language Studies, Nijmegen

T-scan is distributed under the GNU Affero Public Licence (see the file COPYING).

T-scan is an analysis tool for dutch texts to assess the complexity of the
text, and is based on original work by Rogier Kraf (Utrecht University) [See:
Kraf et al., 2009]. The code has been reimplemented and extended by Ko van der
Sloot (Tilburg University), and is currently maintained and continued by
Martijn van der Klis (Utrecht University).

----------------
Documentation
----------------

Extensive documentation (in Dutch) can be found in docs/tscanhandleiding.pdf 
(downloadable via https://github.com/proycon/tscan/raw/master/docs/tscanhandleiding.pdf).

----------------
Installation 
----------------

T-scan heavily depends upon other sofware, such as Frog, Wopr and Alpino.

Installation is not trivial, to be able to succesfully build tscan from the tarball, you need the following packages:
- autotools
- autoconf-archive
- ticcutils
- libfolia
- Alpino 
- frog
- wopr
- CLAM

To facilitate installation, t-scan is included as an extra option in LaMachine (https://proycon.github.io/LaMachine)

We strongly recommend to use LaMachine (https://proycon.github.io/LaMachine) to
install tscan. In addition, t-scan also uses Alpino, which has to be obtained separately from
http://www.let.rug.nl/vannoord/alp/Alpino/.

To install LaMachine with t-scan you need to pass the ``tscan`` argument, as it is **not** included by default:

    $ git clone https://github.com/proycon/LaMachine
    $ ./LaMachine/virtualenv-bootstrap.sh tscan
    $ rm -Rf LaMachine    #repository is not needed anymore

If you do not want to use LaMachine, first make sure you have **all** necessary dependencies and then compile/install as follows:

    $ bash bootstrap.sh
    $ ./configure
    $ make
    $ sudo make install

----------------
Usage
----------------

If you use LaMachine as recommended, always activate the virtual environment first.
    
    $ . /path/to/lamachine/bin/activate

Before you can use t-scan you need to Start the background servers (you may need to edit the scripts to set ports and paths):

    $ cd tscan/webservices
    $ ./startfrog.sh
    $ ./startwopr.sh
    $ ./startalpino.sh

Then either run tscan from the command-line, which will produce a FoLiA XML file,

    $ cd tscan
    $ cp tscan.cfg.example tscan.cfg
    (edit tscan.cfg if necessary)
    $ tscan -c tscan.cfg input.txt 

.. or use the webapplication/webservice:

    $ cd tscan/webservices
    $ clamservice tscan   #this starts the CLAM service for TSCAN

And then navigate to the host and port specified. 

----------------
Data
----------------

Certain parts of T-scan use data from Referentiebestand Nederlands, which we can not distribute due to restrictive licensing issues, so this functionality will not be available.

Certain other data is too large for github, but will be downloaded for you automatically by the ``./downloaddata.sh`` script.

----------------
References
----------------

Kraf, R. & Pander Maat, H. (2009) - Leesbaarheidsonderzoek: oude problemen en nieuwe kansen. Tijdschrift voor Taalbeheersing 31(2). 97-123
Pander Maat, H. & Kraf, R. & van den Bosch, A. & Dekker, N. & van Gompel, M. & Kleijn, S. & Sanders, T. & van der Sloot, K. (2014) - T-Scan: a new tool for analyzing Dutch text  .  Computational Linguistics in the Netherlands 12/2014; 4:53-74. 



