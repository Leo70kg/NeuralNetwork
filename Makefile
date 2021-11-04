OBJ1 	= BSDEConfiguration.o BSDEModel.o BSDEModelRing.o Equation.o HJBLQ.o PricingDefaultRisk.o AllenCahn.o app.o collectives.o
OBJ2 	= BSDEConfiguration.o BSDEModel.o BSDEModelRing.o Equation.o HJBLQ.o PricingDefaultRisk.o AllenCahn.o appRing.o collectives.o
#OBJ2 	= collectives.o timer.o test.o

OBJS	= $(OBJ1) $(OBJ2)

EXEC1	= BSDE.out
EXEC2	= BSDE_Ring.out
EXECS	= $(EXEC1) $(EXEC2)

CC  	= g++ 
MPICC 	= mpicxx

FLAGS  	= -g -c -Wall -O3 -std=c++11
LFLAGS 	=  

all: $(EXECS)	
	

$(EXEC1): $(OBJ1)
	$(MPICC) $(OBJ1) -o $(EXEC1) $(LFLAGS)
	
$(EXEC2): $(OBJ2)
	$(MPICC) $(OBJ2) -o $(EXEC2) $(LFLAGS)


%.o: %.cpp
	$(MPICC) $(FLAGS) $< -o $@

BSDEConfiguration.o: BSDEConfiguration.cpp BSDEConfiguration.h  

BSDEModel.o: BSDEModel.cpp BSDEModel.h 

BSDEModelRing.o: BSDEModelRing.cpp BSDEModelRing.h

Equation.o: Equation.cpp Equation.h 

HJBLQ.o: HJBLQ.cpp HJBLQ.h

AllenCahn.o: AllenCahn.cpp AllenCahn.h

PricingDefaultRisk.o: PricingDefaultRisk.cpp PricingDefaultRisk.h  

collectives.o: collectives.cpp collectives.h

#timer.o: timer.cpp timer.h

app.o: app.cpp

appRing.o: appRing.cpp

#test.o: test.cpp

clean: 
	rm -f $(OBJS) $(EXECS)
