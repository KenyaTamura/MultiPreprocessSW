OBJS = Data.o FileConverter.o PreprocessBase.o PreprocessSingle.o PreprocessDouble.o PreprocessTriple.o PreprocessQuad.o PreprocessSW.o SWBase.o SimpleSW.o PreprocessSWGPU.o Writer.o main.o
BIN = a.out
NVCC = nvcc -std=c++11 -O2 -D_FORCE_INLINES 
GPP = g++ -std=c++11 -O2


$(BIN): $(OBJS)
	$(NVCC) $(OBJS) -o $(BIN)

clean:
	rm *.o

.SUFFIXES: .o .cpp .cu

.cu.o: 
	$(NVCC) -c -o $@ $<

.cpp.o:
	$(GPP) -c -o $@ $<

