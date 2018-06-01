XUI=xui.c xui.h
PERCOLATION=percolation.c percolation.h
EXEC=percolation

$(EXEC): main.c $(XUI) $(PERCOLATION)
	gcc -Wall -g main.c $(XUI) $(PERCOLATION) -o $(EXEC) -lX11
