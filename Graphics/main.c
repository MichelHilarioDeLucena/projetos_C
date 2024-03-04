#include <stdio.h>
#include <X11/Xlib.h>


int main(){
    Display *display;
    Window window;
    XEvent event;
    display=XOpenDisplay(NULL);
    if(!display){
        fprintf(stderr,"Erro ao abrir o display\n");
        return 1;
    }

    window=XCreateSimpleWindow(display,DefaultRootWindow(display),0,0,640,480,1,0,0);
    XSelectInput(display,window,ExposureMask|KeyPressMask|StructureNotifyMask);
    Atom wmDeleteMessage=XInternAtom(display,"WM_DELETE_WINDOW",False);
    XSetWMProtocols(display,window,&wmDeleteMessage,1);
    
    while(1){
        XNextEvent(display,&event);
        if(event.type==ClientMessage&&event.xclient.data.l[0]==wmDeleteMessage)
            break;
    }
    XCloseDisplay(display);
}