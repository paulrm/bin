#!/bin/bash
#Desc: Muestra tareas usuales

SELF=$( basename $0 )
SELF_LONG=$0
SELF_VERSION="0.01"
SELF_AUTHOR="Paul Messina"

TEMP=`getopt -o vh1dm: \
             --long version,help,verbose,debug,memory:,debugfile:,minheap:,maxheap: \
             -n 'h' -- "$@"`

if [ $? != 0 ] ; then echo "Terminating..." >&2 ; exit 1 ; fi

# Note the quotes around `$TEMP': they are essential!
eval set -- "$TEMP"

VERBOSE=false
DEBUG=false
MEMORY=
DEBUGFILE=
JAVA_MISC_OPT=

HELP=$(cat <<End-of-Help
Usage: h [-options] [--help] [--version]
where options include:
    -h            lista los hosts
    -d64          use a 64-bit data model if available
    -server       to select the "server" VM
                  The default VM is server,
                  because you are running on a server-class machine.
End-of-Help
)

##
# Loop Opt
##
while true; do
  case "$1" in
    --version ) VERBOSE=true; shift ;;
    --help ) 
        echo "$HELP"; 
        exit 
        ;;
    -v | --verbose ) VERBOSE=true; shift ;;
    -d | --debug ) DEBUG=true; shift ;;
    -m | --memory ) MEMORY="$2"; shift 2 ;;
    --debugfile ) DEBUGFILE="$2"; shift 2 ;;
    --minheap )
      JAVA_MISC_OPT="$JAVA_MISC_OPT -XX:MinHeapFreeRatio=$2"; shift 2 ;;
    --maxheap )
      JAVA_MISC_OPT="$JAVA_MISC_OPT -XX:MaxHeapFreeRatio=$2"; shift 2 ;;
    -- ) shift; break ;;
    * ) break ;;
  esac
done

##
# Verifico condiciones
##
strip="sed -e /^$/d; -e /^#/d; -e /^\s*$/d;"

function default() {

cat <<End-of-message
-Help--------------------------------
git status                                | muestra estado de los fies
git commit -a                             | commit
git pull origin master                    | pull desde el origen aqui
git push origin master                    | push local repo al origen
git log -1                                | muestra el ultimo commit
git config --global credential.helper 'cache --timeout=604800' # Semana
manifest                                  | lista los files y dirs identificados, y muestra metricas de cuantos faltan
ack --ignore-file=match:/h/ ToDo          | lista los temas pendientes de hacer
find -name '.git*' -prune -o -print       | lista files (sin .git)
s3 list                                   | lista todos los s3 buckets
s3cmd ls [s3://bucketname]                | lista los objetos
s3cmd ls --list-md5   [s3://bucketname]   | lista los objetos + md5
check                                     | Ejecuta el set de test
cmds                                      | Lista los comando disponibles
2do                                       | Conecta con Todoist
time bonnie++ -d /tmp -s 20M -m ctrl-dev -r 10  -x 1 -u root:root
time ansible all -o -m ping               | test ansible
time salt '*' test.ping                   | test salt (desde malbec salt-master)
aws s3 ls glia.sao.paulo
aws s3api list-objects --no-verify-ssl   --bucket glia.sao.paulo
cmd <- test de bsfl
@ <- otro test
-Otros--
mongo "mongodb://\$CLUSTER_MONGO/test?replicaSet=Cluster0-shard-0" --authenticationDatabase admin --ssl --username paul --password moscow80
aws route53 list-hosted-zones --no-verify-ssl  --output text

------------------------------------
End-of-message
}


function page1() {
cat <<End-of-message
-------------------------------------
service --status-all  | egrep "postgresql|nginx|circusd|rabbitmq-server"
dpkg --list > ~/etc/packages.lst
ack --type-set='all:match:.*' -k patt
-------------------------------------
End-of-message
}
function listHosts() {
   if [ -f /etc/ansible/hosts ] 
   then
       echo ":::: Ansible ::::"
       cat /etc/ansible/hosts | $strip
   fi
   if [ -f ~/dev/etc/hosts.txt ]
   then
       echo ":::: Others ::::"
       cat ~/dev/etc/hosts.txt | $strip
   fi
}

while getopts ":a:hc1" opt; do

echo "#opt $opt"
  case $opt in
    a)
      echo "-a was triggered, Parameter: $OPTARG" >&2
      echo $OPTARG >> ~/dev/etc/cmds.txt
      sort -u ~/dev/etc/cmds.txt -o ~/dev/etc/cmds.txt
      ;;
    1)
      page1
      ;;
    c)
      cat ~/dev/etc/cmds.txt
      ;;
    \?)
      echo "Invalid option: -$OPTARG" >&2
      exit 1
      ;;
    h)
      #echo "-$opt was triggered, Parameter: $OPTARG" >&2
      listHosts
      ;;
    :)
      echo "Option -$OPTARG requires an argument." >&2
      exit 1
      ;;
  esac
done

if [ $# -eq 0 ]
then
    default
fi
