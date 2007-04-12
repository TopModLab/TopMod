/* $Id: DLFLApp.cc,v 4.0 2003/12/26 01:58:52 vinod Exp $ */

#include "DLFLAppWindow.hh"
#include <string.h>

int main(int argc, char ** argv)
{
  if ( !strcmp(argv[0],"dlfltimetest") )
     {
          // Time test run, just read the object using alternate reader and exit
       if ( argc > 1 )
          {
            DLFLObject temp;
            ifstream file;
            file.open(argv[1]);
            temp.readObjectAlt(file);
            file.close();
            cout << temp.num_vertices() << " "
                 << temp.num_faces() << " "
                 << temp.num_edges() << " ";
          }
       exit(0);
     }

  DLFLAppWindow * win = new DLFLAppWindow("DLFL Demo");
  win->resizable(*win);
  win->end();

  if ( argc > 1 )
     {
       win->openFile(argv[1]);
       argc--; argv++;
       if ( argc > 1 )
          {
               // Read texture file
            win->readTexture(argv[1]);
            argc--; argv++;
          }
     }

  Fl::visual(FL_DOUBLE|FL_RGB);
  win->show(argc,argv);

  return Fl::run();
}


/*
  $Log: DLFLApp.cc,v $
  Revision 4.0  2003/12/26 01:58:52  vinod
  Major version sync.

  Revision 3.0  2003/09/19 16:06:47  vinod
  Major version update

  Revision 2.0  2003/01/25 08:49:31  vinod
  *** empty log message ***

  Revision 1.8  2002/12/14 21:00:54  vinod
  Modified time test version to just create object without any other windows

  Revision 1.7  2002/10/30 03:44:05  vinod
  Added call to load texture from second argument

  Revision 1.6  2002/10/28 06:50:39  vinod
  Added call to set visual

  Revision 1.5  2002/10/25 05:32:08  vinod
  Added code to use command line arguments

  Revision 1.4  2002/10/22 21:17:30  vinod
  Removed initial size for DLFLAppWindow

  Revision 1.3  2002/10/17 02:10:32  vinod
  Removed definitions for DLFLAppWindow static members

  Revision 1.2  2002/10/08 16:15:09  vinod
  Added definitions for DLFLAppWindow static members

*/
