#include <stdio.h>
#include <string.h>

#include <unistd.h>
#include <stdlib.h>
#include <bits/stdc++.h>
#include <iostream>
#include <sys/stat.h>
#include <sys/types.h>

#include "ulisp.h"

#include "ff.h"


#define atom(x) (!consp(x))
extern object *tee;





/* Insert '/' symbol into begin of filename if it is absent.
"name" => "/name",  "home/name" => "/home/name"
*/
void test_filename(char *name) {
  int len, i ;
  char *cPtr ;
  if (name[0] == '/' ) return ;
  len = strlen(name) ;
  cPtr = &name[len] ;
  *(cPtr+1) = 0 ;

  for (i=0;i<len;i++) {
      *cPtr= *(cPtr-1) ;
      cPtr -- ;
  }

  name[0] = '/' ;
}

//const char string_mount_error[17] = "SD mount error\n";
extern const char *string_mount_error ;
const char argument_must_be_string[]= "arg. must be string.";

/*(probe-file pathspec)  tests whether a file exists.
Returns nil if there is no file named pathspec,
and otherwise returns the truename of pathspec.
*/
object *fn_probefile (object *args, object *env) {
#if defined(sdcardsupport)
  (void) env;
  int type ;  // Files and directories
  char pattern_string[256] = "*" ;
  char dirname_string[512] = "/";

  if (stringp(car(args))) cstring(car(args), pattern_string, 256) ;
  else {
    error(argument_must_be_string, car(args)); return nil;
  }

    cstring(car(args), dirname_string, 256) ;
    if(dirname_string[strlen(dirname_string)-1] == '/')
    {
        dirname_string[strlen(dirname_string)-1] = 0x0 ;
        type = 0x04;
    }
    else type = 0x8 ;

    char *pattern_bgn = &dirname_string[strlen(dirname_string)-1] ;
    while((pattern_bgn!=dirname_string)&&(*pattern_bgn!='/')) pattern_bgn -- ;

    if(*pattern_bgn=='/')
    {
       *pattern_bgn = 0x0 ;
       pattern_bgn ++ ;
       strcpy(pattern_string, pattern_bgn);
    }
    /*else
    {
        strcpy(pattern_string, dirname_string);
        getcwd(dirname_string, 256);
    }*/




#ifdef LINUX_X64
  DIR *Dir;
  Dir=opendir(dirname_string);
  if(Dir==NULL){  pfstring("problem reading from SD card", pserial); return nil; }

#else

  FIL File;
  FATFS fatfs;
  FRESULT fres;
  DIR dir ;
  //FILINFO fileinfo ;

  fres = f_mount(&fatfs, "", 1);
  if(fres != FR_OK) error2(string_mount_error);

  delay(50) ;



  fres = f_open(&File, pattern_string, FA_OPEN_EXISTING | FA_READ);
  if(fres == FR_OK)
  {
	  f_close(&File);
	  object *filename = lispstring((char*)pattern_string);
  	  fres =  f_unmount("");
  	  if(fres != FR_OK)
  	  	return nil ;

  	  return filename;
  }

#endif


#ifdef LINUX_X64
  closedir(Dir);
#else
  f_closedir (&dir);
  fres =  f_unmount("");
#endif

  return nil;
#else
  (void) args, (void) env;
  error2("not supported");
  return nil;
#endif
}



/* (delete-file pathspec)   delete specified file.
Returns true if success and otherwise returns nil.
*/
object *fn_deletefile (object *args, object *env) {
#if defined(sdcardsupport)
  (void) env;
  char pattern_string[256] ;
  object* obj ;

  if (stringp(car(args))) cstring(car(args), pattern_string, 256) ;
  else {
    error(argument_must_be_string, car(args)); return nil;
  }

  test_filename(pattern_string) ;


#ifdef LINUX_X64
  DIR *Dir;
  Dir=opendir(dirname_string);
  if(Dir==NULL){  pfstring("problem reading directory from SD card", pserial); return nil; }

#else
  FATFS fatfs;
  FRESULT fres;

  fres = f_mount(&fatfs, "", 1);
  if(fres != FR_OK) error2(string_mount_error);

  delay(50) ;

	  fres = f_unlink (pattern_string);
	  if(fres == FR_OK) obj = tee ;
	  else obj = nil ;

#endif


#ifdef LINUX_X64
  closedir(Dir);
#else

  fres =  f_unmount("");

#endif

  return obj;
#else
  (void) args, (void) env;
  error2("not supported");
  return nil;
#endif
}




/* (delete-dir pathspec)   delete specified directory.
Returns true if success and otherwise returns nil.
*/
object *fn_deletedir (object *args, object *env) {
#if defined(sdcardsupport)
  (void) env;
    char pattern_string[256] ;

    if (stringp(car(args))) cstring(car(args), pattern_string, 256) ;
    else {
      error(argument_must_be_string, car(args)); return nil;
    }

    test_filename(pattern_string) ;


  #ifdef LINUX_X64
    DIR *Dir;
    Dir=opendir(dirname_string);
    if(Dir==NULL){  pfstring("problem reading directory from SD card", pserial); return nil; }

  #else
    //FIL File;
    FATFS fatfs;
    FRESULT fres;

    fres = f_mount(&fatfs, "", 1);
    if(fres != FR_OK) error2(string_mount_error);

    delay(50) ;

  	  fres = f_unlink(pattern_string);
  	  if(fres != FR_OK)
  	  {
  	  	fres =  f_unmount("");
   	  	return nil ;
  	  }



  #endif


  #ifdef LINUX_X64
    closedir(Dir);
  #else

    fres =  f_unmount("");

  #endif

    return tee;
  #else
    (void) args, (void) env;
    error2("not supported");
    return nil;
  #endif
}



#include <filesystem>  // std::filesystem::rename
#include <string_view> // std::string_view
using namespace std;



/* (rename-file filespec newfile)  rename or moving specified file.
Returns true if success and otherwise returns nil.
*/
object *fn_renamefile (object *args, object *env) {
#if defined(sdcardsupport)
  (void) env;

  char filename_string[256] ;
  char newname_string[256] ;

  if(stringp(car(args))) cstring(car(args), filename_string, 256) ;
  else  {  error(argument_must_be_string, car(args)); return nil; }

  args = cdr(args);
  if(stringp(car(args)))
        cstring(car(args), newname_string, 256) ;
  else  {  error(argument_must_be_string, car(args)); return nil; }

  test_filename(filename_string) ;
  test_filename(newname_string) ;


#ifdef LINUX_X64
  if (rename(filename_string, newname_string) != 0)
        error("cannot rename file", car(args));
      else
        return tee ;

#else

  FATFS fatfs;
  FRESULT fres;

  object* obj ;

  fres = f_mount(&fatfs, "", 1);
  if(fres != FR_OK) error2(string_mount_error);

  delay(50) ;


  fres = f_rename (filename_string, newname_string);
  if(fres != FR_OK){  pfstring("Cannot rename file", pserial); obj = nil ; }
  else
	  obj = tee ;

#endif


#ifdef LINUX_X64
  closedir(Dir);
#else

  fres =  f_unmount("");

#endif

  return obj;

#else
  (void) args, (void) env;
  error2("not supported");
  return nil;
#endif
}




/* (copy-file filespec newfile)  copy specified file.
Returns true if success and otherwise returns nil.
*/
object *fn_copyfile (object *args, object *env) {
#if defined(sdcardsupport)
  (void) env;

  char filename_string[256] ;
  char newname_string[256] ;

  if(stringp(car(args))) cstring(car(args), filename_string, 256) ;
  else  {  pfstring(argument_must_be_string, pserial); return nil; }

  args = cdr(args);
  if(stringp(car(args)))
        cstring(car(args), newname_string, 256) ;
  else  {  pfstring(argument_must_be_string, pserial); return nil; }

  test_filename(filename_string) ;
  test_filename(newname_string) ;


#ifdef LINUX_X64
  FILE *fp_src, *fp_dest;

  fp_src = fopen(filename_string,"rb");
  if(!fp_src){  pfstring("\ncopy-file: Cannot open source file.", pserial); return nil; }
  fp_dest = fopen(newname_string,"wb");
  if(!fp_dest){ fclose(fp_src); pfstring("\ncopy-file: Cannot open source file.", pserial); return nil; }
  char b[8] ;
  while(0 == feof(fp_src)){
      fread(b, 1, 1, fp_src) ;
      fwrite(b, 1, 1, fp_dest) ;
  }


#else

  FIL FileSource, FileDest;
  FATFS fatfs;
  FRESULT fres;

  fres = f_mount(&fatfs, "", 1);
  if(fres != FR_OK) error2(string_mount_error);

  delay(50) ;

  fres = f_open(&FileSource, filename_string,  FA_OPEN_EXISTING | FA_READ);
  if(fres != FR_OK){
	  pfstring("Cannot open source file", pserial);
	  fres =  f_unmount("");
	  return nil ;
  }

  fres = f_open(&FileDest, newname_string,  FA_CREATE_ALWAYS | FA_WRITE);
  if(fres != FR_OK){
	  pfstring("Cannot open destination file", pserial);
	  fres = f_close(&FileSource) ;
	  fres =  f_unmount("");
	  return nil ;
  }


  char buff[256] ;
  UINT br, bw ;


  while((fres = f_read (&FileSource,  buff, 256, &br ))== FR_OK)
  {
	  fres = f_write (&FileSource,  buff, br, &bw );
  }

  f_sync(&FileDest);
  fres = f_close(&FileDest) ;
  fres = f_close(&FileSource) ;
  fres =  f_unmount("");

#endif



  return tee;
#else
  (void) args, (void) env;
  error2("not supported");
  return nil;
#endif
}






/* (ensure-directories-exist pathspec)   Tests whether the specified
directories actually exist, and attempts to create them if they do not.
Returns true if success and otherwise returns nil.
*/
object *fn_ensuredir(object *args, object *env) {
#if defined(sdcardsupport)
  (void) env;
  char dirname_string[256] = "/";

  if(stringp(car(args))) cstring(car(args), dirname_string, 256) ;
  else  {
      error(argument_must_be_string, car(args));
      return nil;
  }

  test_filename(dirname_string) ;

#ifdef LINUX_X64
  DIR *Dir;
  Dir=opendir(dirname_string);
  if(Dir==NULL) {
    if(-1==mkdir(dirname_string,0777)) {
        error("problem to create directory", car(args));
        return nil;
    }
    else
        return tee ;
  }

  closedir(Dir) ;
  return tee;

#else
  FATFS fatfs;
  FRESULT fres;

  fres = f_mount(&fatfs, "", 1);
  if(fres != FR_OK) return nil ;//error2(string_mount_error);

  fres = f_mkdir (dirname_string);
  if(fres != FR_OK){
	  fres =  f_unmount("");
	  return nil ;
  }

#endif


#ifdef LINUX_X64
  closedir(Dir);
#else
  fres =  f_unmount("");
#endif

  return tee;

#else
  (void) args, (void) env;
  error2("not supported");
  return nil;
#endif
}



object *fn_uiopchdir(object *args, object *env) {
  (void) env;
  char dirname_string[256] ;

  if(stringp(car(args))) cstring(car(args), dirname_string, 256) ;
  else  {
      error(argument_must_be_string, car(args));
      return nil;
  }


    /*if(-1==chdir(dirname_string)) {
        error("Cannot to change directory", car(args));
        return nil;
    }*/

  return tee;
}


object *fn_uiopgetcwd(object *args, object *env) {
  (void) env;
  char dirname_string[256] ;

   //getcwd(dirname_string, 256);
   object *dirname = lispstring((char*)dirname_string) ;

  return dirname;
}


