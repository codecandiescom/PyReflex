import sys
import os

class Test(object):

    def __init__(self, name):
        self.name = name
        self.the_test = __import__(self.name)
        self.scram_arch = os.getenv('SCRAM_ARCH')
        
        #if len(argv) < 3:
        #    self._download = 1
        #    self._install = 1
        #    self._run = 1
        #    self._clean = 1
        #else:
        #    self._download = 'download' in sys.argv
        #    self._install = 'install' in sys.argv
        #    self._run = 'run' in sys.argv
        #    self._clean = 'clean' in sys.argv

        self.tmp_directory = '/tmp/pyreflex_clienttest/%s/%s' % (self.name, self.scram_arch)
        self.original_directory = os.getcwd()

    def get_actions(argv):
        kwds = {}
        for key in ('download', 'install', 'run', 'clean'):
            if key in argv and len(argv) > 1:
                kwds[key] = True
            else:
                kwds[key] = False
        return kwds
    get_actions = staticmethod(get_actions)

    def remove_temporary_directory(self):
        return 'rm -rf %(tmp_directory)s;' % self

    def make_temporary_directory(self):
        return 'mkdir -p %(tmp_directory)s;' % self

    def cd_temporary_directory(self):
        return 'cd %(tmp_directory)s;'% self

    def cd_original_directory(self):
        return 'cd %(original_directory)s;' % self

    def __getitem__(self, key):
        return getattr(self, key)

    def perform_actions(self, stream = sys.stdout,
                        download = True,
                        install  = True,
                        run      = True,
                        clean    = True,
                        cd_back  = False):
        print >> stream, self.make_temporary_directory()
        print >> stream, self.cd_temporary_directory()

        if download:
            print >> stream, self.the_test.download(self)

        if install:
            print >> stream, self.the_test.install(self)

        if run:
            print >> stream, self.the_test.run(self)
            #print >> stream, "export STATUS_CODE_of_%s=$?;" % self.name

        if cd_back:
            print >> stream, self.cd_original_directory()

        if clean:
            print >> stream, self.remove_temporary_directory()


    def script_location(self):
        return "%s/clienttests/%s_%s.sh" % (self.original_directory, self.name, self.scram_arch)


if len(sys.argv) == 1:
    namelist = ('gdml',)
    testlist = []

    for name in namelist:
        t = Test(name)
        # Clean must be false, otherwise the success of the cleaning
        # will be taken as the status code of the test
        t.perform_actions(open(t.script_location(), 'w'), download=True, install=True, run=True, clean=False)
        testlist.append(t)

    for test in testlist:
        print "sh %s;" % test.script_location()
        print "export STATUS_OF_%s=$?;" % (test.name)
        print test.remove_temporary_directory()

    print 'echo; echo; echo;'
    print 'echo "************************************************************************";'
    print 'echo;'

    for test in testlist:
        print '''
if [ $STATUS_OF_%s == 0 ]; then
    echo "PASS %s";
else
    echo ---- FAIL ------------ %s ;
fi;
''' % (test.name, test.name, test.name)

    print 'echo;'
    print 'echo "************************************************************************"; echo;'

        
else:        
    t = Test(sys.argv[1])
    t.perform_actions(cd_back=True, **Test.get_actions(sys.argv))

