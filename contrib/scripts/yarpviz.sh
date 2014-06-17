#/bin/bash

# change the following as you want
YARP_CMD="yarp"
GRAPH_GEN="dot"
GRAPH_FORMAT="pdf"
FILENAME="output"
#----------------------------------


GRAPH_FILENAME="${FILENAME}.dot"
GRAPH_OUTPOUT="${FILENAME}.${GRAPH_FORMAT}"

# recreate the file
rm -f ${GRAPH_FILENAME}
touch ${GRAPH_FILENAME}

echo "digraph \"\" {
  graph [ranksep=\"1.0\", nodesp=\"0.5\", rankdir=\"LR\", overlap=\"false\", packmode=\"graph\", fontname=\"Arial\", fontsize=\"12\", concentrate=\"true\", bgcolor=\"#FFFFFF\"];
  node [style=\"filled\", color=\"blue\", fillcolor=\"#F0F0F0\", label=\"\", sides=\"4\", fontcolor=\"black\", fontname=\"Arial\", fontsize=\"12\", shape=\"ellipse\"];
  edge [color=\"#CC0044\", label=\"\", fontname=\"Arial\", fontsize=\"8\", fontcolor=\"#555555\"];
  ">> ${GRAPH_FILENAME}


${YARP_CMD} clean --timeout 0.5 > /dev/null 
namespace=`${YARP_CMD} namespace  | awk -F"YARP namespace: " '{print $2}'`
ports_list=`${YARP_CMD} name list | awk -F"registration name " '{print $2}' | awk -F" ip " '{print $1}' | grep "/"`;

declare -A ports_map
declare -A nodes
declare -A links

i=0;
for port in ${ports_list} ; do  
     if [ -z `echo $port | grep tmp` ] ; then
        ports_map["${port}"]="N__${i}"
     fi        
    i=$((i+1));
done

# adding the connections
i=0;
for port in "${!ports_map[@]}" ; do
    echo "Checking [$port] ..."
    output=`${YARP_CMD} ping $port | grep "output connection"`;
    for tmp in $output ; do
        to=`echo $tmp | awk -F"\"" '{print $2}'`;
        if [ ! -z "$to" -a "$to" != "$port" ]; then
            if [ "${ports_map[$to]}" != "" ]; then
                nodes["$port"]="${ports_map[$port]} [label=\"$port\"];";
                nodes["$to"]="${ports_map[$to]} [label=\"$to\"];";
                links["$i"]="${ports_map[$port]} -> ${ports_map[$to]}";
                i=$((i+1));
            fi
        fi
    done
done

# writing nodes
for index in "${!nodes[@]}" ; do
    echo ${nodes[$index]} >> ${GRAPH_FILENAME}
done

#writing links
for index in "${!links[@]}" ; do
    echo ${links[$index]} >> ${GRAPH_FILENAME}
done

echo "}" >> ${GRAPH_FILENAME}

# rendering graph
echo "rendering graph..."
${GRAPH_GEN} -T${GRAPH_FORMAT} -o ${GRAPH_OUTPOUT} ${GRAPH_FILENAME}

