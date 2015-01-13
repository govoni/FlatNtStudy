export THISDIR=`pwd`
export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${THISDIR}/lib

if [ -n "${DYLD_LIBRARY_PATH}" ] ; then
export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${THISDIR}/lib
fi

export PATH=${PATH}:${THISDIR}/bin
export FlatNtStudy=${THISDIR}/

source /afs/cern.ch/sw/lcg/contrib/gcc/4.8.1/x86_64-slc6-gcc48-opt/setup.sh

if [ -f /data2/rgerosa/root/bin/thisroot.sh ] 
 then
    source /data2/rgerosa/root/bin/thisroot.sh 
else
    source /afs/cern.ch/sw/lcg/app/releases/ROOT/5.99.06/x86_64-slc6-gcc48-opt/root/bin/thisroot.sh
fi

