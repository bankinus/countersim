SOURCEDIR= ./source
OBJDIR= ./build
DEPDIR= ./dep
SOURCES= $(shell find $(SOURCEDIR) -not -path "$(SOURCEDIR)/gui/*" -name "*.cpp")
OBJECTS= $(addprefix $(OBJDIR)/,$(notdir $(SOURCES:.cpp=.o)))
GUIOBJECTS=
DEPS= $(shell find $(DEPDIR) -name "*.d")

.DEFAULT_GOAL := gf

include $(DEPS)

CC=clang++
CFLAGS += -std=c++11 -IQtCore -IQtGui
LDLIBS += -lQtCore -lQtGui -lboost_program_options
DEBUG= -g3 -DDEBUG
gui= -DGUI

.PHONY: gf app clean debug gui

debug: CFLAGS += $(DEBUG)
debug: LDFLAGS += $(DEBUG)
debug: gui
debug: GUIOBJECTS+= $(shell find $(SOURCEDIR)/gui -name "*.o")
debug: $(OBJECTS:.o=.g) $(GUIOBJECTS)
	$(CC) $^ -o countermachine $(LFLAGS)

gui:
	cd source/gui && qmake && make clean && make
gf: gui
gf: GUIOBJECTS+= $(shell find $(SOURCEDIR)/gui -name "*.o")
gf: app

app: countermachine

$(OBJDIR)/%.g: $(SOURCEDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) -MM $(CFLAGS) $< > $(DEPDIR)/$*.d
	@mv -f $(DEPDIR)/$*.d $(DEPDIR)/$*.d.tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.g:|' < $(DEPDIR)/$*.d.tmp > $(DEPDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(DEPDIR)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPDIR)/$*.d
	@rm -f $(DEPDIR)/$*.d.tmp

$(OBJDIR)/%.o: $(SOURCEDIR)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@
	$(CC) $(CFLAGS) -MM $< > $(DEPDIR)/$*.d
	@mv -f $(DEPDIR)/$*.d $(DEPDIR)/$*.d.tmp
	@sed -e 's|.*:|$(OBJDIR)/$*.o:|' < $(DEPDIR)/$*.d.tmp > $(DEPDIR)/$*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $(DEPDIR)/$*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $(DEPDIR)/$*.d
	@rm -f $(DEPDIR)/$*.d.tmp

countermachine: $(OBJECTS) $(GUIOBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS) $(GUIOBJECTS)

clean:
	rm -f countermachine $(OBJECTS) $(OBJECTS:.o=.g) $(DEPS)
	cd $(SOURCEDIR)/gui && make clean && rm -f Makefile
