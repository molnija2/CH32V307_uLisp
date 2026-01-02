
(progn
 (print "graphics-demo start")
 (loop
  (if
   (kbhit)
   (read-byte)
   (return)))
 (loop
  (if
   (touch-press)
   (return))
  (if
   (kbhit)
   (return))
  (fill-triangle
   (random 320)
   (random 480)
   (random 320)
   (random 480)
   (random 320)
   (random 480)
   (random 65535))))