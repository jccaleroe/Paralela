echo "Ejecuentando CUDA"
echo > out.cvs

for(( size = 128; size <= 256; size *= 2))
do
	echo "Matriz N = " $size >> out.cvs
	for block in 1 2 4 8
	do
		echo "Con bloques = " $block >> out.cvs
		for hilo in 32 128 256 1024 1536 2048 3072 4096 8192
		do
			echo "Con " $hilo " hilos" >> out.cvs
			for((i = 1; i <= 10; i++))		
			do
				./matrix $size $block $hilo >> out.cvs
			done
		done
	done
done
