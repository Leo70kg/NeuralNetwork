OBJ 	= BSDEConfiguration.o BSDEModel.o BSDEModelMs.o BSDEModelRing.o Equation.o HJBLQ.o PricingDefaultRisk.o AllenCahn.o collectives.o app.o

EXEC	= BSDE.out

MPICC 	= mpicxx

FLAGS  	= -g -c -Wall -O3 -std=c++11
LFLAGS 	=  

all: $(EXEC)	
	
$(EXEC): $(OBJ)
	$(MPICC) $(OBJ) -o $(EXEC) $(LFLAGS)
	

%.o: %.cpp
	$(MPICC) $(FLAGS) $< -o $@

BSDEConfiguration.o: BSDEConfiguration.cpp BSDEConfiguration.h  

BSDEModel.o: BSDEModel.cpp BSDEModel.h 

BSDEModelMs.o: BSDEModelMs.cpp BSDEModelMs.h

BSDEModelRing.o: BSDEModelRing.cpp BSDEModelRing.h

Equation.o: Equation.cpp Equation.h 

HJBLQ.o: HJBLQ.cpp HJBLQ.h

AllenCahn.o: AllenCahn.cpp AllenCahn.h

PricingDefaultRisk.o: PricingDefaultRisk.cpp PricingDefaultRisk.h  

collectives.o: collectives.cpp collectives.h

app.o: app.cpp

clean: 
	rm -rf $(OBJ) $(EXEC)
