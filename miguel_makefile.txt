# el compilador a utilizar
COMPILADOR = clang++
#/opt/homebrew/Cellar/sdl2/2.26.1 
LIBRERIAS_PATH = C:\SDL2

#folder de los archivos fuente
SRC = src
BUILD = bin


# banderas del procesador y los includes de las librerias
CFLAGS = -std=c++17 -O3 -I"$(LIBRERIAS_PATH)\include" 
CFLAGSDEBUG = -std=c++17 -g -I"$(LIBRERIAS_PATH)/include" 
# banderas del linker y la librerias
LDFLAGS = -L"$(LIBRERIAS_PATH)\lib" -lmingw32 -lSDL2main -lSDL2 -lSDL2_image -lSDL2_ttf -mconsole -lm
#con pkg-conig -> `pkg-config --static --libs --cflags sdl2 SDL2_ttf SDL2_image`
MACROS = -D GRAVEDAD

# nombre del programa
TARGET = main.exe

# reglas, obtener todos los archivos .cpp y .hpp
CPP_SOURCE = $(wildcard *.cpp) $(wildcard $(SRC)/*.cpp) $(wildcard $(SRC)/*/*.cpp) $(wildcard $(SRC)/*/*/*.cpp)
CPP_HEADERS = $(wildcard *.hpp) $(wildcard $(SRC)/*.hpp) $(wildcard $(SRC)/*/*.hpp) $(wildcard $(SRC)/*/*/*.hpp)

CPP_OBJ = $(patsubst %.cpp, $(BUILD)/%.o, $(CPP_SOURCE))
CPP_OBJ_NOMAIN = $(filter-out $(BUILD)/main.o, $(CPP_OBJ))


# compilar
all: $(TARGET)

$(TARGET): objmain OBJETOS
	@echo "COMPILANDO Y LINKENADO"
	$(COMPILADOR) $(CFLAGS) $(MACROS) $(CPP_OBJ) -o $(TARGET) $(LDFLAGS)

# crear objetos
objmain:
	@echo "Creando Objeto del MAIN"
	@echo off && \
	IF exist bin ( @echo "Folder bin existe" ) ELSE ( @mkdir bin )
	$(COMPILADOR) $(CFLAGS) $(MACROS) main.cpp -c -o $(BUILD)\main.o

OBJETOS: $(CPP_OBJ_NOMAIN)
	@echo "OBJETOS CREADOS"
	
$(CPP_OBJ_NOMAIN): $(BUILD)/src/%.o : $(SRC)/%.cpp $(CPP_HEADERS)
	
	@echo "Creando Objeto $(subst /,\,$@)"
	@echo off && \
	IF exist $(subst /,\,$(@D)) ( echo $(subst /,\,$(@D)) existe ) ELSE ( @mkdir $(subst /,\,$(@D)) && @echo "creado folder $(subst /,\,$(@D))" )
	
	$(COMPILADOR) $(CFLAGS) $(MACROS) $< -c -o $(subst /,\,$@) 

debug: $(CPP_SOURCE) $(CPP_HEADERS)
	$(COMPILADOR) $(CFLAGSDEBUG) -o $(TARGET) $(CPP_SOURCE) $(LDFLAGS)

correr:
	@echo "EJECUTANDO PROGRAMA"
	$(TARGET)

# borrar el programa del folder
clean:
	
	rmdir /Q /S $(BUILD)
	del /f $(TARGET)
	
	