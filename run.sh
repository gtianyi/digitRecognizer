#!/bin/bash
# author: Tianyi Gu

size="5"
alg="q"
gamma="0.99"
exps="500"
eps="500"
epsilon="0.1"
alpha="0.1"
lambda="0"
figu="0"

function usage {
   cat << EOF
Usage:
-size <world size> default:5
-alg <alg> either ‘q’ or ‘s’, to determine the algorithm used, default:q
-gamma <discount factor> default:0.99 
-exps <expriments amount> default:500
-eps <episodes amount> default:500
-epsilon <the epsilon used in e-greedy> default:0.1
-alpha <learning rate> default:0.1 
-lambda <parameter for Sarsa(lambda)> default:0
-fig <if generate figure, either 0 or 1> default:0
EOF
   exit 1
}


while :; do
    case $1 in
        -size)
	    if [[ "$2" =~ ^[0-9]+$ ]]
	    then            	
		size=$2
		shift
	    else
		usage
            	exit 1
	    fi
            ;;
        -alg)
	    if [ $2 != "q" -a $2 != "s" ]
	    then
		usage
            	exit 1
	    else
            	alg=$2
		shift
	    fi
            ;;
        -gamma)
	    if [[ "$2" =~ ^(0\.[0-9]+)|[0-1]$ ]]
	    then            	
		gamma=$2
		shift
	    else
		usage
            	exit 1
	    fi            
            ;;
	-exps)
	    if [[ "$2" =~ ^[0-9]+$ ]]
	    then            	
		exps=$2
		shift
	    else
		usage
            	exit 1
	    fi            
            ;;
	-eps)
	    if [[ "$2" =~ ^[0-9]+$ ]]
	    then            	
		eps=$2
		shift
	    else
		usage
            	exit 1
	    fi            
            ;;
	-epsilon)	    
	    if [[ "$2" =~ ^[0-9]+\.?[0-9]*$ ]]
	    then            	
		epsilon=$2
		shift
	    else
		usage
            	exit 1
	    fi              
            ;;
	-alpha)	    
	    if [[ "$2" =~ ^[0-9]+\.?[0-9]*$ ]]
	    then            	
		alpha=$2
		shift
	    else
		usage
            	exit 1
	    fi              
            ;;
	-lambda)
	    if [[ "$2" =~ ^[0-9]+\.?[0-9]*$ ]]
	    then            	
		lambda=$2
		shift
	    else
		usage
            	exit 1
	    fi              
            ;;	
	-fig)
	    if [[ "$2" =~ ^[0-1]$ ]]
	    then            	
		figu=$2
		shift
	    else
		usage
            	exit 1
	    fi              
            ;;	
        -?*)  
            usage
            exit 1
       	    ;;
	*)
	    break
    esac
    shift
done

./gridworldRL "$size" "$alg" "$gamma" "$exps" "$eps" "$epsilon" "$alpha" "$lambda" "$figu"



