(progn

(defun rgb (r g b)
 (logior (ash (logand r #xf8) 8) (ash (logand g #xfc) 3) (ash b -3)))
 
; Define Editor screen size
 (defvar ListEditorWidth (getscrmaxx))
 (defvar ListEditorHeight (getscrmaxy))

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

(defun getfontheight () FontHeight)


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



; Copy the object or list into buffer

 (defVar object_buffer nil)

 (defun copy-to-buffer (lst delf)
    (cond
      ( (atom lst)
        (setq object_buffer lst)
      )
      (t
        (if (eq delf 0)
            (setf object_buffer (copy-list lst))
            (setf object_buffer lst))
      )
    )
 )



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
      (let* ((key 0) (lst (apropos-list pattern_str)) (scrolly 0) (posy 0)
             (len (length lst)) (cursor_item 0)  (ret_item nil) (window 0)
             (WindowHeight (- ListEditorHeight (* (getfontheight)  2)))
             (page_rowsn 0))

       (setq page_rowsn  (- (truncate (/ ListEditorHeight (+ (getfontheight) 2))) 3))

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
        (draw-rect 0 0 (1- ListEditorWidth)  (1- WindowHeight)  ColorBk_LineEditor )
        ret_item
      )
   )
  
  

;  Set Line editor Height for FontHeight + cursor underline
  (defun line-editor-Height () (+ FontHeight 2))

; Text line editor for atom or list with limited lenght of string
  (defun line-editor  (posx posy item_org)
    (let* ((str_item
             (if (stringp item_org)
                 (format nil "~a~a~a" (code-char 34) item_org (code-char 34))
                 (string  item_org)))
           (key 0) (len (length str_item))
           (cursor_char len) (return_flag 0) (first_key 1))
      (set-cursor posx posy)
      (fill-rect posx posy (- ListEditorWidth posx) (line-editor-Height) ColorBk_LineEditor )
      (set-text-color ColorBk_LineEditor Color_LineEditor)
      (draw-string (subseq str_item 0 len) )

      (loop
        (fill-rect (+ posx (* cursor_char FontWidth)) (+ posy FontHeight) FontWidth 2 Color_LineEditor )
        (set-cursor 0 (- ListEditorHeight FontHeight))
        (setq key (read-key))

        (fill-rect (+ posx (* cursor_char FontWidth)) (+ posy FontHeight) FontWidth 2 ColorBk_LineEditor )

        (if (eq first_key 1)
         (progn
            (set-cursor posx posy)
            (set-text-color Color_LineEditor ColorBk_LineEditor)
            (draw-string (subseq str_item 0 len) )
         )
        )

        (set-cursor posx posy)
        (if (eq key 0)
         (progn
          (set-cursor 0 (- ListEditorHeight FontHeight))
          (setq key (read-key))

          (cond
           ((key-forLeft key) (if (> cursor_char 0) (setq cursor_char (1- cursor_char))))
           ((key-forRight key) (if (< cursor_char len) (setq cursor_char (1+ cursor_char))))
           ((key-forQuit key) (setq return_flag 1) )
           ((eq key 7)
             (if (< cursor_char len)
              (setq str_item
               (concatenate 'string
                 (subseq str_item 0  cursor_char)
                 (subseq str_item (1+ cursor_char) len))))
             (if (eq first_key 1)
               (progn
                 (setq first_key 0)
                 (setq str_item "")
                 (setq cursor_char 0)
               )
             )

             (fill-rect 0 posy ListEditorWidth (line-editor-Height) ColorBk_LineEditor )
             (setq len (length str_item))
             (set-cursor posx posy)
             (set-text-color Color_LineEditor ColorBk_LineEditor)
             (draw-string (subseq str_item 0 len) )
           )
           (t (set-text-color ColorBk_LineEditor ColorBk_Editor)
              (set-cursor 0 (- ListEditorHeight FontHeight))
              (format t "0+~a " key))
          )
          (setq first_key 0)
         )
         (progn
          (if (and (eq first_key 1) (not (eq key 13)) (not (eq key 8)) (not (key-forLeft key)) (not (key-forRight key)) )
           (progn
            (setq str_item "")
            (setq cursor_char 0)
            (setq len 0)
            (fill-rect 0 (- ListEditorHeight (line-editor-Height)) ListEditorWidth (line-editor-Height) ColorBk_LineEditor )
           )
          )
          (setq first_key 0)
          (cond
           ((key-forQuit key) (setq return_flag 1) )
           ((key-forUp key)
                 (let ( (item (editor-selector (subseq str_item 0 len))))
                   (if (not (eq item nil) )
                     (progn
                       (setq str_item (string item))
                       (setq len (length str_item))
                       (setq cursor_char len)
                       (set-cursor posx posy)
                       (fill-rect posx posy (- ListEditorWidth posx) (line-editor-Height) ColorBk_LineEditor )
                       (set-text-color Color_LineEditor ColorBk_LineEditor)
                       (draw-string (subseq str_item 0 len) )
                     )
                   )
                 )
           )
           ((key-forLeft key) (if (> cursor_char 0) (setq cursor_char (1- cursor_char))))
           ((key-forRight key) (if (< cursor_char len) (setq cursor_char (1+ cursor_char))))
           ((eq key 8)
            (if (> cursor_char 0)
             (progn
              (setq str_item
               (concatenate 'string
                 (subseq str_item 0 (1- cursor_char))
                 (subseq str_item cursor_char len)))
              (setq cursor_char (1- cursor_char))
              (fill-rect 0 (- ListEditorHeight (line-editor-Height)) ListEditorWidth (line-editor-Height) ColorBk_LineEditor )
             ))
           )
           (t
              (if (eq key 13)
                (progn
                  (setq return_flag 2)
                  (setq cursor_char len) )
              )
              (setq str_item
               (concatenate 'string
                 (subseq str_item 0 cursor_char)
                 (string (code-char key))
                 (subseq str_item cursor_char len)))
                 (setq cursor_char (1+ cursor_char))
           )
          )
          (setq len (length str_item))
          (set-cursor posx posy)
          (set-text-color Color_LineEditor ColorBk_LineEditor)
          (draw-string (subseq str_item 0 len) )
         )
        )
        (if (not (eq return_flag 0)) (return))
      )
      (if (eq return_flag 1) nil)
      (if (eq return_flag 2) (read-from-string str_item) nil)
    )
  )


; Bottom line displays current list which is selected by cursor
  (defun display-editor-item  (posx posy item)
     (let* ((str_item (string item)) (len (length str_item)) (len_org len))
      (set-cursor posx posy)
      (set-text-color Color_LineEditor ColorBk_LineEditor)
      (if (> len 32) (setq len 32))
      (draw-string (subseq str_item 0 len) )
      len_org
    )
  )

  (defvar Color_atom  (rgb 255 255 255))
  (defvar Color_listvar  (rgb 150 255 150))
  (defvar Color_list  (rgb 200 255 200))
  (defvar Color_unbound (rgb 0 0 255))
  (defvar Color_int  (rgb  220 255 255))
  (defvar Color_float  (rgb 255 200 255))
  (defvar Color_string (rgb 255 200 200))
  (defvar Color_stringvar (rgb 255 210 210))
  (defvar Color_array  (rgb 250 255 220))
  (defvar Color_array2  (rgb 255 240 200))




; Mark list items by colors depend of is it atom or list
  (defun item-color-set (item bkcolor)
    (cond
     ((atom item)
       (if (boundp (quote item))
         (cond
          ((floatp  item)  (set-text-color Color_float bkcolor))
          ((numberp  item) (set-text-color Color_int bkcolor))
          ((stringp  item) (set-text-color Color_string bkcolor))
          (t   (set-text-color Color_atom bkcolor))
         )
       )
     )
     (t (set-text-color Color_list bkcolor))
    )
  )


  (defun item-get-string (item &optional maxlen)
   (cond
    ( (atom item)
      (if (stringp item)
         (format nil "~a~a~a" (code-char 34) item (code-char 34))
         (string  item))
    )
    (t
      (if (eq nil maxlen) (setq maxlen 32))
      (let* ((str_item (string item)) (len (length str_item)) )
       (if (> len maxlen) (setq len maxlen))
       (subseq str_item 0 len)
      )
    )
   )
  )

; Print the list items positions evenly for one column of fixed width
  (defun print-list-editor (lst  scrolly)
    (let ((len (length lst))
          (curx 0) (cury 0) (index 0)
          (WindowHeight (- ListEditorHeight (* FontHeight 2)))
          (WindowWidth (truncate (/ ListEditorWidth FontWidth))) )


     (fill-rect 0 0 ListEditorWidth ListEditorHeight bkcolor0 )

     (if (> len 0)
       (progn
         (setq index 0)
         (dolist (x lst)
           (if (and  (<  (- cury scrolly) WindowHeight)
               (>= (- cury scrolly) 0))
             (progn
               (set-cursor 0 (- cury scrolly))
               (item-color-set x bkcolor0)
               (draw-string   (item-get-string x WindowWidth))
             )
           )
           (setq cury (+ cury (+ FontHeight 2)))
           (setq index (1+ index))
         )
       )
     )
    )
  )


; Print the list items positions evenly for one column of fixed width
  (defun print-lsted-cursor (lst scrolly  cursor_index  bkcolor)
    (let ((len (length lst))
          (curx 0) (cury 0)
          (index 0) (item (nth cursor_index lst))
          (WindowHeight (- ListEditorHeight (* (+ (getfontheight) 2) 2)))
          (WindowWidth (truncate (/ ListEditorWidth FontWidth))))

     (if (> len 0)
       (progn
         (setq cury (*  cursor_index (+ (getfontheight) 2)))
         (cond
           ((and  (<  (- cury scrolly) WindowHeight)
                  (>= (- cury scrolly) 0)
                  (/= bkcolor -1))
             (set-cursor 0 (- cury scrolly))
             (item-color-set item bkcolor)
             (draw-string (item-get-string item WindowWidth))
           )
           (t  nil)
         )
       )
     )
     cury
   )
  )





   (defun list-editor (lst &optional level)
       (let ((len (length lst)) (item_edit 0) (changed_flag 0)
             (curx 0) (cury 0) (key 0) (scrolly 0) (posy 0) (page_rowsn 0)
             (bkcolor0 (rgb 50 50 50)) (bkcolor1 (rgb 200 100 100))
             (index 0) (cursor_item 0)
             (item_str_length 0) (scrolly 0) (window 0) )

    (setf ListEditorWidth (getscrmaxx))
    (setf ListEditorHeight (getscrmaxy))


     (if (eq level nil)  (setq level 0) )
     (setq page_rowsn  (- (truncate (/ ListEditorHeight (+ (getfontheight) 2))) 2))
     (print-list-editor lst scrolly)

     (loop
        (print-lsted-cursor lst  scrolly  cursor_item  bkcolor1)

        (fill-rect (* FontWidth 5) (- ListEditorHeight FontHeight) (- ListEditorWidth (* FontWidth 5)) FontHeight bkcolor0 )

        (set-cursor 0 (- ListEditorHeight FontHeight))
        (item-color-set (nth cursor_item lst) bkcolor0)

          (setq key (read-key))

        (set-cursor 0 (- ListEditorHeight FontHeight))
        (item-color-set (nth cursor_item lst) bkcolor0)
        (draw-string (format nil "~a  " key))

        (setq posy (print-lsted-cursor lst scrolly  cursor_item  bkcolor0))

        (if (key-forQuit key)
            (return )
        )

        (cond
          ((key-forUp key)
            (if (> cursor_item 0)
              (setq cursor_item (1- cursor_item)))
            (setq posy (print-lsted-cursor lst scrolly  cursor_item  -1))
            (if (< (- posy scrolly)  0)
             (progn
              (setq scrolly (- scrolly (+ (getfontheight) 2)))
              (print-list-editor lst scrolly)
            ))
          )
          ((key-forDown key)
            (if (< cursor_item (1- len))
              (setq cursor_item (1+ cursor_item)))
            (setq posy (print-lsted-cursor lst scrolly  cursor_item  -1 ))
            (if (> (- posy scrolly)  (- ListEditorHeight (* (+ (getfontheight) 2) 2) ))
              (progn
                (setq scrolly (+ scrolly (+ (getfontheight) 2)))
                (print-list-editor lst scrolly)))
          )

          ((key-forPgUp key)
            (setq cursor_item (- cursor_item page_rowsn))
            (if (< cursor_item 0) (setq cursor_item 0))
            (setq posy (print-lsted-cursor lst scrolly  cursor_item  -1))
            (if (< (- posy scrolly)  0)
             (progn
              (setq scrolly posy)
              (print-list-editor lst scrolly)
            ))
          )

          ( (key-forPgDn key)
            (setq cursor_item (+ cursor_item page_rowsn))
            (if (> cursor_item (1- len)) (setq cursor_item (1- len)))
            (setq posy (print-lsted-cursor lst scrolly  cursor_item  -1))
            (if (> (- posy scrolly)
                   (- ListEditorHeight (* (+ 2 (getfontheight)) 2)))
               (progn
                  (setq scrolly (+ scrolly (* page_rowsn (+ 2 (getfontheight)))))
                  (print-list-editor lst scrolly)))
          )

          ((eq key 13)
            (if (or (not (atom (nth cursor_item lst))) (eq nil (nth cursor_item lst)))
             (progn
              (setq item_edit (list-editor (nth cursor_item lst) (1+ level)))
              (if (not (eq item_edit nil))
               (progn
                (setf (nth cursor_item lst) item_edit)
                (setq changed_flag 1)
               )
              )
             )
            )
            (print-list-editor lst scrolly)
          )

          ;  Run and eval
          ((or (eq key 114) (eq key 82))
            (ignore-errors
             (let* ( (str (string (eval (nth cursor_item lst)))) (len (length str)))
               (if (> len 32) (setq len 32))
               (set-cursor 0 0)
               (set-text-color  ColorBk_Editor (rgb 255 127 255))
               (draw-string (format nil " Ret=~a " (subseq str 0 len)))
             )
            )

            (setq key (read-key))

            (print-list-editor lst scrolly)
          )

          ; program-editor launch
          ((key-forPrEd key)
            (if (not (atom (nth cursor_item lst)))
             (progn
              (setq item_edit (program-editor (nth cursor_item lst) ))
              (if (not (eq item_edit nil))
               (progn
                (setf (nth cursor_item lst) item_edit)
                (setq changed_flag 1)
               )
              )
             )
            )
            (print-list-editor lst scrolly)
          )

          ((or (eq key 68) (eq key 100))
            (copy-to-buffer (nth cursor_item lst) 0)
            (setq lst (append (subseq lst 0 cursor_item) (subseq lst (1+ cursor_item))
                      )
            )
            (setq changed_flag 1)
            (setq len (length lst))

            (if (and (>= cursor_item len) (> cursor_item 0)) (setq cursor_item (1- cursor_item)))
            (print-list-editor lst scrolly)
          )

          ;  c - copy object o buffer
          ( (or (eq key 99) (eq key 67))
            (copy-to-buffer (nth cursor_item lst) 0)
          )


          ;  p - Paste object   t - paste-append to tail
          ((and (or (eq key 112) (eq key 80) (eq key 116) (eq key 74)) (not (eq object_buffer nil)) )

            (if (or (eq key 116) (eq key 74))
                (setf lst (append (subseq lst 0) '(nil)))
                (setf lst (append (subseq lst 0 cursor_item)
                        '(nil) (subseq lst cursor_item)))
            )

            (if (or (eq key 116) (eq key 74))
                (setq cursor_item (1- (length lst)))
            )
            (setf (nth cursor_item lst) object_buffer)

            (if (consp object_buffer)
               (setq object_buffer nil)
            )

            (setq changed_flag 1)
            (setq len (length lst))
            (print-list-editor lst scrolly)
          )


          ((or (eq key 73) (eq key 105) (eq key 97) (eq key 65))

            (if (or (eq key 97) (eq key 65))
              (progn
                (setq cursor_item len)
                (setq lst (append (subseq lst 0) '(nil)))
              )
              (setq lst (append (subseq lst 0 cursor_item) '(nil) (subseq lst cursor_item)))
            )
            (setq item_edit (line-editor (* FontWidth 5) (- ListEditorHeight (line-editor-Height)) (nth cursor_item '())))
            (if (not (eq item_edit nil))
              (setf (nth cursor_item lst) item_edit)
            )

            (setq changed_flag 1)
            (setq len (length lst))
            (print-list-editor lst scrolly)
          )

          ((and (or (eq key 69) (eq key 101)) (< item_str_length 128))
            (setq item_edit (line-editor (* FontWidth 5) (- ListEditorHeight (line-editor-Height)) (nth cursor_item lst)))

            (if (not(eq item_edit nil))
              (progn
                (setq changed_flag 1)
                (setf (nth cursor_item lst) item_edit)
              ))

            (print-list-editor lst scrolly)
          )
          ((key-forHelp key)
           (PrintHelp)
           (print-list-editor lst scrolly)
          )
          (t nil)
        )

      )
      (set-cursor 0 (- ListEditorHeight FontHeight))

      (if (eq changed_flag 1) lst nil)
    )
  )



(defun print-spaces (n s)
  (dotimes (i n) (write-string " " s) )
)


;  Print program into GFX or SDCADR stream
  (defun print-list  (lst0 list_level len s)
    (progn
       (write-byte 10 s)
       (write-byte 13 s)
       (print-spaces list_level s)
       (setq len list_level)
       (write-string "(" s)
    )
    (let ((index 0) (lst_len (length lst0)))
     (dolist (lst lst0)
      (cond
        ((atom lst)
         (let* ((str_item (string  lst))
             (item_len (length str_item)))
          (setq len (+ len item_len))
          (if (>= len (+ list_level 64))
            (progn
              (setq len list_level)
              (write-byte 13 s)
              (print-spaces list_level s))
          )
          (if (> index 0) (write-string " " s))
          (if (stringp lst) (write-byte 34 s))
          (write-string str_item s)
          (if (stringp lst) (write-byte 34 s))
         )
        )
        (t
          (print-list  lst (1+ list_level) (1+ len) s)
        )
     )
     (setq index (1+ index)))
    )
    (write-string ")" s)
  )


 (defun save-program (lst filename)
  (if (consp lst)
   (with-sd-card (s filename 2) (print-list lst 0 0 s))
  )
 )

  (ignore-errors (defvar prog_editor (load-program "prog_editor.l")))
  (ignore-errors (eval prog_editor))
  
  (defun load-list (filename)
    (let ((lst nil))
       (ignore-errors
            (setq lst (with-sd-card (s filename) (read s)))
       )
     (if (consp lst) lst  nil)
    )
  )
 
  (defun edit-prog (filename)
    (let ((listdata (load-list filename)))
       (if (eq listdata nil) (setq listdata (cons 'nil nil) ) )
       (setq listdata (list-editor listdata))
       (if (eq listdata nil) nil
         (progn
           (set-cursor 0 0)
           (set-text-color (rgb 255 255 255) 0)
           (write-string "Save ?  (y/n)");
           (let ((k (read-byte) ))
             (if (or (eq k (char-code #\y)) 
                     (eq k (char-code #\Y)) )
                (save-program listdata filename)
             )     
           )
         )
       )
    )
  )
  (cls)
)
