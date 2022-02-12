for i in {1..50}
do
	ab -n 10000 -c 10 http://localhost:8000/
done
