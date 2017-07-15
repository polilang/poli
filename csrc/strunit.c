// UNIT TESTS

#include "string.c"
#include "types.c"


i32 test(i32 cond, str test)
{
   if (!cond)
   {
      printf("%10s:  FAIL\n", test);
      exit(-1);
   }

   printf("%10s:  OK\n", test);

   return 1;
}



void length()
{
   test
   (
      strf.length("12345") == 5

      ,"length"
   );
}


void equal()
{
   test
   (
      strf.equal("12345", "12345") == 1

      ,"equal"
   );
}


void compare()
{
   test
   (
      strf.compare("asdf", "asdf") == 0
   && strf.compare("ASDF", "asdf")  < 0
   && strf.compare("asdf", "ASDF")  > 0

      ,"compare"
   );
}


void ncompare()
{
   test
   (
      strf.compare("asdf", "asdf", 3) == 0
   && strf.compare("ASDF", "asdf", 3)  < 0
   && strf.compare("asdf", "ASDF", 3)  > 0

      ,"ncompare"
   );
}


void new()
{
   str s = strf.new("new");

   test
   (
      strf.equal(s, "new")
   && strf.equal(s, strf.new("new"))
   && strf.length(s) == 3

      ,"new"
   );
}


void newcpy()
{
   str olds = strf.new("newstr");
   str news = strf.new(olds);

   test
   (
      strf.equal(olds, news)
   && olds != news

      ,"newcpy"
   );
}


void lower()
{
   test
   (
      strf.equal(strf.lower("HELLO"), strf.new("hello"))
   && strf.equal(strf.lower(strf.new("HELLO")), strf.new("hello"))

      ,"lower"
   );
}


void upper()
{
   test
   (
      strf.equal(strf.upper("hello"), strf.new("HELLO"))
   && strf.equal(strf.upper(strf.new("hello")), strf.new("HELLO"))

      ,"upper"
   );
}


void from_i64()
{
   test
   (
      strf.equal(strf.from_i((i64) 123456789), "123456789")
   && strf.equal(strf.from_i((i64)-123456789),"-123456789")

      ,"from_i64"
   );
}


void from_u64()
{
   test
   (
      strf.equal(strf.from_u((u64) 123456789), "123456789")
   && strf.equal(strf.from_u((u64)-123456789), "18446744073586094827")

      ,"from_u64"
   );
}


void from_f64()
{
   test
   (
      strf.equal(strf.from_f( 12345.6789), "12345.6789")
   && strf.equal(strf.from_f(-12345.6789),"-12345.6789")

      ,"from_f64"
   );
}


void to_i64()
{
   test
   (
      (i64) strf.to_i( "123456789") == (i64)  123456789
   && (i64) strf.to_i("-123456789") == (i64) -123456789

      ,"to_i64"
   );
}


void to_u64()
{
   test
   (
      (u64) strf.to_u( "123456789") == (u64) 123456789
   && (u64) strf.to_u("-123456789") == (u64) 18446744073586094827

      ,"to_u64"
   );
}


void to_f64()
{
   f64 pu = strf.to_f( "12345.6789")
   ,   ps = strf.to_f("-12345.6789")
   ,   cu = strf.to_f( "12345,6789")
   ,   cs = strf.to_f("-12345,6789");

   test
   (
      pu >=  12345.6788 && pu <=  12345.6790
   && ps <= -12345.6788 && ps >= -12345.6790
   && cu >=  12345.6788 && cu <=  12345.6790
   && cs <= -12345.6788 && cs >= -12345.6790

      ,"to_f64"
   );
}


void find()
{
   test
   (
      strf.find("single",   "i")   == 1
   && strf.find("multiple", "tip") == 3

      ,"find"
   );
}

void pfind()
{
   str a = "single", b = "multiple";

   test
   (
      strf.pfind(a,   "i") == a+1
   && strf.pfind(b, "tip") == b+3

      ,"pfind"
   );
}

void findc()
{
   str a = "single", b = "multiple";

   test
   (
      strf.findc(a,   "i") == 1
   && strf.findc(b, "pit") == 3

      ,"findc"
   );
}

void pfindc()
{
   str a = "single", b = "multiple";

   test
   (
      strf.pfindc(a,   "i") == a+1
   && strf.pfindc(b, "pit") == b+3

      ,"pfindc"
   );
}


void concat()
{
   str a = strf.new("hello ");
   str b = strf.new("world!");

   test
   (
      strf.equal(a, "hello ")
   && strf.equal(b, "world!")
   && strf.equal(strf.concat("hello ", "world!"), "hello world!")
   && strf.equal(strf.concat(a, b), "hello world!")

      ,"concat"
   );
}


void ltrim()
{
   test
   (
      strf.equal(strf.ltrim(strf.new("   hello   ")), "hello   ")

      ,"ltrim"
   );
}


void rtrim()
{
   test
   (
      strf.equal(strf.rtrim(strf.new("   hello   ")), "   hello")

      ,"rtrim"
   );
}


void trim()
{
   test
   (
      strf.equal(strf.trim(strf.new("   hello   ")), "hello")

      ,"trim"
   );
}


void format()
{
   test
   (
      strf.equal(strf.format("%li", 12345678901234567), "12345678901234567")

      ,"format"
   );
}


void slice()
{
   test
   (
      strf.equal(strf.slice("hello world", 0, 5), "hello")
   && strf.equal(strf.slice("hello world", 6, 11), "world")

      ,"slice"
   );
}


void split()
{
   str* strings = strf.split("hello world", " ");

   test
   (
      strf.equal(strings[0], "hello")
   && strf.equal(strings[1], "world")

      ,"split"
   );
}


int main ()
{
   atexit(gm_purge); // purge garbage on exit

   equal();
   ncompare();
   compare();
   length();

   new();
   newcpy();

   lower();
   upper();

   from_i64();
   from_u64();
   from_f64();

   to_i64();
   to_u64();
   to_f64();

   find();
   pfind();
   findc();
   pfindc();

   concat();

   ltrim();
   rtrim();
   trim();

   format();

   slice();
   split();
}
