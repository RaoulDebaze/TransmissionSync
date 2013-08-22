#!/bin/sh

# Generate files to be included: only overwrite them if changed so make
# won't rebuild everything unless necessary
replace_if_differs ()
{
    if cmp $1 $2 > /dev/null 2>&1; then
      rm -f $1
    else
      mv -f $1 $2
    fi
}

echo "creating sibsync/version.h"

user_agent_prefix=`grep m4_define configure.ac | sed "s/[][)(]/,/g" | grep user_agent_prefix  | cut -d , -f 6`

peer_id_prefix=`grep m4_define configure.ac | sed "s/[][)(]/,/g" | grep peer_id_prefix  | cut -d , -f 6`

major_version=`echo ${user_agent_prefix} | awk -F . '{print $1}'`
minor_version=`echo ${user_agent_prefix} | awk -F . '{print $2 + 0}'`

cat > sibsync/version.h.new << EOF
#define PEERID_PREFIX             "${peer_id_prefix}"
#define USERAGENT_PREFIX          "${user_agent_prefix}"
#define SHORT_VERSION_STRING      "${user_agent_prefix}"
#define LONG_VERSION_STRING       "${user_agent_prefix}"
#define VERSION_STRING_INFOPLIST  ${user_agent_prefix}
#define MAJOR_VERSION             ${major_version}
#define MINOR_VERSION             ${minor_version}
EOF

# Add a release definition
case "${peer_id_prefix}" in
    *X-) echo '#define TR_BETA_RELEASE           1' ;;
    *Z-) echo '#define TR_NIGHTLY_RELEASE        1' ;;
    *)   echo '#define TR_STABLE_RELEASE         1' ;;
esac >> "sibsync/version.h.new"

replace_if_differs sibsync/version.h.new sibsync/version.h
