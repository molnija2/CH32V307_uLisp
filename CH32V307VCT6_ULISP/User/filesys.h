#ifndef FILESYS_H
#define FILESYS_H



object *fn_probefile (object *args, object *env);
object *fn_deletefile (object *args, object *env);
object *fn_renamefile (object *args, object *env);
object *fn_copyfile (object *args, object *env);
object *fn_ensuredir(object *args, object *env);
object *fn_deletedir (object *args, object *env);
object *fn_uiopchdir(object *args, object *env);
object *fn_uiopgetcwd(object *args, object *env);



const char string_probefile[] = "probe-file";
const char string_deletefile[] = "delete-file";
const char string_deletedir[] = "delete-dir";
const char string_renamefile[] = "rename-file";
const char string_copyfile[] = "copy-file";
const char string_ensuredir[] = "ensure-directories-exist";
const char string_uiopchdir[] = "uiop:chdir";
const char string_uiopgetcwd[] = "uiop:getcwd";



const char doc_probefile[] = "(probe-file pathspec)\n"
"tests whether a file exists.\n"
/*" Returns nil if there is no file named pathspec,"
" and otherwise returns the truename of pathspec."*/;



const char doc_deletefile[] = "(delete-file pathspec)\n"
"delete specified file or directory.\n"
" Returns true if success and nil otherwise.";

const char doc_deletedir[] = "(delete-dir pathspec)\n"
"delete specified directory.\n"
" Returns true if success and nil otherwise.";

const char doc_renamefile[] = "(rename-file filespec newfile)\n"
"rename or moving specified file.\n"
" Returns true if success and nil otherwise.";

const char doc_copyfile[] = "(copy-file filespec newfile)\n"
"copy specified file.\n"
" Returns true if success and nil otherwise.";

const char doc_ensuredir[] = "(ensure-directories-exist pathspec)\n"
//"Tests whether the directories containing the specified file actually exist,"
"Create the directory if its not exist.\n"
" Returns true if success and nil otherwise.";

const char doc_uiopchdir[] = "(uiop:chdir pathspec)\n"
"Change current directory by specified directory.\n"
" Returns true if success and otherwise returns nil.";

const char doc_uiopgetcwd[] = "(uiop:getcwd)\n"
"Returns a string which contains current directory.";

#endif // FILESYS_H
