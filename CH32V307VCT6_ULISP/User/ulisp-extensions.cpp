#include <unistd.h>
#include <stdlib.h>
#include <string.h>


#include "ulisp.h"
//#include "graph_tcp.h"

//#include "arrays.h"
#include "ff.h"

#include "ch32v30x_rtc.h"
#include "rtc.h"

#include "ILI9488.h"
#include "XPT2046.h"

#ifdef filesys_commands
	#include "filesys.h"
#endif

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
	  if (nargs > 0) error2("Wrong number of arguments");
  
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


  return  touch_Pressed()  ? tee : nil;
#else
  return nil ;
#endif
}

object *fn_touch_x (object *args, object *env) {
  (void) env;
#ifdef  touchscreen_support
  return number(touch_Xraw());
#else
  return nil ;
#endif
}

object *fn_touch_y (object *args, object *env) {
  (void) env;
#ifdef touchscreen_support


    return number(touch_Yraw());
#else
  return nil ;
#endif
}


object *fn_touch_printcal (object *args, object *env)
{
  (void) env;

#ifdef touchscreen_support

#endif

  return tee ;
}

object *fn_touch_setcal(object *args, object *env)
{
  (void) env;

#ifdef touchscreen_support
  int hmin, hmax, vmin, vmax, rhmin, rhmax, rvmin, rvmax ;
  object *obj = args ;

  hmin = checkinteger(car(obj)) ; obj = cdr(obj);
  hmax = checkinteger(car(obj)) ; obj = cdr(obj);
  vmin = checkinteger(car(obj)) ; obj = cdr(obj);
  vmax = checkinteger(car(obj)) ; obj = cdr(obj);
  rhmin = checkinteger(car(obj)) ; obj = cdr(obj);
  rhmax = checkinteger(car(obj)) ; obj = cdr(obj);
  rvmin = checkinteger(car(obj)) ; obj = cdr(obj);
  rvmax = checkinteger(car(obj)) ; obj = cdr(obj);
  /*xyswap = checkinteger(car(obj)) ; obj = cdr(obj);
  xflip = checkinteger(car(obj)) ; obj = cdr(obj);
  yflip = checkinteger(car(obj)) ;*/

  TS2046_SetCalibration(hmin, hmax, vmin, vmax, rhmin, rhmax, rvmin, rvmax) ;
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

  if(uiRecvReadIndex==uiRecvIndex) return nil;

  return  tee;
}


object *fn_loadfont (object *args, object *env) {
  (void) env;
  #ifdef gfxsupport

  int iFont = checkinteger(first(args)) ;
  if(stringp(second(args)))
  {
	  char buffer[256] ;
	  if(LoadFont(cstring(second(args), buffer, 256), iFont)==1) return tee ;
	  else
		  pfstring("Error loading font.\n", pserial);
  }
  else
	  pfstring("Incorrect arguments.\n", pserial);

  #else
  (void) args;
  #endif
  return nil;
}


object *fn_setfont (object *args, object *env) {
  (void) env;
  #ifdef gfxsupport

  int iFont = checkinteger(first(args)) ;
  //printf("setfont %d\n", iFont) ;
  if(SetFont(iFont)) return tee ;
	else
		  pfstring("Empty font.\n", pserial);

  #else
  (void) args;
  #endif
  return nil;
}

extern FONT_INFO	*FontCurrent ;
//static FONT_INFO	FontInfo[DEF_FONTS_MAXNUMBER] ;


object *fn_gettextwidth (object *args, object *env) {
  (void) env;
  char string[256] ;

    if(stringp(car(args))) cstring(car(args), string, 256) ;
    else  {  pfstring("\nArgument must be string.", pserial); return nil; }

    int w = strlen(string) * FontCurrent->Width ;
  return number(w);
}


object *fn_getfontheight (object *args, object *env) {
  (void) env;
  int w = FontCurrent->Height ; //tcp_getfontheight() ;
  return number(w);
}

object *fn_getfontwidth (object *args, object *env) {
  (void) env;
  int w = FontCurrent->Width ; //tcp_getfontwidth() ;
  return number(w);
}

object *fn_fillpolyinit (object *args, object *env) {
  (void) env;
  void LCD_fillPolyInit();

  LCD_fillPolyInit() ;

  return tee ;
}

object *fn_fillpoly (object *args, object *env) {
  (void) env;
  int icolor = checkinteger(first(args)) ;
  void LCD_fillPoly(int color);

  LCD_fillPoly(icolor) ;

  return tee ;
}


int LCD_getRect(int16_t x, int16_t y, int16_t w, int16_t h, char fname) ;

object *fn_saverect (object *args, object *env) {
  (void) env;
  #ifdef gfxsupport
  uint16_t params[4];
  for (int i=0; i<4; i++) { params[i] = checkinteger(car(args)); args = cdr(args); }

  char pattern_string[128];
  if (stringp(car(args))) cstring(car(args), pattern_string, 256) ;
  else {
    //error(argument_must_be_string, car(args));
    return nil;
  }

  if(LCD_getRect(params[0], params[1], params[2], params[3], pattern_string)) return tee;


  #else
  (void) args;
  #endif
  return nil;
}


int LCD_putRect(int16_t x, int16_t y, char *fname) ;

object *fn_loadrect (object *args, object *env) {
  (void) env;
  #ifdef gfxsupport
  uint16_t params[2];
  for (int i=0; i<2; i++) { params[i] = checkinteger(car(args)); args = cdr(args); }
  char pattern_string[128];
  if (stringp(car(args))) cstring(car(args), pattern_string, 256) ;
  else {
    //error(argument_must_be_string, car(args));
    return nil;
  }

  if(LCD_putRect(params[0], params[1], pattern_string)) return tee;

  #else
  (void) args;
  #endif
  return nil;
}



object *fn_getfontinfo (object *args, object *env) {
  (void) env;
    object *info ;

    if(FontCurrent->DATA)
    {
      info = cons(lispstring(FontCurrent->name),cons(number(FontCurrent->Height),cons(number(FontCurrent->Style),nil)));

       return info ;
    }
    else
    	return nil ;
}



// Symbol names
const char stringnow[]  = "now";
const char string_kbhit[] = "kbhit" ;

#ifdef touchscreen_support
const char stringtouch_press[] = "touch-press";
const char stringtouch_x[] = "touch-x";
const char stringtouch_y[] = "touch-y";
//const char stringtouch_setcal[] = "touch-setcal";
//const char stringtouch_calibrate[] = "touch-calibrate";
//const char stringtouch_printcal[] = "touch-printcal";
#endif




#ifdef graphics_package
const char string_loadfont[] = "load-font";
const char string_setfont[] = "set-font";
const char string_getfontheight[] = "getfontheight" ;
const char string_getfontwidth[] = "getfontwidth" ;
const char string_fillpolyinit[] = "fill-init";
const char string_fillpoly[] = "fill-poly";
const char string_saverect[] = "save-rect";
const char string_loadrect[] = "load-rect";
const char string_getfontinfo[] = "getfontinfo" ;

const char string_gettextwidth[] = "gettextwidth" ;
const char string_setfontname[] = "setfontname" ;
const char string_setviewport[] = "setviewport" ;
const char string_getx[] = "getx" ;
const char string_gety[] = "gety" ;
const char string_getmaxx[] = "getmaxx" ;
const char string_getmaxy[] = "getmaxy" ;
const char string_getscrmaxx[] = "getscrmaxx" ;
const char string_getscrmaxy[] = "getscrmaxy" ;
#endif




// Documentation strings
/*const char docnow[]  = "(now [hh mm ss])\n"
"Sets the current time, or with no arguments returns the current time\n"
"as a list of three integers (hh mm ss).";

const char doc_kbhit[] = "(kbhit) - test whether any keyboard keys hits.\n"
" Returns t if ney char symbols are available"
"and otherwise returnsnil.";
*/


const char docnow[]  = "(now [hh mm ss [dd mon year]])\n"
"Print or sets the current time.";

const char doc_kbhit[] = "(kbhit) - test whether any keys hits.\n";


#ifdef touchscreen_support
const char doctouch_press[] = "(touch-press)\n"
"Returns true if touchscreen is pressed.";
const char doctouch_x[] = "(touch-x)\n"
"Returns pressed touchscreen x-value.";
const char doctouch_y[] = "(touch-y)\n"
"Returns pressed touchscreen y-value.";
/*const char doctouch_setcal[] = "(touch-setcal minx maxx miny maxy\n" "rminx rmaxx rminy rmaxy)\n"
"Set touchscreen calibration parameters.";
const char doctouch_calibrate[] = "(touch-calibrate)\n"
"Runs touchscreen calibration.";
const char doctouch_printcal[] = "(touch-printcal)\n"
"Print touchscreen calibration parameters.";*/
#endif


#ifdef graphics_package

const char doc_loadfont[] = "(load-font Index File)\n"
"Load font number 'Index'(0-49) from the 'File' on SDcard.";
//"Return t if success and nil otherwise";
const char doc_setfont[] = "(set-font Index)\n"
"Set font number 'Index' (0-49).""Returns t if success.";
const char doc_getfontheight[]  = "(getfontheight)\n"
"Returns symbols height in pixels.";
const char doc_getfontwidth[]  = "(getfontwidth)\n"
"Returns symbols width in pixels.";
//"This function works correctly for a fonts which are have regular symbols width\n";

const char doc_gettextwidth[]  = "(gettextwidth STRING)\n"
"Returns graphical width of STRING in pixels using current font size.\n";

const char doc_setfontname[]  = "(setfontname NAME Height Style)\n"
"Search and sets the current font with NAME, Height of symbils and Style."
"Returns t if succesful anr nil otherwise.";

const char doc_getfontinfo[]  = "(getfontinfo)\n"
"Returns information list about current font.";
//" The list contains a name, size and stile, of the font.\n";

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





// Symbol lookup table
const tbl_entry_t lookup_table2[]  = {

    { stringnow, fn_now, 0206, docnow },

#ifdef touchscreen_support
    { stringtouch_press, fn_touch_press, 0200, /*doctouch_press*/NULL },
    { stringtouch_x, fn_touch_x, 0200, /*doctouch_x*/NULL },
    { stringtouch_y, fn_touch_y, 0200, /*doctouch_y*/ NULL },
    //{ stringtouch_setcal, fn_touch_setcal, 0217, doctouch_setcal },
    //{ stringtouch_calibrate, fn_touch_calibrate, 0200, doctouch_calibrate },
    //{ stringtouch_printcal, fn_touch_printcal, 0200, doctouch_printcal },
#endif



#ifdef DEF_ARRAY2
    { string_makearray2, fn_makearray2, 0215, doc_makearray2 },
    { string_delarray2, fn_delarray2, 0215, doc_delarray2 },
    { string_array2p, fn_array2p, 0211, doc_array2p },
    { string_aref2, fn_aref2, 0227, doc_aref2 },
#endif

#ifdef filesys_commands
    //{ string_probefile, fn_probefile, 0211, /*doc_probefile*/ NULL },
    { string_renamefile, fn_renamefile, 0222, /*doc_renamefile*/ NULL},
    //{ string_copyfile, fn_copyfile, 0222, /*doc_copyfile*/NULL },
    { string_deletefile, fn_deletefile, 0211, /*doc_deletefile*/ NULL },
    { string_ensuredir, fn_ensuredir, 0211, /*doc_ensuredir*/ NULL },
    //{ string_deletedir, fn_deletedir, 0211, doc_deletedir },
    //{ string_uiopchdir, fn_uiopchdir, 0211, doc_uiopchdir },
    //{ string_uiopgetcwd, fn_uiopgetcwd, 0200, doc_uiopgetcwd },
#endif


#ifdef graphics_package
    { string_kbhit, fn_kbhit, 0200, doc_kbhit },
    { string_loadfont, fn_loadfont, 0222, doc_loadfont },
    { string_setfont, fn_setfont, 0211, doc_setfont },
    { string_getfontheight, fn_getfontheight, 0200, /*doc_getfontheight*/NULL },
    { string_getfontwidth, fn_getfontwidth, 0200, /*doc_getfontwidth*/NULL },
	{string_fillpolyinit, fn_fillpolyinit, 200, NULL},
	{string_fillpoly, fn_fillpoly, 211, NULL},
	{string_saverect, fn_saverect, 255, NULL},
	{string_loadrect, fn_loadrect, 233, NULL},
    { string_getfontinfo, fn_getfontinfo, 0200, /*doc_getfontinfo*/NULL },
    /*{ string_gettextwidth, fn_gettextwidth, 0211, doc_gettextwidth },
	{ string_getimage, fn_getimage, 0244, doc_getimage },
    { string_putimage, fn_putimage, 0233, doc_putimage },
    { string_imagewidth, fn_imagewidth, 0211, doc_imagewidth },
    { string_imageheight, fn_imageheight, 0211, doc_imageheight },
    { string_loadbitmap, fn_loadbitmap, 0211, doc_loadbitmap },
    { string_savebitmap, fn_savebitmap, 0222, doc_savebitmap },
    { string_drawbitmap, fn_drawbitmap, 0233, doc_drawbitmap },
    { string_setfontname, fn_setfontname, 0233, doc_setfontname },
    { string_setviewport, fn_setviewport, 0244, doc_setviewport },
    { string_getx, fn_getx, 0200, doc_getx },
    { string_gety, fn_gety, 0200, doc_gety },
    { string_getmaxx, fn_getmaxx, 0200, doc_getmaxx },
    { string_getmaxy, fn_getmaxy, 0200, doc_getmaxy },
    { string_getscrmaxx, fn_getscrmaxx, 0200, doc_getscrmaxx },
    { string_getscrmaxy, fn_getscrmaxy, 0200, doc_getscrmaxy },*/
#endif



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

