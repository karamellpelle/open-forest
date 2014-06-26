XMOUNTAINSOBJS = \
	global.o \
	xmountains.o \
	calcalt.o \
	random.o

DATABASEOBJS = \
	database.o \
	terrain.o \
	point.o \
	control.o \
	course.o \
	map.o \
	event.o \
	orienteer.o \
	run.o

NETWORKOBJS = \
	message.o \
	fdManager.o \
	server.o \
	client.o

GUIOBJS = \
	dlgOrienteer.o \
	dlgSelect.o \
	dlgTerrain.o \
	dlgMap.o \
	dlgCourse.o \
	dlgEvent.o \
	dlgResult.o \
	dlgOptions.o \
	dlgSingle.o \
	dlgMenu.o \
	mainWindow.o \
	windowMessage.o \
	dlgStack.o \
	dlgCommon.o \
	dlgHost1.o \
	dlgHost2.o \
	dlgJoin1.o \
	dlgJoin2.o \
	dlgPlayers.o

UTILOBJS = \
	common.o \
	util.o \
	md5.o \
	xmlParser.o \
	texture.o \
	shader.o \
	noise.o \
	forest.o

RUNTIMEOBJS = \
	mesh.o \
	terrainproxy.o \
	oriproxy.o \
	mapproxy.o \
	eventproxy.o

OBJS = \
	$(XMOUNTAINSOBJS) \
	$(DATABASEOBJS) \
	$(NETWORKOBJS) \
	$(GUIOBJS) \
	$(UTILOBJS) \
	$(RUNTIMEOBJS)

INCDIRS = -I/usr/X11R6/include -I/usr/include

LIBDIRS = -L/usr/X11R6/lib -L/opt/local/lib

LIBS = -lplibpuaux -lplibpu -lplibfnt -lplibsg -lplibul -lGLEW -lGL -lGLU -lglut -lm -lc

DEBUG_INFO = -g

PROFILER = -pg
#PROFILER =

CXXFLAGS = $(PROFILER) $(DEBUG_INFO) -Wall $(INCDIRS)

%.o : %.cxx
	g++ $(CXXFLAGS) -c $< -o $@

batb: $(OBJS)
	g++ -o batb $(PROFILER) $(OBJS) $(LIBDIRS) $(LIBS)

clean: 
	rm -f *.o *~ batb core

backup: clean
	(cd ../..; tar cvf batbxx.tar batbxx; gzip batbxx.tar)
