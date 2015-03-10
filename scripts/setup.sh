export THISDIR=`pwd`

if [ -n "${DYLD_LIBRARY_PATH}" ] ; then
    unset DYLD_LIBRARY_PATH
fi

if [ -z "$CMSSW_BASE" ] ; then
    eval `scramv1 runtime -sh`
fi

unset LD_LIBRARY_PATH
unset FlatNtStudy 

if [ -z "$CMSSW_BASE" ] ; then
    eval `scramv1 runtime -sh`
else
    eval `scramv1 runtime -sh`
fi

export LD_LIBRARY_PATH=${LD_LIBRARY_PATH}:${THISDIR}/lib

if [ -n "${DYLD_LIBRARY_PATH}" ] ; then
export DYLD_LIBRARY_PATH=${DYLD_LIBRARY_PATH}:${THISDIR}/lib
fi

export PATH=${PATH}:${THISDIR}/bin
export FlatNtStudy=${THISDIR}/
export ROOFITSYS=${ROOTSYS}/roofit


if [ -f /data2/rgerosa/root/bin/thisroot.sh ] 
 then
    source /afs/cern.ch/sw/lcg/contrib/gcc/4.8.1/x86_64-slc6-gcc48-opt/setup.sh
    source /data2/rgerosa/root/bin/thisroot.sh 
else
    source /afs/cern.ch/sw/lcg/contrib/gcc/4.8.1/x86_64-slc6-gcc48-opt/setup.sh
    source /afs/cern.ch/sw/lcg/app/releases/ROOT/6.03.02/x86_64-slc6-gcc48-opt/root/bin/thisroot.sh
fi