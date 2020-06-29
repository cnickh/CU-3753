target:
	gcc multi-lookup.c util.c -o multi-lookup
	./multi-lookup 3 3 serviced.txt results.txt input/names1.txt input/names2.txt input/names3.txt input/names4.txt input/names5.txt
graph:
	python performance.py multi-lookup > /dev/null
clean:
	rm multi-lookup
