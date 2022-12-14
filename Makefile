CFILES = main.cpp tree\tree.cpp tree\tree_dump.cpp tree\text_tree.cpp diff.cpp tree_tex.cpp create_consts.cpp
OUTPUT = a.exe
IMG_FORMAT = png
DOT_FILE_NAME = diffed_graph.dot

CFLAGS= -Wshadow    			\
	-Winit-self 			\
	-Wredundant-decls 		\
	-Wcast-align			\
	-Wundef 			\
	-Wfloat-equal 			\
	-Winline 			\
	-Wunreachable-code 		\
	-Wmissing-declarations 		\
	-Wmissing-include-dirs 		\
	-Wswitch-enum 			\
	-Wswitch-default 		\
	-Weffc++ 			\
	-Wmain 				\
	-Wextra 			\
	-Wall 				\
	-g 				\
	-pipe 				\
	-fexceptions 			\
	-Wcast-qual 			\
	-Wconversion 			\
	-Wctor-dtor-privacy 		\
	-Wempty-body 			\
	-Wformat-security 		\
	-Wformat=2 			\
	-Wignored-qualifiers 		\
	-Wlogical-op 			\
	-Wmissing-field-initializers 	\
	-Wnon-virtual-dtor 		\
	-Woverloaded-virtual 		\
	-Wpointer-arith 		\
	-Wsign-promo 			\
	-Wstack-usage=8192 		\
	-Wstrict-aliasing 		\
	-Wstrict-null-sentinel 		\
	-Wtype-limits 			\
	-Wwrite-strings 		\
	-D_DEBUG 			\
	-D_EJUDGE_CLIENT_SIDE

all:
	@ g++ -o $(OUTPUT) $(CFLAGS) $(CFILES)
def:
	@g++ -E $(OUTPUT) $(CFLAGS) $(CFILES) >> defines.txt

run:
	@ a.exe
	@ echo Run

.PHONY: clean
clean:
	@ rm -f $(OUTPUT)

.PHONY: clean_cmd
clean_cmd:
	@ cls

.PHONY: graphviz
graphviz:
	@ dot -T $(IMG_FORMAT) -o graph.$(IMG_FORMAT) $(DOT_FILE_NAME)

.PHONY: original_graphviz
original_graphviz:
	@ dot -T $(IMG_FORMAT) -o tree_graph.$(IMG_FORMAT) tree_graph.dot
