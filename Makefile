LOGIN = xskura01-xshcha00
TARGET = diagrams
RM = rm -rf

all: build

build:
	qmake -o qMakefile $(TARGET).pro
	make -f qMakefile

run: build
	./$(TARGET)

doxygen:
	doxygen doc/Doxyfile

zip:
	zip -r $(LOGIN).zip $(TARGET).pro doc examples src Makefile README.txt

clean:
	make -f qMakefile clean
	$(RM) $(TARGET)
	$(RM) qMakefile
	$(RM) .qmake.stash
	$(RM) doc/html
	$(RM) doc/latex
