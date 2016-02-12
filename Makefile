SOURCEDIR= ./source
OBJDIR= ./build
DEPDIR= ./dep
SOURCES= $(shell find $(SOURCEDIR) -name "*.cpp")
OBJECTS= $(addprefix $(OBJDIR)/,$(notdir $(SOURCES:.cpp=.o)))
DEPS= $(shell find $(DEPDIR) -name "*.d")

.DEFAULT_GOAL := all

include $(DEPS)

CC=clang++
CFLAGS= -std=c++11
LFLAGS=
DEBUG= -g3 -DDEBUG

debug: CFLAGS += $(DEBUG)
debug: LFLAGS += $(DEBUG)
debug: all

all: countermachine

$(OBJDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) -MM $(CFLAGS) $< > $(DEPDIR)/$*.d
	@mv -f $(DEPDIR)/$*.d $(DEPDIR)/$*.d.tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.o:|' < $(DEPDIR)/$*.d.tmp > $(DEPDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(DEPDIR)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPDIR)/$*.d
	@rm -f $(DEPDIR)/$*.d.tmp

countermachine: $(OBJECTS)
	$(CC) $^ -o $@ $(LFLAGS)

clean:
	rm -f countermachine $(OBJECTS) $(DEPS)
