# build helloworld executable when user executes "make"

obj=helloworld.o draw.o drawfont.o font.o gb2312_utf8_code.o drawbmp.o

helloworld: $(obj)
	$(CC) $(LDFLAGS) $(obj) -L. -ljpeg -o helloworld
*.o: *.c
	$(CC) $(CFLAGS) -c helloworld.c
	$(CC) $(CFLAGS) -c font.c
	$(CC) $(CFLAGS) -c draw.c
	$(CC) $(CFLAGS) -c drawfont.c
	$(CC) $(CFLAGS) -c gb2312_utf8_code.c
	$(CC) $(CFLAGS) -c drawbmp.c
# remove object files and executable when user executes "make clean"
clean:
	rm *.o helloworld

