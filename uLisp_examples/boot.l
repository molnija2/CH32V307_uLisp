(progn

 (defun load (filename)
   (with-sd-card (s filename 0) (eval (read s))))
   
 (defun copy-file (source dest)
  (if (and (stringp source) (stringp dest))
   (with-sd-card (wrstr dest 2)
    (with-sd-card (rdstr source 0)
      (let ((c 0))
      (loop
         (setq c (read-byte rdstr))
          (if (equal c nil) (return))
          (write-byte c wrstr)
       ))) t)
   nil
  )
 )   

  (defun recieve-file (filename len)
    (with-sd-card (s filename 2) 
     (let ((i 0) (b 0))
       (do ((i 0 (1+ i))) ((= i len))
        (setq b (read-byte))
        (write-byte b s)
        (write-byte (char-code #\Space))
     ))))
     
 (defun print-file (filename)
    (with-sd-card (s filename 0) 
     (cls)
     (let ((i 0))
       (loop
        (setq i (read-byte s))
        (if (equal i nil) (return))
        (write-byte i)
     ))))
 (load-font 1 "/FONTS/DEN16H9.BIN")
 (set-font 1)
 (print "Привет !")
 (print "Hello !")
 (load "lisp-ext.l")
 (load "rvasm.l")
)
