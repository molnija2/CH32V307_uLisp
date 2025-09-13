#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#include "ulisp.h"
//#include "graph_tcp.h"

//#include "arrays.h"
#include "ff.h"

#include "ch32v30x_rtc.h"
#include "rtc.h"
#include "filesys.h"

//#include "numeric.h"

extern object *tee;

/*
 User Extensions
*/


static _calendar_obj calendar ;

// Definitions


object *fn_now (object *args, object *env) {
  (void) env;

  int nargs = listlength(args);


  // Set time
  if (nargs == 3) {

	  RTC_Get(&calendar, 0) ;


	  RTC_Set(calendar.w_year, calendar.w_month, calendar.w_date,
    		checkinteger(first(args)), checkinteger(second(args)), checkinteger(third(args)) ) ;

	  //printf("set time %d %d %d\n",checkinteger(first(args)), checkinteger(second(args)), checkinteger(third(args)));
  }
  else  if (nargs == 6) {

	  RTC_Get(&calendar, 1) ;

	  object *hours = car(args) ;  args = cdr(args) ;
	  object *minutes = car(args) ;  args = cdr(args) ;
	  object *seconds = car(args) ;  args = cdr(args) ;
	  object *day = car(args) ;  args = cdr(args) ;
	  object *mon = car(args) ;  args = cdr(args) ;
	  object *year = car(args) ;  args = cdr(args) ;

	 RTC_Set(checkinteger(year), checkinteger(mon),checkinteger(day),
    		checkinteger(hours), checkinteger(minutes), checkinteger(seconds) ) ;
 } else
	  if (nargs > 0) error2("wrong number of arguments");
  
  // Return time
  RTC_Get(&calendar, 1) ;

  object *seconds = number(calendar.sec);
  object *minutes = number(calendar.min);
  object *hours = number(calendar.hour);
  object *day = number(calendar.w_date);
  object *mon = number(calendar.w_month);
  object *year = number(calendar.w_year);
  object *week = number(calendar.week);
  return cons(hours, cons(minutes, cons(seconds,  cons(day,  cons(mon,  cons(year,  cons(week, NULL)))))));
}



object *fn_touch_press (object *args, object *env) {
  (void) env;
#ifdef touchscreen_support


  return  MouseStateButtons() ? tee : nil;
#else
  return nil ;
#endif
}

object *fn_touch_x (object *args, object *env) {
  (void) env;
#ifdef  touchscreen_support
  return number(MouseStateX());
#else
  return nil ;
#endif
}

object *fn_touch_y (object *args, object *env) {
  (void) env;
#ifdef touchscreen_support


    return number(MouseStateY());
#else
  return nil ;
#endif
}

void PrintToucscreenParameters ()
{
#ifdef touchscreen_support

#endif
}

object *fn_touch_printcal (object *args, object *env)
{
  (void) env;

  PrintToucscreenParameters() ;

  return tee ;
}

object *fn_touch_setcal(object *args, object *env)
{
  (void) env;

#ifdef touchscreen_support
    return tee ;

#endif


return nil ;
}

void drawCalibrationPrompt()
{
  //tft.setTextColor(TFT_RED, TFT_BLACK) ;
  //tft.drawString("Touch calibration", 100, 40) ;
  //tft.setTextColor(TFT_GREEN, TFT_BLACK) ;
  //tft.drawString("Press this point", 100, 50) ;
}


void drawCalibrationTestPrompt()
{
  //tft.setTextColor(TFT_BLUE, TFT_BLACK) ;
  //tft.drawString("Test touch calibration", 100, 40) ;
  //tft.setTextColor(TFT_GREEN, TFT_BLACK) ;
  //tft.drawString("Press this point", 100, 50) ;
}


void drawCalibrationCross(uint16_t x, uint16_t y, uint16_t color)
{
  //tft.drawLine(x-10, y, x+10, y, color) ;
  //tft.drawLine(x, y-10, x, y+10, color) ;
}






object *fn_touch_calibrate(object *args, object *env)
{
  (void) env;

#ifdef touchscreen_support


  return tee;
#else
  return nil ;
#endif
}



/*
(kbhit)  -  test whether any keyboard keys hits"
*/

extern volatile uint8_t uiRecvIndex, uiRecvReadIndex ;

object *fn_kbhit (object *args, object *env) {
  (void) env;

  if(uiRecvReadIndex==uiRecvIndex) nil;

  return  tee;
}





// Symbol names
const char stringnow[]  = "now";
const char stringquit[] = "quit";
const char string_kbhit[] = "kbhit" ;

#ifdef touchscreen_support
const char stringtouch_press[] = "touch-press";
const char stringtouch_x[] = "touch-x";
const char stringtouch_y[] = "touch-y";
const char stringtouch_calibrate[] = "touch-calibrate";
const char stringtouch_setcal[] = "touch-setcal";
const char stringtouch_printcal[] = "touch-printcal";
#endif


#ifdef graphics_package
const char string_setfontname[] = "setfontname" ;
const char string_gettextwidth[] = "gettextwidth" ;
const char string_getfontheight[] = "getfontheight" ;
const char string_getfontwidth[] = "getfontwidth" ;
const char string_getfontinfo[] = "getfontinfo" ;

const char string_setviewport[] = "setviewport" ;
const char string_getx[] = "getx" ;
const char string_gety[] = "gety" ;
const char string_getmaxx[] = "getmaxx" ;
const char string_getmaxy[] = "getmaxy" ;
const char string_getscrmaxx[] = "getscrmaxx" ;
const char string_getscrmaxy[] = "getscrmaxy" ;
#endif




// Documentation strings
const char docnow[]  = "(now [hh mm ss])\n"
"Sets the current time, or with no arguments returns the current time\n"
"as a list of three integers (hh mm ss).";

const char doc_kbhit[] = "(kbhit) - test whether any keyboard keys hits.\n"
" Returns t if ney char symbols are available"
"and otherwise returnsnil.";

const char docquit[] = "(quit)\n"
"Exit from Lisp.";

#ifdef touchscreen_support
const char doctouch_press[] = "(touch-press)\n"
"Returns true if touchscreen is pressed and false otherwise.";
const char doctouch_x[] = "(touch-x)\n"
"Returns pressed touchscreen x-coordinate.";
const char doctouch_y[] = "(touch-y)\n"
"Returns pressed touchscreen y-coordinate.";
const char doctouch_calibrate[] = "(touch-calibrate)\n"
"Runs touchscreen calibration.";
const char doctouch_setcal[] = "(touch-setcal minx maxx miny maxy\n      hres vres axisswap xflip yflip)\n"
"Set touchscreen calibration parameters.";
const char doctouch_printcal[] = "(touch-printcal)\n"
"Print touchscreen calibration parameters.";
#endif


#ifdef graphics_package
const char doc_setfontname[]  = "(setfontname NAME Height Style)\n"
"Search and sets the current font with NAME, Height of symbils and Style."
"Returns t if succesful anr nil otherwise.";

const char doc_gettextwidth[]  = "(gettextwidth STRING)\n"
"Returns graphical width of STRING in pixels using current font size.\n";

const char doc_getfontheight[]  = "(getfontheight)\n"
"Returns symbols graphical height in pixels using current font size.";

const char doc_getfontwidth[]  = "(getfontwidth)\n"
"Returns symbols graphical width in pixels using current font size."
"This function works correctly for a fonts which are have regular symbols width\n";

const char doc_getfontinfo[]  = "(getfontinfo Index)\n"
"Returns information list about the font with Index."
" The list contains a name, size and stile, of the font.\n";

const char doc_setviewport[]  = "(setviewport)\n"
"Sets current view port rectangle.";

const char doc_getx[]  = "(getx)\n"
"Returns current graphical cursor x-position.";

const char doc_gety[]  = "(gety)\n"
"Returns current graphical cursor y-position.";

const char doc_getmaxx[]  = "(getmaxx)\n"
"Returns maximal graphical cursor x-position for a current viewport.";

const char doc_getmaxy[]  = "(getmaxy)\n"
"Returns maximal graphical cursor y-position for a current viewport.";

const char doc_getscrmaxx[]  = "(getscrmaxx)\n"
"Returns maximal graphical cursor x-position for full screen.";

const char doc_getscrmaxy[]  = "(getscrmaxy)\n"
"Returns maximal graphical cursor y-position for full screen.";
#endif

extern int iQuit ;

object *fn_quit (object *args, object *env)
{
  (void) env;

    iQuit = 1 ;

  return tee;
}


// Symbol lookup table
const tbl_entry_t lookup_table2[]  = {

#ifdef touchscreen_support
    { stringnow, fn_now, 0206, docnow },
    { stringtouch_press, fn_touch_press, 0200, doctouch_press },
    { stringtouch_x, fn_touch_x, 0200, doctouch_x },
    { stringtouch_y, fn_touch_y, 0200, doctouch_y },
    { stringtouch_calibrate, fn_touch_calibrate, 0200, doctouch_calibrate },
    { stringtouch_setcal, fn_touch_setcal, 0217, doctouch_setcal },
    { stringtouch_printcal, fn_touch_printcal, 0200, doctouch_printcal },
#endif

#ifdef DEF_ARRAY2
    { string_makearray2, fn_makearray2, 0215, doc_makearray2 },
    { string_delarray2, fn_delarray2, 0215, doc_delarray2 },
    { string_array2p, fn_array2p, 0211, doc_array2p },
    { string_aref2, fn_aref2, 0227, doc_aref2 },
#endif
    { string_probefile, fn_probefile, 0211, doc_probefile },
    { string_renamefile, fn_renamefile, 0222, doc_renamefile },
    { string_copyfile, fn_copyfile, 0222, doc_copyfile },
    { string_deletefile, fn_deletefile, 0211, doc_deletefile },
    { string_ensuredirectoriesexist, fn_ensuredirectoriesexist, 0211, doc_ensuredirectoriesexist },
    { string_deletedir, fn_deletedir, 0211, doc_deletedir },
    { string_uiopchdir, fn_uiopchdir, 0211, doc_uiopchdir },
    { string_uiopgetcwd, fn_uiopgetcwd, 0200, doc_uiopgetcwd },

    { string_kbhit, fn_kbhit, 0200, doc_kbhit },

#ifdef graphics_package
    { string_getimage, fn_getimage, 0244, doc_getimage },
    { string_putimage, fn_putimage, 0233, doc_putimage },
    { string_imagewidth, fn_imagewidth, 0211, doc_imagewidth },
    { string_imageheight, fn_imageheight, 0211, doc_imageheight },
    { string_loadbitmap, fn_loadbitmap, 0211, doc_loadbitmap },
    { string_savebitmap, fn_savebitmap, 0222, doc_savebitmap },
    { string_drawbitmap, fn_drawbitmap, 0233, doc_drawbitmap },
    { string_setfontname, fn_setfontname, 0233, doc_setfontname },
    { string_gettextwidth, fn_gettextwidth, 0211, doc_gettextwidth },
    { string_getfontheight, fn_getfontheight, 0200, doc_getfontheight },
    { string_getfontwidth, fn_getfontwidth, 0200, doc_getfontwidth },
    { string_getfontinfo, fn_getfontinfo, 0201, doc_getfontinfo },
    { string_setviewport, fn_setviewport, 0244, doc_setviewport },
    { string_getx, fn_getx, 0200, doc_getx },
    { string_gety, fn_gety, 0200, doc_gety },
    { string_getmaxx, fn_getmaxx, 0200, doc_getmaxx },
    { string_getmaxy, fn_getmaxy, 0200, doc_getmaxy },
    { string_getscrmaxx, fn_getscrmaxx, 0200, doc_getscrmaxx },
    { string_getscrmaxy, fn_getscrmaxy, 0200, doc_getscrmaxy },
#endif


    { stringquit, fn_quit, 0203, docquit },

#ifdef DEF_ARRAY2
	{ string_char, NULL, 0000, NULL },
    { string_singlefloat, NULL, 0000, NULL },
    { string_doublefloat, NULL, 0000, NULL },
    { string_integer, NULL, 0000, NULL },
#endif
};

// Table cross-reference functions
//extern tbl_entry_t lookup_table[];

unsigned int lookup_table2_size = arraysize(lookup_table2) ;
const tbl_entry_t *lookup_table2_ptr = lookup_table2 ;
/*extern uintptr_t lookup_table_ptr  ;
extern unsigned int lookup_table1_size ;


const tbl_entry_t *tables[] = {(tbl_entry_t*)lookup_table_ptr, lookup_table2};
unsigned int *tablesizes[] = { &lookup_table1_size, &lookup_table2_size };
*/

/*tbl_entry_t *table (int n) {
  return (tbl_entry_t *)tables[n];
}*/

/*unsigned int tablesize (int n) {
  return *tablesizes[n];
}*/
