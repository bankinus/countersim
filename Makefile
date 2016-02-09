SOURCEDIR= ./source
OBJDIR= ./build
DEPDIR= ./dep
SOURCES= $(shell find $(SOURCEDIR) -name "*.cpp")
OBJECTS= $(addprefix $(OBJDIR)/,$(notdir $(SOURCES:.cpp=.o)))
DEPS= $(addprefix $(DEPDIR)/,$(notdir $(SOURCES:.cpp=.d)))

.DEFAULT_GOAL := all

-include $(DEPS)

CC=g++
CFLAGS=
LFLAGS=
DEBUG= -g

all: countermachine

$(OBJDIR)/%.o : $(SOURCEDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) -MM $(CFLAGS) $< > $(DEPDIR)/$*.d
	@cp -f $(DEPDIR)/$*.d $(DEPDIR)/$*.d.tmp
	@sed -e 's/.*://' -e 's/\\$$//' < $(DEPDIR)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPDIR)/$*.d
	@rm -f $(DEPDIR)/$*.d.tmp

countermachine: $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)

clean:
	rm -f countermachine $(OBJECTS) $(DEPS) 
