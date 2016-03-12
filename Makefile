SOURCEDIR= ./source
OBJDIR= ./build
DEPDIR= ./dep
SOURCES= $(shell find $(SOURCEDIR) -not -path "$(SOURCEDIR)/gui/*" -name "*.cpp")
OBJECTS= $(addprefix $(OBJDIR)/,$(notdir $(SOURCES:.cpp=.o)))
GUIOBJECTS=$(shell find $(SOURCEDIR)/gui -name "*.o")
DEPS= $(shell find $(DEPDIR) -name "*.d")

.DEFAULT_GOAL := gf

include $(DEPS)

CC=clang++
CFLAGS += -std=c++11 -IQtCore -IQtGui
LDLIBS += -lQtCore -lQtGui -lboost_program_options
DEBUG= -g3 -DDEBUG
gui= -DGUI

.PHONY: gf app clean debug debug_app gui

debug: CFLAGS += $(DEBUG)
debug: LDFLAGS += $(DEBUG)
debug: gui
debug: debug_app
debug_app: GUIOBJECTS= $(shell find $(SOURCEDIR)/gui -name "*.o")
debug_app: $(OBJECTS:.o=.g) $(GUIOBJECTS)
	$(CC) $^ -o countermachine $(LDFLAGS) $(LDLIBS)

gui:
	cd source/gui && qmake && make
gf: gui
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

countermachine: GUIOBJECTS= $(shell find $(SOURCEDIR)/gui -name "*.o")
countermachine: $(OBJECTS) $(GUIOBJECTS)
	$(CC) $^ -o $@ $(LDFLAGS) $(LDLIBS)

clean:
	rm -f countermachine $(OBJECTS) $(OBJECTS:.o=.g) $(DEPS)
	cd $(SOURCEDIR)/gui && make clean && rm -f Makefile
