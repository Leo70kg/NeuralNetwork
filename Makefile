OBJS 	= BSDEConfiguration.o BSDEModel.o Equation.o HJBLQ.o PricingDefaultRisk.o AllenCahn.o app.o 

OUT 	= BSDE.out 
CC  	= g++ 
MPICC 	= mpicxx

FLAGS  	= -g -c -Wall -O3 -std=c++11
LFLAGS 	=  

all: $(OBJS)  
	$(MPICC) $(OBJS) -o $(OUT) $(LFLAGS)  

%.o: %.cpp
	$(MPICC) $(FLAGS) $< -o $@

BSDEConfiguration.o: BSDEConfiguration.cpp BSDEConfiguration.h  

BSDEModel.o: BSDEModel.cpp BSDEModel.h 

Equation.o: Equation.cpp Equation.h 

HJBLQ.o: HJBLQ.cpp HJBLQ.h

AllenCahn.o: AllenCahn.cpp AllenCahn.h

PricingDefaultRisk.o: PricingDefaultRisk.cpp PricingDefaultRisk.h  

app.o: app.cpp


clean: 
	rm -f $(OBJS) $(OUT)
