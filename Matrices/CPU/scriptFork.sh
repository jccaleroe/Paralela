echo "Ejecuentando version fork"

for(( size = 16; size <= 1024; size *= 2))
	do
	printf "\n"
	echo "Matriz N = " $size
	for forks in 1 2 4
	do
	printf "\n"
	echo "Con " $forks " forks"
		for((i = 1; i <= 10; i++))		
		do
			time ./hilos $size $forks
		done
	done
done
