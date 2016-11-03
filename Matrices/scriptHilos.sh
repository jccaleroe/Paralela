echo "Ejecuentando version de hilos"

for(( size = 16; size <= 1024; size *= 2))
	do
	printf "\n"
	echo "Matriz N = " $size
	for hilos in 1 2 3 8 16
	do
	printf "\n"
	echo "Con " $hilos " hilos"
		for((i = 1; i <= 10; i++))		
		do
			time ./hilos $size $hilos
		done
	done
done
