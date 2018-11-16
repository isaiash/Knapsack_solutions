#pasar el nombre del ejecutable
#corre todas las instancias en ./Uniform
#las guarda en results_"nombredelejecutable".dat
if [ -e $1 ]
then

	if [ -e "results_$1.dat" ]
	then
		rm results_$1.dat
	fi

	for i in ./Uniform/*
	do
		./dp1 $i >> results_$1.dat
	done

else
	echo "$1.cpp: file not found"
fi
