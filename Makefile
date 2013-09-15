
.phony:	all
.phony:	clean
	
all:
	g++ -o madBird madBird.cpp -L/usr/X11R6/lib -lX11
	
clean:
	rm -i a1 
