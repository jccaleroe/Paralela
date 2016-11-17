echo "Ejecutando OMP"
echo > outOmpPad.cvs

for(( size = 16; size <= 1024; size *= 2))
	do
	echo "Matriz N = " $size >> outOmpPad.cvs
	for hilo in 1 2 4 8 16
	do
	echo "Con " $hilo " hilos" >> outOmpPad.cvs
		for((i = 1; i <= 10; i++))		
		do
			./matrixOmpPad $size $hilo >> outOmpPad.cvs
		done
	done
done
