CFLAGS += -Wall -O2

WRT54GMEMOBJS = wrt54g.o

all: wrt54g

wrt54g: $(WRT54GMEMOBJS)
	gcc $(CFLAGS) -o $@ $(WRT54GMEMOBJS)

clean:
	rm -rf *.o wrt54g
