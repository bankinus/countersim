SOURCEDIR= ./source
OBJDIR= ./build
DEPDIR= ./dep
SOURCES= $(shell find $(SOURCEDIR) -name "*.cpp")
OBJECTS= $(addprefix $(OBJDIR)/,$(notdir $(SOURCES:.cpp=.o)))
DEPS= $(shell find $(DEPDIR) -name "*.d")

.DEFAULT_GOAL := all

include $(DEPS)

CC=clang++
CFLAGS += -std=c++11
LDLIBS += -lQtCore -lQtGui -lboost_program_options
DEBUG= -g3 -DDEBUG

.PHONY: all clean debug

debug: CFLAGS += $(DEBUG)
debug: LDFLAGS += $(DEBUG)
debug: $(OBJECTS:.o=.g)
	$(CC) $^ -o countermachine $(LFLAGS)

all: countermachine

$(OBJDIR)/%.g: $(SOURCEDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) -MM $(CFLAGS) $< > $(DEPDIR)/$*.d
	@mv -f $(DEPDIR)/$*.d $(DEPDIR)/$*.d.tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.g:|' < $(DEPDIR)/$*.d.tmp > $(DEPDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(DEPDIR)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPDIR)/$*.d
	@rm -f $(DEPDIR)/$*.d.tmp

$(OBJDIR)/window.o: $(SOURCEDIR)/gui/window.h $(SOURCEDIR)/gui/window.cpp
	cd source/gui && qmake && make && cp window.o ../../$(OBJDIR)/window.o

$(OBJDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) -MM $(CFLAGS) $< > $(DEPDIR)/$*.d
	@mv -f $(DEPDIR)/$*.d $(DEPDIR)/$*.d.tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.o:|' < $(DEPDIR)/$*.d.tmp > $(DEPDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(DEPDIR)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPDIR)/$*.d
	@rm -f $(DEPDIR)/$*.d.tmp

countermachine: $(OBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f countermachine $(OBJECTS) $(OBJECTS:.o=.g) $(DEPS)
