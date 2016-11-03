echo "Ejecuentando version secuencial"

for(( size = 16; size <= 1024; size *= 2))
	do
	printf "\n"
	echo "Matriz N = " $size
	for((i = 1; i <= 10; i++))
	do
		time ./secuencial $size
	done
done
