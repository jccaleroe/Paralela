echo "Ejecuentando CUDA"
echo > out.cvs

for(( size = 512; size <= 1024; size *= 2))
do
	echo "Matriz N = " $size >> out.cvs
	for block in 1 2 4 8
	do
		echo "Con bloques = " $block >> out.cvs
		for hilo in 32 128 1024 2048 4096 6144 8192 12288 24576
		do
			echo "Con " $hilo " hilos" >> out.cvs
			for((i = 1; i <= 10; i++))		
			do
				./matrix $size $block $hilo >> out.cvs
			done
		done
	done
done
