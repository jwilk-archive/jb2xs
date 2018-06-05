/* Copyright © 2007-2018 Jakub Wilk <jwilk@jwilk.net>
 * SPDX-License-Identifier: MIT
 */

#include <JB2Image.h>
#include <GURL.h>
#include <ByteStream.h>
#include <GException.h>
#include <GBitmap.h>

#include <clocale>
#include <sstream>
#include <iomanip>
#include <iostream>

int main(int argc, char * const argv[])
{
  setlocale(LC_ALL, "");
  djvu_programname(argv[0]);
  GArray<GUTF8String> dargv(0, argc - 1);
  for(int i=0; i < argc; i++)
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
      std::string file_name = sstream.str();
      const GURL::Filename::UTF8 url(file_name.c_str());
      GP<ByteStream> obs = ByteStream::create(url, "wb");
      JB2Shape &shape = jimg->get_shape(i);
      if (shape.parent >= 0)
        std::cout << i << "\t" << shape.parent << std::endl;
      shape.bits->save_pbm(*obs);
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

// vim:ts=2 sts=2 sw=2 et
