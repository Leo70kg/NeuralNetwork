OBJS = BSDEConfiguration.o BSDEModel.o Equation.o HJBLQ.o PricingDefaultRisk.o app.o 
SOURCE = BSDEConfiguration.cpp BSDEModel.cpp Equation.cpp HJBLQ.cpp PricingDefaultRisk.cpp app.cpp 
HEADER = BSDEConfiguration.h BSDEModel.h Equation.h HJBLQ.h Matrix.h OneHotVector.h PricingDefaultRisk.h Utility.h Vector.h 

OUT = BSDE.out 
CC  = g++ 
FLAGS  = -g -c -Wall -O3 -std=c++11
LFLAGS =  

all: $(OBJS)  
	$(CC) -g $(OBJS) -o $(OUT) $(LFLAGS)  

BSDEConfiguration.o: BSDEConfiguration.cpp  
	$(CC) $(FLAGS) BSDEConfiguration.cpp  

BSDEModel.o: BSDEModel.cpp 
	$(CC) $(FLAGS) BSDEModel.cpp  

Equation.o: Equation.cpp  
	$(CC) $(FLAGS) Equation.cpp  

HJBLQ.o: HJBLQ.cpp 
	$(CC) $(FLAGS) HJBLQ.cpp 

PricingDefaultRisk.o: PricingDefaultRisk.cpp  
	$(CC) $(FLAGS) PricingDefaultRisk.cpp  

app.o: app.cpp  
	$(CC) $(FLAGS) app.cpp  


clean: 
	rm -f $(OBJS) $(OUT)
