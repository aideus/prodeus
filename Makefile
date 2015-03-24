
OPT = -O2 -pedantic -Wall -Werror -Wno-unused-value -Wno-varargs -std=c++11 

OBJS = expression.o apply.o basic_det.o basic_rnd.o conditions.o cvprob.o data.o \
      eiterator.o environment.o  infer.o lambda.o \
      let.o lib_func.o lists.o probs.o quote.o symbol.o value.o

LIBS = -lopencv_core -lopencv_highgui -lopencv_imgproc

all: example

.cpp.o:
	c++ -c $(OPT) $*.cpp -o $*.o

example:example.cpp $(OBJS)
	c++ $(OPT)  example.cpp -o example $(OBJS) $(LIBS)

clean:
	rm -f *.o example
