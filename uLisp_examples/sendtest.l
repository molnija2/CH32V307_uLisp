(progn

(defun rgb (r g b)
 (logior (ash (logand r #xf8) 8) (ash (logand g #xfc) 3) (ash b -3)))

 ; Define Editor screen size
 (defvar ListEditorWidth 320)
 (defvar ListEditorHeight 480)

; Define current font character size
 (defvar FontWidth (getfontwidth))
 (defvar FontHeight (getfontheight))

 (defvar ColorBk_LineEditor (rgb 255 255 255))
 (defvar ColorBk_Editor (rgb 50 50 50))
 (defvar ColorBk_EditorH (rgb 200 100 100))
 (defvar Color_LineEditor (rgb 0 0 0))
 (defvar Color_CursorEditor (rgb 255 255 255))
 (defvar ColorBk_EditorH (rgb 180 50 50))

(defun key-forUp (key)    (if (or (eq key 16) (eq key 16) (eq key 19)) t nil))
(defun key-forDown (key)  (if (or (eq key 14) (eq key 14) (eq key 21)) t nil))
(defun key-forLeft (key)  (if (or (eq key 2) (eq key 2) (eq key 18)) t nil))
(defun key-forRight (key) (if (or (eq key 6) (eq key 6) (eq key 20)) t nil))
(defun key-forPgUp  (key) (if (or (eq key 23) (eq key 23) (eq key 122)) t nil))
(defun key-forPgDn  (key) (if (or (eq key 22) (eq key 22) (eq key 123)) t nil))
(defun key-forHelp  (key) (if (or (eq key 104) (eq key 72) (eq key 48)) t nil))
(defun key-forPrEd  (key) (if (or (eq key 108) (eq key 76) (eq key 51)) t nil))
(defun key-forQuit  (key) (if (or (eq key #\Q) (eq key #\q) (eq key 17) (eq key 57)) t nil))

(defun program-editor  (lst) nil)

 (defun read-key ()
   (read-byte)
 )


(defvar listedit_help '(
"   Editor control keys:"
"h - read this text"
"Ctrl-b Backward, Ctrl-f Forward,"
"Ctrl-p PrevLine, Ctrl-n NextLine,"
"Ctrl-v PgDn, Ctrl-w PgUp"
"e - edit element"
"Backspace - delete character"
"a - append element to current list"
"i - insert element into current list"
"d - delete element from current list"
"    and save it in the buffer"
"c - element (list) into the buffer"
"p - paste-insert element from the buffer"
"t - paste-append element from the buffer"
"Enter - input into the list"
"l - run program-editor for the list"
"Ctrl-q - exit"
" "
"press any key to return"))


(defun PrintHelp ()
 (let ((index 0) (colorbk (rgb 150 0 0)))
  (with-gfx (str)
    (fill-rect 0 0 ListEditorWidth (- ListEditorHeight FontHeight) colorbk )
    (set-text-color (rgb 255 255 255) colorbk)
      (dolist (x listedit_help)
        (set-cursor 0 (* index (+ FontHeight 2)))
        (write-string x str)
        (setq index (1+ index))
      )
    )
  )

  (read-key)
)


; Draw string
  (defun draw-string (s)
    (if (stringp s)
       ( with-gfx (str) (write-string s str))
    )
  )


;  Select existing symbol

; Print the items of list
  (defun print-list2 (lst  scrolly)
    (let ((len (length lst)) (curx 2) (cury 0) (index 0)
          (WindowHeight (- ListEditorHeight (* (getfontheight) 2)))
          (WindowWidth (truncate (/ ListEditorWidth FontWidth))) )

     (fill-rect 1 1 (- ListEditorWidth 2) (- WindowHeight 2) Color_LineEditor )

     (if (> len 0)
       (progn
         (setq index 0)
         (dolist (x lst)
           (if (and  (<  (- cury scrolly) (- WindowHeight (getfontheight)))
               (>= (- cury scrolly) 0))
             (progn
               (set-cursor curx (+ (- cury scrolly) 2) )
               (set-text-color ColorBk_LineEditor Color_LineEditor)
               (draw-string  (format nil "~a" x))
             )
           )
           (setq cury (+ cury (+ (getfontheight) 2)))
           (setq index (1+ index))
         )
       )
     )
    )
  )
  ; Print the list items positions
    (defun selector-print-cursor (lst scrolly  cursor_index  bkcolor)
      (let ((curx 2) (cury 0)
            (index 0) (item (nth cursor_index lst))
            (WindowHeight (- ListEditorHeight (* (getfontheight) 2)))
            (WindowWidth (truncate (/ ListEditorWidth FontWidth))))

       (if (> len 0)
         (progn
           (setq cury (*  cursor_index (+ (getfontheight) 2)))
           (cond
             ((and  (<  (- cury scrolly) WindowHeight)
                    (>= (- cury scrolly) 0)
                    (/= bkcolor -1))
               (set-cursor curx (+ (- cury scrolly) 2) )
               (if (= bkcolor 0)
                (set-text-color ColorBk_LineEditor Color_LineEditor)
                (set-text-color Color_LineEditor ColorBk_LineEditor)
               )
               (draw-string  (format nil "~a" item))
             )
             (t  nil)
           )
         )
       )
       cury
     )
    )

   (defun editor-selector (pattern_str)
      (let* ((lst (apropos-list pattern_str)) (scrolly 0) (posy 0)
             (len (length lst)) (cursor_item 0)  (ret_item nil) (window 0)
             (WindowHeight (- ListEditorHeight (* (getfontheight)  2)))
             (page_rowsn 0))

       (setq page_rowsn  (- (truncate (/ ListEditorHeight (+ (getfontheight) 2))) 3))

       (setq window (get-image 0 0 ListEditorWidth (+ WindowHeight 2) ))
       (draw-rect 0 0 (1- ListEditorWidth)  (1- WindowHeight)  ColorBk_LineEditor )
       (print-list2 lst scrolly)

       (loop
         (selector-print-cursor lst scrolly  cursor_item  1)
         (set-cursor 0 (- ListEditorHeight (getfontheight)))
         (setq key (read-key))
         (selector-print-cursor lst scrolly  cursor_item  0)
         (if (key-forQuit key)
             (return )
         )

         (cond
           ((key-forUp key)
             (if (> cursor_item 0)
               (setq cursor_item (1- cursor_item)))
             (setq posy (selector-print-cursor lst scrolly  cursor_item  -1))
             (if (< (- posy scrolly)  0)
              (progn
               (setq scrolly (- scrolly (+ (getfontheight) 2)))
               (print-list2 lst scrolly)
             ))
           )
           ((key-forDown key)
             (if (< cursor_item (1- len))
               (setq cursor_item (1+ cursor_item)))
             (setq posy (selector-print-cursor lst scrolly  cursor_item  -1))
             (if (> (- posy scrolly)  (- WindowHeight (getfontheight)) )
               (progn
                 (setq scrolly (+ scrolly (+ (getfontheight) 2)))
                 (print-list2 lst scrolly)))
           )

           ((key-forPgUp key)
             (setq cursor_item (- cursor_item page_rowsn))
             (if (< cursor_item 0) (setq cursor_item 0))
             (setq posy (selector-print-cursor lst scrolly  cursor_item  -1))
             (if (< (- posy scrolly)  0)
              (progn
               (setq scrolly posy)
               (print-list2 lst scrolly)
             ))
           )

           ( (key-forPgDn key)
             (setq cursor_item (+ cursor_item page_rowsn))
             (if (> cursor_item (1- len)) (setq cursor_item (1- len)))
             (setq posy (selector-print-cursor lst scrolly  cursor_item  -1))
             (if (> (- posy scrolly) WindowHeight)
                (progn
                   (setq scrolly (+ scrolly (* page_rowsn (+ 2 (getfontheight)))))
                   (print-list2 lst scrolly)))
           )

           ((eq key 13)
             (print-list2 lst scrolly)
             (setq ret_item (nth cursor_item lst))
             (return )
           )
           (t nil)
         )
        )
        (put-image 0 0 window)
        (del-array* window)
        ret_item
      )
   )


)
