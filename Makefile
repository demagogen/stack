CC=g++

OBJECTS_DIR=build
OUTFILE=stack

SOURCES=main.cpp stack.cpp color_scheme_changer.cpp hash.cpp

CFLAGS= -D _DEBUG -ggdb3 -std=c++17 -O0 -Wall -Wextra -Weffc++ -Waggressive-loop-optimizations 				\
		-Wc++14-compat -Wmissing-declarations -Wcast-align -Wcast-qual -Wchar-subscripts 					\
		-Wconditionally-supported -Wconversion -Wctor-dtor-privacy -Wempty-body -Wfloat-equal 				\
		-Wformat-nonliteral -Wformat-security -Wformat-signedness -Wformat=2 -Winline -Wlogical-op 			\
		-Wnon-virtual-dtor -Wopenmp-simd -Woverloaded-virtual -Wpacked -Winit-self -Wredundant-decls 		\
		-Wshadow -Wsign-conversion -Wsign-promo -Wstrict-null-sentinel -Wstrict-overflow=2 					\
		-Wsuggest-attribute=noreturn -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-override 		\
		-Wswitch-default -Wswitch-enum -Wsync-nand -Wundef -Wunreachable-code -Wunused -Wuseless-cast 		\
		-Wvariadic-macros -Wno-literal-suffix -Wno-missing-field-initializers -Wno-narrowing 				\
		-Wno-old-style-cast -Wno-varargs -Wstack-protector -fcheck-new -fsized-deallocation 				\
		-fstack-protector -fstrict-overflow -flto-odr-type-merging -fno-omit-frame-pointer 					\
		-Wlarger-than=8192 -Wstack-usage=8192 -pie -fPIE -Werror=vla -fsanitize=address,alignment,$			\
		bool,bounds,enum,float-cast-overflow,float-divide-by-zero,integer-divide-by-zero,leak,$				\
		nonnull-attribute,null,object-size,return,returns-nonnull-attribute,shift,signed-integer-overflow,$	\
		undefined,unreachable,vla-bound,vptr

OBJECT_FILES=$(addsuffix .o,$(basename $(SOURCES)))

.PHONY: clean all makedir

all: makedir $(OUTFILE)

$(OUTFILE): $(OBJECT_FILES)
	$(CC) $(addprefix $(OBJECTS_DIR)/,$^) -o $(OBJECTS_DIR)/$(OUTFILE)

$(OBJECT_FILES): %.o: %.cpp
	$(CC) -c $^ -o $(OBJECTS_DIR)/$@

makedir:
	@mkdir -p $(OBJECTS_DIR)

clean:
	@rm -f $(OBJECTS_DIR)/*.o
	@rm -f $(OBJECTS_DIR)/$(OUTFILE)

