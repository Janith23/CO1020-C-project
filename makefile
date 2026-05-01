CC = gcc
CFLAGS = -Wall -Wextra -Iinclude
LDFLAGS = -mconsole

OBJDIR = build
SRCDIR = src
TESTDIR = tests
DEMODIR = demo

OBJS = $(OBJDIR)/canvas.o $(OBJDIR)/math3d.o $(OBJDIR)/renderer.o $(OBJDIR)/animate_soccerball_cube.o $(OBJDIR)/animation.o $(OBJDIR)/lighting.o

.PHONY: all clean mkdir

all: mkdir $(OBJDIR)/libtiny3d.a $(OBJDIR)/test_math.exe $(OBJDIR)/test_pipeline.exe $(OBJDIR)/demo.exe $(OBJDIR)/visual.exe

mkdir:
	if not exist $(OBJDIR) mkdir $(OBJDIR)

$(OBJDIR)/canvas.o: $(SRCDIR)/canvas.c include/canvas.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/math3d.o: $(SRCDIR)/math3d.c include/math3d.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/renderer.o: $(SRCDIR)/renderer.c include/renderer.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/animate_soccerball_cube.o: $(SRCDIR)/animate_soccerball_cube.c include/animate_soccerball_cube.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/animation.o: $(SRCDIR)/animation.c include/animation.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/lighting.o: $(SRCDIR)/lighting.c include/lighting.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/test_math.o: $(TESTDIR)/test_math.c include/math3d.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/test_pipeline.o: $(TESTDIR)/test_pipeline.c include/canvas.h include/math3d.h include/renderer.h include/animate_soccerball_cube.h
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/libtiny3d.a: $(OBJS)
	ar rcs $@ $^

$(OBJDIR)/test_math.exe: $(OBJDIR)/test_math.o $(OBJDIR)/libtiny3d.a
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJDIR)/test_pipeline.exe: $(OBJDIR)/test_pipeline.o $(OBJDIR)/libtiny3d.a
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

$(OBJDIR)/demo.exe: $(DEMODIR)/main.c $(OBJDIR)/libtiny3d.a
	$(CC) $(CFLAGS) $(LDFLAGS) $< $(OBJDIR)/libtiny3d.a -o $@

$(OBJDIR)/visual.exe: $(TESTDIR)/visual_soccerball.c $(OBJDIR)/libtiny3d.a
	$(CC) $(CFLAGS) $(LDFLAGS) $< $(OBJDIR)/libtiny3d.a -o $@

clean:
	-del /Q $(OBJDIR)\*.o $(OBJDIR)\*.a $(OBJDIR)\*.exe 2>nul || exit 0
