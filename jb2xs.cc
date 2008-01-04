#include <JB2Image.h>
#include <GURL.h>
#include <ByteStream.h>
#include <GException.h>
#include <GBitmap.h>

#include <clocale>
#include <sstream>
#include <iomanip>

int main(int argc, char * const argv[])
{
  setlocale(LC_ALL,"");
  djvu_programname(argv[0]);
  GArray<GUTF8String> dargv(0, argc - 1);
  for(int i=0;i<argc;++i)
    dargv[i] = GNativeString(argv[i]);
  G_TRY
  {
    if (argc != 2)
      return 2;
    const GURL::Filename::UTF8 url(dargv[1]);
    GP<ByteStream> ibs = ByteStream::create(url, "rb");
    GP<JB2Image> jimg = JB2Image::create();
    jimg->decode(ibs);
    int shape_count = jimg->get_shape_count();
    for (int i = 0; i < shape_count; i++)
    {
      std::ostringstream sstream;
      sstream << std::setw(6) << std::setfill('0') << i << ".pbm";
      const GURL::Filename::UTF8 url(sstream.str().c_str());
      GP<ByteStream> obs = ByteStream::create(url, "wb");
      JB2Shape &shape = jimg->get_shape(i);
      shape.bits->save_pbm(*obs);
    }
    int blit_count = jimg->get_blit_count();
    printf(">> %d\n", blit_count);
    for (int i = 0; i < blit_count; i++)
    {
      const JB2Blit *blit = jimg->get_blit(i);
      printf("%d\n", blit->shapeno);
    }
  }
  G_CATCH(ex)
  {
    ex.perror();
    return 1;
  }
  G_ENDCATCH;
  return 0;
}

// vim:ts=2 sw=2 et
