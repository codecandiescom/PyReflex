# Should add the possibility to specify a tag
import os, commands


############################## download ##############################
def download(T):
    return """
cvs -d:pserver:anonymous@simu.cvs.cern.ch:/cvs/simu co simu/GDML2/PyGDML;
"""



############################## install ##############################

def install(parameters):
    scram_arch = os.getenv('SCRAM_ARCH')
    if scram_arch.endswith('_dbg'):
        parameters.platform = scram_arch[:-4]
        parameters.dbg = 'export DBG=1;'
    else:
        parameters.platform = scram_arch
        parameters.dbg = ''
    tmp_directory = parameters['tmp_directory']
    parameters.sealbase = commands.getoutput('scram b echo_LOCALTOP').split()[-1]

    return """
export PLATFORM=%(platform)s;
%(dbg)s
cd %(tmp_directory)s/simu/GDML2/PyGDML/G4Binding;
export SEALBASE=%(sealbase)s
. g4setup.sh;
make;
""" % parameters


############################## run ##############################
def run(parameters):
    tmp_directory = parameters['tmp_directory']
    return """
cd %(tmp_directory)s/simu/GDML2/PyGDML/G4Binding/python;
python GDMLG4.py;
""" % parameters
